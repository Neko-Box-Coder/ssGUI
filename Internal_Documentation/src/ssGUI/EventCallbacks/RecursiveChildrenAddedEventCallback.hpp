#ifndef RECURSIVE_CHILDREN_ADDED_EVENT_CALLBACK
#define RECURSIVE_CHILDREN_ADDED_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: RecursiveChildrenAddedEventCallback
    class RecursiveChildrenAddedEventCallback : public BaseEventCallback
    {        
        private:
            RecursiveChildrenAddedEventCallback(RecursiveChildrenAddedEventCallback const &) = default;
            RecursiveChildrenAddedEventCallback& operator=(RecursiveChildrenAddedEventCallback const &) = default;
        
        public:
            RecursiveChildrenAddedEventCallback() = default;
            
            //function: GetEventCallbackName
            virtual std::string GetEventCallbackName() const override; 
            
            //function: Clone
            virtual EventCallback* Clone(ssGUI::GUIObject* container, std::vector<ssGUI::GUIObject*>& originalObjs, std::vector<ssGUI::GUIObject*>& clonedObjs) override;
            
            //const: EVENT_NAME
            static const std::string EVENT_NAME;
    };
}



#endif