#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"


namespace ssGUI::EventCallbacks
{    
    BaseEventCallback::BaseEventCallback() : EventListeners(), EventListenersValid(), NextFreeIndices(), EventListenerCount(0),
                                                Container(nullptr), CurrentObjectsReferences()
    {}

    BaseEventCallback::~BaseEventCallback()
    {
        CurrentObjectsReferences.CleanUp();
    }
    
    int BaseEventCallback::AddEventListener(std::function<void(ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* references)> callback)
    {
        int addedIndex = -1;
        
        if(NextFreeIndices.empty())
        {
            EventListeners.push_back(callback);
            EventListenersValid.push_back(true);
            addedIndex = EventListeners.size() - 1;
        }
        else
        {
            addedIndex = NextFreeIndices.front();
            EventListeners[NextFreeIndices.front()] = callback;
            EventListenersValid[NextFreeIndices.front()] = true;
            NextFreeIndices.pop();
        }

        EventListenerCount++;

        return addedIndex;
    }

    void BaseEventCallback::RemoveEventListener(int index)
    {
        EventListenersValid[index] = false;
        NextFreeIndices.push(index);

        EventListenerCount--;
    }

    int BaseEventCallback::GetEventListenerCount() const
    {
        return EventListenerCount;
    }

    void BaseEventCallback::Notify(ssGUI::GUIObject* source)
    {
        for(int i = 0; i < EventListeners.size(); i++)
        {
            if(EventListenersValid[i] == true)
                EventListeners[i](source, Container, &CurrentObjectsReferences);
        }
    }

    void BaseEventCallback::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;
    }

    ssGUIObjectIndex BaseEventCallback::AddObjectReference(ssGUI::GUIObject* obj)
    {
        return CurrentObjectsReferences.AddObjectReference(obj);
    }

    ssGUI::GUIObject* BaseEventCallback::GetObjectReference(ssGUIObjectIndex index) const
    {
        return CurrentObjectsReferences.GetObjectReference(index);
    }

    void BaseEventCallback::RemoveObjectReference(ssGUIObjectIndex index)
    {
        CurrentObjectsReferences.RemoveObjectReference(index);
    }

    ObjectsReferences* BaseEventCallback::Internal_GetObjectsReferences()
    {
        return &CurrentObjectsReferences;
    }

    std::string BaseEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    EventCallback* BaseEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        BaseEventCallback* temp;
        
        if(copyListeners)
            temp = new BaseEventCallback(*this);
        else
            temp = new BaseEventCallback();
        
        if(newContainer != nullptr)
            newContainer->AddEventCallback(temp);
        
        return temp;
    }

    const std::string BaseEventCallback::EVENT_NAME = "BaseEvent";
}