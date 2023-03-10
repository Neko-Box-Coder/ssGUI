#include "ssGUI/EventCallbacks/ChildRemovedEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string ChildRemovedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    ChildRemovedEventCallback* ChildRemovedEventCallback::Clone(bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(ChildRemovedEventCallback, copyListeners);
    }

    const std::string ChildRemovedEventCallback::EVENT_NAME = "ChildRemovedEvent";
}

}