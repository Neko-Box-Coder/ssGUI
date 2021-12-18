#include "ssGUI/EventCallbacks/ButtonStateChangedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string ButtonStateChangedEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    EventCallback* ButtonStateChangedEventCallback::Clone(ssGUI::GUIObject* container, bool copyListeners)
    {
        if(copyListeners)
            return new ButtonStateChangedEventCallback(*this);
        else
            return new ButtonStateChangedEventCallback();
    }

    const std::string ButtonStateChangedEventCallback::EVENT_NAME = "ButtonStateChangedEvent";
}