#ifndef SSGUI_WINDOW_DRAG_STATE
#define SSGUI_WINDOW_DRAG_STATE

//namespace: ssGUI::Enums
namespace ssGUI::Enums
{
    /*enum: ButtonState
    
    NONE      - TODO 
    STARTED    - TODO (Only trigger once)
    DRAGGING    - TODO (Will be triggered continuously)
    ENDED     - TODO (Only trigger once)
    DISABLED    - TODO
    */
    enum class WindowDragState
    {
        NONE,
        STARTED,
        DRAGGING,
        ENDED,
        DISABLED
    };
}


#endif