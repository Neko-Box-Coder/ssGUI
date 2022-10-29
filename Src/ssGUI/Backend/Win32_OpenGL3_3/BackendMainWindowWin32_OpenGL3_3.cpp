#include "ssGUI/Backend/Win32_OpenGL3_3/BackendMainWindowWin32_OpenGL3_3.hpp"


namespace ssGUI::Backend
{
    BackendMainWindowWin32_OpenGL3_3::BackendMainWindowWin32_OpenGL3_3()
    {
        //ssGUI::Backend::BackendManager::AddMainWindowInterface(static_cast<ssGUI::Backend::BackendMainWindowInterface*>(this));
    }

    BackendMainWindowWin32_OpenGL3_3::~BackendMainWindowWin32_OpenGL3_3()
    {
        //ssGUI::Backend::BackendManager::RemoveMainWindowInterface(static_cast<ssGUI::Backend::BackendMainWindowInterface*>(this));
    }
    
    void BackendMainWindowWin32_OpenGL3_3::SetPosition(glm::ivec2 pos)
    {
        //CurrentWindow.setPosition(sf::Vector2i(pos.x, pos.y));
    }

    glm::ivec2 BackendMainWindowWin32_OpenGL3_3::GetPosition() const
    {
        //sf::Vector2i curPos = CurrentWindow.getPosition();
        //return glm::ivec2(curPos.x, curPos.y);
        return glm::ivec2();
    }

    void BackendMainWindowWin32_OpenGL3_3::SetSize(glm::ivec2 size)
    {
        //CurrentWindow.setSize(sf::Vector2u(size.x, size.y));

        //View is now set in BackendSystemInputSFML
        // CurrentWindow.setView(sf::View(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f((float)size.x, (float)size.y))));
    }

    glm::ivec2 BackendMainWindowWin32_OpenGL3_3::GetSize() const
    {
        //sf::Vector2u curSize = CurrentWindow.getSize();
        //return glm::ivec2(curSize.x, curSize.y);
        return glm::ivec2();
    }

    bool BackendMainWindowWin32_OpenGL3_3::IsClosed() const
    {
        //return Closed;
        return true;
    }

    void BackendMainWindowWin32_OpenGL3_3::Close()
    {
        /*for(int i = 0; i < OnCloseCallback.size(); i++)
        {
            if(OnCloseCallback[i] != nullptr)
                OnCloseCallback[i]();

            if(IsClosingAborted)
            {
                IsClosingAborted = false;
                return;
            }
        }

        Closed = true;

        CurrentWindow.close();*/
    }

    void BackendMainWindowWin32_OpenGL3_3::AbortClosing()
    {
        //IsClosingAborted = true;
    }

    int BackendMainWindowWin32_OpenGL3_3::AddOnCloseEvent(std::function<void()> func)
    {
        //OnCloseCallback.push_back(func);
        //return OnCloseCallback.size() - 1;
        return 0;
    }

    void BackendMainWindowWin32_OpenGL3_3::RemoveOnCloseEvent(int index)
    {
        //OnCloseCallback[index] = nullptr;
    }

    void BackendMainWindowWin32_OpenGL3_3::SetTitle(std::wstring title)
    {
        //Title = title;
        //CurrentWindow.setTitle(Title);
    }

    std::wstring BackendMainWindowWin32_OpenGL3_3::GetTitle() const
    {
        //return Title;

        return L"";
    }

    void BackendMainWindowWin32_OpenGL3_3::SetIcon(const ssGUI::Backend::BackendImageInterface& iconImage)
    {
        //ssGUI::Backend::BackendImageSFML& castedIcon = (ssGUI::Backend::BackendImageSFML&)iconImage;
        //sf::Image sfImg = (*static_cast<sf::Texture*>(castedIcon.GetRawHandle())).copyToImage();

        //CurrentWindow.setIcon(sf::Vector2u(sfImg.getSize().x, sfImg.getSize().y), sfImg.getPixelsPtr());
    }

    void BackendMainWindowWin32_OpenGL3_3::SetVisible(bool visible)
    {
        //CurrentWindow.setVisible(visible);
        //Visible = visible;
    }

    bool BackendMainWindowWin32_OpenGL3_3::IsVisible() const
    {
        //return Visible;
        return true;
    }

    void BackendMainWindowWin32_OpenGL3_3::SetVSync(bool vSync)
    {
        //CurrentWindow.setVerticalSyncEnabled(vSync);
        //VSync = vSync;
    }

    bool BackendMainWindowWin32_OpenGL3_3::IsVSync() const
    {
        //return VSync;
        return true;
    }

    bool BackendMainWindowWin32_OpenGL3_3::IsFocused() const
    {
        //return CurrentWindow.hasFocus();
        return true;
    }

    void BackendMainWindowWin32_OpenGL3_3::SetFocus(bool focus, bool externalByUser)
    {
        /*if(externalByUser)
        {
            for(int i = 0; i < ExternalFocusChangedCallback.size(); i++)
            {
                if(ExternalFocusChangedCallback[i] != nullptr)
                    ExternalFocusChangedCallback[i](focus);
            }
        }
        else if(focus)
            CurrentWindow.requestFocus();*/
    }

    int BackendMainWindowWin32_OpenGL3_3::AddFocusChangedByUserEvent(std::function<void(bool focused)> func)
    {
        //ExternalFocusChangedCallback.push_back(func);
        //return ExternalFocusChangedCallback.size() - 1;
        return 0;
    }

    void BackendMainWindowWin32_OpenGL3_3::RemoveFocusChangedByUserEvent(int index)
    {
        //ExternalFocusChangedCallback[index] = nullptr;
    }

    int BackendMainWindowWin32_OpenGL3_3::GetMSAA() const
    {
        //return CurrentWindow.getSettings().antialiasingLevel;
        return 0;
    }

    void BackendMainWindowWin32_OpenGL3_3::SetMSAA(int level)
    {
        //ResetWindow(GetWindowMode(), IsResizable(), HasTitlebar(), HasCloseButton(), level);
    }

    void BackendMainWindowWin32_OpenGL3_3::SetTitlebar(bool titlebar)
    {
        //ResetWindow(GetWindowMode(), IsResizable(), titlebar, HasCloseButton(), GetMSAA());
        //Titlebar = titlebar;
    }

    bool BackendMainWindowWin32_OpenGL3_3::HasTitlebar() const
    {
        //return GetWindowMode() == ssGUI::Enums::WindowMode::NORMAL ? Titlebar : false;
        return true;
    }

    void BackendMainWindowWin32_OpenGL3_3::SetResizable(bool resizable)
    {
        //ResetWindow(GetWindowMode(), resizable, HasTitlebar(), HasCloseButton(), GetMSAA());
        //Resizable = resizable;
    }

    bool BackendMainWindowWin32_OpenGL3_3::IsResizable() const
    {
        //return GetWindowMode() == ssGUI::Enums::WindowMode::NORMAL ? Resizable : false;
        return true;
    }

    void BackendMainWindowWin32_OpenGL3_3::SetCloseButton(bool closeButton)
    {
        //ResetWindow(GetWindowMode(), IsResizable(), HasTitlebar(), closeButton, GetMSAA());
        //CloseButton = closeButton;
    }

    bool BackendMainWindowWin32_OpenGL3_3::HasCloseButton() const
    {
        //return GetWindowMode() == ssGUI::Enums::WindowMode::NORMAL ? CloseButton : false;
        return true;
    }

    void BackendMainWindowWin32_OpenGL3_3::SetWindowMode(ssGUI::Enums::WindowMode WindowMode)
    {
        //ResetWindow(WindowMode, IsResizable(), HasTitlebar(), HasCloseButton(), GetMSAA());
        //CurrentWindowMode = WindowMode;
    }

    ssGUI::Enums::WindowMode BackendMainWindowWin32_OpenGL3_3::GetWindowMode() const
    {
        //return CurrentWindowMode;
        ssGUI::Enums::WindowMode::NORMAL;
    }

    bool BackendMainWindowWin32_OpenGL3_3::SetGLContext()
    {
        //return CurrentWindow.setActive();
        return true;
    }

    ssGUI::Backend::BackendMainWindowInterface* BackendMainWindowWin32_OpenGL3_3::Clone()
    {
        //return new BackendMainWindowWin32_OpenGL3_3(*this);
        return nullptr;
    }

    void* BackendMainWindowWin32_OpenGL3_3::GetRawHandle()
    {
        //return static_cast<void*>(&CurrentWindow);
        return nullptr;
    }
}