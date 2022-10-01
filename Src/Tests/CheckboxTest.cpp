#include "ssGUI/DebugAndBuild/ssGUIDebugInit.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/GUIObjectClasses/Checkbox.hpp"

int main()
{
    ssGUI::MainWindow mainWindow;

    ssGUI::Checkbox checkbox;
    checkbox.SetParent(&mainWindow);
    checkbox.SetPosition(glm::vec2(100, 100));
    ssLOG_LINE("checkbox.GetSize(): "<<checkbox.GetSize().x<<", "<<checkbox.GetSize().y)

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}