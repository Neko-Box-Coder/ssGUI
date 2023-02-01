#ifndef SSGUI_BACKEND_SYSTEM_INPUT_X11_OPEN_GL_3_3_H
#define SSGUI_BACKEND_SYSTEM_INPUT_X11_OPEN_GL_3_3_H

#include "ssGUI/Backend/Interfaces/BackendSystemInputInterface.hpp"

#include "ssGUI/Backend/BackendManager.hpp"

#include <chrono>
#include <memory>
#include <unordered_map>
#include <X11/Xlib.h>

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    /*class: ssGUI::Backend::BackendSystemInputX11_OpenGL3_3
    This class is the interface for getting all the inputs needed. 
    **There will be changes regarding on how to get key presses and realtime input info soon.**
    
    Variables & Constructor:
    ======================== C++ =======================
    private:
        std::vector<XEvent> CurrentEvents;                                                              //(Internal variable) Allows polling events multiple times within a frame
        std::vector<ssGUI::Enums::GenericButtonAndKeyInput> CurrentKeyPresses;                          //See <GetCurrentButtonAndKeyPresses>
        std::vector<ssGUI::Enums::GenericButtonAndKeyInput> LastKeyPresses;                             //See <GetLastButtonAndKeyPresses>
        std::wstring InputText;                                                                         //See <GetTextInput>
        glm::ivec2 CurrentMousePosition;                                                                //See <GetCurrentMousePosition>
        glm::ivec2 LastMousePosition;                                                                   //See <GetLastMousePosition>
        std::vector<ssGUI::Enums::MouseButton> CurrentMouseButtons;                                     //See <GetCurrentMouseButton>
        std::vector<ssGUI::Enums::MouseButton> LastMouseButtons;                                        //See <GetLastMouseButton>
        glm::vec2 MouseScrollDelta;                                                                     //See <GetCurrentMouseScrollDelta>
        std::vector<ssGUI::RealtimeInputInfo> CurrentInputInfos;                                        //See <GetCurrentRealtimeInputs>
        std::vector<ssGUI::RealtimeInputInfo> LastInputInfos;                                           //See <GetLastRealtimeInputs>
        ssGUI::Enums::CursorType CurrentCursor;                                                         //See <GetCursorType>
        
        struct CursorData
        {
            std::shared_ptr<ssGUI::ImageData> CursorImage;
            glm::ivec2 Hotspot;
            std::unordered_map<Display*, Cursor> X11CursorHandles;
        };
        std::unordered_map<std::string, CursorData> CustomCursors;                                      //See <GetCustomCursor>
        std::string CurrentCustomCursor;                                                                //See <GetCurrentCustomCursorName>
        std::chrono::high_resolution_clock::time_point StartTime;                                       //See <GetElapsedTime>
        bool CursorHidden;                                                                              //(Internal variable) Flag to prevent showing/hiding cursor multiple times
    ====================================================
    ======================== C++ =======================
    BackendSystemInputX11_OpenGL3_3::BackendSystemInputX11_OpenGL3_3() :    CurrentEvents(),
                                                                            CurrentKeyPresses(),
                                                                            LastKeyPresses(),
                                                                            InputText(L""),
                                                                            CurrentMousePosition(),
                                                                            LastMousePosition(),
                                                                            CurrentMouseButtons(),
                                                                            LastMouseButtons(),
                                                                            MouseScrollDelta(),
                                                                            CurrentInputInfos(),
                                                                            LastInputInfos(),
                                                                            CurrentCursor(ssGUI::Enums::CursorType::NORMAL),
                                                                            CustomCursors(),
                                                                            CurrentCustomCursor(),
                                                                            StartTime(),
                                                                            CursorHidden(false)
    {
        StartTime = std::chrono::high_resolution_clock::now();
        ssGUI::Backend::BackendManager::AddInputInterface(static_cast<ssGUI::Backend::BackendSystemInputInterface*>(this));
    }
    ====================================================
    */
    class BackendSystemInputX11_OpenGL3_3 : public BackendSystemInputInterface
    {   
        private:
            std::vector<XEvent> CurrentEvents;                                                              //(Internal variable) Allows polling events multiple times within a frame
            std::vector<ssGUI::Enums::GenericButtonAndKeyInput> CurrentKeyPresses;                          //See <GetCurrentButtonAndKeyPresses>
            std::vector<ssGUI::Enums::GenericButtonAndKeyInput> LastKeyPresses;                             //See <GetLastButtonAndKeyPresses>
            std::wstring InputText;                                                                         //See <GetTextInput>
            glm::ivec2 CurrentMousePosition;                                                                //See <GetCurrentMousePosition>
            glm::ivec2 LastMousePosition;                                                                   //See <GetLastMousePosition>
            std::vector<ssGUI::Enums::MouseButton> CurrentMouseButtons;                                     //See <GetCurrentMouseButton>
            std::vector<ssGUI::Enums::MouseButton> LastMouseButtons;                                        //See <GetLastMouseButton>
            glm::vec2 MouseScrollDelta;                                                                     //See <GetCurrentMouseScrollDelta>
            std::vector<ssGUI::RealtimeInputInfo> CurrentInputInfos;                                        //See <GetCurrentRealtimeInputs>
            std::vector<ssGUI::RealtimeInputInfo> LastInputInfos;                                           //See <GetLastRealtimeInputs>
            ssGUI::Enums::CursorType CurrentCursor;                                                         //See <GetCursorType>
            
            struct CursorData
            {
                std::shared_ptr<ssGUI::ImageData> CursorImage;
                glm::ivec2 Hotspot;
                std::unordered_map<Display*, Cursor> X11CursorHandles;
            };
            std::unordered_map<std::string, CursorData> CustomCursors;                                      //See <GetCustomCursor>
            std::string CurrentCustomCursor;                                                                //See <GetCurrentCustomCursorName>
            std::chrono::high_resolution_clock::time_point StartTime;                                       //See <GetElapsedTime>
            bool CursorHidden;                                                                              //(Internal variable) Flag to prevent showing/hiding cursor multiple times
        
            template <class T>
            void AddNonExistElement(T elementToAdd, std::vector<T>& vectorAddTo);

            template <class T>
            void RemoveExistElement(T elementToRemove, std::vector<T>& vectorRemoveFrom);

            void FetchKeysPressed(ssGUI::Enums::GenericButtonAndKeyInput keysPressedDown, std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& destinationKeyPresses);
            void FetchKeysReleased(ssGUI::Enums::GenericButtonAndKeyInput keysReleased, std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& destinationKeyPresses);
        
            bool PopulateCursorDataHandles(CursorData& cursorData);
        
            BackendSystemInputX11_OpenGL3_3& operator=(BackendSystemInputX11_OpenGL3_3 const& other);

        protected:
            BackendSystemInputX11_OpenGL3_3(BackendSystemInputX11_OpenGL3_3 const& other);

        public:
            BackendSystemInputX11_OpenGL3_3();
            ~BackendSystemInputX11_OpenGL3_3() override;
            
            void FetchEvents();
            
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