#ifndef SSGUI_BUTTON_STATE_H
#define SSGUI_BUTTON_STATE_H

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
    
    inline std::string ToString(ButtonState state)
    {
        static_assert((int)ButtonState::COUNT == 6, "Make sure ToString is updated");
        switch(state)
        {
            case ButtonState::NORMAL:
                return "NORMAL";
            case ButtonState::HOVER:
                return "HOVER";
            case ButtonState::ON_CLICK:
                return "ON_CLICK";
            case ButtonState::CLICKING:
                return "CLICKING";
            case ButtonState::CLICKED:
                return "CLICKED";
            case ButtonState::NOT_INTERACTABLE:
                return "NOT_INTERACTABLE";
            case ButtonState::COUNT:
                return "COUNT";
        }
        
        return "";
    }
}

}

#endif