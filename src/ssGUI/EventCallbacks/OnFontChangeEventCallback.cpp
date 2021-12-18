#include "ssGUI/EventCallbacks/OnFontChangeEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string OnFontChangeEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    EventCallback* OnFontChangeEventCallback::Clone(ssGUI::GUIObject* container, bool copyListeners)
    {
        if(copyListeners)
            return new OnFontChangeEventCallback(*this);
        else
            return new OnFontChangeEventCallback();
    }

    const std::string OnFontChangeEventCallback::EVENT_NAME = "OnFontChangeEvent";
}