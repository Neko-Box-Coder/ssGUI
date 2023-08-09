#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/Extensions/Dockable.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"

using namespace ssGUI::Enums;
int main()
{
    ssGUI::MainWindow mainWindow;

    //Create a standard window, just a fancier window
    auto* window = mainWindow.AddChild<ssGUI::StandardWindow>();
    
    //We don't need docking
    window->RemoveAnyExtension<ssGUI::Extensions::Dockable>();
    window->SetRenderSize(glm::ivec2(450, 80));
    window->AddExtension<ssGUI::Extensions::Layout>();
    
    auto* text = window->AddChild<ssGUI::Text>();
    text->SetText("Click on the button to show the message");
    
    auto* button = window->AddChildWithWrapper<ssGUI::StandardButton>(AlignmentHorizontal::CENTER, AlignmentVertical::CENTER);
    button->SetSize(glm::vec2(50, 30));
    
    //First we get the event callback for button state changed, it is attached to buttons by default.
    auto* buttonEventCallback =  button->GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED);
    
    //Then we add the text GUI Object to be referenced in the listener, 
    //and pass the reference ID to the listener lambda
    ssGUI::ssGUIObjectIndex textId = buttonEventCallback->AddObjectReference(text);
    buttonEventCallback->AddEventListener
    (
        "listenerKey",
        [textId](ssGUI::EventInfo& info)
        {
            auto* textToChange = info.References->GetObjectReference<ssGUI::Text>(textId);
            auto* currentButton = static_cast<ssGUI::StandardButton*>(info.Container);
            if(currentButton->GetButtonState() == ButtonState::CLICKED)
                textToChange->SetText(L"(`oωo´)");
        }
    );

    //A button for cloning the window
    auto* cloneButton = mainWindow.AddChild<ssGUI::StandardButton>();
    cloneButton->GetButtonTextObject()->SetText("Clone!!");

    //We create an create an extension for positioning the button easily
    auto posExt = cloneButton->AddExtension<ssGUI::Extensions::AdvancedPosition>();
    
    //Setting it to be 20 pixels from the bottom of the parent
    posExt->SetVerticalAlignment(AlignmentVertical::BOTTOM);
    posExt->SetVerticalPixel(20);

    ssGUI::ssGUIManager guiManager;
    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.AddPostGUIUpdateEventListener                                    
    (
        [&]()
        {
            //If the button is clicked, we want to clone the window
            if(cloneButton->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
                window->Clone(true);
        }
    );
    guiManager.StartRunning();
    return 0;
}