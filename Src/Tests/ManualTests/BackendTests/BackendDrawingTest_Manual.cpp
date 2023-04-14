#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssLogger/ssLog.hpp"

#include <thread>

#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
    std::string ResourcesFolderPath = "..\\Resources\\";
#else
    std::string ResourcesFolderPath = "./Resources/";
#endif

ssGUI::Backend::BackendDrawingInterface* DrawingBackend = nullptr;
ssGUI::Backend::BackendMainWindowInterface* WindowBackend = nullptr;
ssGUI::Backend::BackendSystemInputInterface* InputBackend = nullptr;

ssGUI::Backend::BackendImageInterface* ImgBackend = nullptr;
ssGUI::Backend::BackendFontInterface* FontBackend = nullptr;

//========================================================================================
//Helper functions
//========================================================================================
void CreateShapes(std::vector<glm::vec2>& vertices, glm::vec2 baseSize)
{
    glm::vec2 startPos = glm::vec2(50, 50);

    glm::vec2 offset = glm::vec2(300, 0);

    vertices =
    {
        //Base Size
        startPos + offset * 0.f, 
        startPos + offset * 0.f + glm::vec2(baseSize.x, 0), 
        startPos + offset * 0.f + baseSize, 
        startPos + offset * 0.f + glm::vec2(0, baseSize.y),
        
        //Stretch X
        startPos + offset * 1.f, 
        startPos + offset * 1.f + glm::vec2(baseSize.x * 2, 0), 
        startPos + offset * 1.f + baseSize + glm::vec2(baseSize.x, 0), 
        startPos + offset * 1.f + glm::vec2(0, baseSize.y),

        //Stretch Y
        startPos + offset * 2.f, 
        startPos + offset * 2.f + glm::vec2(baseSize.x, 0), 
        startPos + offset * 2.f + baseSize + glm::vec2(0, baseSize.y), 
        startPos + offset * 2.f + glm::vec2(0, baseSize.y * 2),

        //Non-quad
        startPos + offset * 3.f, 
        startPos + offset * 3.f + glm::vec2(baseSize.x, 0), 
        startPos + offset * 3.f + baseSize,   
    };
}

void CreateColors(std::vector<glm::u8vec4>& colors, glm::u8vec4 baseColor)
{
    colors = 
    {
        //Plain grey
        baseColor,
        baseColor,
        baseColor,
        baseColor,
    
        //Sampling color
        glm::u8vec4(255, 0, 0, 255),
        glm::u8vec4(0, 255, 0, 255),
        glm::u8vec4(0, 0, 255, 255),
        baseColor,
        
        //Alpha
        glm::u8vec4(127, 127, 127, 127),
        glm::u8vec4(127, 127, 127, 127),
        glm::u8vec4(127, 127, 127, 127),
        glm::u8vec4(127, 127, 127, 127),

        //Sampling with alpha
        glm::u8vec4(255, 0, 0, 127),
        glm::u8vec4(0, 255, 0, 127),
        glm::u8vec4(0, 0, 255, 127),
    };
}

void CreateCounts(std::vector<int>& counts)
{
    counts = 
    {
        4, 4, 4, 3
    };
}

void SetUp()
{
    ImgBackend = ssGUI::Backend::BackendFactory::CreateBackendImageInterface();
    if(!ImgBackend->LoadFromPath(ResourcesFolderPath+"sd.png"))
    {
        ssLOG_LINE("Failed to load image");
    }
    FontBackend = ssGUI::Backend::BackendFactory::CreateBackendFontInterface();
    if(!FontBackend->LoadFromPath(ResourcesFolderPath+"arial.ttf"))
    {
        ssLOG_LINE("Failed to load font");
    }
    DrawingBackend = ssGUI::Backend::BackendFactory::CreateBackendDrawingInterface();
    WindowBackend = ssGUI::Backend::BackendFactory::CreateBackendMainWindowInterface();
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
}

void Instructions()
{
    ssLOG_SIMPLE("Press 1 to test drawing shapes");
    ssLOG_SIMPLE("Press 2 to test drawing textures");
    ssLOG_SIMPLE("Press 3 to test drawing fonts");
    ssLOG_SIMPLE("Press 4 to test removing iamge cache");
    ssLOG_SIMPLE("Press 5 to test getting raw image cache handle pointer");
    ssLOG_SIMPLE("Please note that cache doesn't apply to SFML backend and it will always return the handle regardless.");
}

void DrawColorShapesTest()
{
    if( !InputBackend->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::ONE) &&
        InputBackend->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::ONE))
    {
        DrawingBackend->Render(glm::u8vec3(255, 255, 255));
        DrawingBackend->Render(glm::u8vec3(255, 255, 255));
        
        std::vector<glm::vec2> vertices;
        CreateShapes(vertices, glm::vec2(50, 50));
        
        std::vector<glm::u8vec4> colors;
        CreateColors(colors, glm::u8vec4(127, 127, 127, 255));
        
        std::vector<glm::vec2> texCoords = std::vector<glm::vec2>(15, glm::vec2());
        
        std::vector<int> counts;
        CreateCounts(counts);
        
        std::vector<ssGUI::DrawingProperty> properties = 
        {
            ssGUI::DrawingProperty(), 
            ssGUI::DrawingProperty(), 
            ssGUI::DrawingProperty(),
            ssGUI::DrawingProperty()
        };
    
        DrawingBackend->DrawEntities(vertices, texCoords, colors, counts, properties);
        DrawingBackend->Render(glm::u8vec3(255, 255, 255));
        
        ssLOG_SIMPLE("Shapes drawn");
        ssLOG_SIMPLE("You should see a grey box");
        ssLOG_SIMPLE("Then a rainbow box streched horizontally");
        ssLOG_SIMPLE("Then a semi-transparent box streched vertically");
        ssLOG_SIMPLE("Then a semi-transparent rainbow triangle");
    }
}

void DrawTextureTest()
{
    if( !InputBackend->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::TWO) &&
        InputBackend->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::TWO))
    {
        DrawingBackend->Render(glm::u8vec3(255, 255, 255));
        DrawingBackend->Render(glm::u8vec3(255, 255, 255));
        
        glm::ivec2 imgSize = ImgBackend->GetSize();
        
        std::vector<glm::vec2> vertices;
        CreateShapes(vertices, imgSize);
        
        std::vector<glm::u8vec4> colors;
        CreateColors(colors, glm::u8vec4(255, 255, 255, 255));
        
        std::vector<glm::vec2> texCoords = 
        {
            glm::vec2(0, 0), glm::vec2(imgSize.x, 0), imgSize, glm::vec2(0, imgSize.y),
            glm::vec2(0, 0), glm::vec2(imgSize.x, 0), imgSize, glm::vec2(0, imgSize.y),
            glm::vec2(0, 0), glm::vec2(imgSize.x, 0), imgSize, glm::vec2(0, imgSize.y),
            glm::vec2(0, 0), glm::vec2(imgSize.x, 0), imgSize
        };
        
        std::vector<int> counts;
        CreateCounts(counts);

        ssGUI::DrawingProperty prop;
        prop.imageP = ImgBackend;
        
        std::vector<ssGUI::DrawingProperty> properties = 
        {
            prop,
            prop,
            prop,
            prop
        };
    
        DrawingBackend->DrawEntities(vertices, texCoords, colors, counts, properties);
        DrawingBackend->Render(glm::u8vec3(255, 255, 255));
        
        ssLOG_SIMPLE("Texture drawn");
        ssLOG_SIMPLE("You should see a squared image");
        ssLOG_SIMPLE("Then a rainbow image streched horizontally");
        ssLOG_SIMPLE("Then a semi-transparent image streched vertically");
        ssLOG_SIMPLE("Then a semi-transparent rainbow image triangle");
    }
}

void DrawFontTest()
{
    if( !InputBackend->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::THREE) &&
        InputBackend->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::THREE))
    {
        DrawingBackend->Render(glm::u8vec3(255, 255, 255));
        DrawingBackend->Render(glm::u8vec3(255, 255, 255));
        
        ssGUI::CharacterRenderInfo info = FontBackend->GetCharacterRenderInfo(L'A', 40);
        
        std::vector<glm::vec2> vertices;
        CreateShapes(vertices, info.Size);
        
        std::vector<glm::u8vec4> colors;
        CreateColors(colors, glm::u8vec4(0, 0, 0, 255));
        
        std::vector<glm::vec2> texCoords = 
        {
            glm::vec2(0, 0), glm::vec2(info.Size.x, 0), info.Size, glm::vec2(0, info.Size.y),
            glm::vec2(0, 0), glm::vec2(info.Size.x, 0), info.Size, glm::vec2(0, info.Size.y),
            glm::vec2(0, 0), glm::vec2(info.Size.x, 0), info.Size, glm::vec2(0, info.Size.y),
            glm::vec2(0, 0), glm::vec2(info.Size.x, 0), info.Size
        };
        
        std::vector<int> counts;
        CreateCounts(counts);

        ssGUI::DrawingProperty prop;
        prop.fontP = FontBackend;
        prop.character = L'A';
        prop.characterSize = 40;
        
        std::vector<ssGUI::DrawingProperty> properties = 
        {
            prop,
            prop,
            prop,
            prop
        };
    
        DrawingBackend->DrawEntities(vertices, texCoords, colors, counts, properties);
        DrawingBackend->Render(glm::u8vec3(255, 255, 255));
        
        ssLOG_SIMPLE("Character drawn");
        ssLOG_SIMPLE("You should see the letter A");
        ssLOG_SIMPLE("Then a rainbow letter A streched horizontally");
        ssLOG_SIMPLE("Then a semi-transparent letter A streched vertically");
        ssLOG_SIMPLE("Then a semi-transparent rainbow letter A but cut out from the bottom right");
    }
}

//NOTE: Skipping AddImageCache for now, it is used internally anyway so it should be working.

void RemoveImageCacheTest()
{
    if( !InputBackend->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::FOUR) &&
        InputBackend->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::FOUR))
    {
        DrawingBackend->RemoveImageCache(ImgBackend);
    
        ssLOG_SIMPLE("Removed Image cache");
        ssLOG_SIMPLE("If try to get image cache now, it should be a non-nullptr address");
    }
}

void GetRawImageCacheHandleTest()
{
    if( !InputBackend->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::FIVE) &&
        InputBackend->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::FIVE))
    {
        void* rawImageHandle = DrawingBackend->GetRawImageCacheHandle(ImgBackend);
    
        ssLOG_SIMPLE("rawImageCacheHandle: "<<rawImageHandle);
        ssLOG_SIMPLE("If the image has been rendered before, it should be a non-nullptr address");
        ssLOG_SIMPLE("Otherwise if the image hasn't been rendered or the cache is removed, it should return nullptr");
    }
}

int main()
{
    SetUp();
    
    Instructions();
        
    //Setup window and run it
    WindowBackend->SetRenderSize(glm::ivec2(1280, 720));
    InputBackend->UpdateInput();

    while(!WindowBackend->IsClosed())
    {
        InputBackend->UpdateInput();
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

