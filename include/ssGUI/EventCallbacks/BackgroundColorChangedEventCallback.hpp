#ifndef BACKGROUND_COLOR_CHANGED_EVENT_CALLBACK
#define BACKGROUND_COLOR_CHANGED_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: BackgroundColorChangedEventCallback
    //This event callback is triggered *after* the background color is changed. (Container) will be the source for triggering this event callback.
    class BackgroundColorChangedEventCallback : public BaseEventCallback
    {        
        public:
            friend class ssGUI::Factory;
        
        protected:
            BackgroundColorChangedEventCallback() = default;
            BackgroundColorChangedEventCallback(BackgroundColorChangedEventCallback const &) = default;
            BackgroundColorChangedEventCallback& operator=(BackgroundColorChangedEventCallback const &) = default;
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
            virtual BackgroundColorChangedEventCallback* Clone(ssGUI::GUIObject* newContainer, bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}

#endif