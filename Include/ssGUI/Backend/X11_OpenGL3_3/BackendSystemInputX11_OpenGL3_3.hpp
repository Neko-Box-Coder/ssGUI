#ifndef SSGUI_BACKEND_SYSTEM_INPUT_X11_OPEN_GL_3_3_H
#define SSGUI_BACKEND_SYSTEM_INPUT_X11_OPEN_GL_3_3_H

#include "ssGUI/Backend/Interfaces/BackendSystemInputInterface.hpp"

#include "ssGUI/Backend/BackendManager.hpp"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendSystemInputX11_OpenGL3_3
    class BackendSystemInputX11_OpenGL3_3 : public BackendSystemInputInterface
    {   
        private:
            BackendSystemInputX11_OpenGL3_3& operator=(BackendSystemInputX11_OpenGL3_3 const& other);

        protected:
            BackendSystemInputX11_OpenGL3_3(BackendSystemInputX11_OpenGL3_3 const& other);

        public:
            BackendSystemInputX11_OpenGL3_3();
            ~BackendSystemInputX11_OpenGL3_3() override;
            
            //function: UpdateInput
            //See <BackendMainWindowInterface::UpdateInput>
            void UpdateInput() override;

            //function: GetLastButtonAndKeyPresses
            //See <BackendMainWindowInterface::GetLastButtonAndKeyPresses>
            const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& GetLastButtonAndKeyPresses() override;
            
            //function: GetCurrentButtonAndKeyPresses
            //See <BackendMainWindowInterface::GetCurrentButtonAndKeyPresses>
            const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& GetCurrentButtonAndKeyPresses() override;

            //function: IsButtonOrKeyPressExistLastFrame
            //See <BackendMainWindowInterface::IsButtonOrKeyPressExistLastFrame>
            bool IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const override;

            //function: IsButtonOrKeyPressExistCurrentFrame
            //See <BackendMainWindowInterface::IsButtonOrKeyPressExistCurrentFrame>
            bool IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const override;

            //function: GetLastMousePosition
            //See <BackendMainWindowInterface::GetLastMousePosition>
            glm::ivec2 GetLastMousePosition(ssGUI::MainWindow* mainWindow) const override;
            
            //function: GetCurrentMousePosition
            //See <BackendMainWindowInterface::GetCurrentMousePosition>
            glm::ivec2 GetCurrentMousePosition(ssGUI::MainWindow* mainWindow) const override;
            
            //function: SetMousePosition
            //See <BackendMainWindowInterface::SetMousePosition>
            void SetMousePosition(glm::ivec2 position, ssGUI::MainWindow* mainWindow) override;

            //function: GetLastMouseButton
            //See <BackendMainWindowInterface::GetLastMouseButton>
            bool GetLastMouseButton(ssGUI::Enums::MouseButton button) const override;
            
            //function: GetCurrentMouseButton
            //See <BackendMainWindowInterface::GetCurrentMouseButton>
            bool GetCurrentMouseButton(ssGUI::Enums::MouseButton button) const override;

            //function: GetCurrentMouseScrollDelta
            //See <BackendMainWindowInterface::GetCurrentMouseScrollDelta>
            glm::vec2 GetCurrentMouseScrollDelta() const override;

            //function: GetLastRealtimeInputs
            //See <BackendMainWindowInterface::GetLastRealtimeInputs>
            std::vector<ssGUI::RealtimeInputInfo> const & GetLastRealtimeInputs() const override;

            //function: GetCurrentRealtimeInputs
            //See <BackendMainWindowInterface::GetCurrentRealtimeInputs>
            std::vector<ssGUI::RealtimeInputInfo> const & GetCurrentRealtimeInputs() const override;

            //function: GetTextInput
            //See <BackendMainWindowInterface::GetTextInput>
            std::wstring GetTextInput() const override;
            
            //function: SetCursorType
            //See <BackendMainWindowInterface::SetCursorType>
            void SetCursorType(ssGUI::Enums::CursorType cursorType) override;

            //function: GetCursorType
            //See <BackendMainWindowInterface::GetCursorType>
            ssGUI::Enums::CursorType GetCursorType() const override;

            //function: CreateCustomCursor
            //See <BackendMainWindowInterface::CreateCustomCursor>
            void CreateCustomCursor(ssGUI::ImageData* customCursor, std::string cursorName, glm::ivec2 cursorSize, glm::ivec2 hotspot) override;
            
            //function: SetCurrentCustomCursor
            //See <BackendMainWindowInterface::SetCurrentCustomCursor>
            void SetCurrentCustomCursor(std::string cursorName) override;

            //function: GetCurrentCustomCursor
            //See <BackendMainWindowInterface::GetCurrentCustomCursor>
            void GetCurrentCustomCursor(ssGUI::ImageData& customCursor, glm::ivec2& hotspot) override;

            //function: GetCurrentCustomCursorName
            //See <BackendMainWindowInterface::GetCurrentCustomCursorName>
            std::string GetCurrentCustomCursorName() override;
            
            //function: GetCustomCursor
            //See <BackendMainWindowInterface::GetCustomCursor>
            void GetCustomCursor(ssGUI::ImageData& customCursor, std::string cursorName, glm::ivec2& hotspot) override;

            //function: HasCustomCursor
            //See <BackendMainWindowInterface::HasCustomCursor>
            bool HasCustomCursor(std::string cursorName) override;

            //function: UpdateCursor
            //See <BackendMainWindowInterface::UpdateCursor>
            void UpdateCursor() override;

            //function: ClearClipboard
            //See <BackendMainWindowInterface::ClearClipboard>
            bool ClearClipboard() override;

            //function: ClipbaordHasText
            //See <BackendMainWindowInterface::ClipbaordHasText>
            bool ClipbaordHasText() override;
            
            //function: ClipbaordHasImage
            //See <BackendMainWindowInterface::ClipbaordHasImage>
            bool ClipbaordHasImage() override;

            //function: SetClipboardImage
            //See <BackendMainWindowInterface::SetClipboardImage>
            bool SetClipboardImage(const ssGUI::ImageData& imgData) override;
            
            //function: SetClipboardText
            //See <BackendMainWindowInterface::SetClipboardText>
            bool SetClipboardText(const std::wstring& str) override;
            
            //function: GetClipboardImage
            //See <BackendMainWindowInterface::GetClipboardImage>
            bool GetClipboardImage(ssGUI::ImageData& imgData) override;

            //function: GetClipboardText
            //See <BackendMainWindowInterface::GetClipboardText>
            bool GetClipboardText(std::wstring& str) override;

            //function: GetElapsedTime
            //See <BackendMainWindowInterface::GetElapsedTime>
            uint64_t GetElapsedTime() const override;
    };
}

}

#endif