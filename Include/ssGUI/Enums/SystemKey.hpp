#ifndef SSGUI_SYSTEM_KEY_H
#define SSGUI_SYSTEM_KEY_H

#include "ssGUI/HelperClasses/EnumToStringMacro.hpp"

#include <string>
#include <cstdint>

//We need to undefine the DELETE macro from Win32 because this collides with our enum name
#ifdef SSGUI_MAIN_BACKEND_WIN32_OPENGL
#undef DELETE
#endif

namespace ssGUI
{

//namespace: ssGUI::Enums
namespace Enums
{
    using GenericButtonAndKeyInput = uint16_t;
    
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
                      TODO: Maybe remove NUMPAD_ENTER because it seems to always share the same key as ENTER
    */
    enum class SystemKey : uint16_t
    {
        TAB = 500, 
        CAPS_LOCK, LEFT_SHIFT, LEFT_CTRL, LEFT_SYSTEM, LEFT_ALT, SPACE, RIGHT_ALT,
        RIGHT_SYSTEM, RIGHT_CTRL, RIGHT_SHIFT, ENTER, BACKSPACE, INSERT, HOME, PAGE_UP,
        DELETE, END, PAGE_DOWN, LEFT, UP, DOWN, RIGHT, NUMPAD_ENTER, 
        COUNT = NUMPAD_ENTER - TAB + 1
    };
    
    //function: InputToString
    inline std::string InputToString(SystemKey input)
    {
        static_assert((int)SystemKey::COUNT == 24, "ToString");
        switch(input)
        {
            RETURN_ENUM_STRING(SystemKey::TAB);
            RETURN_ENUM_STRING(SystemKey::CAPS_LOCK);
            RETURN_ENUM_STRING(SystemKey::LEFT_SHIFT);
            RETURN_ENUM_STRING(SystemKey::LEFT_CTRL);
            RETURN_ENUM_STRING(SystemKey::LEFT_SYSTEM);
            RETURN_ENUM_STRING(SystemKey::LEFT_ALT);
            RETURN_ENUM_STRING(SystemKey::SPACE);
            RETURN_ENUM_STRING(SystemKey::RIGHT_ALT);
            RETURN_ENUM_STRING(SystemKey::RIGHT_SYSTEM);
            RETURN_ENUM_STRING(SystemKey::RIGHT_CTRL);
            RETURN_ENUM_STRING(SystemKey::RIGHT_SHIFT);
            RETURN_ENUM_STRING(SystemKey::ENTER);
            RETURN_ENUM_STRING(SystemKey::BACKSPACE);
            RETURN_ENUM_STRING(SystemKey::INSERT);
            RETURN_ENUM_STRING(SystemKey::HOME);
            RETURN_ENUM_STRING(SystemKey::PAGE_UP);
            RETURN_ENUM_STRING(SystemKey::DELETE);
            RETURN_ENUM_STRING(SystemKey::END);
            RETURN_ENUM_STRING(SystemKey::PAGE_DOWN);
            RETURN_ENUM_STRING(SystemKey::LEFT);
            RETURN_ENUM_STRING(SystemKey::UP);
            RETURN_ENUM_STRING(SystemKey::DOWN);
            RETURN_ENUM_STRING(SystemKey::RIGHT);
            RETURN_ENUM_STRING(SystemKey::NUMPAD_ENTER);
            RETURN_ENUM_STRING(SystemKey::COUNT);
        }
        
        return "";
    }
    
    //function: InputIsSystemKey
    inline bool InputIsSystemKey(ssGUI::Enums::GenericButtonAndKeyInput input)
    {
        return input >= (uint16_t)SystemKey::TAB && input < (uint16_t)SystemKey::TAB + (uint16_t)SystemKey::COUNT;
    }
}

}

#endif