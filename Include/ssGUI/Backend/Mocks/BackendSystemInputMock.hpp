#ifndef SSGUI_BACKEND_SYSTEM_INPUT_MOCK_H
#define SSGUI_BACKEND_SYSTEM_INPUT_MOCK_H

#include "CppOverride.hpp"
#include "ssGUI/Backend/Interfaces/BackendSystemInputInterface.hpp"

//#define SSGUI_MOCK_ENABLE_LOG
#include "ssGUI/Backend/Mocks/MockMacro.hpp"

#include <chrono>

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    struct MockCursorData
    {
        ssGUI::Backend::BackendImageInterface* CursorImage = nullptr; 
        std::string CursorName = "";
        glm::ivec2 CursorSize;
        glm::ivec2 Hotspot;
        
        MockCursorData( ssGUI::Backend::BackendImageInterface* cursorImage,
                        std::string cursorName, glm::ivec2 cursorSize, glm::ivec2 hotspot) :    CursorImage(cursorImage),
                                                                                                CursorName(cursorName),
                                                                                                CursorSize(cursorSize),
                                                                                                Hotspot(hotspot)
        {}
    };
    

    //class: ssGUI::Backend::BackendSystemInputMock
    class BackendSystemInputMock : public BackendSystemInputInterface
    {   
        private:
            ssGUI::Backend::BackendSystemInputInterface* UnderlyingInterface;
            std::vector<ssGUI::Enums::GenericButtonAndKeyInput> LastKeyPresses;
            std::vector<ssGUI::Enums::GenericButtonAndKeyInput> CurrentKeyPresses;
            
            glm::ivec2 LastMousePosition;
            glm::ivec2 CurrentMousePosition;
            glm::vec2 ScrollDelta;
            std::vector<ssGUI::RealtimeInputInfo> LastRealtimeInputs;
            std::vector<ssGUI::RealtimeInputInfo> CurrentRealtimeInputs;
            std::wstring CurrentTextInput;
            ssGUI::Enums::CursorType CurrentCursorType;

            std::vector<MockCursorData> CustomCursors;
            int CurrentCursorIndex;
            int RawEventHandleNextId;
            
            std::wstring ClipboardText;
            BackendImageInterface* ClipboardImg;
            std::chrono::high_resolution_clock::time_point StartTime;
            

            CO_DECLARE_MEMBER_INSTNACE(OverrideObject);

            BackendSystemInputMock& operator=(BackendSystemInputMock const& other);

        protected:
            BackendSystemInputMock(BackendSystemInputMock const& other);

        public:
            BackendSystemInputMock(ssGUI::Backend::BackendSystemInputInterface* systemInputInterface);
            ~BackendSystemInputMock() override;
            
            CO_DECLARE_OVERRIDE_METHODS(OverrideObject)
            
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(ssGUI::Backend::BackendSystemInputInterface*, UnderlyingInterface)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(std::vector<ssGUI::Enums::GenericButtonAndKeyInput>, LastKeyPresses)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(std::vector<ssGUI::Enums::GenericButtonAndKeyInput>, CurrentKeyPresses)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(glm::ivec2, LastMousePosition)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(glm::ivec2, CurrentMousePosition)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(glm::vec2, ScrollDelta)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(std::vector<ssGUI::RealtimeInputInfo>, LastRealtimeInputs)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(std::vector<ssGUI::RealtimeInputInfo>, CurrentRealtimeInputs)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(std::wstring, CurrentTextInput)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(ssGUI::Enums::CursorType, CurrentCursorType)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(std::vector<MockCursorData>, CustomCursors)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(int, CurrentCursorIndex)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(int, RawEventHandleNextId)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(std::wstring, ClipboardText)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(BackendImageInterface*, ClipboardImg)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(std::chrono::high_resolution_clock::time_point, StartTime)

            //function: UpdateInput
            //See <BackendSystemInputInterface::UpdateInput>
            void UpdateInput() override;

            //function: GetLastButtonAndKeyPresses
            //See <BackendSystemInputInterface::GetLastButtonAndKeyPresses>
            const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& GetLastButtonAndKeyPresses() override;
            
            //function: GetCurrentButtonAndKeyPresses
            //See <BackendSystemInputInterface::GetCurrentButtonAndKeyPresses>
            const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& GetCurrentButtonAndKeyPresses() override;

            //function: IsButtonOrKeyPressExistLastFrame
            //See <BackendSystemInputInterface::IsButtonOrKeyPressExistLastFrame>
            bool IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const override;

            //function: IsButtonOrKeyPressExistCurrentFrame
            //See <BackendSystemInputInterface::IsButtonOrKeyPressExistCurrentFrame>
            bool IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const override;

            //function: GetLastMousePosition
            //See <BackendSystemInputInterface::GetLastMousePosition>
            glm::ivec2 GetLastMousePosition(ssGUI::Backend::BackendMainWindowInterface* mainWindow) const override;
            
            //function: GetCurrentMousePosition
            //See <BackendSystemInputInterface::GetCurrentMousePosition>
            glm::ivec2 GetCurrentMousePosition(ssGUI::Backend::BackendMainWindowInterface* mainWindow) const override;
            
            //function: SetMousePosition
            //See <BackendSystemInputInterface::SetMousePosition>
            void SetMousePosition(glm::ivec2 position, ssGUI::Backend::BackendMainWindowInterface* mainWindow) override;

            //function: GetLastMouseButton
            //See <BackendSystemInputInterface::GetLastMouseButton>
            bool GetLastMouseButton(ssGUI::Enums::MouseButton button) const override;
            
            //function: GetCurrentMouseButton
            //See <BackendSystemInputInterface::GetCurrentMouseButton>
            bool GetCurrentMouseButton(ssGUI::Enums::MouseButton button) const override;

            //function: GetCurrentMouseScrollDelta
            //See <BackendSystemInputInterface::GetCurrentMouseScrollDelta>
            glm::vec2 GetCurrentMouseScrollDelta() const override;

            //function: GetLastRealtimeInputs
            //See <BackendSystemInputInterface::GetLastRealtimeInputs>
            const std::vector<ssGUI::RealtimeInputInfo>& GetLastRealtimeInputs() const override;

            //function: GetCurrentRealtimeInputs
            //See <BackendSystemInputInterface::GetCurrentRealtimeInputs>
            const std::vector<ssGUI::RealtimeInputInfo>& GetCurrentRealtimeInputs() const override;

            //function: GetTextInput
            //See <BackendSystemInputInterface::GetTextInput>
            std::wstring GetTextInput() const override;
            
            //function: SetCursorType
            //See <BackendSystemInputInterface::SetCursorType>
            void SetCursorType(ssGUI::Enums::CursorType cursorType) override;

            //function: GetCursorType
            //See <BackendSystemInputInterface::GetCursorType>
            ssGUI::Enums::CursorType GetCursorType() const override;

            //function: CreateCustomCursor
            //See <BackendSystemInputInterface::CreateCustomCursor>
            void CreateCustomCursor(ssGUI::Backend::BackendImageInterface* customCursor, std::string cursorName, glm::ivec2 cursorSize, glm::ivec2 hotspot) override;
            
            //function: SetCurrentCustomCursor
            //See <BackendSystemInputInterface::SetCurrentCustomCursor>
            void SetCurrentCustomCursor(std::string cursorName) override;

            //function: GetCurrentCustomCursor
            //See <BackendSystemInputInterface::GetCurrentCustomCursor>
            void GetCurrentCustomCursor(ssGUI::Backend::BackendImageInterface& customCursor, glm::ivec2& hotspot) override;

            //function: GetCurrentCustomCursorName
            //See <BackendSystemInputInterface::GetCurrentCustomCursorName>
            std::string GetCurrentCustomCursorName() override;
            
            //function: GetCustomCursor
            //See <BackendSystemInputInterface::GetCustomCursor>
            void GetCustomCursor(ssGUI::Backend::BackendImageInterface& customCursor, std::string cursorName, glm::ivec2& hotspot) override;

            //function: HasCustomCursor
            //See <BackendSystemInputInterface::HasCustomCursor>
            bool HasCustomCursor(std::string cursorName) override;

            //function: UpdateCursor
            //See <BackendSystemInputInterface::UpdateCursor>
            void UpdateCursor() override;
            
            //function: AddRawEventHandler
            //See <BackendSystemInputInterface::AddRawEventHandler>
            int AddRawEventHandler(std::function<bool(ssGUI::Backend::BackendMainWindowInterface*, void*)> handler) override;
            
            //function: RemoveRawEventHandler
            //See <BackendSystemInputInterface::RemoveRawEventHandler>
            void RemoveRawEventHandler(int id) override;
            
            //function: ClearRawEventHandler
            //See <<BackendSystemInputInterface::ClearRawEventHandler>
            void ClearRawEventHandler() override;

            //function: ClearClipboard
            //See <BackendSystemInputInterface::ClearClipboard>
            bool ClearClipboard() override;

            //function: ClipbaordHasText
            //See <BackendSystemInputInterface::ClipbaordHasText>
            bool ClipbaordHasText() override;
            
            //function: ClipbaordHasImage
            //See <BackendSystemInputInterface::ClipbaordHasImage>
            bool ClipbaordHasImage() override;

            //function: SetClipboardImage
            //See <BackendSystemInputInterface::SetClipboardImage>
            bool SetClipboardImage(const ssGUI::Backend::BackendImageInterface& imgData) override;
            
            //function: SetClipboardText
            //See <BackendSystemInputInterface::SetClipboardText>
            bool SetClipboardText(const std::wstring& str) override;
            
            //function: GetClipboardImage
            //See <BackendSystemInputInterface::GetClipboardImage>
            bool GetClipboardImage(ssGUI::Backend::BackendImageInterface& imgData) override;

            //function: GetClipboardText
            //See <BackendSystemInputInterface::GetClipboardText>
            bool GetClipboardText(std::wstring& str) override;

            //function: GetElapsedTime
            //See <BackendSystemInputInterface::GetElapsedTime>
            uint64_t GetElapsedTime() const override;
    };
}

}

#endif