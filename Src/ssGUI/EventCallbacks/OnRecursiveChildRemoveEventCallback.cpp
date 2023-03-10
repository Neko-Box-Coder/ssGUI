#include "ssGUI/EventCallbacks/OnRecursiveChildRemoveEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string OnRecursiveChildRemoveEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    OnRecursiveChildRemoveEventCallback* OnRecursiveChildRemoveEventCallback::Clone(bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(OnRecursiveChildRemoveEventCallback, copyListeners);
    }

    const std::string OnRecursiveChildRemoveEventCallback::EVENT_NAME = "OnRecursiveChildRemoveEvent";
}

}