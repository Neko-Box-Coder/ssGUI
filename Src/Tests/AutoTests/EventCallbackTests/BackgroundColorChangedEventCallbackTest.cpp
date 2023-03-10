#include "ssTest.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/EventCallbacks/BackgroundColorChangedEventCallback.hpp"

ssGUI::EventCallbacks::BackgroundColorChangedEventCallback* callback = nullptr;
ssGUI::GUIObject* testObj = nullptr;
ssGUI::Window* testWindow = nullptr;
int listerNum = 0;

glm::u8vec4 changeBGColor = glm::u8vec4(123, 111, 100, 201);

ssTEST_INIT();

ssTEST_SET_UP
{
    callback = ssGUI::Factory::Create<ssGUI::EventCallbacks::BackgroundColorChangedEventCallback>();
    testObj = ssGUI::Factory::Create<ssGUI::GUIObject>();
    testWindow = ssGUI::Factory::Create<ssGUI::Window>();
    
    //Timing is making sure the listener is triggered **after** the event
    callback->AddEventListener( "key", [&](ssGUI::EventInfo info)
                                {
                                    ssTEST_OUTPUT_ASSERT("Timing", info.EventSource->GetBackgroundColor() == changeBGColor); 
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

ssTEST("GetEventCallbackNameTest")
{
    ssTEST_OUTPUT_ASSERT(callback->GetEventCallbackName() == "BackgroundColorChangedEvent");
}

ssTEST("EventTest")
{
    testObj->SetBackgroundColor(changeBGColor);
    ssTEST_OUTPUT_ASSERT("GUIObject", listerNum == 1);
    
    listerNum = 0;
    testWindow->SetBackgroundColor(changeBGColor);
    ssTEST_OUTPUT_ASSERT("Window", listerNum == 1);
}

ssTEST_END();