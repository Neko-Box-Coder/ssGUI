#ifndef SSGUI_SIZE_CHANGED_EVENT_CALLBACK_H
#define SSGUI_SIZE_CHANGED_EVENT_CALLBACK_H

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

namespace ssGUI
{

//namespace: ssGUI::EventCallbacks
namespace EventCallbacks
{
    //class: ssGUI::EventCallbacks::SizeChangedEventCallback
    //This event callback is triggered *after* this GUI object's size has changed. Container will be the source for triggering this event callback.
    class SizeChangedEventCallback : public BaseEventCallback
    {        
        public:
            friend class ssGUI::Factory;

        protected:
            SizeChangedEventCallback() = default;
            SizeChangedEventCallback(SizeChangedEventCallback const&) = default;
            SizeChangedEventCallback& operator=(SizeChangedEventCallback const&) = default;
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
            virtual SizeChangedEventCallback* Clone(bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}

}

#endif