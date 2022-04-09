#include "ssGUI/DebugAndBuild/ssGUIDebugInit.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Shape.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardWindow.hpp"

// Cloning test
int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating window
    // ssGUI::StandardWindow window;
    ssGUI::Window window;
    window.SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
    window.SetSize(glm::vec2(150, 150));
    window.SetParent(&mainWindow);

    auto shape = new ssGUI::Extensions::Shape();
    // shape->AddCircle(glm::vec2(), glm::vec2(150, 150), glm::u8vec4(255, 0, 0, 255), true);
    shape->AddRectangle(glm::vec2(), glm::vec2(150, 150), glm::u8vec4(255, 0, 0, 255), true);
    window.AddExtension(shape);

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}