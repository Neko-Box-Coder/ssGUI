#ifndef SSGUI_BACKEND_SYSTEM_INPUT_WIN32_OPEN_GL3_3
#define SSGUI_BACKEND_SYSTEM_INPUT_WIN32_OPEN_GL3_3

#include <string>
#include <vector>
#include "glm/vec2.hpp"
#include "ssGUI/DataClasses/RealtimeInputInfo.hpp"
#include "ssGUI/Backend/Interfaces/BackendMainWindowInterface.hpp"
#include "ssGUI/Enums/MouseButton.hpp"

namespace ssGUI
{
    class GUIObject;
    class MainWindow;
    class ImageData;
    class RealtimeInputInfo;
}


//namespace: ssGUI::Backend
namespace ssGUI::Backend
{
    //class: ssGUI::Backend::BackendSystemInputInterface
    //This class is the interface for getting all the inputs needed. 
    //**There will be changes regarding on how to get key presses and realtime input info soon.**
    class BackendSystemInputWin32_OpenGL3_3
    {   
        private:
            // BackendSystemInputInterface(const BackendSystemInputInterface&);
            // BackendSystemInputInterface& operator=(const BackendSystemInputInterface&);

        public:
            BackendSystemInputWin32_OpenGL3_3();
            ~BackendSystemInputWin32_OpenGL3_3() override;
            
            //function: UpdateInput
            //Poll and updates the input
            void UpdateInput();

            //function: GetLastButtonAndKeyPresses
            //Get the key presses from last frame
            const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& GetLastButtonAndKeyPresses();
            
            //function: GetCurrentButtonAndKeyPresses
            //Get the key presses from current frame
            const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& GetCurrentButtonAndKeyPresses();

            //function: IsButtonOrKeyPressExistLastFrame
            //Check if the button or key is pressed last frame
            bool IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const;
            
            //function: IsButtonOrKeyPressExistLastFrame
            //Check if the button or key is pressed last frame
            template<typename T>
            bool IsButtonOrKeyPressExistLastFrame(T input) const
            {
                return IsButtonOrKeyPressExistLastFrame(static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(input));
            };

            //function: IsButtonOrKeyPressExistCurrentFrame
            //Check if the button or key is pressed current frame
            bool IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const;

            //function: IsButtonOrKeyPressExistCurrentFrame
            //Check if the button or key is pressed current frame
            template<typename T>
            bool IsButtonOrKeyPressExistCurrentFrame(T input) const
            {
                return IsButtonOrKeyPressExistCurrentFrame(static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(input));
            };

            //function: GetLastMousePosition
            //Get mouse position relative to the mainWindow from last frame. If nullptr is passed, it will return global mouse position instead.
            glm::ivec2 GetLastMousePosition(ssGUI::MainWindow* mainWindow) const;
            
            //function: GetCurrentMousePosition
            //Get mouse position relative to the mainWindow from current frame. If nullptr is passed, it will return global mouse position instead.
            glm::ivec2 GetCurrentMousePosition(ssGUI::MainWindow* mainWindow) const;
            
            //function: SetMousePosition
            //Set the mouse posiition relative to the mainWindow. If nullptr is passed, it will set as global mouse position instead.
            void SetMousePosition(glm::ivec2 position, ssGUI::MainWindow* mainWindow);

            //function: GetLastMouseButton
            //Return if a mouse button is being pressed last frame
            bool GetLastMouseButton(ssGUI::Enums::MouseButton button) const;
            
            //function: GetCurrentMouseButton
            //Return if a mouse button is being pressed current frame
            bool GetCurrentMouseButton(ssGUI::Enums::MouseButton button) const;

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
            glm::vec2 GetCurrentMouseScrollDelta() const;

            //function: GetLastRealtimeInputs
            //Return a list of inputs in order happened in last frame. The last input info should be the same as <GetLastKeyPresses>, <GetLastMousePosition>, etc...
            std::vector<ssGUI::RealtimeInputInfo> const & GetLastRealtimeInputs() const;

            //function: GetCurrentRealtimeInputs
            //Return a list of inputs in order happened in current frame. The last input info should be the same as <GetCurrentKeyPresses>, <GetCurrentMouseButton>, etc...
            std::vector<ssGUI::RealtimeInputInfo> const & GetCurrentRealtimeInputs() const;

            //function: GetTextInput
            //Returns all the text typed in current frame.
            //Any characters in here https://en.wikipedia.org/wiki/List_of_Unicode_characters
            //are not recorded in here.
            std::wstring GetTextInput() const;
            
            //function: SetCursorType
            //Sets the cursor type it is currently using. 
            //By default, <ssGUI::ssGUIManager> sets the cursor type back to normal at the end of every frame.
            //Therefore you only need to set non normal cursor type every frame when you need to 
            //and don't need to care about setting it back to normal once you are done with it. 
            void SetCursorType(ssGUI::Enums::CursorType cursorType);

            //function: GetCursorType
            //Gets the cursor type it is currently using. 
            //By default, <ssGUI::ssGUIManager> sets the cursor type back to normal at the end of every frame.
            //Therefore you only need to set non normal cursor type every frame when you need to 
            //and don't need to care about setting it back to normal once you are done with it. 
            ssGUI::Enums::CursorType GetCursorType() const;

            //function: CreateCustomCursor
            //Creates a custom cursor. The image data of customCursor is copied so it is fine to dispose it if needed.
            //The size of customCurrsor image will be resized to cursorSize if needed.
            //The passed in hotspot must be smaller than cursorSize, (0, 0) as top-left corner.
            void CreateCustomCursor(ssGUI::ImageData* customCursor, std::string cursorName, glm::ivec2 cursorSize, glm::ivec2 hotspot);
            
            //function: SetCurrentCustomCursor
            //Sets the current custom cursor from created custom cursor. Nothing is changed if the custom cursor name cannot be found.
            void SetCurrentCustomCursor(std::string cursorName);

            //function: GetCurrentCustomCursor
            //Copies the (resized) current custom cursor image data to customCursor image data and returns the hotspot of the cursor.
            //customCursor & hotspot are unchanged if there's no custom cursor.
            void GetCurrentCustomCursor(ssGUI::ImageData& customCursor, glm::ivec2& hotspot);

            //function: GetCurrentCustomCursorName
            //Gets the name of the current custom cursor. Empty string if nothing is set.
            std::string GetCurrentCustomCursorName();
            
            //function: GetCustomCursor
            //Copies the (resized) custom cursor image data to customCursor image data and returns the hotspot of the cursor.
            //customCursor & hotspot are unchanged if there's no custom cursor.
            void GetCustomCursor(ssGUI::ImageData& customCursor, std::string cursorName, glm::ivec2& hotspot);

            //function: HasCustomCursor
            //Returns if the target custom cursor with cursorName exists
            bool HasCustomCursor(std::string cursorName);

            //function: UpdateCursor
            //Updates the cursor. This needs to be called after a new cursor is set.
            void UpdateCursor();

            //function: ClearClipboard
            //Clears the clipboard
            bool ClearClipboard();

            //function: ClipbaordHasText
            //Returns if the clipboard contains text
            bool ClipbaordHasText();
            
            //function: ClipbaordHasImage
            //Returns if the clipboard contains image
            bool ClipbaordHasImage();

            //function: SetClipboardImage
            //Sets the clipboard image
            bool SetClipboardImage(const ssGUI::ImageData& imgData);
            
            //function: SetClipboardText
            //Sets the clipboard text
            bool SetClipboardText(const std::wstring& str);
            
            //function: GetClipboardImage
            //Gets the clipboard image. It currently only supports 32-bit RGBA image
            bool GetClipboardImage(ssGUI::ImageData& imgData);

            //function: GetClipboardText
            //Gets the clipboard text
            bool GetClipboardText(std::wstring& str);

            //function: GetElapsedTime
            //Returns elapsed time since application startup in millisecond
            uint64_t GetElapsedTime() const;
    };
}


#endif