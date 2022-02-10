#ifndef BASE_EVENT_CALLBACK
#define BASE_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/EventCallback.hpp"
#include "ssGUI/BaseClasses/GUIObject.hpp"
#include <queue>

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    /*class: BaseEventCallback
    Implementation class for <EventCallback>
    
    Variables & Constructor:
    ============================== C++ ==============================
    private:
        //Events
        std::vector<std::function<void(ssGUI::GUIObject*)>> EventListeners;
        std::vector<bool> EventListenersValid;
        std::queue<int> NextFreeIndices;
        int EventListenerCount;
    =================================================================
    ============================== C++ ==============================
    BaseEventCallback::BaseEventCallback() : EventListeners(), EventListenersValid(), NextFreeIndices(), EventListenerCount(0)
    {}
    =================================================================
    */
    class BaseEventCallback : public EventCallback
    {
        private:
            //Events
            std::vector<std::function<void(ssGUI::GUIObject*, ssGUI::GUIObject*)>> EventListeners;
            std::vector<bool> EventListenersValid;
            std::queue<int> NextFreeIndices;
            int EventListenerCount;
            ssGUI::GUIObject* Container;
        
        protected:
            BaseEventCallback(BaseEventCallback const & other) = default;
            BaseEventCallback& operator=(BaseEventCallback const & other) = default;

        public:
            BaseEventCallback();
            
            //function: AddEventListener
            //See <EventCallback::AddEventListener>
            int AddEventListener(std::function<void(ssGUI::GUIObject* src, ssGUI::GUIObject* container)> callback) override;
            
            //function: RemoveEventListener
            //See <EventCallback::RemoveEventListener>
            void RemoveEventListener(int index) override;
            
            //function: GetEventListenerCount
            //See <EventCallback::GetEventListenerCount>
            int GetEventListenerCount() const override;
            
            //function: Notify
            //See <EventCallback::Notify>
            void Notify(ssGUI::GUIObject* source) override;
            
            void BindToObject(ssGUI::GUIObject* bindObj) override;

            //function: GetEventCallbackName
            //See <EventCallback::GetEventCallbackName>
            virtual std::string GetEventCallbackName() const override;
            
            //function: Clone
            //See <EventCallback::Clone>
            virtual EventCallback* Clone(ssGUI::GUIObject* container, bool copyListeners) override;

            //const: EVENT_NAME
            //This is what you use in order to get the EventCallback that is added to the GUIObject
            static const std::string EVENT_NAME;
    };
}



#endif