#ifndef SSGUI_BUTTON_STATE_H
#define SSGUI_BUTTON_STATE_H

#include "ssGUI/HelperClasses/EnumToStringMacro.hpp"

#include <string>
namespace ssGUI
{

//namespace: ssGUI::Enums
namespace Enums
{
    /*enum: ButtonState
    
    NORMAL              - Normal button state 
    HOVER               - Hover button state
    ON_CLICK            - The moment the button is being clicked (Only trigger once)
    CLICKING            - When the button is being clicked (Will be triggered continuously)
    CLICKED             - The moment after the button is being clicked (Only trigger once)
    NOT_INTERACTABLE    - When the button is disabled
    COUNT               - Count
    */
    enum class ButtonState
    {
        NORMAL,
        HOVER,
        ON_CLICK,
        CLICKING,
        CLICKED,
        NOT_INTERACTABLE,
        COUNT
    };
    
    //function: ToString
    inline std::string ToString(ButtonState state)
    {
        static_assert((int)ButtonState::COUNT == 6, "Make sure ToString is updated");
        switch(state)
        {
            RETURN_ENUM_STRING(ButtonState::NORMAL);
            RETURN_ENUM_STRING(ButtonState::HOVER);
            RETURN_ENUM_STRING(ButtonState::ON_CLICK);
            RETURN_ENUM_STRING(ButtonState::CLICKING);
            RETURN_ENUM_STRING(ButtonState::CLICKED);
            RETURN_ENUM_STRING(ButtonState::NOT_INTERACTABLE);
            RETURN_ENUM_STRING(ButtonState::COUNT);
        }
        
        return "";
    }
}

}

#endif