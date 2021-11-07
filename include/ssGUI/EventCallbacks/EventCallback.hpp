#ifndef SSGUI_EVENT_CALLBACK
#define SSGUI_EVENT_CALLBACK

#include <functional>
#include <string>
#include <vector>


namespace ssGUI
{
    class GUIObject;
}

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: EventCallback
    //In order to add a class's function to an event callback 
    //> index = EventCallback->AddEventListener(std::bind(&some::class::function, objectPointer, std::placeholders::_1));
    class EventCallback
    {
        private:
            //EventCallback(EventCallback const &) = default;
            //EventCallback& operator=(EventCallback const &) = default;

        public:
            //EventCallback() = default;
            
            //function: AddEventListener
            virtual int AddEventListener(std::function<void(ssGUI::GUIObject *)> callback) = 0;
            
            //function: RemoveEventListener
            virtual void RemoveEventListener(int index) = 0;
            
            //function: GetEventListenerCount
            virtual int GetEventListenerCount() const = 0;
            
            //function: Notify
            virtual void Notify(ssGUI::GUIObject* source) = 0;
            
            //function: GetEventCallbackName
            virtual std::string GetEventCallbackName() const = 0;
            
            //function: Clone
            virtual EventCallback* Clone(ssGUI::GUIObject* container, std::vector<ssGUI::GUIObject*>& originalObjs, std::vector<ssGUI::GUIObject*>& clonedObjs) = 0;
    };
}

#endif