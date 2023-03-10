#include "ssGUI/EventCallbacks/ChildAddedEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string ChildAddedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    ChildAddedEventCallback* ChildAddedEventCallback::Clone(bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(ChildAddedEventCallback, copyListeners);
    }

    const std::string ChildAddedEventCallback::EVENT_NAME = "ChildAddedEvent";
}

}