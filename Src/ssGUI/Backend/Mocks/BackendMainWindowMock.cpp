#include "ssGUI/Backend/Mocks/BackendMainWindowMock.hpp"

#include "ssGUI/Backend/Mocks/MockMacro.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendMainWindowMock::BackendMainWindowMock(BackendMainWindowMock const& other)
    {
        ssGUI_ERROR(ssGUI_BACKEND_TAG, "Not Implemented");
        ssLOG_EXIT_PROGRAM(1);
    }

    BackendMainWindowMock::BackendMainWindowMock(ssGUI::Backend::BackendMainWindowInterface* mainWindowInterface)
    {
        UnderlyingInterface = mainWindowInterface;
    }

    BackendMainWindowMock::~BackendMainWindowMock()
    {
        if(UnderlyingInterface != nullptr)
            delete UnderlyingInterface;
    }
    
    void BackendMainWindowMock::SetWindowPosition(glm::ivec2 pos)
    {
        SSGUI_MOCK_PASSTHROUGH(SetWindowPosition(pos));
    }

    glm::ivec2 BackendMainWindowMock::GetWindowPosition() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetWindowPosition());
        return glm::ivec2();
    }

    glm::ivec2 BackendMainWindowMock::GetPositionOffset() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetPositionOffset());
        return glm::ivec2();
    }

    void BackendMainWindowMock::SetWindowSize(glm::ivec2 size)
    {
        SSGUI_MOCK_PASSTHROUGH(SetWindowSize(size));
    }

    glm::ivec2 BackendMainWindowMock::GetWindowSize() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetWindowSize());
        return glm::ivec2();
    }

    void BackendMainWindowMock::SetRenderSize(glm::ivec2 size)
    {
        SSGUI_MOCK_PASSTHROUGH(SetRenderSize(size));
    }
    
    glm::ivec2 BackendMainWindowMock::GetRenderSize() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetRenderSize());
        return glm::ivec2();
    }

    bool BackendMainWindowMock::IsClosed() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(IsClosed());
        return true;
    }

    void BackendMainWindowMock::Close()
    {
        SSGUI_MOCK_PASSTHROUGH(Close());
    }
    
    void BackendMainWindowMock::AbortClosing()
    {
        SSGUI_MOCK_PASSTHROUGH(AbortClosing());
    }

    int BackendMainWindowMock::AddOnCloseEvent(std::function<void()> func)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(AddOnCloseEvent(func));
        return 0;
    }

    void BackendMainWindowMock::RemoveOnCloseEvent(int index)
    {
        SSGUI_MOCK_PASSTHROUGH(RemoveOnCloseEvent(index));
    }

    void BackendMainWindowMock::SetTitle(std::wstring title)
    {
        SSGUI_MOCK_PASSTHROUGH(SetTitle(title));
    }

    std::wstring BackendMainWindowMock::GetTitle() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetTitle());
        return L"";
    }

    void BackendMainWindowMock::SetIcon(const ssGUI::Backend::BackendImageInterface& iconImage)
    {
        SSGUI_MOCK_PASSTHROUGH(SetIcon(iconImage));
    }

    void BackendMainWindowMock::SetVisible(bool visible)
    {
        SSGUI_MOCK_PASSTHROUGH(SetVisible(visible));
    }

    bool BackendMainWindowMock::IsVisible() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(IsVisible());
        return true;
    }

    void BackendMainWindowMock::SetVSync(bool vSync)
    {
        SSGUI_MOCK_PASSTHROUGH(SetVSync(vSync));
    }

    bool BackendMainWindowMock::IsVSync() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(IsVSync());
        return true;
    }

    void BackendMainWindowMock::SetFocus(bool focus, bool externalByUser)
    {
        SSGUI_MOCK_PASSTHROUGH(SetFocus(focus, externalByUser));
    }
    
    bool BackendMainWindowMock::IsFocused() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(IsFocused());
        return true;
    }

    int BackendMainWindowMock::AddFocusChangedByUserEvent(std::function<void(bool focused)> func)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(AddFocusChangedByUserEvent(func));
        return 0;
    }

    void BackendMainWindowMock::RemoveFocusChangedByUserEvent(int index)
    {
        SSGUI_MOCK_PASSTHROUGH(RemoveFocusChangedByUserEvent(index));
    }

    void BackendMainWindowMock::SetMSAA(int level)
    {
        SSGUI_MOCK_PASSTHROUGH(SetMSAA(level));
    }

    int BackendMainWindowMock::GetMSAA() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetMSAA());
        return 0;
    }

    void BackendMainWindowMock::SetTitlebar(bool titlebar)
    {
        SSGUI_MOCK_PASSTHROUGH(SetTitlebar(titlebar));
    }

    bool BackendMainWindowMock::HasTitlebar() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(HasTitlebar());
        return true;
    }

    void BackendMainWindowMock::SetResizable(bool resizable)
    {
        SSGUI_MOCK_PASSTHROUGH(SetResizable(resizable));
    }

    bool BackendMainWindowMock::IsResizable() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(IsResizable());
        return true;
    }

    void BackendMainWindowMock::SetCloseButton(bool closeButton)
    {
        SSGUI_MOCK_PASSTHROUGH(SetCloseButton(closeButton));
    }

    bool BackendMainWindowMock::HasCloseButton() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(HasCloseButton());
        return true;
    }

    void BackendMainWindowMock::SetWindowMode(ssGUI::Enums::WindowMode windowMode)
    {
        SSGUI_MOCK_PASSTHROUGH(SetWindowMode(windowMode));
    }

    ssGUI::Enums::WindowMode BackendMainWindowMock::GetWindowMode() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetWindowMode());
        return ssGUI::Enums::WindowMode::NORMAL;
    }

    bool BackendMainWindowMock::SetGLContext()
    {
        SSGUI_MOCK_PASSTHROUGH(SetGLContext());
        return true;
    }
    
    BackendMainWindowInterface* BackendMainWindowMock:: Clone()
    {
        return new BackendMainWindowMock(*this);
    }

    void* BackendMainWindowMock::GetRawHandle()
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetRawHandle());
        return nullptr;
    }
}

}