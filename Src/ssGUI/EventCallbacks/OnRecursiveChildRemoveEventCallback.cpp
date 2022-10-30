#include "ssGUI/EventCallbacks/OnRecursiveChildRemoveEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string OnRecursiveChildRemoveEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    OnRecursiveChildRemoveEventCallback* OnRecursiveChildRemoveEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        OnRecursiveChildRemoveEventCallback* temp;
        
        if(copyListeners)
            temp = new OnRecursiveChildRemoveEventCallback(*this);
        else
            temp = new OnRecursiveChildRemoveEventCallback();
        
        if(newContainer != nullptr)
            newContainer->AddEventCallback(temp);
        
        return temp;
    }

    const std::string OnRecursiveChildRemoveEventCallback::EVENT_NAME = "OnRecursiveChildRemoveEvent";
}

}