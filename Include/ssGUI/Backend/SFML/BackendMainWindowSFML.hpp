#ifndef SSGUI_BACKEND_MAIN_WINDOW_SFML
#define SSGUI_BACKEND_MAIN_WINDOW_SFML

#include <iostream>
#include <vector>
#include "SFML/Graphics.hpp"
#include "ssGUI/Backend/Interfaces/BackendMainWindowInterface.hpp"
#include "ssGUI/Backend/SFML/BackendImageSFML.hpp"
#include "ssGUI/Backend/BackendManager.hpp"

namespace ssGUI
{ 
    
//namespace: ssGUI::Backend
namespace Backend
{
    /*class: ssGUI::Backend::BackendMainWindowSFML
    For functions explainations, please see <BackendMainWindowInterface>. Normally you don't need to deal with this class
    
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
        ssGUI::Backend::BackendManager::AddMainWindowInterface(static_cast<ssGUI::Backend::BackendMainWindowInterface*>(this));
    }
    ===============================================================
    */
    class BackendMainWindowSFML : public BackendMainWindowInterface
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
            //See <BackendMainWindowInterface::SyncPositionOffset>
            void SyncPositionOffset() override;
            
            //function: GetPositionOffset
            //See <BackendMainWindowInterface::GetPositionOffset>
            glm::ivec2 GetPositionOffset() const override;

            //function: SetWindowPosition
            //See <BackendMainWindowInterface::SetWindowPosition>
            void SetWindowPosition(glm::ivec2 pos) override;
            
            //function: GetWindowPosition
            //See <BackendMainWindowInterface::GetWindowPosition>
            glm::ivec2 GetWindowPosition() const override;
            
            //function: SetWindowSize
            //See <BackendMainWindowInterface::SetWindowSize>
            void SetWindowSize(glm::ivec2 size) override;
            
            //function: GetWindowSize
            //See <BackendMainWindowInterface::GetWindowSize>
            glm::ivec2 GetWindowSize() const override;
            
            //function: SetRenderSize
            //See <BackendMainWindowInterface::SetRenderSize>
            void SetRenderSize(glm::ivec2 size) override;

            //function: GetRenderSize
            //See <BackendMainWindowInterface::GetRenderSize>
            glm::ivec2 GetRenderSize() const override;
            
            //function: IsClosed
            //See <BackendMainWindowInterface::IsClosed>
            bool IsClosed() const override;
            
            //function: Close
            //See <BackendMainWindowInterface::Close>
            void Close() override;
            
            //function: AbortClosing
            //See <BackendMainWindowInterface::AbortClosing>
            void AbortClosing() override;

            //function: AddOnCloseEvent
            //See <BackendMainWindowInterface::AddOnCloseEvent>
            int AddOnCloseEvent(std::function<void()> func) override;
            
            //function: RemoveOnCloseEvent
            //See <BackendMainWindowInterface::RemoveOnCloseEvent>
            void RemoveOnCloseEvent(int index) override;

            //function: SetTitle
            //See <BackendMainWindowInterface::SetTitle>
            void SetTitle(std::wstring title) override;
            
            //function: GetTitle
            //See <BackendMainWindowInterface::GetTitle>
            std::wstring GetTitle() const override;
            
            //function: SetIcon
            //See <BackendMainWindowInterface::SetIcon>
            void SetIcon(const ssGUI::Backend::BackendImageInterface& iconImage) override;

            //function: SetVisible
            //See <BackendMainWindowInterface::SetVisible>
            void SetVisible(bool visible) override;
            
            //function: IsVisible
            //See <BackendMainWindowInterface::IsVisible>
            bool IsVisible() const override;

            //function: SetVSync
            //See <BackendMainWindowInterface::SetVSync>
            void SetVSync(bool vSync) override;
            
            //function: IsVSync
            //See <BackendMainWindowInterface::IsVSync>
            bool IsVSync() const override;

            //function: IsFocused
            //See <BackendMainWindowInterface::IsFocused>
            bool IsFocused() const override;
            
            //function: SetFocus
            //See <BackendMainWindowInterface::SetFocus>
            void SetFocus(bool focus, bool externalByUser) override;

            //function:AddFocusChangedByUserEvent 
            //See <BackendMainWindowInterface::AddFocusChangedByUserEvent>
            int AddFocusChangedByUserEvent(std::function<void(bool focused)> func) override;

            //function: RemoveFocusChangedByUserEvent
            //See <BackendMainWindowInterface::RemoveFocusChangedByUserEvent>
            void RemoveFocusChangedByUserEvent(int index) override;

            //function: GetMSAA
            //See <BackendMainWindowInterface::GetMSAA>
            int GetMSAA() const override;
            
            //function: SetMSAA
            //See <BackendMainWindowInterface::SetMSAA>
            void SetMSAA(int level) override;

            //function: SetTitlebar
            //See <BackendMainWindowInterface::SetTitlebar>
            //Setting title bar will cause window to be reloaded
            void SetTitlebar(bool titlebar) override;
            
            //function: HasTitlebar
            //See <BackendMainWindowInterface::HasTitlebar>
            bool HasTitlebar() const override;

            //function: SetResizable
            //See <BackendMainWindowInterface::SetResizable>
            //Setting resizable will cause window to be reloaded
            void SetResizable(bool resizable) override;
            
            //function: IsResizable
            //See <BackendMainWindowInterface::IsResizable>
            bool IsResizable() const override;

            //function: SetCloseButton
            //See <BackendMainWindowInterface::SetCloseButton>
            //Setting close button will cause window to be reloaded
            void SetCloseButton(bool closeButton) override;
            
            //function: HasCloseButton
            //See <BackendMainWindowInterface::HasCloseButton>
            bool HasCloseButton() const override;

            //function: SetWindowMode
            //See <BackendMainWindowInterface::SetWindowMode>
            void SetWindowMode(ssGUI::Enums::WindowMode WindowMode) override;

            //function: GetWindowMode
            //See <BackendMainWindowInterface::GetWindowMode>
            ssGUI::Enums::WindowMode GetWindowMode() const override;

            //function: SetGLContext
            //See <BackendMainWindowInterface::SetGLContext>
            bool SetGLContext() override;

            //function: Clone
            ssGUI::Backend::BackendMainWindowInterface* Clone() override;
            
            //function: GetRawHandle
            //This returns sf::RenderWindow object
            void* GetRawHandle() override;
    };
}

}

#endif