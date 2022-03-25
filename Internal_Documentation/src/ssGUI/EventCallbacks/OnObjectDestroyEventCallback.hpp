#ifndef ON_OBJECT_DESTROY_EVENT_CALLBACK
#define ON_OBJECT_DESTROY_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: OnObjectDestroyEventCallback
    //This event callback is triggered *before* the GUI object is destroyed
    class OnObjectDestroyEventCallback : public BaseEventCallback
    {        
        private:
            OnObjectDestroyEventCallback(OnObjectDestroyEventCallback const &) = default;
            OnObjectDestroyEventCallback& operator=(OnObjectDestroyEventCallback const &) = default;
        
        public:
            OnObjectDestroyEventCallback() = default;
            
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