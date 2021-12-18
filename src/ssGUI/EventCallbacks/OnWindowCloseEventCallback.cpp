#include "ssGUI/EventCallbacks/OnWindowCloseEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string OnWindowCloseEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }
    
    EventCallback* OnWindowCloseEventCallback::Clone(ssGUI::GUIObject* container, bool copyListeners)
    {
        if(copyListeners)
            return new OnWindowCloseEventCallback(*this);
        else
            return new OnWindowCloseEventCallback();
    }

    const std::string OnWindowCloseEventCallback::EVENT_NAME = "OnWindowCloseEvent";
    
}