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
    /*class: EventCallback
    EventCallback enables the ablity to act against a certain event. 

    In order to add a class's function as a listener to an event callback
    > int index = EventCallback->AddEventListener(std::bind(&some::class::function, objectPointer, std::placeholders::_1));
    
    Alternatively, you can add a lambda as an event listener to an <EventCallback> object.
    =================================== C++ ===================================
    int index = EventCallback->AddEventListener
    (
        //source is what *triggered* the callback
        //container is the GUI Object that holds this event callback
        [](ssGUI::GUIObject* source, ssGUI::GUIObject* container)
        {
            //Do something
        }    
    );
    ===========================================================================*/
    class EventCallback
    {
        protected:
            EventCallback(EventCallback const &) = default;
            EventCallback& operator=(EventCallback const &) = default;
            EventCallback() = default;

        public:
            
            /*function: AddEventListener
            Adds a listener to this EventCallback, and returns an index for removing it. 
            The first argument is the source/cause of the event callback. Second argument is the container of the EventCallback.*/
            virtual int AddEventListener(std::function<void(ssGUI::GUIObject* src, ssGUI::GUIObject* container)> callback) = 0;
            
            //function: RemoveEventListener
            //Removes a listener with the index specified
            virtual void RemoveEventListener(int index) = 0;
            
            //function: GetEventListenerCount
            //Returns the number of event listeners
            virtual int GetEventListenerCount() const = 0;
            
            //function: Notify
            //Call this function to trigger this event callback
            virtual void Notify(ssGUI::GUIObject* source) = 0;
            
            //function: BindToObject
            //Binds this event callback to a GUI object
            virtual void BindToObject(ssGUI::GUIObject* bindObj) = 0;

            //function: GetEventCallbackName
            //Returns the name of this event callback
            virtual std::string GetEventCallbackName() const = 0;
            
            //function: Clone
            //Clone this event callback with the option of copying the listeners
            virtual EventCallback* Clone(ssGUI::GUIObject* container, bool copyListeners) = 0;
    };
}

#endif