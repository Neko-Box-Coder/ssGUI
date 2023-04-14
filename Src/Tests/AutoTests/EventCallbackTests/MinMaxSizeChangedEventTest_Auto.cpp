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
        Callback->SetEventType(ssGUI::Enums::EventType::MIN_MAX_SIZE_CHANGED);
        TestObj = ssGUI::Factory::Create<ssGUI::GUIObject>();
        
        //Timing is making sure the listener is triggered **after** the event
        Callback->AddEventListener( "key", [&](ssGUI::EventInfo info)
                                    {
                                        ssTEST_OUTPUT_ASSERT("Timing", TestObj->GetMinSize() != glm::vec2(25, 25));
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
        //Default size of a GUI Object is (25, 25), 
        //need to make sure it is that in order to test size change cuased by changing min size
        ssTEST_OUTPUT_ASSERT("Default Size", TestObj->GetSize() == glm::vec2(25, 25));   
        
        TestObj->SetMinSize(glm::vec2(50, 50));
        
        ssTEST_OUTPUT_ASSERT(ListenerNum == 1);   
        ssTEST_OUTPUT_ASSERT("Size change caused by minSize", TestObj->GetSize() == glm::vec2(50, 50));   
        
        TestObj->SetMaxSize(glm::vec2(500, 500));
        ssTEST_OUTPUT_ASSERT(ListenerNum == 2);   
    }

    ssTEST_END();
}