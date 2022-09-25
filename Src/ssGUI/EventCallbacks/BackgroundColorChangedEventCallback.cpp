#include "ssGUI/EventCallbacks/BackgroundColorChangedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string BackgroundColorChangedEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    BackgroundColorChangedEventCallback* BackgroundColorChangedEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        BackgroundColorChangedEventCallback* temp;
        
        if(copyListeners)
            temp = new BackgroundColorChangedEventCallback(*this);
        else
            temp = new BackgroundColorChangedEventCallback();
        
        if(newContainer != nullptr)
            newContainer->AddEventCallback(temp);
        
        return temp;
    }

    const std::string BackgroundColorChangedEventCallback::EVENT_NAME = "BackgroundColorChangedEvent";
}