#include "ssGUI/EventCallbacks/SizeChangedEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string SizeChangedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    SizeChangedEventCallback* SizeChangedEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(SizeChangedEventCallback, newContainer, copyListeners);
    }

    const std::string SizeChangedEventCallback::EVENT_NAME = "SizeChangedEvent";
}

}