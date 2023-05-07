#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"


//Readme example
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(450, 125));

    //Adds a layout extension to the main window to allow GUI objects to be stacked next to each other
    ssGUI::Extensions::Layout* layoutExtension = mainWindow.AddExtension<ssGUI::Extensions::Layout>();
    layoutExtension->SetHorizontalLayout(false);
    layoutExtension->SetSpacing(10);

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetText("Click on the button to change this text.");
    text.SetTextHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
    text.SetTextVerticalAlignment(ssGUI::Enums::AlignmentVertical::BOTTOM);

    //Create a button and set an event callback to change the text when it is clicked
    ssGUI::Button button;
    button.SetSize(glm::vec2(50, 30));
    button.GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED)->AddEventListener
    (
        "AnyKey",
        [&](ssGUI::EventInfo info)
        {
            if(((ssGUI::Button*)info.EventSource)->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
            {
                text.SetText("Button pressed and this text has changed.");
            }
        }
    );

    //AdvancedPosition extension allows a GUI Object to be centered.
    ssGUI::Extensions::AdvancedPosition* positionExtension = button.AddExtension<ssGUI::Extensions::AdvancedPosition>();
    positionExtension->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
    positionExtension->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::TOP);    

    //Wraps the button inside an empty widget for consistence size under layout extension
    ssGUI::Widget emptyWidget;
    button.SetParent(&emptyWidget);

    //Add the text and button widget (wrapped inside empty widget) to the main window
    text.SetParent(&mainWindow);
    emptyWidget.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}
