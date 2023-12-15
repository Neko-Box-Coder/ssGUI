#include "ssGUI/Backend/SDL2/BackendSystemInputSDL2.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendSystemInputSDL2::BackendSystemInputSDL2(BackendSystemInputSDL2 const& other)
    {
    
    }

    BackendSystemInputSDL2::BackendSystemInputSDL2()
    {
        ssGUI_WARNING(ssGUI_BACKEND_TAG, "BackendSystemInputSDL2 is being created, is this intended?");
    }

    BackendSystemInputSDL2::~BackendSystemInputSDL2()
    {

    }
    
    void BackendSystemInputSDL2::UpdateInput()
    {

    }

    std::vector<Enums::GenericInput> DummyInputList;
    
    const std::vector<Enums::GenericInput>& BackendSystemInputSDL2::GetLastInputs() const
    {
        return DummyInputList;
    }
    
    const std::vector<Enums::GenericInput>& BackendSystemInputSDL2::GetCurrentInputs() const
    {
        return DummyInputList;
    }

    bool BackendSystemInputSDL2::IsInputExistLastFrame(Enums::GenericInput input) const
    {
        return true;
    }

    bool BackendSystemInputSDL2::IsInputExistCurrentFrame(Enums::GenericInput input) const
    {
        return true;
    }

    glm::ivec2 
    BackendSystemInputSDL2::GetLastMousePosition(BackendMainWindowInterface* mainWindow) const
    {
        return glm::ivec2();
    }
    
    glm::ivec2 
    BackendSystemInputSDL2::GetCurrentMousePosition(BackendMainWindowInterface* mainWindow) const
    {
        return glm::ivec2();
    }
    
    void BackendSystemInputSDL2::SetMousePosition(  glm::ivec2 position, 
                                                        BackendMainWindowInterface* mainWindow)
    {

    }

    glm::vec2 BackendSystemInputSDL2::GetCurrentMouseScrollDelta() const
    {
        return glm::vec2();
    }

    std::vector<RealtimeInputInfo> DummyRealtimeInputList;

    const std::vector<RealtimeInputInfo>& BackendSystemInputSDL2::GetLastRealtimeInputs() const
    {
        return DummyRealtimeInputList;
    }

    const std::vector<RealtimeInputInfo>& BackendSystemInputSDL2::GetCurrentRealtimeInputs() const
    {
        return DummyRealtimeInputList;
    }

    void BackendSystemInputSDL2::GetTextInput(std::u32string& outText) const
    {
    
    }
    
    void BackendSystemInputSDL2::GetTextInput(std::string& outText) const
    {
    
    }
    
    void BackendSystemInputSDL2::SetCursorType(Enums::CursorType cursorType)
    {

    }

    Enums::CursorType BackendSystemInputSDL2::GetCursorType() const
    {
        return Enums::CursorType::NORMAL;
    }

    void BackendSystemInputSDL2::CreateCustomCursor(BackendImageInterface* customCursor, 
                                                        std::string cursorName, 
                                                        glm::ivec2 cursorSize, 
                                                        glm::ivec2 hotspot)
    {

    }
    
    void BackendSystemInputSDL2::SetCurrentCustomCursor(std::string cursorName)
    {

    }

    void BackendSystemInputSDL2::GetCurrentCustomCursor(BackendImageInterface& customCursor, 
                                                            glm::ivec2& hotspot) const
    {

    }

    std::string BackendSystemInputSDL2::GetCurrentCustomCursorName() const
    {
        return "";
    }
    
    void BackendSystemInputSDL2::GetCustomCursor(   BackendImageInterface& customCursor, 
                                                        std::string cursorName, 
                                                        glm::ivec2& hotspot) const
    {

    }

    bool BackendSystemInputSDL2::HasCustomCursor(std::string cursorName) const
    {
        return true;
    }

    void BackendSystemInputSDL2::UpdateCursor()
    {

    }
    
    int BackendSystemInputSDL2::AddRawEventHandler(std::function<bool(  BackendMainWindowInterface*, 
                                                                            void*)> handler)
    {
        return 0;
    }
    
    void BackendSystemInputSDL2::RemoveRawEventHandler(int id)
    {
    
    }
    
    void BackendSystemInputSDL2::ClearRawEventHandler()
    {
    
    }

    bool BackendSystemInputSDL2::ClearClipboard()
    {
        return true;
    }

    bool BackendSystemInputSDL2::ClipbaordHasText() const
    {
        return true;
    }
    
    bool BackendSystemInputSDL2::ClipbaordHasImage() const
    {
        return true;
    }

    bool BackendSystemInputSDL2::SetClipboardImage(const BackendImageInterface& imgData)
    {
        return true;
    }
    
    bool BackendSystemInputSDL2::SetClipboardText(const std::u32string& str)
    {
        return true;
    }
    
    bool BackendSystemInputSDL2::SetClipboardText(const std::string& str)
    {
        return true;
    }

    
    bool BackendSystemInputSDL2::GetClipboardImage(BackendImageInterface& imgData) const
    {
        return true;
    }

    bool BackendSystemInputSDL2::GetClipboardText(std::u32string& str) const
    {
        return true;
    }
    
    bool BackendSystemInputSDL2::GetClipboardText(std::string& str) const
    {
        return true;
    }

    uint64_t BackendSystemInputSDL2::GetElapsedTime() const
    {
        return 0;
    }
}

}