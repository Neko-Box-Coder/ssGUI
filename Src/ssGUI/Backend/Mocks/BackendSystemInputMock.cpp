#include "ssGUI/Backend/Mocks/BackendSystemInputMock.hpp"

#include "ssGUI/Backend/Mocks/MockMacro.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendSystemInputMock::BackendSystemInputMock(BackendSystemInputMock const& other)
    {
        ssGUI_ERROR(ssGUI_BACKEND_TAG, "Not Implemented");
        ssLOG_EXIT_PROGRAM(1);
    }

    BackendSystemInputMock::BackendSystemInputMock(ssGUI::Backend::BackendSystemInputInterface* systemInputInterface)
    {
        UnderlyingInterface = systemInputInterface;
    }

    BackendSystemInputMock::~BackendSystemInputMock()
    {
        if(UnderlyingInterface != nullptr)
            delete UnderlyingInterface;
    }
    
    void BackendSystemInputMock::UpdateInput()
    {
        SSGUI_MOCK_PASSTHROUGH(UpdateInput());
    }

    const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& BackendSystemInputMock::GetLastButtonAndKeyPresses()
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetLastButtonAndKeyPresses());
        std::vector<ssGUI::Enums::GenericButtonAndKeyInput> t;
        return t;
    }
    
    const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& BackendSystemInputMock::GetCurrentButtonAndKeyPresses()
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetCurrentButtonAndKeyPresses());
        std::vector<ssGUI::Enums::GenericButtonAndKeyInput> t;
        return t;
    }

    bool BackendSystemInputMock::IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(IsButtonOrKeyPressExistLastFrame(input));
        return true;
    }

    bool BackendSystemInputMock::IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(IsButtonOrKeyPressExistCurrentFrame(input));
        return true;
    }

    glm::ivec2 BackendSystemInputMock::GetLastMousePosition(ssGUI::Backend::BackendMainWindowInterface* mainWindow) const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetLastMousePosition(mainWindow));
        return glm::ivec2();
    }
    
    glm::ivec2 BackendSystemInputMock::GetCurrentMousePosition(ssGUI::Backend::BackendMainWindowInterface* mainWindow) const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetCurrentMousePosition(mainWindow));
        return glm::ivec2();
    }
    
    void BackendSystemInputMock::SetMousePosition(glm::ivec2 position, ssGUI::Backend::BackendMainWindowInterface* mainWindow)
    {
        SSGUI_MOCK_PASSTHROUGH(SetMousePosition(position, mainWindow));
    }

    bool BackendSystemInputMock::GetLastMouseButton(ssGUI::Enums::MouseButton button) const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetLastMouseButton(button));
        return true;
    }
    
    bool BackendSystemInputMock::GetCurrentMouseButton(ssGUI::Enums::MouseButton button) const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetCurrentMouseButton(button));
        return true;
    }

    glm::vec2 BackendSystemInputMock::GetCurrentMouseScrollDelta() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetCurrentMouseScrollDelta());
        return glm::vec2();
    }

    std::vector<ssGUI::RealtimeInputInfo> const & BackendSystemInputMock::GetLastRealtimeInputs() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetLastRealtimeInputs());
        std::vector<ssGUI::RealtimeInputInfo> t;
        return t;
    }

    std::vector<ssGUI::RealtimeInputInfo> const & BackendSystemInputMock::GetCurrentRealtimeInputs() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetCurrentRealtimeInputs());
        std::vector<ssGUI::RealtimeInputInfo> t;
        return t;
    }

    std::wstring BackendSystemInputMock::GetTextInput() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetTextInput());
        return L"";
    }
    
    void BackendSystemInputMock::SetCursorType(ssGUI::Enums::CursorType cursorType)
    {
        SSGUI_MOCK_PASSTHROUGH(SetCursorType(cursorType));
    }

    ssGUI::Enums::CursorType BackendSystemInputMock::GetCursorType() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetCursorType());
        return ssGUI::Enums::CursorType::NORMAL;
    }

    void BackendSystemInputMock::CreateCustomCursor(ssGUI::Backend::BackendImageInterface* customCursor, std::string cursorName, glm::ivec2 cursorSize, glm::ivec2 hotspot)
    {
        SSGUI_MOCK_PASSTHROUGH(CreateCustomCursor(customCursor, cursorName, cursorSize, hotspot));
    }
    
    void BackendSystemInputMock::SetCurrentCustomCursor(std::string cursorName)
    {
        SSGUI_MOCK_PASSTHROUGH(SetCurrentCustomCursor(cursorName));
    }

    void BackendSystemInputMock::GetCurrentCustomCursor(ssGUI::Backend::BackendImageInterface& customCursor, glm::ivec2& hotspot)
    {
        SSGUI_MOCK_PASSTHROUGH(GetCurrentCustomCursor(customCursor, hotspot));
    }

    std::string BackendSystemInputMock::GetCurrentCustomCursorName()
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetCurrentCustomCursorName());
        return "";
    }
    
    void BackendSystemInputMock::GetCustomCursor(ssGUI::Backend::BackendImageInterface& customCursor, std::string cursorName, glm::ivec2& hotspot)
    {
        SSGUI_MOCK_PASSTHROUGH(GetCustomCursor(customCursor, cursorName, hotspot));
    }

    bool BackendSystemInputMock::HasCustomCursor(std::string cursorName)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(HasCustomCursor(cursorName));
        return true;
    }

    void BackendSystemInputMock::UpdateCursor()
    {
        SSGUI_MOCK_PASSTHROUGH(UpdateCursor());
    }
    
    int BackendSystemInputMock::AddRawEventHandler(std::function<bool(ssGUI::Backend::BackendMainWindowInterface*, void*)> handler)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(AddRawEventHandler(handler));
        return 0;
    }
    
    void BackendSystemInputMock::RemoveRawEventHandler(int id)
    {
        SSGUI_MOCK_PASSTHROUGH(RemoveRawEventHandler(id));
    }
    
    void BackendSystemInputMock::ClearRawEventHandler()
    {
        SSGUI_MOCK_PASSTHROUGH(ClearRawEventHandler());
    }

    bool BackendSystemInputMock::ClearClipboard()
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(ClearClipboard());
        return true;
    }

    bool BackendSystemInputMock::ClipbaordHasText()
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(ClipbaordHasText());
        return true;
    }
    
    bool BackendSystemInputMock::ClipbaordHasImage()
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(ClipbaordHasImage());
        return true;
    }

    bool BackendSystemInputMock::SetClipboardImage(const ssGUI::Backend::BackendImageInterface& imgData)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(SetClipboardImage(imgData));
        return true;
    }
    
    bool BackendSystemInputMock::SetClipboardText(const std::wstring& str)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(SetClipboardText(str));
        return true;
    }
    
    bool BackendSystemInputMock::GetClipboardImage(ssGUI::Backend::BackendImageInterface& imgData)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetClipboardImage(imgData));
        return true;
    }

    bool BackendSystemInputMock::GetClipboardText(std::wstring& str)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetClipboardText(str));
        return true;
    }

    uint64_t BackendSystemInputMock::GetElapsedTime() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetElapsedTime());
        return 0;
    }
}

}