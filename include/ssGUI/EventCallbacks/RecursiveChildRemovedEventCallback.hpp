#ifndef RECURSIVE_CHILDREN_REMOVED_EVENT_CALLBACK
#define RECURSIVE_CHILDREN_REMOVED_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: RecursiveChildRemovedEventCallback
    //This event callback is triggered *after* a child is removed recursively on this GUI object
    class RecursiveChildRemovedEventCallback : public BaseEventCallback
    {        
        private:
            RecursiveChildRemovedEventCallback(RecursiveChildRemovedEventCallback const&) = default;
            RecursiveChildRemovedEventCallback& operator=(RecursiveChildRemovedEventCallback const&) = default;
        
        public:
            RecursiveChildRemovedEventCallback() = default;
            
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