#ifndef RECURSIVE_CHILD_ADDED_EVENT_CALLBACK
#define RECURSIVE_CHILD_ADDED_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: ssGUI::EventCallbacks::RecursiveChildAddedEventCallback
    //This event callback is triggered *after* a child is paranted recursively on this GUI object
    //The child object being added will be the source for triggering this event callback.
    class RecursiveChildAddedEventCallback : public BaseEventCallback
    {        
        public:
            friend class ssGUI::Factory;

        protected:
            RecursiveChildAddedEventCallback() = default;
            RecursiveChildAddedEventCallback(RecursiveChildAddedEventCallback const &) = default;
            RecursiveChildAddedEventCallback& operator=(RecursiveChildAddedEventCallback const &) = default;
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
            virtual RecursiveChildAddedEventCallback* Clone(ssGUI::GUIObject* newContainer, bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}



#endif