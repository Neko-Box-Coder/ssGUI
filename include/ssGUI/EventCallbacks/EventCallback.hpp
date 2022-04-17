#ifndef SSGUI_EVENT_CALLBACK
#define SSGUI_EVENT_CALLBACK

#include "ssGUI/Factory.hpp"
#include "ssGUI/DataClasses/ObjectsReferences.hpp"
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
    > int index = EventCallback->AddEventListener(std::bind(&some::class::function, objectPointer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    
    Alternatively, you can add a lambda as an event listener to an <EventCallback> object.
    =================================== C++ ===================================
    int index = EventCallback->AddEventListener
    (
        //source is what *triggered* the callback
        //container is the GUI Object that holds this event callback
        [](ssGUI::GUIObject* source, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* references)
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

            Adds a listener to this EventCallback, and returns an index for removing it.

            Parameters:
                src - GUI Object that triggered the callback
                container - GUI Object that contains this eventcallback
                references - <ObjectsReferences> that this event callback has*/ 
            virtual int AddEventListener(std::function<void(ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* references)> callback) = 0;
            
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