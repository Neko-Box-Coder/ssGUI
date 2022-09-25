#include "ssGUI/EventCallbacks/OnRecursiveChildAddEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string OnRecursiveChildAddEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    OnRecursiveChildAddEventCallback* OnRecursiveChildAddEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        OnRecursiveChildAddEventCallback* temp;
        
        if(copyListeners)
            temp = new OnRecursiveChildAddEventCallback(*this);
        else
            temp = new OnRecursiveChildAddEventCallback();
        
        if(newContainer != nullptr)
            newContainer->AddEventCallback(temp);
        
        return temp;
    }

    const std::string OnRecursiveChildAddEventCallback::EVENT_NAME = "OnRecursiveChildAddEvent";
}