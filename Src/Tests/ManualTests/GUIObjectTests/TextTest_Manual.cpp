#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssLogger/ssLog.hpp"
#include "TestsResources.h"

ssGUI::MainWindow* MainWindow = nullptr;
ssGUI::ssGUIManager* Manager = nullptr;
ssGUI::Text* TestText = nullptr;
ssGUI::Font* TestFont = nullptr;

ssGUI::Enums::GenericButtonAndKeyInput TestMode = ssGUI::Enums::NO_INPUT;

void SetUp()
{
    MainWindow = ssGUI::Create<ssGUI::MainWindow>();
    Manager = ssGUI::Create<ssGUI::ssGUIManager>();
    TestText = MainWindow->AddChild<ssGUI::Text>();
    TestText->AddExtension<ssGUI::Extensions::Border>();
    TestText->SetSize(glm::vec2(250, 250));
    TestText->SetBlockInput(true);      //By default, text doesn't block any inputs 
                                        //which can't do text selection even when text selection is allowed. 
    
    TestText->SetText("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor "\
    "incididunt ut labore et dolore magna aliqua.\nJust\ta\tsentence\tfor\ttesting\ttabs.\na\ta\ta\ta\ta\ta\ta");
    
    TestFont = ssGUI::Create<ssGUI::Font>();
    TestFont->LoadFromMemory((void*)ssGUI_Test_NotoSans_Bold, ssGUI_Test_NotoSans_Bold_size);
}

void CleanUp()
{
    ssGUI::Dispose(Manager);
    ssGUI::Dispose(TestFont);
}

void Instructions()
{
    ssLOG_SIMPLE("");
    ssLOG_SIMPLE("Press 1 to call WrappingModeTest");
    ssLOG_SIMPLE("Press 2 to call AlignmentTest");
    ssLOG_SIMPLE("Press 3 to call FontTest");
    ssLOG_SIMPLE("Press 4 to call PaddingTest");
    ssLOG_SIMPLE("Press 5 to call SpaceTest");
    ssLOG_SIMPLE("Press 6 to call TabSizeTest");
    ssLOG_SIMPLE("Press 7 to call TextSelectionTest");
    ssLOG_SIMPLE("Press 8 to call CharacterPosTest");
    ssLOG_SIMPLE("");
}

void WrappingModeTest(bool firstRun)    //1
{
    if(firstRun)
    {
        ssLOG_SIMPLE("Press 1 to go back");
        ssLOG_SIMPLE("Press 2 to GetWrappingMode");
        ssLOG_SIMPLE("Press 3 to SetWrappingMode to NO_WRAPPING");
        ssLOG_SIMPLE("Press 4 to SetWrappingMode to WORD_WRAPPING");
        ssLOG_SIMPLE("Press 5 to SetWrappingMode to CHARACTER_WRAPPING");
    }
    
    if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::TWO))
    {
        ssLOG_SIMPLE("TestText->GetWrappingMode: "<<ssGUI::Enums::TextWrappingToString(TestText->GetWrappingMode()));
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::THREE))
        TestText->SetWrappingMode(ssGUI::Enums::TextWrapping::NO_WRAPPING);
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FOUR))
        TestText->SetWrappingMode(ssGUI::Enums::TextWrapping::WORD_WRAPPING);
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FIVE))
        TestText->SetWrappingMode(ssGUI::Enums::TextWrapping::CHARACTER_WRAPPING);
}

void AlignmentTest(bool firstRun)   //2
{
    if(firstRun)
    {
        ssLOG_SIMPLE("Press 2 to go back");
        ssLOG_SIMPLE("Press 3 to GetTextHorizontalAlignment & GetTextVerticalAlignment");
        ssLOG_SIMPLE("Press 4 to cycle SetTextHorizontalAlignment");
        ssLOG_SIMPLE("Press 5 to cycle SetTextVerticalAlignment");
    }
    
    if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::THREE))
    {
        ssLOG_SIMPLE("Horizontal: "<<ssGUI::Enums::AlignmentHorizontalToString(TestText->GetTextHorizontalAlignment()));
        ssLOG_SIMPLE("Vertical: "<<ssGUI::Enums::AlignmentVerticalToString(TestText->GetTextVerticalAlignment()));
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FOUR))
    {
        auto newAlign = static_cast<ssGUI::Enums::AlignmentHorizontal>
            (((int)TestText->GetTextHorizontalAlignment() + 1 ) % ((int)ssGUI::Enums::AlignmentHorizontal::COUNT));
        
        TestText->SetTextHorizontalAlignment(newAlign);
        ssLOG_SIMPLE("New Horizontal: "<<ssGUI::Enums::AlignmentHorizontalToString(newAlign));
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FIVE))
    {
        auto newAlign = static_cast<ssGUI::Enums::AlignmentVertical>
            (((int)TestText->GetTextVerticalAlignment() + 1 ) % ((int)ssGUI::Enums::AlignmentVertical::COUNT));
        
        TestText->SetTextVerticalAlignment(newAlign);
        ssLOG_SIMPLE("New Vertical: "<<ssGUI::Enums::AlignmentVerticalToString(newAlign));
    }
}

int CheckFontExists(ssGUI::Font* font)
{
    for(int i = 0; i < TestText->GetFontsCount(); i++)
    {
        if(TestText->GetFont(i) == font)
        {
            ssLOG_SIMPLE("Font already exists");
            return i;
        }
    }
    return -1;
}

void FontTest(bool firstRun)    //3
{
    if(firstRun)
    {
        ssLOG_SIMPLE("Press 3 to go back");
        ssLOG_SIMPLE("Press 4 to AddFont");
        ssLOG_SIMPLE("Press 5 to AddFont with index 0");
        ssLOG_SIMPLE("Press 6 to GetFont with index 0");
        ssLOG_SIMPLE("Press 7 to SetFont with index 0");
        ssLOG_SIMPLE("Press 8 to RemoveFont with index 0");
        ssLOG_SIMPLE("Press 9 to GetFontsCount");
    }
    
    if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FOUR))
    {
        if(CheckFontExists(TestFont) < 0)
        {
            TestText->AddFont(TestFont);
        }
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FIVE))
    {
        if(CheckFontExists(TestFont) < 0)
        {
            TestText->AddFont(TestFont, 0);
        }
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::SIX))
    {
        ssLOG_SIMPLE("GetFont: "<<TestText->GetFont(0));
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::SEVEN))
    {
        TestText->SetFont(TestFont, 0);
        ssLOG_SIMPLE("SetFont called");
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::EIGHT))
    {
        TestText->RemoveFont(0);
        ssLOG_SIMPLE("RemoveFont called");
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::NINE))
    {
        ssLOG_SIMPLE("GetFontsCount: "<<TestText->GetFontsCount());
    }
}

void PaddingTest(bool firstRun) //4
{
    if(firstRun)
    {
        ssLOG_SIMPLE("Press 4 to go back");
        ssLOG_SIMPLE("Press 5 to GetTextHorizontalPadding & GetTextVerticalPadding");
        ssLOG_SIMPLE("Press 6 to SetTextHorizontalPadding to 20");
        ssLOG_SIMPLE("Press 7 to SetTextVerticalPadding to 20");
    }
    
    if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FIVE))
    {
        ssLOG_SIMPLE("GetTextHorizontalPadding: "<<TestText->GetTextHorizontalPadding());    
        ssLOG_SIMPLE("GetTextVerticalPadding: "<<TestText->GetTextVerticalPadding());    
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::SIX))
    {
        TestText->SetTextHorizontalPadding(20);    
        ssLOG_SIMPLE("SetTextHorizontalPadding called");
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::SEVEN))
    {
        TestText->SetTextVerticalPadding(20);
        ssLOG_SIMPLE("SetTextVerticalPadding called");
    }
}

void SpaceTest(bool firstRun)   //5
{
    if(firstRun)
    {
        ssLOG_SIMPLE("Press 5 to go back");
        ssLOG_SIMPLE("Press 6 to GetCharacterSpace & GetTextVerticalPadding");
        ssLOG_SIMPLE("Press 7 to SetCharacterSpace to 5");
        ssLOG_SIMPLE("Press 8 to SetLineSpace to 20");
    }
    
    if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::SIX))
    {
        ssLOG_SIMPLE("GetCharacterSpace: "<<TestText->GetCharacterSpace());    
        ssLOG_SIMPLE("GetTextVerticalPadding: "<<TestText->GetTextVerticalPadding());    
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::SEVEN))
    {
        TestText->SetCharacterSpace(5);
        ssLOG_SIMPLE("SetCharacterSpace called");   
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::EIGHT))
    {
        TestText->SetLineSpace(20);
        ssLOG_SIMPLE("SetLineSpace called");   
    }
}

void TabSizeTest(bool firstRun) //6
{
    if(firstRun)
    {
        ssLOG_SIMPLE("Press 6 to go back");
        ssLOG_SIMPLE("Press 7 to SetTabSize to 10");
        ssLOG_SIMPLE("Press 8 to GetTabSize");
    }
    
    if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::SEVEN))
    {
        TestText->SetTabSize(10);
        ssLOG_SIMPLE("SetTabSize called");
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::EIGHT))
    {
        ssLOG_SIMPLE("GetTabSize: "<<TestText->GetTabSize());
    }
}

void TextSelectionTest(bool firstRun)   //7
{
    if(firstRun)
    {
        ssLOG_SIMPLE("Press 7 to go back");
        ssLOG_SIMPLE("Press 8 to cycle SetTextSelectionAllowed");
        ssLOG_SIMPLE("Press 9 to IsTextSelectionAllowed");
        ssLOG_SIMPLE("Press 0 to SetStartSelectionIndex(5) & SetEndSelectionIndex(10)");
        ssLOG_SIMPLE("Press Q to GetStartSelectionIndex & GetEndSelectionIndex");
        ssLOG_SIMPLE("Press W to SetSelectionColor to Red");
        ssLOG_SIMPLE("Press E to GetSelectionColor");
        ssLOG_SIMPLE("Press R to SetTextSelectedColor to Green");
        ssLOG_SIMPLE("Press T to GetTextSelectedColor");
    }
    
    if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::EIGHT))
    {
        TestText->SetTextSelectionAllowed(!TestText->IsTextSelectionAllowed());
        ssLOG_SIMPLE("SetTextSelectionAllowed: "<<TestText->IsTextSelectionAllowed());
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::NINE))
    {
        ssLOG_SIMPLE("IsTextSelectionAllowed called: "<<TestText->IsTextSelectionAllowed());
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::ZERO))
    {
        TestText->SetStartSelectionIndex(5);
        TestText->SetEndSelectionIndex(10);
        ssLOG_SIMPLE("SetStartSelectionIndex(5) called");
        ssLOG_SIMPLE("SetEndSelectionIndex(10) called");
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::LetterKey::Q))
    {
        ssLOG_SIMPLE("GetStartSelectionIndex called: "<<TestText->GetStartSelectionIndex());
        ssLOG_SIMPLE("GetEndSelectionIndex called: "<<TestText->GetEndSelectionIndex());
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::LetterKey::W))
    {
        TestText->SetSelectionColor(glm::u8vec4(255, 127, 127, 255));
        ssLOG_SIMPLE("SetSelectionColor called");
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::LetterKey::E))
    {
        auto selectionColor = TestText->GetSelectionColor();
        ssLOG_SIMPLE("GetSelectionColor called: "<<(int)selectionColor.r<<", "<<(int)selectionColor.g<<", "<<(int)selectionColor.b<<", "<<(int)selectionColor.a);
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::LetterKey::R))
    {
        TestText->SetTextSelectedColor(glm::u8vec4(127, 255, 127, 255));
        ssLOG_SIMPLE("SetTextSelectedColor called");
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::LetterKey::T))
    {
        auto testColor = TestText->GetTextSelectedColor();
        ssLOG_SIMPLE("GetSelectionColor called: "<<(int)testColor.r<<", "<<(int)testColor.g<<", "<<(int)testColor.b<<", "<<(int)testColor.a);
    }
}

void CharacterPosTest(bool firstRun)    //8
{
    if(firstRun)
    {
        ssLOG_SIMPLE("Press 8 to go back");
        ssLOG_SIMPLE("Press 9 to GetContainedCharacterIndexFromPos (from mouse pos)");
        ssLOG_SIMPLE("Press 0 to GetNearestCharacterIndexFromPos (from mouse pos)");
        ssLOG_SIMPLE("Press Q to GetFirstValidCharacterIndex & GetLastValidCharacterIndex");
        ssLOG_SIMPLE("Press W to IsPosAfterLastCharacter (from mouse pos)");
    }
    
    if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::NINE))
    {
        ssLOG_SIMPLE("GetContainedCharacterIndexFromPos: "<<
            TestText->GetContainedCharacterIndexFromPos(Manager->GetMousePosition(MainWindow)));
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::ZERO))
    {
        ssLOG_SIMPLE("GetNearestCharacterIndexFromPos: "<<
            TestText->GetNearestCharacterIndexFromPos(Manager->GetMousePosition(MainWindow), false));
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::LetterKey::Q))
    {
        ssLOG_SIMPLE("GetFirstValidCharacterIndex: "<<TestText->GetFirstValidCharacterIndex());
        ssLOG_SIMPLE("GetLastValidCharacterIndex: "<<TestText->GetLastValidCharacterIndex());
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::LetterKey::W))
    {
        ssLOG_SIMPLE("IsPosAfterLastCharacter: "<<TestText->IsPosAfterLastCharacter(Manager->GetMousePosition(MainWindow)));
    }
}

int main()
{
    Instructions();
    
    SetUp();
    
    Manager->AddRootGUIObject(MainWindow);
    
    Manager->AddPostGUIUpdateEventListener
    (
        [&]()
        {
            ssGUI::Enums::GenericButtonAndKeyInput lastTestMode = TestMode;
            if( TestMode == ssGUI::Enums::NO_INPUT &&
                !Manager->GetBackendInputInterface()->GetCurrentButtonAndKeyPresses().empty() &&
                Manager->GetBackendInputInterface()->GetLastButtonAndKeyPresses().empty())
            {
                TestMode = Manager->GetBackendInputInterface()->GetCurrentButtonAndKeyPresses()[0];
            }
            
            ssGUI::Enums::GenericButtonAndKeyInput selectedTestMode = TestMode;
            
            #define QUICK_CAST(x) static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(x)
            
            switch(TestMode)
            {
                //Call tests depending on mode
                case QUICK_CAST(ssGUI::Enums::NumberKey::ONE):
                    WrappingModeTest(lastTestMode == ssGUI::Enums::NO_INPUT);
                    if(lastTestMode != ssGUI::Enums::NO_INPUT && Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::ONE))
                        TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::TWO):
                    AlignmentTest(lastTestMode == ssGUI::Enums::NO_INPUT);
                    if(lastTestMode != ssGUI::Enums::NO_INPUT && Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::TWO))
                        TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::THREE):
                    FontTest(lastTestMode == ssGUI::Enums::NO_INPUT);
                    if(lastTestMode != ssGUI::Enums::NO_INPUT && Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::THREE))
                        TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::FOUR):
                    PaddingTest(lastTestMode == ssGUI::Enums::NO_INPUT);
                    if(lastTestMode != ssGUI::Enums::NO_INPUT && Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FOUR))
                        TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::FIVE):
                    SpaceTest(lastTestMode == ssGUI::Enums::NO_INPUT);
                    if(lastTestMode != ssGUI::Enums::NO_INPUT && Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FIVE))
                        TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::SIX):
                    TabSizeTest(lastTestMode == ssGUI::Enums::NO_INPUT);
                    if(lastTestMode != ssGUI::Enums::NO_INPUT && Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::SIX))
                        TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::SEVEN):
                    TextSelectionTest(lastTestMode == ssGUI::Enums::NO_INPUT);
                    if(lastTestMode != ssGUI::Enums::NO_INPUT && Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::SEVEN))
                        TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::EIGHT):
                    CharacterPosTest(lastTestMode == ssGUI::Enums::NO_INPUT);
                    if(lastTestMode != ssGUI::Enums::NO_INPUT && Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::EIGHT))
                        TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                default:
                    TestMode = ssGUI::Enums::NO_INPUT;
                    return;
            }
            
            if(selectedTestMode != ssGUI::Enums::NO_INPUT && TestMode == ssGUI::Enums::NO_INPUT)
                Instructions();
        }
    );
    
    Manager->StartRunning();
    
    CleanUp();   
}