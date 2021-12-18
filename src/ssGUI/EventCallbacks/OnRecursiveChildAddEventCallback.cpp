#include "ssGUI/EventCallbacks/OnRecursiveChildAddEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string OnRecursiveChildAddEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    EventCallback* OnRecursiveChildAddEventCallback::Clone(ssGUI::GUIObject* container, bool copyListeners)
    {
        if(copyListeners)
            return new OnRecursiveChildAddEventCallback(*this);
        else
            return new OnRecursiveChildAddEventCallback();
    }

    const std::string OnRecursiveChildAddEventCallback::EVENT_NAME = "OnRecursiveChildAddEvent";
}