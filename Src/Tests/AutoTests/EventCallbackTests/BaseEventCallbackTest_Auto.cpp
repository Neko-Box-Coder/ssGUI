#include "ssTest.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"

ssGUI::EventCallback* Callback = nullptr;
ssGUI::GUIObject* TestObj = nullptr;

int main()
{
    ssTEST_INIT();

    ssTEST_SET_UP
    {
        Callback = ssGUI::Factory::Create<ssGUI::EventCallback>();
        Callback->SetEventType(ssGUI::Enums::EventType::NONE);
        TestObj = ssGUI::Factory::Create<ssGUI::GUIObject>();
    };

    ssTEST_CLEAN_UP
    {
        ssGUI::Factory::Dispose(Callback);
        ssGUI::Factory::Dispose(TestObj);
    };

    ssTEST_DISABLE_CLEANUP_BETWEEN_TESTS();

    ssTEST("AddEventListenerTest")
    {
        Callback->AddEventListener("key", [](ssGUI::EventInfo& info){});
        ssTEST_OUTPUT_ASSERT("Without adder",   Callback->GetEventListenerCount() == 1 && 
                                                Callback->IsEventListenerExist("key"));
        
        Callback->AddEventListener("key", TestObj, [](ssGUI::EventInfo& info){});
        ssTEST_OUTPUT_ASSERT("With adder",  Callback->GetEventListenerCount() == 2 &&
                                            Callback->IsEventListenerExist("key", TestObj));   
    };

    ssTEST_SKIP("IsEventListenerExistTest (Tested in AddEventListenerTest)"){};

    ssTEST("RemoveEventListenerTest")
    {
        Callback->RemoveEventListener("key");
        ssTEST_OUTPUT_ASSERT("Without adder",   Callback->GetEventListenerCount() == 1 && 
                                                !Callback->IsEventListenerExist("key"));    

        Callback->RemoveEventListener("key", TestObj);
        
        ssTEST_OUTPUT_ASSERT("With adder",  Callback->GetEventListenerCount() == 0 &&
                                            !Callback->IsEventListenerExist("key", TestObj));   
    };

    ssTEST("SetEventListenerOrderTest")
    {
        Callback->AddEventListener("key", [](ssGUI::EventInfo& info){});
        Callback->AddEventListener("key2", [](ssGUI::EventInfo& info){});
        
        ssTEST_OUTPUT_ASSERT("Without adder",   Callback->GetEventListenerOrder("key") == 0 && 
                                                Callback->GetEventListenerOrder("key2") == 1);    

        Callback->AddEventListener("key", TestObj, [](ssGUI::EventInfo& info){});
        Callback->AddEventListener("key2", TestObj, [](ssGUI::EventInfo& info){});

        ssTEST_OUTPUT_ASSERT("With adder",  Callback->GetEventListenerOrder("key", TestObj) == 2 && 
                                            Callback->GetEventListenerOrder("key2", TestObj) == 3);    
    };

    ssTEST_SKIP("GetEventListenerOrder (Tested in SetEventListenerOrderTest)"){};

    ssTEST("ClearEventListenersTest")
    {
        ssTEST_OUTPUT_ASSERT(Callback->GetEventListenerCount() == 4);
        
        Callback->ClearEventListeners();
        
        ssTEST_OUTPUT_ASSERT(Callback->GetEventListenerCount() == 0);
    };

    ssTEST_SKIP("GetEventListenerCountTest (Tested in AddEventListenerTest)"){};

    ssTEST("NotifyTest")
    {
        int testNum = 0;
        Callback->AddEventListener("key",   [&](ssGUI::EventInfo& info)
                                            { 
                                                testNum = 1;
                                                ssTEST_OUTPUT_ASSERT("Nullptr Container", info.Container == nullptr);
                                                ssTEST_OUTPUT_ASSERT("Nullptr Source", info.EventSource == nullptr);
                                            });
        Callback->Notify(nullptr);
        ssTEST_OUTPUT_ASSERT(testNum == 1);

        ssGUI::GUIObject* notifySrcObj = ssGUI::Factory::Create<ssGUI::GUIObject>();
        
        auto* clonedEvent = TestObj->AddEventCallbackCopy(Callback, false);
        clonedEvent->AddEventListener(  "key",   
                                        [&](ssGUI::EventInfo& info)
                                        { 
                                            ssTEST_OUTPUT_ASSERT(   "Container", 
                                                                    info.Container == TestObj);
                                            ssTEST_OUTPUT_ASSERT(   "Source", 
                                                                    info.EventSource == notifySrcObj);
                                        });
        
        clonedEvent->Notify(notifySrcObj);
        ssGUI::Factory::Dispose(notifySrcObj);

        ssTEST_CALL_CLEAN_UP();
        ssTEST_CALL_SET_UP();
    };

    //Used internally, testing is not needed
    ssTEST_SKIP("BindToObjectTest")
    {
    };

    int objRefIndex = -1;

    ssTEST("AddObjectReferenceTest")
    {
        objRefIndex = Callback->AddObjectReference(TestObj);
        ssTEST_OUTPUT_ASSERT(objRefIndex != -1);
    };

    ssTEST("GetObjectReferenceTest")
    {
        ssTEST_OUTPUT_ASSERT(Callback->GetObjectReference(objRefIndex) == TestObj);
    };

    ssTEST("RemoveObjectReferenceTest")
    {
        Callback->RemoveObjectReference(objRefIndex);
        ssTEST_OUTPUT_ASSERT(Callback->GetObjectReference(objRefIndex) == nullptr);
    };

    ssTEST("CloneTest")
    {
        int testNum = 0;
        std::string listenerKey = "key";
        Callback->AddEventListener( listenerKey, 
                                    [&](ssGUI::EventInfo& info)
                                    {
                                        testNum = 1;
                                    });
        
        auto* clonedCallback = Callback->Clone(false);
        ssTEST_OUTPUT_ASSERT("Validity", clonedCallback != nullptr);
        if(clonedCallback == nullptr)
            return;
            
        clonedCallback = Callback->Clone(true);
        clonedCallback->Notify(nullptr);
        ssTEST_OUTPUT_ASSERT("Listener", testNum == 1);
        ssGUI::Factory::Dispose(clonedCallback);
        testNum = 0;
        
        clonedCallback = TestObj->AddEventCallbackCopy(Callback, true);
        ssTEST_OUTPUT_ASSERT(   "New Container", 
                                TestObj->IsEventCallbackExist(ssGUI::Enums::EventType::NONE));
    };

    ssTEST("EventTest")
    {
        //TestObj->SetBackgroundColor(glm::u8vec4(0, 0, 0, 255));
        //ssTEST_OUTPUT_ASSERT("GUIObject", ListenerNum == 1);
        //ListenerNum = 0;
        
        //Callback->Clone(testWindow, true);
        //testWindow->SetBackgroundColor(glm::u8vec4(0, 0, 0, 255));
        //ssTEST_OUTPUT_ASSERT("Window", ListenerNum == 1);
    };

    ssTEST_END();
}