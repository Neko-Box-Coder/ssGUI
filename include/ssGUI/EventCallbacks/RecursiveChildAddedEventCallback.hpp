#ifndef RECURSIVE_CHILD_ADDED_EVENT_CALLBACK
#define RECURSIVE_CHILD_ADDED_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: RecursiveChildAddedEventCallback
    //This event callback is triggered *after* a child is paranted recursively on this GUI object
    class RecursiveChildAddedEventCallback : public BaseEventCallback
    {        
        private:
            RecursiveChildAddedEventCallback(RecursiveChildAddedEventCallback const &) = default;
            RecursiveChildAddedEventCallback& operator=(RecursiveChildAddedEventCallback const &) = default;
        
        public:
            RecursiveChildAddedEventCallback() = default;
            
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