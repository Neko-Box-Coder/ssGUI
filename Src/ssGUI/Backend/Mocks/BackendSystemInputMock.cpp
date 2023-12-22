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
    
    void BackendSystemInputMock::UpdateInput(BackendMainWindowInterface** mainWindows, int count)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(UpdateInput(mainWindows, count));
    }

    using GenericInput = ssGUI::Enums::GenericInput;
    const std::vector<GenericInput>& BackendSystemInputMock::GetLastInputs() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC( GetLastInputs(), 
                                                const std::vector<GenericInput>&);
        return LastKeyPresses;
    }
    
    const std::vector<GenericInput>& BackendSystemInputMock::GetCurrentInputs() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC( GetCurrentInputs(), 
                                                const std::vector<GenericInput>&);
        return CurrentKeyPresses;
    }

    bool BackendSystemInputMock::IsInputExistLastFrame(GenericInput input) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_RETURN_IF_FOUND( OverrideObject, 
                            IsInputExistLastFrame(GenericInput), 
                            bool, 
                            input);
        
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(IsInputExistLastFrame(input), bool);
        
        return std::find(   LastKeyPresses.begin(), 
                            LastKeyPresses.end(), 
                            input) != LastKeyPresses.end();
    }

    bool BackendSystemInputMock::IsInputExistCurrentFrame(GenericInput input) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_RETURN_IF_FOUND( OverrideObject, 
                            IsInputExistCurrentFrame(GenericInput), 
                            bool, 
                            input);
        
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(IsInputExistCurrentFrame(input), bool);
        
        return std::find(   CurrentKeyPresses.begin(), 
                            CurrentKeyPresses.end(), 
                            input) != CurrentKeyPresses.end();
    }

    using MainWindowInterface = ssGUI::Backend::BackendMainWindowInterface;
    glm::ivec2 BackendSystemInputMock::GetLastMousePosition(MainWindowInterface* mainWindow) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        //void* wrapper = (void*)mainWindow;
        CO_RETURN_IF_FOUND( OverrideObject, 
                            GetLastMousePosition(MainWindowInterface*), 
                            glm::ivec2, 
                            (void*&)mainWindow);
        
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetLastMousePosition(mainWindow), glm::ivec2);
        return LastMousePosition;
    }
    
    glm::ivec2 BackendSystemInputMock::GetCurrentMousePosition(MainWindowInterface* mainWindow) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_RETURN_IF_FOUND( OverrideObject, 
                            GetCurrentMousePosition(MainWindowInterface*), 
                            glm::ivec2, 
                            (void*&)mainWindow);
        
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetCurrentMousePosition(mainWindow), glm::ivec2);
        return CurrentMousePosition;
    }
    
    void BackendSystemInputMock::SetMousePosition(glm::ivec2 position, MainWindowInterface* mainWindow)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(SetMousePosition(position, mainWindow));
        
        if(mainWindow == nullptr)
            CurrentMousePosition = position;
        else
        {
            glm::ivec2 topleft;
            glm::ivec2 botright;
            mainWindow->GetDecorationOffsets(topleft, botright);
            
            CurrentMousePosition =  mainWindow->GetWindowPosition() + 
                                    topleft + 
                                    position;
        }
    }

    glm::vec2 BackendSystemInputMock::GetCurrentMouseScrollDelta() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_RETURN_IF_FOUND(OverrideObject, GetCurrentMouseScrollDelta(), glm::vec2);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetCurrentMouseScrollDelta(), glm::vec2);
        return ScrollDelta;
    }

    const std::vector<ssGUI::RealtimeInputInfo>& 
        BackendSystemInputMock::GetLastRealtimeInputs() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC( GetLastRealtimeInputs(), 
                                                const std::vector<ssGUI::RealtimeInputInfo>&);
        return LastRealtimeInputs;
    }

    const std::vector<ssGUI::RealtimeInputInfo>& 
        BackendSystemInputMock::GetCurrentRealtimeInputs() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC( GetCurrentRealtimeInputs(), 
                                                const std::vector<ssGUI::RealtimeInputInfo>&);
        
        return CurrentRealtimeInputs;
    }

    void BackendSystemInputMock::GetTextInput(std::u32string& outText) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_MODIFY_ARGS_AND_RETURN_IF_FOUND( OverrideObject, 
                                            /**/, 
                                            GetTextInput(std::u32string&), 
                                            outText);
        
        SSGUI_MOCK_PASSTHROUGH(GetTextInput(outText));
    }
    
    void BackendSystemInputMock::GetTextInput(std::string& outText) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_MODIFY_ARGS_AND_RETURN_IF_FOUND( OverrideObject, 
                                            /**/, 
                                            GetTextInput(std::string&), 
                                            outText);
        
        SSGUI_MOCK_PASSTHROUGH(GetTextInput(outText));
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
        CO_RETURN_IF_FOUND(OverrideObject, GetCursorType(), ssGUI::Enums::CursorType);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetCursorType(), ssGUI::Enums::CursorType);
        return CurrentCursorType;
    }

    using ImageInterface = ssGUI::Backend::BackendImageInterface;
    void BackendSystemInputMock::CreateCustomCursor(ImageInterface* customCursor, 
                                                    std::string cursorName, 
                                                    glm::ivec2 cursorSize, 
                                                    glm::ivec2 hotspot)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SSGUI_MOCK_PASSTHROUGH(CreateCustomCursor(  customCursor, 
                                                    cursorName, 
                                                    cursorSize, 
                                                    hotspot));
        
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

    void BackendSystemInputMock::GetCurrentCustomCursor(ImageInterface& customCursor, 
                                                        glm::ivec2& hotspot) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_MODIFY_ARGS_AND_RETURN_IF_FOUND( OverrideObject,
                                            /*Empty return*/, 
                                            GetCurrentCustomCursor(ImageInterface&, glm::ivec2&), 
                                            customCursor, 
                                            hotspot);
        
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetCurrentCustomCursor(customCursor, hotspot));
        
        if(CurrentCursorIndex != -1)
        {
            customCursor = *CustomCursors[CurrentCursorIndex].CursorImage;
            hotspot = CustomCursors[CurrentCursorIndex].Hotspot;
        }
    }

    std::string BackendSystemInputMock::GetCurrentCustomCursorName() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_RETURN_IF_FOUND(OverrideObject, GetCurrentCustomCursorName(), std::string);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetCurrentCustomCursorName(), std::string);
        
        if(CurrentCursorIndex != -1)
            return CustomCursors[CurrentCursorIndex].CursorName;
        else
            return "";
    }
    
    void BackendSystemInputMock::GetCustomCursor(   ImageInterface& customCursor, 
                                                    std::string cursorName, 
                                                    glm::ivec2& hotspot) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_MODIFY_ARGS_AND_RETURN_IF_FOUND( OverrideObject,
                                            /*Empty return*/, 
                                            GetCustomCursor(ImageInterface&, 
                                                            std::string, 
                                                            glm::ivec2&), 
                                            customCursor, 
                                            cursorName, 
                                            hotspot);
        
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetCustomCursor(customCursor, cursorName, hotspot));
        
        if(CurrentCursorIndex != -1)
        {
            customCursor = *CustomCursors[CurrentCursorIndex].CursorImage;
            hotspot = CustomCursors[CurrentCursorIndex].Hotspot;
        }
    }

    bool BackendSystemInputMock::HasCustomCursor(std::string cursorName) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_RETURN_IF_FOUND( OverrideObject, 
                            &BackendSystemInputMock::HasCustomCursor, 
                            bool, 
                            cursorName);
        
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
    
    int BackendSystemInputMock::AddRawEventHandler(std::function<bool(  MainWindowInterface*, 
                                                                        void*)> handler)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_RETURN_IF_FOUND( OverrideObject, 
                            &BackendSystemInputMock::AddRawEventHandler, 
                            int, 
                            handler);
        
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
        CO_RETURN_IF_FOUND(OverrideObject, ClearClipboard(), bool);
        
        ClipboardText.clear();
        if(ClipboardImg != nullptr)
        {
            ssGUI::Dispose(ClipboardImg);
            ClipboardImg = nullptr;    
        }
        
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(ClearClipboard(), bool);
        return true;
    }

    bool BackendSystemInputMock::ClipbaordHasText() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_RETURN_IF_FOUND(OverrideObject, ClipbaordHasText(), bool);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(ClipbaordHasText(), bool);
        return !ClipboardText.empty();
    }
    
    bool BackendSystemInputMock::ClipbaordHasImage() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_RETURN_IF_FOUND(OverrideObject, ClipbaordHasImage(), bool);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(ClipbaordHasImage(), bool);
        return ClipboardImg != nullptr && ClipboardImg->IsValid();
    }

    bool BackendSystemInputMock::SetClipboardImage(const ImageInterface& imgData)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_RETURN_IF_FOUND( OverrideObject, 
                            SetClipboardImage(const ImageInterface&), 
                            bool, 
                            imgData);
        
        if(ClipboardImg != nullptr)
            ssGUI::Dispose(ClipboardImg);

        ClipboardImg = ssGUI::Backend::BackendFactory::CreateBackendImageInterface();
        *ClipboardImg = imgData;
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(SetClipboardImage(imgData), bool);
        return true;
    }
    
    bool BackendSystemInputMock::SetClipboardText(const std::u32string& str)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_RETURN_IF_FOUND(OverrideObject, SetClipboardText(const std::u32string&), bool, str);
        ClipboardText = str;
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(SetClipboardText(str), bool);
        return true;
    }
    
    bool BackendSystemInputMock::SetClipboardText(const std::string& str)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_RETURN_IF_FOUND(OverrideObject, SetClipboardText(const std::string&), bool, str);
        std::wstring_convert<std::codecvt_utf8_utf16<char32_t>, char32_t> converter;
        ClipboardText = converter.from_bytes(str);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(SetClipboardText(str), bool);
        return true;
    }
    
    bool BackendSystemInputMock::GetClipboardImage(ImageInterface& imgData) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_RETURN_IF_FOUND( OverrideObject, 
                            GetClipboardImage(ImageInterface&), 
                            bool, 
                            imgData);
        
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetClipboardImage(imgData), bool);
        
        if(ClipboardImg != nullptr && ClipboardImg->IsValid())
        {
            imgData = *ClipboardImg;
            return true;
        }
        else
            return false;
    }

    bool BackendSystemInputMock::GetClipboardText(std::u32string& str) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_RETURN_IF_FOUND(OverrideObject, GetClipboardText(std::u32string&), bool, str);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetClipboardText(str), bool);
        
        if(!ClipboardText.empty())
            str = ClipboardText;
        
        return !ClipboardText.empty();
    }
    
    bool BackendSystemInputMock::GetClipboardText(std::string& str) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_RETURN_IF_FOUND(OverrideObject, GetClipboardText(std::wstring&), bool, str);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetClipboardText(str), bool);
        
        std::wstring_convert<std::codecvt_utf8_utf16<char32_t>, char32_t> converter;
        
        if(!ClipboardText.empty())
            str = converter.to_bytes(ClipboardText);
        
        return !ClipboardText.empty();
    }

    uint64_t BackendSystemInputMock::GetElapsedTime() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_RETURN_IF_FOUND(OverrideObject, GetElapsedTime(), uint64_t);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetElapsedTime(), uint64_t);
        using sysClock = std::chrono::high_resolution_clock;
        
        auto timeDiff = sysClock::duration(sysClock::now() - StartTime);
        return std::chrono::duration_cast<std::chrono::milliseconds>(timeDiff).count();
    }
}

}