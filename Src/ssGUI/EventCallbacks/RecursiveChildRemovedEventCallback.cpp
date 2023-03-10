#include "ssGUI/EventCallbacks/RecursiveChildRemovedEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string RecursiveChildRemovedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    RecursiveChildRemovedEventCallback* RecursiveChildRemovedEventCallback::Clone(bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(RecursiveChildRemovedEventCallback, copyListeners);
    }

    const std::string RecursiveChildRemovedEventCallback::EVENT_NAME = "RecursiveChildRemovedEvent";
}

}