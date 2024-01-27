#ifndef SSGUI_BACKEND_SYSTEM_INPUT_SFML_H
#define SSGUI_BACKEND_SYSTEM_INPUT_SFML_H

#include "ssGUI/Backend/Interfaces/SystemInputInterface.hpp"
#include "ssGUI/Backend/BackendManager.hpp"
#include "ssGUI/Backend/SFML/BackendMainWindowSFML.hpp"
#include "ssGUI/Backend/SFML/SFMLInputConverter.hpp"
#include "ssGUI/HeaderGroups/InputGroup.hpp"
#include "glm/vec2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"

#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <string>
#include <algorithm>

#if !SSGUI_USE_SFML_TIME
    #include <chrono>
#endif

namespace ssGUI 
{ 
    
//namespace ssGUI::Backend
namespace Backend
{
    /*class: ssGUI::Backend::BackendSystemInputSFML
    For functions explainations, please see <SystemInputInterface>. Normally you don't need to deal with this class
    
    Variables & Constructor:
    ============================== C++ ==============================
    private:
        ssGUI::KeyPresses CurrentKeyPresses;                                                    //See <GetCurrentKeyPresses>
        ssGUI::KeyPresses LastKeyPresses;                                                       //See <GetLastKeyPresses>
        std::wstring InputText;                                                                 //See <GetTextInput>
        glm::ivec2 CurrentMousePosition;                                                        //See <GetCurrentMousePosition>
        glm::ivec2 LastMousePosition;                                                           //See <GetLastMousePosition>
        std::vector<ssGUI::Enums::MouseButton> CurrentMouseButtons;                             //See <GetCurrentMouseButton>
        std::vector<ssGUI::Enums::MouseButton> LastMouseButtons;                                //See <GetLastMouseButton>
        glm::vec2 MouseScrollDelta;                                                             //See <GetCurrentMouseScrollDelta>
        std::vector<ssGUI::RealtimeInputInfo> CurrentInputInfos;                                //See <GetCurrentRealtimeInputs>
        std::vector<ssGUI::RealtimeInputInfo> LastInputInfos;                                   //See <GetLastRealtimeInputs>
        sf::Cursor SFMLCursor;                                                                  //(Internal variable) Underlying cursor object
        ssGUI::Enums::CursorType CurrentCursor;                                                 //See <GetCursorType>
        std::unordered_set<ssGUI::Backend::MainWindowInterface*> CursorMappedWindow;     //TODO: Not used, remove it

        std::unordered_map<std::string, std::pair<sf::Image, glm::ivec2>> CustomCursors;        //See <GetCustomCursor>
        std::string CurrentCustomCursor;                                                        //See <GetCurrentCustomCursorName>
        std::vector<std::function
            <bool(ssGUI::Backend::MainWindowInterface*, void*)>> RawEventHandlers;       //See <AddRawEventHandler>

        #if SSGUI_USE_SFML_TIME
            sf::Clock ElapsedTime;                                                              //See <GetElapsedTime>
        #else
            std::chrono::high_resolution_clock::time_point ElapsedTime;                         //See <GetElapsedTime>
        #endif
    =================================================================
    ============================== C++ ==============================
    BackendSystemInputSFML::BackendSystemInputSFML() :  CurrentKeyPresses(),
                                                        LastKeyPresses(),
                                                        InputText(L""),
                                                        CurrentMousePosition(),
                                                        LastMousePosition(),
                                                        CurrentMouseButtons(),
                                                        LastMouseButtons(),
                                                        MouseScrollDelta(),
                                                        CurrentInputInfos(),
                                                        LastInputInfos(),
                                                        SFMLCursor(),
                                                        CurrentCursor(ssGUI::Enums::CursorType::NORMAL),
                                                        CursorMappedWindow(),
                                                        CustomCursors(),
                                                        CurrentCustomCursor(""),
                                                        RawEventHandlers(),
                                                        ElapsedTime()
    {
        if(!SFMLCursor.loadFromSystem(sf::Cursor::Arrow))
        {
            ssGUI_ERROR(ssGUI_TAG_BACKEND, "Failed to load cursor!");
            ssLOG_EXIT_PROGRAM();
        }

        #if !SSGUI_USE_SFML_TIME
            ElapsedTime = std::chrono::high_resolution_clock::now();
        #endif

        ssGUI::Backend::BackendManager::AddInputInterface(static_cast<ssGUI::Backend::SystemInputInterface*>(this));
    }
    =================================================================
    */
    class BackendSystemInputSFML : public SystemInputInterface
    {
        private:
            std::vector<ssGUI::Enums::GenericInput> CurrentKeyPresses;                  //See <GetCurrentInputs>
            std::vector<ssGUI::Enums::GenericInput> LastKeyPresses;                     //See <GetLastInputs>
            std::wstring InputText;                                                                 //See <GetTextInput>
            glm::ivec2 CurrentMousePosition;                                                        //See <GetCurrentMousePosition>
            glm::ivec2 LastMousePosition;                                                           //See <GetLastMousePosition>
            std::vector<ssGUI::Enums::MouseButton> CurrentMouseButtons;                             //See <GetCurrentMouseButton>
            std::vector<ssGUI::Enums::MouseButton> LastMouseButtons;                                //See <GetLastMouseButton>
            glm::vec2 MouseScrollDelta;                                                             //See <GetCurrentMouseScrollDelta>
            std::vector<ssGUI::RealtimeInputInfo> CurrentInputInfos;                                //See <GetCurrentRealtimeInputs>
            std::vector<ssGUI::RealtimeInputInfo> LastInputInfos;                                   //See <GetLastRealtimeInputs>
            sf::Cursor SFMLCursor;                                                                  //(Internal variable) Underlying cursor object
            ssGUI::Enums::CursorType CurrentCursor;                                                 //See <GetCursorType>
            std::unordered_set<ssGUI::Backend::MainWindowInterface*> CursorMappedWindow;     //TODO: Not used, remove it

            std::unordered_map<std::string, std::pair<sf::Image, glm::ivec2>> CustomCursors;        //See <GetCustomCursor>
            std::string CurrentCustomCursor;                                                        //See <GetCurrentCustomCursorName>
            std::vector<std::function
                <bool(ssGUI::Backend::MainWindowInterface*, void*)>> RawEventHandlers;       //See <AddRawEventHandler>

            #if SSGUI_USE_SFML_TIME
                sf::Clock ElapsedTime;                                                              //See <GetElapsedTime>
            #else
                std::chrono::high_resolution_clock::time_point ElapsedTime;                         //See <GetElapsedTime>
            #endif

            template <class T>
            void AddNonExistElement(T elementToAdd, std::vector<T>& vectorAddTo);

            template <class T>
            void RemoveExistElement(T elementToRemove, std::vector<T>& vectorRemoveFrom);

            void FetchKeysPressed(ssGUI::Enums::GenericInput keysPressedDown, std::vector<ssGUI::Enums::GenericInput>& destinationKeyPresses);
            void FetchKeysReleased(ssGUI::Enums::GenericInput keysReleased, std::vector<ssGUI::Enums::GenericInput>& destinationKeyPresses);

            //http://tech-algorithm.com/articles/bilinear-image-scaling/
            //https://stackoverflow.com/questions/21514075/bilinear-re-sizing-with-c-and-vector-of-rgba-pixels
            void ResizeBilinear(const uint8_t* inputPixels, int w, int h, uint8_t* outputPixels, int w2, int h2);
        
        public:
            BackendSystemInputSFML();
            ~BackendSystemInputSFML() override;

            //function: UpdateInput
            //See <SystemInputInterface::UpdateInput>
            void UpdateInput() override;

            //function: GetLastKeyPresses
            //See <SystemInputInterface::GetLastKeyPresses>
            const std::vector<ssGUI::Enums::GenericInput>& GetLastInputs() override;

            //function: GetCurrentKeyPresses
            //See <SystemInputInterface::GetCurrentKeyPresses>
            const std::vector<ssGUI::Enums::GenericInput>& GetCurrentInputs() override;

            //function: IsInputExistLastFrame
            //See <IsInputExistLastFrame>
            bool IsInputExistLastFrame(ssGUI::Enums::GenericInput input) const override;
            
            //function: IsInputExistCurrentFrame
            //See <IsInputExistCurrentFrame>
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
            virtual glm::vec2 GetCurrentMouseScrollDelta() const override;

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
            //See <SystemInputInterface::SetCursorType>
            void SetCursorType(ssGUI::Enums::CursorType cursorType) override;
            
            //function: GetCursorType
            //See <SystemInputInterface::GetCursorType>
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
            //See <SystemInputInterface::AddRawEventHandler>
            int AddRawEventHandler(std::function<bool(ssGUI::Backend::MainWindowInterface*, void*)> handler) override;

            //function: RemoveRawEventHandler
            //See <SystemInputInterface::RemoveRawEventHandler>
            void RemoveRawEventHandler(int id) override;

            //function: ClearRawEventHandler
            //See <<SystemInputInterface::ClearRawEventHandler>
            void ClearRawEventHandler() override;

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