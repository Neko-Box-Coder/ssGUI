#include "ssGUI/GUIObjectClasses/Text.hpp"
#include "ssTest.hpp"


#ifdef SSGUI_FONT_BACKEND_MOCK
    #include "ssGUI/Backend/Mocks/BackendFontMock.hpp"
    #include "ssGUI/Backend/Mocks/Data/MockFontData.hpp"
#endif

ssGUI::Text* TestText = nullptr; 
std::string TestString = "This is some random text for testing";
std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

int main()
{
    ssTEST_INIT();

    ssTEST_SET_UP
    {
        TestText = ssGUI::Create<ssGUI::Text>();
        
        #ifdef SSGUI_FONT_BACKEND_MOCK
            assert(TestText->GetDefaultFontsCount() > 0);
            auto* defaultBackendMockFont = 
                static_cast<ssGUI::Backend::BackendFontMock*>(TestText->GetDefaultFont(0)->GetBackendFontInterface());
            
            ssGUI::SetMockBackendFontData(*defaultBackendMockFont);
        #endif
        
        TestText->SetSize(glm::vec2(50, 50));
        TestText->SetNewTextFontSize(15);
        TestText->SetTextAlignment(ssGUI::Enums::AlignmentHorizontal::LEFT, ssGUI::Enums::AlignmentVertical::TOP);
        TestText->SetText(TestString);
    }

    ssTEST_CLEAN_UP
    {
        ssGUI::Dispose(TestText);
    }

    ssTEST_SKIP("ComputeCharactersPositionAndSizeTest")
    {
    }

    ssTEST("SetTextTest")
    {
        ssTEST_OUTPUT_ASSERT(converter.to_bytes(TestText->GetText()) == TestString);
        
        TestText->SetText(L"≥⚠️☢️🧪🪡💰⚙️🎲❗⛔❕☣️\n💬🐞🎉🌍📄🔌😉✅🛳️⏬⏱️🕸️❌");
        ssTEST_OUTPUT_ASSERT("Wide", TestText->GetText() == L"≥⚠️☢️🧪🪡💰⚙️🎲❗⛔❕☣️\n💬🐞🎉🌍📄🔌😉✅🛳️⏬⏱️🕸️❌");
    }

    ssTEST("AddTextTest")
    {
        std::string newString = " new text";
        TestText->AddText(newString);
        
        ssTEST_OUTPUT_ASSERT(TestText->GetText<std::string>() == TestString + newString);
        
        ssTEST_CALL_CLEAN_UP();
        ssTEST_CALL_SET_UP();
    
        TestText->AddText(newString, 12);
        std::string testCopy = TestString;
        testCopy.insert(testCopy.begin() + 12, newString.begin(), newString.end());
        
        ssTEST_OUTPUT_ASSERT("Indexed", converter.to_bytes(TestText->GetText()) == testCopy);
        
        ssTEST_CALL_CLEAN_UP();
        ssTEST_CALL_SET_UP();
        
        std::wstring wideNewString = L"⚠️☢️🧪";
        TestText->AddText(wideNewString);
        std::wstring wideTestCopy = converter.from_bytes(TestString);
        wideTestCopy.insert(wideTestCopy.end(), wideNewString.begin(), wideNewString.end());
        
        ssTEST_OUTPUT_ASSERT("Wide", TestText->GetText() == wideTestCopy);
        
        ssTEST_CALL_CLEAN_UP();
        ssTEST_CALL_SET_UP();
        
        TestText->AddText(wideNewString, 12);
        wideTestCopy = converter.from_bytes(TestString);
        wideTestCopy.insert(wideTestCopy.begin() + 12, wideNewString.begin(), wideNewString.end());
        
        ssTEST_OUTPUT_ASSERT("Wide Indexed", TestText->GetText() == wideTestCopy);
    }
    
    ssTEST("RemoveTextTest")
    {
        TestText->RemoveText(7, 15);
        std::string testCopy = TestString;
        testCopy.erase(testCopy.begin() + 7, testCopy.begin() + 15);
        
        ssTEST_OUTPUT_ASSERT(converter.to_bytes(TestText->GetText()) == testCopy);
    }

    ssTEST_SKIP("GetTextTest")
    {
    }
    
    ssTEST("GetCharacterCountTest")
    {
        ssTEST_OUTPUT_ASSERT(TestText->GetCharacterCount() == TestString.size());
        
        std::wstring wideNewString = L"⚠️☢️🧪T";
        
        TestText->AddText(wideNewString);
        
        #if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
            //There's a VARIATION SELECTOR-16 character after ⚠️ and ☢️ and 🧪, therefore adding 3 characters
            ssTEST_OUTPUT_ASSERT(TestText->GetCharacterCount() == TestString.size() + 4 + 3);
        #else
            //There's a VARIATION SELECTOR-16 character after ⚠️ and ☢️, therefore adding 2 characters
            ssTEST_OUTPUT_ASSERT(TestText->GetCharacterCount() == TestString.size() + 4 + 2);
        #endif
    }
    
    #define FLOAT_EQ(a, b) (a + 0.0001f > b && a - 0.0001f < b)
    ssTEST("GetCharacterRenderInfoTest")
    {
        TestText->ComputeCharactersPositionAndSize();
        
        auto testRenderInfo = TestText->GetCharacterRenderInfo(13);
        ssTEST_OUTPUT_ASSERT("Advance", FLOAT_EQ(testRenderInfo.Advance, 7));
        ssTEST_OUTPUT_ASSERT("DrawOffset", FLOAT_EQ(testRenderInfo.DrawOffset.x, 1) && FLOAT_EQ(testRenderInfo.DrawOffset.y, -8));
        ssTEST_OUTPUT_ASSERT("Size", FLOAT_EQ(testRenderInfo.Size.x, 6) && FLOAT_EQ(testRenderInfo.Size.y, 8));
        
        //SFML GetKerning is returning -1 for some characters, while FreeType implementation doesn't (for now). 
        #ifdef SSGUI_FONT_BACKEND_SFML
            ssTEST_OUTPUT_ASSERT("BaselinePosition", FLOAT_EQ(testRenderInfo.BaselinePosition.x, 93) && FLOAT_EQ(testRenderInfo.BaselinePosition.y, 20));
        #else
            ssTEST_OUTPUT_ASSERT("BaselinePosition", FLOAT_EQ(testRenderInfo.BaselinePosition.x, 94) && FLOAT_EQ(testRenderInfo.BaselinePosition.y, 20));
        #endif
        
        ssTEST_OUTPUT_ASSERT("CharacterAtNewline", !testRenderInfo.CharacterAtNewline);
        ssTEST_OUTPUT_ASSERT("Rendered", testRenderInfo.Rendered);
        ssTEST_OUTPUT_ASSERT("Valid", testRenderInfo.Valid);
        ssTEST_OUTPUT_ASSERT("LineMinY", FLOAT_EQ(testRenderInfo.LineMinY, -15));
        
        #ifdef SSGUI_FONT_BACKEND_SFML
            ssTEST_OUTPUT_ASSERT("LineMaxY", FLOAT_EQ(testRenderInfo.LineMaxY, 5));
        #else
            ssTEST_OUTPUT_ASSERT("LineMaxY", FLOAT_EQ(testRenderInfo.LineMaxY, 5.4375));
        #endif
        
        ssTEST_OUTPUT_ASSERT("RenderFontSize", FLOAT_EQ(testRenderInfo.RenderFontSize, 15));
        ssTEST_OUTPUT_ASSERT("TargetSizeMultiplier", FLOAT_EQ(testRenderInfo.TargetSizeMultiplier, 1));
        
        
        //For printing all character render info
        //for(int i = 0; i < TestText->GetCharacterCount(); i++)
        //{
        //    auto currentRenderInfo = TestText->GetCharacterRenderInfo(i);
        //    ssLOG_LINE("Character["<<i<<"]: "<<(char)TestText->GetCharacterDetails(i).Character);
        //    ssLOG_LINE("currentRenderInfo["<<i<<"].Advance: "<<currentRenderInfo.Advance);
        //    ssLOG_LINE("currentRenderInfo["<<i<<"].DrawOffset: "<<currentRenderInfo.DrawOffset.x<<", "<<currentRenderInfo.DrawOffset.y);
        //    ssLOG_LINE("currentRenderInfo["<<i<<"].Size: "<<currentRenderInfo.Size.x<<", "<<currentRenderInfo.Size.y);
        //    ssLOG_LINE("currentRenderInfo["<<i<<"].BaselinePosition: "<<currentRenderInfo.BaselinePosition.x<<", "<<currentRenderInfo.BaselinePosition.y);
        //    ssLOG_LINE("currentRenderInfo["<<i<<"].CharacterAtNewline: "<<currentRenderInfo.CharacterAtNewline);
        //    ssLOG_LINE("currentRenderInfo["<<i<<"].Rendered: "<<currentRenderInfo.Rendered);
        //    ssLOG_LINE("currentRenderInfo["<<i<<"].Valid: "<<currentRenderInfo.Valid);
        //    ssLOG_LINE("currentRenderInfo["<<i<<"].LineMinY: "<<currentRenderInfo.LineMinY);
        //    ssLOG_LINE("currentRenderInfo["<<i<<"].LineMaxY: "<<currentRenderInfo.LineMaxY);
        //    ssLOG_LINE("currentRenderInfo["<<i<<"].RenderFontSize: "<<currentRenderInfo.RenderFontSize);
        //    ssLOG_LINE("currentRenderInfo["<<i<<"].TargetSizeMultiplier: "<<currentRenderInfo.TargetSizeMultiplier);
        //    ssLOG_LINE();
        //}
    }

    ssTEST("SetCharacterDetailsTest")
    {
        auto detail = TestText->GetCharacterDetails(9);
        detail.Character = L'J';
        detail.FontSize = 25;
        detail.Underlined = true;
        
        TestText->SetCharacterDetails(9, detail);
        
        ssTEST_OUTPUT_ASSERT(TestText->GetCharacterDetails(9) == detail);
        //ssTEST_OUTPUT_ASSERT(   TestText->GetCharacterDetails(9).Character == detail.Character &&
        //                        TestText->GetCharacterDetails(9).FontSize == detail.FontSize &&
        //                        TestText->GetCharacterDetails(9).Underlined == detail.Underlined &&
        //                        TestText->GetCharacterDetails(9).FontIndex == detail.FontIndex);
    
        //Printing a character detail
        //auto detail = TestText->GetCharacterDetails(9);
        
        //ssLOG_LINE("detail.Character: "<<(char)detail.Character);
        //ssLOG_LINE("detail.FontIndex: "<<detail.FontIndex);
        //ssLOG_LINE("detail.DefaultFontIndex: "<<detail.DefaultFontIndex);
        //ssLOG_LINE("detail.FontSize: "<<detail.FontSize);
        //ssLOG_LINE("detail.CharacterColor: "<<(int)detail.CharacterColor.r<<", "<<(int)detail.CharacterColor.g<<", "<<(int)detail.CharacterColor.b<<", "<<(int)detail.CharacterColor.a);
        //ssLOG_LINE("detail.Underlined: "<<detail.Underlined);
    }
    
    ssTEST_SKIP("GetCharacterDetails")
    {
    }
    
    ssTEST("GetCharactersDetailsCountTest")
    {
        ssTEST_OUTPUT_ASSERT(TestText->GetCharactersDetailsCount() == TestText->GetCharacterCount());
    }
    
    ssTEST("AddCharacterDetailsTest")
    {
        ssGUI::CharacterDetails details;
        details.Character = L'P';
        details.Underlined = true;
        
        TestText->AddCharacterDetails(15, details);
        ssTEST_OUTPUT_ASSERT("Single character with index", TestText->GetCharacterDetails(15) == details);
        
        TestText->AddCharacterDetails(details);
        ssTEST_OUTPUT_ASSERT("Single character", TestText->GetCharacterDetails(TestText->GetCharacterCount() - 1) == details);
    
        std::vector<ssGUI::CharacterDetails> testDetailsList;
        
        details.Character = L'G';
        details.Underlined = false;
        testDetailsList.push_back(details);
        
        details.Character = L'A';
        details.Underlined = true;
        details.FontSize = 51;
        testDetailsList.push_back(details);
        
        details.Character = L'[';
        details.CharacterColor = glm::u8vec4(127, 127, 127, 255);
        testDetailsList.push_back(details);

        TestText->AddCharacterDetails(11, testDetailsList);
        ssTEST_OUTPUT_ASSERT("Multiple characters with index",  TestText->GetCharacterDetails(11) == testDetailsList[0] &&
                                                                TestText->GetCharacterDetails(12) == testDetailsList[1] &&
                                                                TestText->GetCharacterDetails(13) == testDetailsList[2]);
        
        int appendIndex = TestText->GetCharacterCount();
        TestText->AddCharacterDetails(testDetailsList);
        ssTEST_OUTPUT_ASSERT("Multiple characters", TestText->GetCharacterDetails(appendIndex) == testDetailsList[0] &&
                                                    TestText->GetCharacterDetails(appendIndex + 1) == testDetailsList[1] &&
                                                    TestText->GetCharacterDetails(appendIndex + 2) == testDetailsList[2]);
    }

    ssTEST("RemoveCharacterDetailsTest")
    {
        auto details = TestText->GetCharacterDetails(13);
        TestText->RemoveCharacterDetails(13);
        
        ssTEST_OUTPUT_ASSERT(details != TestText->GetCharacterDetails(13));
        
        details = TestText->GetCharacterDetails(5);
        auto details2 = TestText->GetCharacterDetails(6);
        auto details3 = TestText->GetCharacterDetails(7);
        
        TestText->RemoveCharacterDetails(5, 8);
        ssTEST_OUTPUT_ASSERT("Range",   details != TestText->GetCharacterDetails(5) &&
                                        details != TestText->GetCharacterDetails(6) &&
                                        details != TestText->GetCharacterDetails(7));
    }
    
    ssTEST("ClearAllCharacterDetails")
    {
        ssTEST_OUTPUT_ASSERT("Initial", TestText->GetCharacterCount() > 0);
        TestText->ClearAllCharacterDetails();
        ssTEST_OUTPUT_ASSERT(TestText->GetCharacterCount() == 0);
    }
    
    ssTEST("GetCharacterGlobalPositionTest")
    {
        TestText->ComputeCharactersPositionAndSize();
        ssTEST_OUTPUT_ASSERT(TestText->GetCharacterGlobalPosition(9, true) == glm::vec2(59, 12));
        
        //ssLOG_LINE("TestText->GetCharacterDetails(7).Character: "<<(char)TestText->GetCharacterDetails(9).Character);
        //ssLOG_LINE("TestText->GetCharacterGlobalPosition: "<<TestText->GetCharacterGlobalPosition(9, true).x<<", "<<TestText->GetCharacterGlobalPosition(9, true).y);
    }

    ssTEST("IsOverflowTest")
    {
        TestText->SetSize(glm::vec2(200, 50));
        TestText->ComputeCharactersPositionAndSize();
        ssTEST_OUTPUT_ASSERT("Horizontal", TestText->IsOverflow());
        
        TestText->AddText("\n", 16);
        TestText->ComputeCharactersPositionAndSize();
        ssTEST_OUTPUT_ASSERT("None", !TestText->IsOverflow());
        
        TestText->AddText("\n", 32);
        TestText->ComputeCharactersPositionAndSize();
        ssTEST_OUTPUT_ASSERT("Vertical", TestText->IsOverflow());
        
        //for(int i = 0; i < TestText->GetCharacterCount(); i++)
        //{
        //    ssLOG_LINE("GetCharacterDetails("<<i<<").Character: "<<(char)TestText->GetCharacterDetails(i).Character);
        //    ssLOG_LINE("GetCharacterRenderInfo("<<i<<").BaselinePosition: "<<TestText->GetCharacterRenderInfo(i).BaselinePosition.x<<", "<<TestText->GetCharacterRenderInfo(i).BaselinePosition.y); 
        //    ssLOG_LINE("GetCharacterRenderInfo("<<i<<").Size: "<<TestText->GetCharacterRenderInfo(i).Size.x<<", "<<TestText->GetCharacterRenderInfo(i).Size.y); 
        //}
        
        //ssLOG_LINE("TestText->IsOverflow(): "<<TestText->IsOverflow());
    }

    ssTEST("NewTextFontSizeTest")
    {
        ssTEST_OUTPUT_ASSERT("Initial", TestText->GetNewTextFontSize() != 25);
    
        TestText->SetNewTextFontSize(25);
        ssTEST_OUTPUT_ASSERT("Setting", TestText->GetNewTextFontSize() == 25);
    
        TestText->AddText("New Text");
        
        ssTEST_OUTPUT_ASSERT("Character", TestText->GetCharacterDetails(TestText->GetCharacterCount() - 1).FontSize == 25);
    }

    ssTEST("NewTextColorTest")
    {
        ssTEST_OUTPUT_ASSERT("Initial", TestText->GetNewTextColor() != glm::u8vec4(255, 0, 0, 255));
    
        TestText->SetNewTextColor(glm::u8vec4(255, 0, 0, 255));
        ssTEST_OUTPUT_ASSERT("Setting", TestText->GetNewTextColor() == glm::u8vec4(255, 0, 0, 255));
    
        TestText->AddText("New Text");
        
        ssTEST_OUTPUT_ASSERT("Character", TestText->GetCharacterDetails(TestText->GetCharacterCount() - 1).CharacterColor == glm::u8vec4(255, 0, 0, 255));
    }
    
    ssTEST("NewTextUnderlinedTest")
    {
        ssTEST_OUTPUT_ASSERT("Initial", !TestText->IsNewTextUnderlined());
    
        TestText->SetNewTextUnderlined(true);
        ssTEST_OUTPUT_ASSERT("Setting", TestText->IsNewTextUnderlined());
    
        TestText->AddText("New Text");
        
        ssTEST_OUTPUT_ASSERT("Character", TestText->GetCharacterDetails(TestText->GetCharacterCount() - 1).Underlined);
    }

    ssTEST("ApplyNewTextSettingsToExistingTextTest")
    {
        TestText->SetNewTextFontSize(25);
        TestText->SetNewTextColor(glm::u8vec4(255, 0, 0, 255));
        TestText->SetNewTextUnderlined(true);
        
        TestText->ApplyNewTextSettingsToExistingText();
        
        ssTEST_OUTPUT_ASSERT(   TestText->GetCharacterDetails(0).FontSize == 25 &&
                                TestText->GetCharacterDetails(0).CharacterColor == glm::u8vec4(255, 0, 0, 255) &&
                                TestText->GetCharacterDetails(0).Underlined);
    }
    
    ssTEST("MultilineAllowedTest")
    {
        TestText->SetMultilineAllowed(true);
        TestText->AddText("\n", 15);
        TestText->ComputeCharactersPositionAndSize();
        
        ssTEST_OUTPUT_ASSERT("Allowed", TestText->GetCharacterRenderInfo(0).BaselinePosition.y != TestText->GetCharacterRenderInfo(20).BaselinePosition.y);
        
        TestText->SetMultilineAllowed(false);
        TestText->ComputeCharactersPositionAndSize();
        
        ssTEST_OUTPUT_ASSERT("Not Allowed", TestText->GetCharacterRenderInfo(0).BaselinePosition.y == TestText->GetCharacterRenderInfo(20).BaselinePosition.y);
    }

    ssTEST_SKIP("WrappingModeTest")
    {
    }

    ssTEST_SKIP("AlignmentTest")
    {
    }
    
    ssTEST_SKIP("FontTest")
    {
    }
    
    ssTEST_SKIP("PaddingTest")
    {
    }
    
    ssTEST_SKIP("SpaceTest")
    {
    }
    
    ssTEST_SKIP("TabSizeTest")
    {
    }
    
    ssTEST_SKIP("TextSelectionTest")
    {
    }
    
    ssTEST_SKIP("CharacterPosTest")
    {
    }

    ssTEST_END();
}
