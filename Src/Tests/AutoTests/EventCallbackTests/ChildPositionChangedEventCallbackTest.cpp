#include "ssTest.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/EventCallbacks/ChildPositionChangedEventCallback.hpp"

ssGUI::EventCallbacks::ChildPositionChangedEventCallback* callback = nullptr;
ssGUI::GUIObject* testObj = nullptr;
ssGUI::GUIObject* testObj2 = nullptr;
ssGUI::Window* testWindow = nullptr;
int listerNum = 0;

ssTEST_INIT();

ssTEST_SET_UP
{
    callback = ssGUI::Factory::Create<ssGUI::EventCallbacks::ChildPositionChangedEventCallback>();
    testObj = ssGUI::Factory::Create<ssGUI::GUIObject>();
    testObj2 = ssGUI::Factory::Create<ssGUI::GUIObject>();
    testWindow = ssGUI::Factory::Create<ssGUI::Window>();
    
    //Timing is making sure the listener is triggered **after** the event
    callback->AddEventListener( "key", [&](ssGUI::EventInfo info)
                                {
                                    testWindow->MoveChildrenIteratorToLast();
                                    ssTEST_OUTPUT_ASSERT("Timing", testWindow->GetCurrentChild() == testObj); 
                                    ssTEST_OUTPUT_ASSERT("EventSource", info.EventSource == testObj); 
                                    listerNum = 1;
                                });
    callback->Clone(testWindow, true);
}

ssTEST_CLEAN_UP
{
    ssGUI::Factory::Dispose(callback);
    ssGUI::Factory::Dispose(testObj);
    ssGUI::Factory::Dispose(testObj2);
    ssGUI::Factory::Dispose(testWindow);
}

ssTEST("GetEventCallbackNameTest")
{
    ssTEST_OUTPUT_ASSERT(callback->GetEventCallbackName() == "ChildPositionChangedEvent");
}

ssTEST("EventTest")
{
    testObj->SetParent(testWindow);
    testObj2->SetParent(testWindow);
    
    testWindow->MoveChildrenIteratorToFirst();
    ssGUI::Hierarchy::ChildToken tokenFirst = testWindow->GetCurrentChildToken();
    testWindow->MoveChildrenIteratorToLast();
    ssGUI::Hierarchy::ChildToken tokenLast = testWindow->GetCurrentChildToken();
    
    testWindow->ChangeChildOrderToAfterPosition(tokenFirst, tokenLast);
    ssTEST_OUTPUT_ASSERT(listerNum == 1);   
}

ssTEST_END();