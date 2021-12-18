#include "ssGUI/EventCallbacks/OnObjectDestroyEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string OnObjectDestroyEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    EventCallback* OnObjectDestroyEventCallback::Clone(ssGUI::GUIObject* container, bool copyListeners)
    {
        if(copyListeners)
            return new OnObjectDestroyEventCallback(*this);
        else
            return new OnObjectDestroyEventCallback();
    }

    const std::string OnObjectDestroyEventCallback::EVENT_NAME = "OnObjectDestroyEvent";
}