#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

ssGUI::MainWindow* MainWindow;
ssGUI::Button* TestButton;
ssGUI::ssGUIManager* Manager;

void SetUp()
{
    MainWindow = ssGUI::Factory::Create<ssGUI::MainWindow>();
    TestButton = ssGUI::Factory::Create<ssGUI::Button>();
    Manager = ssGUI::Factory::Create<ssGUI::ssGUIManager>();
    auto* ecb = TestButton->GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED);
    ecb->AddEventListener(  "TestKey",    
                            [](ssGUI::EventInfo& info)
                            {
                                ssGUI::Enums::ButtonState state =
                                    static_cast<ssGUI::Button*>(info.Container)->GetButtonState();
                                
                                ssLOG_SIMPLE("State: "<<ssGUI::Enums::ToString(state));
                            });    
    
    TestButton->SetPosition(glm::vec2(50, 50));
    TestButton->SetParent(MainWindow);
    
    Manager->AddRootGUIObject(MainWindow);
    Manager->AddPostGUIUpdateEventListener([&]()
    {
        auto* inputInterface = Manager->GetBackendInputInterface();
        if( !inputInterface->IsInputExistLastFrame(ssGUI::Enums::NumberKey::ONE) &&
            inputInterface->IsInputExistCurrentFrame(ssGUI::Enums::NumberKey::ONE))
        {
            TestButton->SetInteractable(!TestButton->IsInteractable());
        }
    });
}

void CleanUp()
{
    ssGUI::Factory::Dispose(Manager);
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
    
    Manager->StartRunning();
    
    CleanUp();   
}
