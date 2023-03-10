#include "ssGUI/EventCallbacks/ItemSelectedEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string ItemSelectedEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    ItemSelectedEventCallback* ItemSelectedEventCallback::Clone(bool copyListeners)
    {
        return INTERNAL_SSGUI_EVENT_CALLBACK_CLONE(ItemSelectedEventCallback, copyListeners);
    }

    const std::string ItemSelectedEventCallback::EVENT_NAME = "ItemSelectedEvent";
}

}