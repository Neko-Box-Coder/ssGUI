#include "ssGUI/EventCallbacks/OnWindowCloseEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string OnWindowCloseEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }
    
    OnWindowCloseEventCallback* OnWindowCloseEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(OnWindowCloseEventCallback, newContainer, copyListeners);
    }

    const std::string OnWindowCloseEventCallback::EVENT_NAME = "OnWindowCloseEvent";   
}

}