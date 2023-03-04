#ifndef SSGUI_BACKEND_SYSTEM_INPUT_WIN32_OPEN_GL3_3_H
#define SSGUI_BACKEND_SYSTEM_INPUT_WIN32_OPEN_GL3_3_H

#include "ssGUI/Backend/Interfaces/BackendSystemInputInterface.hpp"

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
    class RealtimeInputInfo;
}


namespace ssGUI 
{ 
    
//namespace: ssGUI::Backend
namespace Backend
{
    /*class: ssGUI::Backend::BackendSystemInputInterface
    This class is the interface for getting all the inputs needed. 
    **There will be changes regarding on how to get key presses and realtime input info soon.**
    
    Variables & Constructor:
    ======================== C++ =======================
    private:
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

        std::unordered_map<HWND, ssGUI::Backend::BackendMainWindowInterface*> MainWindowRawHandles;     //(Internal variable) Used to store the HWND for mainWindow without getting it every single time

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
            <bool(ssGUI::Backend::BackendMainWindowInterface*, void*)>> RawEventHandlers;               //See <AddRawEventHandler>

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
        ssGUI::Backend::BackendManager::AddInputInterface(static_cast<ssGUI::Backend::BackendSystemInputInterface*>(this));
    }
    ====================================================
    */
    class BackendSystemInputWin32_OpenGL3_3 : public BackendSystemInputInterface
    {
        private:
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

            std::unordered_map<HWND, ssGUI::Backend::BackendMainWindowInterface*> MainWindowRawHandles;     //(Internal variable) Used to store the HWND for mainWindow without getting it every single time

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
                <bool(ssGUI::Backend::BackendMainWindowInterface*, void*)>> RawEventHandlers;               //See <AddRawEventHandler>

            const std::string SSGUI_EMPTY_CURSOR = "SSGUI_EMPTY_CURSOR";                                    //(Internal variable) Custom cursor for empty cursor

            template <class T>
            void AddNonExistElement(T elementToAdd, std::vector<T>& vectorAddTo);

            template <class T>
            void RemoveExistElement(T elementToRemove, std::vector<T>& vectorRemoveFrom);

            void FetchKeysPressed(ssGUI::Enums::GenericButtonAndKeyInput keysPressedDown, std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& destinationKeyPresses);
            void FetchKeysReleased(ssGUI::Enums::GenericButtonAndKeyInput keysReleased, std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& destinationKeyPresses);

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
            std::vector<ssGUI::RealtimeInputInfo> const & GetLastRealtimeInputs() const override;

            //function: GetCurrentRealtimeInputs
            //See <BackendSystemInputInterface::GetCurrentRealtimeInputs>
            std::vector<ssGUI::RealtimeInputInfo> const & GetCurrentRealtimeInputs() const override;

            //function: GetTextInput
            //See <BackendSystemInputInterface::GetTextInput>
            std::wstring GetTextInput() const override;
            
            //function: SetCursorType
            //Sets the cursor type it is currently using. 
            //By default, <ssGUI::ssGUIManager> sets the cursor type back to normal at the end of every frame.
            //Therefore you only need to set non normal cursor type every frame when you need to 
            //and don't need to care about setting it back to normal once you are done with it. 
            void SetCursorType(ssGUI::Enums::CursorType cursorType) override;

            //function: GetCursorType
            //See <BackendSystemInputInterface::SetCursorType>
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
            virtual int AddRawEventHandler(std::function<bool(ssGUI::Backend::BackendMainWindowInterface*, void*)> handler) override;
            
            //function: RemoveRawEventHandler
            virtual void RemoveRawEventHandler(int id) override;
            
            //function: ClearRawEventHandler
            virtual void ClearRawEventHandler() override;

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