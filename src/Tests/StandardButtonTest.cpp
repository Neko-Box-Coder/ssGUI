#include "ssGUI/DebugAndBuild/ssGUIDebugInit.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardButton.hpp"

int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating window
    ssGUI::Window window;
    window.SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
    window.SetSize(glm::vec2(150, 150));
    window.SetParent(&mainWindow);

    ssGUI::StandardButton button;
    button.GetButtonTextObject()->SetText("Click me!!");
    button.SetPosition(glm::vec2(50, 50));
    button.SetSize(glm::vec2(125, 40));
    button.SetParent(&window);

    ssGUI::Button button2;
    button2.SetPosition(glm::vec2(50, 150));
    button2.SetSize(glm::vec2(125, 40));
    button2.SetParent(&window);


    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}