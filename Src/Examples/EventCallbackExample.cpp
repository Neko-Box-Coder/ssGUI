#include "ssGUI/EventCallbacks/RecursiveChildAddedEventCallback.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"


//Event callback example
int main()
{
    ssGUI::MainWindow mainWindow;

    auto callback = mainWindow.AddEventCallback<ssGUI::EventCallbacks::RecursiveChildAddedEventCallback>();
    callback->AddEventListener
    (
        "AnyKey",
        [](ssGUI::EventInfo info) 
        {
            std::cout<<"child added\n";
        }
    );

    //Creating window
    ssGUI::Window window;
    window.SetSize(glm::vec2(400, 300));
    window.SetPosition(glm::vec2(50, 200));
    window.SetParent(&mainWindow);     //This should trigger the event callback

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}
