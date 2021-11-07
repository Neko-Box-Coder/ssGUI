#ifndef WINDOW_CLOSE_EVENT_CALLBACK
#define WINDOW_CLOSE_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: WindowCloseEventCallback
    class WindowCloseEventCallback : public BaseEventCallback
    {        
        private:
            WindowCloseEventCallback(WindowCloseEventCallback const&) = default;
            WindowCloseEventCallback& operator=(WindowCloseEventCallback const&) = default;
        
        public:
            WindowCloseEventCallback() = default;
            
            //function: GetEventCallbackName
            virtual std::string GetEventCallbackName() const override; 
            
            //function: Clone
            virtual EventCallback* Clone(ssGUI::GUIObject* container, std::vector<ssGUI::GUIObject*>& originalObjs, std::vector<ssGUI::GUIObject*>& clonedObjs) override;
            
            //const: EVENT_NAME
            static const std::string EVENT_NAME;
    };
}



#endif