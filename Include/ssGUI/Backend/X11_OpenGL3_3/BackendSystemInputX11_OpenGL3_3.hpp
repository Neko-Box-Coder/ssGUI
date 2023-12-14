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
        std::vector<ssGUI::Enums::GenericInput> CurrentKeyPresses;                          //See <GetCurrentButtonAndKeyPresses>
        std::vector<ssGUI::Enums::GenericInput> LastKeyPresses;                             //See <GetLastButtonAndKeyPresses>
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
        std::vector<std::pair<Time, wchar_t>> InputCharsBuffer;                                         //(Internal variable) Characters buffer until IME finishes outputing
        Time LastKeyDownTime;                                                                           //(Internal variable) Tracking time to filter out redirected key down events
        Time LastKeyUpTime;                                                                             //(Internal variable) Tracking time to filter out redirected key up events
        std::vector<std::function
            <bool(ssGUI::Backend::BackendMainWindowInterface*, void*)>> RawEventHandlers;               //See <AddRawEventHandler>
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
                                                                            CursorHidden(false),
                                                                            LastKeyDownTime(0),
                                                                            LastKeyUpTime(0),
                                                                            RawEventHandlers()
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
            std::vector<ssGUI::Enums::GenericInput> CurrentKeyPresses;                          //See <GetCurrentButtonAndKeyPresses>
            std::vector<ssGUI::Enums::GenericInput> LastKeyPresses;                             //See <GetLastButtonAndKeyPresses>
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
            std::vector<std::pair<Time, wchar_t>> InputCharsBuffer;                                         //(Internal variable) Characters buffer until IME finishes outputing
            Time LastKeyDownTime;                                                                           //(Internal variable) Tracking time to filter out redirected key down events
            Time LastKeyUpTime;                                                                             //(Internal variable) Tracking time to filter out redirected key up events
            std::vector<std::function
                <bool(ssGUI::Backend::BackendMainWindowInterface*, void*)>> RawEventHandlers;               //See <AddRawEventHandler>
            uint64_t LastFrameTime;                                                                         //(Internal variable) Tracking FPS for resizing text input buffer

            template <class T>
            void AddNonExistElement(T elementToAdd, std::vector<T>& vectorAddTo);

            template <class T>
            void RemoveExistElement(T elementToRemove, std::vector<T>& vectorRemoveFrom);

            void FetchKeysPressed(ssGUI::Enums::GenericInput keysPressedDown, std::vector<ssGUI::Enums::GenericInput>& destinationKeyPresses);
            void FetchKeysReleased(ssGUI::Enums::GenericInput keysReleased, std::vector<ssGUI::Enums::GenericInput>& destinationKeyPresses);
        
            bool PopulateCursorDataHandles(CursorData& cursorData);
            float GetFPS(uint64_t curTimeInMs, uint64_t lastTimeInMs);
            int GetTextInputBufferSize(float fps);
        
            BackendSystemInputX11_OpenGL3_3& operator=(BackendSystemInputX11_OpenGL3_3 const& other);

        protected:
            BackendSystemInputX11_OpenGL3_3(BackendSystemInputX11_OpenGL3_3 const& other);

        public:
            BackendSystemInputX11_OpenGL3_3();
            ~BackendSystemInputX11_OpenGL3_3() override;
            
            void FetchEvents();
            
            //function: UpdateInput
            //See <BackendSystemInputInterface::UpdateInput>
            void UpdateInput() override;

            //function: GetLastButtonAndKeyPresses
            //See <BackendSystemInputInterface::GetLastButtonAndKeyPresses>
            const std::vector<ssGUI::Enums::GenericInput>& GetLastButtonAndKeyPresses() override;
            
            //function: GetCurrentButtonAndKeyPresses
            //See <BackendSystemInputInterface::GetCurrentButtonAndKeyPresses>
            const std::vector<ssGUI::Enums::GenericInput>& GetCurrentButtonAndKeyPresses() override;

            //function: IsButtonOrKeyPressExistLastFrame
            //See <BackendSystemInputInterface::IsButtonOrKeyPressExistLastFrame>
            bool IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::GenericInput input) const override;

            //function: IsButtonOrKeyPressExistCurrentFrame
            //See <BackendSystemInputInterface::IsButtonOrKeyPressExistCurrentFrame>
            bool IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::GenericInput input) const override;

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
            void GetTextInput(std::wstring& outText) const override;
            
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
            //See <BackendSystemInputInterface::ClearRawEventHandler>
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
            
            //function: GetClipboardText
            //See <BackendSystemInputInterface::GetClipboardText>
            bool GetClipboardText(std::string& str) override;

            //function: GetElapsedTime
            //See <BackendSystemInputInterface::GetElapsedTime>
            uint64_t GetElapsedTime() const override;
    };
}

}

#endif