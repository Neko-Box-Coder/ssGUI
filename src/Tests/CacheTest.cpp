#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"

#define SSGUI_CACHE_TEST

int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(800, 800));

    ssGUI::Window window;
    window.SetSize(glm::vec2(200, 200));
    ssGUI::Scrollbar scrollbar;

    ssGUI::Window window2;
    window2.SetSize(glm::vec2(200, 200));
    ssGUI::Text text;
    text.SetText("Test text");

    window.SetParent(&mainWindow);
    window2.SetParent(&mainWindow);

    scrollbar.SetParent(&window);
    text.SetParent(&window2);


    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}
