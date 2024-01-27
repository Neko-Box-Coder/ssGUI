#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Dockable.hpp"
#include "ssGUI/Extensions/Layout.hpp"

int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::ivec2(700, 300));

    //Create a window
    ssGUI::StandardWindow window;
    window.RemoveExtension<ssGUI::Extensions::Dockable>();           //We don't need docking
    window.SetSize(glm::vec2(150, 150));
    window.SetParent(&mainWindow);

    //Add automatic layout to the window
    auto layout = ssGUI::Factory::Create<ssGUI::Extensions::Layout>();
    layout->AddPreferredSizeMultiplier(0.4);
    layout->AddPreferredSizeMultiplier(0.6);
    window.AddExtension(layout);

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetNewTextFontSize(15);
    text.SetNewTextColor(glm::u8vec4(255, 255, 255, 255));
    text.SetText("Click button");
    text.SetTextHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
    text.SetTextVerticalAlignment(ssGUI::Enums::AlignmentVertical::BOTTOM);
    text.SetParent(&window);
    
    //Add wrapper for button so the layout extension is not directly sizing it
    ssGUI::GUIObject wrapper;
    wrapper.SetParent(&window);

    //Create a button and set an event callback to change the text when it is clicked
    ssGUI::StandardButton button;
    auto ap = ssGUI::Factory::Create<ssGUI::Extensions::AdvancedPosition>();
    button.AddExtension(ap);
    button.SetParent(&wrapper);

    //By default, button has state changed event callback attached to it
    //Gets the event callback for the button state change
    auto ecb = button.GetAnyEventCallback<ssGUI::EventCallbacks::ButtonStateChangedEventCallback>();

    //Adds the text widget parented to the window to the event callback so it can be referenced
    //<AddObjectReference> return a <ssGUIObjectIndex> which is just an ID you can use to retrieve the text widget back
    ssGUI::ssGUIObjectIndex textIndex = ecb->AddObjectReference(&text);
    ecb->AddEventListener
    (
        "AnyKey",
        [textIndex](ssGUI::EventInfo& info)
        {
            //When the button is clicked, sets the text
            if(static_cast<ssGUI::Button*>(info.Container)->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
            {
                ssGUI::Text* text = static_cast<ssGUI::Text*>(info.References->GetObjectReference(textIndex));
                text->SetText(L"(`oωo´)");
            }
        }
    );

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    //When the user presses Enter, clone the window
    guiManager.AddPostGUIUpdateEventListener
    (
        [&]()
        {
            ssGUI::Backend::SystemInputInterface* inputInterface = guiManager.GetBackendInputInterface();
            
            if( inputInterface->IsInputExistCurrentFrame(ssGUI::Enums::SystemKey::ENTER) &&
                !inputInterface->IsInputExistLastFrame(ssGUI::Enums::SystemKey::ENTER))
            {
                window.Clone(true)->SetPosition(glm::vec2(window.GetPosition().x + 10, window.GetPosition().y + 10));
            }
        }
    );    

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}