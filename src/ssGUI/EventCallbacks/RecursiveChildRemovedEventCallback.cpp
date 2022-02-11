#include "ssGUI/EventCallbacks/RecursiveChildRemovedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string RecursiveChildRemovedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    EventCallback* RecursiveChildRemovedEventCallback::Clone(ssGUI::GUIObject* container, bool copyListeners)
    {
        if(copyListeners)
            return new RecursiveChildRemovedEventCallback(*this);
        else
            return new RecursiveChildRemovedEventCallback();
    }

    const std::string RecursiveChildRemovedEventCallback::EVENT_NAME = "RecursiveChildRemovedEvent";
}