#ifndef ON_RECURSIVE_CHILD_ADD_EVENT_CALLBACK
#define ON_RECURSIVE_CHILD_ADD_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: OnRecursiveChildAddEventCallback
    //This event callback is triggered *before* a child is paranted recursively on this GUI object
    class OnRecursiveChildAddEventCallback : public BaseEventCallback
    {        
        private:
            OnRecursiveChildAddEventCallback(OnRecursiveChildAddEventCallback const &) = default;
            OnRecursiveChildAddEventCallback& operator=(OnRecursiveChildAddEventCallback const &) = default;
        
        public:
            OnRecursiveChildAddEventCallback() = default;
            
            //function: GetEventCallbackName
            //See <BaseEventCallback::GetEventCallbackName>
            virtual std::string GetEventCallbackName() const override; 
            
            //function: Clone
            //See <BaseEventCallback::Clone>
            virtual EventCallback* Clone(ssGUI::GUIObject* container, bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}



#endif