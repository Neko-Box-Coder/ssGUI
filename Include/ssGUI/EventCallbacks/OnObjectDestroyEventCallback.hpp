#ifndef ON_OBJECT_DESTROY_EVENT_CALLBACK
#define ON_OBJECT_DESTROY_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

namespace ssGUI
{

//namespace: ssGUI::EventCallbacks
namespace EventCallbacks
{
    //class: ssGUI::EventCallbacks::OnObjectDestroyEventCallback
    //This event callback is triggered *before* the GUI object is destroyed. Container will be the source for triggering this event callback.
    class OnObjectDestroyEventCallback : public BaseEventCallback
    {        
        public:
            friend class ssGUI::Factory;
        
        protected:
            OnObjectDestroyEventCallback() = default;
            OnObjectDestroyEventCallback(OnObjectDestroyEventCallback const &) = default;
            OnObjectDestroyEventCallback& operator=(OnObjectDestroyEventCallback const &) = default;
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
            virtual OnObjectDestroyEventCallback* Clone(ssGUI::GUIObject* newContainer, bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}

}

#endif