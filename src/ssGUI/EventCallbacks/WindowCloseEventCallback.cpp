#include "ssGUI/EventCallbacks/WindowCloseEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string WindowCloseEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }
    
    ssGUI::EventCallbacks::EventCallback* WindowCloseEventCallback::Clone(ssGUI::GUIObject* container, std::vector<ssGUI::GUIObject*>& originalObjs, std::vector<ssGUI::GUIObject*>& clonedObjs)
    {
        return new WindowCloseEventCallback();
    }

    const std::string WindowCloseEventCallback::EVENT_NAME = "WindowCloseEvent";
    
}