#include "ssGUI/EventCallback.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"
#include <cassert>

namespace ssGUI
{

    EventCallback::EventCallback() :    EventListeners(),
                                        EventListenersOrder(),
                                        Container(nullptr),
                                        CurrentObjectsReferences(),
                                        CurrentEventType(ssGUI::Enums::EventType::NONE)
    {}

    EventCallback::~EventCallback()
    {
        CurrentObjectsReferences.CleanUp();
    }
    
    void EventCallback::AddEventListener(std::string key, ssGUI::GUIObject* adder, std::function<void(EventInfo&)> callback)
    {
        if(adder != nullptr)
        {
            int adderIndex = CurrentObjectsReferences.AddObjectReference(adder);
            std::string listenerKey = key+"-adder-"+std::to_string(adderIndex);
            if(EventListeners.find(listenerKey) == EventListeners.end())
                EventListenersOrder.push_back(listenerKey);

            EventListeners[listenerKey] = callback;
        }
        else
        {
            if(EventListeners.find(key) == EventListeners.end())
                EventListenersOrder.push_back(key);
            
            EventListeners[key] = callback;
        }
        
        assert(EventListeners.size() == EventListenersOrder.size());
    }

    void EventCallback::AddEventListener(std::string key, std::function<void(EventInfo&)> callback)
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
            std::string listenerKey = key+"-adder-"+std::to_string(adderIndex);
            EventListeners.erase(listenerKey);
            
            for(int i = 0; i < EventListenersOrder.size(); i++)
            {
                if(EventListenersOrder[i] == listenerKey)
                {
                    EventListenersOrder.erase(EventListenersOrder.begin() + i);
                    break;
                }
            }
        }
        else
        {
            EventListeners.erase(key);
            
            for(int i = 0; i < EventListenersOrder.size(); i++)
            {
                if(EventListenersOrder[i] == key)
                {
                    EventListenersOrder.erase(EventListenersOrder.begin() + i);
                    break;
                }
            }
        }
    }

    void EventCallback::RemoveEventListener(std::string key)
    {
        RemoveEventListener(key, nullptr);
    }

    void EventCallback::SetEventListenerOrder(std::string key, ssGUI::GUIObject* adder, int order)
    {
        if(order < 0 || order >= EventListenersOrder.size())
        {
            ssGUI_WARNING(0, "Invalid order index: "<<order);
            return;
        }
    
        std::string listenerKey = key;
    
        if(adder != nullptr)
        {
            int adderIndex = CurrentObjectsReferences.GetObjectIndex(adder);
            listenerKey += "-adder-"+std::to_string(adderIndex);
        }

        for(int i = 0; i < EventListenersOrder.size(); i++)
        {
            if(EventListenersOrder[i] == listenerKey)
            {
                std::string temp = EventListenersOrder[i];
                EventListenersOrder[i] = EventListenersOrder[order];
                EventListenersOrder[order] = temp;
                return;
            }
        }
        
        ssGUI_WARNING(0, "Failed to find event listener with key: "<<key);
    }

    void EventCallback::SetEventListenerOrder(std::string key, int order)
    {
        SetEventListenerOrder(key, nullptr, order);
    }

    int EventCallback::GetEventListenerOrder(std::string key, ssGUI::GUIObject* adder) const
    {
        std::string listenerKey = key;
    
        if(adder != nullptr)
        {
            int adderIndex = CurrentObjectsReferences.GetObjectIndex(adder);
            listenerKey += "-adder-"+std::to_string(adderIndex);
        }
        
        for(int i = 0; i < EventListenersOrder.size(); i++)
        {
            if(EventListenersOrder[i] == listenerKey)
            {
                return i;
            }
        }
        
        ssGUI_WARNING(0, "Failed to find event listener with key: "<<key);
        return -1;
    }

    int EventCallback::GetEventListenerOrder(std::string key) const
    {
        return GetEventListenerOrder(key, nullptr);
    }

    void EventCallback::ClearEventListeners()
    {
        EventListeners.clear();
        EventListenersOrder.clear();
    }

    int EventCallback::GetEventListenerCount() const
    {
        return EventListeners.size();
    }

    void EventCallback::Notify(ssGUI::GUIObject* source, void* customInfo)
    {
        ssGUI_LOG_FUNC();
        assert(EventListeners.size() == EventListenersOrder.size());
        
        for(int i = 0; i < EventListenersOrder.size(); i++)
        {
            ssGUI::EventInfo info;
            info.EventSource = source;
            info.Container = Container;
            info.References = &CurrentObjectsReferences;
            info.DeleteCurrentListener = false;
            info.CustomInfo = customInfo;
            EventListeners.at(EventListenersOrder[i])(info);
        
            if(info.DeleteCurrentListener)
            {
                EventListeners.erase(EventListenersOrder[i]);
                EventListenersOrder.erase(EventListenersOrder.begin() + i);
                i--;
            }
        }
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