#include "ssGUI/GUIObjectClasses/MainWindow.hpp"

#include "ssGUI/EventCallbacks/FocusedEventCallback.hpp"
#include "ssGUI/EventCallbacks/FocusLostEventCallback.hpp"
#include "ssGUI/EventCallbacks/SizeChangedEventCallback.hpp"

namespace ssGUI
{
    MainWindow::MainWindow(MainWindow const& other) : Window(other)
    {
        BackendMainWindow = other.BackendMainWindow->Clone();
        BackendDrawing = ssGUI::Backend::BackendFactory::CreateBackendDrawingInterface();
        LastSize = other.LastSize;
        RedrawCount = other.RedrawCount;
        LastSyncTime = other.LastSyncTime;
        
        BackendMainWindow->AddOnCloseEvent(std::bind(&ssGUI::MainWindow::Internal_OnClose, this));
    }

    void MainWindow::MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                                ssGUI::GUIObject* mainWindow)
    {}
    
    MainWindow::MainWindow() :  BackendMainWindow(nullptr),
                                BackendDrawing(nullptr),
                                LastSize(),
                                RedrawCount(0),
                                LastSyncTime(0)
    {
        BackendMainWindow = ssGUI::Backend::BackendFactory::CreateBackendMainWindowInterface();
        BackendDrawing = ssGUI::Backend::BackendFactory::CreateBackendDrawingInterface();
        BackendMainWindow->AddOnCloseEvent(std::bind(&ssGUI::MainWindow::Internal_OnClose, this));
        BackendMainWindow->AddFocusChangedByUserEvent(std::bind(&ssGUI::MainWindow::Internal_FocusChanged, this, std::placeholders::_1));
        BackendMainWindow->SetMSAA(8);
        SetBackgroundColor(glm::u8vec4(255, 255, 255, 255));
    }

    MainWindow::~MainWindow()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
        delete BackendMainWindow;
        delete BackendDrawing;
    }
    
    void MainWindow::Render()
    {
        BackendDrawing->Render(GetBackgroundColor());
        Redraw = false;
    }

    void MainWindow::ClearBackBuffer()
    {
        BackendDrawing->ClearBackBuffer(GetBackgroundColor());
    }

    ssGUI::Backend::BackendMainWindowInterface* MainWindow::GetBackendWindowInterface()
    {
        return BackendMainWindow;
    }

    ssGUI::Backend::BackendDrawingInterface* MainWindow::GetBackendDrawingInterface()
    {
        return BackendDrawing;
    }
    
    glm::ivec2 MainWindow::GetDisplayPosition() const
    {
        return BackendMainWindow->GetPosition();
    }

    void MainWindow::SetDisplayPosition(glm::ivec2 pos)
    {
        BackendMainWindow->SetPosition(pos);
    }

    glm::ivec2 MainWindow::GetPositionOffset() const
    {
        return BackendMainWindow->GetPositionOffset();
    }

    void MainWindow::SetTitle(std::wstring title)
    {
        BackendMainWindow->SetTitle(title);
    }

    std::wstring MainWindow::GetTitle() const
    {
        return BackendMainWindow->GetTitle();
    }

    void MainWindow::SetIcon(ssGUI::ImageData& iconImage)
    {
        BackendMainWindow->SetIcon(*(iconImage.GetBackendImageInterface()));
    }

    void MainWindow::SetVSync(bool vSync)
    {
        BackendMainWindow->SetVSync(vSync);
    }

    bool MainWindow::IsVSync() const
    {
        return BackendMainWindow->IsVSync();
    }

    bool MainWindow::IsFocused() const
    {
        return BackendMainWindow->IsFocused();
    }
    
    void MainWindow::SetFocus(bool focus)
    {
        bool originalFocus = IsFocused();
        
        BackendMainWindow->SetFocus(focus, false);
    }

    void MainWindow::SetMSAA(int level)
    {
        BackendMainWindow->SetMSAA(level);
    }

    int MainWindow::GetMSAA() const
    {
        return BackendMainWindow->GetMSAA();
    }

    void MainWindow::SetWindowMode(ssGUI::Enums::WindowMode WindowMode)
    {
        BackendMainWindow->SetWindowMode(WindowMode);
    }

    ssGUI::Enums::WindowMode MainWindow::GetWindowMode() const
    {
        return BackendMainWindow->GetWindowMode();
    }

    void MainWindow::Internal_FocusChanged(bool focused)
    {
        if(focused)
            RedrawObject();
        
        if(focused && IsAnyEventCallbackExist<ssGUI::EventCallbacks::FocusedEventCallback>())
            GetAnyEventCallback<ssGUI::EventCallbacks::FocusedEventCallback>()->Notify(this);
        else if(!focused && IsAnyEventCallbackExist<ssGUI::EventCallbacks::FocusLostEventCallback>())
            GetAnyEventCallback<ssGUI::EventCallbacks::FocusLostEventCallback>()->Notify(this);
    }

    void MainWindow::SetVisible(bool visible)
    {
        BackendMainWindow->SetVisible(visible);
    }

    bool MainWindow::IsVisible() const
    {
        return BackendMainWindow->IsVisible();
    }

    void MainWindow::SetEnabled(bool enabled)
    {
        BackendMainWindow->SetVisible(enabled);
    }

    bool MainWindow::IsEnabled() const
    {
        return BackendMainWindow->IsVisible();
    }

    bool MainWindow::IsSelfEnabled() const
    {
        return BackendMainWindow->IsVisible();
    }

    glm::vec2 MainWindow::GetPosition() const
    {
        return glm::vec2(0, 0);// BackendMainWindow->GetPosition();
    }
    void MainWindow::SetPosition(glm::vec2 position)
    {
        //BackendMainWindow->SetPosition(position);
    }

    glm::vec2 MainWindow::GetGlobalPosition()
    {
        return glm::vec2(0, 0);//return BackendMainWindow->GetPosition();
    }
    void MainWindow::SetGlobalPosition(glm::vec2 position)
    {
        //BackendMainWindow->SetPosition(position);
    }

    glm::vec2 MainWindow::GetSize() const
    {
        return BackendMainWindow->GetSize();
    }
    void MainWindow::SetSize(glm::vec2 size)
    {
        size.x = size.x > GetMaxSize().x ? GetMaxSize().x : size.x;
        size.y = size.y > GetMaxSize().y ? GetMaxSize().y : size.y;
        size.x = size.x < GetMinSize().x ? GetMinSize().x : size.x;
        size.y = size.y < GetMinSize().y ? GetMinSize().y : size.y;
        
        BackendMainWindow->SetSize(size);
        RedrawObject();

        //Size changed event callback is done in update to allow user resizing to be captured as well
    }

    ssGUI::Enums::GUIObjectType MainWindow::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::MAIN_WINDOW | ssGUI::Enums::GUIObjectType::WINDOW;
    }

    void MainWindow::Close()
    {
        //Forwarding signal to backend
        BackendMainWindow->Close();
    }

    void MainWindow::AbortClosing()
    {
        //Tell window to stop closing
        Window::AbortClosing();
        
        //Tell backend to stop closing
        BackendMainWindow->AbortClosing();
    } 

    void MainWindow::Internal_OnClose()
    {
        ssLOG_LINE("Main Window closing");
        
        //Forwarding signal to window
        Window::Internal_OnClose();
    }

    void MainWindow::Internal_Draw()
    {
        Internal_Draw(BackendDrawing, this, glm::vec2());
    }

    //TODO : Refactor this, merge it to sync in update function
    void MainWindow::Internal_Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {       
        DisableRedrawObjectRequest();
        
        // if(Redraw)
        // {
            for(auto extension : ExtensionsDrawOrder)
                Extensions.at(extension)->Internal_Draw(true, drawingInterface, mainWindow, mainWindowPositionOffset);
            
            //TODO: Move these to its own set get functions
            //Settings that require window to be relaunched -----------------------------------------
            if(BackendMainWindow->HasTitlebar() != HasTitlebar())
                BackendMainWindow->SetTitlebar(HasTitlebar());

            bool isResizable = GetResizeType() == ssGUI::Enums::ResizeType::ALL;

            if(BackendMainWindow->IsResizable() != isResizable)
                BackendMainWindow->SetResizable(isResizable);

            if(BackendMainWindow->HasCloseButton() != IsClosable())
                BackendMainWindow->SetCloseButton(IsClosable());

            
            //Realtime settings -----------------------------------------

            if(BackendMainWindow->IsVisible() != IsVisible())
                BackendMainWindow->SetVisible(IsVisible());

            //TEST
            /*
            std::vector<glm::vec2> cord;
            std::vector<glm::u8vec4> color;

            
            cord.push_back(glm::vec2(1, 0));
            cord.push_back(glm::vec2(GetSize().x - 1, 0));
            cord.push_back(glm::vec2(GetSize().x - 1, 20));
            cord.push_back(glm::vec2(1, 20));

            color.push_back(glm::u8vec4(0, 0, 0, 255));
            color.push_back(glm::u8vec4(0, 0, 0, 255));
            color.push_back(glm::u8vec4(0, 0, 0, 255));
            color.push_back(glm::u8vec4(0, 0, 0, 255));

            drawingInterface.DrawShape(cord, color);
            */

            UpdateGUIObjectVertexAndShapeIndex();

            for(auto extension : ExtensionsDrawOrder)
                Extensions.at(extension)->Internal_Draw(false, drawingInterface, mainWindow, mainWindowPositionOffset);

            EnableRedrawObjectRequest();

        //     Redraw = false;
        // }
    }

    //TODO : Add WindowDragStateChangedEvent call
    void MainWindow::Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, ssGUI::GUIObject* mainWindow)
    {
        ssLOG_FUNC_ENTRY();
        
        for(auto extension : ExtensionsUpdateOrder)
        {
            //Guard against extension being deleted by other extensions
            if(!IsExtensionExist(extension))
                continue;

            Extensions.at(extension)->Internal_Update(true, inputInterface, inputStatus, mainWindow);
        }

        //Update cursor position offset every .5 seconds
        if(inputInterface->GetElapsedTime() - LastSyncTime > 500)
        {
            BackendMainWindow->SyncPositionOffset();
            LastSyncTime = inputInterface->GetElapsedTime();

            glm::ivec2 currentSize = BackendMainWindow->GetSize();
            
            if(currentSize.x < GetMinSize().x)
                currentSize.x = GetMinSize().x;        

            if(currentSize.y < GetMinSize().y)
                currentSize.y = GetMinSize().y;

            if(currentSize.x > GetMaxSize().x)
                currentSize.x = GetMaxSize().x;

            if(currentSize.y > GetMaxSize().x)
                currentSize.y = GetMaxSize().x;

            if(currentSize != BackendMainWindow->GetSize())
                BackendMainWindow->SetSize(currentSize);
        }

        //Check size different for redraw
        if(GetSize() != LastSize)
        {
            RedrawObject();

            if(IsEventCallbackExist(ssGUI::EventCallbacks::SizeChangedEventCallback::EVENT_NAME))
                GetEventCallback(ssGUI::EventCallbacks::SizeChangedEventCallback::EVENT_NAME)->Notify(this);

            RedrawCount = 0;
        }
        //Seems like a bug but it needs redrawing after changing the size. It might be device independent.
        //Redraw up to 20 frames even when the size is the same to make sure it is properly rendered
        else if(RedrawCount < 20)
        {
            RedrawObject();
            RedrawCount++;
        }

        LastSize = GetSize();

        CheckRightClickMenu(inputInterface, inputStatus, mainWindow);
        MainLogic(inputInterface, inputStatus, mainWindow);

        //Apply focus
        if(inputStatus.MouseInputBlockedObject == nullptr)
        {
            glm::ivec2 currentMousePos = inputInterface->GetCurrentMousePosition(this);

            bool mouseInWindowBoundX = false;
            bool mouseInWindowBoundY = false;
            
            if(currentMousePos.x >= GetGlobalPosition().x && currentMousePos.x <= GetGlobalPosition().x + GetSize().x)
                mouseInWindowBoundX = true;

            if(currentMousePos.y >= GetGlobalPosition().y && currentMousePos.y <= GetGlobalPosition().y + GetSize().y)
                mouseInWindowBoundY = true;
            
            //Input blocking
            if(mouseInWindowBoundX && mouseInWindowBoundY)
                inputStatus.MouseInputBlockedObject = this;

            //If mouse click on this, set focus
            if(mouseInWindowBoundX && mouseInWindowBoundY &&
                ((inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) && !inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::LEFT)) ||
                (inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::MIDDLE) && !inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::MIDDLE)) ||
                (inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::RIGHT) && !inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::RIGHT))))
            {
                GUIObject::SetFocus(true);
            }
        }

        // glm::ivec2 currentMousePos = inputInterface.GetCurrentMousePosition();
        // std::cout << "current mouse pos: "<<currentMousePos.x <<", "<<currentMousePos.y<<"\n";
        // std::cout << "current window pos: "<<GetGlobalPosition().x<<", "<<GetGlobalPosition().y<<"\n";
        // std::cout << "current window size: "<<GetSize().x<<", "<<GetSize().y<<"\n";
        // std::cout << "window offset: "<<GetPositionOffset().x << ", "<<GetPositionOffset().y<<"\n";
        // std::cout<<"\n";

        for(auto extension : ExtensionsUpdateOrder)
        {
            //Guard against extension being deleted by other extensions
            if(!IsExtensionExist(extension))
                continue;

            Extensions.at(extension)->Internal_Update(false, inputInterface, inputStatus, mainWindow);
        }

        ssLOG_FUNC_EXIT();
    }

    MainWindow* MainWindow::Clone(bool cloneChildren)
    {
        ssLOG_FUNC_ENTRY();
        MainWindow* temp = new MainWindow(*this);
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