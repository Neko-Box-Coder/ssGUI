#ifndef SSGUI_BACKEND_SYSTEM_INPUT_MOCK_H
#define SSGUI_BACKEND_SYSTEM_INPUT_MOCK_H

#include "CppOverride.hpp"
#include "ssGUI/Backend/Interfaces/SystemInputInterface.hpp"

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
        ImageInterface* CursorImage = nullptr; 
        std::string CursorName = "";
        glm::ivec2 CursorSize;
        glm::ivec2 Hotspot;
        
        MockCursorData( ImageInterface* cursorImage,
                        std::string cursorName, 
                        glm::ivec2 cursorSize, 
                        glm::ivec2 hotspot) :   CursorImage(cursorImage),
                                                CursorName(cursorName),
                                                CursorSize(cursorSize),
                                                Hotspot(hotspot)
        {}
    };
    

    //class: ssGUI::Backend::BackendSystemInputMock
    class BackendSystemInputMock : public SystemInputInterface
    {   
        private:
            SystemInputInterface* UnderlyingInterface;
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
            ImageInterface* ClipboardImg;
            std::chrono::high_resolution_clock::time_point StartTime;
            
            glm::ivec2 TextInputPos;
            glm::ivec2 TextInputSize;
            bool AcceptTextInput;

            CO_DECLARE_MEMBER_INSTNACE(OverrideObject);

            BackendSystemInputMock& operator=(BackendSystemInputMock const& other);

        protected:
            BackendSystemInputMock(BackendSystemInputMock const& other);

        public:
            BackendSystemInputMock(SystemInputInterface* systemInputInterface);
            ~BackendSystemInputMock() override;
            
            CO_DECLARE_OVERRIDE_METHODS(OverrideObject)
            
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(SystemInputInterface*, UnderlyingInterface)
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
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(ImageInterface*, ClipboardImg)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(std::chrono::high_resolution_clock::time_point, StartTime)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(glm::ivec2, TextInputPos)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(glm::ivec2, TextInputSize)
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(bool, AcceptTextInput)

            //function: UpdateInput
            //See <SystemInputInterface::UpdateInput>
            void UpdateInput(MainWindowInterface** mainWindows, int count) override;

            //function: GetLastInputs
            //See <SystemInputInterface::GetLastInputs>
            const std::vector<Enums::GenericInput>& GetLastInputs() const override;
            
            //function: GetCurrentInputs
            //See <SystemInputInterface::GetCurrentInputs>
            const std::vector<Enums::GenericInput>& GetCurrentInputs() const override;

            //function: IsInputExistLastFrame
            //See <SystemInputInterface::IsInputExistLastFrame>
            bool IsInputExistLastFrame(Enums::GenericInput input) const override;

            //function: IsInputExistCurrentFrame
            //See <SystemInputInterface::IsInputExistCurrentFrame>
            bool IsInputExistCurrentFrame(Enums::GenericInput input) const override;

            //function: GetLastMousePosition
            //See <SystemInputInterface::GetLastMousePosition>
            glm::ivec2 GetLastMousePosition(MainWindowInterface* mainWindow) const override;
            
            //function: GetCurrentMousePosition
            //See <SystemInputInterface::GetCurrentMousePosition>
            glm::ivec2 GetCurrentMousePosition(MainWindowInterface* mainWindow) const override;
            
            //function: SetMousePosition
            //See <SystemInputInterface::SetMousePosition>
            void SetMousePosition(glm::ivec2 position, MainWindowInterface* mainWindow) override;

            //function: GetCurrentMouseScrollDelta
            //See <SystemInputInterface::GetCurrentMouseScrollDelta>
            glm::vec2 GetCurrentMouseScrollDelta() const override;

            //function: GetLastRealtimeInputs
            //See <SystemInputInterface::GetLastRealtimeInputs>
            const std::vector<RealtimeInputInfo>& GetLastRealtimeInputs() const override;

            //function: GetCurrentRealtimeInputs
            //See <SystemInputInterface::GetCurrentRealtimeInputs>
            const std::vector<RealtimeInputInfo>& GetCurrentRealtimeInputs() const override;

            //function: StartTextInput
            //See <SystemInputInterface::StartTextInput>
            void StartTextInput(glm::ivec2 inputPos, glm::ivec2 inputSize) override;
            
            //function: FinishTextInput
            //See <SystemInputInterface::FinishTextInput>
            void FinishTextInput() override;
            
            //function: GetTextInput
            //See <SystemInputInterface::GetTextInput>
            void GetTextInput(std::u32string& outText) const override;

            //function: GetTextInput
            //See <SystemInputInterface::GetTextInput>
            void GetTextInput(std::string& outText) const override;

            //function: SetCursorType
            //See <SystemInputInterface::SetCursorType>
            void SetCursorType(Enums::CursorType cursorType) override;

            //function: GetCursorType
            //See <SystemInputInterface::GetCursorType>
            Enums::CursorType GetCursorType() const override;

            //function: CreateCustomCursor
            //See <SystemInputInterface::CreateCustomCursor>
            void CreateCustomCursor(ImageInterface* customCursor, 
                                    std::string cursorName, 
                                    glm::ivec2 cursorSize, 
                                    glm::ivec2 hotspot) override;
            
            //function: SetCurrentCustomCursor
            //See <SystemInputInterface::SetCurrentCustomCursor>
            void SetCurrentCustomCursor(std::string cursorName) override;

            //function: GetCurrentCustomCursor
            //See <SystemInputInterface::GetCurrentCustomCursor>
            void GetCurrentCustomCursor(ImageInterface& customCursor, 
                                        glm::ivec2& hotspot) const override;

            //function: GetCurrentCustomCursorName
            //See <SystemInputInterface::GetCurrentCustomCursorName>
            std::string GetCurrentCustomCursorName() const override;
            
            //function: GetCustomCursor
            //See <SystemInputInterface::GetCustomCursor>
            void GetCustomCursor(   ImageInterface& customCursor, 
                                    std::string cursorName, 
                                    glm::ivec2& hotspot) const override;

            //function: HasCustomCursor
            //See <SystemInputInterface::HasCustomCursor>
            bool HasCustomCursor(std::string cursorName) const override;

            //function: UpdateCursor
            //See <SystemInputInterface::UpdateCursor>
            void UpdateCursor() override;
            
            //function: AddRawEventHandler
            //See <SystemInputInterface::AddRawEventHandler>
            int AddRawEventHandler(std::function<bool(  MainWindowInterface*, 
                                                        void*)> handler) override;
            
            //function: RemoveRawEventHandler
            //See <SystemInputInterface::RemoveRawEventHandler>
            void RemoveRawEventHandler(int id) override;
            
            //function: ClearRawEventHandler
            //See <<SystemInputInterface::ClearRawEventHandler>
            void ClearRawEventHandler() override;

            //function: ClearClipboard
            //See <SystemInputInterface::ClearClipboard>
            bool ClearClipboard() override;

            //function: ClipbaordHasText
            //See <SystemInputInterface::ClipbaordHasText>
            bool ClipbaordHasText() const override;
            
            //function: ClipbaordHasImage
            //See <SystemInputInterface::ClipbaordHasImage>
            bool ClipbaordHasImage() const override;

            //function: SetClipboardImage
            //See <SystemInputInterface::SetClipboardImage>
            bool SetClipboardImage(const ImageInterface& imgData) override;
            
            //function: SetClipboardText
            //See <SystemInputInterface::SetClipboardText>
            bool SetClipboardText(const std::u32string& str) override;
            
            //function: SetClipboardText
            //See <SystemInputInterface::SetClipboardText>
            bool SetClipboardText(const std::string& str) override;
            
            //function: GetClipboardImage
            //See <SystemInputInterface::GetClipboardImage>
            bool GetClipboardImage(ImageInterface& imgData) const override;

            //function: GetClipboardText
            //See <SystemInputInterface::GetClipboardText>
            bool GetClipboardText(std::u32string& str) const override;

            //function: GetClipboardText
            //See <SystemInputInterface::GetClipboardText>
            bool GetClipboardText(std::string& str) const override;

            //function: GetElapsedTime
            //See <SystemInputInterface::GetElapsedTime>
            uint64_t GetElapsedTime() const override;
    };
}

}

#endif