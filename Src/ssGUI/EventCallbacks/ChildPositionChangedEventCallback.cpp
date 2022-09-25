#include "ssGUI/EventCallbacks/ChildPositionChangedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string ChildPositionChangedEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    ChildPositionChangedEventCallback* ChildPositionChangedEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        ChildPositionChangedEventCallback* temp;
        
        if(copyListeners)
            temp = new ChildPositionChangedEventCallback(*this);
        else
            temp = new ChildPositionChangedEventCallback();
        
        if(newContainer != nullptr)
            newContainer->AddEventCallback(temp);
        
        return temp;
    }

    const std::string ChildPositionChangedEventCallback::EVENT_NAME = "ChildPositionChangedEvent";
}