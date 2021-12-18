#include "ssGUI/EventCallbacks/MinMaxSizeChangedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string MinMaxSizeChangedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    EventCallback* MinMaxSizeChangedEventCallback::Clone(ssGUI::GUIObject* container, bool copyListeners)
    {
        if(copyListeners)
            return new MinMaxSizeChangedEventCallback(*this);
        else
            return new MinMaxSizeChangedEventCallback();
    }

    const std::string MinMaxSizeChangedEventCallback::EVENT_NAME = "MinMaxSizeChangedEvent";
}