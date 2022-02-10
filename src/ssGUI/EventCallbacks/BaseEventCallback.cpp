#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"


namespace ssGUI::EventCallbacks
{    
    BaseEventCallback::BaseEventCallback() : EventListeners(), EventListenersValid(), NextFreeIndices(), EventListenerCount(0),
                                                Container(nullptr)
    {}
    
    int BaseEventCallback::AddEventListener(std::function<void(ssGUI::GUIObject*, ssGUI::GUIObject*)> callback)
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
                EventListeners[i](source, Container);
        }
    }

    void BaseEventCallback::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;
    }

    std::string BaseEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    EventCallback* BaseEventCallback::Clone(ssGUI::GUIObject* container, bool copyListeners)
    {
        if(copyListeners)
            return new BaseEventCallback(*this);
        else
            return new BaseEventCallback();
    }

    const std::string BaseEventCallback::EVENT_NAME = "BaseEvent";
}