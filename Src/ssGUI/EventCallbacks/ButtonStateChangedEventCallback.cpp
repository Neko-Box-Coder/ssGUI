#include "ssGUI/EventCallbacks/ButtonStateChangedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string ButtonStateChangedEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    ButtonStateChangedEventCallback* ButtonStateChangedEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        ButtonStateChangedEventCallback* temp;
        
        if(copyListeners)
            temp = new ButtonStateChangedEventCallback(*this);
        else
            temp = new ButtonStateChangedEventCallback();
        
        if(newContainer != nullptr)
            newContainer->AddEventCallback(temp);
        
        return temp;
    }

    const std::string ButtonStateChangedEventCallback::EVENT_NAME = "ButtonStateChangedEvent";
}