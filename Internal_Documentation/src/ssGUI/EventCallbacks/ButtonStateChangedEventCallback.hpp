#ifndef BUTTON_STATE_CHANGED_EVENT_CALLBACK
#define BUTTON_STATE_CHANGED_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: ssGUI::EventCallbacks::ButtonStateChangedEventCallback
    //This event callback is triggered *after* the button state has changed. Button(Container) will be the source for triggering this event callback.
    class ButtonStateChangedEventCallback : public BaseEventCallback
    {        
        public:
            friend class ssGUI::Factory;

        protected:
            ButtonStateChangedEventCallback() = default;
            ButtonStateChangedEventCallback(ButtonStateChangedEventCallback const &) = default;
            ButtonStateChangedEventCallback& operator=(ButtonStateChangedEventCallback const &) = default;
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
            virtual ButtonStateChangedEventCallback* Clone(ssGUI::GUIObject* newContainer, bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}

#endif