#include "ssGUI/EventCallbacks/SliderValueFinishedChangingEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string SliderValueFinishedChangingEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    SliderValueFinishedChangingEventCallback* SliderValueFinishedChangingEventCallback::Clone(bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(SliderValueFinishedChangingEventCallback, copyListeners);
    }

    const std::string SliderValueFinishedChangingEventCallback::EVENT_NAME = "SliderValueFinishedChangingEvent";
}

}