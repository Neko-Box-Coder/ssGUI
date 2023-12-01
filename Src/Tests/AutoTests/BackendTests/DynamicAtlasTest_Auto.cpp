#include "ssGUI/Backend/DynamicImageAtlas.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

#include "ssGUI/HelperClasses/OutputStreamUtil.hpp"

#include "ssTest.hpp"

using ssGUI::Backend::DynamicImageAtlas;

DynamicImageAtlas::ImageAtlasImageInfo CreateImageInfo(glm::ivec2 size, bool hasMipmap)
{
    DynamicImageAtlas::ImageAtlasImageInfo imgInfo;
    imgInfo.ImageSizeInPixel = size;
    imgInfo.HasMipmap = hasMipmap;
    return imgInfo;
}

int main()
{

    DynamicImageAtlas* dynamicAtlas = nullptr;
    int AtlasRequested = 0;
    auto onRequestAtlasCallback = [&AtlasRequested](){ AtlasRequested++; return true; };
    
    ssTEST_INIT();
    
    ssTEST_SET_UP
    {
    
        dynamicAtlas = new DynamicImageAtlas(   glm::ivec2(500, 500), 
                                                glm::ivec2(50, 50),
                                                onRequestAtlasCallback);
    };
    
    ssTEST_CLEAN_UP
    {
        AtlasRequested = 0;
        delete dynamicAtlas;
    };
    
    ssTEST("RequestImage Test")
    {
        DynamicImageAtlas::ImageAtlasImageInfo imgInfo = CreateImageInfo(glm::ivec2(45, 25), true);
    
        int imgId = -1;
        bool result = dynamicAtlas->RequestImage(imgInfo, imgId);
        
        ssTEST_OUTPUT_ASSERT(result && imgId != -1);
    };
    
    ssTEST("GetImageInfo Test")
    {
        DynamicImageAtlas::ImageAtlasImageInfo imgInfo = CreateImageInfo(glm::ivec2(45, 25), true);

        int imgId = -1;
        ssTEST_OUTPUT_ASSERT("RequestImage", dynamicAtlas->RequestImage(imgInfo, imgId));
        
        DynamicImageAtlas::ImageAtlasImageInfo returnInfo;
        ssTEST_OUTPUT_ASSERT("GetImageInfo", dynamicAtlas->GetImageInfo(imgId, returnInfo));
        
        ssTEST_OUTPUT_ASSERT("Valid returns",   returnInfo.LocationInPixel == glm::ivec3() &&
                                                returnInfo.ImageSizeInPixel == imgInfo.ImageSizeInPixel &&
                                                returnInfo.HasMipmap == imgInfo.HasMipmap);
    };
    
    ssTEST("RemoveImage Test")
    {
        DynamicImageAtlas::ImageAtlasImageInfo imgInfo = CreateImageInfo(glm::ivec2(45, 25), true);

        int imgId = -1;
        ssTEST_OUTPUT_ASSERT("RequestImage", dynamicAtlas->RequestImage(imgInfo, imgId));
    
        DynamicImageAtlas::ImageAtlasImageInfo returnInfo;
        ssTEST_OUTPUT_ASSERT("Pre remove", dynamicAtlas->GetImageInfo(imgId, returnInfo));
        
        dynamicAtlas->RemoveImage(imgId);
        ssTEST_OUTPUT_ASSERT("Post remove", !dynamicAtlas->GetImageInfo(imgId, returnInfo));
    };
    
    ssTEST("RequestNewAtlasCallback Test")
    {
        DynamicImageAtlas::ImageAtlasImageInfo imgInfo = CreateImageInfo(   glm::ivec2(500, 300),  
                                                                            false);
    
        int imgId = -1;
        ssTEST_OUTPUT_ASSERT("RequestImage", dynamicAtlas->RequestImage(imgInfo, imgId));
        
        ssTEST_OUTPUT_ASSERT("New Atlas allocated", AtlasRequested == 1);
        
        ssTEST_OUTPUT_ASSERT("RequestImage 2", dynamicAtlas->RequestImage(imgInfo, imgId));
        
        ssTEST_OUTPUT_ASSERT("New Atlas allocated again", AtlasRequested == 2);
    
        dynamicAtlas->AddOnRequestNewAtlasCallback([](){ return false; });
        
        ssTEST_OUTPUT_ASSERT("RequestImage 3", !dynamicAtlas->RequestImage(imgInfo, imgId));
    };
    
    ssTEST("Allocated Image Position Test")
    {
        int imgId = -1;
        DynamicImageAtlas::ImageAtlasImageInfo imgInfo = CreateImageInfo(   glm::ivec2(350, 200), 
                                                                            false);
        
        ssTEST_OUTPUT_ASSERT("RequestImage", dynamicAtlas->RequestImage(imgInfo, imgId));

        int imgId2 = -1;
        DynamicImageAtlas::ImageAtlasImageInfo imgInfo2 = CreateImageInfo(  glm::ivec2(450, 200), 
                                                                            false);
        
        ssTEST_OUTPUT_ASSERT("RequestImage 2", dynamicAtlas->RequestImage(imgInfo2, imgId2));
        
        int imgId3 = -1;
        DynamicImageAtlas::ImageAtlasImageInfo imgInfo3 = CreateImageInfo(  glm::ivec2(70, 200), 
                                                                            false);
        
        ssTEST_OUTPUT_ASSERT("RequestImage 3", dynamicAtlas->RequestImage(imgInfo3, imgId3));

        //┌────────────┬────┐
        //│            │    │
        //│      1     │ 3  │
        //│            │    │
        //├────────────┴─┬──┤
        //│              │  │
        //│      2       │  │
        //│              │  │
        //├──────────────┘  │
        //└─────────────────┘

        DynamicImageAtlas::ImageAtlasImageInfo getImgInfo;
        DynamicImageAtlas::ImageAtlasImageInfo getImgInfo2;
        DynamicImageAtlas::ImageAtlasImageInfo getImgInfo3;
        
        ssTEST_OUTPUT_ASSERT("GetImageInfo", dynamicAtlas->GetImageInfo(imgId, getImgInfo));
        ssTEST_OUTPUT_ASSERT("GetImageInfo 2", dynamicAtlas->GetImageInfo(imgId2, getImgInfo2));
        ssTEST_OUTPUT_ASSERT("GetImageInfo 3", dynamicAtlas->GetImageInfo(imgId3, getImgInfo3));
        
        //ssLOG_LINE("getImgInfo.LocationInPixel: "<<getImgInfo.LocationInPixel);
        //ssLOG_LINE("getImgInfo2.LocationInPixel: "<<getImgInfo2.LocationInPixel);
        //ssLOG_LINE("getImgInfo3.LocationInPixel: "<<getImgInfo3.LocationInPixel);
        
        ssTEST_OUTPUT_ASSERT("Validate Pos",    getImgInfo.LocationInPixel == glm::ivec3() &&
                                                getImgInfo2.LocationInPixel == glm::ivec3(0, 200, 0) &&
                                                getImgInfo3.LocationInPixel == glm::ivec3(350, 0, 0));

        //New atlas 
        int imgId4 = -1;
        ssTEST_OUTPUT_ASSERT("RequestImage 4", dynamicAtlas->RequestImage(imgInfo3, imgId4));
        
        DynamicImageAtlas::ImageAtlasImageInfo getImgInfo4;
        ssTEST_OUTPUT_ASSERT("GetImageInfo 4", dynamicAtlas->GetImageInfo(imgId4, getImgInfo4));
        
        ssTEST_OUTPUT_ASSERT(   "Validate Pos for new atlas", 
                                getImgInfo4.LocationInPixel == glm::ivec3(0, 0, 1) && 
                                AtlasRequested == 2);
    };
    
    ssTEST("RemoveImage adds free cells back Test")
    {
        int imgId = -1;
        DynamicImageAtlas::ImageAtlasImageInfo imgInfo = CreateImageInfo(   glm::ivec2(350, 200), 
                                                                            false);
        
        ssTEST_OUTPUT_ASSERT("RequestImage", dynamicAtlas->RequestImage(imgInfo, imgId));

        int imgId2 = -1;
        DynamicImageAtlas::ImageAtlasImageInfo imgInfo2 = CreateImageInfo(  glm::ivec2(450, 200), 
                                                                            false);
        
        ssTEST_OUTPUT_ASSERT("RequestImage 2", dynamicAtlas->RequestImage(imgInfo2, imgId2));
        
        int imgId3 = -1;
        DynamicImageAtlas::ImageAtlasImageInfo imgInfo3 = CreateImageInfo(  glm::ivec2(70, 200), 
                                                                            false);
        
        ssTEST_OUTPUT_ASSERT("RequestImage 3", dynamicAtlas->RequestImage(imgInfo3, imgId3));

        //┌────────────┬────┐
        //│            │    │
        //│      1     │ 3  │
        //│            │    │
        //├────────────┴─┬──┤
        //│              │  │
        //│      2       │  │
        //│              │  │
        //├──────────────┘  │
        //└─────────────────┘

        dynamicAtlas->RemoveImage(imgId3);
        
        int imgId4 = -1;
        ssTEST_OUTPUT_ASSERT("RequestImage 4", dynamicAtlas->RequestImage(imgInfo3, imgId4));
        
        DynamicImageAtlas::ImageAtlasImageInfo getImgInfo4;
        ssTEST_OUTPUT_ASSERT("GetImageInfo 4", dynamicAtlas->GetImageInfo(imgId4, getImgInfo4));
        
        
        ssTEST_OUTPUT_ASSERT("Validation",  imgId4 != imgId3 &&
                                            AtlasRequested == 1 &&
                                            getImgInfo4.LocationInPixel == glm::ivec3(350, 0, 0));
    };
 
    ssTEST("Mipmap allocation Test")
    {
        int imgId = -1;
        DynamicImageAtlas::ImageAtlasImageInfo imgInfo = CreateImageInfo(   glm::ivec2(250, 200),  
                                                                            true);
        
        ssTEST_OUTPUT_ASSERT("RequestImage", dynamicAtlas->RequestImage(imgInfo, imgId));

        int imgId2 = -1;
        DynamicImageAtlas::ImageAtlasImageInfo imgInfo2 = CreateImageInfo(  glm::ivec2(300, 200),  
                                                                            true);
        
        ssTEST_OUTPUT_ASSERT("RequestImage 2", dynamicAtlas->RequestImage(imgInfo2, imgId2));
        
        int imgId3 = -1;
        DynamicImageAtlas::ImageAtlasImageInfo imgInfo3 = CreateImageInfo(  glm::ivec2(50, 200),  
                                                                            true);
        
        ssTEST_OUTPUT_ASSERT("RequestImage 3", dynamicAtlas->RequestImage(imgInfo3, imgId3));
        
        // mm = mipmaps
        //┌────────────┬────┐
        //│        │ m │  │ │
        //│      1 │ m │ 3│ │
        //│        │   │  │ │
        //├────────────┴─┬──┤
        //│         │ m  │  │
        //│      2  │ m  │  │
        //│         │    │  │
        //├──────────────┘  │
        //└─────────────────┘
        
        DynamicImageAtlas::ImageAtlasImageInfo getImgInfo;
        DynamicImageAtlas::ImageAtlasImageInfo getImgInfo2;
        DynamicImageAtlas::ImageAtlasImageInfo getImgInfo3;
        
        ssTEST_OUTPUT_ASSERT("GetImageInfo", dynamicAtlas->GetImageInfo(imgId, getImgInfo));
        ssTEST_OUTPUT_ASSERT("GetImageInfo 2", dynamicAtlas->GetImageInfo(imgId2, getImgInfo2));
        ssTEST_OUTPUT_ASSERT("GetImageInfo 3", dynamicAtlas->GetImageInfo(imgId3, getImgInfo3));
        
        //ssLOG_LINE("getImgInfo.LocationInPixel: "<<getImgInfo.LocationInPixel);
        //ssLOG_LINE("getImgInfo2.LocationInPixel: "<<getImgInfo2.LocationInPixel);
        //ssLOG_LINE("getImgInfo3.LocationInPixel: "<<getImgInfo3.LocationInPixel);
        
        ssTEST_OUTPUT_ASSERT("Validate Pos",    getImgInfo.LocationInPixel == glm::ivec3() &&
                                                getImgInfo2.LocationInPixel == glm::ivec3(0, 200, 0) &&
                                                getImgInfo3.LocationInPixel == glm::ivec3(400, 0, 0));
    };
 
    
    ssTEST_END();
}