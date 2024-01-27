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
    
    void BackendSystemInputTemplate::UpdateInput(MainWindowInterface** mainWindows, int count)
    {

    }

    const std::vector<Enums::GenericInput>& BackendSystemInputTemplate::GetLastInputs() const
    {
        return LastInputs;
    }
    
    const std::vector<Enums::GenericInput>& BackendSystemInputTemplate::GetCurrentInputs() const
    {
        return CurrentInputs;
    }

    bool BackendSystemInputTemplate::IsInputExistLastFrame(Enums::GenericInput input) const
    {
        return !LastInputs.empty();
    }

    bool BackendSystemInputTemplate::IsInputExistCurrentFrame(Enums::GenericInput input) const
    {
        return CurrentInputs.empty();
    }

    glm::ivec2 
    BackendSystemInputTemplate::GetLastMousePosition(MainWindowInterface* mainWindow) const
    {
        return glm::ivec2();
    }
    
    glm::ivec2 
    BackendSystemInputTemplate::GetCurrentMousePosition(MainWindowInterface* mainWindow) const
    {
        return glm::ivec2();
    }
    
    void BackendSystemInputTemplate::SetMousePosition(  glm::ivec2 position, 
                                                        MainWindowInterface* mainWindow)
    {

    }

    glm::vec2 BackendSystemInputTemplate::GetCurrentMouseScrollDelta() const
    {
        return glm::vec2();
    }

    const std::vector<RealtimeInputInfo>& BackendSystemInputTemplate::GetLastRealtimeInputs() const
    {
        return LastRealtimeInputs;
    }

    const std::vector<RealtimeInputInfo>& BackendSystemInputTemplate::GetCurrentRealtimeInputs() const
    {
        return CurrentRealtimeInputs;
    }

    void BackendSystemInputTemplate::GetTextInput(std::u32string& outText) const
    {
    
    }
    
    void BackendSystemInputTemplate::GetTextInput(std::string& outText) const
    {
    
    }
    
    void BackendSystemInputTemplate::SetCursorType(Enums::CursorType cursorType)
    {

    }

    Enums::CursorType BackendSystemInputTemplate::GetCursorType() const
    {
        return Enums::CursorType::NORMAL;
    }

    void BackendSystemInputTemplate::CreateCustomCursor(ImageInterface* customCursor, 
                                                        std::string cursorName, 
                                                        glm::ivec2 cursorSize, 
                                                        glm::ivec2 hotspot)
    {

    }
    
    void BackendSystemInputTemplate::SetCurrentCustomCursor(std::string cursorName)
    {

    }

    void BackendSystemInputTemplate::GetCurrentCustomCursor(ImageInterface& customCursor, 
                                                            glm::ivec2& hotspot) const
    {

    }

    std::string BackendSystemInputTemplate::GetCurrentCustomCursorName() const
    {
        return "";
    }
    
    void BackendSystemInputTemplate::GetCustomCursor(   ImageInterface& customCursor, 
                                                        std::string cursorName, 
                                                        glm::ivec2& hotspot) const
    {

    }

    bool BackendSystemInputTemplate::HasCustomCursor(std::string cursorName) const
    {
        return true;
    }

    void BackendSystemInputTemplate::UpdateCursor()
    {

    }
    
    int BackendSystemInputTemplate::AddRawEventHandler(std::function<bool(  MainWindowInterface*, 
                                                                            void*)> handler)
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

    bool BackendSystemInputTemplate::ClipbaordHasText() const
    {
        return true;
    }
    
    bool BackendSystemInputTemplate::ClipbaordHasImage() const
    {
        return true;
    }

    bool BackendSystemInputTemplate::SetClipboardImage(const ImageInterface& imgData)
    {
        return true;
    }
    
    bool BackendSystemInputTemplate::SetClipboardText(const std::u32string& str)
    {
        return true;
    }
    
    bool BackendSystemInputTemplate::SetClipboardText(const std::string& str)
    {
        return true;
    }

    
    bool BackendSystemInputTemplate::GetClipboardImage(ImageInterface& imgData) const
    {
        return true;
    }

    bool BackendSystemInputTemplate::GetClipboardText(std::u32string& str) const
    {
        return true;
    }
    
    bool BackendSystemInputTemplate::GetClipboardText(std::string& str) const
    {
        return true;
    }

    uint64_t BackendSystemInputTemplate::GetElapsedTime() const
    {
        return 0;
    }
}

}