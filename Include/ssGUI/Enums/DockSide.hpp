#ifndef SSGUI_DOCK_SIDE_HPP
#define SSGUI_DOCK_SIDE_HPP

#include "ssGUI/HelperClasses/EnumToStringMacro.hpp"
#include <string>
namespace ssGUI 
{ 
    
//namespace: ssGUI::Enums
namespace Enums
{
    /*enum: DockSide
    TOP             - Docking to top half
    RIGHT           - Docking to right half
    BOTTOM          - Docking to bottom half
    LEFT            - Docking to left half
    CENTER          - Docking to the whole area
    COUNT           - Count
    */
    enum class DockSide
    {
        TOP,
        RIGHT,
        BOTTOM,
        LEFT,
        CENTER,
        COUNT
    };
    
    //function: DockSideToString
    inline std::string DockSideToString(DockSide side)
    {
        static_assert((int)DockSide::COUNT == 5, "ToString");
        switch(side)
        {
            RETURN_ENUM_STRING(DockSide::TOP);
            RETURN_ENUM_STRING(DockSide::RIGHT);
            RETURN_ENUM_STRING(DockSide::BOTTOM);
            RETURN_ENUM_STRING(DockSide::LEFT);
            RETURN_ENUM_STRING(DockSide::CENTER);
            RETURN_ENUM_STRING(DockSide::COUNT);
        }
        
        return "";
    }
}

}

#endif