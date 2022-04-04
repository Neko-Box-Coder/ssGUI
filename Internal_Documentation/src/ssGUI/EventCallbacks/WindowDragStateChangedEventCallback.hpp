#ifndef WINDOW_DRAG_STATE_CHANGED_EVENT_CALLBACK
#define WINDOW_DRAG_STATE_CHANGED_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: WindowDragStateChangedEventCallback
    //This event callback is triggered *after* this GUI object (window) drag state is changed. Window(Container) will be the source for triggering this event callback.
    class WindowDragStateChangedEventCallback : public BaseEventCallback
    {        
        private:
            WindowDragStateChangedEventCallback(WindowDragStateChangedEventCallback const &) = default;
            WindowDragStateChangedEventCallback& operator=(WindowDragStateChangedEventCallback const &) = default;
        
        public:
            WindowDragStateChangedEventCallback() = default;
            
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