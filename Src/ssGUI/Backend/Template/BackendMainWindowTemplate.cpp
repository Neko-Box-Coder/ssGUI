#include "ssGUI/Backend/Template/BackendMainWindowTemplate.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendMainWindowTemplate::BackendMainWindowTemplate(BackendMainWindowTemplate const& other)
    {
    
    }

    BackendMainWindowTemplate::BackendMainWindowTemplate()
    {
        ssGUI_WARNING(ssGUI_TAG_BACKEND, "BackendMainWindowTemplate is being created, is this intended?");
    }

    BackendMainWindowTemplate::~BackendMainWindowTemplate()
    {

    }
    
    void BackendMainWindowTemplate::SetWindowPosition(glm::ivec2 pos)
    {

    }

    glm::ivec2 BackendMainWindowTemplate::GetWindowPosition() const
    {
        return glm::ivec2();
    }

    void BackendMainWindowTemplate::GetDecorationOffsets(   glm::ivec2& topLeft, 
                                                            glm::ivec2& bottomRight) const
    {
    
    }

    void BackendMainWindowTemplate::SetWindowSize(glm::ivec2 size)
    {

    }

    glm::ivec2 BackendMainWindowTemplate::GetWindowSize() const
    {
        return glm::ivec2();
    }

    void BackendMainWindowTemplate::SetRenderSize(glm::ivec2 size)
    {
    
    }
    
    glm::ivec2 BackendMainWindowTemplate::GetRenderSize() const
    {
        return glm::ivec2();
    }

    bool BackendMainWindowTemplate::IsClosed() const
    {
        return WindowClosed;
    }

    void BackendMainWindowTemplate::Close()
    {
        for(int i = 0; i < OnCloseCallback.size(); i++)
        {
            if(OnCloseCallback[i] != nullptr)
                OnCloseCallback[i]();

            if(WindowClosingAborted)
            {
                WindowClosingAborted = false;
                return;
            }
        }
        
        WindowClosed = true;
        DestroyWindow();
    }
    
    void BackendMainWindowTemplate::AbortClosing()
    {
        WindowClosingAborted = true;
    }

    int BackendMainWindowTemplate::AddOnCloseEvent(std::function<void()> func)
    {
        OnCloseCallback.push_back(func);
        return OnCloseCallback.size() - 1;
    }

    void BackendMainWindowTemplate::RemoveOnCloseEvent(int index)
    {
        if(index < 0 || index >= OnCloseCallback.size())
            return;

        OnCloseCallback.at(index) = nullptr;
    }

    void BackendMainWindowTemplate::SetTitle(std::u32string title)
    {
    
    }
    
    void BackendMainWindowTemplate::SetTitle(std::string title)
    {

    }

    void BackendMainWindowTemplate::GetTitle(std::u32string& title) const
    {
    
    }

    void BackendMainWindowTemplate::GetTitle(std::string& title) const
    {
    
    }

    void BackendMainWindowTemplate::SetIcon(const ssGUI::Backend::ImageInterface& iconImage)
    {

    }

    void BackendMainWindowTemplate::SetVisible(bool visible)
    {

    }

    bool BackendMainWindowTemplate::IsVisible() const
    {
        return true;
    }

    void BackendMainWindowTemplate::SetVSync(bool vSync)
    {

    }

    bool BackendMainWindowTemplate::IsVSync() const
    {
        return true;
    }

    void BackendMainWindowTemplate::SetFocus(bool focus, bool externalByUser)
    {
        if(externalByUser)
        {
            for(int i = 0; i < ExternalFocusChangedCallback.size(); i++)
            {
                if(ExternalFocusChangedCallback[i] != nullptr)
                    ExternalFocusChangedCallback[i](focus);
            }
        }
        else
        {
            //Set window focus...
        }
    }
    
    bool BackendMainWindowTemplate::IsFocused() const
    {
        return true;
    }

    int BackendMainWindowTemplate::AddFocusChangedByUserEvent(std::function<void(bool focused)> func)
    {
        ExternalFocusChangedCallback.push_back(func);
        return ExternalFocusChangedCallback.size() - 1;
    }

    void BackendMainWindowTemplate::RemoveFocusChangedByUserEvent(int id)
    {
        if(id < 0 || id >= ExternalFocusChangedCallback.size())
            return;

        ExternalFocusChangedCallback.at(id) = nullptr;
    }

    void BackendMainWindowTemplate::SetAntiAliasingLevel(int level)
    {

    }

    int BackendMainWindowTemplate::GetAntiAliasingLevel() const
    {
        return 0;
    }

    void BackendMainWindowTemplate::SetResizable(bool resizable)
    {

    }

    bool BackendMainWindowTemplate::IsResizable() const
    {
        return true;
    }

    void BackendMainWindowTemplate::SetCloseButton(bool closeButton)
    {

    }

    bool BackendMainWindowTemplate::HasCloseButton() const
    {
        return true;
    }

    void BackendMainWindowTemplate::SetWindowMode(Enums::WindowMode windowMode)
    {

    }

    Enums::WindowMode BackendMainWindowTemplate::GetWindowMode() const
    {
        return Enums::WindowMode::NORMAL;
    }

    bool BackendMainWindowTemplate::SetDrawingContext()
    {
        return true;
    }
    
    MainWindowInterface* BackendMainWindowTemplate:: Clone()
    {
        return nullptr;
    }

    void* BackendMainWindowTemplate::GetRawHandle() const
    {
        return nullptr;
    }
}

}