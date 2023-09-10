#ifndef SSGUI_INPUT_STATUS_H
#define SSGUI_INPUT_STATUS_H

#include "ssGUI/DataClasses/BlockData.hpp"
#include "ssGUI/DataClasses/DragData.hpp"

//namespace: ssGUI
namespace ssGUI
{
    class GUIObject;
    
    //class: ssGUI::InputStatus
    //This holds the status information such as blocking or docking for different inputs
    struct InputStatus
    {
        public:
            //var: MouseInputBlockedData
            //The object/data that is currently blocking mouse input
            BlockData MouseInputBlockedData;
            
            //var: KeyInputBlockedData
            //The object/data that is currently blocking key press input
            BlockData KeyInputBlockedData;

            //var: LegacyDockingBlockedObject (Deprecated)
            //Blocks docking of a window if there's another GUI Object on top of it.
            ssGUI::GUIObject* LegacyDockingBlockedObject = nullptr;
            
            //var: CurrentDragData
            //The data currently being dragged by the mouse
            DragData CurrentDragData;
    };
}

#endif