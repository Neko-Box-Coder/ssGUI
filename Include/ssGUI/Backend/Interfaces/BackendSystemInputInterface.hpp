#ifndef SSGUI_BACKEND_SYSTEM_INPUT_INTERFACE_HPP
#define SSGUI_BACKEND_SYSTEM_INPUT_INTERFACE_HPP

#include <string>
#include <vector>
#include "ssGUI/DataClasses/RealtimeInputInfo.hpp"
#include "ssGUI/Backend/Interfaces/BackendMainWindowInterface.hpp"
#include "ssGUI/Enums/CursorType.hpp"
#include "ssGUI/Enums/MouseButton.hpp"

namespace ssGUI
{ 
    
    class GUIObject;
    class MainWindow;
    class ImageData;
    struct RealtimeInputInfo;
    
//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendSystemInputInterface
    //This class is the interface for getting all the inputs needed. 
    //**There will be changes regarding on how to get key presses and realtime input info soon.**
    class BackendSystemInputInterface
    {   
        public:
            BackendSystemInputInterface(){}
            virtual ~BackendSystemInputInterface() = 0;
            
            //function: UpdateInput
            //Poll and updates the input
            virtual void UpdateInput() = 0;

            using GenericInputs = ssGUI::Enums::GenericButtonAndKeyInput;

            //function: GetLastButtonAndKeyPresses
            //Get the key presses from last frame
            virtual const std::vector<GenericInputs>& GetLastButtonAndKeyPresses() = 0;
            
            //function: GetCurrentButtonAndKeyPresses
            //Get the key presses from current frame
            virtual const std::vector<GenericInputs>& GetCurrentButtonAndKeyPresses() = 0;

            //function: IsButtonOrKeyPressExistLastFrame
            //Check if the button or key is pressed last frame
            virtual bool IsButtonOrKeyPressExistLastFrame(GenericInputs input) const = 0;
            
            //function: IsButtonOrKeyPressExistLastFrame
            //Check if the button or key is pressed last frame
            template<typename T>
            bool IsButtonOrKeyPressExistLastFrame(T input) const
            {
                return IsButtonOrKeyPressExistLastFrame(static_cast<GenericInputs>(input));
            }

            //function: IsButtonOrKeyPressExistCurrentFrame
            //Check if the button or key is pressed current frame
            virtual bool IsButtonOrKeyPressExistCurrentFrame(GenericInputs input) const = 0;

            //function: IsButtonOrKeyPressExistCurrentFrame
            //Check if the button or key is pressed current frame
            template<typename T>
            bool IsButtonOrKeyPressExistCurrentFrame(T input) const
            {
                return IsButtonOrKeyPressExistCurrentFrame(static_cast<GenericInputs>(input));
            }

            //function: IsButtonOrKeyDown
            //See <IsButtonOrKeyPressExistCurrentFrame> and <IsButtonOrKeyPressExistLastFrame>
            template<typename T>
            bool IsButtonOrKeyDown(T input) const
            {
                return  IsButtonOrKeyPressExistCurrentFrame(static_cast<GenericInputs>(input)) && 
                        !IsButtonOrKeyPressExistLastFrame(static_cast<GenericInputs>(input));
            }

            //function: IsButtonOrKeyHeld
            //See <IsButtonOrKeyPressExistCurrentFrame> and <IsButtonOrKeyPressExistLastFrame>
            template<typename T>
            bool IsButtonOrKeyHeld(T input) const
            {
                return  IsButtonOrKeyPressExistCurrentFrame(static_cast<GenericInputs>(input)) && 
                        IsButtonOrKeyPressExistLastFrame(static_cast<GenericInputs>(input));
            }

            //function: IsButtonOrKeyUp
            //See <IsButtonOrKeyPressExistCurrentFrame> and <IsButtonOrKeyPressExistLastFrame>
            template<typename T>
            bool IsButtonOrKeyUp(T input) const
            {
                return  !IsButtonOrKeyPressExistCurrentFrame(static_cast<GenericInputs>(input)) && 
                        IsButtonOrKeyPressExistLastFrame(static_cast<GenericInputs>(input));
            }

            using MainWindowInterface = ssGUI::Backend::BackendMainWindowInterface;

            //function: GetLastMousePosition
            //Get mouse position relative to the mainWindow from last frame. 
            //If nullptr is passed, it will return global mouse position instead.
            virtual glm::ivec2 GetLastMousePosition(MainWindowInterface* mainWindow) const = 0;
            
            //function: GetCurrentMousePosition
            //Get mouse position relative to the mainWindow from current frame. 
            //If nullptr is passed, it will return global mouse position instead.
            virtual glm::ivec2 GetCurrentMousePosition(MainWindowInterface* mainWindow) const = 0;
            
            //function: SetMousePosition
            //Set the mouse posiition relative to the mainWindow. 
            //If nullptr is passed, it will set as global mouse position instead.
            virtual void SetMousePosition(glm::ivec2 position, MainWindowInterface* mainWindow) = 0;

            //function: GetLastMouseButton
            //Return if a mouse button is being pressed last frame
            virtual bool GetLastMouseButton(ssGUI::Enums::MouseButton button) const = 0;
            
            //function: GetCurrentMouseButton
            //Return if a mouse button is being pressed current frame
            virtual bool GetCurrentMouseButton(ssGUI::Enums::MouseButton button) const = 0;

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
            virtual glm::vec2 GetCurrentMouseScrollDelta() const = 0;

            //function: GetLastRealtimeInputs
            //Return a list of inputs in order happened in last frame. 
            //The last input info should be the same as <GetLastKeyPresses>, 
            //  <GetLastMousePosition>, etc...
            virtual const std::vector<ssGUI::RealtimeInputInfo>& GetLastRealtimeInputs() const = 0;

            //function: GetCurrentRealtimeInputs
            //Return a list of inputs in order happened in current frame. 
            //The last input info should be the same as <GetCurrentKeyPresses>, 
            //  <GetCurrentMouseButton>, etc...
            virtual const std::vector<ssGUI::RealtimeInputInfo>& GetCurrentRealtimeInputs() const = 0;

            //function: GetTextInput
            //Returns all the text typed in current frame.
            //Any characters in here 
            //  https://en.wikipedia.org/wiki/List_of_Unicode_characters#Control_codes
            //are not recorded in here.
            virtual void GetTextInput(std::wstring& outText) const = 0;
            
            //function: GetTextInput
            //See above
            virtual void GetTextInput(std::string& outText) const = 0;
            
            /*function: SetCursorType
            Sets the cursor type it is currently using. 
            To apply the changes, <UpdateCursor> needs to be called.
            
            By default, <ssGUI::ssGUIManager> sets the cursor type 
                back to normal at the beginning of every frame.
            
            Therefore you only need to set non normal cursor type every frame when you need to 
            and don't need to care about setting it back to normal once you are done with it. 
            */
            virtual void SetCursorType(ssGUI::Enums::CursorType cursorType) = 0;

            /*function: GetCursorType
            Gets the cursor type it is currently using. 
            Note that the return cursor type does not necessarily mean 
                the current cursor type being shown on the screen as <UpdateCursor> needs to be called
                to apply the changes.
            
            By default, <ssGUI::ssGUIManager> sets the cursor type 
            back to normal at the beginning of every frame.
            
            Therefore you only need to set non normal cursor type every frame when you need to 
            and don't need to care about setting it back to normal once you are done with it. 
            */
            virtual ssGUI::Enums::CursorType GetCursorType() const = 0;

            /*function: CreateCustomCursor
            Creates a custom cursor. 
            The image data of customCursor is copied so it is fine to dispose it if needed.
            The size of customCurrsor image will be resized to cursorSize if needed.
            The passed in hotspot must be smaller than cursorSize, (0, 0) as top-left corner.
            */
            virtual void CreateCustomCursor(ssGUI::Backend::BackendImageInterface* customCursor, 
                                            std::string cursorName, 
                                            glm::ivec2 cursorSize, 
                                            glm::ivec2 hotspot) = 0;
            
            //function: SetCurrentCustomCursor
            //Sets the current custom cursor from created custom cursor. 
            //Nothing is changed if the custom cursor name cannot be found.
            virtual void SetCurrentCustomCursor(std::string cursorName) = 0;

            //function: GetCurrentCustomCursor
            //Copies the (resized) current custom cursor image data to customCursor image data and 
            //  returns the hotspot of the cursor.
            //customCursor & hotspot are unchanged if there's no custom cursor.
            virtual void GetCurrentCustomCursor(ssGUI::Backend::BackendImageInterface& customCursor, 
                                                glm::ivec2& hotspot) = 0;

            //function: GetCurrentCustomCursorName
            //Gets the name of the current custom cursor. Empty string if nothing is set.
            virtual std::string GetCurrentCustomCursorName() = 0;
            
            //function: GetCustomCursor
            //Copies the (resized) custom cursor image data to customCursor image data and 
            //  returns the hotspot of the cursor.
            //customCursor & hotspot are unchanged if there's no custom cursor.
            virtual void GetCustomCursor(   ssGUI::Backend::BackendImageInterface& customCursor, 
                                            std::string cursorName, 
                                            glm::ivec2& hotspot) = 0;

            //function: HasCustomCursor
            //Returns if the target custom cursor with cursorName exists
            virtual bool HasCustomCursor(std::string cursorName) = 0;

            //function: UpdateCursor
            //Updates the cursor. This needs to be called after a new cursor is set.
            virtual void UpdateCursor() = 0;
            
            /*function: AddRawEventHandler
            Adds an event handler for processing before passing it to ssGUI 
                and returns an id for removal.
            
            The main window interface that receives the input and 
                also the input data will be passed to the handler.
            
            If the handler returns true, the event will be ingored by ssGUI and not passed down.
            */
            virtual int AddRawEventHandler(std::function<bool(MainWindowInterface*, void*)> handler) = 0;
            
            //function: RemoveRawEventHandler
            //Removes the event handler for processing before passing it to ssGUI.
            //For more details on raw event handler, see <AddRawEventHandler>.
            virtual void RemoveRawEventHandler(int id) = 0;
            
            //function: ClearRawEventHandler
            //Removes all the event handler for processing before passing it to ssGUI.
            //For more details on raw event handler, see <AddRawEventHandler>.
            virtual void ClearRawEventHandler() = 0;

            //function: ClearClipboard
            //Clears the clipboard
            virtual bool ClearClipboard() = 0;

            //function: ClipbaordHasText
            //Returns if the clipboard contains text
            virtual bool ClipbaordHasText() = 0;
            
            //function: ClipbaordHasImage
            //Returns if the clipboard contains image
            virtual bool ClipbaordHasImage() = 0;

            //function: SetClipboardImage
            //Sets the clipboard image
            virtual bool SetClipboardImage(const ssGUI::Backend::BackendImageInterface& imgData) = 0;
            
            //function: SetClipboardText
            //Sets the clipboard text
            virtual bool SetClipboardText(const std::wstring& str) = 0;
            
            //function: GetClipboardImage
            //Gets the clipboard image. It currently only supports 32-bit RGBA image
            virtual bool GetClipboardImage(ssGUI::Backend::BackendImageInterface& imgData) = 0;

            //function: GetClipboardText
            //Gets the clipboard text
            virtual bool GetClipboardText(std::wstring& str) = 0;

            //function: GetClipboardText
            //See above
            virtual bool GetClipboardText(std::string& str) = 0;

            //function: GetElapsedTime
            //Returns elapsed time since application startup in millisecond
            virtual uint64_t GetElapsedTime() const = 0;
    };
    
    //Pure virtual destructor needs to be defined
    inline BackendSystemInputInterface::~BackendSystemInputInterface(){}
}

}


#endif