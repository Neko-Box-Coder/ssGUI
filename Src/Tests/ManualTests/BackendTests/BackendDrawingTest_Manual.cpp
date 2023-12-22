#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

#include <thread>
#include "TestsResources.h"

ssGUI::Backend::BackendDrawingInterface* DrawingBackend = nullptr;
ssGUI::Backend::BackendMainWindowInterface* WindowBackend = nullptr;
ssGUI::Backend::BackendSystemInputInterface* InputBackend = nullptr;

ssGUI::Backend::BackendImageInterface* ImgBackend = nullptr;
ssGUI::Backend::BackendFontInterface* FontBackend = nullptr;

const std::string DrawingTestShapeName = "Backend Test Shape";

//========================================================================================
//Helper functions
//========================================================================================
ssGUI::DrawingEntity CreateShapes(  glm::vec2 pos, 
                                    glm::vec2 size, 
                                    glm::u8vec4 color, 
                                    bool isRainbow = false,
                                    bool isTriangle = false)
{
    ssGUI::DrawingEntity entity;
    
    //Base Size
    //Plain grey
    entity.Vertices.push_back(pos);
    entity.Vertices.push_back(pos + glm::vec2(size.x, 0));
    entity.Vertices.push_back(pos + size);
    
    if(!isTriangle)
        entity.Vertices.push_back(pos + glm::vec2(0, size.y));
    
    if(!isRainbow)
    {
        entity.Colors.push_back(color);
        entity.Colors.push_back(color);
        entity.Colors.push_back(color);
        
        if(!isTriangle)
            entity.Colors.push_back(color);
    }
    else
    {
        entity.Colors.push_back(glm::u8vec4(color.x, 0, 0, color.w));
        entity.Colors.push_back(glm::u8vec4(0, color.y, 0, color.w));
        entity.Colors.push_back(glm::u8vec4(0, 0, color.z, color.w));
        
        if(!isTriangle)
            entity.Colors.push_back(glm::u8vec4(0, 0, 0, color.w));
    }

    entity.EntityName = DrawingTestShapeName;

    return entity;
}

void SetUp()
{
    ssGUI::Backend::BackendFactory::Initialize();
    ImgBackend = ssGUI::Backend::BackendFactory::CreateBackendImageInterface();
    if(!ImgBackend->LoadImgFileFromMemory(ssGUI_Test_sd_edge, ssGUI_Test_sd_edge_size))
    {
        ssLOG_LINE("Failed to load image");
    }
    FontBackend = ssGUI::Backend::BackendFactory::CreateBackendFontInterface();
    if(!FontBackend->LoadFromMemory((void*)ssGUI_Test_arial, ssGUI_Test_arial_size))
    {
        ssLOG_LINE("Failed to load font");
    }
    WindowBackend = ssGUI::Backend::BackendFactory::CreateBackendMainWindowInterface();
    DrawingBackend = ssGUI::Backend::BackendFactory::CreateBackendDrawingInterface(WindowBackend);
    InputBackend = ssGUI::Backend::BackendFactory::CreateBackendInputInterface();
}

//========================================================================================
//Test functions
//========================================================================================

void CleanUp()
{
    ssGUI::Factory::Dispose(ImgBackend);
    ssGUI::Factory::Dispose(FontBackend);
    ssGUI::Factory::Dispose(DrawingBackend);
    ssGUI::Factory::Dispose(WindowBackend);
    ssGUI::Factory::Dispose(InputBackend);
    ssGUI::Backend::BackendFactory::Cleanup();
}

void Instructions()
{
    ssLOG_SIMPLE("Press 1 to test drawing shapes");
    ssLOG_SIMPLE("Press 2 to test drawing textures");
    ssLOG_SIMPLE("Press 3 to test drawing fonts");
    ssLOG_SIMPLE("Press 4 to test removing iamge cache");
    ssLOG_SIMPLE("Press 5 to test getting raw image cache handle pointer");
    ssLOG_SIMPLE(   "Please note that cache doesn't apply to SFML backend and "
                    "it will always return the handle regardless.");
}

void DrawColorShapesTest()
{
    if( !InputBackend->IsInputExistLastFrame(ssGUI::Enums::NumberKey::ONE) &&
        InputBackend->IsInputExistCurrentFrame(ssGUI::Enums::NumberKey::ONE))
    {
        DrawingBackend->Render(glm::u8vec3(255, 255, 255));
        
        std::vector<ssGUI::DrawingEntity> drawingEntities;
        
        drawingEntities.push_back(CreateShapes( glm::vec2(50, 50), 
                                                glm::vec2(100, 100), 
                                                glm::u8vec4(127, 127, 127, 255)));
        
        drawingEntities.push_back(CreateShapes( glm::vec2(200, 50), 
                                                glm::vec2(200, 100), 
                                                glm::u8vec4(255, 255, 255, 255), true));
        
        drawingEntities.push_back(CreateShapes( glm::vec2(500, 50), 
                                                glm::vec2(100, 200), 
                                                glm::u8vec4(127, 127, 127, 127)));
        
        drawingEntities.push_back(CreateShapes( glm::vec2(700, 50), 
                                                glm::vec2(100, 100), 
                                                glm::u8vec4(255, 255, 255, 127), true, true));
        
        DrawingBackend->CreateDrawingEntities(drawingEntities);
        DrawingBackend->Render(glm::u8vec3(255, 255, 255));
        
        ssLOG_SIMPLE("Shapes drawn");
        ssLOG_SIMPLE("You should see a grey box");
        ssLOG_SIMPLE("Then a rainbow box streched horizontally");
        ssLOG_SIMPLE("Then a semi-transparent grey box streched vertically");
        ssLOG_SIMPLE("Then a semi-transparent rainbow triangle");
    }
}

void DrawTextureTest()
{
    if( !InputBackend->IsInputExistLastFrame(ssGUI::Enums::NumberKey::TWO) &&
        InputBackend->IsInputExistCurrentFrame(ssGUI::Enums::NumberKey::TWO))
    {
        //DrawingBackend->Render(glm::u8vec3(255, 255, 255));
        
        glm::ivec2 imgSize = ImgBackend->GetSize();
        
        std::vector<ssGUI::DrawingEntity> drawingEntities;
        
        drawingEntities.push_back(CreateShapes( glm::vec2(50, 50), 
                                                imgSize, 
                                                glm::u8vec4(255, 255, 255, 255)));
        
        drawingEntities.push_back(CreateShapes( glm::vec2(450, 50), 
                                                imgSize, 
                                                glm::u8vec4(255, 255, 255, 255), true));
        
        drawingEntities.push_back(CreateShapes( glm::vec2(850, 50), 
                                                imgSize, 
                                                glm::u8vec4(255, 255, 255, 127)));
        
        drawingEntities.push_back(CreateShapes( glm::vec2(1050, 50), 
                                                imgSize, 
                                                glm::u8vec4(255, 255, 255, 127), true, true));
        
        std::vector<glm::vec2> texCoords = 
        {
            glm::vec2(0, 0), glm::vec2(imgSize.x, 0), glm::vec2(imgSize), glm::vec2(0, imgSize.y),
        };
        
        for(int i = 0; i < 4; i++)
            drawingEntities.at(i).BackendImage = ImgBackend;
        
        drawingEntities.at(0).TexCoords = texCoords;
        drawingEntities.at(1).TexCoords = texCoords;
        drawingEntities.at(2).TexCoords = texCoords;
        
        texCoords = 
        {
            glm::vec2(0, 0), glm::vec2(imgSize.x, 0), imgSize
        };
        drawingEntities.at(3).TexCoords = texCoords;
        
        texCoords = 
        {
            glm::vec2(0, 0), glm::vec2(imgSize.x, 0), glm::vec2(imgSize), glm::vec2(0, imgSize.y),
        };
        
        //Mipmap level 1
        drawingEntities.push_back(  CreateShapes(glm::vec2(50, 500), 
                                    imgSize / 2, 
                                    glm::u8vec4(255, 255, 255, 255)));
        
        drawingEntities.at(4).TexCoords = texCoords;
        drawingEntities.at(4).BackendImage = ImgBackend;
    
        //Mipmap level 2
        drawingEntities.push_back(  CreateShapes(glm::vec2(450, 500), 
                                    imgSize / 4, 
                                    glm::u8vec4(255, 255, 255, 255)));
        
        drawingEntities.at(5).TexCoords = texCoords;
        drawingEntities.at(5).BackendImage = ImgBackend;
        
        //Mipmap level 3
        drawingEntities.push_back(  CreateShapes(glm::vec2(850, 500), 
                                    imgSize / 8, 
                                    glm::u8vec4(255, 255, 255, 255)));
        
        drawingEntities.at(6).TexCoords = texCoords;
        drawingEntities.at(6).BackendImage = ImgBackend;
    
    
        DrawingBackend->CreateDrawingEntities(drawingEntities);
        DrawingBackend->Render(glm::u8vec3(255, 255, 255));
        
        ssLOG_SIMPLE("Texture drawn");
        ssLOG_SIMPLE("You should see a squared image");
        ssLOG_SIMPLE("Then a rainbow image streched horizontally");
        ssLOG_SIMPLE("Then a semi-transparent image streched vertically");
        ssLOG_SIMPLE("Then a semi-transparent rainbow image triangle");
        ssLOG_SIMPLE("Then 3 images in quartered size below, which will trigger mipmap");
        ssLOG_SIMPLE("If you have SSGUI_DEBUG_OPENGL_MIPMAP defined:");
        ssLOG_SIMPLE("  - The first mipmap should have red square at the top left corner");
        ssLOG_SIMPLE("  - The second mipmap should have green square at the top left corner");
        ssLOG_SIMPLE("  - The third mipmap should have blue square at the top left corner");
    }
}

void DrawFontTest()
{
    if( !InputBackend->IsInputExistLastFrame(ssGUI::Enums::NumberKey::THREE) &&
        InputBackend->IsInputExistCurrentFrame(ssGUI::Enums::NumberKey::THREE))
    {
        DrawingBackend->Render(glm::u8vec3(255, 255, 255));
        DrawingBackend->Render(glm::u8vec3(255, 255, 255));
        
        ssGUI::CharacterRenderInfo info = FontBackend->GetCharacterRenderInfo(L'A', 80);
        
        std::vector<ssGUI::DrawingEntity> drawingEntities;
        //std::vector<glm::vec2> vertices;
        
        //CreateShapes(drawingEntities, info.Size, glm::u8vec4(0, 0, 0, 255));
        
        drawingEntities.push_back(CreateShapes( glm::vec2(50, 50), 
                                                info.Size, 
                                                glm::u8vec4(255, 255, 255, 255)));
        
        drawingEntities.push_back(CreateShapes( glm::vec2(200, 50), 
                                                info.Size, 
                                                glm::u8vec4(255, 255, 255, 255), true));
        
        drawingEntities.push_back(CreateShapes( glm::vec2(500, 50), 
                                                info.Size, 
                                                glm::u8vec4(255, 255, 255, 127)));
        
        drawingEntities.push_back(CreateShapes( glm::vec2(700, 50), 
                                                info.Size, 
                                                glm::u8vec4(255, 255, 255, 127), true, true));
        
        //std::vector<glm::u8vec4> colors;
        //CreateColors(colors, glm::u8vec4(0, 0, 0, 255));
        
        std::vector<glm::vec2> texCoords = 
        {
            glm::vec2(0, 0), glm::vec2(info.Size.x, 0), info.Size, glm::vec2(0, info.Size.y),
        };
        
        drawingEntities[0].TexCoords = texCoords;
        drawingEntities[1].TexCoords = texCoords;
        drawingEntities[2].TexCoords = texCoords;
        
        texCoords = 
        {
            glm::vec2(0, 0), glm::vec2(info.Size.x, 0), info.Size
        };
        drawingEntities[3].TexCoords = texCoords;
        
        for(int i = 0; i < 4; i++)
        {
            drawingEntities[i].BackendFont = FontBackend;
            drawingEntities[i].Character = L'A';
            drawingEntities[i].CharacterSize = 80;
        }
        
        texCoords = 
        {
            glm::vec2(0, 0), glm::vec2(info.Size.x, 0), info.Size, glm::vec2(0, info.Size.y),
        };
        
        //Mipmap level 1
        drawingEntities.push_back(CreateShapes( glm::vec2(50, 500), 
                                                glm::ivec2(info.Size) / 2, 
                                                glm::u8vec4(255, 255, 255, 255)));
        
        drawingEntities.at(4).TexCoords = texCoords;
        drawingEntities.at(4).BackendFont = FontBackend;
        drawingEntities.at(4).Character = L'A';
        drawingEntities.at(4).CharacterSize = 80;

    
        //Mipmap level 2
        drawingEntities.push_back(CreateShapes( glm::vec2(450, 500), 
                                                glm::ivec2(info.Size) / 4, 
                                                glm::u8vec4(255, 255, 255, 255)));
        
        drawingEntities.at(5).TexCoords = texCoords;
        drawingEntities.at(5).BackendFont = FontBackend;
        drawingEntities.at(5).Character = L'A';
        drawingEntities.at(5).CharacterSize = 80;
        
        //Mipmap level 3
        drawingEntities.push_back(CreateShapes( glm::vec2(850, 500), 
                                                glm::ivec2(info.Size) / 8, 
                                                glm::u8vec4(255, 255, 255, 255)));
        
        drawingEntities.at(6).TexCoords = texCoords;
        drawingEntities.at(6).BackendFont = FontBackend;
        drawingEntities.at(6).Character = L'A';
        drawingEntities.at(6).CharacterSize = 80;
    
        DrawingBackend->CreateDrawingEntities(drawingEntities);
        DrawingBackend->Render(glm::u8vec3(0, 0, 0));
        
        ssLOG_SIMPLE("Character drawn");
        ssLOG_SIMPLE("You should see the letter A");
        ssLOG_SIMPLE("Then a rainbow letter A streched horizontally");
        ssLOG_SIMPLE("Then a semi-transparent letter A streched vertically");
        ssLOG_SIMPLE("Then a semi-transparent rainbow letter A but cut out from the bottom right");
        ssLOG_SIMPLE("Then 3 letters in quartered size below, which will trigger mipmap");
        ssLOG_SIMPLE("If you have SSGUI_DEBUG_OPENGL_MIPMAP defined:");
        ssLOG_SIMPLE("  - The first mipmap should have red square at the top left corner");
        ssLOG_SIMPLE("  - The second mipmap should have green square at the top left corner");
        ssLOG_SIMPLE("  - The third mipmap should have blue square at the top left corner");
    }
}

//TODO: Need to test AddImageCache

void RemoveImageCacheTest()
{
    if( !InputBackend->IsInputExistLastFrame(ssGUI::Enums::NumberKey::FOUR) &&
        InputBackend->IsInputExistCurrentFrame(ssGUI::Enums::NumberKey::FOUR))
    {
        DrawingBackend->RemoveImageCache(ImgBackend);
    
        ssLOG_SIMPLE("Removed Image cache");
        ssLOG_SIMPLE("If try to get image cache now, it should be a non-nullptr address");
    }
}

void GetRawImageCacheHandleTest()
{
    if( !InputBackend->IsInputExistLastFrame(ssGUI::Enums::NumberKey::FIVE) &&
        InputBackend->IsInputExistCurrentFrame(ssGUI::Enums::NumberKey::FIVE))
    {
        void* rawImageHandle = DrawingBackend->GetRawImageCacheHandle(ImgBackend);
    
        ssLOG_SIMPLE("rawImageCacheHandle: "<<rawImageHandle);
        ssLOG_SIMPLE("If the image has been rendered before, it should be a non-nullptr address");
        ssLOG_SIMPLE(   "Otherwise if the image hasn't been rendered or the cache is removed, "
                        "it should return nullptr");
    }
}

int main()
{
    SetUp();
    
    Instructions();
        
    //Setup window and run it
    WindowBackend->SetRenderSize(glm::ivec2(1280, 720));
    InputBackend->UpdateInput(&WindowBackend, 1);

    while(!WindowBackend->IsClosed())
    {
        InputBackend->UpdateInput(&WindowBackend, 1);
        if(!WindowBackend->IsClosed())
        {
            DrawColorShapesTest();
            DrawTextureTest();
            DrawFontTest();
            RemoveImageCacheTest();
            GetRawImageCacheHandleTest();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    CleanUp();   
}

