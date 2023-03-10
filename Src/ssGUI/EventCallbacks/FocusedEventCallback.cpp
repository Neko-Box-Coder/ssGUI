#include "ssGUI/EventCallbacks/FocusedEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string FocusedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    FocusedEventCallback* FocusedEventCallback::Clone(bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(FocusedEventCallback, copyListeners);
    }

    const std::string FocusedEventCallback::EVENT_NAME = "FocusedEvent";
}

}