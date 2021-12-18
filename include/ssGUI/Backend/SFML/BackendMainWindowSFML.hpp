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
    //class: BackendMainWindowSFML
    //For functions explainations, please see <BackendMainWindowInterface>. Normally you don't need to deal with this class
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

            glm::ivec2 PositionOffset;

            BackendMainWindowSFML& operator=(BackendMainWindowSFML const& other);
            void ResetWindow(bool resize, bool titlebar, bool canClose, int msaa);
        
        protected:
            BackendMainWindowSFML(BackendMainWindowSFML const& other);

        public:
            //BackendMainWindowSFML(const BackendDrawingSFML& backendDrawing) = default;
            BackendMainWindowSFML();
            virtual ~BackendMainWindowSFML();

            //function: SyncPositionOffset
            void SyncPositionOffset() override;
            
            //function: GetPositionOffset
            glm::ivec2 GetPositionOffset() const override;

            //function: SetPosition
            void SetPosition(glm::ivec2 pos) override;
            
            //function: GetPosition
            glm::ivec2 GetPosition() const override;

            //function: SetSize
            void SetSize(glm::ivec2 size) override;
            
            //function: GetSize
            glm::ivec2 GetSize() const override;
            
            //function: IsClosed
            bool IsClosed() const override;
            
            //function: Close
            void Close() override;
            
            //function: AbortClosing
            void AbortClosing() override;

            //function: AddOnCloseEvent
            int AddOnCloseEvent(std::function<void()> func) override;
            
            //function: RemoveOnCloseEvent
            void RemoveOnCloseEvent(int index) override;

            //function: SetTitle
            void SetTitle(std::string title) override;
            
            //function: GetTitle
            std::string GetTitle() const override;
            
            //function: SetIcon
            void SetIcon(const ssGUI::Backend::BackendImageInterface& iconImage) override;

            //function: SetVisible
            void SetVisible(bool visible) override;
            
            //function: IsVisible
            bool IsVisible() const override;

            //function: SetVSync
            void SetVSync(bool vSync) override;
            
            //function: IsVSync
            bool IsVSync() const override;

            //function: IsFocused
            bool IsFocused() const override;
            
            //function: SetFocus
            void SetFocus(bool focus) override; 

            //function: GetMSAA
            int GetMSAA() const override;
            
            //function: SetMSAA
            void SetMSAA(int level) override;

            
            //function: SetTitlebar
            //Setting title bar will cause window to be reloaded
            void SetTitlebar(bool titlebar) override;
            
            //function: HasTitlebar
            bool HasTitlebar() const override;

            //function: SetResizable
            //Setting resizable will cause window to be reloaded
            void SetResizable(bool resizable) override;
            
            //function: IsResizable
            bool IsResizable() const override;

            
            //function: SetCloseButton
            //Setting close button will cause window to be reloaded
            void SetCloseButton(bool closeButton) override;
            
            //function: HasCloseButton
            bool HasCloseButton() const override;

            //function: Clone
            ssGUI::Backend::BackendMainWindowInterface* Clone() override;

            
            //function: GetRawHandle
            //This returns sf::RenderWindow object
            virtual void* GetRawHandle() override;
    };
}

#endif