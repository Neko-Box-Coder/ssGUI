#ifndef SSGUI_SYSTEM_KEY
#define SSGUI_SYSTEM_KEY

//namespace: ssGUI::Enums
namespace ssGUI::Enums
{
    /*enum: SystemKey

    TAB             - Tab key
    CAPS_LOCK       - Caps lock key
    LEFT_SHIFT      - Left shift key
    LEFT_CTRL       - Left ctrl key
    LEFT_SYSTEM     - Left system key (Windows key)
    LEFT_ALT        - Left alt key
    SPACE           - Space key
    RIGHT_ALT       - Right alt key  
    RIGHT_SYSTEM    - Right system key (Windows key)
    RIGHT_CTRL      - Right ctrl key
    RIGHT_SHIFT     - Right shift key
    ENTER           - Enter key
    BACKSPACE       - Backspace key
    INSERT          - Insert key
    HOME            - Home key
    PAGE_UP         - Page up key
    DELETE          - Delete key
    END             - End key
    PAGE_DOWN       - Page down key
    LEFT            - Left arrow key
    UP              - Up arrow key
    DOWN            - Down arrow key
    RIGHT           - Right arrow key
    NUMPAD_ENTER    - Numpad enter key (SFML backend doesn't support it)
    */
    enum class SystemKey
    {
        TAB, CAPS_LOCK, LEFT_SHIFT, LEFT_CTRL, LEFT_SYSTEM, LEFT_ALT, SPACE, RIGHT_ALT,
        RIGHT_SYSTEM, RIGHT_CTRL, RIGHT_SHIFT, ENTER, BACKSPACE, INSERT, HOME, PAGE_UP,
        DELETE, END, PAGE_DOWN, LEFT, UP, DOWN, RIGHT, NUMPAD_ENTER
    };
}


#endif