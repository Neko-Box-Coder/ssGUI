#include "ssGUI/Backend/Mocks/BackendSystemInputMock.hpp"

#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"
#include "ssGUI/Factory.hpp"

#include <algorithm>


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
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(UpdateInput());
    }

    const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& BackendSystemInputMock::GetLastButtonAndKeyPresses()
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetLastButtonAndKeyPresses(), const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>&);
        return LastKeyPresses;
    }
    
    const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& BackendSystemInputMock::GetCurrentButtonAndKeyPresses()
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetCurrentButtonAndKeyPresses(), const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>&);
        return CurrentKeyPresses;
    }

    bool BackendSystemInputMock::IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SO_RETURN_IF_FOUND(OverrideObject, IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::GenericButtonAndKeyInput), bool, input);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(IsButtonOrKeyPressExistLastFrame(input), bool);
        return std::find(LastKeyPresses.begin(), LastKeyPresses.end(), input) != LastKeyPresses.end();
    }

    bool BackendSystemInputMock::IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SO_RETURN_IF_FOUND(OverrideObject, IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::GenericButtonAndKeyInput), bool, input);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(IsButtonOrKeyPressExistCurrentFrame(input), bool);
        return std::find(CurrentKeyPresses.begin(), CurrentKeyPresses.end(), input) != CurrentKeyPresses.end();
    }

    glm::ivec2 BackendSystemInputMock::GetLastMousePosition(ssGUI::Backend::BackendMainWindowInterface* mainWindow) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        //void* wrapper = (void*)mainWindow;
        SO_RETURN_IF_FOUND(OverrideObject, GetLastMousePosition(ssGUI::Backend::BackendMainWindowInterface*), glm::ivec2, (void*&)mainWindow);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetLastMousePosition(mainWindow), glm::ivec2);
        return LastMousePosition;
    }
    
    glm::ivec2 BackendSystemInputMock::GetCurrentMousePosition(ssGUI::Backend::BackendMainWindowInterface* mainWindow) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SO_RETURN_IF_FOUND(OverrideObject, GetCurrentMousePosition(ssGUI::Backend::BackendMainWindowInterface*), glm::ivec2, (void*&)mainWindow);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetCurrentMousePosition(mainWindow), glm::ivec2);
        return CurrentMousePosition;
    }
    
    void BackendSystemInputMock::SetMousePosition(glm::ivec2 position, ssGUI::Backend::BackendMainWindowInterface* mainWindow)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(SetMousePosition(position, mainWindow));
        
        if(mainWindow == nullptr)
            CurrentMousePosition = position;
        else
            CurrentMousePosition = mainWindow->GetWindowPosition() + mainWindow->GetPositionOffset() + position;
    }

    bool BackendSystemInputMock::GetLastMouseButton(ssGUI::Enums::MouseButton button) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SO_RETURN_IF_FOUND(OverrideObject, GetLastMouseButton(ssGUI::Enums::MouseButton), bool, button);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetLastMouseButton(button), bool);
        return std::find(LastKeyPresses.begin(), LastKeyPresses.end(), static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(button)) != LastKeyPresses.end();
    }
    
    bool BackendSystemInputMock::GetCurrentMouseButton(ssGUI::Enums::MouseButton button) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SO_RETURN_IF_FOUND(OverrideObject, GetCurrentMouseButton(ssGUI::Enums::MouseButton), bool, button);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetCurrentMouseButton(button), bool);
        return std::find(CurrentKeyPresses.begin(), CurrentKeyPresses.end(), static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(button)) != CurrentKeyPresses.end();
    }

    glm::vec2 BackendSystemInputMock::GetCurrentMouseScrollDelta() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SO_RETURN_IF_FOUND(OverrideObject, GetCurrentMouseScrollDelta(), glm::vec2);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetCurrentMouseScrollDelta(), glm::vec2);
        return ScrollDelta;
    }

    const std::vector<ssGUI::RealtimeInputInfo>& BackendSystemInputMock::GetLastRealtimeInputs() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetLastRealtimeInputs(), const std::vector<ssGUI::RealtimeInputInfo>&);
        return LastRealtimeInputs;
    }

    const std::vector<ssGUI::RealtimeInputInfo>& BackendSystemInputMock::GetCurrentRealtimeInputs() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetCurrentRealtimeInputs(), const std::vector<ssGUI::RealtimeInputInfo>&);
        return CurrentRealtimeInputs;
    }

    std::wstring BackendSystemInputMock::GetTextInput() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SO_RETURN_IF_FOUND(OverrideObject, GetTextInput(), std::wstring);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetTextInput(), std::wstring);
        return CurrentTextInput;
    }
    
    void BackendSystemInputMock::SetCursorType(ssGUI::Enums::CursorType cursorType)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(SetCursorType(cursorType));
        CurrentCursorType = cursorType;
    }

    ssGUI::Enums::CursorType BackendSystemInputMock::GetCursorType() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SO_RETURN_IF_FOUND(OverrideObject, GetCursorType(), ssGUI::Enums::CursorType);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetCursorType(), ssGUI::Enums::CursorType);
        return CurrentCursorType;
    }

    void BackendSystemInputMock::CreateCustomCursor(ssGUI::Backend::BackendImageInterface* customCursor, std::string cursorName, glm::ivec2 cursorSize, glm::ivec2 hotspot)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(CreateCustomCursor(customCursor, cursorName, cursorSize, hotspot));
        
        if(customCursor != nullptr)
        {
            for(int i = 0; i < CustomCursors.size(); i++)
            {
                if(CustomCursors[i].CursorName == cursorName)
                    return;
            }
            CustomCursors.push_back({ customCursor, cursorName, cursorSize, hotspot});
        }
    }
    
    void BackendSystemInputMock::SetCurrentCustomCursor(std::string cursorName)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(SetCurrentCustomCursor(cursorName));
        for(int i = 0; i < CustomCursors.size(); i++)
        {
            if(CustomCursors[i].CursorName == cursorName)
            {
                CurrentCursorIndex = i;
                return;
            }
        }
    }

    void BackendSystemInputMock::GetCurrentCustomCursor(ssGUI::Backend::BackendImageInterface& customCursor, glm::ivec2& hotspot)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SO_MODIFY_ARGUMENTS_AND_RETURN_IF_FOUND(OverrideObject,
                                                /*Empty return*/, 
                                                GetCurrentCustomCursor(ssGUI::Backend::BackendImageInterface&, glm::ivec2&), 
                                                (SO_NonComparable<glm::ivec2>&)hotspot, 
                                                hotspot);
        
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetCurrentCustomCursor(customCursor, hotspot));
        
        if(CurrentCursorIndex != -1)
        {
            customCursor = *CustomCursors[CurrentCursorIndex].CursorImage;
            hotspot = CustomCursors[CurrentCursorIndex].Hotspot;
        }
    }

    std::string BackendSystemInputMock::GetCurrentCustomCursorName()
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SO_RETURN_IF_FOUND(OverrideObject, GetCurrentCustomCursorName(), std::string);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetCurrentCustomCursorName(), std::string);
        
        if(CurrentCursorIndex != -1)
            return CustomCursors[CurrentCursorIndex].CursorName;
        else
            return "";
    }
    
    void BackendSystemInputMock::GetCustomCursor(ssGUI::Backend::BackendImageInterface& customCursor, std::string cursorName, glm::ivec2& hotspot)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SO_MODIFY_ARGUMENTS_AND_RETURN_IF_FOUND(OverrideObject,
                                                /*Empty return*/, 
                                                GetCustomCursor(ssGUI::Backend::BackendImageInterface&, std::string, glm::ivec2&), 
                                                (SO_NonComparable<ssGUI::Backend::BackendImageInterface>&)customCursor, 
                                                cursorName, 
                                                hotspot);
        
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetCustomCursor(customCursor, cursorName, hotspot));
        
        if(CurrentCursorIndex != -1)
        {
            customCursor = *CustomCursors[CurrentCursorIndex].CursorImage;
            hotspot = CustomCursors[CurrentCursorIndex].Hotspot;
        }
    }

    bool BackendSystemInputMock::HasCustomCursor(std::string cursorName)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SO_RETURN_IF_FOUND(OverrideObject, &BackendSystemInputMock::HasCustomCursor, bool, cursorName);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(HasCustomCursor(cursorName), bool);
        
        for(int i = 0; i < CustomCursors.size(); i++)
        {
            if(CustomCursors[i].CursorName == cursorName)
                return true;
        }
        
        return false;
    }

    void BackendSystemInputMock::UpdateCursor()
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(UpdateCursor());
    }
    
    int BackendSystemInputMock::AddRawEventHandler(std::function<bool(ssGUI::Backend::BackendMainWindowInterface*, void*)> handler)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SO_RETURN_IF_FOUND(OverrideObject, &BackendSystemInputMock::AddRawEventHandler, int, SO_ANY);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(AddRawEventHandler(handler), int);
        return RawEventHandleNextId++;
    }
    
    void BackendSystemInputMock::RemoveRawEventHandler(int id)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(RemoveRawEventHandler(id));
    }
    
    void BackendSystemInputMock::ClearRawEventHandler()
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(ClearRawEventHandler());
    }

    bool BackendSystemInputMock::ClearClipboard()
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SO_RETURN_IF_FOUND(OverrideObject, ClearClipboard(), bool);
        
        ClipboardText.clear();
        if(ClipboardImg != nullptr)
        {
            ssGUI::Dispose(ClipboardImg);
            ClipboardImg = nullptr;    
        }
        
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(ClearClipboard(), bool);
        return true;
    }

    bool BackendSystemInputMock::ClipbaordHasText()
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SO_RETURN_IF_FOUND(OverrideObject, ClipbaordHasText(), bool);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(ClipbaordHasText(), bool);
        return !ClipboardText.empty();
    }
    
    bool BackendSystemInputMock::ClipbaordHasImage()
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SO_RETURN_IF_FOUND(OverrideObject, ClipbaordHasImage(), bool);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(ClipbaordHasImage(), bool);
        return ClipboardImg != nullptr && ClipboardImg->IsValid();
    }

    bool BackendSystemInputMock::SetClipboardImage(const ssGUI::Backend::BackendImageInterface& imgData)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SO_RETURN_IF_FOUND( OverrideObject, 
                            SetClipboardImage(const ssGUI::Backend::BackendImageInterface&), 
                            bool, 
                            (SO_NonComparable<const ssGUI::Backend::BackendImageInterface>&)imgData);
        
        if(ClipboardImg != nullptr)
            ssGUI::Dispose(ClipboardImg);

        ClipboardImg = ssGUI::Backend::BackendFactory::CreateBackendImageInterface();
        *ClipboardImg = imgData;
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(SetClipboardImage(imgData), bool);
        return true;
    }
    
    bool BackendSystemInputMock::SetClipboardText(const std::wstring& str)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SO_RETURN_IF_FOUND(OverrideObject, SetClipboardText(const std::wstring&), bool, str);
        ClipboardText = str;
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(SetClipboardText(str), bool);
        return true;
    }
    
    bool BackendSystemInputMock::GetClipboardImage(ssGUI::Backend::BackendImageInterface& imgData)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SO_RETURN_IF_FOUND( OverrideObject, 
                            GetClipboardImage(ssGUI::Backend::BackendImageInterface&), 
                            bool, 
                            (SO_NonComparable<const ssGUI::Backend::BackendImageInterface>&)imgData);
        
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetClipboardImage(imgData), bool);
        
        if(ClipboardImg != nullptr && ClipboardImg->IsValid())
        {
            imgData = *ClipboardImg;
            return true;
        }
        else
            return false;
    }

    bool BackendSystemInputMock::GetClipboardText(std::wstring& str)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SO_RETURN_IF_FOUND(OverrideObject, GetClipboardText(std::wstring&), bool, str);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetClipboardText(str), bool);
        
        if(!ClipboardText.empty())
            str = ClipboardText;
        
        return !ClipboardText.empty();
    }

    uint64_t BackendSystemInputMock::GetElapsedTime() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SO_RETURN_IF_FOUND(OverrideObject, GetElapsedTime(), uint64_t);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetElapsedTime(), uint64_t);
        using sysClock = std::chrono::high_resolution_clock;
        return std::chrono::duration_cast<std::chrono::milliseconds>(sysClock::duration(sysClock::now() - StartTime)).count();
    }
}

}