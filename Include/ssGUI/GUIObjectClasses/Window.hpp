#ifndef SSGUI_WINDOW_H
#define SSGUI_WINDOW_H

#include "ssGUI/GUIObjectClasses/GUIObject.hpp"
#include "ssGUI/Enums/ResizeType.hpp"
#include "ssGUI/DataClasses/WindowResizeDragData.hpp"
#include "ssGUI/Enums/WindowDragState.hpp"
#include <cmath>
#include <functional>

//namespace: ssGUI
namespace ssGUI
{
    /*class: ssGUI::Window
    A base classes for any window GUI Object (including <MainWindow>). By itself, it can be used as a window that's inside <MainWindow>. 
    By default, it can be resized or move by a cursor.

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        //Window status
        bool Titlebar;                                          //See <HasTitlebar>
        int TitlebarHeight;                                     //See <GetTitlebarHeight>
        ssGUI::Enums::ResizeType ResizeType;                    //See <GetResizeType>
        bool Draggable;                                         //See <IsDraggable>
        bool Closable;                                          //See <IsClosable>
        bool Closed;                                            //See <IsClosed>
        bool IsClosingAborted;                                  //(Internal variable) See <AbortClosing>
        glm::ivec4 TitlebarColorDifference;                     //(Internal variable) Used for <IsAdaptiveTitlebarColor>
        bool AdaptiveTitlebarColor;                             //See <IsAdaptiveTitlebarColor>
        bool DeleteAfterClosed;                                 //See <IsDeleteAfterClosed>
        bool OnTopWhenFocused;                                  //See <IsOnTopWhenFocused>

        //Resize/Drag settings
        ssGUI::Enums::WindowDragState CurrentDragState;         //See <GetResizeDragData>
        int ResizeHitbox;                                       //See <GetResizeHitbox>
        bool ResizingTop;                                       //(Internal variable) Used to keep track of resizing status
        bool ResizingBot;                                       //(Internal variable) Used to keep track of resizing status
        bool ResizingLeft;                                      //(Internal variable) Used to keep track of resizing status
        bool ResizingRight;                                     //(Internal variable) Used to keep track of resizing status
        bool Dragging;                                          //(Internal variable) Used to keep track of dragging status
        glm::vec2 TransformTotalMovedDistance;                  //(Internal variable) Used for window dragging
        glm::vec2 OnTransformBeginSize;                         //(Internal variable) Used for window resizing
        glm::vec2 MouseDownPosition;                            //(Internal variable) Used for <GetResizeDragData> 
    =================================================================
    ============================== C++ ==============================
    Window::Window() :  Titlebar(true),
                        TitlebarHeight(20),
                        ResizeType(ssGUI::Enums::ResizeType::ALL),
                        Draggable(true),
                        Closable(true),
                        Closed(false),
                        IsClosingAborted(false),
                        TitlebarColorDifference(-40, -40, -40, 0),
                        AdaptiveTitlebarColor(false),
                        DeleteAfterClosed(true),
                        OnTopWhenFocused(true),
                        CurrentDragState(ssGUI::Enums::WindowDragState::NONE),
                        ResizeHitbox(5),
                        ResizingTop(false),
                        ResizingBot(false),
                        ResizingLeft(false),
                        ResizingRight(false),
                        Dragging(false),
                        TransformTotalMovedDistance(),
                        OnTransformBeginSize(),
                        MouseDownPosition()
    {       
        AddEventCallback(ssGUI::Enums::EventType::BEFORE_WINDOW_CLOSE);
        AddExtension<ssGUI::Extensions::Border>();
        SetSize(glm::vec2(200, 200));
        SetAdaptiveTitlebarColor(true);
        SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));
        SetAdaptiveTitlebarColor(false);
    }
    =================================================================    
    */
    class Window : public GUIObject
    {
        private:
            Window& operator=(Window const& other) = default;

        protected:
            //Window status
            bool Titlebar;                                          //See <HasTitlebar>
            int TitlebarHeight;                                     //See <GetTitlebarHeight>
            ssGUI::Enums::ResizeType ResizeType;                    //See <GetResizeType>
            bool Draggable;                                         //See <IsDraggable>
            bool Closable;                                          //See <IsClosable>
            bool Closed;                                            //See <IsClosed>
            bool IsClosingAborted;                                  //(Internal variable) See <AbortClosing>
            glm::ivec4 TitlebarColorDifference;                     //(Internal variable) Used for <IsAdaptiveTitlebarColor>
            bool AdaptiveTitlebarColor;                             //See <IsAdaptiveTitlebarColor>
            bool DeleteAfterClosed;                                 //See <IsDeleteAfterClosed>
            bool OnTopWhenFocused;                                  //See <IsOnTopWhenFocused>

            //Resize/Drag settings
            ssGUI::Enums::WindowDragState CurrentDragState;         //See <GetResizeDragData>
            int ResizeHitbox;                                       //See <GetResizeHitbox>
            bool ResizingTop;                                       //(Internal variable) Used to keep track of resizing status
            bool ResizingBot;                                       //(Internal variable) Used to keep track of resizing status
            bool ResizingLeft;                                      //(Internal variable) Used to keep track of resizing status
            bool ResizingRight;                                     //(Internal variable) Used to keep track of resizing status
            bool Dragging;                                          //(Internal variable) Used to keep track of dragging status
            glm::vec2 TransformTotalMovedDistance;                  //(Internal variable) Used for window dragging
            glm::vec2 OnTransformBeginSize;                         //(Internal variable) Used for window resizing
            glm::vec2 MouseDownPosition;                            //(Internal variable) Used for <GetResizeDragData> 

            virtual void SetWindowDragState(ssGUI::Enums::WindowDragState dragState);
            virtual void OnMouseDownUpdate(glm::vec2 currentMousePos, ssGUI::InputStatus& inputStatus);
            virtual void OnMouseDragOrResizeUpdate(ssGUI::InputStatus& inputStatus, glm::vec2 mouseDelta, ssGUI::Backend::BackendSystemInputInterface* inputInterface);
            virtual void BlockMouseInputAndUpdateCursor(ssGUI::InputStatus& inputStatus, glm::vec2 currentMousePos, ssGUI::Backend::BackendSystemInputInterface* inputInterface);

            Window(Window const& other);
            virtual void ConstructRenderInfo() override;
            virtual void MainLogic( ssGUI::Backend::BackendSystemInputInterface* inputInterface, 
                                    ssGUI::InputStatus& currentInputStatus, 
                                    ssGUI::InputStatus& lastInputStatus, 
                                    ssGUI::GUIObject* mainWindow) override;

        public:
            static const std::string WINDOW_BASE_SHAPE_NAME;
            static const std::string WINDOW_TITLEBAR_SHAPE_NAME;
            
            Window();
            virtual ~Window() override;
            
            //function: Close
            //Calling this function will triggers the <ssGUI::EventCallbacks::OnWindowCloseEventCallback> and sets this window's parent to nullptr.
            //By default, the window will be deleted automatically after closing. You can change this behaviour by calling <SetDeleteAfterClosed>.
            virtual void Close();
            
            //function: IsClosed
            //Returns if this window is closed or not
            virtual bool IsClosed() const;
            
            //function: AbortClosing
            //Calling this function will abort the closing operation. Meaning calling <IsClosed> will return false.
            //Note that you *have to* call this in the <ssGUI::EventCallbacks::OnWindowCloseEventCallback> in order for this to work.
            virtual void AbortClosing();

            //function: Internal_OnClose
            //(Internal ssGUI function) You should not be calling this function normally.
            virtual void Internal_OnClose();
            
            //function: SetClosable
            //Sets if there's a close button for this window object. This *does not* affect <Close> function. 
            //This has no affect on the <Window> class. Classes that derives this class (such as <StandardWindow>) may use this.
            virtual void SetClosable(bool closable);
            
            //function: IsClosable
            //Gets if there's a close button for this window object. This *does not* affect <Close> function. 
            //This has no affect on the <Window> class. Classes that derives this class (such as <StandardWindow>) may use this.
            virtual bool IsClosable() const;
            
            //function: SetDeleteAfterClosed
            //If sets to true, the window will be deleted automatically after being closed
            virtual void SetDeleteAfterClosed(bool deleteAfterClosed);

            //function: IsDeleteAfterClosed
            //If returns true, the window will be deleted automatically after being closed
            virtual bool IsDeleteAfterClosed() const;

            //function: SetTitlebar
            //Sets if the titlebar exists or not
            virtual void SetTitlebar(bool set);
            
            //function: HasTitlebar
            //Gets if the titlebar exists or not
            virtual bool HasTitlebar() const;
            
            //function: SetTitlebarHeight
            virtual void SetTitlebarHeight(int height);
            
            //function: GetTitlebarHeight
            virtual int GetTitlebarHeight() const;

            //function: SetTitlebarColor
            virtual void SetTitlebarColor(glm::u8vec4 color);

            //function: GetTitlebarColor
            virtual glm::u8vec4 GetTitlebarColor() const;

            //function: SetAdaptiveTitlebarColor
            //Sets if the titlebar color "adapts" to the background color dynamically
            virtual void SetAdaptiveTitlebarColor(bool adaptive);

            //function: IsAdaptiveTitlebarColor
            //Returns if the titlebar "adapts" to the background color dynamically
            virtual bool IsAdaptiveTitlebarColor() const;
            
            //function: SetResizeType
            //Sets the resize type of the window. For MainWindow, only <Enums::ResizeType::ALL> or <Enums::ResizeType::NONE> will work.
            virtual void SetResizeType(ssGUI::Enums::ResizeType resizeType);
            
            //function: GetResizeType
            //Returns the resize type of the window
            virtual ssGUI::Enums::ResizeType GetResizeType() const;
            
            //function: GetResizeHitbox
            //Gets the resize hitbox. This is the range from the edge of the window for both direction.
            virtual int GetResizeHitbox() const;
            
            //function: SetResizeHitbox
            //Sets the resize hitbox. This is the range from the edge of the window for both direction.
            virtual void SetResizeHitbox(int hitbox);
            
            //function: IsResizing
            //Returns true if the user is currently resizing the window. Not supporting MainWindow for now.
            virtual bool IsResizing() const;

            //function: SetDraggable
            //Sets if the window can be dragged to move around
            virtual void SetDraggable(bool draggable);

            //function: IsDraggable
            //Gets if the window can be dragged to move around
            virtual bool IsDraggable() const;

            //function: GetWindowDragState
            virtual ssGUI::Enums::WindowDragState GetWindowDragState() const;

            //fucntion: IsDragging
            //Returns true if the user is currently dragging the window. Not supporting MainWindow for now.
            virtual bool IsDragging() const;

            //function: GetResizeDragData
            //Returns the current resize and drag status of the window
            virtual ssGUI::WindowResizeDragData GetResizeDragData() const;

            //function: SetResizeDragData
            //Sets the current resize and drag status of the window. 
            //Only use it if you know what you are doing 
            virtual void SetResizeDragData(ssGUI::WindowResizeDragData data);

            //function: SetOnTopWhenFocused
            //Set if the window will be on top when focused, meaning it will be the last child if true
            virtual void SetOnTopWhenFocused(bool top);

            //function: IsOnTopWhenFocused
            //Returns if the window will be on top when focused, meaning it will be the last child if true
            virtual bool IsOnTopWhenFocused() const;

            //function: SetRenderSize
            //Sets the render size (area under the titlebar)           
            virtual void SetRenderSize(glm::ivec2 size);
            
            //function: GetRenderSize
            //Gets the render size (area under the titlebar)
            virtual glm::ivec2 GetRenderSize() const;

            //function: SetFocus
            //See <ssGUI::Hierarchy::SetFocus>
            virtual void SetFocus(bool focus) override;

            //function: Internal_SetSelfFocus
            //See <ssGUI::Hierarchy::Internal_SetSelfFocus>
            virtual void Internal_SetSelfFocus(bool focus) override;

            //function: SetBackgroundColor
            //See <ssGUI::Renderer::SetBackgroundColor>
            virtual void SetBackgroundColor(glm::u8vec4 color) override;

            //function: GetType
            //See <GUIObject::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: Clone
            //See <GUIObject::Clone>
            virtual Window* Clone(bool cloneChildren) override;
    };
}

#endif