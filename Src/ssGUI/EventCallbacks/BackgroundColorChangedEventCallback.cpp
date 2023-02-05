#include "ssGUI/EventCallbacks/BackgroundColorChangedEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string BackgroundColorChangedEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    BackgroundColorChangedEventCallback* BackgroundColorChangedEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(BackgroundColorChangedEventCallback, newContainer, copyListeners);
    }

    const std::string BackgroundColorChangedEventCallback::EVENT_NAME = "BackgroundColorChangedEvent";
}

}