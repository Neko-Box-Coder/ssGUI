#ifndef ON_WINDOW_CLOSE_EVENT_CALLBACK
#define ON_WINDOW_CLOSE_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: OnWindowCloseEventCallback
    class OnWindowCloseEventCallback : public BaseEventCallback
    {        
        private:
            OnWindowCloseEventCallback(OnWindowCloseEventCallback const&) = default;
            OnWindowCloseEventCallback& operator=(OnWindowCloseEventCallback const&) = default;
        
        public:
            OnWindowCloseEventCallback() = default;
            
            //function: GetEventCallbackName
            virtual std::string GetEventCallbackName() const override; 
            
            //function: Clone
            virtual EventCallback* Clone(ssGUI::GUIObject* container, bool copyListeners) override;
            
            //const: EVENT_NAME
            static const std::string EVENT_NAME;
    };
}



#endif