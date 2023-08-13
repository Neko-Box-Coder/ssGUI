#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/Dropdown.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

ssGUI::ssGUIManager* Manager;
ssGUI::MainWindow* MainWindow;
ssGUI::Dropdown* TestDropdown;

void SetUp()
{
    Manager = ssGUI::Factory::Create<ssGUI::ssGUIManager>();
    MainWindow = ssGUI::Factory::Create<ssGUI::MainWindow>();
    TestDropdown = ssGUI::Factory::Create<ssGUI::Dropdown>();

    TestDropdown->SetParent(MainWindow);
    TestDropdown->SetPosition(glm::vec2(50, 50));
    TestDropdown->AddItem("Test");
    TestDropdown->AddItem("Test2");
    TestDropdown->AddItem("Test3");
    
    auto* ecb = TestDropdown->AddEventCallback(ssGUI::Enums::EventType::ITEM_SELECTED);
    ecb->AddEventListener(  "key", 
                            [](ssGUI::EventInfo& info)
                            {
                                assert(info.Container == info.EventSource);
                                assert(info.Container->GetType() == ssGUI::Enums::GUIObjectType::DROPDOWN);
                                auto* curDropdown = static_cast<ssGUI::Dropdown*>(info.Container);
                                ssLOG_SIMPLE("Selected text: "<<curDropdown->GetItem(curDropdown->GetSelectedIndex()));
                            });
    
    Manager->AddRootGUIObject(MainWindow);
}

void CleanUp()
{
    ssGUI::Factory::Dispose(Manager);
}

void Instructions()
{
    ssLOG_SIMPLE("Use mouse to interact with dropdown");
    ssLOG_SIMPLE("You should be able to see selected text from console");
}

int main()
{
    Instructions();
    
    SetUp();

    Manager->StartRunning();
    
    CleanUp();   
}
