#ifndef SSGUI_BUTTON_STATE
#define SSGUI_BUTTON_STATE

namespace ssGUI
{

//namespace: ssGUI::Enums
namespace Enums
{
    /*enum: ButtonState
    
    NORMAL      - Normal button state 
    HOVER       - Hover button state
    ON_CLICK    - The moment the button is being clicked (Only trigger once)
    CLICKING    - When the button is being clicked (Will be triggered continuously)
    CLICKED     - The moment after the button is being clicked (Only trigger once)
    DISABLED    - When the button is disabled
    */
    enum class ButtonState
    {
        NORMAL,
        HOVER,
        ON_CLICK,
        CLICKING,
        CLICKED,
        DISABLED
    };
}

}

#endif