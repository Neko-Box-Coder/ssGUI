#ifndef ON_RECURSIVE_CHILD_ADD_EVENT_CALLBACK
#define ON_RECURSIVE_CHILD_ADD_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: OnRecursiveChildAddEventCallback
    class OnRecursiveChildAddEventCallback : public BaseEventCallback
    {        
        private:
            OnRecursiveChildAddEventCallback(OnRecursiveChildAddEventCallback const &) = default;
            OnRecursiveChildAddEventCallback& operator=(OnRecursiveChildAddEventCallback const &) = default;
        
        public:
            OnRecursiveChildAddEventCallback() = default;
            
            //function: GetEventCallbackName
            virtual std::string GetEventCallbackName() const override; 
            
            //function: Clone
            virtual EventCallback* Clone(ssGUI::GUIObject* container, bool copyListeners) override;
            
            //const: EVENT_NAME
            static const std::string EVENT_NAME;
    };
}



#endif