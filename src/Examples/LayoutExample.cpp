
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"


//Layout example
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(450, 650));

    ssGUI::Window window;
    window.SetSize(glm::vec2(300, 600));
    window.SetParent(&mainWindow);
    window.AddExtension(new ssGUI::Extensions::Layout());
    
    ssGUI::Window window2;
    window2.SetBackgroundColor(glm::u8vec4(200, 127, 127, 255));
    ssGUI::Window window3;
    window3.SetBackgroundColor(glm::u8vec4(127, 200, 127, 255));
    ssGUI::Window window4;
    window4.SetBackgroundColor(glm::u8vec4(127, 127, 200, 255));

    window2.SetParent(&window);
    window3.SetParent(&window);
    window4.SetParent(&window);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}
