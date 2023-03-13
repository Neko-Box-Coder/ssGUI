#include "ssTest.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"

ssGUI::EventCallback* callback = nullptr;
ssGUI::GUIObject* testObj = nullptr;
ssGUI::Window* testWindow = nullptr;
int listerNum = 0;

ssTEST_INIT();

ssTEST_SET_UP
{
    callback = ssGUI::Factory::Create<ssGUI::EventCallback>();
    callback->SetEventType(ssGUI::Enums::EventType::CHILD_ADDED);
    testObj = ssGUI::Factory::Create<ssGUI::GUIObject>();
    testWindow = ssGUI::Factory::Create<ssGUI::Window>();
    
    //Timing is making sure the listener is triggered **after** the event
    callback->AddEventListener( "key", [&](ssGUI::EventInfo info)
                                {
                                    ssTEST_OUTPUT_ASSERT("Timing", testObj->GetParent() == testWindow); 
                                    listerNum = 1;
                                });
    testObj->AddEventCallbackCopy(callback, true);
    testWindow->AddEventCallbackCopy(callback, true);
}

ssTEST_CLEAN_UP
{
    ssGUI::Factory::Dispose(callback);
    ssGUI::Factory::Dispose(testObj);
    ssGUI::Factory::Dispose(testWindow);
}

ssTEST("EventTest")
{
    testObj->SetParent(testWindow);
    ssTEST_OUTPUT_ASSERT("GUIObject", listerNum == 1);   
}

ssTEST_END();