#ifndef BUTTON_STATE_CHANGED_EVENT_CALLBACK
#define BUTTON_STATE_CHANGED_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: ButtonStateChangedEventCallback
    //This event callback is triggered *after* the button state has changed
    class ButtonStateChangedEventCallback : public BaseEventCallback
    {        
        private:
            ButtonStateChangedEventCallback(ButtonStateChangedEventCallback const &) = default;
            ButtonStateChangedEventCallback& operator=(ButtonStateChangedEventCallback const &) = default;
        
        public:
            ButtonStateChangedEventCallback() = default;
            
            //function: GetEventCallbackName
            //See <BaseEventCallback::GetEventCallbackName>
            virtual std::string GetEventCallbackName() const override;
            
            //function: Clone
            //See <BaseEventCallback::Clone>
            virtual EventCallback* Clone(ssGUI::GUIObject* container, bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}

#endif