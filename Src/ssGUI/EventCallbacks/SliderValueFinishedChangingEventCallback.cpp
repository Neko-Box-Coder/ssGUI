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
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(SliderValueFinishedChangingEventCallback, newContainer, copyListeners);
    }

    const std::string SliderValueFinishedChangingEventCallback::EVENT_NAME = "SliderValueFinishedChangingEvent";
}

}