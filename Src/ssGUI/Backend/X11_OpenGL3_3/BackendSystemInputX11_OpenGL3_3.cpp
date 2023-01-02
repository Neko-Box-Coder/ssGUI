#include "ssGUI/Backend/X11_OpenGL3_3/BackendSystemInputX11_OpenGL3_3.hpp"

#include "ssGUI/Backend/X11_OpenGL3_3/BackendMainWindowX11_OpenGL3_3.hpp"

#include "ssLogger/ssLog.hpp"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    BackendSystemInputX11_OpenGL3_3::BackendSystemInputX11_OpenGL3_3(BackendSystemInputX11_OpenGL3_3 const& other)
    {
    
    }

    BackendSystemInputX11_OpenGL3_3::BackendSystemInputX11_OpenGL3_3()
    {
        ssLOG_LINE("BackendSystemInputX11_OpenGL3_3 is being created, is this intended?");
    }

    BackendSystemInputX11_OpenGL3_3::~BackendSystemInputX11_OpenGL3_3()
    {

    }
    
    //DEBUG
    #include "glad/glad.h"
    
    void BackendSystemInputX11_OpenGL3_3::UpdateInput()
    {
        auto* rawHandle = static_cast<ssGUI::Backend::X11RawHandle*>(
                            ssGUI::Backend::BackendManager::GetMainWindowInterface(0)->GetRawHandle());
        
        
        bool closeWindow = false;
        
        while (XPending(rawHandle->WindowDisplay)) 
        {
            XEvent xev;
            XNextEvent(rawHandle->WindowDisplay, &xev);

            if (xev.type == KeyPress) {
                ssLOG_LINE("Key pressed");
            }
            
            else if(xev.type == ClientMessage && (Atom)xev.xclient.data.l[0] == rawHandle->WindowCloseEventId)
            {
                closeWindow = true;
                ssLOG_LINE("Window closing");
                break;
            }
            //if(xev.type == )
        }
        
        if(closeWindow)
            ssGUI::Backend::BackendManager::GetMainWindowInterface(0)->Close();
    }

    const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& BackendSystemInputX11_OpenGL3_3::GetLastButtonAndKeyPresses()
    {
        std::vector<ssGUI::Enums::GenericButtonAndKeyInput> t;
        return t;
    }
    
    const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& BackendSystemInputX11_OpenGL3_3::GetCurrentButtonAndKeyPresses()
    {
        std::vector<ssGUI::Enums::GenericButtonAndKeyInput> t;
        return t;
    }

    bool BackendSystemInputX11_OpenGL3_3::IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const
    {
        return true;
    }

    bool BackendSystemInputX11_OpenGL3_3::IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const
    {
        return true;
    }

    glm::ivec2 BackendSystemInputX11_OpenGL3_3::GetLastMousePosition(ssGUI::MainWindow* mainWindow) const
    {
        return glm::ivec2();
    }
    
    glm::ivec2 BackendSystemInputX11_OpenGL3_3::GetCurrentMousePosition(ssGUI::MainWindow* mainWindow) const
    {
        return glm::ivec2();
    }
    
    void BackendSystemInputX11_OpenGL3_3::SetMousePosition(glm::ivec2 position, ssGUI::MainWindow* mainWindow)
    {

    }

    bool BackendSystemInputX11_OpenGL3_3::GetLastMouseButton(ssGUI::Enums::MouseButton button) const
    {
        return true;
    }
    
    bool BackendSystemInputX11_OpenGL3_3::GetCurrentMouseButton(ssGUI::Enums::MouseButton button) const
    {
        return true;
    }

    glm::vec2 BackendSystemInputX11_OpenGL3_3::GetCurrentMouseScrollDelta() const
    {
        return glm::vec2();
    }

    std::vector<ssGUI::RealtimeInputInfo> const & BackendSystemInputX11_OpenGL3_3::GetLastRealtimeInputs() const
    {
        std::vector<ssGUI::RealtimeInputInfo> t;
        return t;
    }

    std::vector<ssGUI::RealtimeInputInfo> const & BackendSystemInputX11_OpenGL3_3::GetCurrentRealtimeInputs() const
    {
        std::vector<ssGUI::RealtimeInputInfo> t;
        return t;
    }

    std::wstring BackendSystemInputX11_OpenGL3_3::GetTextInput() const
    {
        return L"";
    }
    
    void BackendSystemInputX11_OpenGL3_3::SetCursorType(ssGUI::Enums::CursorType cursorType)
    {

    }

    ssGUI::Enums::CursorType BackendSystemInputX11_OpenGL3_3::GetCursorType() const
    {
        return ssGUI::Enums::CursorType::NORMAL;
    }

    void BackendSystemInputX11_OpenGL3_3::CreateCustomCursor(ssGUI::ImageData* customCursor, std::string cursorName, glm::ivec2 cursorSize, glm::ivec2 hotspot)
    {

    }
    
    void BackendSystemInputX11_OpenGL3_3::SetCurrentCustomCursor(std::string cursorName)
    {

    }

    void BackendSystemInputX11_OpenGL3_3::GetCurrentCustomCursor(ssGUI::ImageData& customCursor, glm::ivec2& hotspot)
    {

    }

    std::string BackendSystemInputX11_OpenGL3_3::GetCurrentCustomCursorName()
    {
        return "";
    }
    
    void BackendSystemInputX11_OpenGL3_3::GetCustomCursor(ssGUI::ImageData& customCursor, std::string cursorName, glm::ivec2& hotspot)
    {

    }

    bool BackendSystemInputX11_OpenGL3_3::HasCustomCursor(std::string cursorName)
    {
        return true;
    }

    void BackendSystemInputX11_OpenGL3_3::UpdateCursor()
    {

    }

    bool BackendSystemInputX11_OpenGL3_3::ClearClipboard()
    {
        return true;
    }

    bool BackendSystemInputX11_OpenGL3_3::ClipbaordHasText()
    {
        return true;
    }
    
    bool BackendSystemInputX11_OpenGL3_3::ClipbaordHasImage()
    {
        return true;
    }

    bool BackendSystemInputX11_OpenGL3_3::SetClipboardImage(const ssGUI::ImageData& imgData)
    {
        return true;
    }
    
    bool BackendSystemInputX11_OpenGL3_3::SetClipboardText(const std::wstring& str)
    {
        return true;
    }
    
    bool BackendSystemInputX11_OpenGL3_3::GetClipboardImage(ssGUI::ImageData& imgData)
    {
        return true;
    }

    bool BackendSystemInputX11_OpenGL3_3::GetClipboardText(std::wstring& str)
    {
        return true;
    }

    uint64_t BackendSystemInputX11_OpenGL3_3::GetElapsedTime() const
    {
        return 0;
    }
}

}