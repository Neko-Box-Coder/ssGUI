#include "ssGUI/Backend/SDL2/BackendMainWindowSDL2.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendMainWindowSDL2::BackendMainWindowSDL2(BackendMainWindowSDL2 const& other)
    {
    
    }

    BackendMainWindowSDL2::BackendMainWindowSDL2()
    {
        ssGUI_WARNING(ssGUI_BACKEND_TAG, "BackendMainWindowSDL2 is being created, is this intended?");
    }

    BackendMainWindowSDL2::~BackendMainWindowSDL2()
    {

    }
    
    void BackendMainWindowSDL2::SetWindowPosition(glm::ivec2 pos)
    {

    }

    glm::ivec2 BackendMainWindowSDL2::GetWindowPosition() const
    {
        return glm::ivec2();
    }

    void BackendMainWindowSDL2::GetDecorationOffsets(   glm::ivec2& topLeft, 
                                                            glm::ivec2& bottomRight) const
    {
    
    }

    void BackendMainWindowSDL2::SetWindowSize(glm::ivec2 size)
    {

    }

    glm::ivec2 BackendMainWindowSDL2::GetWindowSize() const
    {
        return glm::ivec2();
    }

    void BackendMainWindowSDL2::SetRenderSize(glm::ivec2 size)
    {
    
    }
    
    glm::ivec2 BackendMainWindowSDL2::GetRenderSize() const
    {
        return glm::ivec2();
    }

    bool BackendMainWindowSDL2::IsClosed() const
    {
        return true;
    }

    void BackendMainWindowSDL2::Close()
    {

    }
    
    void BackendMainWindowSDL2::AbortClosing()
    {

    }

    int BackendMainWindowSDL2::AddOnCloseEvent(std::function<void()> func)
    {
        return 0;
    }

    void BackendMainWindowSDL2::RemoveOnCloseEvent(int index)
    {

    }

    void BackendMainWindowSDL2::SetTitle(std::u32string title)
    {
    
    }
    
    void BackendMainWindowSDL2::SetTitle(std::string title)
    {

    }

    void BackendMainWindowSDL2::GetTitle(std::u32string& title) const
    {
    
    }

    void BackendMainWindowSDL2::GetTitle(std::string& title) const
    {
    
    }

    void BackendMainWindowSDL2::SetIcon(const ssGUI::Backend::BackendImageInterface& iconImage)
    {

    }

    void BackendMainWindowSDL2::SetVisible(bool visible)
    {

    }

    bool BackendMainWindowSDL2::IsVisible() const
    {
        return true;
    }

    void BackendMainWindowSDL2::SetVSync(bool vSync)
    {

    }

    bool BackendMainWindowSDL2::IsVSync() const
    {
        return true;
    }

    void BackendMainWindowSDL2::SetFocus(bool focus, bool externalByUser)
    {

    }
    
    bool BackendMainWindowSDL2::IsFocused() const
    {
        return true;
    }

    int BackendMainWindowSDL2::AddFocusChangedByUserEvent(std::function<void(bool focused)> func)
    {
        return 0;
    }

    void BackendMainWindowSDL2::RemoveFocusChangedByUserEvent(int id)
    {

    }

    void BackendMainWindowSDL2::SetAntiAliasingLevel(int level)
    {

    }

    int BackendMainWindowSDL2::GetAntiAliasingLevel() const
    {
        return 0;
    }

    void BackendMainWindowSDL2::SetResizable(bool resizable)
    {

    }

    bool BackendMainWindowSDL2::IsResizable() const
    {
        return true;
    }

    void BackendMainWindowSDL2::SetCloseButton(bool closeButton)
    {

    }

    bool BackendMainWindowSDL2::HasCloseButton() const
    {
        return true;
    }

    void BackendMainWindowSDL2::SetWindowMode(ssGUI::Enums::WindowMode windowMode)
    {

    }

    ssGUI::Enums::WindowMode BackendMainWindowSDL2::GetWindowMode() const
    {
        return ssGUI::Enums::WindowMode::NORMAL;
    }

    bool BackendMainWindowSDL2::SetDrawingContext()
    {
        return true;
    }
    
    BackendMainWindowInterface* BackendMainWindowSDL2:: Clone()
    {
        return nullptr;
    }

    void* BackendMainWindowSDL2::GetRawHandle() const
    {
        return nullptr;
    }
}

}