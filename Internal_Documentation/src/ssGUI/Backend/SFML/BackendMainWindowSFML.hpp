#ifndef SSGUI_BACKEND_MAIN_WINDOW_SFML
#define SSGUI_BACKEND_MAIN_WINDOW_SFML

#include <iostream>
#include <vector>
#include "SFML/Graphics.hpp"
#include "ssGUI/Backend/Interfaces/BackendMainWindowInterface.hpp"
#include "ssGUI/Backend/SFML/BackendImageSFML.hpp"
#include "ssGUI/Backend/BackendManager.hpp"

//namespace: ssGUI::Backend
namespace ssGUI::Backend
{
    /*class: ssGUI::Backend::BackendMainWindowSFML
    For functions explainations, please see <BackendMainWindowInterface>. Normally you don't need to deal with this class
    
    Variables & Constructor:
    ============================= C++ =============================
    private:
        sf::RenderWindow CurrentWindow;
        std::vector<std::function<void()>> OnCloseCallback;
        bool Visible;
        bool VSync;
        bool Closed;

        bool Titlebar;
        bool Resizable;
        bool CloseButton;
        std::string Title;
        bool IsClosingAborted;

        glm::vec2 PositionOffset;
    ===============================================================
    ============================= C++ =============================
    BackendMainWindowSFML::BackendMainWindowSFML() : CurrentWindow(sf::VideoMode(800, 600), ""), Visible(false), VSync(false), Closed(false),
                                                        Titlebar(true), Resizable(true), CloseButton(true), Title(), IsClosingAborted(false), 
                                                        PositionOffset()
    {
        ssGUI::Backend::BackendManager::AddMainWindowInterface(static_cast<ssGUI::Backend::BackendMainWindowInterface*>(this));
    }
    ===============================================================
    */
    class BackendMainWindowSFML : public BackendMainWindowInterface
    {
        private:
            sf::RenderWindow CurrentWindow;
            std::vector<std::function<void()>> OnCloseCallback;
            bool Visible;
            bool VSync;
            bool Closed;

            bool Titlebar;
            bool Resizable;
            bool CloseButton;
            std::string Title;
            bool IsClosingAborted;

            glm::vec2 PositionOffset;

            BackendMainWindowSFML& operator=(BackendMainWindowSFML const& other);
            void ResetWindow(bool resize, bool titlebar, bool canClose, int msaa);
        
        protected:
            BackendMainWindowSFML(BackendMainWindowSFML const& other);

        public:
            //BackendMainWindowSFML(const BackendDrawingSFML& backendDrawing) = default;
            BackendMainWindowSFML();
            virtual ~BackendMainWindowSFML();

            //function: SyncPositionOffset
            //See <BackendMainWindowInterface::SyncPositionOffset>
            void SyncPositionOffset() override;
            
            //function: GetPositionOffset
            //See <BackendMainWindowInterface::GetPositionOffset>
            glm::ivec2 GetPositionOffset() const override;

            //function: SetPosition
            //See <BackendMainWindowInterface::SetPosition>
            void SetPosition(glm::ivec2 pos) override;
            
            //function: GetPosition
            //See <BackendMainWindowInterface::GetPosition>
            glm::ivec2 GetPosition() const override;

            //function: SetSize
            //See <BackendMainWindowInterface::SetSize>
            void SetSize(glm::ivec2 size) override;
            
            //function: GetSize
            //See <BackendMainWindowInterface::GetSize>
            glm::ivec2 GetSize() const override;
            
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
            void SetTitle(std::string title) override;
            
            //function: GetTitle
            //See <BackendMainWindowInterface::GetTitle>
            std::string GetTitle() const override;
            
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
            void SetFocus(bool focus) override; 

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

            //function: Clone
            ssGUI::Backend::BackendMainWindowInterface* Clone() override;

            
            //function: GetRawHandle
            //This returns sf::RenderWindow object
            virtual void* GetRawHandle() override;
    };
}

#endif