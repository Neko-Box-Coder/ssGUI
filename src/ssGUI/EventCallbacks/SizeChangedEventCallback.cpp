#include "ssGUI/EventCallbacks/SizeChangedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string SizeChangedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    SizeChangedEventCallback* SizeChangedEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        SizeChangedEventCallback* temp;
        
        if(copyListeners)
            temp = new SizeChangedEventCallback(*this);
        else
            temp = new SizeChangedEventCallback();
        
        if(newContainer != nullptr)
            newContainer->AddEventCallback(temp);
        
        return temp;
    }

    const std::string SizeChangedEventCallback::EVENT_NAME = "SizeChangedEvent";
}