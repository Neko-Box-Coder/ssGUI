#ifndef CHILD_POSITION_CHANGED_EVENT_CALLBACK
#define CHILD_POSITION_CHANGED_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: ChildPositionChangedEventCallback
    class ChildPositionChangedEventCallback : public BaseEventCallback
    {        
        private:
            ChildPositionChangedEventCallback(ChildPositionChangedEventCallback const &) = default;
            ChildPositionChangedEventCallback& operator=(ChildPositionChangedEventCallback const &) = default;
        
        public:
            ChildPositionChangedEventCallback() = default;
            
            //function: GetEventCallbackName
            virtual std::string GetEventCallbackName() const override;
            
            //function: Clone
            virtual EventCallback* Clone(ssGUI::GUIObject* container, bool copyListeners) override;
            
            //const: EVENT_NAME
            static const std::string EVENT_NAME;
    };
}

#endif