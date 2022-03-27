#ifndef ON_FONT_CHANGE_EVENT_CALLBACK
#define ON_FONT_CHANGE_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: OnFontChangeEventCallback
    //This event callback is triggered *before* the font of a text is changed. Text(Container) will be the source for triggering this event callback.
    class OnFontChangeEventCallback : public BaseEventCallback
    {        
        private:
            OnFontChangeEventCallback(OnFontChangeEventCallback const &) = default;
            OnFontChangeEventCallback& operator=(OnFontChangeEventCallback const &) = default;
        
        public:
            OnFontChangeEventCallback() = default;
            
            //function: GetEventCallbackName
            //See <BaseEventCallback::GetEventCallbackName>
            virtual std::string GetEventCallbackName() const override;
            
            //function: Clone
            //See <BaseEventCallback::Clone>
            virtual EventCallback* Clone(ssGUI::GUIObject* newContainer, bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}

#endif