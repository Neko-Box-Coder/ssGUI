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
    
    //Adds the text widget parented to the window to the event callback so it can be referenced
    //<AddObjectReference> return a <ssGUIObjectIndex> which is just an ID you can use to retrieve the text widget back
    ssGUI::ssGUIObjectIndex textIndex = ecb->AddObjectReference(&text);
    ecb->AddEventListener
    (
        "AnyKey",
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

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    //When the user presses Enter, clone the window
    guiManager.AddPostUpdateEventListener
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