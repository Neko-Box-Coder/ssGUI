#ifndef SSGUI_BACKEND_MAIN_WINDOW_SFML_H
#define SSGUI_BACKEND_MAIN_WINDOW_SFML_H

#include <iostream>
#include <vector>
#include "SFML/Graphics.hpp"
#include "ssGUI/Backend/Interfaces/MainWindowInterface.hpp"
#include "ssGUI/Backend/SFML/BackendImageSFML.hpp"
#include "ssGUI/Backend/BackendManager.hpp"

namespace ssGUI
{ 
    
//namespace: ssGUI::Backend
namespace Backend
{
    /*class: ssGUI::Backend::BackendMainWindowSFML
    For functions explainations, please see <MainWindowInterface>. Normally you don't need to deal with this class
    
    Variables & Constructor:
    ============================= C++ =============================
    private:
        sf::RenderWindow CurrentWindow;                                                 //See <GetRawHandle>
        std::vector<std::function<void()>> OnCloseCallback;                             //See <AddOnCloseEvent>
        std::vector<std::function<void(bool focused)>> ExternalFocusChangedCallback;    //See <AddFocusChangedByUserEvent>
        bool Visible;                                                                   //See <IsVisible>
        bool VSync;                                                                     //See <IsVSync>
        bool Closed;                                                                    //See <IsClosed>
        ssGUI::Enums::WindowMode CurrentWindowMode;                                     //See <GetWindowMode>

        bool Titlebar;                                                                  //See <HasTitlebar>
        bool Resizable;                                                                 //See <IsResizable>
        bool CloseButton;                                                               //See <HasCloseButton>
        std::wstring Title;                                                             //See <GetTitle>
        bool IsClosingAborted;                                                          //(Internal variable) Flag to stop closing operation, see <AbortClosing>

        glm::vec2 PositionOffset;                                                       //See <GetPositionOffset>
    ===============================================================
    ============================= C++ =============================
    BackendMainWindowSFML::BackendMainWindowSFML() :    CurrentWindow(sf::VideoMode(sf::Vector2u(800, 600)), "", sf::Style::Default, sf::ContextSettings(24)),
                                                        OnCloseCallback(),
                                                        ExternalFocusChangedCallback(),
                                                        Visible(true),
                                                        VSync(false),
                                                        Closed(false),
                                                        CurrentWindowMode(ssGUI::Enums::WindowMode::NORMAL),
                                                        Titlebar(true),
                                                        Resizable(true),
                                                        CloseButton(true),
                                                        Title(L""),
                                                        IsClosingAborted(false),
                                                        PositionOffset()
    {
        ssGUI::Backend::BackendManager::AddMainWindowInterface(static_cast<ssGUI::Backend::MainWindowInterface*>(this));
    }
    ===============================================================
    */
    class BackendMainWindowSFML : public MainWindowInterface
    {
        private:
            sf::RenderWindow CurrentWindow;                                                 //See <GetRawHandle>
            std::vector<std::function<void()>> OnCloseCallback;                             //See <AddOnCloseEvent>
            std::vector<std::function<void(bool focused)>> ExternalFocusChangedCallback;    //See <AddFocusChangedByUserEvent>
            bool Visible;                                                                   //See <IsVisible>
            bool VSync;                                                                     //See <IsVSync>
            bool Closed;                                                                    //See <IsClosed>
            ssGUI::Enums::WindowMode CurrentWindowMode;                                     //See <GetWindowMode>

            bool Titlebar;                                                                  //See <HasTitlebar>
            bool Resizable;                                                                 //See <IsResizable>
            bool CloseButton;                                                               //See <HasCloseButton>
            std::wstring Title;                                                             //See <GetTitle>
            bool IsClosingAborted;                                                          //(Internal variable) Flag to stop closing operation, see <AbortClosing>

            glm::vec2 PositionOffset;                                                       //See <GetPositionOffset>

            BackendMainWindowSFML& operator=(BackendMainWindowSFML const& other);
            void ResetWindow(ssGUI::Enums::WindowMode mode, bool resize, bool titlebar, bool canClose, int msaa);
        
        protected:
            BackendMainWindowSFML(BackendMainWindowSFML const& other);

        public:
            //BackendMainWindowSFML(const BackendDrawingSFML& backendDrawing) = default;
            BackendMainWindowSFML();
            ~BackendMainWindowSFML() override;

            //function: SyncPositionOffset
            //See <MainWindowInterface::SyncPositionOffset>
            void SyncPositionOffset();
            
            //function: GetPositionOffset
            //See <MainWindowInterface::GetPositionOffset>
            glm::ivec2 GetPositionOffset() const override;

            //function: SetWindowPosition
            //See <MainWindowInterface::SetWindowPosition>
            void SetWindowPosition(glm::ivec2 pos) override;
            
            //function: GetWindowPosition
            //See <MainWindowInterface::GetWindowPosition>
            glm::ivec2 GetWindowPosition() const override;
            
            //function: SetWindowSize
            //See <MainWindowInterface::SetWindowSize>
            void SetWindowSize(glm::ivec2 size) override;
            
            //function: GetWindowSize
            //See <MainWindowInterface::GetWindowSize>
            glm::ivec2 GetWindowSize() const override;
            
            //function: SetRenderSize
            //See <MainWindowInterface::SetRenderSize>
            void SetRenderSize(glm::ivec2 size) override;

            //function: GetRenderSize
            //See <MainWindowInterface::GetRenderSize>
            glm::ivec2 GetRenderSize() const override;
            
            //function: IsClosed
            //See <MainWindowInterface::IsClosed>
            bool IsClosed() const override;
            
            //function: Close
            //See <MainWindowInterface::Close>
            void Close() override;
            
            //function: AbortClosing
            //See <MainWindowInterface::AbortClosing>
            void AbortClosing() override;

            //function: AddOnCloseEvent
            //See <MainWindowInterface::AddOnCloseEvent>
            int AddOnCloseEvent(std::function<void()> func) override;
            
            //function: RemoveOnCloseEvent
            //See <MainWindowInterface::RemoveOnCloseEvent>
            void RemoveOnCloseEvent(int index) override;

            //function: SetTitle
            //See <MainWindowInterface::SetTitle>
            void SetTitle(std::wstring title) override;
            
            //function: GetTitle
            //See <MainWindowInterface::GetTitle>
            std::wstring GetTitle() const override;
            
            //function: SetIcon
            //See <MainWindowInterface::SetIcon>
            void SetIcon(const ssGUI::Backend::ImageInterface& iconImage) override;

            //function: SetVisible
            //See <MainWindowInterface::SetVisible>
            void SetVisible(bool visible) override;
            
            //function: IsVisible
            //See <MainWindowInterface::IsVisible>
            bool IsVisible() const override;

            //function: SetVSync
            //See <MainWindowInterface::SetVSync>
            void SetVSync(bool vSync) override;
            
            //function: IsVSync
            //See <MainWindowInterface::IsVSync>
            bool IsVSync() const override;

            //function: IsFocused
            //See <MainWindowInterface::IsFocused>
            bool IsFocused() const override;
            
            //function: SetFocus
            //See <MainWindowInterface::SetFocus>
            void SetFocus(bool focus, bool externalByUser) override;

            //function:AddFocusChangedByUserEvent 
            //See <MainWindowInterface::AddFocusChangedByUserEvent>
            int AddFocusChangedByUserEvent(std::function<void(bool focused)> func) override;

            //function: RemoveFocusChangedByUserEvent
            //See <MainWindowInterface::RemoveFocusChangedByUserEvent>
            void RemoveFocusChangedByUserEvent(int index) override;

            //function: GetAntiAliasingLevel
            //See <MainWindowInterface::GetAntiAliasingLevel>
            int GetAntiAliasingLevel() const override;
            
            //function: SetMSAA
            //See <MainWindowInterface::SetMSAA>
            void SetMSAA(int level) override;

            //function: SetTitlebar
            //See <MainWindowInterface::SetTitlebar>
            //Setting title bar will cause window to be reloaded
            void SetTitlebar(bool titlebar) override;
            
            //function: HasTitlebar
            //See <MainWindowInterface::HasTitlebar>
            bool HasTitlebar() const override;

            //function: SetResizable
            //See <MainWindowInterface::SetResizable>
            //Setting resizable will cause window to be reloaded
            void SetResizable(bool resizable) override;
            
            //function: IsResizable
            //See <MainWindowInterface::IsResizable>
            bool IsResizable() const override;

            //function: SetCloseButton
            //See <MainWindowInterface::SetCloseButton>
            //Setting close button will cause window to be reloaded
            void SetCloseButton(bool closeButton) override;
            
            //function: HasCloseButton
            //See <MainWindowInterface::HasCloseButton>
            bool HasCloseButton() const override;

            //function: SetWindowMode
            //See <MainWindowInterface::SetWindowMode>
            void SetWindowMode(ssGUI::Enums::WindowMode WindowMode) override;

            //function: GetWindowMode
            //See <MainWindowInterface::GetWindowMode>
            ssGUI::Enums::WindowMode GetWindowMode() const override;

            //function: SetGLContext
            //See <MainWindowInterface::SetGLContext>
            bool SetGLContext() override;

            //function: Clone
            ssGUI::Backend::MainWindowInterface* Clone() override;
            
            //function: GetRawHandle
            //This returns sf::RenderWindow object
            void* GetRawHandle() override;
    };
}

}

#endif