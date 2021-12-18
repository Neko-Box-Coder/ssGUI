#include "ssGUI/EventCallbacks/RecursiveChildrenRemovedEventCallback.hpp"

namespace ssGUI::EventCallbacks
{
    std::string RecursiveChildrenRemovedEventCallback::GetEventCallbackName() const
    {   
        return EVENT_NAME;
    }

    EventCallback* RecursiveChildrenRemovedEventCallback::Clone(ssGUI::GUIObject* container, bool copyListeners)
    {
        if(copyListeners)
            return new RecursiveChildrenRemovedEventCallback(*this);
        else
            return new RecursiveChildrenRemovedEventCallback();
    }

    const std::string RecursiveChildrenRemovedEventCallback::EVENT_NAME = "RecursiveChildrenRemovedEvent";
}