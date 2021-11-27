#include "ssGUI/EventCallbacks/WindowDragStateChangedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string WindowDragStateChangedEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    ssGUI::EventCallbacks::EventCallback* WindowDragStateChangedEventCallback::Clone(ssGUI::GUIObject* container, std::vector<ssGUI::GUIObject*>& originalObjs, std::vector<ssGUI::GUIObject*>& clonedObjs)
    {
        return new ssGUI::EventCallbacks::WindowDragStateChangedEventCallback();
    }

    const std::string WindowDragStateChangedEventCallback::EVENT_NAME = "WindowDragStateChangedEvent";
}