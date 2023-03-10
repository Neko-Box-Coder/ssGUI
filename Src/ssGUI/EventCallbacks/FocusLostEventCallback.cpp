#include "ssGUI/EventCallbacks/FocusLostEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string FocusLostEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    FocusLostEventCallback* FocusLostEventCallback::Clone(bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(FocusLostEventCallback, copyListeners);
    }

    const std::string FocusLostEventCallback::EVENT_NAME = "FocusLostEvent";
}

}