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
        ssGUI::GUIObject* Container;                                                        //See <BindToObject>
        ssGUI::ObjectsReferences CurrentObjectsReferences;                                  //See <GetObjectReference> and <Internal_GetObjectsReferences>
        ssGUI::Enums::EventType CurrentEventType;                                           //See <GetEventType>
    =================================================================
    ============================== C++ ==============================
    BaseEventCallback::BaseEventCallback() :    EventListeners(),
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
            //See <EventCallback::AddEventListener>
            virtual void AddEventListener(std::string key, ssGUI::GUIObject* adder, std::function<void(EventInfo)> callback);
            
            //function: AddEventListener
            //See <EventCallback::AddEventListener>
            virtual void AddEventListener(std::string key, std::function<void(EventInfo)> callback);
            
            //function: IsEventListenerExist
            //See <EventCallback::IsEventListenerExist>
            virtual bool IsEventListenerExist(std::string key, ssGUI::GUIObject* adder);

            //function: IsEventListenerExist
            //See <EventCallback::IsEventListenerExist>
            virtual bool IsEventListenerExist(std::string key);

            //function: RemoveEventListener
            //See <EventCallback::RemoveEventListener>
            virtual void RemoveEventListener(std::string key, ssGUI::GUIObject* adder);

            //function: RemoveEventListener
            //See <EventCallback::RemoveEventListener>
            virtual void RemoveEventListener(std::string key);

            //function: ClearEventListeners
            //See <EventCallback::ClearEventListeners>
            virtual void ClearEventListeners();
            
            //function: GetEventListenerCount
            //See <EventCallback::GetEventListenerCount>
            virtual int GetEventListenerCount() const;
            
            //function: Notify
            //See <EventCallback::Notify>
            virtual void Notify(ssGUI::GUIObject* source, void* customInfo = nullptr);
            
            //function: BindToObject
            //See <EventCallback::BindToObject>
            virtual void BindToObject(ssGUI::GUIObject* bindObj);

            //function: AddObjectReference
            //See <EventCallback::AddObjectReference>
            virtual ssGUI::ssGUIObjectIndex AddObjectReference(ssGUI::GUIObject* obj);

            //function: GetObjectReference
            //See <EventCallback::GetObjectReference>
            virtual ssGUI::GUIObject* GetObjectReference(ssGUI::ssGUIObjectIndex index) const;

            //function: RemoveObjectReference
            //See <EventCallback::RemoveObjectReference>
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
            //See <EventCallback::Clone>
            virtual EventCallback* Clone(bool copyListeners);
    };

}

#endif