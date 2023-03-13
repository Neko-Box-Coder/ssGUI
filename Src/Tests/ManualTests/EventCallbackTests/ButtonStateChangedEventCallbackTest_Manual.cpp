#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssLogger/ssLog.hpp"

ssGUI::MainWindow* mainWindow;
ssGUI::Button* button;
ssGUI::ssGUIManager* manager;

void SetUp()
{
    mainWindow = ssGUI::Factory::Create<ssGUI::MainWindow>();
    button = ssGUI::Factory::Create<ssGUI::Button>();
    manager = ssGUI::Factory::Create<ssGUI::ssGUIManager>();
    auto* ecb = button->GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED);
    ecb->AddEventListener(  "TestKey",    
                            [](ssGUI::EventInfo info)
                            {
                                ssGUI::Enums::ButtonState state =
                                    static_cast<ssGUI::Button*>(info.Container)->GetButtonState();
                                
                                ssLOG_SIMPLE("State: "<<ssGUI::Enums::ToString(state));
                            });    
    
    button->SetPosition(glm::vec2(50, 50));
    button->SetParent(mainWindow);
    
    manager->AddRootGUIObject(mainWindow);
    manager->AddPostGUIUpdateEventListener([&]()
    {
        auto* inputInterface = manager->GetBackendInputInterface();
        if( !inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::ONE) &&
            inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::ONE))
        {
            button->SetInteractable(!button->IsInteractable());
        }
    });
}

void CleanUp()
{
    ssGUI::Factory::Dispose(manager);
}

void Instructions()
{
    ssLOG_SIMPLE("Press 1 to change button interactable or not");
    ssLOG_SIMPLE("Use mouse to interact with the button");
    ssLOG_SIMPLE("You should be able to see button state change text in console when interacting with the button");
}

int main()
{
    Instructions();
    
    SetUp();
    
    manager->StartRunning();
    
    CleanUp();   
}
