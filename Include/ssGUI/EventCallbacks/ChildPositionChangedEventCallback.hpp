#ifndef CHILD_POSITION_CHANGED_EVENT_CALLBACK
#define CHILD_POSITION_CHANGED_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

namespace ssGUI
{

//namespace: ssGUI::EventCallbacks
namespace EventCallbacks
{
    //class: ssGUI::EventCallbacks::ChildPositionChangedEventCallback
    //This event callback is triggered *after* <GUIObject::ChangeChildOrderToBeforePosition> or <GUIObject::ChangeChildOrderToAfterPosition> is called.
    //The child that is passed to <GUIObject::ChangeChildOrderToBeforePosition> or <GUIObject::ChangeChildOrderToAfterPosition> will be the source 
    //for triggering this event callback.
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
            virtual ChildPositionChangedEventCallback* Clone(ssGUI::GUIObject* newContainer, bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}

}
#endif