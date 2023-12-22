#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/Backend/Interfaces/BackendImageInterface.hpp"
#include "ssGUI/Factory.hpp"
#include "ssTest.hpp"
#include "TestsResources.h"
#include "ssGUI/EmbeddedResources.h"

#include <fstream>
#include <thread>

ssGUI::Backend::BackendImageInterface* TestImage = nullptr;

#ifdef SSGUI_IMAGE_BACKEND_MOCK
    #include "ssGUI/Backend/Mocks/BackendImageMock.hpp"
    
    #define SSGUI_MOCK_IMAGE (*static_cast<ssGUI::Backend::BackendImageMock*>(TestImage))
#endif

int main()
{
    std::fstream checkExistFs;
    checkExistFs.open("./sd.png");
    if(checkExistFs.fail())
    {
        std::ofstream ofs("./sd.png", std::ofstream::binary | std::ofstream::out);
        ofs.write((char*)ssGUI_Test_sd, ssGUI_Test_sd_size);
        ofs.close();
    }
    else
        checkExistFs.close();

    ssTEST_INIT();

    ssTEST_SET_UP
    {
        ssGUI::Backend::BackendFactory::Initialize();
        TestImage = ssGUI::Backend::BackendFactory::CreateBackendImageInterface();
    };

    ssTEST_CLEAN_UP
    {
        ssGUI::Factory::Dispose(TestImage);
        ssGUI::Backend::BackendFactory::Cleanup();
    };

    ssTEST_DISABLE_CLEANUP_BETWEEN_TESTS();

    ssTEST("GetRawHandleTest")
    {
        #ifdef SSGUI_IMAGE_BACKEND_STB_IMAGE
            ssTEST_OUTPUT_SKIP("No Image loaded");
            ssTEST_OUTPUT_SKIP("Image loaded");
        #else
            
            #ifdef SSGUI_IMAGE_BACKEND_MOCK
                bool loadFromPathSet = false;
                CO_OVERRIDE_RETURNS (SSGUI_MOCK_IMAGE, LoadFromPath(std::string))
                                    .Returns(true)
                                    .WhenCalledWith(std::string("./sd.png"))
                                    .WhenCalledExpectedly_Do
                                    (
                                        [&loadFromPathSet](...)
                                        {
                                            loadFromPathSet = true;
                                        }
                                    );
                                
                CO_OVERRIDE_RETURNS(SSGUI_MOCK_IMAGE, GetRawHandle())
                                    .ReturnsByAction<void*>
                                    (
                                        [](const std::vector<void*>& args, void* out)
                                        {
                                            (*static_cast<void**>(out)) = (void*)1;
                                        }
                                    )
                                    .If
                                    (
                                        [&loadFromPathSet](const std::vector<void*>& args)
                                        {
                                            return loadFromPathSet;
                                        }
                                    );
            #endif
        
            ssTEST_OUTPUT_ASSERT("No Image loaded", TestImage->GetRawHandle() == nullptr);
            TestImage->LoadFromPath("./sd.png");
            ssTEST_OUTPUT_ASSERT("Image loaded", TestImage->GetRawHandle() != nullptr);
        #endif
    };

    ssTEST("IsValidTest")
    {
        ssTEST_CALL_CLEAN_UP();
        ssTEST_CALL_SET_UP();

        #ifdef SSGUI_IMAGE_BACKEND_MOCK
            SSGUI_MOCK_IMAGE.ClearAllOverrideReturns();
            bool loadFromPathSet = false;
            CO_OVERRIDE_RETURNS(SSGUI_MOCK_IMAGE, LoadFromPath(std::string))
                            .Returns(true)
                            .WhenCalledWith(std::string("./sd.png"))
                            .WhenCalledExpectedly_Do
                            (
                                [&loadFromPathSet](...)
                                {
                                    loadFromPathSet = true;
                                }
                            );
                            
            CO_OVERRIDE_RETURNS(SSGUI_MOCK_IMAGE, IsValid())
                            .ReturnsByAction<bool>
                            (
                                [&loadFromPathSet](const std::vector<void*>& args, void* out)
                                {
                                    (*static_cast<bool*>(out)) = loadFromPathSet;
                                }
                            );
        #endif

        ssTEST_OUTPUT_ASSERT("No image loaded", !TestImage->IsValid());
        TestImage->LoadFromPath("./sd.png");
        ssTEST_OUTPUT_ASSERT("Image loaded", TestImage->IsValid());
    };

    ssTEST("LoadFromPathTest")
    {
        #ifdef SSGUI_IMAGE_BACKEND_MOCK
            SSGUI_MOCK_IMAGE.ClearAllOverrideReturns();
            bool loadFromPathSet = false;
            CO_OVERRIDE_RETURNS(SSGUI_MOCK_IMAGE, LoadFromPath(std::string))
                            .Returns(true)
                            .WhenCalledWith(std::string("./sd.png"))
                            .WhenCalledExpectedly_Do
                            (
                                [&loadFromPathSet](...)
                                {
                                    loadFromPathSet = true;
                                }
                            );
                            
            CO_OVERRIDE_RETURNS(SSGUI_MOCK_IMAGE, GetSize())
                                .ReturnsByAction<glm::ivec2>
                                (
                                    [](const std::vector<void*>& args, void* out)
                                    {
                                        (*static_cast<glm::ivec2*>(out)) = glm::ivec2(293, 293);
                                    }
                                )
                                .If
                                (
                                    [&loadFromPathSet](...)
                                    {
                                        return loadFromPathSet;
                                    }
                                );
        #endif
    
        ssTEST_OUTPUT_ASSERT("Loading", TestImage->LoadFromPath("./sd.png"));
        ssTEST_OUTPUT_ASSERT("Validation", TestImage->GetSize().x == 293 && TestImage->GetSize().y == 293);
    };

    ssTEST("LoadImgFileFromMemoryTest")
    {
        #ifdef SSGUI_IMAGE_BACKEND_MOCK
            SSGUI_MOCK_IMAGE.ClearAllOverrideReturns();
            bool loadImgFileFromMemorySet = false;
            CO_OVERRIDE_RETURNS(SSGUI_MOCK_IMAGE, LoadImgFileFromMemory(const void*, std::size_t))
                            .Returns(true)
                            .WhenCalledWith((const void*)::WindowIcon, WindowIcon_size)
                            .WhenCalledExpectedly_Do
                            (
                                [&loadImgFileFromMemorySet](...)
                                {
                                    loadImgFileFromMemorySet = true;
                                }
                            );
                            
            CO_OVERRIDE_RETURNS(SSGUI_MOCK_IMAGE, GetSize())
                                .ReturnsByAction<glm::ivec2>
                                (
                                    [](const std::vector<void*>& args, void* out)
                                    {
                                        (*static_cast<glm::ivec2*>(out)) = glm::ivec2(48, 48);
                                    }
                                )
                                .If
                                (
                                    [&loadImgFileFromMemorySet](...)
                                    {
                                        return loadImgFileFromMemorySet;
                                    }
                                );
        #endif
    
        ssTEST_OUTPUT_ASSERT("Loading", TestImage->LoadImgFileFromMemory(::WindowIcon, WindowIcon_size));
        ssTEST_OUTPUT_ASSERT("Validation", TestImage->GetSize().x == 48 && TestImage->GetSize().y == 48);
    };

    //This is retarded how MSVC not evaluating this as constant
    #ifndef _MSC_VER
        const int imgWidth = 64;
        const int imgHeight = 64;
    #else
        #define imgWidth 64
        #define imgHeight 64
    #endif
    const int columnWidth = imgWidth / 4;

    ssTEST("LoadRawFromMemoryTest")
    {
        uint8_t dummyImg[imgWidth * imgHeight * 4] = {0};
        
        for(int y = 0; y < imgHeight; y++)
        {
            for(int x = 0; x < imgWidth; x++)
            {
                dummyImg[(y * imgWidth + x) * 4 + 0] = x < columnWidth ? 255 : 0;
                dummyImg[(y * imgWidth + x) * 4 + 1] =  x >= columnWidth && x < columnWidth * 2 ? 
                                                        255 : 
                                                        0;
                
                dummyImg[(y * imgWidth + x) * 4 + 2] =  x >= columnWidth * 2 && x < columnWidth * 3 ? 
                                                        255 : 
                                                        0;
                
                dummyImg[(y * imgWidth + x) * 4 + 3] =  x >= columnWidth * 3 ? 
                                                        127 : 
                                                        255;
            }
        }
        
        #ifdef SSGUI_IMAGE_BACKEND_MOCK
            SSGUI_MOCK_IMAGE.ClearAllOverrideReturns();
            bool loadRawFromMemorySet = false;
            CO_OVERRIDE_RETURNS(SSGUI_MOCK_IMAGE, LoadRawFromMemory(const void*, 
                                                                    ssGUI::ImageFormat, 
                                                                    glm::ivec2))
                            .Returns(true)
                            .WhenCalledWith(    (const void*)dummyImg, 
                                                ssGUI::ImageFormat(), 
                                                glm::ivec2(64, 64))
                            .WhenCalledExpectedly_Do
                            (
                                [&loadRawFromMemorySet](...)
                                {
                                    loadRawFromMemorySet = true;
                                }
                            );
                            
            CO_OVERRIDE_RETURNS(SSGUI_MOCK_IMAGE, IsValid())
                            .ReturnsByAction<bool>
                            (
                                [&loadRawFromMemorySet](const std::vector<void*>& args, void* out)
                                {
                                    (*static_cast<bool*>(out)) = loadRawFromMemorySet;
                                }
                            );
        #endif
        
        TestImage->LoadRawFromMemory(dummyImg, ssGUI::ImageFormat(), glm::ivec2(64, 64));
        
        ssTEST_OUTPUT_ASSERT(TestImage->IsValid());
    };

    ssTEST("GetSizeTest")
    {
        #ifdef SSGUI_IMAGE_BACKEND_MOCK
            SSGUI_MOCK_IMAGE.ClearAllOverrideReturns();
            CO_OVERRIDE_RETURNS(SSGUI_MOCK_IMAGE, GetSize())
                            .Returns(glm::ivec2(imgWidth, imgWidth));
        #endif
    
        ssTEST_OUTPUT_ASSERT(TestImage->GetSize().x == imgWidth && TestImage->GetSize().y == imgHeight);
    };

    ssTEST("GetPixelPtrTest")
    {
        ssGUI::ImageFormat format;
        
        #ifdef SSGUI_IMAGE_BACKEND_MOCK
            SSGUI_MOCK_IMAGE.ClearAllOverrideReturns();
            
            uint8_t retImg[imgWidth * imgHeight * 4] = {0};
        
            for(int y = 0; y < imgHeight; y++)
            {
                for(int x = 0; x < imgWidth; x++)
                {
                    retImg[(y * imgWidth + x) * 4 + 0] = x < columnWidth ? 255 : 0;
                    retImg[(y * imgWidth + x) * 4 + 1] =    x >= columnWidth && x < columnWidth * 2 ? 
                                                            255 : 
                                                            0;
                    
                    retImg[(y * imgWidth + x) * 4 + 2] =    (x >= columnWidth * 2 && 
                                                            x < columnWidth * 3) ? 
                                                            255 : 
                                                            0;
                    
                    retImg[(y * imgWidth + x) * 4 + 3] = x >= columnWidth * 3 ? 127 : 255;
                }
            }
            
            CO_OVERRIDE_RETURNS(SSGUI_MOCK_IMAGE, GetPixelPtr(ssGUI::ImageFormat&))
                            .Returns((void*)retImg);
            
            ssGUI::ImageFormat setFormat;
            setFormat.BitDepthPerChannel = 8;
            setFormat.ImgType = ssGUI::Enums::ImageType::RGB;
            setFormat.HasAlpha = true;
            setFormat.IndexR = 0;
            setFormat.IndexG = 1;
            setFormat.IndexB = 2;
            setFormat.IndexA = 3;
            
            CO_OVERRIDE_ARGS(SSGUI_MOCK_IMAGE, GetPixelPtr(ssGUI::ImageFormat&))
                            .SetArgs(setFormat);
        #endif
        
        uint8_t* pixelPtr = static_cast<uint8_t*>(TestImage->GetPixelPtr(format));
        
        ssTEST_OUTPUT_ASSERT("Format Valid",
                                format.BitDepthPerChannel == 8 &&
                                format.ImgType == ssGUI::Enums::ImageType::RGB &&
                                format.HasAlpha == true &&
                                format.IndexR == 0 &&
                                format.IndexG == 1 &&
                                format.IndexB == 2 &&
                                format.IndexA == 3);

        ssTEST_OUTPUT_ASSERT("Pixel Valid",
                                pixelPtr != nullptr &&
                                //Red
                                pixelPtr[0 * 4 + 0] == 255 &&
                                pixelPtr[0 * 4 + 1] == 0 &&
                                pixelPtr[0 * 4 + 2] == 0 &&
                                pixelPtr[0 * 4 + 3] == 255 &&
                                
                                //Green
                                pixelPtr[columnWidth * 4 + 0] == 0 &&
                                pixelPtr[columnWidth * 4 + 1] == 255 &&
                                pixelPtr[columnWidth * 4 + 2] == 0 &&
                                pixelPtr[columnWidth * 4 + 3] == 255 &&
                                
                                //Blue
                                pixelPtr[columnWidth*2 * 4 + 0] == 0 &&
                                pixelPtr[columnWidth*2 * 4 + 1] == 0 &&
                                pixelPtr[columnWidth*2 * 4 + 2] == 255 &&
                                pixelPtr[columnWidth*2 * 4 + 3] == 255 &&
                                
                                //Green
                                pixelPtr[columnWidth*3 * 4 + 0] == 0 &&
                                pixelPtr[columnWidth*3 * 4 + 1] == 0 &&
                                pixelPtr[columnWidth*3 * 4 + 2] == 0 &&
                                pixelPtr[columnWidth*3 * 4 + 3] == 127);
    };

    ssTEST_SKIP("Internal_AddBackendDrawingRecordTest")
    {
    };

    ssTEST_SKIP("Internal_RemoveBackendDrawingRecord")
    {
    };

    ssTEST("CloneTest")
    {
        #ifdef SSGUI_IMAGE_BACKEND_MOCK
            SSGUI_MOCK_IMAGE.ClearAllOverrideReturns();
            SSGUI_MOCK_IMAGE.ClearAllOverrideArgs();
            CO_OVERRIDE_RETURNS (SSGUI_MOCK_IMAGE, GetSize())
                                .Returns(glm::ivec2(imgWidth, imgWidth));
        #endif
    
        ssGUI::Backend::BackendImageInterface* clonedImg = TestImage->Clone();
        
        ssTEST_OUTPUT_ASSERT(   clonedImg != nullptr &&
                                clonedImg->GetSize().x == TestImage->GetSize().x &&
                                clonedImg->GetSize().y == TestImage->GetSize().y);

        ssGUI::Factory::Dispose(clonedImg);
    };

    ssTEST_END();
}

