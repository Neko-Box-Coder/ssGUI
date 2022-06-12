#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/DebugAndBuild/ssGUIDebugInit.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"


//Text test
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    //mainWindow.SetResizeType(ssGUI::Enums::ResizeType::NONE);
    //mainWindow.SetSize(glm::ivec2(500, 150));

    //Loading the font
    //ssGUI::Font font;
    //if(font.GetBackendFontInterface()->LoadFromPath("Resources/NotoSans-Regular.ttf"))
    // if(font.GetBackendFontInterface()->LoadFromPath("Resources/SourceHanSansJP-Normal.otf"))
    //    std::cout<<"font loaded\n";

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetPosition(glm::ivec2(75, 25));
    text.SetSize(glm::ivec2(500, 300));
    text.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Border>());
    //text.SetFont(&font);
    text.SetFontSize(20);
    text.SetTextUnderlined(true);
    text.SetTextColor(glm::u8vec4(255, 0, 0, 255));
    // text.SetText(L"これは非常に長い日本語の文章です~~~~~");
    text.SetText(L"Click on the button to change this text.");

    //Add the text and button widget to the main window
    text.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}