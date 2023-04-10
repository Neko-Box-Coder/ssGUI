#ifndef SSGUI_ANCHOR_TYPE_H
#define SSGUI_ANCHOR_TYPE_H

#include "ssGUI/HelperClasses/EnumToStringMacro.hpp"
#include <string>
namespace ssGUI 
{ 
    
//namespace: ssGUI::Enums
namespace Enums
{
    /*enum: AnchorType

    TOP_LEFT        - Anchoring top left
    TOP_RIGHT       - Anchoring top right
    BOTTOM_LEFT     - Anchoring bottom left
    BOTTOM_RIGHT    - Anchoring bottom right
    COUNT           - Count
    */
    enum class AnchorType
    {
        TOP_LEFT,
        TOP_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_RIGHT,
        COUNT
    };
    
    //function: AnchorTypeToString
    inline std::string AnchorTypeToString(AnchorType anchor)
    {
        static_assert((int)AnchorType::COUNT == 4, "ToString");
        switch(anchor)
        {
            RETURN_ENUM_STRING(AnchorType::TOP_LEFT);
            RETURN_ENUM_STRING(AnchorType::TOP_RIGHT);
            RETURN_ENUM_STRING(AnchorType::BOTTOM_LEFT);
            RETURN_ENUM_STRING(AnchorType::BOTTOM_RIGHT);
            RETURN_ENUM_STRING(AnchorType::COUNT);
        }
        
        return "";
    }
}

}

#endif