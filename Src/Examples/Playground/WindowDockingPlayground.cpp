
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardWindow.hpp"
#include "ssGUI/Extensions/DockingArea.hpp"
#include "ssGUI/Extensions/DockableV2.hpp"

#include <tuple>

//Layout example
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(450, 650));

    ssGUI::Window window;
    window.SetSize(glm::vec2(300, 600));
    window.SetParent(&mainWindow);
    window.AddExtension<ssGUI::Extensions::DockingArea>();
    // window.GetExtension<ssGUI::Extensions::Layout>()->SetPadding(5);
    window.SetBackgroundColor(glm::u8vec4(180, 180, 180, 255));
    
    ssGUI::StandardWindow window2;
    window2.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));
    window2.SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
    window2.AddExtension<ssGUI::Extensions::DockableV2>();
    window2.SetPosition(window2.GetPosition() + glm::vec2(5, 5));
    
    ssGUI::StandardWindow window3;
    window3.SetTitlebarColor(glm::u8vec4(127, 255, 127, 255));
    window3.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));
    window3.AddExtension<ssGUI::Extensions::DockableV2>();
    window3.SetPosition(window2.GetPosition() + glm::vec2(5, 5));
    //window3.AddExtension<ssGUI::Extensions::Layout>();
    
    ssGUI::StandardWindow window4;
    window4.SetTitlebarColor(glm::u8vec4(127, 127, 255, 255));
    window4.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));
    window4.AddExtension<ssGUI::Extensions::DockableV2>();
    window4.SetPosition(window3.GetPosition() + glm::vec2(5, 5));

    ssGUI::StandardWindow window5;
    // window5.SetTitlebarColor(glm::u8vec4(127, 127, 127, 255));
    window5.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));
    window5.AddExtension<ssGUI::Extensions::DockableV2>();
    window5.SetPosition(window4.GetPosition() + glm::vec2(5, 5));

    window2.SetParent(&mainWindow);
    window3.SetParent(&mainWindow);
    window4.SetParent(&mainWindow);
    window5.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);
    
    guiManager.StartRunning();
    return 0;
}
