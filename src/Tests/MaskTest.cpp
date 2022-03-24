#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/Extensions/Mask.hpp"


//Mask test
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::ivec2(800, 800));

    ssGUI::Window window;
    window.SetSize(glm::ivec2(400, 400));
    window.AddExtension(new ssGUI::Extensions::Mask());

    ssGUI::Window window2;
    window2.SetSize(glm::ivec2(200, 200));
    window2.AddExtension(new ssGUI::Extensions::Mask());

    ssGUI::Window window3;
    window3.SetSize(glm::ivec2(100, 100));
    window3.AddExtension(new ssGUI::Extensions::Mask());

    window.SetParent(&mainWindow);
    window2.SetParent(&window);
    window3.SetParent(&window2);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}
