#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssLogger/ssLog.hpp"

ssGUI::MainWindow* MainWindow = nullptr;
ssGUI::ssGUIManager* Manager = nullptr;
ssGUI::Menu* TestMenu = nullptr;

ssGUI::Enums::GenericButtonAndKeyInput TestMode = ssGUI::Enums::NO_INPUT;

void SetUp()
{
    MainWindow = ssGUI::Create<ssGUI::MainWindow>();
    Manager = ssGUI::Create<ssGUI::ssGUIManager>();
    TestMenu = MainWindow->AddChild<ssGUI::Menu>();
    MainWindow->RegisterRightClickMenu(TestMenu);
}

void CleanUp()
{
    ssGUI::Factory::Dispose(Manager);
}

void Instructions()
{
    ssLOG_SIMPLE("");
    ssLOG_SIMPLE("Press 1 to call MenuSpawnDirectionTest");
    ssLOG_SIMPLE("Press 2 to call SpawnMenuTest");
    ssLOG_SIMPLE("Press 3 to call CanSpawnMenuTest");
    ssLOG_SIMPLE("Press 4 to call ForceSpawnMenuTest");
    ssLOG_SIMPLE("Press 5 to call RegisterMenuItemTest");
    ssLOG_SIMPLE("Press 6 to call RemoveMenuItemTest");
    ssLOG_SIMPLE("Press 7 to call AddMenuItemTest");
    ssLOG_SIMPLE("Press 8 to call LinkMenuItemToSubMenuTest");
    ssLOG_SIMPLE("Press 9 to call GetMenuTargetTest");
    ssLOG_SIMPLE("");
}

void PrintCursorPosition()
{
    ssLOG_SIMPLE("position: "<<Manager->GetMousePosition(MainWindow).x<<", "<<Manager->GetMousePosition(MainWindow).y);
}

void MenuSpawnDirectionTest()   //1
{
    auto direction = TestMenu->GetMenuSpawnDirection(Manager->GetMousePosition(MainWindow));
    ssLOG_SIMPLE("direction: "<<ssGUI::Enums::MenuSpawnDirectionToString(direction));
    PrintCursorPosition();
}

void SpawnMenuTest()    //2
{
    TestMenu->SpawnMenu(Manager->GetMousePosition(MainWindow));
    ssLOG_SIMPLE("menu spawned");
    PrintCursorPosition();
}

void CanSpawnMenuTest(bool firstRun)    //3
{
    if(firstRun)
    {
        ssLOG_SIMPLE("Press 3 to go back");
        ssLOG_SIMPLE("Press 4 to check CanSpawnMenu with TOP_LEFT direction");
        ssLOG_SIMPLE("Press 5 to check CanSpawnMenu with TOP_RIGHT direction");
        ssLOG_SIMPLE("Press 6 to check CanSpawnMenu with BOTTOM_RIGHT direction");
        ssLOG_SIMPLE("Press 7 to check CanSpawnMenu with BOTTOM_LEFT direction");
    }
    
    if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FOUR))
    {
        bool canSpawn = TestMenu->CanSpawnMenu(Manager->GetMousePosition(MainWindow), ssGUI::Enums::MenuSpawnDirection::TOP_LEFT);
        PrintCursorPosition();
        ssLOG_SIMPLE("Can force spawn with TOP_LEFT direction: "<<canSpawn);
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FIVE))
    {
        bool canSpawn = TestMenu->CanSpawnMenu(Manager->GetMousePosition(MainWindow), ssGUI::Enums::MenuSpawnDirection::TOP_RIGHT);
        PrintCursorPosition();
        ssLOG_SIMPLE("Can force spawn with TOP_RIGHT direction: "<<canSpawn);

    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::SIX))
    {
        bool canSpawn = TestMenu->CanSpawnMenu(Manager->GetMousePosition(MainWindow), ssGUI::Enums::MenuSpawnDirection::BOTTOM_RIGHT);
        PrintCursorPosition();
        ssLOG_SIMPLE("Can force spawn with BOTTOM_RIGHT direction: "<<canSpawn);
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::SEVEN))
    {
        bool canSpawn = TestMenu->CanSpawnMenu(Manager->GetMousePosition(MainWindow), ssGUI::Enums::MenuSpawnDirection::BOTTOM_LEFT);
        PrintCursorPosition();
        ssLOG_SIMPLE("Can force spawn with BOTTOM_LEFT direction: "<<canSpawn);
    }
}

void ForceSpawnMenuTest(bool firstRun)  //4
{
    if(firstRun)
    {
        ssLOG_SIMPLE("Press 4 to go back");
        ssLOG_SIMPLE("Press 5 to ForceSpawnMenu with TOP_LEFT direction");
        ssLOG_SIMPLE("Press 6 to ForceSpawnMenu with TOP_RIGHT direction");
        ssLOG_SIMPLE("Press 7 to ForceSpawnMenu with BOTTOM_RIGHT direction");
        ssLOG_SIMPLE("Press 8 to ForceSpawnMenu with BOTTOM_LEFT direction");
    }
    
    if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FIVE))
    {
        TestMenu->ForceSpawnMenu(Manager->GetMousePosition(MainWindow), ssGUI::Enums::MenuSpawnDirection::TOP_LEFT);
        PrintCursorPosition();
        ssLOG_SIMPLE("Force spawn with TOP_LEFT direction");
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::SIX))
    {
        TestMenu->ForceSpawnMenu(Manager->GetMousePosition(MainWindow), ssGUI::Enums::MenuSpawnDirection::TOP_RIGHT);
        PrintCursorPosition();
        ssLOG_SIMPLE("Force spawn with TOP_RIGHT direction");

    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::SEVEN))
    {
        TestMenu->ForceSpawnMenu(Manager->GetMousePosition(MainWindow), ssGUI::Enums::MenuSpawnDirection::BOTTOM_RIGHT);
        PrintCursorPosition();
        ssLOG_SIMPLE("Force spawn with BOTTOM_RIGHT direction");
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::EIGHT))
    {
        TestMenu->ForceSpawnMenu(Manager->GetMousePosition(MainWindow), ssGUI::Enums::MenuSpawnDirection::BOTTOM_LEFT);
        PrintCursorPosition();
        ssLOG_SIMPLE("Force spawn with BOTTOM_LEFT direction");
    }   
}

void RegisterMenuItemTest() //5
{
    auto* menuItem = TestMenu->AddChild<ssGUI::MenuItem>();
    menuItem->GetButtonTextObject()->SetText("Item " + std::to_string(TestMenu->GetListOfMenuItems().size()));
    TestMenu->RegisterMenuItem(menuItem);
    ssLOG_SIMPLE("RegisterMenuItem with text: Item "<<(TestMenu->GetListOfMenuItems().size()));
}

void RemoveMenuItemTest()   //6
{
    if(TestMenu->GetListOfMenuItems().size() == 0)
    {
        ssLOG_SIMPLE("There's no menu item");
        return;
    }

    TestMenu->RemoveMenuItem(dynamic_cast<ssGUI::MenuItem*>(TestMenu->GetListOfMenuItems().back()));
    ssLOG_SIMPLE("Last menu item removed");
}

void AddMenuItemTest()  //7
{
    auto* menuItem = TestMenu->AddMenuItem();
    menuItem->GetButtonTextObject()->SetText("Item " + std::to_string(TestMenu->GetListOfMenuItems().size()));
    ssLOG_SIMPLE("AddMenuItem with text: Item "<<(TestMenu->GetListOfMenuItems().size()));
}

void LinkMenuItemToSubMenuTest()    //8
{
    if(TestMenu->GetListOfMenuItems().size() == 0)
    {
        ssLOG_SIMPLE("There's no menu item");
        return;
    }

    auto* subMenu = TestMenu->AddChild<ssGUI::Menu>();
    for(int i = 0; i < 5; i++)
    {
        auto* subMenuItem = subMenu->AddChild<ssGUI::MenuItem>();
        subMenuItem->GetButtonTextObject()->SetText("Sub menu item " + std::to_string(i));
        subMenu->RegisterMenuItem(subMenuItem);
    }
    
    TestMenu->LinkMenuItemToSubMenu(dynamic_cast<ssGUI::MenuItem*>(TestMenu->GetListOfMenuItems().back()), subMenu);
    ssLOG_SIMPLE("Created sub menu and linked to the last menu item");
}

void GetMenuTargetTest()   //9
{
    if(TestMenu->GetMenuTarget() == MainWindow)
    {
        ssLOG_SIMPLE("GetMenuTarget working");
    }
    else
    {
        ssLOG_SIMPLE("GetMenuTarget failed");
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
                    MenuSpawnDirectionTest();
                    TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::TWO):
                    SpawnMenuTest();
                    TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::THREE):
                    CanSpawnMenuTest(lastTestMode == ssGUI::Enums::NO_INPUT);
                    if(lastTestMode != ssGUI::Enums::NO_INPUT && Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::THREE))
                        TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::FOUR):
                    ForceSpawnMenuTest(lastTestMode == ssGUI::Enums::NO_INPUT);
                    if(lastTestMode != ssGUI::Enums::NO_INPUT && Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FOUR))
                        TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::FIVE):
                    RegisterMenuItemTest();
                    TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::SIX):
                    RemoveMenuItemTest();
                    TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::SEVEN):
                    AddMenuItemTest();
                    TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::EIGHT):
                    LinkMenuItemToSubMenuTest();
                    TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::NINE):
                    GetMenuTargetTest();
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