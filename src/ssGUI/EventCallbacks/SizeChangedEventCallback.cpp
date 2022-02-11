#include "ssGUI/EventCallbacks/SizeChangedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string SizeChangedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    EventCallback* SizeChangedEventCallback::Clone(ssGUI::GUIObject* container, bool copyListeners)
    {
        if(copyListeners)
            return new SizeChangedEventCallback(*this);
        else
            return new SizeChangedEventCallback();
    }

    const std::string SizeChangedEventCallback::EVENT_NAME = "SizeChangedEvent";
}