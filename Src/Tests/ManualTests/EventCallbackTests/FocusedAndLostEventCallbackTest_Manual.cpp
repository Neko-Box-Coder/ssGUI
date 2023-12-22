#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"


ssGUI::MainWindow* MainWindow;
ssGUI::Window* TestWindow;
ssGUI::Window* TestWindow2;
ssGUI::Widget* TestWidget;
ssGUI::Widget* TestWidget2;

ssGUI::ssGUIManager* Manager;


void SetUp()
{
    MainWindow = ssGUI::Factory::Create<ssGUI::MainWindow>();
    TestWindow = ssGUI::Factory::Create<ssGUI::Window>();
    TestWindow2 = ssGUI::Factory::Create<ssGUI::Window>();
    TestWidget = ssGUI::Factory::Create<ssGUI::Widget>();
    TestWidget2 = ssGUI::Factory::Create<ssGUI::Widget>();
    Manager = ssGUI::Factory::Create<ssGUI::ssGUIManager>();
    
    auto objToPtr = [&](void* ptr)
    {
        if(ptr == TestWindow)
            return "window";
        else if(ptr == TestWindow2)
            return "window2";
        else if(ptr == TestWidget)
            return "widget";
        else if(ptr == TestWidget2)
            return "widget2";
        else
            return "unknown";
    };
    
    auto* ecb = TestWindow->AddEventCallback(ssGUI::Enums::EventType::FOCUSED);
    auto* ecb2 = TestWindow->AddEventCallback(ssGUI::Enums::EventType::FOCUSED);
    ecb->AddEventListener(  "TestKey",    
                            [&](ssGUI::EventInfo& info)
                            {
                                ssLOG_SIMPLE("Focused: "<<objToPtr(info.EventSource));
                                assert(info.EventSource->IsFocused());
                            });
                            
    ecb2->AddEventListener(  "TestKey",    
                            [&](ssGUI::EventInfo& info)
                            {
                                ssLOG_SIMPLE("Focus lost: "<<objToPtr(info.EventSource));
                                assert(!info.EventSource->IsFocused());
                            });
    
    TestWindow->SetBackgroundColor(glm::u8vec4(255, 0, 0, 255));
    TestWindow2->SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));
    TestWidget->SetBackgroundColor(glm::u8vec4(0, 255, 0, 255));
    TestWidget2->SetBackgroundColor(glm::u8vec4(0, 0, 255, 255));
    
    TestWindow2->AddEventCallbackCopy(ecb, true);
    TestWindow2->AddEventCallbackCopy(ecb2, true);
    TestWidget->AddEventCallbackCopy(ecb, true);
    TestWidget->AddEventCallbackCopy(ecb2, true);
    TestWidget2->AddEventCallbackCopy(ecb, true);
    TestWidget2->AddEventCallbackCopy(ecb2, true);

    TestWindow->SetParent(MainWindow);
    TestWindow2->SetParent(MainWindow);
    TestWidget->SetParent(MainWindow);
    TestWidget2->SetParent(TestWindow2);

    ssLOG_SIMPLE("window: Red");
    ssLOG_SIMPLE("window2: Grey");
    ssLOG_SIMPLE("widget: Green");
    ssLOG_SIMPLE("widget2: Blue");
    ssLOG_SIMPLE("");
    ssLOG_SIMPLE("");

    Manager->AddRootGUIObject(MainWindow);
    Manager->AddPostGUIUpdateEventListener([&]()
    {
        auto* inputInterface = Manager->GetBackendInputInterface();
        if( !inputInterface->IsInputExistLastFrame(ssGUI::Enums::NumberKey::ONE) &&
            inputInterface->IsInputExistCurrentFrame(ssGUI::Enums::NumberKey::ONE))
        {
            TestWindow->SetFocus(!TestWindow->IsFocused());
            ssLOG_SIMPLE("window focus set to: "<<TestWindow->IsFocused());
        }
        
        if( !inputInterface->IsInputExistLastFrame(ssGUI::Enums::NumberKey::TWO) &&
            inputInterface->IsInputExistCurrentFrame(ssGUI::Enums::NumberKey::TWO))
        {
            TestWindow2->SetFocus(!TestWindow2->IsFocused());
            ssLOG_SIMPLE("window2 focus set to: "<<TestWindow2->IsFocused());
        }
        
        if( !inputInterface->IsInputExistLastFrame(ssGUI::Enums::NumberKey::THREE) &&
            inputInterface->IsInputExistCurrentFrame(ssGUI::Enums::NumberKey::THREE))
        {
            TestWidget->SetFocus(!TestWidget->IsFocused());
            ssLOG_SIMPLE("widget focus set to: "<<TestWidget->IsFocused());
        }
        
        if( !inputInterface->IsInputExistLastFrame(ssGUI::Enums::NumberKey::FOUR) &&
            inputInterface->IsInputExistCurrentFrame(ssGUI::Enums::NumberKey::FOUR))
        {
            TestWidget2->SetFocus(!TestWidget2->IsFocused());
            ssLOG_SIMPLE("widget2 focus set to: "<<TestWidget2->IsFocused());
        }
    });
}

void CleanUp()
{
    ssGUI::Factory::Dispose(Manager);
}

void Instructions()
{
    ssLOG_SIMPLE("Use mouse to interact with windows and widgets");
    ssLOG_SIMPLE("You should be able to see focus change text in console");
    ssLOG_SIMPLE("Press 1 to toggle window focus");
    ssLOG_SIMPLE("Press 2 to toggle window2 focus");
    ssLOG_SIMPLE("Press 3 to toggle widget focus");
    ssLOG_SIMPLE("Press 4 to toggle widget2 focus");
}

int main()
{
    Instructions();
    SetUp();
    
    Manager->StartRunning();
    
    CleanUp();   
}
