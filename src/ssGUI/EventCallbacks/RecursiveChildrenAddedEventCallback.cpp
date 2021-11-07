#include "ssGUI/EventCallbacks/RecursiveChildrenAddedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string RecursiveChildrenAddedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    ssGUI::EventCallbacks::EventCallback* RecursiveChildrenAddedEventCallback::Clone(ssGUI::GUIObject* container, std::vector<ssGUI::GUIObject*>& originalObjs, std::vector<ssGUI::GUIObject*>& clonedObjs)
    {
        return new RecursiveChildrenAddedEventCallback();
    }

    const std::string RecursiveChildrenAddedEventCallback::EVENT_NAME = "RecursiveChildrenAddedEvent";
}