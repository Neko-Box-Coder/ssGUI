#ifndef SSGUI_MOUSE_BUTTON
#define SSGUI_MOUSE_BUTTON

#include <cstdint>

//namespace: ssGUI::Enums
namespace ssGUI::Enums
{
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
        COUNT = RIGHT - LEFT + 1
    };
}


#endif