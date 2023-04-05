#include "ssGUI/EventCallback.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

    EventCallback::EventCallback() :    EventListeners(),
                                        Container(nullptr),
                                        CurrentObjectsReferences(),
                                        CurrentEventType(ssGUI::Enums::EventType::NONE)
    {}

    EventCallback::~EventCallback()
    {
        CurrentObjectsReferences.CleanUp();
    }
    
    void EventCallback::AddEventListener(std::string key, ssGUI::GUIObject* adder, std::function<void(EventInfo)> callback)
    {
        if(adder != nullptr)
        {
            int adderIndex = CurrentObjectsReferences.AddObjectReference(adder);
            EventListeners[key+"-adder-"+std::to_string(adderIndex)] = callback;
        }
        else
            EventListeners[key] = callback;
    }

    void EventCallback::AddEventListener(std::string key, std::function<void(EventInfo)> callback)
    {
        AddEventListener(key, nullptr, callback);
    }
    
    bool EventCallback::IsEventListenerExist(std::string key, ssGUI::GUIObject* adder)
    {
        if(adder != nullptr)
        {
            int adderIndex = CurrentObjectsReferences.GetObjectIndex(adder);
            if(adderIndex == -1)
                return false;
            
            return EventListeners.find(key+"-adder-"+std::to_string(adderIndex)) != EventListeners.end();
        }

        else
            return EventListeners.find(key) != EventListeners.end();
    }

    bool EventCallback::IsEventListenerExist(std::string key)
    {
        return IsEventListenerExist(key, nullptr);
    }

    void EventCallback::RemoveEventListener(std::string key, ssGUI::GUIObject* adder)
    {
        if(!IsEventListenerExist(key, adder))
            return;

        if(adder != nullptr)
        {
            int adderIndex = CurrentObjectsReferences.GetObjectIndex(adder);
            EventListeners.erase(key+"-adder-"+std::to_string(adderIndex));
        }
        else
            EventListeners.erase(key);
    }

    void EventCallback::RemoveEventListener(std::string key)
    {
        RemoveEventListener(key, nullptr);
    }

    void EventCallback::ClearEventListeners()
    {
        EventListeners.clear();
    }

    int EventCallback::GetEventListenerCount() const
    {
        return EventListeners.size();
    }

    void EventCallback::Notify(ssGUI::GUIObject* source, void* customInfo)
    {
        ssLOG_FUNC_ENTRY();
        for(auto it = EventListeners.begin(); it != EventListeners.end(); it++)
        {
            ssGUI::EventInfo info;
            info.EventSource = source;
            info.Container = Container;
            info.References = &CurrentObjectsReferences;
            info.CustomInfo = customInfo;
            it->second(info);
        }
        ssLOG_FUNC_EXIT();
    }

    void EventCallback::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;
    }

    ssGUIObjectIndex EventCallback::AddObjectReference(ssGUI::GUIObject* obj)
    {
        return CurrentObjectsReferences.AddObjectReference(obj);
    }

    ssGUI::GUIObject* EventCallback::GetObjectReference(ssGUIObjectIndex index) const
    {
        return CurrentObjectsReferences.GetObjectReference(index);
    }

    void EventCallback::RemoveObjectReference(ssGUIObjectIndex index)
    {
        CurrentObjectsReferences.RemoveObjectReference(index);
    }

    ObjectsReferences* EventCallback::Internal_GetObjectsReferences()
    {
        return &CurrentObjectsReferences;
    }

    void EventCallback::SetEventType(ssGUI::Enums::EventType eventType)
    {
        CurrentEventType = eventType;
    }
    
    ssGUI::Enums::EventType EventCallback::GetEventType() const
    {
        return CurrentEventType;
    }

    EventCallback* EventCallback::Clone(bool copyListeners)
    {
        EventCallback* temp;
        if(copyListeners)
            temp = new EventCallback(*this);
        else
            temp = new EventCallback();
        return temp;
    }

}