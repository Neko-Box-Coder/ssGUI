#include "ssGUI/BaseClasses/Window.hpp"

#include "ssGUI/BaseClasses/MainWindow.hpp" //For getting mouse position

namespace ssGUI
{
    void Window::SetWindowDragState(ssGUI::Enums::WindowDragState dragState)
    {
        if(CurrentDragState == dragState)
            return;
        
        CurrentDragState = dragState;


        //TODO : Cache this
        if(IsEventCallbackExist(ssGUI::EventCallbacks::WindowDragStateChangedEventCallback::EVENT_NAME))
           GetEventCallback(ssGUI::EventCallbacks::WindowDragStateChangedEventCallback::EVENT_NAME)->Notify(this);
    }

    void Window::OnMouseDownUpdate(glm::ivec2 currentMousePos, ssGUI::InputStatus& globalInputStatus)
    {
        FUNC_DEBUG_ENTRY();
        
        MouseDownPosition = currentMousePos;
        OnTransformBeginPosition = GetGlobalPosition();
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
        
        //Titlebar Drag
        if(ResizingBot || ResizingLeft || ResizingRight || ResizingTop)
        {
            globalInputStatus.MouseInputBlocked = true;
            FUNC_DEBUG_EXIT();
            return;
        }
            
        if( currentMousePos.x >= GetGlobalPosition().x && currentMousePos.x <= GetGlobalPosition().x + GetSize().x && 
            currentMousePos.y >= GetGlobalPosition().y && currentMousePos.y <= GetGlobalPosition().y + TitlebarHeight &&
            IsDraggable())
        {
            Dragging = true;
            globalInputStatus.MouseInputBlocked = true;
            SetWindowDragState(ssGUI::Enums::WindowDragState::STARTED);
            FUNC_DEBUG_EXIT();
            return;
        }

        //Input blocking
        if( currentMousePos.x >= GetGlobalPosition().x && currentMousePos.x <= GetGlobalPosition().x + GetSize().x && 
            currentMousePos.y >= GetGlobalPosition().y && currentMousePos.y <= GetGlobalPosition().y + GetSize().y)
        {
            globalInputStatus.MouseInputBlocked = true;   
        }

        FUNC_DEBUG_EXIT();
    }

    void Window::OnMouseDragOrResizeUpdate(ssGUI::InputStatus& globalInputStatus, glm::ivec2 mouseDelta, ssGUI::Backend::BackendSystemInputInterface* inputInterface)
    {
        FUNC_DEBUG_ENTRY();
        globalInputStatus.MouseInputBlocked = true;
            
        glm::ivec2 newPos = OnTransformBeginPosition;
        if(ResizingLeft || ResizingRight || ResizingTop || ResizingBot)
        {
            //Resize
            glm::ivec2 newSize = OnTransformBeginSize;
            if(ResizingTop)
            {
                newPos += glm::ivec2(0, mouseDelta.y);
                newSize -= glm::ivec2(0, mouseDelta.y);

                //Bound new pos to prevent moving the window when resizing reaches max or min
                if(newSize.y < GetMinSize().y)
                    newPos.y = OnTransformBeginPosition.y + (OnTransformBeginSize.y - GetMinSize().y);
                else if(newSize.y > GetMaxSize().y)
                    newPos.y = OnTransformBeginPosition.y - (GetMaxSize().y - OnTransformBeginSize.y);
            }
            else if(ResizingBot)
            {
                newSize += glm::ivec2(0, mouseDelta.y);
            }
            
            if(ResizingLeft)
            {
                newPos += glm::ivec2(mouseDelta.x, 0);
                newSize -= glm::ivec2(mouseDelta.x, 0);

                //Bound new pos to prevent moving the window when resizing reaches max or min
                if(newSize.x < GetMinSize().x)
                    newPos.x = OnTransformBeginPosition.x + (OnTransformBeginSize.x - GetMinSize().x);
                else if(newSize.x > GetMaxSize().x)
                    newPos.x = OnTransformBeginPosition.x - (GetMaxSize().x - OnTransformBeginSize.x);
            }
            else if(ResizingRight)
            {
                newSize += glm::ivec2(mouseDelta.x, 0);
            }
            SetSize(newSize);
        }
            
        //Titlebar Drag
        if(Dragging)
        {
            newPos += mouseDelta;
            SetWindowDragState(ssGUI::Enums::WindowDragState::DRAGGING);
        }

        SetGlobalPosition(newPos);

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

        FUNC_DEBUG_EXIT();
    }

    void Window::BlockMouseInputAndUpdateCursor(ssGUI::InputStatus& globalInputStatus, glm::ivec2 currentMousePos, ssGUI::Backend::BackendSystemInputInterface* inputInterface)
    {
        FUNC_DEBUG_ENTRY();
        
        if(!globalInputStatus.MouseInputBlocked)
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
                globalInputStatus.MouseInputBlocked = true;
            
            
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

        OnTransformBeginSize = glm::ivec2();
        OnTransformBeginPosition = glm::ivec2();
        MouseDownPosition = glm::ivec2();

        FUNC_DEBUG_EXIT();
    }
        
    Window::Window() : Titlebar(true), TitlebarHeight(20), ResizeType(ssGUI::Enums::ResizeType::ALL), Draggable(true), Closable(true), Closed(false),
                       IsClosingAborted(false), TitlebarColorDifference(-40, -40, -40, 0), CurrentDragState(ssGUI::Enums::WindowDragState::NONE), ResizeHitbox(5), ResizingTop(false), ResizingBot(false), 
                       ResizingLeft(false), ResizingRight(false), Dragging(false), OnTransformBeginPosition(), OnTransformBeginSize(), MouseDownPosition()
    {       
        AddEventCallback(new ssGUI::EventCallbacks::OnWindowCloseEventCallback());
        AddExtension(new ssGUI::Extensions::Border());
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
        /*
        for(int i = 0; i < OnCloseEventListeners.size(); i++)
        {
            if(OnCloseEventListenersValid[i])
                OnCloseEventListeners[i]();
        }*/

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

        SetParent(nullptr);
    }

    void Window::AbortClosing()
    {
        IsClosingAborted = true;
    }

    void Window::Internal_OnClose()
    {        
        /*
        for(int i = 0; i < OnCloseEventListeners.size(); i++)
        {
            if(OnCloseEventListenersValid[i])
                OnCloseEventListeners[i]();
        }*/

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
    }

    void Window::SetTitlebar(bool set)
    {
        Titlebar = set;
    }

    bool Window::HasTitlebar() const
    {
        return Titlebar;
    }

    void Window::SetTitlebarHeight(int height)
    {
        TitlebarHeight = height;
    }

    int Window::GetTitlebarHeight() const
    {
        return HasTitlebar() ? TitlebarHeight : 0;
    }

    void Window::SetTitlebarColor(glm::u8vec4 color)
    {
        TitlebarColorDifference = static_cast<glm::ivec4>(color) - static_cast<glm::ivec4>(GetBackgroundColor());
    }

    glm::u8vec4 Window::GetTitlebarColor() const
    {
        return static_cast<glm::u8vec4>(static_cast<glm::ivec4>(GetBackgroundColor()) + TitlebarColorDifference);
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

    int Window::AddOnCloseEventListener(std::function<void()> onClose)
    {
        if(!IsEventCallbackExist(ssGUI::EventCallbacks::OnWindowCloseEventCallback::EVENT_NAME))
            AddEventCallback(new ssGUI::EventCallbacks::OnWindowCloseEventCallback());
        
        return GetEventCallback(ssGUI::EventCallbacks::OnWindowCloseEventCallback::EVENT_NAME)->AddEventListener([onClose](ssGUI::GUIObject*, ssGUI::GUIObject*){onClose();});
    }
    
    void Window::RemoveOnCloseEventListener(int index)
    {
        if(!IsEventCallbackExist(ssGUI::EventCallbacks::OnWindowCloseEventCallback::EVENT_NAME))
            return;
        
        GetEventCallback(ssGUI::EventCallbacks::OnWindowCloseEventCallback::EVENT_NAME)->RemoveEventListener(index);   
    }

    ssGUI::Enums::GUIObjectType Window::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::WINDOW;
    }

    void Window::Delete()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
        ssGUI::BaseGUIObject::Delete();
    }

    void Window::Internal_Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {
        FUNC_DEBUG_ENTRY();
        
        if(!IsVisible())
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        for(auto extension : ExtensionsDrawOrder)
            Extensions.at(extension)->Internal_Draw(true, drawingInterface, mainWindowP, mainWindowPositionOffset);

        // std::cout<<"local pos: "<<GetPosition().x<<", "<<GetPosition().y<<"\n";
        // std::cout<<"rendering pos: "<<GetGlobalPosition().x<<", "<<GetGlobalPosition().y<<"\n";
        // std::cout<<"rendering size: "<<GetSize().x<<", "<<GetSize().y<<"\n";

        glm::ivec2 drawPosition = GetGlobalPosition();

        //TODO: Some optimisation maybe possible

        //Base window
        DrawingVerticies.push_back(drawPosition);
        DrawingUVs.push_back(glm::ivec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingVerticies.push_back(drawPosition + glm::ivec2(GetSize().x, 0));
        DrawingUVs.push_back(glm::ivec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingVerticies.push_back(drawPosition + glm::ivec2(GetSize().x, GetSize().y));
        DrawingUVs.push_back(glm::ivec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingVerticies.push_back(drawPosition + glm::ivec2(0, GetSize().y));
        DrawingUVs.push_back(glm::ivec2());        //TODO : Caching
        DrawingColours.push_back(GetBackgroundColor());

        DrawingCounts.push_back(4);
        DrawingProperties.push_back(ssGUI::DrawingProperty());

        //Title bar 
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
        DrawingUVs.push_back(glm::ivec2());
        DrawingColours.push_back(titlebarColor);

        DrawingVerticies.push_back(drawPosition + glm::ivec2(GetSize().x, 0));
        DrawingUVs.push_back(glm::ivec2());
        DrawingColours.push_back(titlebarColor);

        DrawingVerticies.push_back(drawPosition + glm::ivec2(GetSize().x, TitlebarHeight));
        DrawingUVs.push_back(glm::ivec2());
        DrawingColours.push_back(titlebarColor);

        DrawingVerticies.push_back(drawPosition + glm::ivec2(0, TitlebarHeight));
        DrawingUVs.push_back(glm::ivec2());
        DrawingColours.push_back(titlebarColor);

        DrawingCounts.push_back(4);
        DrawingProperties.push_back(ssGUI::DrawingProperty());
        
        // std::cout<<"drawPosition: "<<drawPosition.x<<", "<<drawPosition.y<<"\n";

        for(auto extension : ExtensionsDrawOrder)
            Extensions.at(extension)->Internal_Draw(false, drawingInterface, mainWindowP, mainWindowPositionOffset);

        drawingInterface->DrawEntities(DrawingVerticies, DrawingUVs, DrawingColours, DrawingCounts, DrawingProperties);
        DrawingVerticies.clear();
        DrawingUVs.clear();
        DrawingColours.clear();
        DrawingCounts.clear();
        DrawingProperties.clear();

        FUNC_DEBUG_EXIT();
    }

    void Window::Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {        
        FUNC_DEBUG_ENTRY();
        
        //If it is not visible, don't even update/draw it
        if(!IsVisible())
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        for(auto extension : ExtensionsUpdateOrder)
            Extensions.at(extension)->Internal_Update(true, inputInterface, globalInputStatus, windowInputStatus, mainWindow);
        
        glm::ivec2 currentMousePos = inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow));
        glm::ivec2 mouseDelta = currentMousePos - MouseDownPosition;

        // std::cout << "current mouse pos: "<<currentMousePos.x <<", "<<currentMousePos.y<<"\n";
        // std::cout << "current window pos: "<<GetGlobalPosition().x<<", "<<GetGlobalPosition().y<<"\n";
        // std::cout << "current window size: "<<GetSize().x<<", "<<GetSize().y<<"\n";
        // std::cout<<"\n";
        
        //Resize
        //On mouse down
        if(inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) && !inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::LEFT) &&
            !globalInputStatus.MouseInputBlocked && !windowInputStatus.MouseInputBlocked)
        {
            OnMouseDownUpdate(currentMousePos, globalInputStatus);
        }
        //When the user is resizing or dragging the window
        else if(inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) && (ResizingLeft || ResizingRight || ResizingTop || ResizingBot || Dragging))
        {
            OnMouseDragOrResizeUpdate(globalInputStatus, mouseDelta, inputInterface);
        }
        //Otherwise show resize cursor if necessary 
        else if(!globalInputStatus.MouseInputBlocked && !windowInputStatus.MouseInputBlocked)
        {
            BlockMouseInputAndUpdateCursor(globalInputStatus, currentMousePos, inputInterface);
        }

        for(auto extension : ExtensionsUpdateOrder)
            Extensions.at(extension)->Internal_Update(false, inputInterface, globalInputStatus, windowInputStatus, mainWindow);

        FUNC_DEBUG_EXIT();
    }

    GUIObject* Window::Clone(bool cloneChildren)
    {
        FUNC_DEBUG_ENTRY();
        Window* temp = new Window(*this);
        CloneExtensionsAndEventCallbacks(temp);   
        
        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
            {
                FUNC_DEBUG_EXIT();
                return nullptr;
            }
        }

        FUNC_DEBUG_EXIT();
        return temp;
    }
}