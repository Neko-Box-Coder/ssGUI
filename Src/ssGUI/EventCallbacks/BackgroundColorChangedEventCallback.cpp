#include "ssGUI/EventCallbacks/BackgroundColorChangedEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string BackgroundColorChangedEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    BackgroundColorChangedEventCallback* BackgroundColorChangedEventCallback::Clone(bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(BackgroundColorChangedEventCallback, copyListeners);
    }

    const std::string BackgroundColorChangedEventCallback::EVENT_NAME = "BackgroundColorChangedEvent";
}

}