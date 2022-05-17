#ifndef SSGUI_MAIN_WINDOW
#define SSGUI_MAIN_WINDOW


#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/GUIObjectClasses/Window.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"
#include <functional>


//namespace: ssGUI
namespace ssGUI
{
    /*class: ssGUI::MainWindow
    Main Window is the *actual* window. Not to be confused with <ssGUI::Window> which lives inside MainWindow.
    You should call <GUIObject::RedrawObject> after changing the properties of the MainWindow.
    This class should not be overwritten unless you know what you are doing
    
    Variables & Constructor:
    ============================== C++ ==============================
    private:
        ssGUI::Backend::BackendMainWindowInterface* BackendMainWindow;
        ssGUI::Backend::BackendDrawingInterface* BackendDrawing;
        glm::vec2 LastSize;
        int RedrawCount;

        uint64_t LastSyncTime;
    =================================================================
    ============================== C++ ==============================
    MainWindow::MainWindow() : BackendMainWindow(), BackendDrawing(), LastSize(glm::vec2(0, 0)), RedrawCount(0), LastSyncTime(0)
    {
        BackendMainWindow = ssGUI::Backend::BackendFactory::CreateBackendMainWindowInterface();
        BackendDrawing = ssGUI::Backend::BackendFactory::CreateBackendDrawingInterface();
        BackendMainWindow->AddOnCloseEvent(std::bind(&ssGUI::MainWindow::Internal_OnClose, this));
        BackendMainWindow->SetMSAA(8);
        SetBackgroundColor(glm::u8vec4(255, 255, 255, 255));
    }
    =================================================================
    */
    class MainWindow : public Window
    {
        private:
            ssGUI::Backend::BackendMainWindowInterface* BackendMainWindow;
            ssGUI::Backend::BackendDrawingInterface* BackendDrawing;
            glm::vec2 LastSize;
            int RedrawCount;

            uint64_t LastSyncTime;
            MainWindow& operator=(MainWindow const& other);

        protected:
            MainWindow(MainWindow const& other);

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
            
            //function: GetDisplayPosition
            //See <BackendMainWindowInterface::GetDisplayPosition>
            virtual glm::ivec2 GetDisplayPosition() const;

            //function: SetDisplayPosition
            //See <BackendMainWindowInterface::SetDisplayPosition>
            virtual void SetDisplayPosition(glm::ivec2 pos);

            //function: GetPositionOffset
            //See <BackendMainWindowInterface::GetPositionOffset>
            virtual glm::ivec2 GetPositionOffset() const;

            //function: SetTitle
            //See <BackendMainWindowInterface::IsVSync>
            virtual void SetTitle(std::wstring title);
            //function: GetTitle
            //See <BackendMainWindowInterface::IsVSync>
            virtual std::wstring GetTitle() const;

            //function: SetIcon
            //See <BackendMainWindowInterface::SetIcon>
            virtual void SetIcon(ssGUI::ImageData& iconImage);

            //function: SetVSync
            //See <BackendMainWindowInterface::SetVSync>
            virtual void SetVSync(bool vSync);
            
            //function: IsVSync
            //See <BackendMainWindowInterface::IsVSync>
            virtual bool IsVSync() const;

            //function: IsFocused
            //See <BackendMainWindowInterface::IsFocused>
            virtual bool IsFocused() const;
            
            //function: SetFocus
            //See <BackendMainWindowInterface::SetFocus>
            virtual void SetFocus(bool focus);
            
            //function: SetMSAA
            //See <BackendMainWindowInterface::SetMSAA>
            virtual void SetMSAA(int level);

            //function: GetMSAA
            //See <BackendMainWindowInterface::GetMSAA>
            virtual int GetMSAA() const;

            //function: SetWindowMode
            //See <BackendMainWindowInterface::SetWindowMode>
            virtual void SetWindowMode(ssGUI::Enums::WindowMode WindowMode);

            //function: GetWindowMode
            //See <BackendMainWindowInterface::GetWindowMode>
            virtual ssGUI::Enums::WindowMode GetWindowMode() const;

            //function: SetVisible
            //See <BackendMainWindowInterface::SetVisible>
            virtual void SetVisible(bool visible) override;
            
            //function: IsVisible
            //See <BackendMainWindowInterface::IsVisible>
            virtual bool IsVisible() const override;
            
            //function: GetPosition
            //This will always return (0, 0). To get the main window's position on the monitor, see <GetDisplayPosition>
            //See <GUIObject::GetPosition>
            virtual glm::vec2 GetPosition() const override;
            
            //function: SetPosition
            //This will not set the position of the main window, to do that, see <SetDisplayPosition>
            //See <GUIObject::SetPosition>
            virtual void SetPosition(glm::vec2 position) override;
            
            //function: GetGlobalPosition
            //This will always return (0, 0). To get the main window's position on the monitor, see <GetDisplayPosition>
            //See <GUIObject::GetGlobalPosition>
            virtual glm::vec2 GetGlobalPosition() override;
            
            //function: SetGlobalPosition
            //This will not set the position of the main window, to do that, see <SetDisplayPosition>
            //See <GUIObject::SetGlobalPosition>
            virtual void SetGlobalPosition(glm::vec2 position) override;
            
            //function: GetSize
            //See <BackendMainWindowInterface::GetSize>
            virtual glm::vec2 GetSize() const override;
            
            //function: SetSize
            //See <BackendMainWindowInterface::SetSize>
            virtual void SetSize(glm::vec2 size) override;

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
            
            //function: Internal_Draw
            //This checks for window state before rendering
            virtual void Internal_Draw();
            
            //function: Internal_Draw
            //This checks for window state before rendering
            virtual void Internal_Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;
            
            //function: Internal_Update
            //See <Window::Internal_Update>
            virtual void Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;
    
            //function: Clone
            //See <Window::Clone>
            virtual MainWindow* Clone(bool cloneChildren) override;

    };
}

#endif