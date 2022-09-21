#ifndef SSGUI_BACKEND_SYSTEM_INPUT_SFML
#define SSGUI_BACKEND_SYSTEM_INPUT_SFML

#include "ssGUI/Backend/Interfaces/BackendSystemInputInterface.hpp"
#include "ssGUI/Backend/BackendManager.hpp"
#include "ssGUI/Backend/SFML/BackendMainWindowSFML.hpp"
#include "ssGUI/Backend/SFML/SFMLInputConverter.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/HeaderGroups/KeyGroup.hpp"
#include "glm/vec2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"

#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <string>
#include <algorithm>

#if !USE_SFML_TIME
    #include <chrono>
#endif

//namespace ssGUI::Backend
namespace ssGUI::Backend
{
    /*class: ssGUI::Backend::BackendSystemInputSFML
    For functions explainations, please see <BackendSystemInputInterface>. Normally you don't need to deal with this class
    
    Variables & Constructor:
    ============================== C++ ==============================
    private:
        ssGUI::KeyPresses CurrentKeyPresses = ssGUI::KeyPresses();                                  //See <GetCurrentKeyPresses>
        ssGUI::KeyPresses LastKeyPresses = ssGUI::KeyPresses();                                     //See <GetLastKeyPresses>
        std::wstring InputText = L"";                                                               //See <GetTextInput>
        glm::ivec2 CurrentMousePosition = glm::ivec2();                                             //See <GetCurrentMousePosition>
        glm::ivec2 LastMousePosition = glm::ivec2();                                                //See <GetLastMousePosition>
        std::vector<ssGUI::Enums::MouseButton> CurrentMouseButtons = {};                            //See <GetCurrentMouseButton>
        std::vector<ssGUI::Enums::MouseButton> LastMouseButtons = {};                               //See <GetLastMouseButton>
        glm::vec2 MouseScrollDelta = glm::vec2();                                                   //See <GetCurrentMouseScrollDelta>
        std::vector<ssGUI::RealtimeInputInfo> CurrentInputInfos = {};                               //See <GetCurrentRealtimeInputs>
        std::vector<ssGUI::RealtimeInputInfo> LastInputInfos = {};                                  //See <GetLastRealtimeInputs>
        sf::Cursor SFMLCursor = sf::Cursor();                                                       //(Internal variable) Underlying cursor object
        ssGUI::Enums::CursorType CurrentCursor = ssGUI::Enums::CursorType::NORMAL;                  //See <GetCursorType>
        std::unordered_set<ssGUI::Backend::BackendMainWindowInterface*> CursorMappedWindow = {};    //TODO: Not used, remove it

        std::unordered_map<std::string, std::pair<sf::Image, glm::ivec2>> CustomCursors = {};       //See <GetCustomCursor>
        std::string CurrentCustomCursor = "";                                                       //See <GetCurrentCustomCursorName>

        #if USE_SFML_TIME
            sf::Clock ElapsedTime = sf::Clock();                                                    //See <GetElapsedTime>
        #else
            std::chrono::high_resolution_clock::time_point ElapsedTime = {};                        //See <GetElapsedTime>
        #endif
    =================================================================
    ============================== C++ ==============================
    BackendSystemInputSFML::BackendSystemInputSFML()
    {
        if(!SFMLCursor.loadFromSystem(sf::Cursor::Arrow))
        {
            DEBUG_LINE("Failed to load cursor!");
            DEBUG_EXIT_PROGRAM();
        }

        #if !USE_SFML_TIME
            ElapsedTime = std::chrono::high_resolution_clock::now();
        #endif

        ssGUI::Backend::BackendManager::AddInputInterface(static_cast<ssGUI::Backend::BackendSystemInputInterface*>(this));
    }
    =================================================================
    */
    class BackendSystemInputSFML : public BackendSystemInputInterface
    {
        private:
            ssGUI::KeyPresses CurrentKeyPresses = ssGUI::KeyPresses();                                  //See <GetCurrentKeyPresses>
            ssGUI::KeyPresses LastKeyPresses = ssGUI::KeyPresses();                                     //See <GetLastKeyPresses>
            std::wstring InputText = L"";                                                               //See <GetTextInput>
            glm::ivec2 CurrentMousePosition = glm::ivec2();                                             //See <GetCurrentMousePosition>
            glm::ivec2 LastMousePosition = glm::ivec2();                                                //See <GetLastMousePosition>
            std::vector<ssGUI::Enums::MouseButton> CurrentMouseButtons = {};                            //See <GetCurrentMouseButton>
            std::vector<ssGUI::Enums::MouseButton> LastMouseButtons = {};                               //See <GetLastMouseButton>
            glm::vec2 MouseScrollDelta = glm::vec2();                                                   //See <GetCurrentMouseScrollDelta>
            std::vector<ssGUI::RealtimeInputInfo> CurrentInputInfos = {};                               //See <GetCurrentRealtimeInputs>
            std::vector<ssGUI::RealtimeInputInfo> LastInputInfos = {};                                  //See <GetLastRealtimeInputs>
            sf::Cursor SFMLCursor = sf::Cursor();                                                       //(Internal variable) Underlying cursor object
            ssGUI::Enums::CursorType CurrentCursor = ssGUI::Enums::CursorType::NORMAL;                  //See <GetCursorType>
            std::unordered_set<ssGUI::Backend::BackendMainWindowInterface*> CursorMappedWindow = {};    //TODO: Not used, remove it

            std::unordered_map<std::string, std::pair<sf::Image, glm::ivec2>> CustomCursors = {};       //See <GetCustomCursor>
            std::string CurrentCustomCursor = "";                                                       //See <GetCurrentCustomCursorName>

            #if USE_SFML_TIME
                sf::Clock ElapsedTime = sf::Clock();                                                    //See <GetElapsedTime>
            #else
                std::chrono::high_resolution_clock::time_point ElapsedTime = {};                        //See <GetElapsedTime>
            #endif

            template <class T>
            void AddNonExistElements(std::vector<T>& elementsToAdd, std::vector<T>& vectorAddTo);

            template <class T>
            void RemoveExistElements(std::vector<T>& elementsToRemove, std::vector<T>& vectorRemoveFrom);

            void FetchKeysPressed(ssGUI::KeyPresses keysPressedDown, ssGUI::KeyPresses& destinationKeyPresses);
            void FetchKeysReleased(ssGUI::KeyPresses keysReleased, ssGUI::KeyPresses& destinationKeyPresses);

            //http://tech-algorithm.com/articles/bilinear-image-scaling/
            //https://stackoverflow.com/questions/21514075/bilinear-re-sizing-with-c-and-vector-of-rgba-pixels
            void ResizeBilinear(const uint8_t* inputPixels, int w, int h, uint8_t* outputPixels, int w2, int h2);
        
        public:
            BackendSystemInputSFML();
            ~BackendSystemInputSFML() override;

            //function: UpdateInput
            //See <BackendSystemInputInterface::UpdateInput>
            void UpdateInput() override;

            //function: GetLastKeyPresses
            //See <BackendSystemInputInterface::GetLastKeyPresses>
            ssGUI::KeyPresses const & GetLastKeyPresses() override;
            
            //function: GetCurrentKeyPresses
            //See <BackendSystemInputInterface::GetCurrentKeyPresses>
            ssGUI::KeyPresses const & GetCurrentKeyPresses() override;

            //function: GetLastMousePosition
            //See <BackendSystemInputInterface::GetLastMousePosition>
            glm::ivec2 GetLastMousePosition(ssGUI::MainWindow* mainWindow) const override;
            
            //function: GetCurrentMousePosition
            //See <BackendSystemInputInterface::GetCurrentMousePosition>
            glm::ivec2 GetCurrentMousePosition(ssGUI::MainWindow* mainWindow) const override;
            
            //function: SetMousePosition
            //See <BackendSystemInputInterface::SetMousePosition>
            void SetMousePosition(glm::ivec2 position, ssGUI::MainWindow* mainWindow) override;          

            //function: GetLastMouseButton
            //See <BackendSystemInputInterface::GetLastMouseButton>
            bool GetLastMouseButton(ssGUI::Enums::MouseButton button) const override;
            
            //function: GetCurrentMouseButton
            //See <BackendSystemInputInterface::GetCurrentMouseButton>
            bool GetCurrentMouseButton(ssGUI::Enums::MouseButton button) const override;

            //function: GetCurrentMouseScrollDelta
            //See <BackendSystemInputInterface::GetCurrentMouseScrollDelta>
            virtual glm::vec2 GetCurrentMouseScrollDelta() const override;

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
            //See <BackendSystemInputInterface::SetCursorType>
            void SetCursorType(ssGUI::Enums::CursorType cursorType) override;
            
            //function: GetCursorType
            //See <BackendSystemInputInterface::GetCursorType>
            ssGUI::Enums::CursorType GetCursorType() const override;

            //function: CreateCustomCursor
            //See <BackendSystemInputInterface::CreateCustomCursor>
            void CreateCustomCursor(ssGUI::ImageData* customCursor, std::string cursorName, glm::ivec2 cursorSize, glm::ivec2 hotspot) override;

            //function: SetCurrentCustomCursor
            //See <BackendSystemInputInterface::SetCurrentCustomCursor>
            void SetCurrentCustomCursor(std::string cursorName) override;

            //function: GetCurrentCustomCursor
            //See <BackendSystemInputInterface::GetCurrentCustomCursor>
            void GetCurrentCustomCursor(ssGUI::ImageData& customCursor, glm::ivec2& hotspot) override;

            //function: GetCurrentCustomCursorName
            //See <BackendSystemInputInterface::GetCurrentCustomCursorName>
            std::string GetCurrentCustomCursorName() override;
            
            //function: GetCustomCursor
            //See <BackendSystemInputInterface::GetCustomCursor>
            void GetCustomCursor(ssGUI::ImageData& customCursor, std::string cursorName, glm::ivec2& hotspot) override;
            
            //function: HasCustomCursor
            //See <BackendSystemInputInterface::HasCustomCursor>
            bool HasCustomCursor(std::string cursorName) override;
            
            //function: UpdateCursor
            //See <BackendSystemInputInterface::UpdateCursor>
            void UpdateCursor() override;

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
            bool SetClipboardImage(const ssGUI::ImageData& imgData) override;
            
            //function: SetClipboardText
            //See <BackendSystemInputInterface::SetClipboardText>            
            bool SetClipboardText(const std::wstring& str) override;
            
            //function: GetClipboardImage
            //See <BackendSystemInputInterface::GetClipboardImage>            
            bool GetClipboardImage(ssGUI::ImageData& imgData) override;

            //function: GetClipboardText
            //See <BackendSystemInputInterface::GetClipboardText>
            bool GetClipboardText(std::wstring& str) override;

            //function: GetElapsedTime
            //See <BackendSystemInputInterface::GetElapsedTime>
            uint64_t GetElapsedTime() const override;
    };
}


#endif