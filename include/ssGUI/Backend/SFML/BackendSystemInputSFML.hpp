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
        ssGUI::KeyPresses CurrentKeyPresses;
        ssGUI::KeyPresses LastKeyPresses;
        std::wstring InputText;
        glm::ivec2 CurrentMousePosition;
        glm::ivec2 LastMousePosition;
        std::vector<ssGUI::Enums::MouseButton> CurrentMouseButtons;
        std::vector<ssGUI::Enums::MouseButton> LastMouseButtons;
        std::vector<ssGUI::RealtimeInputInfo> CurrentInputInfos;
        std::vector<ssGUI::RealtimeInputInfo> LastInputInfos;
        sf::Cursor SFMLCursor;
        ssGUI::Enums::CursorType CurrentCursor;
        std::unordered_set<ssGUI::Backend::BackendMainWindowInterface*> CursorMappedWindow;

        sf::Image CustomCursorImage;
        glm::ivec2 Hotspot;

        #if USE_SFML_TIME
            sf::Clock ElapsedTime;
        #else
            std::chrono::high_resolution_clock::time_point ElapsedTime;
        #endif
    =================================================================
    ============================== C++ ==============================
    BackendSystemInputSFML::BackendSystemInputSFML() : CurrentKeyPresses(), LastKeyPresses(), InputText(), CurrentMousePosition(), LastMousePosition(),
                                            CurrentMouseButtons(), LastMouseButtons(), CurrentInputInfos(), LastInputInfos(), SFMLCursor(), 
                                            CurrentCursor(ssGUI::Enums::CursorType::NORMAL), CursorMappedWindow(), ElapsedTime(), CustomCursorImage(),
                                            Hotspot()
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
            ssGUI::KeyPresses CurrentKeyPresses;
            ssGUI::KeyPresses LastKeyPresses;
            std::wstring InputText;
            glm::ivec2 CurrentMousePosition;
            glm::ivec2 LastMousePosition;
            std::vector<ssGUI::Enums::MouseButton> CurrentMouseButtons;
            std::vector<ssGUI::Enums::MouseButton> LastMouseButtons;
            std::vector<ssGUI::RealtimeInputInfo> CurrentInputInfos;
            std::vector<ssGUI::RealtimeInputInfo> LastInputInfos;
            sf::Cursor SFMLCursor;
            ssGUI::Enums::CursorType CurrentCursor;
            std::unordered_set<ssGUI::Backend::BackendMainWindowInterface*> CursorMappedWindow;

            sf::Image CustomCursorImage;
            glm::ivec2 Hotspot;

            #if USE_SFML_TIME
                sf::Clock ElapsedTime;
            #else
                std::chrono::high_resolution_clock::time_point ElapsedTime;
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
            virtual ~BackendSystemInputSFML();

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

            //function: SetCustomCursor
            //See <BackendSystemInputInterface::SetCustomCursor>
            void SetCustomCursor(ssGUI::ImageData* customCursor, glm::ivec2 cursorSize, glm::ivec2 hotspot) override;

            //function: GetCustomCursor
            //See <BackendSystemInputInterface::GetCustomCursor>
            void GetCustomCursor(ssGUI::ImageData& customCursor, glm::ivec2& hotspot) override;
            
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