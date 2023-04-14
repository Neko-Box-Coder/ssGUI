#include "ssTest.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"

ssGUI::Widget* TestWidget = nullptr;

int main()
{
    ssTEST_INIT();

    ssTEST_SET_UP
    {
        TestWidget = ssGUI::Create<ssGUI::Widget>();
    }

    ssTEST_CLEAN_UP
    {
        ssGUI::Dispose(TestWidget);
    }

    ssTEST("InteractableTest")
    {
        TestWidget->SetInteractable(true);
        ssTEST_OUTPUT_ASSERT("True", TestWidget->IsInteractable());
        TestWidget->SetInteractable(false);
        ssTEST_OUTPUT_ASSERT("False", !TestWidget->IsInteractable());
    }
    
    ssTEST("BlockInputTest")
    {
        TestWidget->SetBlockInput(true);
        ssTEST_OUTPUT_ASSERT("True", TestWidget->IsBlockInput());
        TestWidget->SetBlockInput(false);
        ssTEST_OUTPUT_ASSERT("False", !TestWidget->IsBlockInput());
    }
    
    ssTEST("GetTypeTest")
    {
        ssTEST_OUTPUT_ASSERT(TestWidget->GetType() == ssGUI::Enums::GUIObjectType::WIDGET);
    }
    
    ssTEST("Clone")
    {
        TestWidget->SetInteractable(true);
        TestWidget->SetBlockInput(false);
        
        auto* clonedWidget = TestWidget->Clone(true);
        ssTEST_OUTPUT_ASSERT(clonedWidget->IsInteractable() && !clonedWidget->IsBlockInput());
    }

    ssTEST_END();
}
