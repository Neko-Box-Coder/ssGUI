#include "ssGUI/EventCallbacks/SizeChangedEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string SizeChangedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    SizeChangedEventCallback* SizeChangedEventCallback::Clone(bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(SizeChangedEventCallback, copyListeners);
    }

    const std::string SizeChangedEventCallback::EVENT_NAME = "SizeChangedEvent";
}

}