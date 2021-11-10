#ifndef SSGUI_BACKEND_SYSTEM_INPUT_SFML
#define SSGUI_BACKEND_SYSTEM_INPUT_SFML

#include "ssGUI/Backend/Interfaces/BackendSystemInputInterface.hpp"
#include "ssGUI/Backend/BackendManager.hpp"
#include "ssGUI/Backend/SFML/BackendMainWindowSFML.hpp"
#include "ssGUI/Backend/SFML/SFMLInputConverter.hpp"
#include "ssGUI/ssGUIBuildConfig.hpp"
#include <unordered_set>
#include <memory>
#include <string>
#include <algorithm>
#include "glm/vec2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "ssGUI/HeaderGroups/KeyGroup.hpp"

#if !USE_SFML_TIME
    #include <chrono>
#endif

//namespace ssGUI::Backend
namespace ssGUI::Backend
{
    //class: BackendSystemInputSFML
    //For functions explainations, please see <BackendSystemInputInterface>. Normally you don't need to deal with this class
    class BackendSystemInputSFML : public BackendSystemInputInterface
    {           
        private:
            ssGUI::KeyPresses CurrentKeyPresses;
            ssGUI::KeyPresses LastKeyPresses;
            std::wstring InputText;
            glm::ivec2 CurrentMousePosition;
            glm::ivec2 LastMousePosition;
            std::vector<ssGUI::Enums::MouseButton> CurrentMouseButtons;
            std::vector<ssGUI::Enums::MouseButton> LastMouseButtons;
            sf::Cursor SFMLCursor;
            ssGUI::Enums::CursorType CurrentCursor;
            std::unordered_set<ssGUI::Backend::BackendMainWindowInterface*> CursorMappedWindow;

            #if USE_SFML_TIME
                sf::Clock ElapsedTime;
            #else
                std::chrono::high_resolution_clock::time_point ElapsedTime;
            #endif


            template <class T>
            void AddNonExistElements(std::vector<T>& elementsToAdd, std::vector<T>& vectorAddTo);

            template <class T>
            void RemoveExistElements(std::vector<T>& elementsToRemove, std::vector<T>& vectorRemoveFrom);

            void FetchKeysPressed(ssGUI::KeyPresses keysPressedDown);
            void FetchKeysReleased(ssGUI::KeyPresses keysReleased);
        
        public:
            BackendSystemInputSFML();
            virtual ~BackendSystemInputSFML();

            //function: UpdateInput
            void UpdateInput() override;

            //function: GetLastKeyPresses
            ssGUI::KeyPresses const & GetLastKeyPresses() override;
            
            //function: GetCurrentKeyPresses
            ssGUI::KeyPresses const & GetCurrentKeyPresses() override;

            //function: GetLastMousePosition
            glm::ivec2 GetLastMousePosition(ssGUI::GUIObject* mainWindow) const override;
            
            //function: GetCurrentMousePosition
            glm::ivec2 GetCurrentMousePosition(ssGUI::GUIObject* mainWindow) const override;
            
            //function: SetMousePosition
            void SetMousePosition(glm::ivec2 position) override;          

            //function: GetLastMouseButton
            bool GetLastMouseButton(ssGUI::Enums::MouseButton button) const override;
            
            //function: GetCurrentMouseButton
            bool GetCurrentMouseButton(ssGUI::Enums::MouseButton button) const override;

            //function: GetTextInput
            std::wstring GetTextInput() const override;

            //sf::Cursor* GetSFMLCursor();
            
            //function: SetCursorType
            void SetCursorType(ssGUI::Enums::CursorType cursorType) override;
            
            //function: GetCursorType
            ssGUI::Enums::CursorType GetCursorType() const override;
            
            //function: UpdateCursor
            void UpdateCursor() override;

            //function: GetElapsedTime
            uint64_t GetElapsedTime() const override;
    };
}


#endif