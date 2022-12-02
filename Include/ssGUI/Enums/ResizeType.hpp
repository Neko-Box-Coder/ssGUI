#ifndef SSGUI_RESIZE_TYPE_H
#define SSGUI_RESIZE_TYPE_H

namespace ssGUI
{

//namespace: ssGUI::Enums
namespace Enums
{
    /*enum: ResizeType
    
    NONE    - Window doesn't resize
    ALL     - Window can be resized from all side
    TOP     - Window can only be reside from the top 
    LEFT    - Window can only be reside from the left
    BOTTOM  - Window can only be reside from the bottom
    RIGHT   - Window can only be reside from the right
    */
    enum class ResizeType
    {
        NONE,
        ALL,
        TOP,
        LEFT,
        BOTTOM,
        RIGHT
    };
}

}

#endif