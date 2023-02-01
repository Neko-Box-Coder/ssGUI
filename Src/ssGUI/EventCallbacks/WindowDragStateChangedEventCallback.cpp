#include "ssGUI/EventCallbacks/WindowDragStateChangedEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string WindowDragStateChangedEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    WindowDragStateChangedEventCallback* WindowDragStateChangedEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(WindowDragStateChangedEventCallback, newContainer, copyListeners);
    }

    const std::string WindowDragStateChangedEventCallback::EVENT_NAME = "WindowDragStateChangedEvent";
}

}