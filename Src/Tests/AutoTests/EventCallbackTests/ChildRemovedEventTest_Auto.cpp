#include "ssTest.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"

ssGUI::EventCallback* Callback = nullptr;
ssGUI::GUIObject* TestObj = nullptr;
ssGUI::Window* TestWindow = nullptr;
int ListenerNum = 0;

int main()
{
    ssTEST_INIT();

    ssTEST_SET_UP
    {
        Callback = ssGUI::Factory::Create<ssGUI::EventCallback>();
        Callback->SetEventType(ssGUI::Enums::EventType::CHILD_REMOVED);
        TestObj = ssGUI::Factory::Create<ssGUI::GUIObject>();
        TestWindow = ssGUI::Factory::Create<ssGUI::Window>();
        
        //Timing is making sure the listener is triggered **after** the event
        Callback->AddEventListener( "key", [&](ssGUI::EventInfo& info)
                                    {
                                        ssTEST_OUTPUT_ASSERT("Timing", TestObj->GetParent() == nullptr); 
                                        ssTEST_OUTPUT_ASSERT("EventSource", info.EventSource == TestObj); 
                                        ListenerNum = 1;
                                    });
        TestWindow->AddEventCallbackCopy(Callback, true);
    };

    ssTEST_CLEAN_UP
    {
        ssGUI::Factory::Dispose(Callback);
        ssGUI::Factory::Dispose(TestObj);
        ssGUI::Factory::Dispose(TestWindow);
    };

    ssTEST("EventTest")
    {
        TestObj->SetParent(TestWindow);
        TestObj->SetParent(nullptr);
        
        ssTEST_OUTPUT_ASSERT(ListenerNum == 1);   
    };

    ssTEST_END();
}