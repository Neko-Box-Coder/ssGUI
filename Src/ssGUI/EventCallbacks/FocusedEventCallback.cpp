#include "ssGUI/EventCallbacks/FocusedEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string FocusedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    FocusedEventCallback* FocusedEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(FocusedEventCallback, newContainer, copyListeners);
    }

    const std::string FocusedEventCallback::EVENT_NAME = "FocusedEvent";
}

}