#ifndef SSGUI_BACKEND_SYSTEM_INPUT_INTERFACE
#define SSGUI_BACKEND_SYSTEM_INPUT_INTERFACE

#include "ssGUI/Backend/Interfaces/BackendMainWindowInterface.hpp"
#include <string>
#include "ssGUI/DataClasses/KeyPresses.hpp"
#include "ssGUI/Enums/MouseButton.hpp"
#include <vector>
#include "glm/vec2.hpp"

namespace ssGUI
{
    class GUIObject;
    class MainWindow;
    class ImageData;
}


//namespace: ssGUI::Backend
namespace ssGUI::Backend
{
    //class: BackendSystemInputInterface
    class BackendSystemInputInterface
    {   
        private:
            // BackendSystemInputInterface(const BackendSystemInputInterface&);
            // BackendSystemInputInterface& operator=(const BackendSystemInputInterface&);

        public:
            BackendSystemInputInterface(){}
            //function: UpdateInput
            virtual void UpdateInput() = 0;

            //function: GetLastKeyPresses
            virtual ssGUI::KeyPresses const & GetLastKeyPresses() = 0;
            //function: GetCurrentKeyPresses
            virtual ssGUI::KeyPresses const & GetCurrentKeyPresses() = 0;

            //function: GetLastMousePosition
            virtual glm::ivec2 GetLastMousePosition(ssGUI::MainWindow* mainWindow) const = 0;
            //function: GetCurrentMousePosition
            virtual glm::ivec2 GetCurrentMousePosition(ssGUI::MainWindow* mainWindow) const = 0;
            //function: SetMousePosition
            virtual void SetMousePosition(glm::ivec2 position) = 0;

            //TODO : Add GetLastMousePosition(ssGUI::Backend::BackendMainWindowInterface* backendMainWindow) = 0; variant for other backend that can't poll global mouse position

            //function: GetLastMouseButton
            virtual bool GetLastMouseButton(ssGUI::Enums::MouseButton button) const = 0;
            //function: GetCurrentMouseButton
            virtual bool GetCurrentMouseButton(ssGUI::Enums::MouseButton button) const = 0;

            //function: GetTextInput
            virtual std::wstring GetTextInput() const = 0;
            
            //function: SetCursorType
            virtual void SetCursorType(ssGUI::Enums::CursorType cursorType) = 0;
            //function: GetCursorType
            virtual ssGUI::Enums::CursorType GetCursorType() const = 0;

            //function: SetCustomCursor
            //Sets the custom cursor image and hotspot.
            //The image data of customCursor is copied so it is fine to dispose it if needed.
            virtual void SetCustomCursor(ssGUI::ImageData* customCursor, glm::ivec2 cursorSize, glm::ivec2 hotspot) = 0;

            //function: GetCustomCursor
            //Copies the custom cursor image to customCursor and returns hotspot of cursor.
            //customCursor & hotspot are unchanged if there's no custom cursor.
            virtual void GetCustomCursor(ssGUI::ImageData& customCursor, glm::ivec2& hotspot) = 0;

            //function: UpdateCursor
            //Updates the cursor. This needs to be called after a new cursor is set.
            virtual void UpdateCursor() = 0;

            //function: GetElapsedTime
            //Returns elapsed time since application startup in millisecond
            virtual uint64_t GetElapsedTime() const = 0;
    };
}


#endif