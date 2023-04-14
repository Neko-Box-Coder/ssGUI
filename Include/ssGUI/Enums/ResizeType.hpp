#ifndef SSGUI_RESIZE_TYPE_H
#define SSGUI_RESIZE_TYPE_H

#include "ssGUI/HelperClasses/EnumToStringMacro.hpp"
#include <string>
namespace ssGUI
{

//namespace: ssGUI::Enums
namespace Enums
{
    /*enum: ResizeType
    
    NONE    - Window doesn't resize
    ALL     - Window can be resized from all side
    TOP     - Window can only be reside from the top 
    LEFT    - Window can only be reside from the left
    BOTTOM  - Window can only be reside from the bottom
    RIGHT   - Window can only be reside from the right
    COUNT   - Count
    */
    enum class ResizeType
    {
        NONE,
        ALL,
        TOP,
        LEFT,
        BOTTOM,
        RIGHT,
        COUNT
    };
    
    //function: ResizeTypeToString
    inline std::string ResizeTypeToString(ResizeType resizeType)
    {
        static_assert((int)ResizeType::COUNT == 6, "ToString");
        switch(resizeType)
        {
            RETURN_ENUM_STRING(ResizeType::NONE);
            RETURN_ENUM_STRING(ResizeType::ALL);
            RETURN_ENUM_STRING(ResizeType::TOP);
            RETURN_ENUM_STRING(ResizeType::LEFT);
            RETURN_ENUM_STRING(ResizeType::BOTTOM);
            RETURN_ENUM_STRING(ResizeType::RIGHT);
            RETURN_ENUM_STRING(ResizeType::COUNT);
        }
        
        return "";
    }
}

}

#endif