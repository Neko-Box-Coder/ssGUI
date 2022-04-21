#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"


//Text font test
int main()
{
    ssGUI::MainWindow mainWindow;

    //Loading the font
    ssGUI::Font font;
    ssGUI::Font font2;
    if(font.GetBackendFontInterface()->LoadFromPath("NotoSans-Regular.ttf"))
       std::cout<<"font loaded\n";
    // if(font2.GetBackendFontInterface()->LoadFromPath("SourceHanSansJP-Normal.otf"))
    //    std::cout<<"font loaded\n";

    if(font2.GetBackendFontInterface()->LoadFromPath("NotoSans-Bold.ttf"))
       std::cout<<"font loaded\n";

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetPosition(glm::ivec2(75, 25));
    text.SetSize(glm::ivec2(300, 150));
    text.AddFont(&font);
    text.AddFont(&font2);
    // ssGUI::Text::AddDefaultFont(&font);
    text.SetFontSize(20);
    text.SetWrappingMode(ssGUI::Enums::TextWrapping::NO_WRAPPING);
    // text.SetWrappingMode(ssGUI::Enums::TextWrapping::CHARACTER_WRAPPING);
    // text.SetText(L"これは非常に長い日本語の文章です~~~~~");

    // text.SetText(L"This is a very long test sentence!!!!!");

    std::wstring s = L"This is a very long test sentence!!!!!";

    for(int i = 0; i < s.size(); i++)
    {
        ssGUI::CharacterDetails details;

        details.Character = s[i];
        
        if(i >= 10 && i <= 13)
        {
            details.CharacterColor = glm::u8vec4(0, 0, 255, 255);
            details.FontIndex = 1;
        }
        else
            details.FontIndex = 0;

        // if(i == s.size() - 7)
        //     details.FontSize = 40;
        // else
            details.FontSize = 20;
        
        text.AddOverrideCharacterDetails(details);
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
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}
