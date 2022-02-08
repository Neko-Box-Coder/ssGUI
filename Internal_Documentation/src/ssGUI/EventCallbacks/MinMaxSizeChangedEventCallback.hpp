#ifndef MIN_MAX_SIZE_CHANGED_EVENT_CALLBACK
#define MIN_MAX_SIZE_CHANGED_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: MinMaxSizeChangedEventCallback
    //This event callback is triggered *after* min or max size of a GUI object has changed
    class MinMaxSizeChangedEventCallback : public BaseEventCallback
    {        
        private:
            MinMaxSizeChangedEventCallback(MinMaxSizeChangedEventCallback const &) = default;
            MinMaxSizeChangedEventCallback& operator=(MinMaxSizeChangedEventCallback const &) = default;
        
        public:
            MinMaxSizeChangedEventCallback() = default;
            
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