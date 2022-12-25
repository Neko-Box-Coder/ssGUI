#ifndef SSGUI_MOUSE_BUTTON_H
#define SSGUI_MOUSE_BUTTON_H

#include <cstdint>

namespace ssGUI 
{ 

//namespace: ssGUI::Enums
namespace Enums
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

}

#endif