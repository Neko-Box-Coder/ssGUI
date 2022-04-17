#include "ssGUI/EventCallbacks/OnWindowCloseEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string OnWindowCloseEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }
    
    OnWindowCloseEventCallback* OnWindowCloseEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        OnWindowCloseEventCallback* temp;
        
        if(copyListeners)
            temp = new OnWindowCloseEventCallback(*this);
        else
            temp = new OnWindowCloseEventCallback();
        
        if(newContainer != nullptr)
            newContainer->AddEventCallback(temp);
        
        return temp;
    }

    const std::string OnWindowCloseEventCallback::EVENT_NAME = "OnWindowCloseEvent";
    
}