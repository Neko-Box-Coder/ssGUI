#ifndef SSGUI_BACKEND_MAIN_WINDOW_MOCK_H
#define SSGUI_BACKEND_MAIN_WINDOW_MOCK_H

//#define SSGUI_MOCK_ENABLE_LOG
#include "ssGUI/Backend/Mocks/MockMacro.hpp"

#include "CppOverride.hpp"
#include "ssGUI/Backend/Interfaces/BackendMainWindowInterface.hpp"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendMainWindowMock
    class BackendMainWindowMock : public BackendMainWindowInterface
    {
        private:
            BackendMainWindowInterface* UnderlyingInterface;
            glm::ivec2 WindowPosition;
            glm::ivec2 WindowTopLeftDecor;
            glm::ivec2 WindowBotRightDecor;
            glm::ivec2 WindowSize;
            //glm::ivec2 RenderSize;
            std::vector<std::pair<std::function<void()>, bool>> OnCloseListeners;
            bool Closed;
            bool ClosingAborted;
            std::u32string Title;
            BackendImageInterface* IconImage;
            bool Visible;
            bool VSync;
            bool Focused;
            bool FocusSetByExternal;
            std::vector<std::pair<std::function<void(bool)>, bool>> FocusChangedListeners;
            int MSAA;
            bool Titlebar;
            bool Resizable;
            bool CloseButton;
            Enums::WindowMode WindowMode;
            
            CO_DECLARE_MEMBER_INSTNACE(OverrideObject);
        
            BackendMainWindowMock& operator=(BackendMainWindowMock const& other);

        protected:
            BackendMainWindowMock(BackendMainWindowMock const& other);

        public:
            BackendMainWindowMock(BackendMainWindowInterface* mainWindowInterface);
            ~BackendMainWindowMock() override;

            CO_DECLARE_OVERRIDE_METHODS(OverrideObject)

            void SetMockDecorOffset(glm::ivec2 topLeft, glm::ivec2 bottomRight);

            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(BackendMainWindowInterface*, UnderlyingInterface)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(glm::ivec2, WindowPosition)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(glm::ivec2, WindowTopLeftDecor)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(glm::ivec2, WindowBotRightDecor)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(glm::ivec2, WindowSize)
            //SSGUI_MOCK_DECLARE_VARIABLE_GETTER(glm::ivec2, RenderSize)
            
            using CloseListeners = std::vector<std::pair<std::function<void()>, bool>>;
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(CloseListeners, OnCloseListeners)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(bool, Closed)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(bool, ClosingAborted)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(std::u32string, Title)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(BackendImageInterface*, IconImage)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(bool, Visible)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(bool, VSync)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(bool, Focused)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(bool, FocusSetByExternal)
            
            using FocusListeners = std::vector<std::pair<std::function<void(bool)>, bool>>;
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(FocusListeners, FocusChangedListeners)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(int, MSAA)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(bool, Titlebar)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(bool, Resizable)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(bool, CloseButton)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(Enums::WindowMode, WindowMode)

            //function: SetWindowPosition
            //See <BackendMainWindowInterface::SetWindowPosition>
            void SetWindowPosition(glm::ivec2 pos) override;

            //function: GetWindowPosition
            //See <BackendMainWindowInterface::GetWindowPosition>
            glm::ivec2 GetWindowPosition() const override;

            //function: GetDecorationOffsets
            //See <BackendMainWindowInterface::GetDecorationOffsets>
            void GetDecorationOffsets(glm::ivec2& topLeft, glm::ivec2& bottomRight) const override;

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
            void SetTitle(std::u32string title) override;

            //function: SetTitle
            //See <BackendMainWindowInterface::SetTitle>
            void SetTitle(std::string title) override;

            //function: GetTitle
            //See <BackendMainWindowInterface::GetTitle>
            void GetTitle(std::u32string& outTitle) const override;

            //function: GetTitle
            //See <BackendMainWindowInterface::GetTitle>
            void GetTitle(std::string& outTitle) const override;

            //function: SetIcon
            //See <BackendMainWindowInterface::SetIcon>
            void SetIcon(const BackendImageInterface& iconImage) override;

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

            //function: SetFocus
            //See <BackendMainWindowInterface::SetFocus>
            void SetFocus(bool focus, bool externalByUser) override;
            
            //function: IsFocused
            //See <BackendMainWindowInterface::IsFocused>
            bool IsFocused() const override;

            //function: AddFocusChangedByUserEvent
            //See <BackendMainWindowInterface::AddFocusChangedByUserEvent>
            int AddFocusChangedByUserEvent(std::function<void(bool focused)> func) override;

            //function: RemoveFocusChangedByUserEvent
            //See <BackendMainWindowInterface::RemoveFocusChangedByUserEvent>
            void RemoveFocusChangedByUserEvent(int index) override;

            //function: SetMSAA
            //See <BackendMainWindowInterface::SetMSAA>
            void SetAntiAliasingLevel(int level) override;

            //function: GetAntiAliasingLevel
            //See <BackendMainWindowInterface::GetAntiAliasingLevel>
            int GetAntiAliasingLevel() const override;

            //function: SetResizable
            //See <BackendMainWindowInterface::SetResizable>
            void SetResizable(bool resizable) override;

            //function: IsResizable
            //See <BackendMainWindowInterface::IsResizable>
            bool IsResizable() const override;

            //function: SetCloseButton
            //See <BackendMainWindowInterface::SetCloseButton>
            void SetCloseButton(bool closeButton) override;

            //function: HasCloseButton
            //See <BackendMainWindowInterface::HasCloseButton>
            bool HasCloseButton() const override;

            //function: SetWindowMode
            //See <BackendMainWindowInterface::SetWindowMode>
            void SetWindowMode(Enums::WindowMode windowMode) override;

            //function: GetWindowMode
            //See <BackendMainWindowInterface::GetWindowMode>
            Enums::WindowMode GetWindowMode() const override;

            //function: SetGLContext
            //See <BackendMainWindowInterface::SetGLContext>
            bool SetDrawingContext() override;
            
            //function: Clone
            //See <BackendMainWindowInterface::Clone>
            BackendMainWindowInterface* Clone() override;

            //function: GetRawHandle
            //See <BackendMainWindowInterface::GetRawHandle>
            void* GetRawHandle() const override;
    };
}

}

#endif