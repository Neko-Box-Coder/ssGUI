#include "ssGUI/EventCallbacks/ChildPositionChangedEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string ChildPositionChangedEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    ChildPositionChangedEventCallback* ChildPositionChangedEventCallback::Clone(bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(ChildPositionChangedEventCallback, copyListeners);
    }

    const std::string ChildPositionChangedEventCallback::EVENT_NAME = "ChildPositionChangedEvent";
}

}