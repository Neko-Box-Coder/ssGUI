#ifndef SSGUI_MOUSE_BUTTON_H
#define SSGUI_MOUSE_BUTTON_H

#include "ssGUI/HelperClasses/EnumToStringMacro.hpp"

#include <string>
#include <cstdint>

namespace ssGUI 
{ 

//namespace: ssGUI::Enums
namespace Enums
{
    using GenericInput = uint16_t;

    /*enum: MouseButton
    
    LEFT    - Left mouse button
    MIDDLE  - Middle mouse button
    RIGHT   - Right mouse button
    */
    enum class MouseButton : uint16_t
    {
        LEFT = 1100,
        MIDDLE,
        RIGHT,
        EXTRA_1,
        EXTRA_2,
        COUNT = EXTRA_2 - LEFT + 1
    };
    
    //function: InputToString
    inline std::string InputToString(MouseButton input)
    {
        static_assert((int)MouseButton::COUNT == 5, "ToString");
        switch(input)
        {
            RETURN_ENUM_STRING(MouseButton::LEFT);
            RETURN_ENUM_STRING(MouseButton::MIDDLE);
            RETURN_ENUM_STRING(MouseButton::RIGHT);
            RETURN_ENUM_STRING(MouseButton::EXTRA_1);
            RETURN_ENUM_STRING(MouseButton::EXTRA_2);
            RETURN_ENUM_STRING(MouseButton::COUNT);
        }
        
        return "";
    }
    
    //function: InputIsMouseButton
    inline bool InputIsMouseButton(GenericInput input)
    {
        return  input >= (uint16_t)MouseButton::LEFT && 
                input < (uint16_t)MouseButton::LEFT + (uint16_t)MouseButton::COUNT;
    }
}

}

#endif