#ifndef ON_RECURSIVE_CHILD_REMOVE_EVENT_CALLBACK
#define ON_RECURSIVE_CHILD_REMOVE_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: OnRecursiveChildRemoveEventCallback
    //This event callback is triggered *before* a child is removed recursively on this GUI object. 
    //The child object being removed will be the source for triggering this event callback.
    class OnRecursiveChildRemoveEventCallback : public BaseEventCallback
    {        
        public:
            friend class ssGUI::Factory;
        
        protected:
            OnRecursiveChildRemoveEventCallback() = default;
            OnRecursiveChildRemoveEventCallback(OnRecursiveChildRemoveEventCallback const &) = default;
            OnRecursiveChildRemoveEventCallback& operator=(OnRecursiveChildRemoveEventCallback const &) = default;
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
            virtual OnRecursiveChildRemoveEventCallback* Clone(ssGUI::GUIObject* newContainer, bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}



#endif