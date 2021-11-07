#include "ssGUI/BaseClasses/Window.hpp"

namespace ssGUI
{
    Window::Window() : Titlebar(true), TitlebarHeight(20), ResizeType(ssGUI::Enums::ResizeType::ALL), Closable(true), Closed(false),
                       IsClosingAborted(false), ResizeHitbox(5), IsResizingTop(false), IsResizingBot(false), IsResizingLeft(false), 
                       IsResizingRight(false), IsDragging(false), OnTransformBeginPosition(), OnTransformBeginSize(), MouseDownOffset(), 
                       MouseDownPosition()
    {
        AddEventCallback(new ssGUI::EventCallbacks::WindowCloseEventCallback());
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

        SetParentP(nullptr);
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
        return TitlebarHeight;
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
            extension->Draw(true, drawingInterface, mainWindowP, mainWindowPositionOffset);

        //std::cout<<"Drawed: "<<GetPositon().x<<", "<<GetPositon().y<<"\n";
        //std::cout<<"rendering pos: "<<GetGlobalPositon().y<<"\n";
        //std::cout<<"rendering size: "<<GetSize().y<<"\n";


        glm::ivec2 drawPosition = GetGlobalPosition() - mainWindowP->GetGlobalPosition() - mainWindowPositionOffset;

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
        DrawingUVs.push_back(glm::ivec2());
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
        
        

        for(auto extension : Extensions)
            extension->Draw(false, drawingInterface, mainWindowP, mainWindowPositionOffset);

        drawingInterface->DrawEntities(DrawingVerticies, DrawingUVs, DrawingColours, DrawingCounts, DrawingProperties);
        DrawingVerticies.clear();
        DrawingUVs.clear();
        DrawingColours.clear();
        DrawingCounts.clear();
        DrawingProperties.clear();
    }

    void Window::Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus)
    {        
        //If it is not visible, don't even update/draw it
        if(!IsVisible())
            return;
        
        for(auto extension : Extensions)
            extension->Update(true, inputInterface, globalInputStatus, windowInputStatus);
        
        //Only block mouse input when the user is not resizing or dragging the window 
        if(!IsResizingLeft && !IsResizingRight && !IsResizingTop && !IsResizingBot && !IsDragging)
        {
            if(windowInputStatus.MouseInputBlocked || globalInputStatus.MouseInputBlocked)
                goto endUpdate;
        }
        
        {
        glm::ivec2 currentMousePos = inputInterface->GetCurrentMousePosition();

        // std::cout << "current mouse pos: "<<currentMousePos.x <<", "<<currentMousePos.y<<"\n";
        // std::cout << "current window pos: "<<GetGlobalPosition().x<<", "<<GetGlobalPosition().y<<"\n";
        // std::cout << "current window size: "<<GetSize().x<<", "<<GetSize().y<<"\n";
        // std::cout<<"\n";

        glm::ivec2 mouseDelta = currentMousePos - MouseDownPosition;
        
        //Resize
        //On mouse down
        if(inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) && !inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::LEFT))
        {
            MouseDownOffset = currentMousePos - GetGlobalPosition();
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
                        IsResizingTop = true;
                    }
                    else if(std::abs(currentMousePos.y - (GetGlobalPosition().y + GetSize().y)) <= ResizeHitbox && 
                        (GetResizeType() == ssGUI::Enums::ResizeType::ALL || GetResizeType() == ssGUI::Enums::ResizeType::BOTTOM))
                    {
                        IsResizingBot = true;                        
                    }
                }

                if(currentMousePos.y >= GetGlobalPosition().y - ResizeHitbox && currentMousePos.y <= GetGlobalPosition().y + GetSize().y + ResizeHitbox)
                {
                    if(std::abs(currentMousePos.x - GetGlobalPosition().x) <= ResizeHitbox && 
                        (GetResizeType() == ssGUI::Enums::ResizeType::ALL || GetResizeType() == ssGUI::Enums::ResizeType::LEFT))
                    {
                        IsResizingLeft = true;   
                    }
                    else if(std::abs(currentMousePos.x - (GetGlobalPosition().x + GetSize().x)) <= ResizeHitbox && 
                        (GetResizeType() == ssGUI::Enums::ResizeType::ALL || GetResizeType() == ssGUI::Enums::ResizeType::RIGHT))
                    {
                        IsResizingRight = true;
                    }
                }
            }
            
            //Titlebar Drag
            if(IsResizingBot || IsResizingLeft || IsResizingRight || IsResizingTop)
            {
                globalInputStatus.MouseInputBlocked = true;
                goto endUpdate;
            }
                
            if( currentMousePos.x >= GetGlobalPosition().x && currentMousePos.x <= GetGlobalPosition().x + GetSize().x && 
                currentMousePos.y >= GetGlobalPosition().y && currentMousePos.y <= GetGlobalPosition().y + TitlebarHeight)
            {
                IsDragging = true;
                globalInputStatus.MouseInputBlocked = true;
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
        else if(inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) && (IsResizingLeft || IsResizingRight || IsResizingTop || IsResizingBot || IsDragging))
        {
            //Resize
            glm::ivec2 newPos = OnTransformBeginPosition;
            glm::ivec2 newSize = OnTransformBeginSize;
            if(IsResizingTop)
            {
                newPos += glm::ivec2(0, mouseDelta.y);
                newSize -= glm::ivec2(0, mouseDelta.y);

                //Bound new pos to prevent moving the window when resizing reaches max or min
                if(newSize.y < GetMinSize().y)
                    newPos.y = OnTransformBeginPosition.y + (OnTransformBeginSize.y - GetMinSize().y);
                else if(newSize.y > GetMaxSize().y)
                    newPos.y = OnTransformBeginPosition.y - (GetMaxSize().y - OnTransformBeginSize.y);
                
                globalInputStatus.MouseInputBlocked = true;
            }
            else if(IsResizingBot)
            {
                newSize += glm::ivec2(0, mouseDelta.y);
                globalInputStatus.MouseInputBlocked = true;
            }
            
            if(IsResizingLeft)
            {
                newPos += glm::ivec2(mouseDelta.x, 0);
                newSize -= glm::ivec2(mouseDelta.x, 0);

                //Bound new pos to prevent moving the window when resizing reaches max or min
                if(newSize.x < GetMinSize().x)
                    newPos.x = OnTransformBeginPosition.x + (OnTransformBeginSize.x - GetMinSize().x);
                else if(newSize.x > GetMaxSize().x)
                    newPos.x = OnTransformBeginPosition.x - (GetMaxSize().x - OnTransformBeginSize.x);

                globalInputStatus.MouseInputBlocked = true;
            }
            else if(IsResizingRight)
            {
                newSize += glm::ivec2(mouseDelta.x, 0);
                globalInputStatus.MouseInputBlocked = true;
            }
                
            //Titlebar Drag
            if(IsDragging)
            {
                globalInputStatus.MouseInputBlocked = true;
                newPos += mouseDelta;
                
            }

            SetSize(newSize);
            SetGlobalPosition(newPos);

            //Updating cursor
            if(IsResizingTop && IsResizingLeft)
                inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_TOP_LEFT);
            else if(IsResizingTop && IsResizingRight)
                inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_TOP_RIGHT);
            else if(IsResizingBot && IsResizingLeft)
                inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_BOTTOM_LEFT);
            else if(IsResizingBot && IsResizingRight)
                inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_BOTTOM_RIGHT);
            else if(IsResizingTop)
                inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_UP);
            else if(IsResizingLeft)
                inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_LEFT);
            else if(IsResizingBot)
                inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_DOWN);
            else if(IsResizingRight)
                inputInterface->SetCursorType(ssGUI::Enums::CursorType::RESIZE_RIGHT);
        }
        //Otherwise
        else
        {
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

            IsResizingTop = false;
            IsResizingBot = false;
            IsResizingLeft = false;
            IsResizingRight = false;
            IsDragging = false;
            OnTransformBeginSize = glm::ivec2();
            OnTransformBeginPosition = glm::ivec2();
            MouseDownOffset = glm::ivec2();
            MouseDownPosition = glm::ivec2();
        }
        }   //Extra bracket to tell compiler that the variables are not created if endUpdate label is used

        endUpdate:
            for(auto extension : Extensions)
                extension->Update(false, inputInterface, globalInputStatus, windowInputStatus);
    }

    GUIObject* Window::Clone(std::vector<GUIObject*>& originalObjs, bool cloneChildren)
    {
        Window* temp = new Window(*this);

        for(auto extension : Extensions)
        {
            if(!temp->IsExtensionExist(extension->GetExtensionName()))
                temp->AddExtension(extension->Clone(this));
        }

        for(auto eventCallback : EventCallbacks)
        {
            std::vector<ssGUI::GUIObject*> tempVec = std::vector<ssGUI::GUIObject*>();

            if(!temp->IsEventCallbackExist(eventCallback->GetEventCallbackName()))
                temp->AddEventCallback(eventCallback->Clone(this, originalObjs, tempVec));
        }

        return temp;
    }
}