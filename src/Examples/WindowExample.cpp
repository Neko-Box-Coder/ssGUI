#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"

//Window example
int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating window
    ssGUI::Window window;
    window.SetSize(glm::vec2(150, 150));
    window.SetParent(&mainWindow);

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}
