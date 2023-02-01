#ifndef SSGUI_BASE_EVENT_CALLBACK_H
#define SSGUI_BASE_EVENT_CALLBACK_H

#include "ssGUI/EventCallbacks/EventCallback.hpp"
#include "ssGUI/GUIObjectClasses/GUIObject.hpp"
#include <queue>

namespace ssGUI
{

//namespace: ssGUI::EventCallbacks
namespace EventCallbacks
{
    /*class: ssGUI::EventCallbacks::BaseEventCallback
    Implementation class for <EventCallback>
    
    Variables & Constructor:
    ============================== C++ ==============================
    private:
        //Events
        std::unordered_map<std::string, std::function<void(EventInfo)>> EventListeners;     //See <AddEventListener>            
        ssGUI::GUIObject* Container;                                                        //See <BindToObject>
        ssGUI::ObjectsReferences CurrentObjectsReferences;                                  //See <GetObjectReference> and <Internal_GetObjectsReferences>
    =================================================================
    ============================== C++ ==============================
    BaseEventCallback::BaseEventCallback() :    EventListeners(),
                                                Container(nullptr),
                                                CurrentObjectsReferences()
    {}
    =================================================================
    */
    class BaseEventCallback : public EventCallback
    {
        public:
            friend class ssGUI::Factory;
        
        private:
            //Events
            std::unordered_map<std::string, std::function<void(EventInfo)>> EventListeners;     //See <AddEventListener>            
            ssGUI::GUIObject* Container;                                                        //See <BindToObject>
            ssGUI::ObjectsReferences CurrentObjectsReferences;                                  //See <GetObjectReference> and <Internal_GetObjectsReferences>
        
        protected:
            BaseEventCallback();
            ~BaseEventCallback();
            BaseEventCallback(BaseEventCallback const & other) = default;
            BaseEventCallback& operator=(BaseEventCallback const & other) = default;
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};
            
            #define INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(eventType, newContainer, copyListeners)\
            [&](){\
                eventType* temp;\
                if(copyListeners)\
                    temp = new eventType(*this);\
                else\
                    temp = new eventType();\
                if(newContainer != nullptr)\
                    newContainer->AddEventCallback(temp);\
                return temp;\
            }()

        public:
            //function: AddEventListener
            //See <EventCallback::AddEventListener>
            virtual void AddEventListener(std::string key, ssGUI::GUIObject* adder, std::function<void(EventInfo)> callback) override;
            
            //function: AddEventListener
            //See <EventCallback::AddEventListener>
            virtual void AddEventListener(std::string key, std::function<void(EventInfo)> callback) override;
            
            //function: IsEventListenerExist
            //See <EventCallback::IsEventListenerExist>
            virtual bool IsEventListenerExist(std::string key, ssGUI::GUIObject* adder) override;

            //function: IsEventListenerExist
            //See <EventCallback::IsEventListenerExist>
            virtual bool IsEventListenerExist(std::string key) override;

            //function: RemoveEventListener
            //See <EventCallback::RemoveEventListener>
            virtual void RemoveEventListener(std::string key, ssGUI::GUIObject* adder) override;

            //function: RemoveEventListener
            //See <EventCallback::RemoveEventListener>
            virtual void RemoveEventListener(std::string key) override;

            //function: ClearEventListeners
            //See <EventCallback::ClearEventListeners>
            virtual void ClearEventListeners() override;
            
            //function: GetEventListenerCount
            //See <EventCallback::GetEventListenerCount>
            virtual int GetEventListenerCount() const override;
            
            //function: Notify
            //See <EventCallback::Notify>
            virtual void Notify(ssGUI::GUIObject* source) override;
            
            //function: BindToObject
            //See <EventCallback::BindToObject>
            virtual void BindToObject(ssGUI::GUIObject* bindObj) override;

            //function: AddObjectReference
            //See <EventCallback::AddObjectReference>
            virtual ssGUI::ssGUIObjectIndex AddObjectReference(ssGUI::GUIObject* obj) override;

            //function: GetObjectReference
            //See <EventCallback::GetObjectReference>
            virtual ssGUI::GUIObject* GetObjectReference(ssGUI::ssGUIObjectIndex index) const override;

            //function: RemoveObjectReference
            //See <EventCallback::RemoveObjectReference>
            virtual void RemoveObjectReference(ssGUI::ssGUIObjectIndex index) override;

            //function: Internal_GetObjectsReferences
            //(Internal ssGUI function) Returns structure that contains all the referenced GUI Objects. 
            virtual ssGUI::ObjectsReferences* Internal_GetObjectsReferences() override;

            //function: GetEventCallbackName
            //See <EventCallback::GetEventCallbackName>
            virtual std::string GetEventCallbackName() const override;
            
            //function: Clone
            //See <EventCallback::Clone>
            virtual BaseEventCallback* Clone(ssGUI::GUIObject* newContainer, bool copyListeners) override;

            //const: EVENT_NAME
            //This is what you use in order to get the EventCallback that is added to the GUIObject
            static const std::string EVENT_NAME;
    };
}

}

#endif