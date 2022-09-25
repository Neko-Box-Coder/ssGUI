#include "ssGUI/Factory.hpp"

#include "ssGUI/GUIObjectClasses/GUIObject.hpp"

namespace ssGUI
{
    void Factory::SetHeapAllocatedFlagForGUIObject(ssGUI::GUIObject* obj)
    {
        obj->SetHeapAllocated(true);
    }
}