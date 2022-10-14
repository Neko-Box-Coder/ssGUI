#include "ssGUI/GUIObjectClasses/Window.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

#include "ssGUI/EventCallbacks/BackgroundColorChangedEventCallback.hpp"
#include "ssGUI/Enums/MouseButton.hpp"
#include "ssGUI/EventCallbacks/OnWindowCloseEventCallback.hpp"
#include "ssGUI/EventCallbacks/WindowDragStateChangedEventCallback.hpp"
#include "ssGUI/Extensions/Border.hpp"

namespace ssGUI
{
    void Window::SetWindowDragState(ssGUI::Enums::WindowDragState dragState)
    {
        if(CurrentDragState == dragState)
            return;
        
        CurrentDragState = dragState;

        if(IsEventCallbackExist(ssGUI::EventCallbacks::WindowDragStateChangedEventCallback::EVENT_NAME))
           GetEventCallback(ssGUI::EventCallbacks::WindowDragStateChangedEventCallback::EVENT_NAME)->Notify(this);
    }

    void Window::OnMouseDownUpdate(glm::vec2 currentMousePos, ssGUI::InputStatus& inputStatus)
    {
        ssLOG_FUNC_ENTRY();
        
        MouseDownPosition = currentMousePos;
        TransformTotalMovedDistance = glm::vec2();
        OnTransformBeginSize = GetSize();

        if(GetResizeType() != ssGUI::Enums::ResizeType::NONE)
        {
            if(currentMousePos.x >= GetGlobalPosition().x - ResizeHitbox && currentMousePos.x <= GetGlobalPosition().x + GetSize().x + ResizeHitbox)
            {
                if(std::abs(currentMousePos.y - GetGlobalPosition().y) <= ResizeHitbox && 
                    (GetResizeType() == ssGUI::Enums::ResizeType::ALL || GetResizeType() == ssGUI::Enums::ResizeType::TOP))
                {
                    ResizingTop = true;
                }
                else if(std::abs(currentMousePos.y - (GetGlobalPosition().y + GetSize().y)) <= ResizeHitbox && 
                    (GetResizeType() == ssGUI::Enums::ResizeType::ALL || GetResizeType() == ssGUI::Enums::ResizeType::BOTTOM))
                {
                    ResizingBot = true;                        
                }
            }

            if(currentMousePos.y >= GetGlobalPosition().y - ResizeHitbox && currentMousePos.y <= GetGlobalPosition().y + GetSize().y + ResizeHitbox)
            {
                if(std::abs(currentMousePos.x - GetGlobalPosition().x) <= ResizeHitbox && 
                    (GetResizeType() == ssGUI::Enums::ResizeType::ALL || GetResizeType() == ssGUI::Enums::ResizeType::LEFT))
                {
                    ResizingLeft = true;   
                }
                else if(std::abs(currentMousePos.x - (GetGlobalPosition().x + GetSize().x)) <= ResizeHitbox && 
                    (GetResizeType() == ssGUI::Enums::ResizeType::ALL || GetResizeType() == ssGUI::Enums::ResizeType::RIGHT))
                {
                    ResizingRight = true;
                }
            }
        }
        
        //Resize
        if(ResizingBot || ResizingLeft || ResizingRight || ResizingTop)
        {
            inputStatus.MouseInputBlockedObject = this;
            SetFocus(true);
            ssLOG_FUNC_EXIT();
            return;
        }
            
        //Titlebar drag
        if( currentMousePos.x >= GetGlobalPosition().x && currentMousePos.x <= GetGlobalPosition().x + GetSize().x && 
            currentMousePos.y >= GetGlobalPosition().y && currentMousePos.y <= GetGlobalPosition().y + TitlebarHeight &&
            IsDraggable())
        {
            Dragging = true;
            inputStatus.MouseInputBlockedObject = this;
            SetFocus(true);
            SetWindowDragState(ssGUI::Enums::WindowDragState::STARTED);
            
            ssLOG_FUNC_EXIT();
            return;
        }
        //Input blocking
        if( currentMousePos.x >= GetGlobalPosition().x && currentMousePos.x <= GetGlobalPosition().x + GetSize().x && 
            currentMousePos.y >= GetGlobalPosition().y && currentMousePos.y <= GetGlobalPosition().y + GetSize().y)
        {
            inputStatus.MouseInputBlockedObject = this;                
            SetFocus(true);
        }


        ssLOG_FUNC_EXIT();
    }

    void Window::OnMouseDragOrResizeUpdate(ssGUI::InputStatus& inputStatus, glm::vec2 mouseDelta, ssGUI::Backend::BackendSystemInputInterface* inputInterface)
    {
        ssLOG_FUNC_ENTRY();
        inputStatus.MouseInputBlockedObject = this;
       
        if(ResizingLeft || ResizingRight || ResizingTop || ResizingBot)
        {
            //Resize
            glm::vec2 newSize = OnTransformBeginSize;
            if(ResizingTop)
            {
                glm::vec2 newPos = GetGlobalPosition();
                newSize -= glm::vec2(0, mouseDelta.y);

                if(newSize.y < GetMinSize().y)
                    newSize.y = GetMinSize().y;
                else if(newSize.y > GetMaxSize().y)
                    newSize.y = GetMaxSize().y;

                float posYDelta = OnTransformBeginSize.y - newSize.y;
                if(posYDelta - TransformTotalMovedDistance.y != 0)
                {
                    newPos.y += posYDelta - TransformTotalMovedDistance.y;
                    TransformTotalMovedDistance.y = posYDelta;
                    SetGlobalPosition(newPos);
                }
            }
            else if(ResizingBot)
            {
                newSize += glm::vec2(0, mouseDelta.y);
            }
            
            if(ResizingLeft)
            {
                glm::vec2 newPos = GetGlobalPosition();
                newSize -= glm::vec2(mouseDelta.x, 0);

                if(newSize.x < GetMinSize().x)
                    newSize.x = GetMinSize().x;
                else if(newSize.x > GetMaxSize().x)
                    newSize.x = GetMaxSize().x;

                float posXDelta = OnTransformBeginSize.x - newSize.x;
                if(posXDelta - TransformTotalMovedDistance.x != 0)
                {
                    newPos.x += posXDelta - TransformTotalMovedDistance.x;
                    TransformTotalMovedDistance.x = posXDelta;
                    SetGlobalPosition(newPos);
                }
            }
            else if(ResizingRight)
            {
                newSize += glm::vec2(mouseDelta.x, 0);
            }
            SetSize(newSize);
        }
        //Titlebar Drag
        else if(Dragging)
        {
            glm::vec2 newPos = GetGlobalPosition();
            newPos += mouseDelta - TransformTotalMovedDistance;
            TransformTotalMovedDistance = mouseDelta;
            SetGlobalPosition(newPos);
            SetWindowDragState(ssGUI::Enums::WindowDragState::DRAGGING);
        }

        //Updating cursor
        if(ResizingTop && ResizingLeft)
            inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_TOP_LEFT);
        else if(ResizingTop && ResizingRight)
            inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_TOP_RIGHT);
        else if(ResizingBot && ResizingLeft)
            inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_BOTTOM_LEFT);
        else if(ResizingBot && ResizingRight)
            inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_BOTTOM_RIGHT);
        else if(ResizingTop)
            inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_UP);
        else if(ResizingLeft)
            inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_LEFT);
        else if(ResizingBot)
            inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_DOWN);
        else if(ResizingRight)
            inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_RIGHT);

        ssLOG_FUNC_EXIT();
    }

    void Window::BlockMouseInputAndUpdateCursor(ssGUI::InputStatus& inputStatus, glm::vec2 currentMousePos, ssGUI::Backend::BackendSystemInputInterface* inputInterface)
    {
        ssLOG_FUNC_ENTRY();
        
        if(inputStatus.MouseInputBlockedObject == nullptr)
        {
            //Mouse Input blocking
            bool mouseInWindowBoundX = false;
            bool mouseInWindowBoundY = false;
            
            if(currentMousePos.x >= GetGlobalPosition().x && currentMousePos.x <= GetGlobalPosition().x + GetSize().x)
                mouseInWindowBoundX = true;

            if(currentMousePos.y >= GetGlobalPosition().y && currentMousePos.y <= GetGlobalPosition().y + GetSize().y)
                mouseInWindowBoundY = true;
            
            //Input blocking
            if(mouseInWindowBoundX && mouseInWindowBoundY)
                inputStatus.MouseInputBlockedObject = this;
            
            //Updating cursor
            bool canResizeTop = false;
            bool canResizeBot = false;
            bool canResizeLeft = false;
            bool canResizeRight = false;

            if(currentMousePos.x >= GetGlobalPosition().x - ResizeHitbox && currentMousePos.x <= GetGlobalPosition().x + GetSize().x + ResizeHitbox)
            {
                if(std::abs(currentMousePos.y - GetGlobalPosition().y) <= ResizeHitbox && 
                    (GetResizeType() == ssGUI::Enums::ResizeType::ALL || GetResizeType() == ssGUI::Enums::ResizeType::TOP))
                {
                    canResizeTop = true;
                }
                else if(std::abs(currentMousePos.y - (GetGlobalPosition().y + GetSize().y)) <= ResizeHitbox && 
                    (GetResizeType() == ssGUI::Enums::ResizeType::ALL || GetResizeType() == ssGUI::Enums::ResizeType::BOTTOM))
                {
                    canResizeBot = true;                        
                }
            }

            if(currentMousePos.y >= GetGlobalPosition().y - ResizeHitbox && currentMousePos.y <= GetGlobalPosition().y + GetSize().y + ResizeHitbox)
            {
                if(std::abs(currentMousePos.x - GetGlobalPosition().x) <= ResizeHitbox && 
                    (GetResizeType() == ssGUI::Enums::ResizeType::ALL || GetResizeType() == ssGUI::Enums::ResizeType::LEFT))
                {
                    canResizeLeft = true;   
                }
                else if(std::abs(currentMousePos.x - (GetGlobalPosition().x + GetSize().x)) <= ResizeHitbox && 
                    (GetResizeType() == ssGUI::Enums::ResizeType::ALL || GetResizeType() == ssGUI::Enums::ResizeType::RIGHT))
                {
                    canResizeRight = true;
                }
            }

            if(canResizeTop && canResizeLeft)
                inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_TOP_LEFT);
            else if(canResizeTop && canResizeRight)
                inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_TOP_RIGHT);
            else if(canResizeBot && canResizeLeft)
                inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_BOTTOM_LEFT);
            else if(canResizeBot && canResizeRight)
                inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_BOTTOM_RIGHT);
            else if(canResizeTop)
                inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_UP);
            else if(canResizeLeft)
                inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_LEFT);
            else if(canResizeBot)
                inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_DOWN);
            else if(canResizeRight)
                inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_RIGHT);
        }
        
        //Reset drag and resize variables
        ResizingTop = false;
        ResizingBot = false;
        ResizingLeft = false;
        ResizingRight = false;
        Dragging = false;

        if(GetWindowDragState() == ssGUI::Enums::WindowDragState::DRAGGING)
            SetWindowDragState(ssGUI::Enums::WindowDragState::ENDED);
        else
            SetWindowDragState(ssGUI::Enums::WindowDragState::NONE);

        OnTransformBeginSize = glm::vec2();
        TransformTotalMovedDistance = glm::vec2();
        MouseDownPosition = glm::vec2();

        ssLOG_FUNC_EXIT();
    }

    void Window::ConstructRenderInfo()
    {
        glm::vec2 drawPosition = GetGlobalPosition();

        //Base window
        DrawingVerticies.push_back(drawPosition);
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingVerticies.push_back(drawPosition + glm::vec2(GetSize().x, 0));
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingVerticies.push_back(drawPosition + glm::vec2(GetSize().x, GetSize().y));
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingVerticies.push_back(drawPosition + glm::vec2(0, GetSize().y));
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingCounts.push_back(4);
        DrawingProperties.push_back(ssGUI::DrawingProperty());

        //Title bar
        if(!HasTitlebar())
            return;

        glm::u8vec4 titlebarColor = GetBackgroundColor();

        auto rgbAdder = [](glm::uint8* rgbField, int fieldDifference)->void
        {
            if((int)*rgbField + fieldDifference < 0)
                *rgbField = 0;
            else if((int)*rgbField + fieldDifference > 255)
                *rgbField = 255;
            else
                *rgbField += fieldDifference;
        };

        rgbAdder(&titlebarColor.r, TitlebarColorDifference.r);
        rgbAdder(&titlebarColor.g, TitlebarColorDifference.g);
        rgbAdder(&titlebarColor.b, TitlebarColorDifference.b);
        rgbAdder(&titlebarColor.a, TitlebarColorDifference.a);

        DrawingVerticies.push_back(drawPosition);
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(titlebarColor);

        DrawingVerticies.push_back(drawPosition + glm::vec2(GetSize().x, 0));
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(titlebarColor);

        DrawingVerticies.push_back(drawPosition + glm::vec2(GetSize().x, TitlebarHeight));
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(titlebarColor);

        DrawingVerticies.push_back(drawPosition + glm::vec2(0, TitlebarHeight));
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(titlebarColor);

        DrawingCounts.push_back(4);
        DrawingProperties.push_back(ssGUI::DrawingProperty());
        
        // std::cout<<"drawPosition: "<<drawPosition.x<<", "<<drawPosition.y<<"\n";
    }

    void Window::MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                            ssGUI::GUIObject* mainWindow)
    {
        glm::vec2 currentMousePos = inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow));
        glm::vec2 mouseDelta = currentMousePos - MouseDownPosition;

        // std::cout << "current mouse pos: "<<currentMousePos.x <<", "<<currentMousePos.y<<"\n";
        // std::cout << "current window pos: "<<GetGlobalPosition().x<<", "<<GetGlobalPosition().y<<"\n";
        // std::cout << "current window size: "<<GetSize().x<<", "<<GetSize().y<<"\n";
        // std::cout<<"\n";
        
        //Resize
        //On mouse down
        if(inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) && !inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::LEFT) &&
            inputStatus.MouseInputBlockedObject == nullptr)
        {
            OnMouseDownUpdate(currentMousePos, inputStatus);
        }
        //When the user is resizing or dragging the window
        else if(inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) && (ResizingLeft || ResizingRight || ResizingTop || ResizingBot || Dragging))
        {
            OnMouseDragOrResizeUpdate(inputStatus, mouseDelta, inputInterface);
        }
        //Otherwise show resize cursor if necessary 
        else if(inputStatus.MouseInputBlockedObject == nullptr)
        {
            BlockMouseInputAndUpdateCursor(inputStatus, currentMousePos, inputInterface);
        }
    }
        
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
        AddEventCallback(ssGUI::Factory::Create<ssGUI::EventCallbacks::OnWindowCloseEventCallback>());
        AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Border>());
        SetSize(glm::vec2(200, 200));
        SetAdaptiveTitlebarColor(true);
        SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));
        SetAdaptiveTitlebarColor(false);
    }

    Window::~Window()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
    }

    bool Window::IsClosed() const
    {
        return Closed;
    }

    void Window::Close()
    {
        Internal_OnClose();
    }

    void Window::AbortClosing()
    {
        IsClosingAborted = true;
    }

    void Window::Internal_OnClose()
    {        
        if(IsEventCallbackExist(ssGUI::EventCallbacks::OnWindowCloseEventCallback::EVENT_NAME))
        {
            GetEventCallback(ssGUI::EventCallbacks::OnWindowCloseEventCallback::EVENT_NAME)->Notify(this);
            if(IsClosingAborted)
            {
                IsClosingAborted = false;
                return;
            }
        }

        Closed = true;

        Delete();
    }

    void Window::SetTitlebar(bool set)
    {
        Titlebar = set;
        RedrawObject();
    }

    bool Window::HasTitlebar() const
    {
        return Titlebar;
    }

    void Window::SetTitlebarHeight(int height)
    {
        TitlebarHeight = height;
        RedrawObject();
    }

    int Window::GetTitlebarHeight() const
    {
        return HasTitlebar() ? TitlebarHeight : 0;
    }

    void Window::SetTitlebarColor(glm::u8vec4 color)
    {
        TitlebarColorDifference = static_cast<glm::ivec4>(color) - static_cast<glm::ivec4>(GetBackgroundColor());
        RedrawObject();
    }

    glm::u8vec4 Window::GetTitlebarColor() const
    {
        return static_cast<glm::u8vec4>(static_cast<glm::ivec4>(GetBackgroundColor()) + TitlebarColorDifference);
    }

    void Window::SetAdaptiveTitlebarColor(bool adaptive)
    {
        AdaptiveTitlebarColor = adaptive;
    }

    bool Window::IsAdaptiveTitlebarColor() const
    {
        return AdaptiveTitlebarColor;
    }

    void Window::SetResizeType(ssGUI::Enums::ResizeType resizeType)
    {
        ResizeType = resizeType;
    }

    ssGUI::Enums::ResizeType Window::GetResizeType() const
    {
        return ResizeType;
    }

    int Window::GetResizeHitbox() const
    {
        return ResizeHitbox;
    }
    
    void Window::SetResizeHitbox(int hitbox)
    {
        ResizeHitbox = hitbox;
    }

    void Window::SetClosable(bool closable)
    {
        Closable = closable;
    }

    bool Window::IsClosable() const
    {
        return Closable;
    }

    void Window::SetDraggable(bool draggable)
    {
        if(draggable)
            SetWindowDragState(ssGUI::Enums::WindowDragState::NONE);
        else
            SetWindowDragState(ssGUI::Enums::WindowDragState::DISABLED);
        
        Draggable = draggable;
    }

    bool Window::IsDraggable() const
    {
        return Draggable;
    }

    ssGUI::Enums::WindowDragState Window::GetWindowDragState() const
    {
        return CurrentDragState;
    }

    bool Window::IsDragging() const
    {
        return Dragging;
    }

    bool Window::IsResizing() const
    {
        return ResizingTop || ResizingRight || ResizingBot || ResizingLeft;
    }

    ssGUI::WindowResizeDragData Window::GetResizeDragData() const
    {
        ssGUI::WindowResizeDragData returnData;
        returnData.CurrentDragState = CurrentDragState;
        returnData.ResizingTop = ResizingTop;
        returnData.ResizingBot = ResizingBot;
        returnData.ResizingLeft = ResizingLeft;
        returnData.ResizingRight = ResizingRight;
        returnData.Dragging = Dragging;
        returnData.TransformTotalMovedDistance = TransformTotalMovedDistance;
        returnData.OnTransformBeginSize = OnTransformBeginSize;
        returnData.MouseDownPosition = MouseDownPosition;

        return returnData;
    }

    void Window::SetResizeDragData(ssGUI::WindowResizeDragData data)
    {
        ssGUI::WindowResizeDragData returnData;
        CurrentDragState = data.CurrentDragState;
        ResizingTop = data.ResizingTop;
        ResizingBot = data.ResizingBot;
        ResizingLeft = data.ResizingLeft;
        ResizingRight = data.ResizingRight;
        Dragging = data.Dragging;
        TransformTotalMovedDistance = data.TransformTotalMovedDistance;
        OnTransformBeginSize = data.OnTransformBeginSize;
        MouseDownPosition = data.MouseDownPosition;
    }

    void Window::SetDeleteAfterClosed(bool deleteAfterClosed)
    {
        DeleteAfterClosed = deleteAfterClosed;
    }

    bool Window::IsDeleteAfterClosed() const
    {
        return DeleteAfterClosed;
    }

    void Window::SetOnTopWhenFocused(bool top)
    {
        OnTopWhenFocused = top;
    }

    bool Window::IsOnTopWhenFocused() const
    {
        return OnTopWhenFocused;
    }

    void Window::SetFocus(bool focus)
    {       
        if(focus && IsOnTopWhenFocused())
            SetParent(GetParent());

        GUIObject::SetFocus(focus);
    }

    void Window::Internal_SetSelfFocus(bool focus)
    {
        if(focus && IsOnTopWhenFocused())
            SetParent(GetParent());
        
        GUIObject::Internal_SetSelfFocus(focus);
    }

    void Window::SetBackgroundColor(glm::u8vec4 color)
    {
        if(AdaptiveTitlebarColor)
            BackgroundColour = color;
        else
        {
            auto titlebarColor = GetTitlebarColor();
            BackgroundColour = color;
            //Reapply titlebar color
            SetTitlebarColor(titlebarColor);
        }

        if(IsAnyEventCallbackExist<ssGUI::EventCallbacks::BackgroundColorChangedEventCallback>())
            GetAnyEventCallback<ssGUI::EventCallbacks::BackgroundColorChangedEventCallback>()->Notify(this);

        RedrawObject();
    }

    ssGUI::Enums::GUIObjectType Window::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::WINDOW;
    }

    Window* Window::Clone(bool cloneChildren)
    {
        ssLOG_FUNC_ENTRY();
        Window* temp = new Window(*this);
        CloneExtensionsAndEventCallbacks(temp);   
        
        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
            {
                ssLOG_FUNC_EXIT();
                return nullptr;
            }
        }

        ssLOG_FUNC_EXIT();
        return temp;
    }
}