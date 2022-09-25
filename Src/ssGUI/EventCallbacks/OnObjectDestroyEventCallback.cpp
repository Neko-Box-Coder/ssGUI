#include "ssGUI/EventCallbacks/OnObjectDestroyEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string OnObjectDestroyEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    OnObjectDestroyEventCallback* OnObjectDestroyEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        OnObjectDestroyEventCallback* temp;
        
        if(copyListeners)
            temp = new OnObjectDestroyEventCallback(*this);
        else
            temp = new OnObjectDestroyEventCallback();
        
        if(newContainer != nullptr)
            newContainer->AddEventCallback(temp);
        
        return temp;
    }

    const std::string OnObjectDestroyEventCallback::EVENT_NAME = "OnObjectDestroyEvent";
}