#include "ssGUI/EventCallbacks/WindowDragStateChangedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string WindowDragStateChangedEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    WindowDragStateChangedEventCallback* WindowDragStateChangedEventCallback::Clone(ssGUI::GUIObject* newContainer, bool copyListeners)
    {
        WindowDragStateChangedEventCallback* temp;
        
        if(copyListeners)
            temp = new WindowDragStateChangedEventCallback(*this);
        else
            temp = new WindowDragStateChangedEventCallback();
        
        if(newContainer != nullptr)
            newContainer->AddEventCallback(temp);
        
        return temp;
    }

    const std::string WindowDragStateChangedEventCallback::EVENT_NAME = "WindowDragStateChangedEvent";
}