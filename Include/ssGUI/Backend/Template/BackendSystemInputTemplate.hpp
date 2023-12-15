#ifndef SSGUI_BACKEND_SYSTEM_INPUT_TEMPLATE_H
#define SSGUI_BACKEND_SYSTEM_INPUT_TEMPLATE_H

#include "ssGUI/Backend/Interfaces/BackendSystemInputInterface.hpp"


namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendSystemInputTemplate
    class BackendSystemInputTemplate : public BackendSystemInputInterface
    {   
        private:
            BackendSystemInputTemplate& operator=(BackendSystemInputTemplate const& other);

        protected:
            BackendSystemInputTemplate(BackendSystemInputTemplate const& other);

        public:
            BackendSystemInputTemplate();
            ~BackendSystemInputTemplate() override;
            
            //function: UpdateInput
            //See <BackendSystemInputInterface::UpdateInput>
            void UpdateInput() override;

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
            const std::vector<ssGUI::RealtimeInputInfo>& GetLastRealtimeInputs() const override;

            //function: GetCurrentRealtimeInputs
            //See <BackendSystemInputInterface::GetCurrentRealtimeInputs>
            const std::vector<ssGUI::RealtimeInputInfo>& GetCurrentRealtimeInputs() const override;

            //function: GetTextInput
            //See <BackendSystemInputInterface::GetTextInput>
            void GetTextInput(std::u32string& outText) const override;
            
            //function: GetTextInput
            //See <BackendSystemInputInterface::GetTextInput>
            void GetTextInput(std::string& outText) const override;
            
            //function: SetCursorType
            //See <BackendSystemInputInterface::SetCursorType>
            void SetCursorType(ssGUI::Enums::CursorType cursorType) override;

            //function: GetCursorType
            //See <BackendSystemInputInterface::GetCursorType>
            ssGUI::Enums::CursorType GetCursorType() const override;

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