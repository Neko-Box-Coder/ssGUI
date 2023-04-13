#ifndef SSGUI_WINDOW_DRAG_STATE_H
#define SSGUI_WINDOW_DRAG_STATE_H

#include "ssGUI/HelperClasses/EnumToStringMacro.hpp"
#include <string>
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
    COUNT       - Count
    */
    enum class WindowDragState
    {
        NONE,
        STARTED,
        DRAGGING,
        ENDED,
        DISABLED,
        COUNT
    };
    
    //function: WindowDragStateToString
    inline std::string WindowDragStateToString(WindowDragState dragState)
    {
        static_assert((int)WindowDragState::COUNT == 5, "ToString");
        switch(dragState)
        {
            RETURN_ENUM_STRING(WindowDragState::NONE);
            RETURN_ENUM_STRING(WindowDragState::STARTED);
            RETURN_ENUM_STRING(WindowDragState::DRAGGING);
            RETURN_ENUM_STRING(WindowDragState::ENDED);
            RETURN_ENUM_STRING(WindowDragState::DISABLED);
            RETURN_ENUM_STRING(WindowDragState::COUNT);
        }
        
        return "";
    }
}

}

#endif