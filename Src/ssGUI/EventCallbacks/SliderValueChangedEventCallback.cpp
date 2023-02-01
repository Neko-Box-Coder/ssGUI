#include "ssGUI/EventCallbacks/SliderValueChangedEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string SliderValueChangedEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    SliderValueChangedEventCallback* SliderValueChangedEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(SliderValueChangedEventCallback, newContainer, copyListeners);
    }

    const std::string SliderValueChangedEventCallback::EVENT_NAME = "SliderValueChangedEvent";
}

}