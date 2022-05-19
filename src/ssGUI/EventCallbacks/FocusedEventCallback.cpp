#include "ssGUI/EventCallbacks/FocusedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string FocusedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    FocusedEventCallback* FocusedEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        FocusedEventCallback* temp;
        
        if(copyListeners)
            temp = new FocusedEventCallback(*this);
        else
            temp = new FocusedEventCallback();
        
        if(newContainer != nullptr)
            newContainer->AddEventCallback(temp);
        
        return temp;
    }

    const std::string FocusedEventCallback::EVENT_NAME = "FocusedEvent";
}