#include "ssGUI/EventCallbacks/RecursiveChildrenRemovedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string RecursiveChildrenRemovedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    ssGUI::EventCallbacks::EventCallback* RecursiveChildrenRemovedEventCallback::Clone(ssGUI::GUIObject* container, std::vector<ssGUI::GUIObject*>& originalObjs, std::vector<ssGUI::GUIObject*>& clonedObjs)
    {
        return new RecursiveChildrenRemovedEventCallback();
    }

    const std::string RecursiveChildrenRemovedEventCallback::EVENT_NAME = "RecursiveChildrenRemovedEvent";
}