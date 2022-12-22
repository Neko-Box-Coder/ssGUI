#ifndef SSGUI_WINDOW_RESIZE_DRAG_DATA_H
#define SSGUI_WINDOW_RESIZE_DRAG_DATA_H

#include "glm/vec2.hpp"
#include "ssGUI/Enums/WindowDragState.hpp"


//namespace: ssGUI
namespace ssGUI
{
    //class: ssGUI::WindowResizeDragData
    class WindowResizeDragData
    {
        public:
            //var: CurrentDragState
            //The state of user dragging of thw window
            ssGUI::Enums::WindowDragState CurrentDragState;

            //var: ResizingTop
            //Is user currently resizing window from top
            bool ResizingTop;

            //var: ResizingBot
            //Is user currently resizing window from bottom
            bool ResizingBot;

            //var: ResizingLeft
            //Is user currently resizing window from left
            bool ResizingLeft;

            //var: ResizingRight
            //Is user currently resizing window from right
            bool ResizingRight;

            //var: Dragging
            //Is user currently dragging the window from titlebar
            bool Dragging;

            //var: TransformTotalMovedDistance
            //The total vector ther window has moved by user
            glm::vec2 TransformTotalMovedDistance;

            //var: OnTransformBeginSize
            //The window size at the beginning of resizing
            glm::vec2 OnTransformBeginSize;

            //var: MouseDownPosition
            //The global position where the mouse first clicked when resizing/dragging the window
            glm::vec2 MouseDownPosition;
    };
}


#endif