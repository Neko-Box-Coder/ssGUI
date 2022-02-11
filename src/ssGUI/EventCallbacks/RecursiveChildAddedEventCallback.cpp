#include "ssGUI/EventCallbacks/RecursiveChildAddedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string RecursiveChildAddedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    EventCallback* RecursiveChildAddedEventCallback::Clone(ssGUI::GUIObject* container, bool copyListeners)
    {
        if(copyListeners)
            return new RecursiveChildAddedEventCallback(*this);
        else
            return new RecursiveChildAddedEventCallback();
    }

    const std::string RecursiveChildAddedEventCallback::EVENT_NAME = "RecursiveChildAddedEvent";
}