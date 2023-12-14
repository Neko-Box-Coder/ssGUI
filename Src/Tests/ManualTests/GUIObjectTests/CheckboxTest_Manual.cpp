#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

ssGUI::MainWindow* MainWindow = nullptr;
ssGUI::ssGUIManager* Manager = nullptr;
ssGUI::Checkbox* TestCheckbox = nullptr;
ssGUI::Checkbox* ClonedCheckbox = nullptr;

ssGUI::Enums::GenericInput TestMode = ssGUI::Enums::NO_INPUT;

void SetUp()
{
    MainWindow = ssGUI::Create<ssGUI::MainWindow>();
    Manager = ssGUI::Create<ssGUI::ssGUIManager>();
    TestCheckbox = MainWindow->AddChild<ssGUI::Checkbox>();
}

void CleanUp()
{
    ssGUI::Factory::Dispose(Manager);
}

void Instructions()
{
    ssLOG_SIMPLE("");
    ssLOG_SIMPLE("Press 1 to call InnerBoxSpaceTest");
    ssLOG_SIMPLE("Press 2 to call CheckedTest");
    ssLOG_SIMPLE("Press 3 to call GetTypeTest");
    ssLOG_SIMPLE("Press 4 to call CloneTest");
    ssLOG_SIMPLE("");
}

void InnerBoxSpaceTest(bool firstRun)   //1
{
    if(firstRun)
    {
        ssLOG_SIMPLE("Press 1 to go back");
        ssLOG_SIMPLE("Press 2 to SetInnerBoxSpace to 15");
        ssLOG_SIMPLE("Press 3 to GetInnerBoxSpace");
    }
    
    if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::TWO))
    {
        TestCheckbox->SetInnerBoxSpace(15);
        ssLOG_SIMPLE("InnerBoxSpace Set");
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::THREE))
    {
        ssLOG_SIMPLE("GetInnerBoxSpace: "<< TestCheckbox->GetInnerBoxSpace());
    }
}

void CheckedTest(bool firstRun) //2
{
    if(firstRun)
    {
        ssLOG_SIMPLE("Press 2 to go back");
        ssLOG_SIMPLE("Press 3 to SetChecked(true)");
        ssLOG_SIMPLE("Press 4 to SetChecked(false)");
        ssLOG_SIMPLE("Press 5 to IsChecked");
    }

    if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::THREE))
    {
        TestCheckbox->SetChecked(true);
        ssLOG_SIMPLE("SetChecked(true)");
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FOUR))
    {
        TestCheckbox->SetChecked(false);
        ssLOG_SIMPLE("SetChecked(false)");
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FIVE))
    {
        ssLOG_SIMPLE("IsChecked: "<<TestCheckbox->IsChecked());
    }
}

void GetTypeTest()  //3
{
    if(TestCheckbox->GetType() == (ssGUI::Enums::GUIObjectType::BUTTON | ssGUI::Enums::GUIObjectType::WIDGET))
    {
        ssLOG_SIMPLE("GetType working");
    }
    else
    {
        ssLOG_SIMPLE("GetType failed");
    }
}

void CloneTest()    //4
{
    if(ClonedCheckbox == nullptr)
    {
        ClonedCheckbox = TestCheckbox->Clone(true);
        ClonedCheckbox->SetPosition(ClonedCheckbox->GetPosition() + glm::vec2(50, 50));
    }
    else
    {
        ssLOG_SIMPLE("Checkbox is already cloned");
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
            ssGUI::Enums::GenericInput lastTestMode = TestMode;
            if( TestMode == ssGUI::Enums::NO_INPUT &&
                !Manager->GetBackendInputInterface()->GetCurrentButtonAndKeyPresses().empty() &&
                Manager->GetBackendInputInterface()->GetLastButtonAndKeyPresses().empty())
            {
                TestMode = Manager->GetBackendInputInterface()->GetCurrentButtonAndKeyPresses()[0];
            }
            
            ssGUI::Enums::GenericInput selectedTestMode = TestMode;
            
            #define QUICK_CAST(x) static_cast<ssGUI::Enums::GenericInput>(x)
            
            switch(TestMode)
            {
                case QUICK_CAST(ssGUI::Enums::NumberKey::ONE):
                    InnerBoxSpaceTest(lastTestMode == ssGUI::Enums::NO_INPUT);
                    if(lastTestMode != ssGUI::Enums::NO_INPUT && Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::ONE))
                        TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::TWO):
                    CheckedTest(lastTestMode == ssGUI::Enums::NO_INPUT);
                    if(lastTestMode != ssGUI::Enums::NO_INPUT && Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::TWO))
                        TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::THREE):
                    GetTypeTest();
                    TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::FOUR):
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