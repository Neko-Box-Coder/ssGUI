#include "ssGUI/EventCallbacks/ButtonStateChangedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string ButtonStateChangedEventCallback::GetEventCallbackName() const
    {
        return EVENT_NAME;
    }

    ssGUI::EventCallbacks::EventCallback* ButtonStateChangedEventCallback::Clone(ssGUI::GUIObject* container, std::vector<ssGUI::GUIObject*>& originalObjs, std::vector<ssGUI::GUIObject*>& clonedObjs)
    {
        return new ssGUI::EventCallbacks::ButtonStateChangedEventCallback();
    }

    const std::string ButtonStateChangedEventCallback::EVENT_NAME = "ButtonStateChangedEvent";
}