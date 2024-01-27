#ifndef SSGUI_BACKEND_SYSTEM_INPUT_X11_OPEN_GL_3_3_H
#define SSGUI_BACKEND_SYSTEM_INPUT_X11_OPEN_GL_3_3_H

#include "ssGUI/Backend/Interfaces/SystemInputInterface.hpp"

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
        std::vector<ssGUI::Enums::GenericInput> CurrentKeyPresses;                          //See <GetCurrentInputs>
        std::vector<ssGUI::Enums::GenericInput> LastKeyPresses;                             //See <GetLastInputs>
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
            <bool(ssGUI::Backend::MainWindowInterface*, void*)>> RawEventHandlers;               //See <AddRawEventHandler>
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
        ssGUI::Backend::BackendManager::AddInputInterface(static_cast<ssGUI::Backend::SystemInputInterface*>(this));
    }
    ====================================================
    */
    class BackendSystemInputX11_OpenGL3_3 : public SystemInputInterface
    {   
        private:
            std::vector<XEvent> CurrentEvents;                                                              //(Internal variable) Allows polling events multiple times within a frame
            std::vector<ssGUI::Enums::GenericInput> CurrentKeyPresses;                          //See <GetCurrentInputs>
            std::vector<ssGUI::Enums::GenericInput> LastKeyPresses;                             //See <GetLastInputs>
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
                <bool(ssGUI::Backend::MainWindowInterface*, void*)>> RawEventHandlers;               //See <AddRawEventHandler>
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
            //See <SystemInputInterface::UpdateInput>
            void UpdateInput() override;

            //function: GetLastInputs
            //See <SystemInputInterface::GetLastInputs>
            const std::vector<ssGUI::Enums::GenericInput>& GetLastInputs() override;
            
            //function: GetCurrentInputs
            //See <SystemInputInterface::GetCurrentInputs>
            const std::vector<ssGUI::Enums::GenericInput>& GetCurrentInputs() override;

            //function: IsInputExistLastFrame
            //See <SystemInputInterface::IsInputExistLastFrame>
            bool IsInputExistLastFrame(ssGUI::Enums::GenericInput input) const override;

            //function: IsInputExistCurrentFrame
            //See <SystemInputInterface::IsInputExistCurrentFrame>
            bool IsInputExistCurrentFrame(ssGUI::Enums::GenericInput input) const override;

            //function: GetLastMousePosition
            //See <SystemInputInterface::GetLastMousePosition>
            glm::ivec2 GetLastMousePosition(ssGUI::Backend::MainWindowInterface* mainWindow) const override;
            
            //function: GetCurrentMousePosition
            //See <SystemInputInterface::GetCurrentMousePosition>
            glm::ivec2 GetCurrentMousePosition(ssGUI::Backend::MainWindowInterface* mainWindow) const override;
            
            //function: SetMousePosition
            //See <SystemInputInterface::SetMousePosition>
            void SetMousePosition(glm::ivec2 position, ssGUI::Backend::MainWindowInterface* mainWindow) override;

            //function: GetLastMouseButton
            //See <SystemInputInterface::GetLastMouseButton>
            bool GetLastMouseButton(ssGUI::Enums::MouseButton button) const override;
            
            //function: GetCurrentMouseButton
            //See <SystemInputInterface::GetCurrentMouseButton>
            bool GetCurrentMouseButton(ssGUI::Enums::MouseButton button) const override;

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
            void GetTextInput(std::wstring& outText) const override;
            
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
            void CreateCustomCursor(ssGUI::Backend::ImageInterface* customCursor, std::string cursorName, glm::ivec2 cursorSize, glm::ivec2 hotspot) override;
            
            //function: SetCurrentCustomCursor
            //See <SystemInputInterface::SetCurrentCustomCursor>
            void SetCurrentCustomCursor(std::string cursorName) override;

            //function: GetCurrentCustomCursor
            //See <SystemInputInterface::GetCurrentCustomCursor>
            void GetCurrentCustomCursor(ssGUI::Backend::ImageInterface& customCursor, glm::ivec2& hotspot) override;

            //function: GetCurrentCustomCursorName
            //See <SystemInputInterface::GetCurrentCustomCursorName>
            std::string GetCurrentCustomCursorName() override;
            
            //function: GetCustomCursor
            //See <SystemInputInterface::GetCustomCursor>
            void GetCustomCursor(ssGUI::Backend::ImageInterface& customCursor, std::string cursorName, glm::ivec2& hotspot) override;

            //function: HasCustomCursor
            //See <SystemInputInterface::HasCustomCursor>
            bool HasCustomCursor(std::string cursorName) override;

            //function: UpdateCursor
            //See <SystemInputInterface::UpdateCursor>
            void UpdateCursor() override;
            
            //function: AddRawEventHandler
            //See <SystemInputInterface::AddRawEventHandler>
            int AddRawEventHandler(std::function<bool(ssGUI::Backend::MainWindowInterface*, void*)> handler) override;
            
            //function: RemoveRawEventHandler
            //See <SystemInputInterface::RemoveRawEventHandler>
            void RemoveRawEventHandler(int id) override;
            
            //function: ClearRawEventHandler
            //See <SystemInputInterface::ClearRawEventHandler>
            void ClearRawEventHandler() override;

            //function: ClearClipboard
            //See <SystemInputInterface::ClearClipboard>
            bool ClearClipboard() override;

            //function: ClipbaordHasText
            //See <SystemInputInterface::ClipbaordHasText>
            bool ClipbaordHasText() override;
            
            //function: ClipbaordHasImage
            //See <SystemInputInterface::ClipbaordHasImage>
            bool ClipbaordHasImage() override;

            //function: SetClipboardImage
            //See <SystemInputInterface::SetClipboardImage>
            bool SetClipboardImage(const ssGUI::Backend::ImageInterface& imgData) override;
            
            //function: SetClipboardText
            //See <SystemInputInterface::SetClipboardText>
            bool SetClipboardText(const std::wstring& str) override;
            
            //function: GetClipboardImage
            //See <SystemInputInterface::GetClipboardImage>
            bool GetClipboardImage(ssGUI::Backend::ImageInterface& imgData) override;

            //function: GetClipboardText
            //See <SystemInputInterface::GetClipboardText>
            bool GetClipboardText(std::wstring& str) override;
            
            //function: GetClipboardText
            //See <SystemInputInterface::GetClipboardText>
            bool GetClipboardText(std::string& str) override;

            //function: GetElapsedTime
            //See <SystemInputInterface::GetElapsedTime>
            uint64_t GetElapsedTime() const override;
    };
}

}

#endif