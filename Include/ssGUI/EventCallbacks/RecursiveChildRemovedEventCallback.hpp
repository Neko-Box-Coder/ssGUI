#ifndef SSGUI_RECURSIVE_CHILD_REMOVED_EVENT_CALLBACK_H
#define SSGUI_RECURSIVE_CHILD_REMOVED_EVENT_CALLBACK_H

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

namespace ssGUI
{

//namespace: ssGUI::EventCallbacks
namespace EventCallbacks
{
    //class: ssGUI::EventCallbacks::RecursiveChildRemovedEventCallback
    //This event callback is triggered *after* a child is removed recursively on this GUI object
    //The child object being removed will be the source for triggering this event callback.
    class RecursiveChildRemovedEventCallback : public BaseEventCallback
    {     
        public:
            friend class ssGUI::Factory;

        protected:
            RecursiveChildRemovedEventCallback() = default;
            RecursiveChildRemovedEventCallback(RecursiveChildRemovedEventCallback const&) = default;
            RecursiveChildRemovedEventCallback& operator=(RecursiveChildRemovedEventCallback const&) = default;
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
            virtual RecursiveChildRemovedEventCallback* Clone(bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}

}

#endif