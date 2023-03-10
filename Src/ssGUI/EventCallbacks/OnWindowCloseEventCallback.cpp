#include "ssGUI/EventCallbacks/OnWindowCloseEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string OnWindowCloseEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }
    
    OnWindowCloseEventCallback* OnWindowCloseEventCallback::Clone(bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(OnWindowCloseEventCallback, copyListeners);
    }

    const std::string OnWindowCloseEventCallback::EVENT_NAME = "OnWindowCloseEvent";   
}

}