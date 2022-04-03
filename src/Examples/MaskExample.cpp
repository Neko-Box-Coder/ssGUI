#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/Extensions/Mask.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardWindow.hpp"

//Mask test
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(800, 800));

    ssGUI::StandardWindow window;
    window.SetSize(glm::vec2(400, 400));
    window.AddExtension(new ssGUI::Extensions::Mask());

    ssGUI::Window window2;
    window2.SetSize(glm::vec2(200, 200));
    window2.AddExtension(new ssGUI::Extensions::Mask());
    window2.SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));

    ssGUI::Window window3;
    window3.SetSize(glm::vec2(100, 100));
    window3.AddExtension(new ssGUI::Extensions::Mask());
    window3.SetTitlebarColor(glm::u8vec4(127, 127, 255, 255));

    window.SetParent(&mainWindow);
    window2.SetParent(&window);
    window3.SetParent(&window2);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}
