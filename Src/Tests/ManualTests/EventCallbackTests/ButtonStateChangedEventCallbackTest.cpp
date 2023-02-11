#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssLogger/ssLog.hpp"

void SetUp()
{
}

void CleanUp()
{
}

void Instructions()
{
    ssLOG_SIMPLE("Press 1 to change button interactable or not");
    ssLOG_SIMPLE("Use mouse to interact with the button");
    ssLOG_SIMPLE("You should be able to see button state change text in console when interacting with the button");
}

int main()
{
    SetUp();
    
    Instructions();
    
    ssGUI::MainWindow mainWindow;
    ssGUI::Button button;
    auto* ecb = button.GetAnyEventCallback<ssGUI::EventCallbacks::ButtonStateChangedEventCallback>();
    ecb->AddEventListener(  "TestKey",    
                            [](ssGUI::EventInfo info)
                            {
                                ssGUI::Enums::ButtonState state =
                                    static_cast<ssGUI::Button*>(info.Container)->GetButtonState();
                                
                                ssLOG_SIMPLE("State: "<<ssGUI::Enums::ToString(state));
                            });    
    
    button.SetPosition(glm::vec2(50, 50));
    button.SetParent(&mainWindow);
    
    ssGUI::ssGUIManager manager;
    manager.AddGUIObject(&mainWindow);
    manager.AddPostGUIUpdateEventListener([&]()
    {
        auto* inputInterface = manager.GetBackendInputInterface();
        if( !inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::ONE) &&
            inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::ONE))
        {
            button.SetInteractable(!button.IsInteractable());
        }
    });
    
    manager.StartRunning();
    
    CleanUp();   
}
