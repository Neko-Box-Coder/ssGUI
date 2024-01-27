#ifndef SSGUI_BACKEND_MAIN_WINDOW_TEMPLATE_HPP
#define SSGUI_BACKEND_MAIN_WINDOW_TEMPLATE_HPP

#include "ssGUI/Backend/Interfaces/MainWindowInterface.hpp"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendMainWindowTemplate
    class BackendMainWindowTemplate : public MainWindowInterface
    {
        private:
            bool WindowClosed;
            bool WindowClosingAborted;
            std::vector<std::function<void()>> OnCloseCallback;
            std::vector<std::function<void(bool focused)>> ExternalFocusChangedCallback;
            
            BackendMainWindowTemplate& operator=(BackendMainWindowTemplate const& other);
            
            void DestroyWindow();

        protected:
            BackendMainWindowTemplate(BackendMainWindowTemplate const& other);

        public:
            BackendMainWindowTemplate();
            ~BackendMainWindowTemplate() override;

            //function: SetWindowPosition
            //See <MainWindowInterface::SetWindowPosition>
            void SetWindowPosition(glm::ivec2 pos) override;

            //function: GetWindowPosition
            //See <MainWindowInterface::GetWindowPosition>
            glm::ivec2 GetWindowPosition() const override;

            //function: GetDecorationOffsets
            //See <MainWindowInterface::GetDecorationOffsets>
            void GetDecorationOffsets(glm::ivec2& topLeft, glm::ivec2& bottomRight) const override;

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
            void RemoveOnCloseEvent(int id) override;

            //function: SetTitle
            //See <MainWindowInterface::SetTitle>
            void SetTitle(std::u32string title) override;

            //function: SetTitle
            //See <MainWindowInterface::SetTitle>
            void SetTitle(std::string title) override;

            //function: GetTitle
            //See <MainWindowInterface::GetTitle>
            void GetTitle(std::u32string& title) const override;

            //function: GetTitle
            //See <MainWindowInterface::GetTitle>
            void GetTitle(std::string& title) const override;

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

            //function: SetFocus
            //See <MainWindowInterface::SetFocus>
            void SetFocus(bool focus, bool externalByUser) override;
            
            //function: IsFocused
            //See <MainWindowInterface::IsFocused>
            bool IsFocused() const override;

            //function: AddFocusChangedByUserEvent
            //See <MainWindowInterface::AddFocusChangedByUserEvent>
            int AddFocusChangedByUserEvent(std::function<void(bool focused)> func) override;

            //function: RemoveFocusChangedByUserEvent
            //See <MainWindowInterface::RemoveFocusChangedByUserEvent>
            void RemoveFocusChangedByUserEvent(int id) override;

            //function: SetAntiAliasingLevel
            //See <MainWindowInterface::SetAntiAliasingLevel>
            void SetAntiAliasingLevel(int level) override;

            //function: GetAntiAliasingLevel
            //See <MainWindowInterface::GetAntiAliasingLevel>
            int GetAntiAliasingLevel() const override;

            //function: SetResizable
            //See <MainWindowInterface::SetResizable>
            void SetResizable(bool resizable) override;

            //function: IsResizable
            //See <MainWindowInterface::IsResizable>
            bool IsResizable() const override;

            //function: SetCloseButton
            //See <MainWindowInterface::SetCloseButton>
            void SetCloseButton(bool closeButton) override;

            //function: HasCloseButton
            //See <MainWindowInterface::HasCloseButton>
            bool HasCloseButton() const override;

            //function: SetWindowMode
            //See <MainWindowInterface::SetWindowMode>
            void SetWindowMode(Enums::WindowMode windowMode) override;

            //function: GetWindowMode
            //See <MainWindowInterface::GetWindowMode>
            Enums::WindowMode GetWindowMode() const override;

            //function: SetDrawingContext
            //See <MainWindowInterface::SetDrawingContext>
            bool SetDrawingContext() override;
            
            //function: Clone
            //See <MainWindowInterface::Clone>
            MainWindowInterface* Clone() override;

            //function: GetRawHandle
            //See <MainWindowInterface::GetRawHandle>
            void* GetRawHandle() const override;
    };
}

}

#endif