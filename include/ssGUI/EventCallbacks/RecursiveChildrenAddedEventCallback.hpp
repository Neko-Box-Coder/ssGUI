#ifndef RECURSIVE_CHILDREN_ADDED_EVENT_CALLBACK
#define RECURSIVE_CHILDREN_ADDED_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//TODO : Rename this to child instead of children

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: RecursiveChildrenAddedEventCallback
    //This event callback is triggered *after* a child is paranted recursively on this GUI object
    class RecursiveChildrenAddedEventCallback : public BaseEventCallback
    {        
        private:
            RecursiveChildrenAddedEventCallback(RecursiveChildrenAddedEventCallback const &) = default;
            RecursiveChildrenAddedEventCallback& operator=(RecursiveChildrenAddedEventCallback const &) = default;
        
        public:
            RecursiveChildrenAddedEventCallback() = default;
            
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