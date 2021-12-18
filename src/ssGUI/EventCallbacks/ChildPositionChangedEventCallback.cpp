#include "ssGUI/EventCallbacks/ChildPositionChangedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string ChildPositionChangedEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    EventCallback* ChildPositionChangedEventCallback::Clone(ssGUI::GUIObject* container, bool copyListeners)
    {
        if(copyListeners)
            return new ChildPositionChangedEventCallback(*this);
        else
            return new ChildPositionChangedEventCallback();
    }

    const std::string ChildPositionChangedEventCallback::EVENT_NAME = "ChildPositionChangedEvent";
}