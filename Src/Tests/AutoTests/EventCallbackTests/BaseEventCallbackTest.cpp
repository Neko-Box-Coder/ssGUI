#include "ssTest.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"

ssGUI::EventCallbacks::BaseEventCallback* callback = nullptr;
ssGUI::GUIObject* testObj = nullptr;

ssTEST_INIT();

ssTEST_SET_UP
{
    callback = ssGUI::Factory::Create<ssGUI::EventCallbacks::BaseEventCallback>();
    testObj = ssGUI::Factory::Create<ssGUI::GUIObject>();
}

ssTEST_CLEAN_UP
{
    ssGUI::Factory::Dispose(callback);
    ssGUI::Factory::Dispose(testObj);
}

ssTEST("AddEventListenerTest")
{
    callback->AddEventListener("key", [](ssGUI::EventInfo info){});
    ssTEST_OUTPUT_ASSERT("Without adder",   callback->GetEventListenerCount() == 1 && 
                                            callback->IsEventListenerExist("key"));
    
    callback->AddEventListener("key", testObj, [](ssGUI::EventInfo info){});
    ssTEST_OUTPUT_ASSERT("With adder",  callback->GetEventListenerCount() == 2 &&
                                        callback->IsEventListenerExist("key", testObj));   
}

ssTEST_SKIP("IsEventListenerExistTest (Tested in AddEventListenerTest)"){}

ssTEST("RemoveEventListenerTest")
{
    callback->RemoveEventListener("key");
    ssTEST_OUTPUT_ASSERT("Without adder",   callback->GetEventListenerCount() == 1 && 
                                            !callback->IsEventListenerExist("key"));    

    callback->RemoveEventListener("key", testObj);
    
    ssTEST_OUTPUT_ASSERT("With adder",  callback->GetEventListenerCount() == 0 &&
                                        !callback->IsEventListenerExist("key", testObj));   
}

ssTEST("ClearEventListenersTest")
{
    callback->AddEventListener("key", [](ssGUI::EventInfo info){});
    callback->AddEventListener("key1", [](ssGUI::EventInfo info){});
    callback->AddEventListener("key2", [](ssGUI::EventInfo info){});
    callback->AddEventListener("key3", testObj, [](ssGUI::EventInfo info){});
    
    callback->ClearEventListeners();
    
    ssTEST_OUTPUT_ASSERT(callback->GetEventListenerCount() == 0);
}

ssTEST_SKIP("GetEventListenerCountTest (Tested in AddEventListenerTest)"){}

ssTEST("NotifyTest")
{
    int testNum = 0;
    callback->AddEventListener("key",   [&](ssGUI::EventInfo info)
                                        { 
                                            testNum = 1;
                                            ssTEST_OUTPUT_ASSERT("Nullptr Container", info.EventCallbackContainer == nullptr);
                                            ssTEST_OUTPUT_ASSERT("Nullptr Source", info.EventSource == nullptr);
                                        });
    callback->Notify(nullptr);
    ssTEST_OUTPUT_ASSERT(testNum == 1);

    ssGUI::GUIObject* notifySrcObj = ssGUI::Factory::Create<ssGUI::GUIObject>();
    
    auto* clonedEvent = callback->Clone(testObj, false);
    clonedEvent->AddEventListener(  "key",   
                                    [&](ssGUI::EventInfo info)
                                    { 
                                        ssTEST_OUTPUT_ASSERT(   "Container", 
                                                                info.EventCallbackContainer == testObj);
                                        ssTEST_OUTPUT_ASSERT(   "Source", 
                                                                info.EventSource == notifySrcObj);
                                    });
    
    clonedEvent->Notify(notifySrcObj);
    ssGUI::Factory::Dispose(notifySrcObj);

    ssTEST_CALL_CLEAN_UP();
    ssTEST_CALL_SET_UP();
}

//Used internally, testing is not needed
ssTEST_SKIP("BindToObjectTest")
{
};

int objRefIndex = -1;

ssTEST("AddObjectReferenceTest")
{
    objRefIndex = callback->AddObjectReference(testObj);
    ssTEST_OUTPUT_ASSERT(objRefIndex != -1);
}

ssTEST("GetObjectReferenceTest")
{
    ssTEST_OUTPUT_ASSERT(callback->GetObjectReference(objRefIndex) == testObj);
}

ssTEST("RemoveObjectReferenceTest")
{
    callback->RemoveObjectReference(objRefIndex);
    ssTEST_OUTPUT_ASSERT(callback->GetObjectReference(objRefIndex) == nullptr);
}

ssTEST("GetEventCallbackNameTest")
{
    ssTEST_OUTPUT_ASSERT(callback->GetEventCallbackName() == "BaseEvent");
}

ssTEST("CloneTest")
{
    int testNum = 0;
    std::string listenerKey = "key";
    callback->AddEventListener( listenerKey, 
                                [&](ssGUI::EventInfo info)
                                {
                                    testNum = 1;
                                });
    
    auto* clonedCallback = callback->Clone(nullptr, false);
    ssTEST_OUTPUT_ASSERT("Validity", clonedCallback != nullptr);
    if(clonedCallback == nullptr)
        return;
        
    clonedCallback = callback->Clone(nullptr, true);
    clonedCallback->Notify(nullptr);
    ssTEST_OUTPUT_ASSERT("Listener", testNum == 1);
    ssGUI::Factory::Dispose(clonedCallback);
    testNum = 0;
    
    clonedCallback = callback->Clone(testObj, true);
    ssTEST_OUTPUT_ASSERT(   "New Container", 
                            testObj->IsAnyEventCallbackExist<ssGUI::EventCallbacks::BaseEventCallback>());
}

ssTEST("EventTest")
{
    //testObj->SetBackgroundColor(glm::u8vec4(0, 0, 0, 255));
    //ssTEST_OUTPUT_ASSERT("GUIObject", listerNum == 1);
    //listerNum = 0;
    
    //callback->Clone(testWindow, true);
    //testWindow->SetBackgroundColor(glm::u8vec4(0, 0, 0, 255));
    //ssTEST_OUTPUT_ASSERT("Window", listerNum == 1);
}

ssTEST_END();