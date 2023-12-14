#include "ssGUI/Backend/Template/BackendSystemInputTemplate.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendSystemInputTemplate::BackendSystemInputTemplate(BackendSystemInputTemplate const& other)
    {
    
    }

    BackendSystemInputTemplate::BackendSystemInputTemplate()
    {
        ssGUI_WARNING(ssGUI_BACKEND_TAG, "BackendSystemInputTemplate is being created, is this intended?");
    }

    BackendSystemInputTemplate::~BackendSystemInputTemplate()
    {

    }
    
    void BackendSystemInputTemplate::UpdateInput()
    {

    }

    const std::vector<ssGUI::Enums::GenericInput>& BackendSystemInputTemplate::GetLastButtonAndKeyPresses()
    {
        std::vector<ssGUI::Enums::GenericInput> t;
        return t;
    }
    
    const std::vector<ssGUI::Enums::GenericInput>& BackendSystemInputTemplate::GetCurrentButtonAndKeyPresses()
    {
        std::vector<ssGUI::Enums::GenericInput> t;
        return t;
    }

    bool BackendSystemInputTemplate::IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::GenericInput input) const
    {
        return true;
    }

    bool BackendSystemInputTemplate::IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::GenericInput input) const
    {
        return true;
    }

    glm::ivec2 BackendSystemInputTemplate::GetLastMousePosition(ssGUI::MainWindow* mainWindow) const
    {
        return glm::ivec2();
    }
    
    glm::ivec2 BackendSystemInputTemplate::GetCurrentMousePosition(ssGUI::MainWindow* mainWindow) const
    {
        return glm::ivec2();
    }
    
    void BackendSystemInputTemplate::SetMousePosition(glm::ivec2 position, ssGUI::MainWindow* mainWindow)
    {

    }

    bool BackendSystemInputTemplate::GetLastMouseButton(ssGUI::Enums::MouseButton button) const
    {
        return true;
    }
    
    bool BackendSystemInputTemplate::GetCurrentMouseButton(ssGUI::Enums::MouseButton button) const
    {
        return true;
    }

    glm::vec2 BackendSystemInputTemplate::GetCurrentMouseScrollDelta() const
    {
        return glm::vec2();
    }

    const std::vector<ssGUI::RealtimeInputInfo>& BackendSystemInputTemplate::GetLastRealtimeInputs() const
    {
        std::vector<ssGUI::RealtimeInputInfo> t;
        return t;
    }

    const std::vector<ssGUI::RealtimeInputInfo>& BackendSystemInputTemplate::GetCurrentRealtimeInputs() const
    {
        std::vector<ssGUI::RealtimeInputInfo> t;
        return t;
    }

    std::wstring BackendSystemInputTemplate::GetTextInput() const
    {
        return L"";
    }
    
    void BackendSystemInputTemplate::SetCursorType(ssGUI::Enums::CursorType cursorType)
    {

    }

    ssGUI::Enums::CursorType BackendSystemInputTemplate::GetCursorType() const
    {
        return ssGUI::Enums::CursorType::NORMAL;
    }

    void BackendSystemInputTemplate::CreateCustomCursor(ssGUI::ImageData* customCursor, std::string cursorName, glm::ivec2 cursorSize, glm::ivec2 hotspot)
    {

    }
    
    void BackendSystemInputTemplate::SetCurrentCustomCursor(std::string cursorName)
    {

    }

    void BackendSystemInputTemplate::GetCurrentCustomCursor(ssGUI::ImageData& customCursor, glm::ivec2& hotspot)
    {

    }

    std::string BackendSystemInputTemplate::GetCurrentCustomCursorName()
    {
        return "";
    }
    
    void BackendSystemInputTemplate::GetCustomCursor(ssGUI::ImageData& customCursor, std::string cursorName, glm::ivec2& hotspot)
    {

    }

    bool BackendSystemInputTemplate::HasCustomCursor(std::string cursorName)
    {
        return true;
    }

    void BackendSystemInputTemplate::UpdateCursor()
    {

    }
    
    int BackendSystemInputTemplate::AddRawEventHandler(std::function<bool(ssGUI::Backend::BackendMainWindowInterface*, void*)> handler)
    {
        return 0;
    }
    
    void BackendSystemInputTemplate::RemoveRawEventHandler(int id)
    {
    
    }
    
    void BackendSystemInputTemplate::ClearRawEventHandler()
    {
    
    }

    bool BackendSystemInputTemplate::ClearClipboard()
    {
        return true;
    }

    bool BackendSystemInputTemplate::ClipbaordHasText()
    {
        return true;
    }
    
    bool BackendSystemInputTemplate::ClipbaordHasImage()
    {
        return true;
    }

    bool BackendSystemInputTemplate::SetClipboardImage(const ssGUI::ImageData& imgData)
    {
        return true;
    }
    
    bool BackendSystemInputTemplate::SetClipboardText(const std::wstring& str)
    {
        return true;
    }
    
    bool BackendSystemInputTemplate::GetClipboardImage(ssGUI::ImageData& imgData)
    {
        return true;
    }

    bool BackendSystemInputTemplate::GetClipboardText(std::wstring& str)
    {
        return true;
    }

    uint64_t BackendSystemInputTemplate::GetElapsedTime() const
    {
        return 0;
    }
}

}