#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/Factory.hpp"
#include "ssGUI/Backend/Interfaces/BackendFontInterface.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"
#include "ssTest.hpp"
#include "ssLogger/ssLog.hpp"

#include <fstream>

//TODO: Atm SFML backend seems to crash regarding about wgl context, need to look into it.
//      Probably to do with OpenGL textures used for fonts where in the case of ssGUI on native Backend,
//      the wgl or glx is initialized only when you create a mainWindow. This could be the same for SFML as well.

ssGUI::Backend::BackendFontInterface* TestFont = nullptr;

#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
    std::string ResourcesFolderPath = "..\\Resources\\";
#else
    std::string ResourcesFolderPath = "./Resources/";
#endif

int main()
{
    ssTEST_INIT();

    ssTEST_SET_UP
    {
        TestFont = ssGUI::Backend::BackendFactory::CreateBackendFontInterface();
    }

    ssTEST_CLEAN_UP
    {
        ssGUI::Factory::Dispose(TestFont);
    }
    
    ssTEST_DISABLE_CLEANUP_BETWEEN_TESTS();

    ssTEST("IsValidTest")
    {
        ssTEST_OUTPUT_ASSERT("Not loaded", !TestFont->IsValid());

        TestFont->LoadFromPath(ResourcesFolderPath+"arial.ttf");

        ssTEST_OUTPUT_ASSERT("loaded", TestFont->IsValid());
    }

    ssTEST("GetCharacterRenderInfoTest")
    {
        TestFont->LoadFromPath(ResourcesFolderPath+"arial.ttf");
        
        ssGUI::CharacterRenderInfo info = TestFont->GetCharacterRenderInfo(L'A', 20);
        
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
        ssTEST_OUTPUT_ASSERT("Supported", TestFont->IsCharacterSupported(L'Z'));
                                    
        ssTEST_OUTPUT_ASSERT("Not Supported", !TestFont->IsCharacterSupported(L'あ'));
    }

    ssTEST_SKIP("GetKerningTest")
    {
        //TestFont->LoadFromPath(ResourcesFolderPath+"NotoSans-Regular.ttf");
        //ssLOG_LINE("AV Kerning: "<<TestFont->GetKerning(L'A', L'V', 20));
        
        //NOTE: Seems like HarfBuzz is needed for normal font kerning, 
        //but right now it is disabled because it is causing issues on CMake
        //so I will leave this skipped for now.
        ssTEST_OUTPUT_ASSERT(false);
    }

    #define FLOAT_EQUAL(a, b) (a > b - 0.1f && a < b + 0.1f)

    ssTEST("GetLineSpacingTest")
    {
        ssTEST_OUTPUT_ASSERT(__func__, FLOAT_EQUAL(TestFont->GetLineSpacing(20), 23));
    }

    ssTEST("GetUnderlineOffsetTest")
    {
        ssTEST_OUTPUT_ASSERT(__func__, FLOAT_EQUAL(TestFont->GetUnderlineOffset(20), 2.859));
    }

    ssTEST("GetUnderlineThicknessTest")
    {
        ssTEST_OUTPUT_ASSERT(__func__, FLOAT_EQUAL(TestFont->GetUnderlineThickness(20), 1.46f));
    }

    ssTEST("LoadFromPathTest")
    {
        ssTEST_CALL_CLEAN_UP();
        ssTEST_CALL_SET_UP();
        ssTEST_OUTPUT_ASSERT(__func__, TestFont->LoadFromPath(ResourcesFolderPath+"arial.ttf"));
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
        
        ssTEST_OUTPUT_ASSERT(, TestFont->LoadFromMemory(buffer, size));delete[] buffer;
    }

    #if !defined(SSGUI_FONT_BACKEND_SFML)
        ssTEST("GetFixedAvailableFontSizesTest")
        {
            std::vector<float> fontSizes;
            ssTEST_CALL_CLEAN_UP();
            ssTEST_CALL_SET_UP();
            
            TestFont->LoadFromPath(ResourcesFolderPath+"NotoColorEmoji.ttf");
            
            ssTEST_OUTPUT_ASSERT("Operation", TestFont->GetFixedAvailableFontSizes(fontSizes));
            ssTEST_OUTPUT_ASSERT("Length", fontSizes.size() == 1);
            ssTEST_OUTPUT_ASSERT("Size", FLOAT_EQUAL(fontSizes[0],  128));
        }
    #else
        ssTEST_SKIP("GetFixedAvailableFontSizesTest")
        {
        }
    #endif

    ssTEST("GetCharacterImageTest")
    {
        ssTEST_CALL_CLEAN_UP();
        ssTEST_CALL_SET_UP();
        TestFont->LoadFromPath(ResourcesFolderPath+"arial.ttf");   

        ssGUI::ImageData data;
        ssTEST_OUTPUT_ASSERT("Operation", TestFont->GetCharacterImage(L'A', 20, data));
        ssTEST_OUTPUT_ASSERT("Size", data.GetSize() ==  glm::ivec2(15, 15));
    }

    ssTEST("GetRawHandleTest")
    {
        ssTEST_OUTPUT_ASSERT(TestFont->GetRawHandle() != nullptr);
    }

    ssTEST("CloneTest")
    {
        ssGUI::Backend::BackendFontInterface* clonedFont = TestFont->Clone();
        ssTEST_OUTPUT_ASSERT("Validation", clonedFont != nullptr);
        
        glm::vec2 fontSize = clonedFont->GetCharacterRenderInfo(L'A', 20).Size;
        ssTEST_OUTPUT_ASSERT("Size", FLOAT_EQUAL(fontSize.x, 15) && FLOAT_EQUAL(fontSize.y, 15));
        
    }

    ssTEST_END();
}

