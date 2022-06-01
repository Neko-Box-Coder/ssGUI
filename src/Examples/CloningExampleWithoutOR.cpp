#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"

int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::ivec2(700, 300));

    //Create a window
    ssGUI::Window window;
    window.SetSize(glm::ivec2(450, 110 + window.GetTitlebarHeight()));      //The reason why adding titlebar height is because Window includes titlebar while MainWindow doesn't
    window.SetResizeType(ssGUI::Enums::ResizeType::NONE);
    window.SetParent(&mainWindow);

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetSize(glm::vec2(450, 45));
    text.SetText(L"Click on the button to show the message");
    text.SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::CENTER);
    text.SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::BOTTOM);
    text.SetParent(&window);
    
    //Create a button and set an event callback to change the text when it is clicked
    ssGUI::Button button;
    button.SetSize(glm::ivec2(50, 30));
    button.SetPosition(glm::vec2(200, 60));
    button.SetParent(&window);

    //Gets the event callback for the button state change
    auto ecb = button.GetAnyEventCallback<ssGUI::EventCallbacks::ButtonStateChangedEventCallback>();
    ecb->AddEventListener
    (
        "AnyKey",
        [&](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* refs)
        {
            if(((ssGUI::Button*)src)->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
                text.SetText(L"(`oωo´)");
        }
    );

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    //When the user presses Enter, clone the window
    guiManager.AddOnUpdateEventListener
    (
        [&]()
        {
            ssGUI::Backend::BackendSystemInputInterface* inputInterface = guiManager.GetBackendInputInterface();
            
            if(inputInterface->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER) &&
                !inputInterface->GetLastKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER))
            {
                window.Clone(true)->SetPosition(glm::vec2(window.GetPosition().x + 10, window.GetPosition().y + 10));
            }
        }
    );    

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}