#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

#include "ssLogger/ssLog.hpp"

namespace ssGUI
{

namespace EventCallbacks
{    
    BaseEventCallback::BaseEventCallback() :    EventListeners(),
                                                Container(nullptr),
                                                CurrentObjectsReferences()
    {}

    BaseEventCallback::~BaseEventCallback()
    {
        CurrentObjectsReferences.CleanUp();
    }
    
    void BaseEventCallback::AddEventListener(std::string key, ssGUI::GUIObject* adder, std::function<void(EventInfo)> callback)
    {
        if(adder != nullptr)
        {
            int adderIndex = CurrentObjectsReferences.AddObjectReference(adder);
            EventListeners[key+"-adder-"+std::to_string(adderIndex)] = callback;
        }
        else
            EventListeners[key] = callback;
    }

    void BaseEventCallback::AddEventListener(std::string key, std::function<void(EventInfo)> callback)
    {
        AddEventListener(key, nullptr, callback);
    }
    
    bool BaseEventCallback::IsEventListenerExist(std::string key, ssGUI::GUIObject* adder)
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

    bool BaseEventCallback::IsEventListenerExist(std::string key)
    {
        return IsEventListenerExist(key, nullptr);
    }

    void BaseEventCallback::RemoveEventListener(std::string key, ssGUI::GUIObject* adder)
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

    void BaseEventCallback::RemoveEventListener(std::string key)
    {
        RemoveEventListener(key, nullptr);
    }

    void BaseEventCallback::ClearEventListeners()
    {
        EventListeners.clear();
    }

    int BaseEventCallback::GetEventListenerCount() const
    {
        return EventListeners.size();
    }

    void BaseEventCallback::Notify(ssGUI::GUIObject* source)
    {
        ssLOG_FUNC_ENTRY();
        for(auto it = EventListeners.begin(); it != EventListeners.end(); it++)
        {
            ssGUI::EventInfo info;
            info.EventSource = source;
            info.EventCallbackContainer = Container;
            info.EventCallbackReferences = &CurrentObjectsReferences;
            it->second(info);
        }
        ssLOG_FUNC_EXIT();
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

    BaseEventCallback* BaseEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(BaseEventCallback, newContainer, copyListeners);
    }

    const std::string BaseEventCallback::EVENT_NAME = "BaseEvent";
}

}