#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/Dockable.hpp"


//Clone test 2
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::ivec2(450, 125));
    // mainWindow.GetBackendWindowInterface()->SetMSAA(8);

    //AdvancedPosition extension allows more option to position a GUI Object. By default it will center the GUI object.
    auto positionExtension = ssGUI::Factory::Create<ssGUI::Extensions::AdvancedPosition>();
    positionExtension->SetVerticalUsePercentage(false);

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetText(L"Click on the button to show the message");
    text.SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::CENTER);
    text.SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::BOTTOM);
    positionExtension->SetVerticalPixel(-30);
    text.AddExtension(positionExtension);
    
    //Create a button and set an event callback to change the text when it is clicked
    ssGUI::Button button;
    button.SetSize(glm::ivec2(50, 30));
    ssGUI::ssGUIObjectIndex textIndex = 
        button.GetEventCallback(ssGUI::EventCallbacks::ButtonStateChangedEventCallback::EVENT_NAME)->AddObjectReference(&text);
    button.GetEventCallback(ssGUI::EventCallbacks::ButtonStateChangedEventCallback::EVENT_NAME)->AddEventListener
    (
        [textIndex](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* refs)
        {
            if(((ssGUI::Button*)src)->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
            {
                ssGUI::GUIObject* refText = refs->GetObjectReference(textIndex);

                if(refText != nullptr)
                    static_cast<ssGUI::Text*>(refText)->SetText(L"(`oωo´)");
            }
        }
    );
    

    //Clone the extension for the button widget
    ssGUI::Extensions::AdvancedPosition* positionExtension2 = 
        static_cast<ssGUI::Extensions::AdvancedPosition*>(positionExtension->Clone(&button));
    positionExtension2->SetVerticalPixel(20);

    ssGUI::Window window;
    window.SetSize(glm::ivec2(100, 100));
    window.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Dockable>());

    //Add the text and button widget to the main window
    text.SetParent(&window);
    button.SetParent(&window);
    window.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddOnUpdateEventListener
    (
        [&]()
        {
            ssGUI::Backend::BackendSystemInputInterface* inputInterface = guiManager.GetBackendInputInterface();
            
            if(inputInterface->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER) &&
                !inputInterface->GetLastKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER))
            {
                window.Clone(true);
            }
        }
    );    

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}