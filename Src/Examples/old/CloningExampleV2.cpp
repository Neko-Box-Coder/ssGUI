#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/Extensions/Dockable.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"

using namespace ssGUI::Enums;
int main()
{
    ssGUI::MainWindow mainWindow;
    
    auto* window = mainWindow.AddChild<ssGUI::StandardWindow>();                        //Create a standard window, just a fancier window
    window->RemoveAnyExtension<ssGUI::Extensions::Dockable>();                          //We don't need docking
    window->SetRenderSize(glm::ivec2(450, 80));                                         //Render size same as before
    auto layout = window->AddExtension<ssGUI::Extensions::Layout>();                    //Add layout for auto sizing child GUI objects
    
    auto* text = window->AddChild<ssGUI::Text>();                                       //Create a text widget and set the respective properties
    text->SetNewTextFontSize(17);
    text->SetText("Click on the button to show the message");
    text->SetTextAlignment(AlignmentHorizontal::CENTER, AlignmentVertical::CENTER);         //We center the text right above the button we will be adding later
    
    auto* button = ssGUI::Create<ssGUI::StandardButton>();                              //Create a standard button, just a fancier button.
    button->SetSize(glm::vec2(50, 30));
    layout->AddChildWithAlignment(button,   AlignmentHorizontal::CENTER,                //Attach button to main window with alignment, so that the size
                                            AlignmentVertical::CENTER);                 //      stays the same and won't be changed by layout
    
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

    auto* cloneButton = mainWindow.AddChild<ssGUI::StandardButton>();                   //A button for cloning the window
    cloneButton->GetButtonTextObject()->SetText("Clone!!");                             //Text for indication
    auto posExt = cloneButton->AddExtension<ssGUI::Extensions::AdvancedPosition>();     //We create an create an extension for positioning the button easily
    posExt->SetTextVerticalAlignment(AlignmentVertical::BOTTOM);                            //By default, it centers the GUI Object relative to its parent, we are setting it bottom 
    posExt->SetVerticalPixel(20);                                                       //Setting it to be 20 pixels from the bottom of the parent   

    ssGUI::ssGUIManager guiManager;                                                     //Create the GUIManager, which manages the flow of the program.
    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);                        //Add the main window (which has both text and button parented to it)
    guiManager.AddPostGUIUpdateEventListener                                    
    (
        [&]()
        {
            if(cloneButton->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)     //If the button is clicked, we want to clone the window
                window->Clone(true);
        }
    );
    guiManager.StartRunning();                                                          //Finally we start running the program
    return 0;
}