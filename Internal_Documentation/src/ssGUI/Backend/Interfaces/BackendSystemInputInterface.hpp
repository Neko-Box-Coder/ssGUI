#ifndef SSGUI_BACKEND_SYSTEM_INPUT_INTERFACE
#define SSGUI_BACKEND_SYSTEM_INPUT_INTERFACE

#include "ssGUI/Backend/Interfaces/BackendMainWindowInterface.hpp"
#include <string>
#include "ssGUI/BaseClasses/KeyPresses.hpp"
#include "ssGUI/Enums/MouseButton.hpp"
#include <vector>
#include "glm/vec2.hpp"

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
            virtual glm::ivec2 GetLastMousePosition() const = 0;
            //function: GetCurrentMousePosition
            virtual glm::ivec2 GetCurrentMousePosition() const = 0;
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

            //function: UpdateCursor
            virtual void UpdateCursor() = 0;

            //function: GetElapsedTime
            virtual uint64_t GetElapsedTime() const = 0;
    };
}


#endif