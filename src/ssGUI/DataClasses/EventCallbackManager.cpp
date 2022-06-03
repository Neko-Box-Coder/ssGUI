#include "ssGUI/DataClasses/EventCallbackManager.hpp"

#include "ssGUI/DataClasses/Renderer.hpp"
#include "ssGUI/Factory.hpp"

namespace ssGUI
{
    EventCallbackManager::EventCallbackManager(EventCallbackManager const& other)
    {
        EventCallbacks = std::unordered_map<std::string, ssGUI::EventCallbacks::EventCallback*>();
        CurrentRenderer = nullptr;
        CurrentObject = nullptr;
    }
    
    EventCallbackManager::EventCallbackManager() : EventCallbacks(), CurrentRenderer(nullptr), CurrentObject(nullptr)
    {}

    EventCallbackManager::~EventCallbackManager()
    {

    }

    void EventCallbackManager::SetDependentComponents(ssGUI::Renderer* renderer, ssGUI::GUIObject* obj)
    {
        CurrentRenderer = renderer;
        CurrentObject = obj;
    }

    void EventCallbackManager::AddEventCallback(ssGUI::EventCallbacks::EventCallback* eventCallback)
    {
        if(IsEventCallbackExist(eventCallback->GetEventCallbackName()))
            return;

        EventCallbacks[eventCallback->GetEventCallbackName()] = eventCallback;
        eventCallback->BindToObject(CurrentObject);
        CurrentRenderer->RedrawObject();
    }

    ssGUI::EventCallbacks::EventCallback* EventCallbackManager::GetEventCallback(std::string eventCallbackName)
    {
        if(!IsEventCallbackExist(eventCallbackName))
            return nullptr;

        return EventCallbacks[eventCallbackName];
    }

    bool EventCallbackManager::IsEventCallbackExist(std::string eventCallbackName) const
    {
        return EventCallbacks.find(eventCallbackName) != EventCallbacks.end();
    }

    void EventCallbackManager::RemoveEventCallback(std::string eventCallbackName)
    {
        if(!IsEventCallbackExist(eventCallbackName))
            return;

        auto ecb = EventCallbacks.at(eventCallbackName);
        EventCallbacks.erase(eventCallbackName);
        ssGUI::Factory::Dispose(ecb);
        CurrentRenderer->RedrawObject();
    }

    std::vector<ssGUI::EventCallbacks::EventCallback*> EventCallbackManager::GetListOfEventCallbacks()
    {
        std::vector<ssGUI::EventCallbacks::EventCallback*> returnVector = std::vector<ssGUI::EventCallbacks::EventCallback*>();
        
        for(auto eventCallback : EventCallbacks)
            returnVector.push_back(eventCallback.second);
        
        return returnVector;
    }




}