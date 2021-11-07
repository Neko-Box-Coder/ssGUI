#ifndef BASE_EVENT_CALLBACK
#define BASE_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/EventCallback.hpp"
#include "ssGUI/BaseClasses/GUIObject.hpp"
#include <queue>

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: BaseEventCallback
    class BaseEventCallback : public EventCallback
    {
        private:
            //Events
            std::vector<std::function<void(ssGUI::GUIObject*)>> EventListeners;
            std::vector<bool> EventListenersValid;
            std::queue<int> NextFreeIndices;
            int EventListenerCount;
        
        // protected:
            BaseEventCallback(BaseEventCallback const & other);
            BaseEventCallback& operator=(BaseEventCallback const & other);

        public:
            BaseEventCallback();
            
            //function: AddEventListener
            int AddEventListener(std::function<void(ssGUI::GUIObject*)> callback) override;
            
            //function: RemoveEventListener
            void RemoveEventListener(int index) override;
            
            //function: GetEventListenerCount
            int GetEventListenerCount() const override;
            
            //function: Notify
            void Notify(ssGUI::GUIObject* source) override;
            
            //function: GetEventCallbackName
            virtual std::string GetEventCallbackName() const override;
            
            //function: Clone
            virtual EventCallback* Clone(ssGUI::GUIObject* container, std::vector<ssGUI::GUIObject*>& originalObjs, std::vector<ssGUI::GUIObject*>& clonedObjs) override;
            
            //const: EVENT_NAME
            static const std::string EVENT_NAME;
    };
}



#endif