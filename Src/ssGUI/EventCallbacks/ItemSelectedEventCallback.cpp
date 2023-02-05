#include "ssGUI/EventCallbacks/ItemSelectedEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string ItemSelectedEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    ItemSelectedEventCallback* ItemSelectedEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(ItemSelectedEventCallback, newContainer, copyListeners);
    }

    const std::string ItemSelectedEventCallback::EVENT_NAME = "ItemSelectedEvent";
}

}