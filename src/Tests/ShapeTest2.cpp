#include "ssGUI/DebugAndBuild/ssGUIDebugInit.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Shape.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardWindow.hpp"

//This tests if RemoveGUIObjectShape works
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
    shape->AddAdditionalCircle(glm::vec2(), glm::vec2(150, 150), glm::u8vec4(0, 255, 0, 255), false);
    shape->AddAdditionalRectangle(glm::vec2(), glm::vec2(100, 100), glm::u8vec4(255, 0, 0, 255), true);
    shape->RemoveGUIObjectShape(0); //This removes window background
    shape->RemoveGUIObjectShape(1); //This removes window titlebar
    shape->RestoreGUIObjectShape(0); //This restores window background
    window.AddExtension(shape);

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}