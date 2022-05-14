#ifndef BASE_EVENT_CALLBACK
#define BASE_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/EventCallback.hpp"
#include "ssGUI/GUIObjectClasses/GUIObject.hpp"
#include <queue>

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    /*class: ssGUI::EventCallbacks::BaseEventCallback
    Implementation class for <EventCallback>
    
    Variables & Constructor:
    ============================== C++ ==============================
    private:
        //Events
        std::vector<std::function<void(ssGUI::GUIObject*)>> EventListeners;
        std::vector<bool> EventListenersValid;
        std::queue<int> NextFreeIndices;
        int EventListenerCount;
        ssGUI::GUIObject* Container;

        ssGUI::ObjectsReferences CurrentObjectsReferences;
    =================================================================
    ============================== C++ ==============================
    BaseEventCallback::BaseEventCallback() : EventListeners(), EventListenersValid(), NextFreeIndices(), EventListenerCount(0),
                                                Container(nullptr), CurrentObjectsReferences()
    {}
    =================================================================
    */
    class BaseEventCallback : public EventCallback
    {
        public:
            friend class ssGUI::Factory;
        
        private:
            //Events
            std::vector<std::function<void(ssGUI::GUIObject*, ssGUI::GUIObject*, ssGUI::ObjectsReferences*)>> EventListeners;
            std::vector<bool> EventListenersValid;
            std::queue<int> NextFreeIndices;
            int EventListenerCount;
            ssGUI::GUIObject* Container;

            ssGUI::ObjectsReferences CurrentObjectsReferences;
        
        protected:
            BaseEventCallback();
            ~BaseEventCallback();
            BaseEventCallback(BaseEventCallback const & other) = default;
            BaseEventCallback& operator=(BaseEventCallback const & other) = default;
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};

        public:
            //function: AddEventListener
            //See <EventCallback::AddEventListener>
            virtual int AddEventListener(std::function<void(ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* references)> callback) override;
            
            //function: RemoveEventListener
            //See <EventCallback::RemoveEventListener>
            virtual void RemoveEventListener(int index) override;

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
            //Returns all the referenced GUI Objects. If nullptr is returned, this extension is not referencing any GUI Object (exception for container).
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



#endif