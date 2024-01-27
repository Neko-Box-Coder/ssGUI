#ifndef SSGUI_BACKEND_SYSTEM_INPUT_WIN32_OPEN_GL3_3_H
#define SSGUI_BACKEND_SYSTEM_INPUT_WIN32_OPEN_GL3_3_H

#include "ssGUI/Backend/Interfaces/SystemInputInterface.hpp"

#ifndef UNICODE
#define UNICODE
#define _UNICODE
#endif

#include <windows.h>            /* must include this before GL/gl.h */

#include <queue>
#include <utility>
#include <unordered_map>
#include <memory>
#include <chrono>

namespace ssGUI
{
    class GUIObject;
    class MainWindow;
    class ImageData;
    struct RealtimeInputInfo;
}


namespace ssGUI 
{ 
    
//namespace: ssGUI::Backend
namespace Backend
{
    /*class: ssGUI::Backend::SystemInputInterface
    This class is the interface for getting all the inputs needed. 
    **There will be changes regarding on how to get key presses and realtime input info soon.**
    
    Variables & Constructor:
    ======================== C++ =======================
    private:
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

        std::unordered_map<HWND, ssGUI::Backend::MainWindowInterface*> MainWindowRawHandles;     //(Internal variable) Used to store the HWND for mainWindow without getting it every single time

        struct CursorData
        {
            std::shared_ptr<ssGUI::ImageData> CursorImage;
            glm::ivec2 Hotspot;
            HCURSOR Win32CursorHandle;
        };
        std::unordered_map<std::string, CursorData> CustomCursors;                                      //See <GetCustomCursor>
        std::string CurrentCustomCursor;                                                                //See <GetCurrentCustomCursorName>

        std::chrono::high_resolution_clock::time_point StartTime;                                       //See <GetElapsedTime>
        std::vector<std::function
            <bool(ssGUI::Backend::MainWindowInterface*, void*)>> RawEventHandlers;               //See <AddRawEventHandler>

        const std::string SSGUI_EMPTY_CURSOR = "SSGUI_EMPTY_CURSOR";                                    //(Internal variable) Custom cursor for empty cursor
    ====================================================
    ======================== C++ =======================
    BackendSystemInputWin32_OpenGL3_3::BackendSystemInputWin32_OpenGL3_3() :    CurrentKeyPresses(),
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
                                                                                MainWindowRawHandles(),
                                                                                CustomCursors(),
                                                                                CurrentCustomCursor(""),
                                                                                StartTime(),
                                                                                RawEventHandlers()
    {
        StartTime = std::chrono::high_resolution_clock::now();
        ssGUI::Backend::BackendManager::AddInputInterface(static_cast<ssGUI::Backend::SystemInputInterface*>(this));
    }
    ====================================================
    */
    class BackendSystemInputWin32_OpenGL3_3 : public SystemInputInterface
    {
        private:
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

            std::unordered_map<HWND, ssGUI::Backend::MainWindowInterface*> MainWindowRawHandles;     //(Internal variable) Used to store the HWND for mainWindow without getting it every single time

            struct CursorData
            {
                std::shared_ptr<ssGUI::ImageData> CursorImage;
                glm::ivec2 Hotspot;
                HCURSOR Win32CursorHandle;
            };
            std::unordered_map<std::string, CursorData> CustomCursors;                                      //See <GetCustomCursor>
            std::string CurrentCustomCursor;                                                                //See <GetCurrentCustomCursorName>

            std::chrono::high_resolution_clock::time_point StartTime;                                       //See <GetElapsedTime>
            std::vector<std::function
                <bool(ssGUI::Backend::MainWindowInterface*, void*)>> RawEventHandlers;               //See <AddRawEventHandler>

            const std::string SSGUI_EMPTY_CURSOR = "SSGUI_EMPTY_CURSOR";                                    //(Internal variable) Custom cursor for empty cursor

            template <class T>
            void AddNonExistElement(T elementToAdd, std::vector<T>& vectorAddTo);

            template <class T>
            void RemoveExistElement(T elementToRemove, std::vector<T>& vectorRemoveFrom);

            void FetchKeysPressed(ssGUI::Enums::GenericInput keysPressedDown, std::vector<ssGUI::Enums::GenericInput>& destinationKeyPresses);
            void FetchKeysReleased(ssGUI::Enums::GenericInput keysReleased, std::vector<ssGUI::Enums::GenericInput>& destinationKeyPresses);

            //Creates Win32 Cursor Handle from image data and hotspot.
            //imgData has to be BGRA32 (8 bit each channel), otherwise this will return nullptr
            HCURSOR CreateWin32Cursor(glm::ivec2 hotspot, ssGUI::ImageData* imgData);

            void CreateEmptyCursorIfNeeded();

        public:
            BackendSystemInputWin32_OpenGL3_3();
            ~BackendSystemInputWin32_OpenGL3_3() override;
            
            //function: HandleMessage
            //(Internal ssGUI function) Used by BackendMainWindow to pass Win32 event to here
            bool HandleMessage(MSG msg);

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
            std::wstring GetTextInput() const override;
            
            //function: SetCursorType
            //Sets the cursor type it is currently using. 
            //By default, <ssGUI::ssGUIManager> sets the cursor type back to normal at the end of every frame.
            //
            //Therefore you only need to set non normal cursor type every frame when you need to 
            //and don't need to care about setting it back to normal once you are done with it. 
            void SetCursorType(ssGUI::Enums::CursorType cursorType) override;

            //function: GetCursorType
            //See <SystemInputInterface::SetCursorType>
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
            virtual int AddRawEventHandler(std::function<bool(ssGUI::Backend::MainWindowInterface*, void*)> handler) override;
            
            //function: RemoveRawEventHandler
            virtual void RemoveRawEventHandler(int id) override;
            
            //function: ClearRawEventHandler
            virtual void ClearRawEventHandler() override;

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

            //function: GetElapsedTime
            //See <SystemInputInterface::GetElapsedTime>
            uint64_t GetElapsedTime() const override;
    };
} 

}


#endif