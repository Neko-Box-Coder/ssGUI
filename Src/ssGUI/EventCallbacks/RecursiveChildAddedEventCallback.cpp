#include "ssGUI/EventCallbacks/RecursiveChildAddedEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string RecursiveChildAddedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    RecursiveChildAddedEventCallback* RecursiveChildAddedEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(RecursiveChildAddedEventCallback, newContainer, copyListeners);
    }

    const std::string RecursiveChildAddedEventCallback::EVENT_NAME = "RecursiveChildAddedEvent";
}

}