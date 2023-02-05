#include "ssGUI/EventCallbacks/ChildAddedEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string ChildAddedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    ChildAddedEventCallback* ChildAddedEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(ChildAddedEventCallback, newContainer, copyListeners);
    }

    const std::string ChildAddedEventCallback::EVENT_NAME = "ChildAddedEvent";
}

}