#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/Factory.hpp"
#include "ssGUI/Backend/Interfaces/BackendFontInterface.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"
#include "ssTest.hpp"
#include "ssLogger/ssLog.hpp"

#include <fstream>


ssGUI::Backend::BackendFontInterface* font = nullptr;

#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
    std::string ResourcesFolderPath = "..\\Resources\\";
#else
    std::string ResourcesFolderPath = "./Resources/";
#endif

ssTEST_INIT();

ssTEST_SET_UP
{
    font = ssGUI::Backend::BackendFactory::CreateBackendFontInterface();
}

ssTEST_CLEAN_UP
{
    ssGUI::Factory::Dispose(font);
}

ssTEST("IsValidTest")
{
    ssTEST_OUTPUT_ASSERT("Not loaded", !font->IsValid());

    font->LoadFromPath(ResourcesFolderPath+"arial.ttf");

    ssTEST_OUTPUT_ASSERT("loaded", font->IsValid());
}

ssTEST("GetCharacterRenderInfoTest")
{
    font->LoadFromPath(ResourcesFolderPath+"arial.ttf");
    
    ssGUI::CharacterRenderInfo info = font->GetCharacterRenderInfo(L'A', 20);
    
    ssTEST_OUTPUT_ASSERT("Advance", info.Advance == 13);
    
    ssTEST_OUTPUT_ASSERT("DrawOffset", info.DrawOffset == glm::vec2(-1, -15));
                                
    ssTEST_OUTPUT_ASSERT("Size", info.Size == glm::vec2(15, 15));
    
    //ssLOG_LINE("info.Advance: "<<info.Advance); 
    //ssLOG_LINE("info.DrawOffset: "<<info.DrawOffset.x<<", "<<info.DrawOffset.y); 
    //ssLOG_LINE("info.Size: "<<info.Size.x<<", "<<info.Size.y); 
    //ssLOG_LINE("info.BaselinePosition: "<<info.BaselinePosition.x<<", "<<info.BaselinePosition.y); 
    //ssLOG_LINE("info.CharacterAtNewline: "<<info.CharacterAtNewline);
    //ssLOG_LINE("info.Rendered: "<<info.Rendered);
    //ssLOG_LINE("info.Valid: "<<info.Valid);
    //ssLOG_LINE("info.LineMinY: "<<info.LineMinY);
    //ssLOG_LINE("info.LineMaxY: "<<info.LineMaxY);
}

ssTEST("IsCharacterSupportedTest")
{
    ssTEST_OUTPUT_ASSERT("Supported", font->IsCharacterSupported(L'Z'));
                                
    ssTEST_OUTPUT_ASSERT("Not Supported", !font->IsCharacterSupported(L'あ'));
}

ssTEST_SKIP("GetKerningTest")
{
    //font->LoadFromPath(ResourcesFolderPath+"NotoSans-Regular.ttf");
    //ssLOG_LINE("AV Kerning: "<<font->GetKerning(L'A', L'V', 20));
    
    //NOTE: Seems like HarfBuzz is needed for normal font kerning, 
    //but right now it is disabled because it is causing issues on CMake
    //so I will leave this skipped for now.
    ssTEST_OUTPUT_ASSERT(false);
}

#define FLOAT_EQUAL(a, b) (a > b - 0.1f && a < b + 0.1f)

ssTEST("GetLineSpacingTest")
{
    ssTEST_OUTPUT_ASSERT(__func__, FLOAT_EQUAL(font->GetLineSpacing(20), 23));
}

ssTEST("GetUnderlineOffsetTest")
{
    ssTEST_OUTPUT_ASSERT(__func__, FLOAT_EQUAL(font->GetUnderlineOffset(20), 2.859));
}

ssTEST("GetUnderlineThicknessTest")
{
    ssTEST_OUTPUT_ASSERT(__func__, FLOAT_EQUAL(font->GetUnderlineThickness(20), 1.46f));
}

ssTEST("LoadFromPathTest")
{
    ssTEST_CALL_CLEAN_UP();
    ssTEST_CALL_SET_UP();
    ssTEST_OUTPUT_ASSERT(__func__, font->LoadFromPath(ResourcesFolderPath+"arial.ttf"));
    //TODO: Test absolute path
}

ssTEST("LoadFromMemoryTest")
{
    ssTEST_CALL_CLEAN_UP();
    ssTEST_CALL_SET_UP();
    
    std::ifstream fs;
    fs.open(ResourcesFolderPath+"arial.ttf", std::ios::binary | std::ios::ate);
    if(!fs.is_open())
    {
        ssTEST_OUTPUT_ASSERT(__func__, false);
        return;
    }
    size_t size = fs.tellg();
    fs.seekg(0, std::ios::beg);
    uint8_t* buffer = new uint8_t[size];
    fs.read((char*)buffer, size); 
    
    ssTEST_OUTPUT_ASSERT(, font->LoadFromMemory(buffer, size));delete[] buffer;
}

ssTEST("GetFixedAvailableFontSizesTest")
{
    std::vector<float> fontSizes;
    ssTEST_CALL_CLEAN_UP();
    ssTEST_CALL_SET_UP();
    
    font->LoadFromPath(ResourcesFolderPath+"NotoColorEmoji.ttf");
    
    ssTEST_OUTPUT_ASSERT("Operation", font->GetFixedAvailableFontSizes(fontSizes));
    ssTEST_OUTPUT_ASSERT("Length", fontSizes.size() == 1);
    ssTEST_OUTPUT_ASSERT("Size", FLOAT_EQUAL(fontSizes[0],  128));
    
    ssTEST_CALL_CLEAN_UP();
    ssTEST_CALL_SET_UP();
    font->LoadFromPath(ResourcesFolderPath+"arial.ttf");   
}

ssTEST("GetCharacterImageTest")
{
    ssGUI::ImageData data;
    ssTEST_OUTPUT_ASSERT("Operation", font->GetCharacterImage(L'A', 20, data));
    ssTEST_OUTPUT_ASSERT("Size", data.GetSize() ==  glm::ivec2(15, 15));
}

ssTEST("GetRawHandleTest")
{
    ssTEST_OUTPUT_ASSERT(font->GetRawHandle() != nullptr);
}

ssTEST("CloneTest")
{
    ssGUI::Backend::BackendFontInterface* clonedFont = font->Clone();
    ssTEST_OUTPUT_ASSERT("Validation", clonedFont != nullptr);
    
    glm::vec2 fontSize = clonedFont->GetCharacterRenderInfo(L'A', 20).Size;
    ssTEST_OUTPUT_ASSERT("Size", FLOAT_EQUAL(fontSize.x, 15) && FLOAT_EQUAL(fontSize.y, 15));
    
}

ssTEST_END();
