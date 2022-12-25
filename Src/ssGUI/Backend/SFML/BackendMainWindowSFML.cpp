#include "ssGUI/Backend/SFML/BackendMainWindowSFML.hpp"
#include "ssLogger/ssLog.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendMainWindowSFML::BackendMainWindowSFML(BackendMainWindowSFML const& other) : 
        CurrentWindow(sf::VideoMode(sf::Vector2u(other.CurrentWindow.getSize().x, other.CurrentWindow.getSize().y)), other.GetTitle()), 
        Visible(other.IsVisible()), VSync(other.IsVSync()), Closed(other.IsClosed()), CurrentWindowMode(other.GetWindowMode()), Titlebar(other.HasTitlebar()), 
        Resizable(other.IsResizable()), CloseButton(other.HasCloseButton()), Title(other.GetTitle()), IsClosingAborted(other.IsClosingAborted),
        PositionOffset(other.GetPositionOffset())
    {
        //TODO : finish backend copy constructor and assignment and clone function
        ssLOG_LINE("Not implemented");
        ssLOG_EXIT_PROGRAM();
    }
    
    void BackendMainWindowSFML::ResetWindow(ssGUI::Enums::WindowMode mode, bool resize, bool titlebar, bool canClose, int msaa)
    {
        int newStyle = 0;

        if(mode == ssGUI::Enums::WindowMode::NORMAL && titlebar)
        {
            newStyle = sf::Style::Titlebar;
            newStyle |= resize ? sf::Style::Resize : newStyle;
            newStyle |= canClose ? sf::Style::Close : newStyle;
        }
        else if(mode != ssGUI::Enums::WindowMode::FULLSCREEN)
            newStyle = sf::Style::None;
        else
            newStyle = sf::Style::Fullscreen;

        sf::ContextSettings settings;
        settings.antialiasingLevel = msaa;
        settings.depthBits = 24;
        settings.majorVersion = 3;
        settings.minorVersion = 0;

        CurrentWindow.create(sf::VideoMode(sf::Vector2u(CurrentWindow.getSize().x, CurrentWindow.getSize().y)), GetTitle(), newStyle, settings);
    }

    void BackendMainWindowSFML::SyncPositionOffset()
    {
        glm::ivec2 cursorInSceenSpace;
        glm::ivec2 cursorInWindowSpace;

        cursorInSceenSpace = glm::ivec2(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
        cursorInWindowSpace = glm::ivec2(sf::Mouse::getPosition(CurrentWindow).x, sf::Mouse::getPosition(CurrentWindow).y);

        PositionOffset = cursorInSceenSpace - GetWindowPosition() - cursorInWindowSpace;

        /*
        std::cout<<"cursorInSceenSpace: "<<cursorInSceenSpace.x<<", "<<cursorInSceenSpace.y<<"\n";
        std::cout<<"cursorInWindowSpace: "<<cursorInWindowSpace.x<<", "<<cursorInWindowSpace.y<<"\n";
        std::cout<<"GetPosition: "<<GetPosition().x<<", "<<GetPosition().y<<"\n";
        std::cout<<"GetPositionOffset: "<<PositionOffset.x<<", "<<PositionOffset.y<<"\n\n";
        */
    }

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

    BackendMainWindowSFML::~BackendMainWindowSFML()
    {
        ssGUI::Backend::BackendManager::RemoveMainWindowInterface(static_cast<ssGUI::Backend::BackendMainWindowInterface*>(this));
    }

    glm::ivec2 BackendMainWindowSFML::GetPositionOffset() const
    {
        return PositionOffset;
    }

    void BackendMainWindowSFML::SetWindowPosition(glm::ivec2 pos)
    {
        CurrentWindow.setPosition(sf::Vector2i(pos.x, pos.y));
    }

    glm::ivec2 BackendMainWindowSFML::GetWindowPosition() const
    {
        sf::Vector2i curPos = CurrentWindow.getPosition();
        return glm::ivec2(curPos.x, curPos.y);
    }

    void BackendMainWindowSFML::SetWindowSize(glm::ivec2 size)
    {
        //ssLOG_LINE("SFML doesn't support setting window size, setting render size instead");
        SetRenderSize(size);
    }

    glm::ivec2 BackendMainWindowSFML::GetWindowSize() const
    {
        //ssLOG_LINE("SFML doesn't support setting window size, getting render size instead");
        return GetRenderSize();
    }
    
    void BackendMainWindowSFML::SetRenderSize(glm::ivec2 size)
    {
        CurrentWindow.setSize(sf::Vector2u(size.x, size.y));
        
        //View is now set in BackendSystemInputSFML
        // CurrentWindow.setView(sf::View(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f((float)size.x, (float)size.y))));
    }
    
    glm::ivec2 BackendMainWindowSFML::GetRenderSize() const
    {
        sf::Vector2u curSize = CurrentWindow.getSize();
        return glm::ivec2(curSize.x, curSize.y);
    }

    bool BackendMainWindowSFML::IsClosed() const
    {
        return Closed;
    }

    void BackendMainWindowSFML::Close()
    {
        for(int i = 0; i < OnCloseCallback.size(); i++)
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

        CurrentWindow.close();
    }

    void BackendMainWindowSFML::AbortClosing()
    {
        IsClosingAborted = true;
    }

    int BackendMainWindowSFML::AddOnCloseEvent(std::function<void()> func)
    {
        OnCloseCallback.push_back(func);
        return OnCloseCallback.size() - 1;
    }
    
    void BackendMainWindowSFML::RemoveOnCloseEvent(int index)
    {
        OnCloseCallback[index] = nullptr;
    }

    void BackendMainWindowSFML::SetTitle(std::wstring title)
    {
        Title = title;
        CurrentWindow.setTitle(Title);
    }

    std::wstring BackendMainWindowSFML::GetTitle() const
    {
        return Title;
    }

    void BackendMainWindowSFML::SetIcon(const ssGUI::Backend::BackendImageInterface& iconImage)
    {
        ssGUI::Backend::BackendImageSFML& castedIcon = (ssGUI::Backend::BackendImageSFML&)iconImage;
        sf::Image sfImg = (*static_cast<sf::Texture*>(castedIcon.GetRawHandle())).copyToImage();
        
        CurrentWindow.setIcon(sf::Vector2u(sfImg.getSize().x, sfImg.getSize().y), sfImg.getPixelsPtr());
    }

    void BackendMainWindowSFML::SetVisible(bool visible)
    {
        CurrentWindow.setVisible(visible);
        Visible = visible;
    }

    bool BackendMainWindowSFML::IsVisible() const
    {
        return Visible;
    }

    void BackendMainWindowSFML::SetVSync(bool vSync)
    {
        CurrentWindow.setVerticalSyncEnabled(vSync);
        VSync = vSync;
    }

    bool BackendMainWindowSFML::IsVSync() const
    {
        return VSync;
    }

    bool BackendMainWindowSFML::IsFocused() const
    {
        return CurrentWindow.hasFocus();
    }

    void BackendMainWindowSFML::SetFocus(bool focus, bool externalByUser)
    {
        if(externalByUser)
        {
            for(int i = 0; i < ExternalFocusChangedCallback.size(); i++)
            {
                if(ExternalFocusChangedCallback[i] != nullptr)
                    ExternalFocusChangedCallback[i](focus);
            }
        }
        else if(focus)
            CurrentWindow.requestFocus();
    }

    int BackendMainWindowSFML::AddFocusChangedByUserEvent(std::function<void(bool focused)> func)
    {
        ExternalFocusChangedCallback.push_back(func);
        return ExternalFocusChangedCallback.size() - 1;
    }

    void BackendMainWindowSFML::RemoveFocusChangedByUserEvent(int index)
    {
        ExternalFocusChangedCallback[index] = nullptr;
    }

    int BackendMainWindowSFML::GetMSAA() const
    {
        return CurrentWindow.getSettings().antialiasingLevel;
    }

    void BackendMainWindowSFML::SetMSAA(int level)
    {
        ResetWindow(GetWindowMode(), IsResizable(), HasTitlebar(), HasCloseButton(), level);
    }

    void BackendMainWindowSFML::SetTitlebar(bool titlebar)
    {
        ResetWindow(GetWindowMode(), IsResizable(), titlebar, HasCloseButton(), GetMSAA());
        Titlebar = titlebar;
    }

    bool BackendMainWindowSFML::HasTitlebar() const
    {
        return GetWindowMode() == ssGUI::Enums::WindowMode::NORMAL ? Titlebar : false;
    }

    void BackendMainWindowSFML::SetResizable(bool resizable)
    {
        ResetWindow(GetWindowMode(), resizable, HasTitlebar(), HasCloseButton(), GetMSAA());
        Resizable = resizable;
    }

    bool BackendMainWindowSFML::IsResizable() const
    {
        return GetWindowMode() == ssGUI::Enums::WindowMode::NORMAL ? Resizable : false;
    }

    void BackendMainWindowSFML::SetCloseButton(bool closeButton)
    {
        ResetWindow(GetWindowMode(), IsResizable(), HasTitlebar(), closeButton, GetMSAA());
        CloseButton = closeButton;
    }

    bool BackendMainWindowSFML::HasCloseButton() const
    {
        return GetWindowMode() == ssGUI::Enums::WindowMode::NORMAL ? CloseButton : false;
    }

    void BackendMainWindowSFML::SetWindowMode(ssGUI::Enums::WindowMode WindowMode)
    {
        ResetWindow(WindowMode, IsResizable(), HasTitlebar(), HasCloseButton(), GetMSAA());
        CurrentWindowMode = WindowMode;
    }

    ssGUI::Enums::WindowMode BackendMainWindowSFML::GetWindowMode() const
    {
        return CurrentWindowMode;
    }

    bool BackendMainWindowSFML::SetGLContext()
    {
        return CurrentWindow.setActive();
    }

    ssGUI::Backend::BackendMainWindowInterface* BackendMainWindowSFML::Clone()
    {
        return new BackendMainWindowSFML(*this);
    }

    void* BackendMainWindowSFML::GetRawHandle()
    {
        return static_cast<void*>(&CurrentWindow);
    }
}

}

