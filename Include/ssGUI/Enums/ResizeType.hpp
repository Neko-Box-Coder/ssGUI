#ifndef SSGUI_RESIZE_TYPE
#define SSGUI_RESIZE_TYPE

//namespace: ssGUI::Enums
namespace ssGUI::Enums
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

#endif