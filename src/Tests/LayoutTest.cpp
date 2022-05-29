
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"


//Layout example
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(700, 650));

    ssGUI::Window window;
    window.SetSize(glm::vec2(400, 400));
    window.SetParent(&mainWindow);
    auto layout = ssGUI::Factory::Create<ssGUI::Extensions::Layout>();
    layout->SetHorizontalLayout(true);
    // layout->SetReverseOrder(true);
    window.AddExtension(layout);
    window.SetBackgroundColor(glm::u8vec4(180, 180, 180, 255));
    
    ssGUI::Window window2;
    window2.SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
    window2.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));
    ssGUI::Window window3;
    window3.SetTitlebarColor(glm::u8vec4(127, 255, 127, 255));
    window3.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));
    ssGUI::Window window4;
    window4.SetTitlebarColor(glm::u8vec4(127, 127, 255, 255));
    window4.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));
    window4.SetMaxSize(glm::vec2(450, 450));
    ssGUI::Window window5;
    window4.SetTitlebarColor(glm::u8vec4(127, 255, 255, 255));
    window4.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));

    ssGUI::Window window6;
    window6.SetTitlebarColor(glm::u8vec4(127, 127, 127, 255));
    window6.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));
    window6.SetSize(glm::vec2(300, 300));

    window2.SetParent(&window);
    window3.SetParent(&window);
    window4.SetParent(&window);
    window5.SetParent(&window);

    window6.SetParent(&window);

    ssGUI::Button deleteButton;
    deleteButton.SetPosition(glm::vec2(400, 400));
    auto buttonCallback = deleteButton.GetEventCallback(ssGUI::EventCallbacks::ButtonStateChangedEventCallback::EVENT_NAME);

    buttonCallback->AddEventListener
    (
        [&](ssGUI::GUIObject*src, ssGUI::GUIObject*container, ssGUI::ObjectsReferences*references)
        {
            if(deleteButton.GetButtonState() == ssGUI::Enums::ButtonState::ON_CLICK)
            {
                window5.Delete();
            }
        }
    );
    deleteButton.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}
