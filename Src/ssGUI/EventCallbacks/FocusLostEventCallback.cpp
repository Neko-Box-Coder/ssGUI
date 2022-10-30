#include "ssGUI/EventCallbacks/FocusLostEventCallback.hpp"

namespace ssGUI
{

namespace EventCallbacks
{
    std::string FocusLostEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    FocusLostEventCallback* FocusLostEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        FocusLostEventCallback* temp;
        
        if(copyListeners)
            temp = new FocusLostEventCallback(*this);
        else
            temp = new FocusLostEventCallback();
        
        if(newContainer != nullptr)
            newContainer->AddEventCallback(temp);
        
        return temp;
    }

    const std::string FocusLostEventCallback::EVENT_NAME = "FocusLostEvent";
}

}