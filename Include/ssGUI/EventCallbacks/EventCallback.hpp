#ifndef SSGUI_EVENT_CALLBACK
#define SSGUI_EVENT_CALLBACK

#include "ssGUI/Factory.hpp"
#include "ssGUI/DataClasses/ObjectsReferences.hpp"
#include "ssGUI/DataClasses/EventInfo.hpp"
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
    /*class: ssGUI::EventCallbacks::EventCallback
    EventCallback enables the ablity to act against a certain event. 

    In order to add a class's function as a listener to an event callback
    > int index = EventCallback->AddEventListener("AnyKey", std::bind(&some::class::function, objectPointer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    
    Alternatively, you can add a lambda as an event listener to an <EventCallback> object.
    =================================== C++ ===================================
    int index = EventCallback->AddEventListener
    (
        "AnyKey",
        //source is what *triggered* the callback
        //container is the GUI Object that holds this event callback
        [](ssGUI::EventInfo info)
        {
            //Do something
        }    
    );
    ===========================================================================*/
    class EventCallback
    {
        public:
            friend class ssGUI::Factory;

        protected:
            EventCallback() = default;
            EventCallback(EventCallback const &) = default;
            EventCallback& operator=(EventCallback const &) = default;
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};

        public:
            /*function: AddEventListener
            Adds a listener to this EventCallback by passing a key string for the ability to remove it later.

            Parameters:
                key - Key string for the ability to remove it later
                adder - GUI Object that added the event listener, nullptr if non-GUI Object added the listener
                callback - Listener callback function when the event happens*/
            virtual void AddEventListener(std::string key, ssGUI::GUIObject* adder, std::function<void(EventInfo)> callback) = 0;
            
            //function: AddEventListener
            //Same as above but with adder as nullptr
            virtual void AddEventListener(std::string key, std::function<void(EventInfo)> callback) = 0;
            
            //function: IsEventListenerExist
            //Check if the listener exists with key and adder specified. Note that same key but with different adder is diffentiated.
            virtual bool IsEventListenerExist(std::string key, ssGUI::GUIObject* adder) = 0;
            
            //function: IsEventListenerExist
            //Same as above but with adder as nullptr
            virtual bool IsEventListenerExist(std::string key) = 0;

            //function: RemoveEventListener
            //Removes a listener with the key and adder specified
            virtual void RemoveEventListener(std::string key, ssGUI::GUIObject* adder) = 0;

            //function: RemoveEventListener
            //Same as above but with adder as nullptr
            virtual void RemoveEventListener(std::string key) = 0;

            //function: ClearEventListeners
            //Clears all event listeners
            virtual void ClearEventListeners() = 0;
            
            //function: GetEventListenerCount
            //Returns the number of event listeners
            virtual int GetEventListenerCount() const = 0;
            
            //function: Notify
            //Call this function to trigger this event callback
            virtual void Notify(ssGUI::GUIObject* source) = 0;
            
            //function: BindToObject
            //Binds this event callback to a GUI object
            virtual void BindToObject(ssGUI::GUIObject* bindObj) = 0;

            //function: AddObjectReference
            //Proxy function to <ObjectsReferences::AddObjectReference>
            virtual ssGUIObjectIndex AddObjectReference(ssGUI::GUIObject* obj) = 0;

            //function: GetObjectReference
            //Proxy function to <ObjectsReferences::GetObjectReference>
            virtual ssGUI::GUIObject* GetObjectReference(ssGUIObjectIndex index) const = 0;

            //function: RemoveObjectReference
            //Proxy function to <ObjectsReferences::RemoveObjectReference>
            virtual void RemoveObjectReference(ssGUIObjectIndex index) = 0;

            //function: Internal_GetObjectsReferences
            //(Internal ssGUI function) Returns the <ObjectsReferences> associated with this event callback
            virtual ssGUI::ObjectsReferences* Internal_GetObjectsReferences() = 0;

            //function: GetEventCallbackName
            //Returns the name of this event callback
            virtual std::string GetEventCallbackName() const = 0;
            
            //function: Clone
            //Clone this event callback with the option of copying the listeners
            virtual EventCallback* Clone(ssGUI::GUIObject* newContainer, bool copyListeners) = 0;
    };
}

#endif