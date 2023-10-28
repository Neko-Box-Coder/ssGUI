#include "ssTest.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"

ssGUI::EventCallback* Callback = nullptr;
ssGUI::GUIObject* TestObj = nullptr;
ssGUI::Window* TestWindow = nullptr;
int ListenerNum = 0;

glm::u8vec4 ChangeBGColor = glm::u8vec4(123, 111, 100, 201);

int main()
{
    ssTEST_INIT();

    ssTEST_SET_UP
    {
        Callback = ssGUI::Factory::Create<ssGUI::EventCallback>();
        Callback->SetEventType(ssGUI::Enums::EventType::BACKGROUND_COLOR_CHANGED);
        TestObj = ssGUI::Factory::Create<ssGUI::GUIObject>();
        TestWindow = ssGUI::Factory::Create<ssGUI::Window>();
        
        //Timing is making sure the listener is triggered **after** the event
        Callback->AddEventListener( "key", [&](ssGUI::EventInfo& info)
                                    {
                                        ssTEST_OUTPUT_ASSERT("Timing", info.EventSource->GetBackgroundColor() == ChangeBGColor); 
                                        ListenerNum = 1;
                                    });
        TestObj->AddEventCallbackCopy(Callback, true);
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
        TestObj->SetBackgroundColor(ChangeBGColor);
        ssTEST_OUTPUT_ASSERT("GUIObject", ListenerNum == 1);
        
        ListenerNum = 0;
        TestWindow->SetBackgroundColor(ChangeBGColor);
        ssTEST_OUTPUT_ASSERT("Window", ListenerNum == 1);
    };

    ssTEST_END();
}