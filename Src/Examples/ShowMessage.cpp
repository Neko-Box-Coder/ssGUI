#include "ssGUI/HeaderGroups/StandardGroup.hpp"                     //Includes all the core ssGUI classes
#include "ssGUI/Extensions/AdvancedPosition.hpp"

//Readme example
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(450, 125));

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetText(L"Click on the button to show the message");
    text.SetTextHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
    text.SetTextVerticalAlignment(ssGUI::Enums::AlignmentVertical::BOTTOM);
    
    //AdvancedPosition extension allows more option to position a GUI Object. By default it will center the GUI object.
    text.AddExtension<ssGUI::Extensions::AdvancedPosition>()->SetVerticalPixel(-30);
    
    //Create a button and set an event callback to change the text when it is clicked
    ssGUI::Button button;
    button.SetSize(glm::vec2(50, 30));
    button.GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED)->AddEventListener
    (
        "AnyKey",
        [&](ssGUI::EventInfo& info)
        {
            if(((ssGUI::Button*)info.EventSource)->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
            {
                text.SetText(L"(`oωo´)");
            }
        }
    );

    //Clone the extension for the button widget
    button.AddExtensionCopy(text.GetExtension<ssGUI::Extensions::AdvancedPosition>())->SetVerticalPixel(20);

    //Add the text and button widget to the main window
    text.SetParent(&mainWindow);
    button.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}