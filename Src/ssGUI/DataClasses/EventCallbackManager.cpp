#include "ssGUI/DataClasses/EventCallbackManager.hpp"

#include "ssGUI/DataClasses/Renderer.hpp"
#include "ssGUI/Factory.hpp"
#include "ssGUI/GUIObjectClasses/GUIObject.hpp"

namespace ssGUI
{
    EventCallbackManager::EventCallbackManager(EventCallbackManager const& other)
    {
        EventCallbacks = std::unordered_map<ssGUI::Enums::EventType, ssGUI::EventCallback*>();
        CurrentRenderer = nullptr;
        CurrentObject = nullptr;
    }
    
    EventCallbackManager::EventCallbackManager() :  EventCallbacks(),
                                                    CurrentRenderer(nullptr),
                                                    CurrentObject(nullptr)
    {}

    EventCallbackManager::~EventCallbackManager()
    {

    }

    void EventCallbackManager::SetDependentComponents(ssGUI::Renderer* renderer, ssGUI::GUIObject* obj)
    {
        CurrentRenderer = renderer;
        CurrentObject = obj;
    }
    
    ssGUI::EventCallback* EventCallbackManager::AddEventCallback(ssGUI::Enums::EventType eventType)
    {
        if(IsEventCallbackExist(eventType))
            return GetEventCallback(eventType);
                        
        ssGUI::EventCallback* eventCallback = ssGUI::Factory::Create<ssGUI::EventCallback>();
        eventCallback->SetEventType(eventType);
        EventCallbacks[eventCallback->GetEventType()] = eventCallback;
        eventCallback->BindToObject(CurrentObject);
        CurrentRenderer->RedrawObject();
        return eventCallback;
    }
    
    ssGUI::EventCallback* EventCallbackManager::AddEventCallbackCopy(ssGUI::EventCallback* copy, bool copyListeners)
    {
        if(IsEventCallbackExist(copy->GetEventType()))
            return GetEventCallback(copy->GetEventType());
                        
        ssGUI::EventCallback* eventCallback = copy->Clone(copyListeners);
        EventCallbacks[eventCallback->GetEventType()] = eventCallback;
        eventCallback->BindToObject(CurrentObject);
        CurrentRenderer->RedrawObject();
        return eventCallback;
    }

    ssGUI::EventCallback* EventCallbackManager::GetEventCallback(ssGUI::Enums::EventType eventType)
    {
        if(!IsEventCallbackExist(eventType))
            return nullptr;

        return EventCallbacks[eventType];
    }

    bool EventCallbackManager::IsEventCallbackExist(ssGUI::Enums::EventType eventType) const
    {
        return EventCallbacks.find(eventType) != EventCallbacks.end();
    }

    void EventCallbackManager::RemoveEventCallback(ssGUI::Enums::EventType eventType)
    {
        if(!IsEventCallbackExist(eventType))
            return;

        auto ecb = EventCallbacks.at(eventType);
        EventCallbacks.erase(eventType);
        ssGUI::Factory::Dispose(ecb);
        CurrentRenderer->RedrawObject();
    }

    std::vector<ssGUI::EventCallback*> EventCallbackManager::GetListOfEventCallbacks()
    {
        std::vector<ssGUI::EventCallback*> returnVector = std::vector<ssGUI::EventCallback*>();
        
        for(auto eventCallback : EventCallbacks)
            returnVector.push_back(eventCallback.second);
        
        return returnVector;
    }
    
    void EventCallbackManager::ForwardEvent(ssGUI::GUIObject* objToForwardTo, ssGUI::Enums::EventType forwardEvent)
    {
        if(objToForwardTo == nullptr)
        {
            ssGUI_WARNING(ssGUI_TAG_DATA, "Can't forward event to nullptr GUI Object");
            return;
        }
    
        auto* ecb = AddEventCallback(forwardEvent);
        ssGUI::ssGUIObjectIndex forwardObjId = ecb->AddObjectReference(objToForwardTo);
        ecb->AddEventListener
        (
            "Event Callback Manager",
            CurrentObject,
            [forwardObjId, forwardEvent](ssGUI::EventInfo& info)
            {
                ssGUI::GUIObject* forwardToObj = info.References->GetObjectReference(forwardObjId);
                if(forwardToObj == nullptr)
                    return;

                if(forwardToObj->IsEventCallbackExist(forwardEvent))
                    forwardToObj->GetEventCallback(forwardEvent)->Notify(info.EventSource, info.CustomInfo);        
            }
        );
    }
}