#include "ssGUI/Backend/SFML/BackendMainWindowSFML.hpp"

namespace ssGUI::Backend
{
    BackendMainWindowSFML::BackendMainWindowSFML(BackendMainWindowSFML const& other) : 
        CurrentWindow(sf::VideoMode(other.CurrentWindow.getSize().x, other.CurrentWindow.getSize().y), other.GetTitle()), 
        Visible(other.IsVisible()), VSync(other.IsVSync()), Closed(other.IsClosed()), Titlebar(other.HasTitlebar()), 
        Resizable(other.IsResizable()), CloseButton(other.HasCloseButton()), Title(other.GetTitle()), IsClosingAborted(other.IsClosingAborted),
        PositionOffset(other.GetPositionOffset())
    {
        //TODO : finish backend copy constructor and assignment and clone function
    }
    
    void BackendMainWindowSFML::ResetWindow(bool resize, bool titlebar, bool canClose, int msaa)
    {
        int newStyle = 0;

        if(titlebar)
        {
            newStyle = sf::Style::Titlebar;
            newStyle |= resize ? sf::Style::Resize : newStyle;
            newStyle |= canClose ? sf::Style::Close : newStyle;
        }
        else
            newStyle = sf::Style::None;

        sf::ContextSettings settings;
        settings.antialiasingLevel = msaa;

        CurrentWindow.create(sf::VideoMode(CurrentWindow.getSize().x, CurrentWindow.getSize().y), GetTitle(), newStyle, settings);
    }

    void BackendMainWindowSFML::SyncPositionOffset()
    {
        glm::ivec2 cursorInSceenSpace;
        glm::ivec2 cursorInWindowSpace;

        cursorInSceenSpace = glm::ivec2(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
        cursorInWindowSpace = glm::ivec2(sf::Mouse::getPosition(CurrentWindow).x, sf::Mouse::getPosition(CurrentWindow).y);

        PositionOffset = cursorInSceenSpace - GetPosition() - cursorInWindowSpace;

        /*
        std::cout<<"cursorInSceenSpace: "<<cursorInSceenSpace.x<<", "<<cursorInSceenSpace.y<<"\n";
        std::cout<<"cursorInWindowSpace: "<<cursorInWindowSpace.x<<", "<<cursorInWindowSpace.y<<"\n";
        std::cout<<"GetPosition: "<<GetPosition().x<<", "<<GetPosition().y<<"\n";
        std::cout<<"GetPositionOffset: "<<PositionOffset.x<<", "<<PositionOffset.y<<"\n\n";
        */
    }

    BackendMainWindowSFML::BackendMainWindowSFML() : CurrentWindow(sf::VideoMode(800, 600), ""), Visible(false), VSync(false), Closed(false),
                                                        Titlebar(true), Resizable(true), CloseButton(true), Title(), IsClosingAborted(false), 
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

    void BackendMainWindowSFML::SetPosition(glm::ivec2 pos)
    {
        CurrentWindow.setPosition(sf::Vector2i(pos.x, pos.y));
    }

    glm::ivec2 BackendMainWindowSFML::GetPosition() const
    {
        sf::Vector2i curPos = CurrentWindow.getPosition();
        return glm::ivec2(curPos.x, curPos.y);
    }

    void BackendMainWindowSFML::SetSize(glm::ivec2 size)
    {
        CurrentWindow.setSize(sf::Vector2u(size.x, size.y));
    }

    glm::ivec2 BackendMainWindowSFML::GetSize() const
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

    void BackendMainWindowSFML::SetTitle(std::string title)
    {
        std::string Title = title;
        CurrentWindow.setTitle(Title);
    }

    std::string BackendMainWindowSFML::GetTitle() const
    {
        return Title;
    }

    void BackendMainWindowSFML::SetIcon(const ssGUI::Backend::BackendImageInterface& iconImage)
    {
        ssGUI::Backend::BackendImageSFML& castedIcon = (ssGUI::Backend::BackendImageSFML&)iconImage;
        sf::Image sfImg = (*castedIcon.GetGPUTextureP()).copyToImage();
        
        CurrentWindow.setIcon(sfImg.getSize().x, sfImg.getSize().y, sfImg.getPixelsPtr());
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

    void BackendMainWindowSFML::SetFocus(bool focus)
    {
        CurrentWindow.requestFocus();
    } 

    int BackendMainWindowSFML::GetMSAA() const
    {
        return CurrentWindow.getSettings().antialiasingLevel;
    }

    void BackendMainWindowSFML::SetMSAA(int level)
    {
        ResetWindow(IsResizable(), HasTitlebar(), HasCloseButton(), level);
    }

    void BackendMainWindowSFML::SetTitlebar(bool titlebar)
    {
        ResetWindow(IsResizable(), titlebar, HasCloseButton(), GetMSAA());
        Titlebar = titlebar;
    }

    bool BackendMainWindowSFML::HasTitlebar() const
    {
        return Titlebar;
    }

    void BackendMainWindowSFML::SetResizable(bool resizable)
    {
        ResetWindow(resizable, HasTitlebar(), HasCloseButton(), GetMSAA());
        Resizable = resizable;
    }

    bool BackendMainWindowSFML::IsResizable() const
    {
        return Resizable;
    }

    void BackendMainWindowSFML::SetCloseButton(bool closeButton)
    {
        ResetWindow(IsResizable(), HasTitlebar(), closeButton, GetMSAA());
        CloseButton = closeButton;
    }

    bool BackendMainWindowSFML::HasCloseButton() const
    {
        return CloseButton;
    }

    void* BackendMainWindowSFML::GetRawHandle()
    {
        return static_cast<void*>(&CurrentWindow);
    }

    ssGUI::Backend::BackendMainWindowInterface* BackendMainWindowSFML::Clone()
    {
        return new BackendMainWindowSFML(*this);
    }

}



