#ifndef SSGUI_ON_FOCUS_LOST_EVENT_CALLBACK_H
#define SSGUI_ON_FOCUS_LOST_EVENT_CALLBACK_H

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

namespace ssGUI
{

//namespace: ssGUI::EventCallbacks
namespace EventCallbacks
{
    //class: ssGUI::EventCallbacks::FocusLostEventCallback
    //This event callback is triggered after the GUI Object lost focus
    class FocusLostEventCallback : public BaseEventCallback
    {        
        public:
            friend class ssGUI::Factory;
        
        protected:
            FocusLostEventCallback() = default;
            FocusLostEventCallback(FocusLostEventCallback const &) = default;
            FocusLostEventCallback& operator=(FocusLostEventCallback const &) = default;
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
            virtual FocusLostEventCallback* Clone(bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}

}

#endif