#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"


namespace ssGUI::EventCallbacks
{    
    BaseEventCallback::BaseEventCallback() : EventListeners(), EventListenersValid(), NextFreeIndices(), EventListenerCount(0)
    {}
    
    int BaseEventCallback::AddEventListener(std::function<void(ssGUI::GUIObject*)> callback)
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
                EventListeners[i](source);
        }
    }

    std::string BaseEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    EventCallback* BaseEventCallback::Clone(ssGUI::GUIObject* container, std::vector<ssGUI::GUIObject*>& originalObjs, std::vector<ssGUI::GUIObject*>& clonedObjs)
    {
        return new BaseEventCallback();
    }

    // ssGUI::EventCallbacks::EventCallback* BaseEventCallback::Clone()
    // {
    //     return new ssGUI::EventCallbacks::BaseEventCallback(*this);
    // }

    const std::string BaseEventCallback::EVENT_NAME = "BaseEvent";
}