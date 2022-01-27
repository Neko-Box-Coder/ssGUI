#ifndef ON_WINDOW_CLOSE_EVENT_CALLBACK
#define ON_WINDOW_CLOSE_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: OnWindowCloseEventCallback
    //This event callback is triggered *before* this GUI Object (Window) is closed
    class OnWindowCloseEventCallback : public BaseEventCallback
    {        
        private:
            OnWindowCloseEventCallback(OnWindowCloseEventCallback const&) = default;
            OnWindowCloseEventCallback& operator=(OnWindowCloseEventCallback const&) = default;
        
        public:
            OnWindowCloseEventCallback() = default;
            
            //function: GetEventCallbackName
            //See <BaseEventCallback::GetEventCallbackName>
            virtual std::string GetEventCallbackName() const override; 
            
            //function: Clone
            //See <BaseEventCallback::Clone>
            virtual EventCallback* Clone(ssGUI::GUIObject* container, bool copyListeners) override;
            
            //const: EVENT_NAME
            static const std::string EVENT_NAME;
    };
}



#endif