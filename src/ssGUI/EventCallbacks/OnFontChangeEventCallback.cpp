#include "ssGUI/EventCallbacks/OnFontChangeEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string OnFontChangeEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    OnFontChangeEventCallback* OnFontChangeEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        OnFontChangeEventCallback* temp;
        
        if(copyListeners)
            temp = new OnFontChangeEventCallback(*this);
        else
            temp = new OnFontChangeEventCallback();
        
        if(newContainer != nullptr)
            newContainer->AddEventCallback(temp);
        
        return temp;
    }

    const std::string OnFontChangeEventCallback::EVENT_NAME = "OnFontChangeEvent";
}