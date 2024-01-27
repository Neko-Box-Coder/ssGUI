#ifndef SSGUI_BACKEND_MAIN_WINDOW_MOCK_H
#define SSGUI_BACKEND_MAIN_WINDOW_MOCK_H

//#define SSGUI_MOCK_ENABLE_LOG
#include "ssGUI/Backend/Mocks/MockMacro.hpp"

#include "CppOverride.hpp"
#include "ssGUI/Backend/Interfaces/MainWindowInterface.hpp"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendMainWindowMock
    class BackendMainWindowMock : public MainWindowInterface
    {
        private:
            MainWindowInterface* UnderlyingInterface;
            glm::ivec2 WindowPosition;
            glm::ivec2 WindowTopLeftDecor;
            glm::ivec2 WindowBotRightDecor;
            glm::ivec2 WindowSize;
            //glm::ivec2 RenderSize;
            std::vector<std::pair<std::function<void()>, bool>> OnCloseListeners;
            bool Closed;
            bool ClosingAborted;
            std::u32string Title;
            ImageInterface* IconImage;
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
            BackendMainWindowMock(MainWindowInterface* mainWindowInterface);
            ~BackendMainWindowMock() override;

            CO_DECLARE_OVERRIDE_METHODS(OverrideObject)

            void SetMockDecorOffset(glm::ivec2 topLeft, glm::ivec2 bottomRight);

            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(MainWindowInterface*, UnderlyingInterface)
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
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(ImageInterface*, IconImage)
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
            void RemoveOnCloseEvent(int index) override;

            //function: SetTitle
            //See <MainWindowInterface::SetTitle>
            void SetTitle(std::u32string title) override;

            //function: SetTitle
            //See <MainWindowInterface::SetTitle>
            void SetTitle(std::string title) override;

            //function: GetTitle
            //See <MainWindowInterface::GetTitle>
            void GetTitle(std::u32string& outTitle) const override;

            //function: GetTitle
            //See <MainWindowInterface::GetTitle>
            void GetTitle(std::string& outTitle) const override;

            //function: SetIcon
            //See <MainWindowInterface::SetIcon>
            void SetIcon(const ImageInterface& iconImage) override;

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
            void RemoveFocusChangedByUserEvent(int index) override;

            //function: SetMSAA
            //See <MainWindowInterface::SetMSAA>
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

            //function: SetGLContext
            //See <MainWindowInterface::SetGLContext>
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