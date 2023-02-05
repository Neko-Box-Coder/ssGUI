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
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(SliderValueChangedViaGuiEventCallback, newContainer, copyListeners);
    }

    const std::string SliderValueChangedViaGuiEventCallback::EVENT_NAME = "SliderValueChangedViaGuiEvent";
}

}