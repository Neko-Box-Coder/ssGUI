#include "ssGUI/EventCallbacks/RecursiveChildrenAddedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string RecursiveChildrenAddedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    EventCallback* RecursiveChildrenAddedEventCallback::Clone(ssGUI::GUIObject* container, bool copyListeners)
    {
        if(copyListeners)
            return new RecursiveChildrenAddedEventCallback(*this);
        else
            return new RecursiveChildrenAddedEventCallback();
    }

    const std::string RecursiveChildrenAddedEventCallback::EVENT_NAME = "RecursiveChildrenAddedEvent";
}