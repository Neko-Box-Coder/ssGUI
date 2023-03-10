#ifndef SSGUI_CHILD_POSITION_CHANGED_EVENT_CALLBACK_H
#define SSGUI_CHILD_POSITION_CHANGED_EVENT_CALLBACK_H

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

namespace ssGUI
{

//namespace: ssGUI::EventCallbacks
namespace EventCallbacks
{
    /*class: ssGUI::EventCallbacks::ChildPositionChangedEventCallback
    This event callback is triggered *after* <GUIObject::ChangeChildOrderToBeforePosition> or <GUIObject::ChangeChildOrderToAfterPosition> is called.
    The child that is passed to <GUIObject::ChangeChildOrderToBeforePosition> or <GUIObject::ChangeChildOrderToAfterPosition> will be the source 
    for triggering this event callback.
    
    The event will only be triggered once as a result of the ChangeChildOrderToXXXPosition function
    and will not be triggered by the change of position for other children as a result of this function call.
    
    Meaning only the child GUI Object that is being **actively** changed position will have the event triggered
    (i.e. the child that is passed to the function for changing the position).*/
    class ChildPositionChangedEventCallback : public BaseEventCallback
    {        
        public:
            friend class ssGUI::Factory;
        
        protected:
            ChildPositionChangedEventCallback() = default;
            ChildPositionChangedEventCallback(ChildPositionChangedEventCallback const &) = default;
            ChildPositionChangedEventCallback& operator=(ChildPositionChangedEventCallback const &) = default;
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};
        
        public:
            
            //function: GetEventCallbackName
            //See <BaseEventCallback::GetEventCallbackName>
            virtual std::string GetEventCallbackName() const override;
            
            //function: Clone
            //See <BaseEventCallback::Clone>
            virtual ChildPositionChangedEventCallback* Clone(bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}

}
#endif