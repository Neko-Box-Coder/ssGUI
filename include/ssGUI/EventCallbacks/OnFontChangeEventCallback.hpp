#ifndef ON_FONT_CHANGE_EVENT_CALLBACK
#define ON_FONT_CHANGE_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: OnFontChangeEventCallback
    class OnFontChangeEventCallback : public BaseEventCallback
    {        
        private:
            OnFontChangeEventCallback(OnFontChangeEventCallback const &) = default;
            OnFontChangeEventCallback& operator=(OnFontChangeEventCallback const &) = default;
        
        public:
            OnFontChangeEventCallback() = default;
            
            //function: GetEventCallbackName
            virtual std::string GetEventCallbackName() const override;
            
            //function: Clone
            virtual EventCallback* Clone(ssGUI::GUIObject* container, bool copyListeners) override;
            
            //const: EVENT_NAME
            static const std::string EVENT_NAME;
    };
}

#endif