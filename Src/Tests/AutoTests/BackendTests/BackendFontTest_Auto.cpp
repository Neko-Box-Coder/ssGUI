#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/Factory.hpp"
#include "ssGUI/Backend/Interfaces/BackendFontInterface.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"
#include "ssTest.hpp"
#include "ssLogger/ssLog.hpp"

#include "TestsResources.h"
#include <fstream>

#ifdef SSGUI_FONT_BACKEND_MOCK
    #include "ssGUI/Backend/Mocks/BackendFontMock.hpp"
    
    #define SSGUI_MOCK_FONT (*static_cast<ssGUI::Backend::BackendFontMock*>(TestFont))
#endif

//TODO: Atm SFML backend seems to crash regarding about wgl context, need to look into it.
//      Probably to do with OpenGL textures used for fonts where in the case of ssGUI on native Backend,
//      the wgl or glx is initialized only when you create a mainWindow. This could be the same for SFML as well.

ssGUI::Backend::BackendFontInterface* TestFont = nullptr;

int main()
{
    std::fstream checkExistFs;
    checkExistFs.open("./arial.ttf");
    if(checkExistFs.fail())
    {
        std::ofstream ofs("./arial.ttf", std::ofstream::binary | std::ofstream::out);
        ofs.write((char*)ssGUI_Test_arial, ssGUI_Test_arial_size);
        ofs.close();
    }
    else
        checkExistFs.close();
    
    checkExistFs.open("./NotoColorEmoji.ttf");
    
    if(checkExistFs.fail())
    {
        std::ofstream ofs("./NotoColorEmoji.ttf", std::ofstream::binary | std::ofstream::out);
        ofs.write((char*)ssGUI_Test_NotoColorEmoji, ssGUI_Test_NotoColorEmoji_size);
        ofs.close();
    }
    else
        checkExistFs.close();
    
    ssTEST_INIT();

    ssTEST_SET_UP
    {
        TestFont = ssGUI::Backend::BackendFactory::CreateBackendFontInterface();
    };

    ssTEST_CLEAN_UP
    {
        ssGUI::Factory::Dispose(TestFont);
    };
    
    ssTEST_DISABLE_CLEANUP_BETWEEN_TESTS();

    ssTEST("IsValidTest")
    {
        #ifdef SSGUI_FONT_BACKEND_MOCK
            SSGUI_MOCK_FONT .OverrideReturns(IsValid())
                            .Returns(false);
        #endif
    
        ssTEST_OUTPUT_ASSERT("Not loaded", !TestFont->IsValid());

        #ifdef SSGUI_FONT_BACKEND_MOCK
            SSGUI_MOCK_FONT.ClearAllOverrideReturns();
            SSGUI_MOCK_FONT .OverrideReturns(LoadFromMemory(void*, int))
                            .Returns(true)
                            .WhenCalledWith((void*)ssGUI_Test_arial, (int)ssGUI_Test_arial_size)
                            .Otherwise_Do([](...){ssGUI_ERROR(0, "Failed");});
            
            SSGUI_MOCK_FONT .OverrideReturns(LoadFromMemory(void*, int))
                            .Returns(false)
                            .WhenCalledWith(FO_ANY, FO_ANY);
        #endif

        bool result = TestFont->LoadFromMemory((void*)ssGUI_Test_arial, ssGUI_Test_arial_size);
        
        #ifdef SSGUI_FONT_BACKEND_MOCK
            SSGUI_MOCK_FONT.ClearAllOverrideReturns();
            SSGUI_MOCK_FONT .OverrideReturns(IsValid())
                            .Returns(true)
                            .If([&result](...) { return result; });
            
            SSGUI_MOCK_FONT .OverrideReturns(IsValid())
                            .Returns(false);
        #endif

        ssTEST_OUTPUT_ASSERT("loaded", TestFont->IsValid());
        
        #ifdef SSGUI_FONT_BACKEND_MOCK
            SSGUI_MOCK_FONT.ClearAllOverrideReturns();
        #endif
    };

    ssTEST("GetCharacterRenderInfoTest")
    {
        #ifdef SSGUI_FONT_BACKEND_MOCK
            SSGUI_MOCK_FONT .OverrideReturns(LoadFromMemory(void*, int))
                            .Returns(true);
        #endif
    
        TestFont->LoadFromMemory((void*)ssGUI_Test_arial, ssGUI_Test_arial_size);
        
        #ifdef SSGUI_FONT_BACKEND_MOCK
            ssGUI::CharacterRenderInfo retInfo;
            retInfo.Advance = 13;
            retInfo.DrawOffset = glm::vec2(-1, -15);
            retInfo.Size = glm::vec2(15, 15);
            
            SSGUI_MOCK_FONT .OverrideReturns(GetCharacterRenderInfo(wchar_t,float))
                            .Returns(retInfo)
                            .WhenCalledWith(L'A', 20.f)
                            .Otherwise_Do([](...){ssGUI_ERROR(0, "Failed");});
        #endif

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
        
        #ifdef SSGUI_FONT_BACKEND_MOCK
            SSGUI_MOCK_FONT.ClearAllOverrideReturns();
        #endif
    };

    ssTEST("IsCharacterSupportedTest")
    {
        #ifdef SSGUI_FONT_BACKEND_MOCK
            SSGUI_MOCK_FONT .OverrideReturns(IsCharacterSupported(wchar_t))
                            .Returns(true)
                            .WhenCalledWith(L'Z');
            
            SSGUI_MOCK_FONT .OverrideReturns(IsCharacterSupported(wchar_t))
                            .Returns(false)
                            .WhenCalledWith(L'あ')
                            .Otherwise_Do([](...){ssGUI_ERROR(0, "Failed");});
        #endif
    
        ssTEST_OUTPUT_ASSERT("Supported", TestFont->IsCharacterSupported(L'Z'));
                                    
        ssTEST_OUTPUT_ASSERT("Not Supported", !TestFont->IsCharacterSupported(L'あ'));
        
        #ifdef SSGUI_FONT_BACKEND_MOCK
            SSGUI_MOCK_FONT.ClearAllOverrideReturns();
        #endif
    };

    ssTEST_SKIP("GetKerningTest")
    {
        //TestFont->LoadFromPath(ResourcesFolderPath+"NotoSans-Regular.ttf");
        //ssLOG_LINE("AV Kerning: "<<TestFont->GetKerning(L'A', L'V', 20));
        
        //NOTE: Seems like HarfBuzz is needed for normal font kerning, 
        //but right now it is disabled because it is causing issues on CMake
        //so I will leave this skipped for now.
        ssTEST_OUTPUT_ASSERT(false);
    };

    #define FLOAT_EQUAL(a, b) (a > b - 0.1f && a < b + 0.1f)

    ssTEST("GetLineSpacingTest")
    {
        #ifdef SSGUI_FONT_BACKEND_MOCK
            SSGUI_MOCK_FONT .OverrideReturns(GetLineSpacing(float))
                            .Returns(23.f)
                            .WhenCalledWith(20.f)
                            .Otherwise_Do([](...){ssGUI_ERROR(0, "Failed");});
        #endif
    
        ssTEST_OUTPUT_ASSERT(__func__, FLOAT_EQUAL(TestFont->GetLineSpacing(20), 23));
        
        #ifdef SSGUI_FONT_BACKEND_MOCK
            SSGUI_MOCK_FONT.ClearAllOverrideReturns();
        #endif
    };

    ssTEST("GetUnderlineOffsetTest")
    {
        #ifdef SSGUI_FONT_BACKEND_MOCK
            SSGUI_MOCK_FONT .OverrideReturns(GetUnderlineOffset(float))
                            .Returns(2.859f)
                            .WhenCalledWith(20.f)
                            .Otherwise_Do([](...){ssGUI_ERROR(0, "Failed");});
        #endif

        ssTEST_OUTPUT_ASSERT(__func__, FLOAT_EQUAL(TestFont->GetUnderlineOffset(20), 2.859));
        
        #ifdef SSGUI_FONT_BACKEND_MOCK
            SSGUI_MOCK_FONT.ClearAllOverrideReturns();
        #endif
    };

    ssTEST("GetUnderlineThicknessTest")
    {
        #ifdef SSGUI_FONT_BACKEND_MOCK
            SSGUI_MOCK_FONT .OverrideReturns(GetUnderlineThickness(float))
                            .Returns(1.46f)
                            .WhenCalledWith(20.f)
                            .Otherwise_Do([](...){ssGUI_ERROR(0, "Failed");});
        #endif
        
        ssTEST_OUTPUT_ASSERT(__func__, FLOAT_EQUAL(TestFont->GetUnderlineThickness(20), 1.46f));
        
        #ifdef SSGUI_FONT_BACKEND_MOCK
            SSGUI_MOCK_FONT.ClearAllOverrideReturns();
        #endif
    };

    ssTEST("LoadFromPathTest")
    {
        ssTEST_CALL_CLEAN_UP();
        ssTEST_CALL_SET_UP();

        #ifdef SSGUI_FONT_BACKEND_MOCK
            SSGUI_MOCK_FONT .OverrideReturns(LoadFromPath(std::string))
                            .Returns(true)
                            .WhenCalledWith(std::string("./arial.ttf"))
                            .Otherwise_Do([](...){ssGUI_ERROR(0, "Failed");});
        #endif

        ssTEST_OUTPUT_ASSERT(__func__, TestFont->LoadFromPath("./arial.ttf"));
        //TODO: Test absolute path
        
        #ifdef SSGUI_FONT_BACKEND_MOCK
            SSGUI_MOCK_FONT.ClearAllOverrideReturns();
        #endif
    };

    ssTEST("LoadFromMemoryTest")
    {
        ssTEST_CALL_CLEAN_UP();
        ssTEST_CALL_SET_UP();
        
        std::ifstream fs;
        fs.open("./arial.ttf", std::ios::binary | std::ios::ate);
        if(!fs.is_open())
        {
            ssTEST_OUTPUT_ASSERT(__func__, false);
            return;
        }
        size_t size = fs.tellg();
        fs.seekg(0, std::ios::beg);
        uint8_t* buffer = new uint8_t[size];
        fs.read((char*)buffer, size); 
        
        #ifdef SSGUI_FONT_BACKEND_MOCK
            SSGUI_MOCK_FONT .OverrideReturns(LoadFromMemory(void*, int))
                            .Returns(true)
                            .WhenCalledWith((void*)buffer, (int)size)
                            .Otherwise_Do([](...){ssGUI_ERROR(0, "Failed");});
        #endif
        
        ssTEST_OUTPUT_ASSERT(TestFont->LoadFromMemory(buffer, size));
        delete[] buffer;
        
        #ifdef SSGUI_FONT_BACKEND_MOCK
            SSGUI_MOCK_FONT.ClearAllOverrideReturns();
        #endif
    };

    #if !defined(SSGUI_FONT_BACKEND_SFML)
        ssTEST("GetFixedAvailableFontSizesTest")
        {
            std::vector<float> fontSizes;
            ssTEST_CALL_CLEAN_UP();
            ssTEST_CALL_SET_UP();
            
            TestFont->LoadFromPath("./NotoColorEmoji.ttf");
            
            #ifdef SSGUI_FONT_BACKEND_MOCK
                std::vector<float> setFontSizes;
                setFontSizes.push_back(128.f);
                SSGUI_MOCK_FONT .OverrideArgs(GetFixedAvailableFontSizes(std::vector<float>&))
                                .SetArgs(setFontSizes)
                                .Otherwise_Do([](...){ssGUI_ERROR(0, "Failed");});
            #endif
            
            ssTEST_OUTPUT_ASSERT("Operation", TestFont->GetFixedAvailableFontSizes(fontSizes));
            ssTEST_OUTPUT_ASSERT("Length", fontSizes.size() == 1);
            ssTEST_OUTPUT_ASSERT("Size", FLOAT_EQUAL(fontSizes[0],  128));
            
            #ifdef SSGUI_FONT_BACKEND_MOCK
                SSGUI_MOCK_FONT.ClearAllOverrideArgs();
            #endif
        };
    #else
        ssTEST_SKIP("GetFixedAvailableFontSizesTest")
        {
        };
    #endif

    ssTEST("GetCharacterImageTest")
    {
        ssTEST_CALL_CLEAN_UP();
        ssTEST_CALL_SET_UP();
        TestFont->LoadFromPath("./arial.ttf");   

        #ifdef SSGUI_FONT_BACKEND_MOCK
            bool calledCorrectly = false;
            SSGUI_MOCK_FONT .OverrideArgs(GetCharacterImage(wchar_t, float, ssGUI::ImageData&))
                            .SetArgs(FO_DONT_SET, FO_DONT_SET, FO_DONT_SET) //NOTE: We can't set ssGUI::ImageData because it is not copyable 
                            .WhenCalledWith(L'A', 20.f, FO_ANY)             //      with default copy constructor or assignment
                            .WhenCalledExpectedly_Do([&calledCorrectly](...){ calledCorrectly = true; });
        #endif

        ssGUI::ImageData data;
        ssTEST_OUTPUT_ASSERT("Operation", TestFont->GetCharacterImage(L'A', 20, data));
        
        #ifdef SSGUI_FONT_BACKEND_MOCK
            if(calledCorrectly)
            {
                auto& imgBackendMock = (*static_cast<ssGUI::Backend::BackendImageMock*>(data.GetBackendImageInterface()));
                imgBackendMock  .OverrideReturns(GetSize())
                                .Returns(glm::ivec2(15, 15));
            }
        #endif
        
        ssTEST_OUTPUT_ASSERT("Size", data.GetSize() ==  glm::ivec2(15, 15));
        
        #ifdef SSGUI_FONT_BACKEND_MOCK
            SSGUI_MOCK_FONT.ClearAllOverrideArgs();
        #endif
    };

    ssTEST("GetRawHandleTest")
    {
        #ifdef SSGUI_FONT_BACKEND_MOCK
            SSGUI_MOCK_FONT .OverrideReturns(GetRawHandle())
                            .Returns((void*)0x1);
        #endif
    
        ssTEST_OUTPUT_ASSERT(TestFont->GetRawHandle() != nullptr);
        
        #ifdef SSGUI_FONT_BACKEND_MOCK
            SSGUI_MOCK_FONT.ClearAllOverrideReturns();
        #endif
    };

    ssTEST("CloneTest")
    {
        #ifdef SSGUI_FONT_BACKEND_MOCK
            ssGUI::CharacterRenderInfo retInfo;
            retInfo.Size = glm::vec2(15, 15);
            
            SSGUI_MOCK_FONT .OverrideReturns(GetCharacterRenderInfo(wchar_t,float))
                            .Returns(retInfo)
                            .WhenCalledWith(L'A', 20.f)
                            .Otherwise_Do([](...){ssGUI_ERROR(0, "Failed");});
        #endif
    
        ssGUI::Backend::BackendFontInterface* clonedFont = TestFont->Clone();
        ssTEST_OUTPUT_ASSERT("Validation", clonedFont != nullptr);
        
        glm::vec2 fontSize = clonedFont->GetCharacterRenderInfo(L'A', 20).Size;
        ssTEST_OUTPUT_ASSERT("Size", FLOAT_EQUAL(fontSize.x, 15) && FLOAT_EQUAL(fontSize.y, 15));
    };

    ssTEST_END();
}

