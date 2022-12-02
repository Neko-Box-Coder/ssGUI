#include "ssGUI/Backend/Template/BackendMainWindowTemplate.hpp"

#include "ssLogger/ssLog.hpp"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    BackendMainWindowTemplate::BackendMainWindowTemplate(BackendMainWindowTemplate const& other)
    {
    
    }

    BackendMainWindowTemplate::BackendMainWindowTemplate()
    {
        ssLOG_LINE("BackendMainWindowTemplate is being created, is this intended?");
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

    void BackendMainWindowTemplate::SyncPositionOffset()
    {

    }

    glm::ivec2 BackendMainWindowTemplate::GetPositionOffset() const
    {
        return glm::ivec2();
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
        return true;
    }

    void BackendMainWindowTemplate::Close()
    {

    }
    
    void BackendMainWindowTemplate::AbortClosing()
    {

    }

    int BackendMainWindowTemplate::AddOnCloseEvent(std::function<void()> func)
    {
        return 0;
    }

    void BackendMainWindowTemplate::RemoveOnCloseEvent(int index)
    {

    }

    void BackendMainWindowTemplate::SetTitle(std::wstring title)
    {

    }

    std::wstring BackendMainWindowTemplate::GetTitle() const
    {
        return L"";
    }

    void BackendMainWindowTemplate::SetIcon(const ssGUI::Backend::BackendImageInterface& iconImage)
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

    }
    
    bool BackendMainWindowTemplate::IsFocused() const
    {
        return true;
    }

    int BackendMainWindowTemplate::AddFocusChangedByUserEvent(std::function<void(bool focused)> func)
    {
        return 0;
    }

    void BackendMainWindowTemplate::RemoveFocusChangedByUserEvent(int index)
    {

    }

    void BackendMainWindowTemplate::SetMSAA(int level)
    {

    }

    int BackendMainWindowTemplate::GetMSAA() const
    {
        return 0;
    }

    void BackendMainWindowTemplate::SetTitlebar(bool titlebar)
    {

    }

    bool BackendMainWindowTemplate::HasTitlebar() const
    {
        return true;
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

    void BackendMainWindowTemplate::SetWindowMode(ssGUI::Enums::WindowMode WindowMode)
    {

    }

    ssGUI::Enums::WindowMode BackendMainWindowTemplate::GetWindowMode() const
    {
        return ssGUI::Enums::WindowMode::NORMAL;
    }

    bool BackendMainWindowTemplate::SetGLContext()
    {
        return true;
    }
    
    BackendMainWindowInterface* BackendMainWindowTemplate:: Clone()
    {
        return nullptr;
    }

    void* BackendMainWindowTemplate::GetRawHandle()
    {
        return nullptr;
    }
}

}