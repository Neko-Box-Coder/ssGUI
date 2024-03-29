#include "ssGUI/HeaderGroups/StandardGroup.hpp"


//Introduction Example
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetResizeType(ssGUI::Enums::ResizeType::NONE);
    mainWindow.SetSize(glm::vec2(500, 150));

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetPosition(glm::vec2(75, 25));
    text.SetNewTextFontSize(20);
    text.SetText(L"Click on the button to change this text.");

    //Create a button and set an event callback to change the text when it is clicked
    ssGUI::Button button;
    button.SetSize(glm::vec2(50, 30));
    button.SetPosition(glm::vec2(225, 85));
    button.GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED)->AddEventListener
    (
        "AnyKey",
        [&](ssGUI::EventInfo& info)
        {
            if(((ssGUI::Button*)info.EventSource)->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
            {
                text.SetText(L"Button pressed and this text has changed.");
            }
        }
    );

    //Add the text and button widget to the main window
    text.SetParent(&mainWindow);
    button.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}
