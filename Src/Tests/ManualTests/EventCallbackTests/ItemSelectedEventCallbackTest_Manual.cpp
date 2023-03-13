#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/Dropdown.hpp"
#include "ssLogger/ssLog.hpp"

ssGUI::ssGUIManager* manager;
ssGUI::MainWindow* mainWindow;
ssGUI::Dropdown* dropdown;

void SetUp()
{
    manager = ssGUI::Factory::Create<ssGUI::ssGUIManager>();
    mainWindow = ssGUI::Factory::Create<ssGUI::MainWindow>();
    dropdown = ssGUI::Factory::Create<ssGUI::Dropdown>();

    dropdown->SetParent(mainWindow);
    dropdown->SetPosition(glm::vec2(50, 50));
    dropdown->AddItem("Test");
    dropdown->AddItem("Test2");
    dropdown->AddItem("Test3");
    
    auto* ecb = dropdown->AddEventCallback(ssGUI::Enums::EventType::ITEM_SELECTED);
    ecb->AddEventListener(  "key", 
                            [](ssGUI::EventInfo info)
                            {
                                assert(info.Container == info.EventSource);
                                assert(info.Container->GetType() == ssGUI::Enums::GUIObjectType::DROPDOWN);
                                auto* curDropdown = static_cast<ssGUI::Dropdown*>(info.Container);
                                ssLOG_SIMPLE("Selected text: "<<curDropdown->GetItem(curDropdown->GetSelectedIndex()));
                            });
    
    manager->AddRootGUIObject(mainWindow);
}

void CleanUp()
{
    ssGUI::Factory::Dispose(manager);
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

    manager->StartRunning();
    
    CleanUp();   
}
