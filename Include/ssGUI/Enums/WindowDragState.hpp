#ifndef SSGUI_WINDOW_DRAG_STATE
#define SSGUI_WINDOW_DRAG_STATE

namespace ssGUI
{

//namespace: ssGUI::Enums
namespace Enums
{
    /*enum: WindowDragState
    
    NONE        - Window is not being dragged
    STARTED     - Window dragging begins in this update (Only trigger once)
    DRAGGING    - Window is currently being dragged (Will be triggered continuously)
    ENDED       - Window dragging ends in this update (Only trigger once)
    DISABLED    - Window can't be dragged
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

}

#endif