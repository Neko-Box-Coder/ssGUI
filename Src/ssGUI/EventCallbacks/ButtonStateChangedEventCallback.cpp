#include "ssGUI/EventCallbacks/ButtonStateChangedEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string ButtonStateChangedEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    ButtonStateChangedEventCallback* ButtonStateChangedEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(ButtonStateChangedEventCallback, newContainer, copyListeners);
    }

    const std::string ButtonStateChangedEventCallback::EVENT_NAME = "ButtonStateChangedEvent";
}

}