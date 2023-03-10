#include "ssGUI/EventCallbacks/OnObjectDestroyEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string OnObjectDestroyEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    OnObjectDestroyEventCallback* OnObjectDestroyEventCallback::Clone(bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(OnObjectDestroyEventCallback, copyListeners);
    }

    const std::string OnObjectDestroyEventCallback::EVENT_NAME = "OnObjectDestroyEvent";
}

}