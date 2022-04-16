#ifndef SSGUI_WINDOW
#define SSGUI_WINDOW

#include "ssGUI/GUIObjectClasses/BaseGUIObject.hpp"
#include "ssGUI/Enums/MouseButton.hpp"
#include "ssGUI/Enums/ResizeType.hpp"
#include "ssGUI/DataClasses/WindowResizeDragData.hpp"
#include "ssGUI/Enums/WindowDragState.hpp"
#include "ssGUI/EventCallbacks/OnWindowCloseEventCallback.hpp"
#include "ssGUI/EventCallbacks/WindowDragStateChangedEventCallback.hpp"
#include "ssGUI/Extensions/Border.hpp"
#include <cmath>
#include <functional>

//namespace: ssGUI
namespace ssGUI
{
    /*class: Window
    A base classes for any window GUI Object (including <MainWindow>). By itself, it can be used as a window that's inside <MainWindow>. 
    By default, it can be resized or move by a cursor.

    Variables & Constructor:
    ============================== C++ ==============================
    private:
        //Window status
        bool Titlebar;
        int TitlebarHeight;
        ssGUI::Enums::ResizeType ResizeType;
        bool Draggable;
        bool Closable;
        bool Closed;
        bool IsClosingAborted;
        glm::ivec4 TitlebarColorDifference;
        bool DeleteAfterClosed;

        //Resize/Drag settings
        ssGUI::Enums::WindowDragState CurrentDragState;
        int ResizeHitbox;
        bool ResizingTop;
        bool ResizingBot;
        bool ResizingLeft;
        bool ResizingRight;
        bool Dragging;
        glm::vec2 TransformTotalMovedDistance;
        glm::vec2 OnTransformBeginSize;
        glm::vec2 MouseDownPosition;
    =================================================================
    ============================== C++ ==============================
    Window::Window() : Titlebar(true), TitlebarHeight(20), ResizeType(ssGUI::Enums::ResizeType::ALL), Draggable(true), Closable(true), Closed(false),
                       IsClosingAborted(false), TitlebarColorDifference(-40, -40, -40, 0), DeleteAfterClosed(true), 
                       CurrentDragState(ssGUI::Enums::WindowDragState::NONE), ResizeHitbox(5), ResizingTop(false), ResizingBot(false), ResizingLeft(false), 
                       ResizingRight(false), Dragging(false), TransformTotalMovedDistance(), OnTransformBeginSize(), MouseDownPosition()
    {       
        AddEventCallback(new ssGUI::EventCallbacks::OnWindowCloseEventCallback());
        AddExtension(new ssGUI::Extensions::Border());
        SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));
    }
    =================================================================    
    */
    class Window : public BaseGUIObject
    {
        private:
            Window& operator=(Window const& other) = default;

        protected:
            //Window status
            bool Titlebar;
            int TitlebarHeight;
            ssGUI::Enums::ResizeType ResizeType;
            bool Draggable;
            bool Closable;
            bool Closed;
            bool IsClosingAborted;
            glm::ivec4 TitlebarColorDifference;
            bool DeleteAfterClosed;

            //Resize/Drag settings
            ssGUI::Enums::WindowDragState CurrentDragState;
            int ResizeHitbox;
            bool ResizingTop;
            bool ResizingBot;
            bool ResizingLeft;
            bool ResizingRight;
            bool Dragging;
            glm::vec2 TransformTotalMovedDistance;
            glm::vec2 OnTransformBeginSize;
            glm::vec2 MouseDownPosition;

            virtual void SetWindowDragState(ssGUI::Enums::WindowDragState dragState);
            virtual void OnMouseDownUpdate(glm::vec2 currentMousePos, ssGUI::InputStatus& globalInputStatus);
            virtual void OnMouseDragOrResizeUpdate(ssGUI::InputStatus& globalInputStatus, glm::vec2 mouseDelta, ssGUI::Backend::BackendSystemInputInterface* inputInterface);
            virtual void BlockMouseInputAndUpdateCursor(ssGUI::InputStatus& globalInputStatus, glm::vec2 currentMousePos, ssGUI::Backend::BackendSystemInputInterface* inputInterface);

            Window(Window const& other) = default;
            virtual void ConstructRenderInfo() override;

        public:
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
            
            //function: SetClosable
            //Sets if there's a close button for this window object. This *does not* affect <Close> function. 
            //This has no affect on the <Window> class. Classes that derives this class (such as <StandardWindow>) may use this.
            virtual void SetClosable(bool closable);
            
            //function: IsClosable
            //Gets if there's a close button for this window object. This *does not* affect <Close> function. 
            //This has no affect on the <Window> class. Classes that derives this class (such as <StandardWindow>) may use this.
            virtual bool IsClosable() const;

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

            //function: IsResizing
            //Returns true if the user is currently resizing the window. Not supporting MainWindow for now.
            virtual bool IsResizing() const;

            //function: GetResizeDragData
            //Returns the current resize and drag status of the window
            virtual ssGUI::WindowResizeDragData GetResizeDragData() const;

            //function: SetResizeDragData
            //Sets the current resize and drag status of the window. 
            //Only use it if you know what you are doing 
            virtual void SetResizeDragData(ssGUI::WindowResizeDragData data);

            //function: SetDeleteAfterClosed
            //If sets to true, the window will be deleted automatically after being closed
            virtual void SetDeleteAfterClosed(bool deleteAfterClosed);

            //function: IsDeleteAfterClosed
            //If returns true, the window will be deleted automatically after being closed
            virtual bool IsDeleteAfterClosed() const;

            //function: AddOnCloseEventListener [Deprecated]
            //Proxy function for adding listener and <EventCallbacks::OnWindowCloseEventCallback> to this object 
            virtual int AddOnCloseEventListener(std::function<void()> onClose);
            
            //function: RemoveOnCloseEventListener [Deprecated]
            //Proxy function for removing listener from <EventCallbacks::OnWindowCloseEventCallback> on this object 
            virtual void RemoveOnCloseEventListener(int index);

            //function: SetBackgroundColor
            //See <BaseGUIObject::SetBackgroundColor>
            virtual void SetBackgroundColor(glm::u8vec4 color) override;

            //function: GetType
            //See <BaseGUIObject::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;
            
            //function: Delete 
            //See <BaseGUIObject::Delete>
            virtual void Delete() override;

            //function: Internal_Draw
            //See <BaseGUIObject::Internal_Draw>
            virtual void Internal_Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;
            
            //function: Internal_Update
            //See <BaseGUIObject::Internal_Update>
            virtual void Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;
            
            //function: Clone
            //See <BaseGUIObject::Clone>
            virtual GUIObject* Clone(bool cloneChildren) override;
    };
}






#endif