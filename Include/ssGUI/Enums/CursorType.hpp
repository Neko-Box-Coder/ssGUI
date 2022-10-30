#ifndef SSGUI_CURSOR_TYPE
#define SSGUI_CURSOR_TYPE

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
        CUSTOM
    };
}

}


#endif