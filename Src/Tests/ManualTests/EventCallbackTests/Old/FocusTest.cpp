#include "ssGUI/EventCallbacks/FocusLostEventCallback.hpp"
#include "ssGUI/EventCallbacks/FocusedEventCallback.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"


int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(800, 800));


    ssGUI::Window window;
    window.SetSize(glm::vec2(200, 200));
    auto focusEvent = ssGUI::Factory::Create<ssGUI::EventCallbacks::FocusedEventCallback>();
    focusEvent->AddEventListener
    (
        "test",
        [](ssGUI::EventInfo info)
        {
            info.EventCallbackContainer->SetBackgroundColor(glm::u8vec4(255, 0, 0, 255));
        }
    );
    
    auto focusLostEvent = ssGUI::Factory::Create<ssGUI::EventCallbacks::FocusLostEventCallback>();
    focusLostEvent->AddEventListener
    (
        "test",
        [](ssGUI::EventInfo info)
        {
            info.EventCallbackContainer->SetBackgroundColor(glm::u8vec4(255, 255, 255, 255));
        }
    );

    window.AddEventCallback(focusEvent);
    window.AddEventCallback(focusLostEvent);

    ssGUI::Window* window2 = window.Clone(true);
    window2->SetSize(glm::vec2(100, 100));
    window2->SetTitlebarColor(glm::u8vec4(255, 255, 0, 255));
    
    ssGUI::Window* window3 = window.Clone(true);
    window3->SetSize(glm::vec2(100, 100));
    window3->SetTitlebarColor(glm::u8vec4(0, 255, 0, 255));
    
    ssGUI::Window* window4 = window.Clone(true);
    window4->SetSize(glm::vec2(100, 100));
    window4->SetTitlebarColor(glm::u8vec4(0, 0, 255, 255));
    
    ssGUI::Window* window5 = window.Clone(true);
    window5->SetSize(glm::vec2(100, 100));
    window5->SetTitlebarColor(glm::u8vec4(0, 255, 255, 255));

    window.SetParent(&mainWindow);
    window2->SetParent(&window);
    window3->SetParent(&window);
    window4->SetParent(window2);
    window5->SetParent(window3);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}
