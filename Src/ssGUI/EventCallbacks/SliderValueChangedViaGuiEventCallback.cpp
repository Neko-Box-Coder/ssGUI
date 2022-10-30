#include "ssGUI/EventCallbacks/SliderValueChangedViaGuiEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string SliderValueChangedViaGuiEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    SliderValueChangedViaGuiEventCallback* SliderValueChangedViaGuiEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        SliderValueChangedViaGuiEventCallback* temp;
        
        if(copyListeners)
            temp = new SliderValueChangedViaGuiEventCallback(*this);
        else
            temp = new SliderValueChangedViaGuiEventCallback();
        
        if(newContainer != nullptr)
            newContainer->AddEventCallback(temp);
        
        return temp;
    }

    const std::string SliderValueChangedViaGuiEventCallback::EVENT_NAME = "SliderValueChangedViaGuiEvent";
}

}