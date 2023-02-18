#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssLogger/ssLog.hpp"

#include <thread>

#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
    std::string ResourcesFolderPath = "..\\Resources\\";
#else
    std::string ResourcesFolderPath = "./Resources/";
#endif

ssGUI::Backend::BackendDrawingInterface* drawing = nullptr;
ssGUI::Backend::BackendMainWindowInterface* window = nullptr;
ssGUI::Backend::BackendSystemInputInterface* inputs = nullptr;

ssGUI::Backend::BackendImageInterface* img;
ssGUI::Backend::BackendFontInterface* font;

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
    img = ssGUI::Backend::BackendFactory::CreateBackendImageInterface();
    if(!img->LoadFromPath(ResourcesFolderPath+"sd.png"))
    {
        ssLOG_LINE("Failed to load image");
    }
    font = ssGUI::Backend::BackendFactory::CreateBackendFontInterface();
    if(!font->LoadFromPath(ResourcesFolderPath+"arial.ttf"))
    {
        ssLOG_LINE("Failed to load font");
    }
    drawing = ssGUI::Backend::BackendFactory::CreateBackendDrawingInterface();
    window = ssGUI::Backend::BackendFactory::CreateBackendMainWindowInterface();
    inputs = ssGUI::Backend::BackendFactory::CreateBackendInputInterface();
}

//========================================================================================
//Test functions
//========================================================================================

void CleanUp()
{
    ssGUI::Factory::Dispose(img);
    ssGUI::Factory::Dispose(font);
    ssGUI::Factory::Dispose(drawing);
    ssGUI::Factory::Dispose(window);
    ssGUI::Factory::Dispose(inputs);
}

void Instructions()
{
    ssLOG_SIMPLE("Press 1 to test drawing shapes");
    ssLOG_SIMPLE("Press 2 to test drawing textures");
    ssLOG_SIMPLE("Press 3 to test drawing fonts");
}

void DrawColorShapesTest()
{
    if( !inputs->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::ONE) &&
        inputs->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::ONE))
    {
        drawing->Render(glm::u8vec3(255, 255, 255));
        drawing->Render(glm::u8vec3(255, 255, 255));
        
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
    
        drawing->DrawEntities(vertices, texCoords, colors, counts, properties);
        drawing->Render(glm::u8vec3(255, 255, 255));
        
        ssLOG_SIMPLE("Shapes drawn");
        ssLOG_SIMPLE("You should see a grey box");
        ssLOG_SIMPLE("Then a rainbow box streched horizontally");
        ssLOG_SIMPLE("Then a semi-transparent box streched vertically");
        ssLOG_SIMPLE("Then a semi-transparent rainbow triangle");
    }
}

void DrawTextureTest()
{
    if( !inputs->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::TWO) &&
        inputs->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::TWO))
    {
        drawing->Render(glm::u8vec3(255, 255, 255));
        drawing->Render(glm::u8vec3(255, 255, 255));
        
        glm::ivec2 imgSize = img->GetSize();
        
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
        prop.imageP = img;
        
        std::vector<ssGUI::DrawingProperty> properties = 
        {
            prop,
            prop,
            prop,
            prop
        };
    
        drawing->DrawEntities(vertices, texCoords, colors, counts, properties);
        drawing->Render(glm::u8vec3(255, 255, 255));
        
        ssLOG_SIMPLE("Texture drawn");
        ssLOG_SIMPLE("You should see a squared image");
        ssLOG_SIMPLE("Then a rainbow image streched horizontally");
        ssLOG_SIMPLE("Then a semi-transparent image streched vertically");
        ssLOG_SIMPLE("Then a semi-transparent rainbow image triangle");
    }
}

void DrawFontTest()
{
    if( !inputs->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::THREE) &&
        inputs->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::THREE))
    {
        drawing->Render(glm::u8vec3(255, 255, 255));
        drawing->Render(glm::u8vec3(255, 255, 255));
        
        ssGUI::CharacterRenderInfo info = font->GetCharacterRenderInfo(L'A', 40);
        
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
        prop.fontP = font;
        prop.character = L'A';
        prop.characterSize = 40;
        
        std::vector<ssGUI::DrawingProperty> properties = 
        {
            prop,
            prop,
            prop,
            prop
        };
    
        drawing->DrawEntities(vertices, texCoords, colors, counts, properties);
        drawing->Render(glm::u8vec3(255, 255, 255));
        
        ssLOG_SIMPLE("Character drawn");
        ssLOG_SIMPLE("You should see the letter A");
        ssLOG_SIMPLE("Then a rainbow letter A streched horizontally");
        ssLOG_SIMPLE("Then a semi-transparent letter A streched vertically");
        ssLOG_SIMPLE("Then a semi-transparent rainbow letter A but cut out from the bottom right");
    }
}

int main()
{
    SetUp();
    
    Instructions();
        
    //Setup window and run it
    window->SetRenderSize(glm::ivec2(1280, 720));
    inputs->UpdateInput();

    while(!window->IsClosed())
    {
        inputs->UpdateInput();
        if(!window->IsClosed())
        {
            DrawColorShapesTest();
            DrawTextureTest();
            DrawFontTest();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    CleanUp();   
}

