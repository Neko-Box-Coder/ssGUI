#ifndef CHILD_POSITION_CHANGED_EVENT_CALLBACK
#define CHILD_POSITION_CHANGED_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: ChildPositionChangedEventCallback
    //This event callback is triggered *after* <GUIObject::ChangeChildOrderToBeforePosition> or <GUIObject::ChangeChildOrderToAfterPosition> is called.
    //The child that is passed to <GUIObject::ChangeChildOrderToBeforePosition> or <GUIObject::ChangeChildOrderToAfterPosition> will be the source 
    //for triggering this event callback.
    class ChildPositionChangedEventCallback : public BaseEventCallback
    {        
        private:
            ChildPositionChangedEventCallback(ChildPositionChangedEventCallback const &) = default;
            ChildPositionChangedEventCallback& operator=(ChildPositionChangedEventCallback const &) = default;
        
        public:
            ChildPositionChangedEventCallback() = default;
            
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