#ifndef SSGUI_BASE_EVENT_CALLBACK_H
#define SSGUI_BASE_EVENT_CALLBACK_H

#include "ssGUI/DataClasses/ObjectsReferences.hpp"
#include "ssGUI/Factory.hpp"
#include "ssGUI/Enums/EventType.hpp"
#include "ssGUI/DataClasses/EventInfo.hpp"
#include <functional>
#include <queue>
#include <unordered_map>

namespace ssGUI
{

    /*class: ssGUI::EventCallback    
    EventCallback enables the ablity to act against a certain event. 

    In order to add a class's function as a listener to an event callback
    > int index = EventCallback->AddEventListener("AnyKey", std::bind(&some::class::function, objectPointer, std::placeholders::_1));
    
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
    ===========================================================================
    
    Variables & Constructor:
    ============================== C++ ==============================
    private:
        //Events
        std::unordered_map<std::string, std::function<void(EventInfo)>> EventListeners;     //See <AddEventListener>            
        std::vector<std::string> EventListenersOrder;                                       //See <GetEventListenerOrder>
        ssGUI::GUIObject* Container;                                                        //See <BindToObject>
        ssGUI::ObjectsReferences CurrentObjectsReferences;                                  //See <GetObjectReference> and <Internal_GetObjectsReferences>
        ssGUI::Enums::EventType CurrentEventType;                                           //See <GetEventType>
    =================================================================
    ============================== C++ ==============================
    BaseEventCallback::BaseEventCallback() :    EventListeners(),
                                                EventListenersOrder(),
                                                Container(nullptr),
                                                CurrentObjectsReferences(),
                                                CurrentEventType(ssGUI::Enums::EventType::NONE)
    {}
    =================================================================
    */
    class EventCallback
    {
        public:
            friend class ssGUI::Factory;
        
        private:
            //Events
            std::unordered_map<std::string, std::function<void(EventInfo)>> EventListeners;     //See <AddEventListener>            
            std::vector<std::string> EventListenersOrder;                                       //See <GetEventListenerOrder>
            ssGUI::GUIObject* Container;                                                        //See <BindToObject>
            ssGUI::ObjectsReferences CurrentObjectsReferences;                                  //See <GetObjectReference> and <Internal_GetObjectsReferences>
            ssGUI::Enums::EventType CurrentEventType;                                           //See <GetEventType>
        
        protected:
            EventCallback();
            virtual ~EventCallback();
            EventCallback(EventCallback const & other) = default;
            EventCallback& operator=(EventCallback const & other) = default;
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};
            
        public:
            //function: AddEventListener
            //Adds a callback listener function with key and adder for unique identification which triggers when <Notify> is called
            virtual void AddEventListener(std::string key, ssGUI::GUIObject* adder, std::function<void(EventInfo)> callback);
            
            //function: AddEventListener
            //See <AddEventListener> where adder is nullptr
            virtual void AddEventListener(std::string key, std::function<void(EventInfo)> callback);
            
            //function: IsEventListenerExist
            //Checks if the callback listener function with unique identification key and adder exists
            virtual bool IsEventListenerExist(std::string key, ssGUI::GUIObject* adder);

            //function: IsEventListenerExist
            //See <IsEventListenerExist> where adder is nullptr
            virtual bool IsEventListenerExist(std::string key);

            //function: RemoveEventListener
            //Removes the callback listener function with unique identification key and adder
            virtual void RemoveEventListener(std::string key, ssGUI::GUIObject* adder);

            //function: RemoveEventListener
            //See <RemoveEventListener> where adder is nullptr
            virtual void RemoveEventListener(std::string key);

            //function: SetEventListenerOrder
            //Sets the order for the callback listener function with unique identification key and adder being called from <Notify> 
            virtual void SetEventListenerOrder(std::string key, ssGUI::GUIObject* adder, int order);
            
            //function: SetEventListenerOrder
            //See <SetEventListenerOrder> where adder is nullptr
            virtual void SetEventListenerOrder(std::string key, int order);
            
            //function: GetEventListenerOrder
            //Gets the order for the callback listener function with unique identification key and adder being called from <Notify>.
            //Returns -1 if such event callback function with unique identification key and adder is not found.
            virtual int GetEventListenerOrder(std::string key, ssGUI::GUIObject* adder) const;
            
            //function: GetEventListenerOrder
            //See <GetEventListenerOrder> where adder is nullptr
            virtual int GetEventListenerOrder(std::string key) const;

            //function: ClearEventListeners
            //Removes all callback listener functions.
            virtual void ClearEventListeners();
            
            //function: GetEventListenerCount
            //Gets the number of callback listener functions
            virtual int GetEventListenerCount() const;
            
            //function: Notify
            //Calls all the callback listener functions added from <AddEventListener>
            virtual void Notify(ssGUI::GUIObject* source, void* customInfo = nullptr);
            
            //function: BindToObject
            //Binds this event callback to a GUI Object
            virtual void BindToObject(ssGUI::GUIObject* bindObj);

            //function: AddObjectReference
            //Adds a reference to a GUI Object that can be retrieved using the returned ssGUIObjectIndex
            //inside the callback listener functions
            virtual ssGUI::ssGUIObjectIndex AddObjectReference(ssGUI::GUIObject* obj);

            //function: GetObjectReference
            //Gets the GUI Object using the ssGUIObjectIndex obtained when calling <AddObjectReference>
            virtual ssGUI::GUIObject* GetObjectReference(ssGUI::ssGUIObjectIndex index) const;

            //function: RemoveObjectReference
            //Removes the reference to the GUI Object using the ssGUIObjectIndex obtained when calling <AddObjectReference>
            virtual void RemoveObjectReference(ssGUI::ssGUIObjectIndex index);

            //function: Internal_GetObjectsReferences
            //(Internal ssGUI function) Returns structure that contains all the referenced GUI Objects. 
            virtual ssGUI::ObjectsReferences* Internal_GetObjectsReferences();

            //function: SetEventType
            //This sets the type of the event callback, normally you don't need to set this.
            virtual void SetEventType(ssGUI::Enums::EventType eventType);

            //function: GetEventType
            //This gets the type of the event callback
            virtual ssGUI::Enums::EventType GetEventType() const;
            
            //function: Clone
            //This creates a copy of this event callback with the option of also copying
            //the callback listener functions 
            virtual EventCallback* Clone(bool copyListeners);
    };

}

#endif