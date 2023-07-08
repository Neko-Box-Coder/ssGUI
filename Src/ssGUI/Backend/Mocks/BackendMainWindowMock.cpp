#include "ssGUI/Backend/Mocks/BackendMainWindowMock.hpp"
#include "ssGUI/HelperClasses/OutputStreamUtil.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendMainWindowMock::BackendMainWindowMock(BackendMainWindowMock const& other) :
        UnderlyingInterface(nullptr),
        WindowPosition(other.WindowPosition),
        PositionOffset(other.PositionOffset),
        SizeOffset(other.SizeOffset),
        WindowSize(other.WindowSize),
        //RenderSize(other.RenderSize),
        OnCloseListeners(),
        Closed(false),
        ClosingAborted(false),
        Title(other.Title),
        IconImage(),
        Visible(other.Visible),
        VSync(other.Visible),
        Focused(true),
        FocusSetByExternal(true),
        FocusChangedListeners(),
        MSAA(other.MSAA),
        Titlebar(other.Titlebar),
        Resizable(other.Resizable),
        CloseButton(other.CloseButton),
        WindowMode(other.WindowMode),
        OverrideObject(other.OverrideObject)
    {
        if(other.UnderlyingInterface != nullptr)
            UnderlyingInterface = other.UnderlyingInterface->Clone();
    }

    BackendMainWindowMock::BackendMainWindowMock(ssGUI::Backend::BackendMainWindowInterface* mainWindowInterface) :
        UnderlyingInterface(mainWindowInterface),
        WindowPosition(),
        PositionOffset(glm::ivec2(5, 5)),
        SizeOffset(glm::ivec2(10, 10)),
        WindowSize(100, 100),
        //RenderSize(90, 90),
        OnCloseListeners(),
        Closed(false),
        ClosingAborted(false),
        Title(),
        IconImage(nullptr),
        Visible(true),
        VSync(false),
        Focused(true),
        FocusSetByExternal(false),
        FocusChangedListeners(),
        MSAA(0),
        Titlebar(true),
        Resizable(true),
        CloseButton(true),
        WindowMode(ssGUI::Enums::WindowMode::NORMAL)
    {}

    BackendMainWindowMock::~BackendMainWindowMock()
    {
        if(UnderlyingInterface != nullptr)
            delete UnderlyingInterface;
    }
    
    void BackendMainWindowMock::SetMockPositionOffset(glm::ivec2 offset)
    {
        PositionOffset = offset;
    }
    
    void BackendMainWindowMock::SetWindowPosition(glm::ivec2 pos)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(SetWindowPosition(pos));
        WindowPosition = pos;
    }

    glm::ivec2 BackendMainWindowMock::GetWindowPosition() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, GetWindowPosition(), glm::ivec2);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetWindowPosition(), glm::ivec2);
        return WindowPosition;
    }

    glm::ivec2 BackendMainWindowMock::GetPositionOffset() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, GetPositionOffset(), glm::ivec2);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetPositionOffset(), glm::ivec2);
        return PositionOffset;
    }

    void BackendMainWindowMock::SetWindowSize(glm::ivec2 size)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(SetWindowSize(size));
        WindowSize = size;
    }

    glm::ivec2 BackendMainWindowMock::GetWindowSize() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, GetWindowSize(), glm::ivec2);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetWindowSize(), glm::ivec2);
        return WindowSize;
    }

    void BackendMainWindowMock::SetRenderSize(glm::ivec2 size)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(SetRenderSize(size));
        WindowSize = size + SizeOffset;
        //RenderSize = size;
    }
    
    glm::ivec2 BackendMainWindowMock::GetRenderSize() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, GetRenderSize(), glm::ivec2);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetRenderSize(), glm::ivec2);
        //return RenderSize;
        return WindowSize - SizeOffset;
    }

    bool BackendMainWindowMock::IsClosed() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, IsClosed(), bool);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(IsClosed(), bool);
        return Closed;
    }

    void BackendMainWindowMock::Close()
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(Close());

        for(int i = 0; i < OnCloseListeners.size(); i++)
        {
            if(OnCloseListeners[i].second)
                OnCloseListeners[i].first();

            if(ClosingAborted)
            {
                ClosingAborted = false;
                return;
            }
        }
        
        Closed = true;
    }
    
    void BackendMainWindowMock::AbortClosing()
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(AbortClosing());
        ClosingAborted = true;
    }

    int BackendMainWindowMock::AddOnCloseEvent(std::function<void()> func)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, AddOnCloseEvent(std::function<void()>), int, FO_ANY);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(AddOnCloseEvent(func), int);
        
        OnCloseListeners.push_back(std::pair<std::function<void()>, bool>(func, true));
        return OnCloseListeners.size() - 1;
    }

    void BackendMainWindowMock::RemoveOnCloseEvent(int index)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(RemoveOnCloseEvent(index));
        
        if(index >= 0 && index < OnCloseListeners.size())
            OnCloseListeners[index].second = false;
    }

    void BackendMainWindowMock::SetTitle(std::wstring title)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(SetTitle(title));
        
        Title = title;
    }

    std::wstring BackendMainWindowMock::GetTitle() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, GetTitle(), std::wstring);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetTitle(), std::wstring);
        return Title;
    }

    void BackendMainWindowMock::SetIcon(const ssGUI::Backend::BackendImageInterface& iconImage)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(SetIcon(iconImage));
        IconImage = &const_cast<ssGUI::Backend::BackendImageInterface&>(iconImage);
    }

    void BackendMainWindowMock::SetVisible(bool visible)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(SetVisible(visible));
        Visible = visible;
    }

    bool BackendMainWindowMock::IsVisible() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, IsVisible(), bool);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(IsVisible(), bool);
        return Visible;
    }

    void BackendMainWindowMock::SetVSync(bool vSync)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(SetVSync(vSync));
        VSync = vSync;
    }

    bool BackendMainWindowMock::IsVSync() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, IsVSync(), bool);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(IsVSync(), bool);
        return VSync;
    }

    void BackendMainWindowMock::SetFocus(bool focus, bool externalByUser)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(SetFocus(focus, externalByUser));
        Focused = focus;
        FocusSetByExternal = externalByUser;
        
        if(UnderlyingInterface != nullptr)
            return;
        
        for(int i = 0; i < FocusChangedListeners.size(); i++)
        {
            if(FocusChangedListeners[i].second)
                FocusChangedListeners[i].first(focus);
        }
    }
    
    bool BackendMainWindowMock::IsFocused() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, IsFocused(), bool);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(IsFocused(), bool);
        return Focused;
    }

    int BackendMainWindowMock::AddFocusChangedByUserEvent(std::function<void(bool focused)> func)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, AddFocusChangedByUserEvent(std::function<void(bool focused)>), int, FO_ANY);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(AddFocusChangedByUserEvent(func), int);
        
        FocusChangedListeners.push_back(std::pair<std::function<void(bool)>, bool>(func, true));
        return FocusChangedListeners.size() - 1;
    }

    void BackendMainWindowMock::RemoveFocusChangedByUserEvent(int index)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(RemoveFocusChangedByUserEvent(index));
        
        if(index >= 0 && index < FocusChangedListeners.size())
            FocusChangedListeners[index].second = false;
    }

    void BackendMainWindowMock::SetMSAA(int level)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(SetMSAA(level));
        MSAA = level;
    }

    int BackendMainWindowMock::GetMSAA() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, GetMSAA(), int);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetMSAA(), int);
        return MSAA;
    }

    void BackendMainWindowMock::SetTitlebar(bool titlebar)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(SetTitlebar(titlebar));
        Titlebar = titlebar;
    }

    bool BackendMainWindowMock::HasTitlebar() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, HasTitlebar(), bool);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(HasTitlebar(), bool);
        return Titlebar;
    }

    void BackendMainWindowMock::SetResizable(bool resizable)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(SetResizable(resizable));
        Resizable = resizable;
    }

    bool BackendMainWindowMock::IsResizable() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, IsResizable(), bool);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(IsResizable(), bool);
        return Resizable;
    }

    void BackendMainWindowMock::SetCloseButton(bool closeButton)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(SetCloseButton(closeButton));
        CloseButton = closeButton;
    }

    bool BackendMainWindowMock::HasCloseButton() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, HasCloseButton(), bool);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(HasCloseButton(), bool);
        return CloseButton;
    }

    void BackendMainWindowMock::SetWindowMode(ssGUI::Enums::WindowMode windowMode)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(SetWindowMode(windowMode));
        WindowMode = windowMode;
    }

    ssGUI::Enums::WindowMode BackendMainWindowMock::GetWindowMode() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, GetWindowMode(), ssGUI::Enums::WindowMode);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetWindowMode(), ssGUI::Enums::WindowMode);
        return WindowMode;
    }

    bool BackendMainWindowMock::SetGLContext()
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, SetGLContext(), bool);
        SSGUI_MOCK_PASSTHROUGH(SetGLContext());
        return true;
    }
    
    BackendMainWindowInterface* BackendMainWindowMock:: Clone()
    {
        return new BackendMainWindowMock(*this);
    }

    void* BackendMainWindowMock::GetRawHandle()
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, GetRawHandle(), void*);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetRawHandle(), void*);
        return nullptr;
    }
}

}