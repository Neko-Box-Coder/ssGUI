#ifndef SSGUI_BACKEND_SYSTEM_INPUT_TEMPLATE_HPP
#define SSGUI_BACKEND_SYSTEM_INPUT_TEMPLATE_HPP

#include "ssGUI/Backend/Interfaces/SystemInputInterface.hpp"


//TODO(NOW): Sync this with SDL2

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendSystemInputTemplate
    class BackendSystemInputTemplate : public SystemInputInterface
    {   
        private:
            std::vector<Enums::GenericInput> CurrentInputs;
            std::vector<Enums::GenericInput> LastInputs;
        
            std::vector<RealtimeInputInfo> CurrentRealtimeInputs;
            std::vector<RealtimeInputInfo> LastRealtimeInputs;
            
            BackendSystemInputTemplate& operator=(BackendSystemInputTemplate const& other);

        protected:
            BackendSystemInputTemplate(BackendSystemInputTemplate const& other);

        public:
            BackendSystemInputTemplate();
            ~BackendSystemInputTemplate() override;
            
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
            const std::vector<ssGUI::RealtimeInputInfo>& GetLastRealtimeInputs() const override;

            //function: GetCurrentRealtimeInputs
            //See <SystemInputInterface::GetCurrentRealtimeInputs>
            const std::vector<ssGUI::RealtimeInputInfo>& GetCurrentRealtimeInputs() const override;

            //function: GetTextInput
            //See <SystemInputInterface::GetTextInput>
            void GetTextInput(std::u32string& outText) const override;
            
            //function: GetTextInput
            //See <SystemInputInterface::GetTextInput>
            void GetTextInput(std::string& outText) const override;
            
            //function: SetCursorType
            //See <SystemInputInterface::SetCursorType>
            void SetCursorType(ssGUI::Enums::CursorType cursorType) override;

            //function: GetCursorType
            //See <SystemInputInterface::GetCursorType>
            ssGUI::Enums::CursorType GetCursorType() const override;

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