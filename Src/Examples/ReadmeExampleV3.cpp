#include "ssGUI/HeaderGroups/StandardGroup.hpp"

//Readme example
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(450, 110));
    mainWindow.SetResizeType(ssGUI::Enums::ResizeType::NONE);

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetSize(glm::vec2(450, 45));
    text.SetText("Click on the button to show the message");
    text.SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
    text.SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::BOTTOM);

    //Create a button and set an event callback to change the text when it is clicked
    ssGUI::Button button;
    button.SetSize(glm::vec2(50, 30));
    button.SetPosition(glm::vec2(200, 60));
    button.GetEventCallback(ssGUI::EventCallbacks::ButtonStateChangedEventCallback::EVENT_NAME)->AddEventListener
    (
        "AnyKey",
        [&](ssGUI::EventInfo info)
        {
            if(((ssGUI::Button*)info.EventSource)->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
                text.SetText("(`oωo´)");
        }
    );

    button.SetParent(&mainWindow);
    text.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}