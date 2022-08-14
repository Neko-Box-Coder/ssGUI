#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"


//Text font test
int main()
{
    ssGUI::MainWindow mainWindow;

    //Loading the font
    ssGUI::Font font;
    ssGUI::Font font2;
    if(font.GetBackendFontInterface()->LoadFromPath("Resources/NotoSans-Regular.ttf"))
       std::cout<<"font loaded\n";
    // if(font2.GetBackendFontInterface()->LoadFromPath("Resources/SourceHanSansJP-Normal.otf"))
    //    std::cout<<"font loaded\n";

    if(font2.GetBackendFontInterface()->LoadFromPath("Resources/NotoSans-Bold.ttf"))
       std::cout<<"font loaded\n";

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetPosition(glm::ivec2(75, 25));
    text.SetSize(glm::ivec2(300, 150));
    text.AddFont(&font);
    text.AddFont(&font2);
    // ssGUI::Text::AddDefaultFont(&font);
    text.SetNewCharacterFontSize(20);
    text.SetWrappingMode(ssGUI::Enums::TextWrapping::NO_WRAPPING);
    // text.SetWrappingMode(ssGUI::Enums::TextWrapping::CHARACTER_WRAPPING);
    // text.SetText(L"これは非常に長い日本語の文章です~~~~~");

    // text.SetText(L"This is a very long test sentence!!!!!");

    std::wstring s = L"This is a very long test \nsentence!!!!!";

    for(int i = 0; i < s.size(); i++)
    {
        ssGUI::CharacterDetails details;

        details.Character = s[i];
        
        if(i >= 10 && i <= 13)
        {
            details.CharacterColor = glm::u8vec4(0, 0, 255, 255);
            details.FontIndex = 1;  //Bold
            details.FontSize = 20;
            details.Underlined = true;
        }
        else if(i >= 20 && i <= 23)
        {
            details.FontSize = 50;
            // details.FontSize = 20;
            details.Underlined = true;
        }
        else
        {
            details.FontIndex = 0;
            details.FontSize = 20;
            details.Underlined = true;
        }
        
        text.AddCurrentCharacterDetails(details);
    }

    text.ComputeCharactersPositionAndSize();


    DEBUG_LINE("overflow: "<<text.IsOverflow());

    // text.SetText("Test text");

    DEBUG_LINE("Character supp: "<<font.IsCharacterSupported(L'は'));

    DEBUG_LINE(text.GetCharacterRenderInfo(0).RenderPosition.y);

    text.SetParent(&mainWindow);

    text.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Border>());

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;

    guiManager.AddPreGUIUpdateEventListener
    (
        [&]()
        {
            if(guiManager.GetBackendInputInterface()->GetCurrentKeyPresses().IsSymbolKeyPresent(ssGUI::Enums::SymbolKey::EQUAL) &&
                !guiManager.GetBackendInputInterface()->GetLastKeyPresses().IsSymbolKeyPresent(ssGUI::Enums::SymbolKey::EQUAL))
            {
                for(int i = 0; i < text.GetCurrentCharactersDetailsCount(); i++)
                {
                    auto curDetails = text.GetCurrentCharacterDetails(i);
                    curDetails.FontSize = curDetails.FontSize+1;
                    text.SetCurrentCharacterDetails(i, curDetails);
                }
            }
            else if(guiManager.GetBackendInputInterface()->GetCurrentKeyPresses().IsSymbolKeyPresent(ssGUI::Enums::SymbolKey::MINUS) &&
                    !guiManager.GetBackendInputInterface()->GetLastKeyPresses().IsSymbolKeyPresent(ssGUI::Enums::SymbolKey::MINUS))
            {
                for(int i = 0; i < text.GetCurrentCharactersDetailsCount(); i++)
                {
                    auto curDetails = text.GetCurrentCharacterDetails(i);
                    curDetails.FontSize = curDetails.FontSize-1;
                    text.SetCurrentCharacterDetails(i, curDetails);
                }
            }
        }
    );

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}
