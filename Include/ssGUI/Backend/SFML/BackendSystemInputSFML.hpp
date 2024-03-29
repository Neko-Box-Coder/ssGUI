#ifndef SSGUI_BACKEND_SYSTEM_INPUT_SFML_H
#define SSGUI_BACKEND_SYSTEM_INPUT_SFML_H

#include "ssGUI/Backend/Interfaces/BackendSystemInputInterface.hpp"
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
    For functions explainations, please see <BackendSystemInputInterface>. Normally you don't need to deal with this class
    
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
        std::unordered_set<ssGUI::Backend::BackendMainWindowInterface*> CursorMappedWindow;     //TODO: Not used, remove it

        std::unordered_map<std::string, std::pair<sf::Image, glm::ivec2>> CustomCursors;        //See <GetCustomCursor>
        std::string CurrentCustomCursor;                                                        //See <GetCurrentCustomCursorName>
        std::vector<std::function
            <bool(ssGUI::Backend::BackendMainWindowInterface*, void*)>> RawEventHandlers;       //See <AddRawEventHandler>

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
            ssGUI_ERROR(ssGUI_BACKEND_TAG, "Failed to load cursor!");
            ssLOG_EXIT_PROGRAM();
        }

        #if !SSGUI_USE_SFML_TIME
            ElapsedTime = std::chrono::high_resolution_clock::now();
        #endif

        ssGUI::Backend::BackendManager::AddInputInterface(static_cast<ssGUI::Backend::BackendSystemInputInterface*>(this));
    }
    =================================================================
    */
    class BackendSystemInputSFML : public BackendSystemInputInterface
    {
        private:
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
            sf::Cursor SFMLCursor;                                                                  //(Internal variable) Underlying cursor object
            ssGUI::Enums::CursorType CurrentCursor;                                                 //See <GetCursorType>
            std::unordered_set<ssGUI::Backend::BackendMainWindowInterface*> CursorMappedWindow;     //TODO: Not used, remove it

            std::unordered_map<std::string, std::pair<sf::Image, glm::ivec2>> CustomCursors;        //See <GetCustomCursor>
            std::string CurrentCustomCursor;                                                        //See <GetCurrentCustomCursorName>
            std::vector<std::function
                <bool(ssGUI::Backend::BackendMainWindowInterface*, void*)>> RawEventHandlers;       //See <AddRawEventHandler>

            #if SSGUI_USE_SFML_TIME
                sf::Clock ElapsedTime;                                                              //See <GetElapsedTime>
            #else
                std::chrono::high_resolution_clock::time_point ElapsedTime;                         //See <GetElapsedTime>
            #endif

            template <class T>
            void AddNonExistElement(T elementToAdd, std::vector<T>& vectorAddTo);

            template <class T>
            void RemoveExistElement(T elementToRemove, std::vector<T>& vectorRemoveFrom);

            void FetchKeysPressed(ssGUI::Enums::GenericButtonAndKeyInput keysPressedDown, std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& destinationKeyPresses);
            void FetchKeysReleased(ssGUI::Enums::GenericButtonAndKeyInput keysReleased, std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& destinationKeyPresses);

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
            const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& GetLastButtonAndKeyPresses() override;

            //function: GetCurrentKeyPresses
            //See <BackendSystemInputInterface::GetCurrentKeyPresses>
            const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& GetCurrentButtonAndKeyPresses() override;

            //function: IsButtonOrKeyPressExistLastFrame
            //See <IsButtonOrKeyPressExistLastFrame>
            bool IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const override;
            
            //function: IsButtonOrKeyPressExistCurrentFrame
            //See <IsButtonOrKeyPressExistCurrentFrame>
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
            virtual glm::vec2 GetCurrentMouseScrollDelta() const override;

            //function: GetLastRealtimeInputs
            //See <BackendSystemInputInterface::GetLastRealtimeInputs>
            const std::vector<ssGUI::RealtimeInputInfo>& GetLastRealtimeInputs() const override;
            
            //function: GetCurrentRealtimeInputs
            //See <BackendSystemInputInterface::GetCurrentRealtimeInputs>
            const std::vector<ssGUI::RealtimeInputInfo>& GetCurrentRealtimeInputs() const override;

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
            //See <BackendSystemInputInterface::AddRawEventHandler>
            int AddRawEventHandler(std::function<bool(ssGUI::Backend::BackendMainWindowInterface*, void*)> handler) override;

            //function: RemoveRawEventHandler
            //See <BackendSystemInputInterface::RemoveRawEventHandler>
            void RemoveRawEventHandler(int id) override;

            //function: ClearRawEventHandler
            //See <<BackendSystemInputInterface::ClearRawEventHandler>
            void ClearRawEventHandler() override;

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