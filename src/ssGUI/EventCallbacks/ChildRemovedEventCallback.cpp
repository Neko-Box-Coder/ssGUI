#include "ssGUI/EventCallbacks/ChildRemovedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string ChildRemovedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    ChildRemovedEventCallback* ChildRemovedEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        ChildRemovedEventCallback* temp;
        
        if(copyListeners)
            temp = new ChildRemovedEventCallback(*this);
        else
            temp = new ChildRemovedEventCallback();
        
        if(newContainer != nullptr)
            newContainer->AddEventCallback(temp);
        
        return temp;
    }

    const std::string ChildRemovedEventCallback::EVENT_NAME = "ChildRemovedEvent";
}