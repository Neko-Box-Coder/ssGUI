#include "ssGUI/EventCallbacks/OnRecursiveChildAddEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string OnRecursiveChildAddEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    OnRecursiveChildAddEventCallback* OnRecursiveChildAddEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(OnRecursiveChildAddEventCallback, newContainer, copyListeners);
    }

    const std::string OnRecursiveChildAddEventCallback::EVENT_NAME = "OnRecursiveChildAddEvent";
}

}