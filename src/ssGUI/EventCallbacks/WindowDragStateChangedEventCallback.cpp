#include "ssGUI/EventCallbacks/WindowDragStateChangedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string WindowDragStateChangedEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    EventCallback* WindowDragStateChangedEventCallback::Clone(ssGUI::GUIObject* container, bool copyListeners)
    {
        if(copyListeners)
            return new WindowDragStateChangedEventCallback(*this);
        else
            return new WindowDragStateChangedEventCallback();
    }

    const std::string WindowDragStateChangedEventCallback::EVENT_NAME = "WindowDragStateChangedEvent";
}