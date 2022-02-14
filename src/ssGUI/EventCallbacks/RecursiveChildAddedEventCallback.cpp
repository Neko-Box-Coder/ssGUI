#include "ssGUI/EventCallbacks/RecursiveChildAddedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string RecursiveChildAddedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    EventCallback* RecursiveChildAddedEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        RecursiveChildAddedEventCallback* temp;
        
        if(copyListeners)
            temp = new RecursiveChildAddedEventCallback(*this);
        else
            temp = new RecursiveChildAddedEventCallback();
        
        if(newContainer != nullptr)
            newContainer->AddEventCallback(temp);
        
        return temp;
    }

    const std::string RecursiveChildAddedEventCallback::EVENT_NAME = "RecursiveChildAddedEvent";
}