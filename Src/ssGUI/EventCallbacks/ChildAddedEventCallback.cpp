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
        ChildAddedEventCallback* temp;
        
        if(copyListeners)
            temp = new ChildAddedEventCallback(*this);
        else
            temp = new ChildAddedEventCallback();
        
        if(newContainer != nullptr)
            newContainer->AddEventCallback(temp);
        
        return temp;
    }

    const std::string ChildAddedEventCallback::EVENT_NAME = "ChildAddedEvent";
}

}