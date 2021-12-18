#ifndef BUTTON_STATE_CHANGED_EVENT_CALLBACK
#define BUTTON_STATE_CHANGED_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: ButtonStateChangedEventCallback
    class ButtonStateChangedEventCallback : public BaseEventCallback
    {        
        private:
            ButtonStateChangedEventCallback(ButtonStateChangedEventCallback const &) = default;
            ButtonStateChangedEventCallback& operator=(ButtonStateChangedEventCallback const &) = default;
        
        public:
            ButtonStateChangedEventCallback() = default;
            
            //function: GetEventCallbackName
            virtual std::string GetEventCallbackName() const override;
            
            //function: Clone
            virtual EventCallback* Clone(ssGUI::GUIObject* container, bool copyListeners) override;
            
            //const: EVENT_NAME
            static const std::string EVENT_NAME;
    };
}

#endif