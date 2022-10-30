#ifndef ON_WINDOW_CLOSE_EVENT_CALLBACK
#define ON_WINDOW_CLOSE_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

namespace ssGUI
{

//namespace: ssGUI::EventCallbacks
namespace EventCallbacks
{
    //class: ssGUI::EventCallbacks::OnWindowCloseEventCallback
    //This event callback is triggered *before* this GUI Object (Window) is closed. Window(Container) will be the source for triggering this event callback.
    class OnWindowCloseEventCallback : public BaseEventCallback
    {        
        public:
            friend class ssGUI::Factory;
        
        protected:
            OnWindowCloseEventCallback() = default;
            OnWindowCloseEventCallback(OnWindowCloseEventCallback const&) = default;
            OnWindowCloseEventCallback& operator=(OnWindowCloseEventCallback const&) = default;
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
            virtual OnWindowCloseEventCallback* Clone(ssGUI::GUIObject* newContainer, bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}

}

#endif