#include "ssGUI/EventCallbacks/ItemSelectedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string ItemSelectedEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    ItemSelectedEventCallback* ItemSelectedEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        ItemSelectedEventCallback* temp;
        
        if(copyListeners)
            temp = new ItemSelectedEventCallback(*this);
        else
            temp = new ItemSelectedEventCallback();
        
        if(newContainer != nullptr)
            newContainer->AddEventCallback(temp);
        
        return temp;
    }

    const std::string ItemSelectedEventCallback::EVENT_NAME = "ItemSelectedEvent";
}