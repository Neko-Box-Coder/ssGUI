#ifndef ON_OBJECT_DESTROY_EVENT_CALLBACK
#define ON_OBJECT_DESTROY_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: OnObjectDestroyEventCallback
    class OnObjectDestroyEventCallback : public BaseEventCallback
    {        
        private:
            OnObjectDestroyEventCallback(OnObjectDestroyEventCallback const &) = default;
            OnObjectDestroyEventCallback& operator=(OnObjectDestroyEventCallback const &) = default;
        
        public:
            OnObjectDestroyEventCallback() = default;
            
            //function: GetEventCallbackName
            virtual std::string GetEventCallbackName() const override;
            
            //function: Clone
            virtual EventCallback* Clone(ssGUI::GUIObject* container, bool copyListeners) override;
            
            //const: EVENT_NAME
            static const std::string EVENT_NAME;
    };
}

#endif