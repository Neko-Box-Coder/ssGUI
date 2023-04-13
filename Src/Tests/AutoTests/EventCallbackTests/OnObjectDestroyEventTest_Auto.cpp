#include "ssTest.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"

ssGUI::EventCallback* Callback = nullptr;
ssGUI::GUIObject* TestObj = nullptr;
int ListenerNum = 0;

int main()
{
    ssTEST_INIT();

    ssTEST_SET_UP
    {
        Callback = ssGUI::Factory::Create<ssGUI::EventCallback>();
        Callback->SetEventType(ssGUI::Enums::EventType::BEFORE_OBJECT_DESTROY);
        TestObj = ssGUI::Factory::Create<ssGUI::GUIObject>();
        
        //Timing is making sure the listener is triggered **before** the event
        Callback->AddEventListener( "key", [&](ssGUI::EventInfo info)
                                    {
                                        ssTEST_OUTPUT_ASSERT("Timing", !TestObj->Internal_IsDeleted());
                                        ListenerNum += 1;
                                    });
        TestObj->AddEventCallbackCopy(Callback, true);
    }

    ssTEST_CLEAN_UP
    {
        ssGUI::Factory::Dispose(Callback);
        ssGUI::Factory::Dispose(TestObj);
    }

    ssTEST("EventTest")
    {
        TestObj->Delete();

        ssTEST_OUTPUT_ASSERT("Event listener", ListenerNum == 1);   
        ssTEST_OUTPUT_ASSERT(TestObj->Internal_IsDeleted());
    }

    ssTEST_END();
}