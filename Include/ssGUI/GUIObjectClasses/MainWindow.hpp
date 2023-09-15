#ifndef SSGUI_MAIN_WINDOW_H
#define SSGUI_MAIN_WINDOW_H

#include "ssGUI/GUIObjectClasses/Window.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"
#include <functional>

//namespace: ssGUI
namespace ssGUI
{
    /*class: ssGUI::MainWindow
    Main Window is the *actual* window. Not to be confused with <ssGUI::Window> which lives inside MainWindow.
    You should call <Renderer::RedrawObject> after changing the properties of the MainWindow.
    This class should not be overwritten unless you know what you are doing
    
    Variables & Constructor:
    ============================== C++ ==============================
    private:
        ssGUI::Backend::BackendMainWindowInterface* BackendMainWindow;      //See <GetBackendWindowInterface>
        ssGUI::Backend::BackendDrawingInterface* BackendDrawing;            //See <GetBackendDrawingInterface>
        glm::vec2 LastSize;                                                 //(Internal variable) Used to trigger redraw and <SizeChangedEventCallback>
        int RedrawCount;                                                    //(Internal variable) Used to make sure the redraw is actually done

        uint64_t LastSyncTime;                                              //(Internal variable) Used to update the window every half a second
    =================================================================
    ============================== C++ ==============================
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
    =================================================================
    */
    class MainWindow : public Window
    {
        private:
            ssGUI::Backend::BackendMainWindowInterface* BackendMainWindow;      //See <GetBackendWindowInterface>
            ssGUI::Backend::BackendDrawingInterface* BackendDrawing;            //See <GetBackendDrawingInterface>
            glm::vec2 LastSize;                                                 //(Internal variable) Used to trigger redraw and <SizeChangedEventCallback>
            int RedrawCount;                                                    //(Internal variable) Used to make sure the redraw is actually done

            uint64_t LastSyncTime;                                              //(Internal variable) Used to update the window every half a second
            
            MainWindow& operator=(MainWindow const& other);

        protected:
            MainWindow(MainWindow const& other);

            virtual void MainLogic( ssGUI::Backend::BackendSystemInputInterface* inputInterface, 
                                    ssGUI::InputStatus& currentInputStatus, 
                                    ssGUI::InputStatus& lastInputStatus, 
                                    ssGUI::GUIObject* mainWindow) override;

        public:
            //MainWindow(MainWindow& mainWindow);
            MainWindow();
            virtual ~MainWindow() override;
            
            //function: Render
            //Renders all the entities drawn on this MainWindow. Called by <ssGUI::ssGUIManager> normally.
            virtual void Render();
            
            //function: ClearBackBuffer
            //Clears all the entities drawn on this MainWindow
            virtual void ClearBackBuffer();

            //function: GetBackendWindowInterface
            virtual ssGUI::Backend::BackendMainWindowInterface* GetBackendWindowInterface();
            
            //function: GetBackendDrawingInterface
            virtual ssGUI::Backend::BackendDrawingInterface* GetBackendDrawingInterface();
            
            //function: GetWindowPosition
            //See <ssGUI::Backend::BackendMainWindowInterface::GetWindowPosition>
            virtual glm::ivec2 GetWindowPosition() const;

            //function: SetWindowPosition
            //See <ssGUI::Backend::BackendMainWindowInterface::SetWindowPosition>
            virtual void SetWindowPosition(glm::ivec2 pos);

            //function: GetPositionOffset
            //See <ssGUI::Backend::BackendMainWindowInterface::GetPositionOffset>
            virtual glm::ivec2 GetPositionOffset() const;

            //function: SetTitle
            //See <ssGUI::Backend::BackendMainWindowInterface::IsVSync>
            virtual void SetTitle(std::wstring title);
            //function: GetTitle
            //See <ssGUI::Backend::BackendMainWindowInterface::IsVSync>
            virtual std::wstring GetTitle() const;

            //function: SetIcon
            //See <ssGUI::Backend::BackendMainWindowInterface::SetIcon>
            virtual void SetIcon(ssGUI::ImageData& iconImage);

            //function: SetVSync
            //See <ssGUI::Backend::BackendMainWindowInterface::SetVSync>
            virtual void SetVSync(bool vSync);
            
            //function: IsVSync
            //See <ssGUI::Backend::BackendMainWindowInterface::IsVSync>
            virtual bool IsVSync() const;
            
            //function: SetMSAA
            //See <ssGUI::Backend::BackendMainWindowInterface::SetMSAA>
            virtual void SetMSAA(int level);

            //function: GetMSAA
            //See <ssGUI::Backend::BackendMainWindowInterface::GetMSAA>
            virtual int GetMSAA() const;

            //function: SetWindowMode
            //See <ssGUI::Backend::BackendMainWindowInterface::SetWindowMode>
            virtual void SetWindowMode(ssGUI::Enums::WindowMode WindowMode);

            //function: GetWindowMode
            //See <ssGUI::Backend::BackendMainWindowInterface::GetWindowMode>
            virtual ssGUI::Enums::WindowMode GetWindowMode() const;

            //function: Internal_FocusChanged
            //(Internal ssGUI function) You should not be calling this function normally.
            virtual void Internal_FocusChanged(bool focused);

            //function: SetVisible
            //See <ssGUI::Backend::BackendMainWindowInterface::SetVisible>
            virtual void SetVisible(bool visible);
            
            //function: IsVisible
            //See <ssGUI::Backend::BackendMainWindowInterface::IsVisible>
            virtual bool IsVisible() const;

            //function: GetRelativeMousePosition
            //A helper function that can return the relative position to mainWindow. Normally use with <RealtimeInputInfo>
            virtual glm::ivec2 GetRelativeMousePosition(glm::ivec2 mousePos) const;
            
            //function: SetWindowSize
            //See <ssGUI::Backend::BackendMainWindowInterface::SetWindowSize>
            virtual void SetWindowSize(glm::ivec2 size);
            
            //function: GetWindowSize
            //See <ssGUI::Backend::BackendMainWindowInterface::GetWindowSize>
            virtual glm::ivec2 GetWindowSize() const;
            
            //function: SetRenderContext 
            //See <ssGUI::Backend::BackendMainWindowInterface::SetGLContext>
            bool SetRenderContext();
            
            //function: SaveState 
            //See <BackendDrawingInterface::SaveState>
            void SaveState();

            //function: RestoreState 
            //See <BackendDrawingInterface::RestoreState>
            void RestoreState();

            //function: SetEnabled
            //Proxy function for <SetVisible>
            virtual void SetEnabled(bool enabled) override;
            
            //function: IsEnabled
            //Proxy function for <IsVisible>
            virtual bool IsEnabled() const override;

            //function: IsSelfEnabled
            //Proxy function for <IsVisible>
            virtual bool IsSelfEnabled() const override;

            //function: IsFocused
            //See <ssGUI::Backend::BackendMainWindowInterface::IsFocused>
            virtual bool IsFocused() const override;
            
            //function: SetFocus
            //See <ssGUI::Backend::BackendMainWindowInterface::SetFocus>
            virtual void SetFocus(bool focus) override;
            
            //function: GetPosition
            //This will always return (0, 0). To get the main window's position on the monitor, see <GetWindowPosition>
            //See <ssGUI::Hierarchy::GetPosition>
            virtual glm::vec2 GetPosition() const override;
            
            //function: SetPosition
            //This will not set the position of the main window, to do that, see <SetWindowPosition>
            //See <ssGUI::Hierarchy::SetPosition>
            virtual void SetPosition(glm::vec2 position) override;
            
            //function: GetGlobalPosition
            //This will always return (0, 0). To get the main window's position on the monitor, see <GetWindowPosition>
            //See <ssGUI::Hierarchy::GetGlobalPosition>
            virtual glm::vec2 GetGlobalPosition() override;
            
            //function: SetGlobalPosition
            //This will not set the position of the main window, to do that, see <SetWindowPosition>
            //See <ssGUI::Hierarchy::SetGlobalPosition>
            virtual void SetGlobalPosition(glm::vec2 position) override;
            
            //function: GetSize
            //See <ssGUI::Backend::BackendMainWindowInterface::GetRenderSize>
            virtual glm::vec2 GetSize() const override;
            
            //function: SetSize
            //See <ssGUI::Backend::BackendMainWindowInterface::SetRenderSize>
            virtual void SetSize(glm::vec2 size) override;
            
            //function: SetRenderSize
            //See <ssGUI::Backend::BackendMainWindowInterface::SetRenderSize>
            virtual void SetRenderSize(glm::ivec2 size) override;

            //function: GetRenderSize
            //See <ssGUI::Backend::BackendMainWindowInterface::GetRenderSize>
            virtual glm::ivec2 GetRenderSize() const override;

            //function: GetType
            //See <Window::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: Close
            //See <Window::Close>
            virtual void Close() override;
            
            //function: AbortClosing
            //See <Window::AbortClosing>
            virtual void AbortClosing() override;
            
            //function: Internal_OnClose
            //See <Window::Internal_OnClose>
            virtual void Internal_OnClose() override; 
            
            //function: SetTitlebarHeight
            //This has no effect, you cannot set titlebar height
            virtual void SetTitlebarHeight(int height) override;
            
            //function: GetTitlebarHeight
            //This will just return "GetPositionOffset().y"
            virtual int GetTitlebarHeight() const override;

            //function: SetTitlebarColor
            //This has no effect, you cannot set titlebar color
            virtual void SetTitlebarColor(glm::u8vec4 color) override;

            //function: GetTitlebarColor
            //This will always return white
            virtual glm::u8vec4 GetTitlebarColor() const override;

            //function: SetAdaptiveTitlebarColor
            //This has no effect, you cannot set titlebar color
            virtual void SetAdaptiveTitlebarColor(bool adaptive) override;

            //function: IsAdaptiveTitlebarColor
            //This will always return false
            virtual bool IsAdaptiveTitlebarColor() const override;
            
            //function: Internal_Draw
            //This checks for window state before rendering
            virtual void Internal_Draw();
            
            //function: Internal_Draw
            //This checks for window state before rendering
            virtual void Internal_Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;
            
            //function: Internal_Update
            //See <GUIObject::Internal_Update>
            virtual void Internal_Update(   ssGUI::Backend::BackendSystemInputInterface* inputInterface, 
                                            ssGUI::InputStatus& currentInputStatus, 
                                            ssGUI::InputStatus& lastInputStatus, 
                                            ssGUI::GUIObject* mainWindow) override;
    
            //function: Clone
            //See <Window::Clone>
            virtual MainWindow* Clone(bool cloneChildren) override;

    };
}

#endif