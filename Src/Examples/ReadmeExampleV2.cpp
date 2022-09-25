#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"


//Readme example
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(450, 125));

    //Adds a layout extension to the main window to allow GUI objects to be stacked next to each other
    ssGUI::Extensions::Layout* layoutExtension = ssGUI::Factory::Create<ssGUI::Extensions::Layout>();
    layoutExtension->SetHorizontalLayout(false);
    layoutExtension->SetSpacing(10);
    mainWindow.AddExtension(layoutExtension);

    //AdvancedPosition extension allows a GUI Object to be centered.
    ssGUI::Extensions::AdvancedPosition* positionExtension = ssGUI::Factory::Create<ssGUI::Extensions::AdvancedPosition>();
    positionExtension->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
    positionExtension->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::TOP);    

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetText("Click on the button to change this text.");
    text.SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::CENTER);
    text.SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::BOTTOM);

    //Create a button and set an event callback to change the text when it is clicked
    ssGUI::Button button;
    button.SetSize(glm::vec2(50, 30));
    button.AddExtension(positionExtension);
    button.GetEventCallback(ssGUI::EventCallbacks::ButtonStateChangedEventCallback::EVENT_NAME)->AddEventListener
    (
        "AnyKey",
        [&](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* refs)
        {
            if(((ssGUI::Button*)src)->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
            {
                text.SetText("Button pressed and this text has changed.");
            }
        }
    );

    //Wraps the button inside an empty widget for consistence size under layout extension
    ssGUI::Widget emptyWidget;
    button.SetParent(&emptyWidget);

    //Add the text and button widget (wrapped inside empty widget) to the main window
    text.SetParent(&mainWindow);
    emptyWidget.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}
