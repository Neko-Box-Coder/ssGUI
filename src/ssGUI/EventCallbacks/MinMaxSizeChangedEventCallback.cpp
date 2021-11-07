#include "ssGUI/EventCallbacks/MinMaxSizeChangedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string MinMaxSizeChangedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    ssGUI::EventCallbacks::EventCallback* MinMaxSizeChangedEventCallback::Clone(ssGUI::GUIObject* container, std::vector<ssGUI::GUIObject*>& originalObjs, std::vector<ssGUI::GUIObject*>& clonedObjs)
    {
        return new MinMaxSizeChangedEventCallback();
    }

    const std::string MinMaxSizeChangedEventCallback::EVENT_NAME = "MinMaxSizeChangedEvent";
}