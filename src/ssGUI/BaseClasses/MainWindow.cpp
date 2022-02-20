#include "ssGUI/BaseClasses/MainWindow.hpp"


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
    
    MainWindow::MainWindow() : BackendMainWindow(), BackendDrawing(), LastSize(glm::ivec2(0, 0)), RedrawCount(0), LastSyncTime(0)
    {
        BackendMainWindow = ssGUI::Backend::BackendFactory::CreateBackendMainWindowInterface();
        BackendDrawing = ssGUI::Backend::BackendFactory::CreateBackendDrawingInterface();
        
        BackendMainWindow->AddOnCloseEvent(std::bind(&ssGUI::MainWindow::Internal_OnClose, this));
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
        Internal_Draw(BackendDrawing, this, glm::ivec2());
    }

    //TODO : Refactor this, merge it to sync in update function
    void MainWindow::Internal_Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {       
        DisableRedrawObjectRequest();
        
        // if(Redraw)
        // {
            for(auto extension : ExtensionsDrawOrder)
                Extensions.at(extension)->Internal_Draw(true, drawingInterface, mainWindowP, mainWindowPositionOffset);
            
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
            std::vector<glm::ivec2> cord;
            std::vector<glm::u8vec4> color;

            
            cord.push_back(glm::ivec2(1, 0));
            cord.push_back(glm::ivec2(GetSize().x - 1, 0));
            cord.push_back(glm::ivec2(GetSize().x - 1, 20));
            cord.push_back(glm::ivec2(1, 20));

            color.push_back(glm::u8vec4(0, 0, 0, 255));
            color.push_back(glm::u8vec4(0, 0, 0, 255));
            color.push_back(glm::u8vec4(0, 0, 0, 255));
            color.push_back(glm::u8vec4(0, 0, 0, 255));

            drawingInterface.DrawShape(cord, color);
            */

            //TODO : Add backend drawing here

            for(auto extension : ExtensionsDrawOrder)
                Extensions.at(extension)->Internal_Draw(false, drawingInterface, mainWindowP, mainWindowPositionOffset);

            EnableRedrawObjectRequest();

        //     Redraw = false;
        // }
    }   

    glm::ivec2 MainWindow::GetPosition() const
    {
        return glm::ivec2(0, 0);// BackendMainWindow->GetPosition();
    }
    void MainWindow::SetPosition(glm::ivec2 position)
    {
        //BackendMainWindow->SetPosition(position);
    }

    glm::ivec2 MainWindow::GetGlobalPosition()
    {
        return glm::ivec2(0, 0);//return BackendMainWindow->GetPosition();
    }
    void MainWindow::SetGlobalPosition(glm::ivec2 position)
    {
        //BackendMainWindow->SetPosition(position);
    }

    glm::ivec2 MainWindow::GetSize() const
    {
        return BackendMainWindow->GetSize();
    }
    void MainWindow::SetSize(glm::ivec2 size)
    {
        size.x = size.x > GetMaxSize().x ? GetMaxSize().x : size.x;
        size.y = size.y > GetMaxSize().y ? GetMaxSize().y : size.y;
        size.x = size.x < GetMinSize().x ? GetMinSize().x : size.x;
        size.y = size.y < GetMinSize().y ? GetMinSize().y : size.y;
        
        BackendMainWindow->SetSize(size);
        RedrawObject();
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

    void MainWindow::Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();
        
        for(auto extension : ExtensionsUpdateOrder)
            Extensions.at(extension)->Internal_Update(true, inputInterface, globalInputStatus, windowInputStatus, mainWindow);
            

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
            RedrawCount = 0;
        }
        //Redraw up to 3 frames even when the size is the same to make sure it is properly rendered
        else if(RedrawCount < 3)
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
            Extensions.at(extension)->Internal_Update(false, inputInterface, globalInputStatus, windowInputStatus, mainWindow);

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