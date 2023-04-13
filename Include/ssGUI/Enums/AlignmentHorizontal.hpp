#ifndef SSGUI_ALIGNMENT_HORIZONTAL_H
#define SSGUI_ALIGNMENT_HORIZONTAL_H

#include "ssGUI/HelperClasses/EnumToStringMacro.hpp"
#include <string>
namespace ssGUI
{

//namespace: ssGUI::Enums
namespace Enums
{
    /*enum: AlignmentHorizontal
    LEFT    - Left allignment
    CENTER  - Center allignment
    RIGHT   - Right allignment
    COUNT   - Count
    */
    enum class AlignmentHorizontal
    {
        LEFT,
        CENTER,
        RIGHT,
        COUNT
    };
    
    //function: AlignmentHorizontalToString
    inline std::string AlignmentHorizontalToString(AlignmentHorizontal align)
    {
        static_assert((int)AlignmentHorizontal::COUNT == 3, "ToString");
        switch(align)
        {
            RETURN_ENUM_STRING(AlignmentHorizontal::LEFT);
            RETURN_ENUM_STRING(AlignmentHorizontal::CENTER);
            RETURN_ENUM_STRING(AlignmentHorizontal::RIGHT);
            RETURN_ENUM_STRING(AlignmentHorizontal::COUNT);
        }
        
        return "";
    }
}

}

#endif