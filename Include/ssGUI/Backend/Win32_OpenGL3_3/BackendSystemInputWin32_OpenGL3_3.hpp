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
    //class: ssGUI::Backend::BackendSystemInputInterface
    //This class is the interface for getting all the inputs needed. 
    //**There will be changes regarding on how to get key presses and realtime input info soon.**
    class BackendSystemInputWin32_OpenGL3_3 : public BackendSystemInputInterface
    {
        private:
            // BackendSystemInputInterface(const BackendSystemInputInterface&);
            // BackendSystemInputInterface& operator=(const BackendSystemInputInterface&);
            std::vector<ssGUI::Enums::GenericButtonAndKeyInput> CurrentKeyPresses;                  //See <GetCurrentButtonAndKeyPresses>
            std::vector<ssGUI::Enums::GenericButtonAndKeyInput> LastKeyPresses;                     //See <GetLastButtonAndKeyPresses>
            std::wstring InputText;                                                                 //See <GetTextInput>
            glm::ivec2 CurrentMousePosition;                                                        //See <GetCurrentMousePosition>
            glm::ivec2 LastMousePosition;                                                           //See <GetLastMousePosition>
            std::vector<ssGUI::Enums::MouseButton> CurrentMouseButtons;                             //See <GetCurrentMouseButton>
            std::vector<ssGUI::Enums::MouseButton> LastMouseButtons;                                //See <GetLastMouseButton>
            glm::vec2 MouseScrollDelta;                                                             //See <GetCurrentMouseScrollDelta>
            std::vector<ssGUI::RealtimeInputInfo> CurrentInputInfos;                                //See <GetCurrentRealtimeInputs>
            std::vector<ssGUI::RealtimeInputInfo> LastInputInfos;                                   //See <GetLastRealtimeInputs>
            ssGUI::Enums::CursorType CurrentCursor;                                                 //See <GetCursorType>

            std::unordered_map<HWND, ssGUI::Backend::BackendMainWindowInterface*> MainWindowRawHandles;

            struct CursorData
            {
                std::shared_ptr<ssGUI::ImageData> CursorImage;
                glm::ivec2 Hotspot;
                HCURSOR Win32CursorHandle;
            };
            std::unordered_map<std::string, CursorData> CustomCursors;        //See <GetCustomCursor>
            std::string CurrentCustomCursor;                                                        //See <GetCurrentCustomCursorName>

            std::chrono::high_resolution_clock::time_point StartTime;                         //See <GetElapsedTime>

            const std::string SSGUI_EMPTY_CURSOR = "SSGUI_EMPTY_CURSOR";

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
            
            bool HandleMessage(MSG msg);

            //function: UpdateInput
            //Poll and updates the input
            void UpdateInput() override;

            //function: GetLastButtonAndKeyPresses
            //Get the key presses from last frame
            const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& GetLastButtonAndKeyPresses() override;
            
            //function: GetCurrentButtonAndKeyPresses
            //Get the key presses from current frame
            const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& GetCurrentButtonAndKeyPresses() override;

            //function: IsButtonOrKeyPressExistLastFrame
            //Check if the button or key is pressed last frame
            bool IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const override;
            

            //function: IsButtonOrKeyPressExistCurrentFrame
            //Check if the button or key is pressed current frame
            bool IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const override;

            //function: GetLastMousePosition
            //Get mouse position relative to the mainWindow from last frame. If nullptr is passed, it will return global mouse position instead.
            glm::ivec2 GetLastMousePosition(ssGUI::MainWindow* mainWindow) const override;
            
            //function: GetCurrentMousePosition
            //Get mouse position relative to the mainWindow from current frame. If nullptr is passed, it will return global mouse position instead.
            glm::ivec2 GetCurrentMousePosition(ssGUI::MainWindow* mainWindow) const override;
            
            //function: SetMousePosition
            //Set the mouse posiition relative to the mainWindow. If nullptr is passed, it will set as global mouse position instead.
            void SetMousePosition(glm::ivec2 position, ssGUI::MainWindow* mainWindow) override;

            //function: GetLastMouseButton
            //Return if a mouse button is being pressed last frame
            bool GetLastMouseButton(ssGUI::Enums::MouseButton button) const override;
            
            //function: GetCurrentMouseButton
            //Return if a mouse button is being pressed current frame
            bool GetCurrentMouseButton(ssGUI::Enums::MouseButton button) const override;

            /*function: GetCurrentMouseScrollDelta
            Return the direction of mouse scrolling for the current frame.
            ================== Text ==================
                 ▲ 1
            -1   │    1
            ◄────┼────►
                 │
                 ▼-1
            ==========================================
            */
            glm::vec2 GetCurrentMouseScrollDelta() const override;

            //function: GetLastRealtimeInputs
            //Return a list of inputs in order happened in last frame. The last input info should be the same as <GetLastKeyPresses>, <GetLastMousePosition>, etc...
            std::vector<ssGUI::RealtimeInputInfo> const & GetLastRealtimeInputs() const override;

            //function: GetCurrentRealtimeInputs
            //Return a list of inputs in order happened in current frame. The last input info should be the same as <GetCurrentKeyPresses>, <GetCurrentMouseButton>, etc...
            std::vector<ssGUI::RealtimeInputInfo> const & GetCurrentRealtimeInputs() const override;

            //function: GetTextInput
            //Returns all the text typed in current frame.
            //Any characters in here https://en.wikipedia.org/wiki/List_of_Unicode_characters
            //are not recorded in here.
            std::wstring GetTextInput() const override;
            
            //function: SetCursorType
            //Sets the cursor type it is currently using. 
            //By default, <ssGUI::ssGUIManager> sets the cursor type back to normal at the end of every frame.
            //Therefore you only need to set non normal cursor type every frame when you need to 
            //and don't need to care about setting it back to normal once you are done with it. 
            void SetCursorType(ssGUI::Enums::CursorType cursorType) override;

            //function: GetCursorType
            //Gets the cursor type it is currently using. 
            //By default, <ssGUI::ssGUIManager> sets the cursor type back to normal at the end of every frame.
            //Therefore you only need to set non normal cursor type every frame when you need to 
            //and don't need to care about setting it back to normal once you are done with it. 
            ssGUI::Enums::CursorType GetCursorType() const override;

            //function: CreateCustomCursor
            //Creates a custom cursor. The image data of customCursor is copied so it is fine to dispose it if needed.
            //The size of customCurrsor image will be resized to cursorSize if needed.
            //The passed in hotspot must be smaller than cursorSize, (0, 0) as top-left corner.
            void CreateCustomCursor(ssGUI::ImageData* customCursor, std::string cursorName, glm::ivec2 cursorSize, glm::ivec2 hotspot) override;
            
            //function: SetCurrentCustomCursor
            //Sets the current custom cursor from created custom cursor. Nothing is changed if the custom cursor name cannot be found.
            void SetCurrentCustomCursor(std::string cursorName) override;

            //function: GetCurrentCustomCursor
            //Copies the (resized) current custom cursor image data to customCursor image data and returns the hotspot of the cursor.
            //customCursor & hotspot are unchanged if there's no custom cursor.
            void GetCurrentCustomCursor(ssGUI::ImageData& customCursor, glm::ivec2& hotspot) override;

            //function: GetCurrentCustomCursorName
            //Gets the name of the current custom cursor. Empty string if nothing is set.
            std::string GetCurrentCustomCursorName() override;
            
            //function: GetCustomCursor
            //Copies the (resized) custom cursor image data to customCursor image data and returns the hotspot of the cursor.
            //customCursor & hotspot are unchanged if there's no custom cursor.
            void GetCustomCursor(ssGUI::ImageData& customCursor, std::string cursorName, glm::ivec2& hotspot) override;

            //function: HasCustomCursor
            //Returns if the target custom cursor with cursorName exists
            bool HasCustomCursor(std::string cursorName) override;

            //function: UpdateCursor
            //Updates the cursor. This needs to be called after a new cursor is set.
            void UpdateCursor() override;

            //function: ClearClipboard
            //Clears the clipboard
            bool ClearClipboard() override;

            //function: ClipbaordHasText
            //Returns if the clipboard contains text
            bool ClipbaordHasText() override;
            
            //function: ClipbaordHasImage
            //Returns if the clipboard contains image
            bool ClipbaordHasImage() override;

            //function: SetClipboardImage
            //Sets the clipboard image
            bool SetClipboardImage(const ssGUI::ImageData& imgData) override;
            
            //function: SetClipboardText
            //Sets the clipboard text
            bool SetClipboardText(const std::wstring& str) override;
            
            //function: GetClipboardImage
            //Gets the clipboard image. It currently only supports 32-bit RGBA image
            bool GetClipboardImage(ssGUI::ImageData& imgData) override;

            //function: GetClipboardText
            //Gets the clipboard text
            bool GetClipboardText(std::wstring& str) override;

            //function: GetElapsedTime
            //Returns elapsed time since application startup in millisecond
            uint64_t GetElapsedTime() const override;
    };
} 

}


#endif