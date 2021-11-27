#include "ssGUI/BaseClasses/Window.hpp"

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
    
    
    Window::Window() : Titlebar(true), TitlebarHeight(20), ResizeType(ssGUI::Enums::ResizeType::ALL), Draggable(true), Closable(true), Closed(false),
                       IsClosingAborted(false), CurrentDragState(ssGUI::Enums::WindowDragState::NONE), ResizeHitbox(5), ResizingTop(false), ResizingBot(false), 
                       ResizingLeft(false), ResizingRight(false), Dragging(false), OnTransformBeginPosition(), OnTransformBeginSize(), MouseDownPosition()
    {
        AddEventCallback(new ssGUI::EventCallbacks::WindowCloseEventCallback());
        AddExtension(new ssGUI::Extensions::Border());
    }

    Window::~Window()
    {}

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

        if(IsEventCallbackExist(ssGUI::EventCallbacks::WindowCloseEventCallback::EVENT_NAME))
        {
            GetEventCallback(ssGUI::EventCallbacks::WindowCloseEventCallback::EVENT_NAME)->Notify(this);
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

        if(IsEventCallbackExist(ssGUI::EventCallbacks::WindowCloseEventCallback::EVENT_NAME))
        {
            GetEventCallback(ssGUI::EventCallbacks::WindowCloseEventCallback::EVENT_NAME)->Notify(this);
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
        if(!IsEventCallbackExist(ssGUI::EventCallbacks::WindowCloseEventCallback::EVENT_NAME))
            AddEventCallback(new ssGUI::EventCallbacks::WindowCloseEventCallback());
        
        return GetEventCallback(ssGUI::EventCallbacks::WindowCloseEventCallback::EVENT_NAME)->AddEventListener([onClose](ssGUI::GUIObject* obj){onClose();});
    }
    
    void Window::RemoveOnCloseEventListener(int index)
    {
        if(!IsEventCallbackExist(ssGUI::EventCallbacks::WindowCloseEventCallback::EVENT_NAME))
            return;
        
        GetEventCallback(ssGUI::EventCallbacks::WindowCloseEventCallback::EVENT_NAME)->RemoveEventListener(index);   
    }

    ssGUI::Enums::GUIObjectType Window::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::WINDOW;
    }


    void Window::Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {
        if(!IsVisible())
            return;

        for(auto extension : Extensions)
            extension.second->Draw(true, drawingInterface, mainWindowP, mainWindowPositionOffset);

        // std::cout<<"local pos: "<<GetPosition().x<<", "<<GetPosition().y<<"\n";
        // std::cout<<"rendering pos: "<<GetGlobalPosition().x<<", "<<GetGlobalPosition().y<<"\n";
        // std::cout<<"rendering size: "<<GetSize().x<<", "<<GetSize().y<<"\n";

        glm::ivec2 drawPosition = GetGlobalPosition();

        //TODO: Some optimisation maybe possible

        //Base window
        DrawingVerticies.push_back(drawPosition);
        DrawingUVs.push_back(glm::ivec2());
        DrawingColours.push_back(GetBackgroundColour());

        DrawingVerticies.push_back(drawPosition + glm::ivec2(GetSize().x, 0));
        DrawingUVs.push_back(glm::ivec2());
        DrawingColours.push_back(GetBackgroundColour());

        DrawingVerticies.push_back(drawPosition + glm::ivec2(GetSize().x, GetSize().y));
        DrawingUVs.push_back(glm::ivec2());
        DrawingColours.push_back(GetBackgroundColour());

        DrawingVerticies.push_back(drawPosition + glm::ivec2(0, GetSize().y));
        DrawingUVs.push_back(glm::ivec2());        //TODO : Caching
        DrawingColours.push_back(GetBackgroundColour());

        DrawingCounts.push_back(4);
        DrawingProperties.push_back(ssGUI::DrawingProperty());

        //Title bar 
        int darken = 40;
        glm::u8vec4 titlebarColor = GetBackgroundColour() - glm::u8vec4(darken, darken, darken, 0);

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

        for(auto extension : Extensions)
            extension.second->Draw(false, drawingInterface, mainWindowP, mainWindowPositionOffset);

        drawingInterface->DrawEntities(DrawingVerticies, DrawingUVs, DrawingColours, DrawingCounts, DrawingProperties);
        DrawingVerticies.clear();
        DrawingUVs.clear();
        DrawingColours.clear();
        DrawingCounts.clear();
        DrawingProperties.clear();
    }

    void Window::Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {        
        //If it is not visible, don't even update/draw it
        if(!IsVisible())
            return;
        
        for(auto extension : Extensions)
            extension.second->Update(true, inputInterface, globalInputStatus, windowInputStatus, mainWindow);
        
        {
        glm::ivec2 currentMousePos = inputInterface->GetCurrentMousePosition(mainWindow);

        // std::cout << "current mouse pos: "<<currentMousePos.x <<", "<<currentMousePos.y<<"\n";
        // std::cout << "current window pos: "<<GetGlobalPosition().x<<", "<<GetGlobalPosition().y<<"\n";
        // std::cout << "current window size: "<<GetSize().x<<", "<<GetSize().y<<"\n";
        // std::cout<<"\n";

        glm::ivec2 mouseDelta = currentMousePos - MouseDownPosition;
        
        //Resize
        //On mouse down
        if(inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) && !inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::LEFT) &&
            !globalInputStatus.MouseInputBlocked)
        {
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
                goto endUpdate;
            }
                
            if( currentMousePos.x >= GetGlobalPosition().x && currentMousePos.x <= GetGlobalPosition().x + GetSize().x && 
                currentMousePos.y >= GetGlobalPosition().y && currentMousePos.y <= GetGlobalPosition().y + TitlebarHeight &&
                IsDraggable())
            {
                Dragging = true;
                globalInputStatus.MouseInputBlocked = true;
                SetWindowDragState(ssGUI::Enums::WindowDragState::STARTED);

                goto endUpdate;
            }

            //Input blocking
            if( currentMousePos.x >= GetGlobalPosition().x && currentMousePos.x <= GetGlobalPosition().x + GetSize().x && 
                currentMousePos.y >= GetGlobalPosition().y && currentMousePos.y <= GetGlobalPosition().y + GetSize().y)
            {
                globalInputStatus.MouseInputBlocked = true;   
            }
        }
        //When the user is resizing or dragging the window
        else if(inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) && (ResizingLeft || ResizingRight || ResizingTop || ResizingBot || Dragging))
        {
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
        }
        //Otherwise show resize cursor if necessary 
        else
        {
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
        }
        }   //Extra bracket to tell compiler that the variables are not created if endUpdate label is used

        endUpdate:
            for(auto extension : Extensions)
                extension.second->Update(false, inputInterface, globalInputStatus, windowInputStatus, mainWindow);
    }

    GUIObject* Window::Clone(std::vector<GUIObject*>& originalObjs, bool cloneChildren)
    {
        Window* temp = new Window(*this);

        for(auto extension : Extensions)
        {
            if(!temp->IsExtensionExist(extension.second->GetExtensionName()))
                temp->AddExtension(extension.second->Clone(this));
        }

        for(auto eventCallback : EventCallbacks)
        {
            std::vector<ssGUI::GUIObject*> tempVec = std::vector<ssGUI::GUIObject*>();

            if(!temp->IsEventCallbackExist(eventCallback.second->GetEventCallbackName()))
                temp->AddEventCallback(eventCallback.second->Clone(this, originalObjs, tempVec));
        }

        return temp;
    }
}