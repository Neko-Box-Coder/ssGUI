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
        BackendImageInterface* CursorImage = nullptr; 
        std::string CursorName = "";
        glm::ivec2 CursorSize;
        glm::ivec2 Hotspot;
        
        MockCursorData( BackendImageInterface* cursorImage,
                        std::string cursorName, 
                        glm::ivec2 cursorSize, 
                        glm::ivec2 hotspot) :   CursorImage(cursorImage),
                                                CursorName(cursorName),
                                                CursorSize(cursorSize),
                                                Hotspot(hotspot)
        {}
    };
    

    //class: ssGUI::Backend::BackendSystemInputMock
    class BackendSystemInputMock : public BackendSystemInputInterface
    {   
        private:
            BackendSystemInputInterface* UnderlyingInterface;
            std::vector<Enums::GenericInput> LastKeyPresses;
            std::vector<Enums::GenericInput> CurrentKeyPresses;
            
            glm::ivec2 LastMousePosition;
            glm::ivec2 CurrentMousePosition;
            glm::vec2 ScrollDelta;
            std::vector<RealtimeInputInfo> LastRealtimeInputs;
            std::vector<RealtimeInputInfo> CurrentRealtimeInputs;
            std::u32string CurrentTextInput;
            Enums::CursorType CurrentCursorType;

            std::vector<MockCursorData> CustomCursors;
            int CurrentCursorIndex;
            int RawEventHandleNextId;
            
            std::u32string ClipboardText;
            BackendImageInterface* ClipboardImg;
            std::chrono::high_resolution_clock::time_point StartTime;
            
            glm::ivec2 TextInputPos;
            glm::ivec2 TextInputSize;
            bool AcceptTextInput;

            CO_DECLARE_MEMBER_INSTNACE(OverrideObject);

            BackendSystemInputMock& operator=(BackendSystemInputMock const& other);

        protected:
            BackendSystemInputMock(BackendSystemInputMock const& other);

        public:
            BackendSystemInputMock(BackendSystemInputInterface* systemInputInterface);
            ~BackendSystemInputMock() override;
            
            CO_DECLARE_OVERRIDE_METHODS(OverrideObject)
            
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(BackendSystemInputInterface*, UnderlyingInterface)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(std::vector<Enums::GenericInput>, LastKeyPresses)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(std::vector<Enums::GenericInput>, CurrentKeyPresses)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(glm::ivec2, LastMousePosition)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(glm::ivec2, CurrentMousePosition)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(glm::vec2, ScrollDelta)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(std::vector<RealtimeInputInfo>, LastRealtimeInputs)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(std::vector<RealtimeInputInfo>, CurrentRealtimeInputs)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(std::u32string, CurrentTextInput)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(Enums::CursorType, CurrentCursorType)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(std::vector<MockCursorData>, CustomCursors)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(int, CurrentCursorIndex)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(int, RawEventHandleNextId)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(std::u32string, ClipboardText)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(BackendImageInterface*, ClipboardImg)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(std::chrono::high_resolution_clock::time_point, StartTime)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(glm::ivec2, TextInputPos)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(glm::ivec2, TextInputSize)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(bool, AcceptTextInput)

            //function: UpdateInput
            //See <BackendSystemInputInterface::UpdateInput>
            void UpdateInput(BackendMainWindowInterface** mainWindows, int count) override;

            //function: GetLastInputs
            //See <BackendSystemInputInterface::GetLastInputs>
            const std::vector<Enums::GenericInput>& GetLastInputs() const override;
            
            //function: GetCurrentInputs
            //See <BackendSystemInputInterface::GetCurrentInputs>
            const std::vector<Enums::GenericInput>& GetCurrentInputs() const override;

            //function: IsInputExistLastFrame
            //See <BackendSystemInputInterface::IsInputExistLastFrame>
            bool IsInputExistLastFrame(Enums::GenericInput input) const override;

            //function: IsInputExistCurrentFrame
            //See <BackendSystemInputInterface::IsInputExistCurrentFrame>
            bool IsInputExistCurrentFrame(Enums::GenericInput input) const override;

            //function: GetLastMousePosition
            //See <BackendSystemInputInterface::GetLastMousePosition>
            glm::ivec2 GetLastMousePosition(BackendMainWindowInterface* mainWindow) const override;
            
            //function: GetCurrentMousePosition
            //See <BackendSystemInputInterface::GetCurrentMousePosition>
            glm::ivec2 GetCurrentMousePosition(BackendMainWindowInterface* mainWindow) const override;
            
            //function: SetMousePosition
            //See <BackendSystemInputInterface::SetMousePosition>
            void SetMousePosition(glm::ivec2 position, BackendMainWindowInterface* mainWindow) override;

            //function: GetCurrentMouseScrollDelta
            //See <BackendSystemInputInterface::GetCurrentMouseScrollDelta>
            glm::vec2 GetCurrentMouseScrollDelta() const override;

            //function: GetLastRealtimeInputs
            //See <BackendSystemInputInterface::GetLastRealtimeInputs>
            const std::vector<RealtimeInputInfo>& GetLastRealtimeInputs() const override;

            //function: GetCurrentRealtimeInputs
            //See <BackendSystemInputInterface::GetCurrentRealtimeInputs>
            const std::vector<RealtimeInputInfo>& GetCurrentRealtimeInputs() const override;

            //function: StartTextInput
            //See <BackendSystemInputInterface::StartTextInput>
            void StartTextInput(glm::ivec2 inputPos, glm::ivec2 inputSize) override;
            
            //function: FinishTextInput
            //See <BackendSystemInputInterface::FinishTextInput>
            void FinishTextInput() override;
            
            //function: GetTextInput
            //See <BackendSystemInputInterface::GetTextInput>
            void GetTextInput(std::u32string& outText) const override;

            //function: GetTextInput
            //See <BackendSystemInputInterface::GetTextInput>
            void GetTextInput(std::string& outText) const override;

            //function: SetCursorType
            //See <BackendSystemInputInterface::SetCursorType>
            void SetCursorType(Enums::CursorType cursorType) override;

            //function: GetCursorType
            //See <BackendSystemInputInterface::GetCursorType>
            Enums::CursorType GetCursorType() const override;

            //function: CreateCustomCursor
            //See <BackendSystemInputInterface::CreateCustomCursor>
            void CreateCustomCursor(BackendImageInterface* customCursor, 
                                    std::string cursorName, 
                                    glm::ivec2 cursorSize, 
                                    glm::ivec2 hotspot) override;
            
            //function: SetCurrentCustomCursor
            //See <BackendSystemInputInterface::SetCurrentCustomCursor>
            void SetCurrentCustomCursor(std::string cursorName) override;

            //function: GetCurrentCustomCursor
            //See <BackendSystemInputInterface::GetCurrentCustomCursor>
            void GetCurrentCustomCursor(BackendImageInterface& customCursor, 
                                        glm::ivec2& hotspot) const override;

            //function: GetCurrentCustomCursorName
            //See <BackendSystemInputInterface::GetCurrentCustomCursorName>
            std::string GetCurrentCustomCursorName() const override;
            
            //function: GetCustomCursor
            //See <BackendSystemInputInterface::GetCustomCursor>
            void GetCustomCursor(   BackendImageInterface& customCursor, 
                                    std::string cursorName, 
                                    glm::ivec2& hotspot) const override;

            //function: HasCustomCursor
            //See <BackendSystemInputInterface::HasCustomCursor>
            bool HasCustomCursor(std::string cursorName) const override;

            //function: UpdateCursor
            //See <BackendSystemInputInterface::UpdateCursor>
            void UpdateCursor() override;
            
            //function: AddRawEventHandler
            //See <BackendSystemInputInterface::AddRawEventHandler>
            int AddRawEventHandler(std::function<bool(  BackendMainWindowInterface*, 
                                                        void*)> handler) override;
            
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
            bool ClipbaordHasText() const override;
            
            //function: ClipbaordHasImage
            //See <BackendSystemInputInterface::ClipbaordHasImage>
            bool ClipbaordHasImage() const override;

            //function: SetClipboardImage
            //See <BackendSystemInputInterface::SetClipboardImage>
            bool SetClipboardImage(const BackendImageInterface& imgData) override;
            
            //function: SetClipboardText
            //See <BackendSystemInputInterface::SetClipboardText>
            bool SetClipboardText(const std::u32string& str) override;
            
            //function: SetClipboardText
            //See <BackendSystemInputInterface::SetClipboardText>
            bool SetClipboardText(const std::string& str) override;
            
            //function: GetClipboardImage
            //See <BackendSystemInputInterface::GetClipboardImage>
            bool GetClipboardImage(BackendImageInterface& imgData) const override;

            //function: GetClipboardText
            //See <BackendSystemInputInterface::GetClipboardText>
            bool GetClipboardText(std::u32string& str) const override;

            //function: GetClipboardText
            //See <BackendSystemInputInterface::GetClipboardText>
            bool GetClipboardText(std::string& str) const override;

            //function: GetElapsedTime
            //See <BackendSystemInputInterface::GetElapsedTime>
            uint64_t GetElapsedTime() const override;
    };
}

}

#endif