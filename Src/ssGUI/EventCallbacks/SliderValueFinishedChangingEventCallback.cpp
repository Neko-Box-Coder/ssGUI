#include "ssGUI/EventCallbacks/SliderValueFinishedChangingEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string SliderValueFinishedChangingEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    SliderValueFinishedChangingEventCallback* SliderValueFinishedChangingEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        SliderValueFinishedChangingEventCallback* temp;
        
        if(copyListeners)
            temp = new SliderValueFinishedChangingEventCallback(*this);
        else
            temp = new SliderValueFinishedChangingEventCallback();
        
        if(newContainer != nullptr)
            newContainer->AddEventCallback(temp);
        
        return temp;
    }

    const std::string SliderValueFinishedChangingEventCallback::EVENT_NAME = "SliderValueFinishedChangingEvent";
}

}