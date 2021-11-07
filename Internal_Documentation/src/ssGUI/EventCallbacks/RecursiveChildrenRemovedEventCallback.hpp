#ifndef RECURSIVE_CHILDREN_REMOVED_EVENT_CALLBACK
#define RECURSIVE_CHILDREN_REMOVED_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: RecursiveChildrenRemovedEventCallback
    class RecursiveChildrenRemovedEventCallback : public BaseEventCallback
    {        
        private:
            RecursiveChildrenRemovedEventCallback(RecursiveChildrenRemovedEventCallback const&) = default;
            RecursiveChildrenRemovedEventCallback& operator=(RecursiveChildrenRemovedEventCallback const&) = default;
        
        public:
            RecursiveChildrenRemovedEventCallback() = default;
            
            //function: GetEventCallbackName
            virtual std::string GetEventCallbackName() const override; 
            
            //function: Clone
            virtual EventCallback* Clone(ssGUI::GUIObject* container, std::vector<ssGUI::GUIObject*>& originalObjs, std::vector<ssGUI::GUIObject*>& clonedObjs) override;
            
            //const: EVENT_NAME
            static const std::string EVENT_NAME;
    };
}



#endif