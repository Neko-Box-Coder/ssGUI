#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssLogger/ssLog.hpp"

//Text test
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetBackgroundColor(glm::u8vec4(35, 38, 41, 255));
    //mainWindow.SetMSAA(8);
    //mainWindow.SetResizeType(ssGUI::Enums::ResizeType::NONE);
    //mainWindow.SetSize(glm::ivec2(500, 150));

    //Loading the font
    //ssGUI::Font font;
    //if(font.GetFontInterface()->LoadFromPath("Resources/NotoSans-Regular.ttf"))
    // if(font.GetFontInterface()->LoadFromPath("Resources/SourceHanSansJP-Normal.otf"))
    //    std::cout<<"font loaded\n";

    ssGUI::Font font;
    if(font.GetFontInterface()->LoadFromPath("Resources/NotoColorEmoji.ttf"))
        ssLOG_LINE("font loaded");


    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetPosition(glm::ivec2(75, 25));
    text.SetSize(glm::ivec2(500, 300));
    text.AddFont(&font);
    text.SetNewTextFontSize(20);
    //text.SetNewTextUnderlined(true);
    text.SetNewTextColor(glm::u8vec4(255, 255, 255, 255));
    text.AddExtension<ssGUI::Extensions::Border>();
    // text.SetText(L"これは非常に長い日本語の文章です~~~~~");
    //text.SetText(L"Click on the button to change this text.");
    text.SetText(L"≥⚠️☢️🧪🪡💰⚙️🎲❗⛔❕☣️\n💬🐞🎉🌍📄🔌😉✅🛳️⏬⏱️🕸️❌");
    //text.SetText("≥🎲⚠️🎲☢️🎲⚙️🎲☣️🎲💰");

    //Add the text and button widget to the main window
    text.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}