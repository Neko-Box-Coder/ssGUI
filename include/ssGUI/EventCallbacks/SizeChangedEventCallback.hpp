#ifndef SIZE_CHANGED_EVENT_CALLBACK
#define SIZE_CHANGED_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: SizeChangedEventCallback
    //This event callback is triggered *after* this GUI object's size has changed
    class SizeChangedEventCallback : public BaseEventCallback
    {        
        private:
            SizeChangedEventCallback(SizeChangedEventCallback const&) = default;
            SizeChangedEventCallback& operator=(SizeChangedEventCallback const&) = default;
        
        public:
            SizeChangedEventCallback() = default;
            
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