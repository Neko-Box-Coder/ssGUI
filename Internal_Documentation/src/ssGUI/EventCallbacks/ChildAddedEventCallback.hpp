#ifndef CHILD_ADDED_EVENT_CALLBACK
#define CHILD_ADDED_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: ChildAddedEventCallback
    //This event callback is triggered *after* a child is paranted on this GUI object
    //The child object being added will be the source for triggering this event callback.
    class ChildAddedEventCallback : public BaseEventCallback
    {        
        private:
            ChildAddedEventCallback(ChildAddedEventCallback const &) = default;
            ChildAddedEventCallback& operator=(ChildAddedEventCallback const &) = default;
        
        public:
            ChildAddedEventCallback() = default;
            
            //function: GetEventCallbackName
            //See <BaseEventCallback::GetEventCallbackName>
            virtual std::string GetEventCallbackName() const override; 
            
            //function: Clone
            //See <BaseEventCallback::Clone>
            virtual EventCallback* Clone(ssGUI::GUIObject* newContainer, bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}



#endif