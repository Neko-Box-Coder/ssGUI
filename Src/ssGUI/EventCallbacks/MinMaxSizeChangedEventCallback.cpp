#include "ssGUI/EventCallbacks/MinMaxSizeChangedEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string MinMaxSizeChangedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    MinMaxSizeChangedEventCallback* MinMaxSizeChangedEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(MinMaxSizeChangedEventCallback, newContainer, copyListeners);
    }

    const std::string MinMaxSizeChangedEventCallback::EVENT_NAME = "MinMaxSizeChangedEvent";
}

}