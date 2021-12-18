#ifndef MIN_MAX_SIZE_CHANGED_EVENT_CALLBACK
#define MIN_MAX_SIZE_CHANGED_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: MinMaxSizeChangedEventCallback
    class MinMaxSizeChangedEventCallback : public BaseEventCallback
    {        
        private:
            MinMaxSizeChangedEventCallback(MinMaxSizeChangedEventCallback const &) = default;
            MinMaxSizeChangedEventCallback& operator=(MinMaxSizeChangedEventCallback const &) = default;
        
        public:
            MinMaxSizeChangedEventCallback() = default;
            
            //function: GetEventCallbackName
            virtual std::string GetEventCallbackName() const override;
            
            //function: Clone
            virtual EventCallback* Clone(ssGUI::GUIObject* container, bool copyListeners) override;
            
            //const: EVENT_NAME
            static const std::string EVENT_NAME;
    };
}

#endif