#ifndef WINDOW_DRAG_STATE_CHANGED_EVENT_CALLBACK
#define WINDOW_DRAG_STATE_CHANGED_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: WindowDragStateChangedEventCallback
    class WindowDragStateChangedEventCallback : public BaseEventCallback
    {        
        private:
            WindowDragStateChangedEventCallback(WindowDragStateChangedEventCallback const &) = default;
            WindowDragStateChangedEventCallback& operator=(WindowDragStateChangedEventCallback const &) = default;
        
        public:
            WindowDragStateChangedEventCallback() = default;
            
            //function: GetEventCallbackName
            virtual std::string GetEventCallbackName() const override;
            
            //function: Clone
            virtual EventCallback* Clone(ssGUI::GUIObject* container, std::vector<ssGUI::GUIObject*>& originalObjs, std::vector<ssGUI::GUIObject*>& clonedObjs) override;
            
            //const: EVENT_NAME
            static const std::string EVENT_NAME;
    };
}

#endif