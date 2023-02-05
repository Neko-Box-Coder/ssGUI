#include "ssGUI/EventCallbacks/OnFontChangeEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string OnFontChangeEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    OnFontChangeEventCallback* OnFontChangeEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(OnFontChangeEventCallback, newContainer, copyListeners);
    }

    const std::string OnFontChangeEventCallback::EVENT_NAME = "OnFontChangeEvent";
}

}