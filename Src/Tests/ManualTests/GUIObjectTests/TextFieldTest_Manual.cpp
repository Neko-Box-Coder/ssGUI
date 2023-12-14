#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"
#include <cassert>

ssGUI::MainWindow* MainWindow = nullptr;
ssGUI::ssGUIManager* Manager = nullptr;
ssGUI::TextField* TestTextField = nullptr;

ssGUI::Enums::GenericInput TestMode = ssGUI::Enums::NO_INPUT;

void SetUp()
{
    MainWindow = ssGUI::Create<ssGUI::MainWindow>();
    Manager = ssGUI::Create<ssGUI::ssGUIManager>();
    TestTextField = MainWindow->AddChild<ssGUI::TextField>();
    TestTextField->SetSize(glm::vec2(200, 200));

    assert(TestTextField->GetType() == (ssGUI::Enums::GUIObjectType::WIDGET | ssGUI::Enums::GUIObjectType::TEXT | ssGUI::Enums::GUIObjectType::TEXT_FIELD));
    auto* clone = TestTextField->Clone(true);
    clone->SetPosition(glm::vec2(250, 250));
    clone->SetSize(glm::vec2(200, 35));
}

void CleanUp()
{
    ssGUI::Dispose(Manager);
}

void Instructions()
{
    ssLOG_SIMPLE("");
    ssLOG_SIMPLE("No instructions needed, just need to make sure it works properly");
    ssLOG_SIMPLE("");
}

int main()
{
    Instructions();
    
    SetUp();
    
    Manager->AddRootGUIObject(MainWindow);
    
    Manager->StartRunning();
    
    CleanUp();   
}