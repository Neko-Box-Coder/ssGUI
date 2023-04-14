#ifndef SSGUI_ALIGNMENT_VERTICAL_H
#define SSGUI_ALIGNMENT_VERTICAL_H

#include "ssGUI/HelperClasses/EnumToStringMacro.hpp"
#include <string>
namespace ssGUI
{

//namespace: ssGUI::Enums
namespace Enums
{
    /*enum: AlignmentVertical
    TOP     - Top allignment
    CENTER  - Center allignment
    BOTTOM  - Bottom allignment
    COUNT   - Count
    */
    enum class AlignmentVertical
    {
        TOP,
        CENTER,
        BOTTOM,
        COUNT
    };

    //function: AlignmentVerticalToString
    inline std::string AlignmentVerticalToString(AlignmentVertical align)
    {
        static_assert((int)AlignmentVertical::COUNT == 3, "ToString");
        switch(align)
        {
            RETURN_ENUM_STRING(AlignmentVertical::TOP);
            RETURN_ENUM_STRING(AlignmentVertical::CENTER);
            RETURN_ENUM_STRING(AlignmentVertical::BOTTOM);
            RETURN_ENUM_STRING(AlignmentVertical::COUNT);
        }
        
        return "";
    }
}

}

#endif