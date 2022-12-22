#ifndef SSGUI_ON_RECURSIVE_CHILD_ADD_EVENT_CALLBACK_H
#define SSGUI_ON_RECURSIVE_CHILD_ADD_EVENT_CALLBACK_H

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

namespace ssGUI
{

//namespace: ssGUI::EventCallbacks
namespace EventCallbacks
{
    //class: ssGUI::EventCallbacks::OnRecursiveChildAddEventCallback
    //This event callback is triggered *before* a child is paranted recursively on this GUI object. 
    //The child object being added will be the source for triggering this event callback.
    class OnRecursiveChildAddEventCallback : public BaseEventCallback
    {        
        public:
            friend class ssGUI::Factory;

        protected:
            OnRecursiveChildAddEventCallback() = default;
            OnRecursiveChildAddEventCallback(OnRecursiveChildAddEventCallback const &) = default;
            OnRecursiveChildAddEventCallback& operator=(OnRecursiveChildAddEventCallback const &) = default;
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
            virtual OnRecursiveChildAddEventCallback* Clone(ssGUI::GUIObject* newContainer, bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}

}

#endif