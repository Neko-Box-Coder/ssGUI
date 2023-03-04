#ifndef SSGUI_ON_FONT_CHANGE_EVENT_CALLBACK_H
#define SSGUI_ON_FONT_CHANGE_EVENT_CALLBACK_H

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

namespace ssGUI
{

//namespace: ssGUI::EventCallbacks
namespace EventCallbacks
{
    //class: ssGUI::EventCallbacks::OnFontChangeEventCallback
    //This event callback is triggered *before* the font of a text is changed. Text(Container) will be the source for triggering this event callback.
    //Please note that this is not triggered by any changes to default fonts.
    class OnFontChangeEventCallback : public BaseEventCallback
    {        
        public:
            friend class ssGUI::Factory;
        
        protected:
            OnFontChangeEventCallback() = default;
            OnFontChangeEventCallback(OnFontChangeEventCallback const &) = default;
            OnFontChangeEventCallback& operator=(OnFontChangeEventCallback const &) = default;
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
            virtual OnFontChangeEventCallback* Clone(ssGUI::GUIObject* newContainer, bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}

}

#endif