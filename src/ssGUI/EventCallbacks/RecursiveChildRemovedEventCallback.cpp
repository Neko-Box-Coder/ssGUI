#include "ssGUI/EventCallbacks/RecursiveChildRemovedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string RecursiveChildRemovedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    EventCallback* RecursiveChildRemovedEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        RecursiveChildRemovedEventCallback* temp;
        
        if(copyListeners)
            temp = new RecursiveChildRemovedEventCallback(*this);
        else
            temp = new RecursiveChildRemovedEventCallback();
        
        if(newContainer != nullptr)
            newContainer->AddEventCallback(temp);
        
        return temp;
    }

    const std::string RecursiveChildRemovedEventCallback::EVENT_NAME = "RecursiveChildRemovedEvent";
}