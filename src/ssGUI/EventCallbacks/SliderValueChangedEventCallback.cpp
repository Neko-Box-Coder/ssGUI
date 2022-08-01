#include "ssGUI/EventCallbacks/SliderValueChangedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string SliderValueChangedEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    SliderValueChangedEventCallback* SliderValueChangedEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        SliderValueChangedEventCallback* temp;
        
        if(copyListeners)
            temp = new SliderValueChangedEventCallback(*this);
        else
            temp = new SliderValueChangedEventCallback();
        
        if(newContainer != nullptr)
            newContainer->AddEventCallback(temp);
        
        return temp;
    }

    const std::string SliderValueChangedEventCallback::EVENT_NAME = "SliderValueChangedEvent";
}