#include "ssGUI/EventCallbacks/MinMaxSizeChangedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string MinMaxSizeChangedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    MinMaxSizeChangedEventCallback* MinMaxSizeChangedEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        MinMaxSizeChangedEventCallback* temp;
        
        if(copyListeners)
            temp = new MinMaxSizeChangedEventCallback(*this);
        else
            temp = new MinMaxSizeChangedEventCallback();
        
        if(newContainer != nullptr)
            newContainer->AddEventCallback(temp);
        
        return temp;
    }

    const std::string MinMaxSizeChangedEventCallback::EVENT_NAME = "MinMaxSizeChangedEvent";
}