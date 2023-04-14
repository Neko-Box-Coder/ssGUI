#ifndef SSGUI_CURSOR_TYPE_H
#define SSGUI_CURSOR_TYPE_H

#include "ssGUI/HelperClasses/EnumToStringMacro.hpp"
#include <string>

namespace ssGUI 
{ 
    
//namespace: ssGUI::Enums
namespace Enums
{
    /*enum: CursorType
    NONE                - Invisible cursor
    NORMAL              - Normal cursor
    TEXT                - Text cursor
    HAND                - Hand cursor
    RESIZE_LEFT         - Resize left cursor
    RESIZE_RIGHT        - Resize right cursor
    RESIZE_UP           - Resize up cursor
    RESIZE_DOWN         - Resize down cursor
    RESIZE_TOP_LEFT     - Resize top left cursor
    RESIZE_TOP_RIGHT    - Resize top right cursor
    RESIZE_BOTTOM_RIGHT - Resize bottom right cursor
    RESIZE_BOTTOM_LEFT  - Resize bottom left cursor
    MOVE                - Move cursor
    HELP                - Help cursor
    NOT_ALLOWED         - Not allowed cursor
    CUSTOM              - Custom cursor (Will need to set the custom cursor from <BackendSystemInputInterface> first)
    COUNT               - Count
    */
    enum class CursorType
    {
        NONE,
        NORMAL,
        TEXT,
        HAND,
        RESIZE_LEFT,
        RESIZE_RIGHT,
        RESIZE_UP,
        RESIZE_DOWN,
        RESIZE_TOP_LEFT,
        RESIZE_TOP_RIGHT,
        RESIZE_BOTTOM_RIGHT,
        RESIZE_BOTTOM_LEFT,
        MOVE,
        HELP,
        NOT_ALLOWED,
        CUSTOM,
        COUNT
    };
    
    //function: CursorTypeToString
    inline std::string CursorTypeToString(CursorType input)
    {
        static_assert((int)CursorType::COUNT == 16, "ToString");
        switch(input)
        {
            RETURN_ENUM_STRING(CursorType::NONE);
            RETURN_ENUM_STRING(CursorType::NORMAL);
            RETURN_ENUM_STRING(CursorType::TEXT);
            RETURN_ENUM_STRING(CursorType::HAND);
            RETURN_ENUM_STRING(CursorType::RESIZE_LEFT);
            RETURN_ENUM_STRING(CursorType::RESIZE_RIGHT);
            RETURN_ENUM_STRING(CursorType::RESIZE_UP);
            RETURN_ENUM_STRING(CursorType::RESIZE_DOWN);
            RETURN_ENUM_STRING(CursorType::RESIZE_TOP_LEFT);
            RETURN_ENUM_STRING(CursorType::RESIZE_TOP_RIGHT);
            RETURN_ENUM_STRING(CursorType::RESIZE_BOTTOM_RIGHT);
            RETURN_ENUM_STRING(CursorType::RESIZE_BOTTOM_LEFT);
            RETURN_ENUM_STRING(CursorType::MOVE);
            RETURN_ENUM_STRING(CursorType::HELP);
            RETURN_ENUM_STRING(CursorType::NOT_ALLOWED);
            RETURN_ENUM_STRING(CursorType::CUSTOM);
            RETURN_ENUM_STRING(CursorType::COUNT);
        }
        
        return "";
    }
}

}


#endif