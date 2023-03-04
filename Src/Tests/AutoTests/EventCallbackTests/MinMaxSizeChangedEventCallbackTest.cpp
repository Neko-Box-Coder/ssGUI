#include "ssTest.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/EventCallbacks/MinMaxSizeChangedEventCallback.hpp"

ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback* callback = nullptr;
ssGUI::GUIObject* testObj = nullptr;
int listerNum = 0;

ssTEST_INIT();

ssTEST_SET_UP
{
    callback = ssGUI::Factory::Create<ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback>();
    testObj = ssGUI::Factory::Create<ssGUI::GUIObject>();
    
    //Timing is making sure the listener is triggered **after** the event
    callback->AddEventListener( "key", [&](ssGUI::EventInfo info)
                                {
                                    ssTEST_OUTPUT_ASSERT("Timing", testObj->GetMinSize() != glm::vec2(25, 25));
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
    ssTEST_OUTPUT_ASSERT(callback->GetEventCallbackName() == "MinMaxSizeChangedEvent");
}

ssTEST("EventTest")
{
    //Default size of a GUI Object is (25, 25), 
    //need to make sure it is that in order to test size change cuased by changing min size
    ssTEST_OUTPUT_ASSERT("Default Size", testObj->GetSize() == glm::vec2(25, 25));   
    
    testObj->SetMinSize(glm::vec2(50, 50));
    
    ssTEST_OUTPUT_ASSERT(listerNum == 1);   
    ssTEST_OUTPUT_ASSERT("Size change caused by minSize", testObj->GetSize() == glm::vec2(50, 50));   
    
    testObj->SetMaxSize(glm::vec2(500, 500));
    ssTEST_OUTPUT_ASSERT(listerNum == 2);   
}

ssTEST_END();