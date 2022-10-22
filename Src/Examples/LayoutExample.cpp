#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardWindow.hpp"
#include "ssGUI/Extensions/Docker.hpp"

//Layout example
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(450, 650));

    ssGUI::Window window;
    window.SetSize(glm::vec2(300, 600));
    window.SetParent(&mainWindow);
    window.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Layout>());
    window.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Docker>());
    window.SetBackgroundColor(glm::u8vec4(180, 180, 180, 255));
    
    ssGUI::StandardWindow window2;
    window2.SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
    window2.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));
    ssGUI::StandardWindow window3;
    window3.SetTitlebarColor(glm::u8vec4(127, 255, 127, 255));
    window3.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));
    ssGUI::StandardWindow window4;
    window4.SetTitlebarColor(glm::u8vec4(127, 127, 255, 255));
    window4.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));

    window2.SetParent(&window);
    window3.SetParent(&window);
    window4.SetParent(&window);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}
