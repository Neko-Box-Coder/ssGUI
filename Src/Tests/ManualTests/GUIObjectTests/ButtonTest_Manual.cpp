#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssLogger/ssLog.hpp"

ssGUI::MainWindow* MainWindow = nullptr;
ssGUI::ssGUIManager* Manager = nullptr;
ssGUI::Button* TestButton = nullptr;
ssGUI::Button* clonedButton = nullptr;

ssGUI::Enums::GenericButtonAndKeyInput TestMode = ssGUI::Enums::NO_INPUT;

void SetUp()
{
    MainWindow = ssGUI::Create<ssGUI::MainWindow>();
    Manager = ssGUI::Create<ssGUI::ssGUIManager>();
    TestButton = MainWindow->AddChild<ssGUI::Button>();
}

void CleanUp()
{
    ssGUI::Factory::Dispose(Manager);
}

void Instructions()
{
    ssLOG_SIMPLE("");
    ssLOG_SIMPLE("Press 1 to call NotifyButtonEventCallbackManuallyTest");
    ssLOG_SIMPLE("Press 2 to call GetButtonStateTest");
    ssLOG_SIMPLE("Press 3 to call ButtonColorTest");
    ssLOG_SIMPLE("Press 4 to call GetTypeTest");
    ssLOG_SIMPLE("Press 5 to call SetInteractableTest");
    ssLOG_SIMPLE("Press 6 to call CloneTest");
    ssLOG_SIMPLE("");
}

void NotifyButtonEventCallbackManuallyTest()    //1
{
    bool called = false;
    TestButton->GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED)->AddEventListener
    (
        "Key",
        [&called](ssGUI::EventInfo info)
        {
            called = true;
        }
    );
    
    TestButton->NotifyButtonEventCallbackManually();
    
    if(called)
    {
        ssLOG_SIMPLE("NotifyButtonEventCallbackManually working");
    }
    else
    {
        ssLOG_SIMPLE("NotifyButtonEventCallbackManually failed")
    }
    
    TestButton->GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED)->RemoveEventListener("Key");
}

void GetButtonStateTest(bool firstRun)  //2
{
    if(firstRun)
    {
        ssLOG_SIMPLE("Press 2 to go back");
        ssLOG_SIMPLE("Press 3 to add listener for printering button state");
        ssLOG_SIMPLE("Press 4 to remove listener for printering button state");
    }

    if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::THREE))
    {
        TestButton->GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED)->AddEventListener
        (
            "PrintButtonState",
            [&](ssGUI::EventInfo info)
            {
                ssLOG_SIMPLE(ssGUI::Enums::ToString(TestButton->GetButtonState()));
            }
        );
        ssLOG_SIMPLE("Added event listener");
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FOUR))
    {
        TestButton->GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED)->RemoveEventListener("PrintButtonState");
        ssLOG_SIMPLE("Removed event listener");
    }
}

void ButtonColorTest(bool firstRun) //3
{
    if(firstRun)
    {
        ssLOG_SIMPLE("Press 3 to go back");
        ssLOG_SIMPLE("Press 4 to set button color to red");
        ssLOG_SIMPLE("Press 5 to get current button color");
    }
    
    if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FOUR))
    {
        TestButton->SetButtonColor(glm::u8vec4(255, 127, 127, 255));
        ssLOG_SIMPLE("Button color set");
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FIVE))
    {
        auto buttonColor = TestButton->GetButtonColor();
        ssLOG_SIMPLE("Button color: "<<(int)buttonColor.r<< ", "<<(int)buttonColor.g<<", "<<(int)buttonColor.b<<", "<<(int)buttonColor.a);
    }
}

void GetTypeTest()  //4
{
    if(TestButton->GetType() == (ssGUI::Enums::GUIObjectType::BUTTON | ssGUI::Enums::GUIObjectType::WIDGET))
    {
        ssLOG_SIMPLE("GetType working");
    }
    else
    {
        ssLOG_SIMPLE("GetType failed");
    }
}

void SetInteractableTest(bool firstRun) //5
{
    if(firstRun)
    {
        ssLOG_SIMPLE("Press 5 to go back");
        ssLOG_SIMPLE("Press 6 to set button Interactable to false");
        ssLOG_SIMPLE("Press 7 to set button Interactable to true");
    }
    
    if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::SIX))
    {
        TestButton->SetInteractable(false);
        ssLOG_SIMPLE("SetInteractable(false)");
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::SEVEN))
    {
        TestButton->SetInteractable(true);
        ssLOG_SIMPLE("SetInteractable(true)");
    }
}

void CloneTest()    //6
{
    if(clonedButton == nullptr)
    {
        clonedButton = TestButton->Clone(true);
        clonedButton->SetPosition(clonedButton->GetPosition() + glm::vec2(50, 50));
    }
    else
    {
        ssLOG_SIMPLE("Button is already cloned");
    }
}

int main()
{
    Instructions();
    
    SetUp();
    
    Manager->AddRootGUIObject(MainWindow);
    
    Manager->AddPostGUIUpdateEventListener
    (
        [&]()
        {
            ssGUI::Enums::GenericButtonAndKeyInput lastTestMode = TestMode;
            if( TestMode == ssGUI::Enums::NO_INPUT &&
                !Manager->GetBackendInputInterface()->GetCurrentButtonAndKeyPresses().empty() &&
                Manager->GetBackendInputInterface()->GetLastButtonAndKeyPresses().empty())
            {
                TestMode = Manager->GetBackendInputInterface()->GetCurrentButtonAndKeyPresses()[0];
            }
            
            ssGUI::Enums::GenericButtonAndKeyInput selectedTestMode = TestMode;
            
            #define QUICK_CAST(x) static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(x)
            
            switch(TestMode)
            {
                case QUICK_CAST(ssGUI::Enums::NumberKey::ONE):
                    NotifyButtonEventCallbackManuallyTest();
                    TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::TWO):
                    GetButtonStateTest(lastTestMode == ssGUI::Enums::NO_INPUT);
                    if(lastTestMode != ssGUI::Enums::NO_INPUT && Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::TWO))
                        TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::THREE):
                    ButtonColorTest(lastTestMode == ssGUI::Enums::NO_INPUT);
                    if(lastTestMode != ssGUI::Enums::NO_INPUT && Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::THREE))
                        TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::FOUR):
                    GetTypeTest();
                    TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::FIVE):
                    SetInteractableTest(lastTestMode == ssGUI::Enums::NO_INPUT);
                    if(lastTestMode != ssGUI::Enums::NO_INPUT && Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FIVE))
                        TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::SIX):
                    CloneTest();
                    TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                default:
                    TestMode = ssGUI::Enums::NO_INPUT;
                    return;
            }
            
            if(selectedTestMode != ssGUI::Enums::NO_INPUT && TestMode == ssGUI::Enums::NO_INPUT)
                Instructions();
        }
    );
    
    Manager->StartRunning();
    
    CleanUp();   
}

