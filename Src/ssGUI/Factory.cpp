#include "ssGUI/Factory.hpp"

#include "ssGUI/GUIObjectClasses/GUIObject.hpp"
#include "ssGUI/GUIObjectClasses/Text.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardWindow.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/Dropdown.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/ImageCanvas.hpp"

namespace ssGUI
{
    void Factory::SetHeapAllocatedFlagForGUIObject(ssGUI::GUIObject* obj)
    {
        obj->SetHeapAllocated(true);
    }
    
    void CleanUpDefaultResources()
    {
        ssGUI::StandardWindow::CleanUpDefaultIcon();
        ssGUI::ImageCanvas::CleanUpDefaultRotationCursor();
        ssGUI::Dropdown::CleanUpDefaultDropdownArrowImage();
        ssGUI::Text::CleanUpAllDefaultFonts();
    }
}