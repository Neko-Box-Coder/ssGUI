#include "ssTest.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/EventCallbacks/OnObjectDestroyEventCallback.hpp"

ssGUI::EventCallbacks::OnObjectDestroyEventCallback* callback = nullptr;
ssGUI::GUIObject* testObj = nullptr;
int listerNum = 0;

ssTEST_INIT();

ssTEST_SET_UP
{
    callback = ssGUI::Factory::Create<ssGUI::EventCallbacks::OnObjectDestroyEventCallback>();
    testObj = ssGUI::Factory::Create<ssGUI::GUIObject>();
    
    //Timing is making sure the listener is triggered **before** the event
    callback->AddEventListener( "key", [&](ssGUI::EventInfo info)
                                {
                                    ssTEST_OUTPUT_ASSERT("Timing", !testObj->Internal_IsDeleted());
                                    listerNum += 1;
                                });
    callback->Clone(testObj, true);
}

ssTEST_CLEAN_UP
{
    ssGUI::Factory::Dispose(callback);
    ssGUI::Factory::Dispose(testObj);
}

ssTEST("GetEventCallbackNameTest")
{
    ssTEST_OUTPUT_ASSERT(callback->GetEventCallbackName() == "OnObjectDestroyEvent");
}

ssTEST("EventTest")
{
    testObj->Delete();

    ssTEST_OUTPUT_ASSERT("Event listener", listerNum == 1);   
    ssTEST_OUTPUT_ASSERT(testObj->Internal_IsDeleted());
}

ssTEST_END();