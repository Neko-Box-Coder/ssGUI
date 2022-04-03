#include "ssGUI/GUIObjectClasses/MainWindow.hpp"


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
    
    MainWindow::MainWindow() : BackendMainWindow(), BackendDrawing(), LastSize(glm::vec2(0, 0)), RedrawCount(0), LastSyncTime(0)
    {
        BackendMainWindow = ssGUI::Backend::BackendFactory::CreateBackendMainWindowInterface();
        BackendDrawing = ssGUI::Backend::BackendFactory::CreateBackendDrawingInterface();
        BackendMainWindow->AddOnCloseEvent(std::bind(&ssGUI::MainWindow::Internal_OnClose, this));
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
    
    bool MainWindow::IsFocused() const
    {
        return BackendMainWindow->IsFocused();
    }
    
    void MainWindow::SetFocus(bool focus)
    {
        BackendMainWindow->SetFocus(focus);
    }

    glm::ivec2 MainWindow::GetPositionOffset() const
    {
        return BackendMainWindow->GetPositionOffset();
    }

    glm::ivec2 MainWindow::GetDisplayPosition() const
    {
        return BackendMainWindow->GetPosition();
    }

    void MainWindow::SetDisplayPosition(glm::ivec2 pos)
    {
        BackendMainWindow->SetPosition(pos);
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

            //TODO : Add backend drawing here

            for(auto extension : ExtensionsDrawOrder)
                Extensions.at(extension)->Internal_Draw(false, drawingInterface, mainWindow, mainWindowPositionOffset);

            EnableRedrawObjectRequest();

        //     Redraw = false;
        // }
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

    void MainWindow::Delete()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
        ssGUI::BaseGUIObject::Delete();
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
        //Forwarding signal to window
        Window::Internal_OnClose();
    }

    //TODO : Add WindowDragStateChangedEvent call
    void MainWindow::Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();
        
        for(auto extension : ExtensionsUpdateOrder)
        {
            //Guard against extension being deleted by other extensions
            if(!IsExtensionExist(extension))
                continue;

            Extensions.at(extension)->Internal_Update(true, inputInterface, globalInputStatus, windowInputStatus, mainWindow);
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

            Extensions.at(extension)->Internal_Update(false, inputInterface, globalInputStatus, windowInputStatus, mainWindow);
        }

        FUNC_DEBUG_EXIT();
    }

    GUIObject* MainWindow::Clone(bool cloneChildren)
    {
        FUNC_DEBUG_ENTRY();
        MainWindow* temp = new MainWindow(*this);
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