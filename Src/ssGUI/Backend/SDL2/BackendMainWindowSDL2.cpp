#include "ssGUI/Backend/SDL2/BackendMainWindowSDL2.hpp"
#include "ssGUI/Backend/SDL2/BackendDrawingSDL2.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"


//TODO(NOW): Check if window is closed for every function that needs it

namespace ssGUI
{

namespace Backend
{
    void BackendMainWindowSDL2::CreateWindow()
    {
        //TODO(NOW)
    }
    
    void BackendMainWindowSDL2::DestroyWindow()
    {
        //TODO(NOW)
        SDL_DestroyWindow(CurrentWindow);
    }

    glm::vec2 BackendMainWindowSDL2::GetDPIScaling() const
    {
        glm::ivec2 logicalSize;
        glm::ivec2 pixelSize;
        
        SDL_GetWindowSize(CurrentWindow, &logicalSize.x, &logicalSize.y);
        
        if(logicalSize.x <= 0 || logicalSize.y <= 0)
        {
            ssGUI_ERROR(ssGUI_BACKEND_TAG,  "SDL2 is unable to get the window size to get the DPI " <<
                                            "scaling, error: " << SDL_GetError());
            
            return glm::vec2(1.0f);
        }
        
        SDL_GetWindowSizeInPixels(CurrentWindow, &pixelSize.x, &pixelSize.y);
        
        return static_cast<glm::vec2>(pixelSize) / static_cast<glm::vec2>(logicalSize);
    }
    
    BackendMainWindowSDL2::BackendMainWindowSDL2(BackendMainWindowSDL2 const& other)
    {
        //TODO(NOW)
    }

    BackendMainWindowSDL2::BackendMainWindowSDL2()
    {
        CurrentWindow = SDL_CreateWindow(   "Test", 
                                            SDL_WINDOWPOS_UNDEFINED, 
                                            SDL_WINDOWPOS_UNDEFINED, 
                                            800, 
                                            600, 
                                            SDL_WINDOW_SHOWN);

        if(!CurrentWindow)
        {
            ssGUI_ERROR(ssGUI_BACKEND_TAG,  "SDL2 Window couldn't be created, error: " << 
                                            SDL_GetError());
        }
        
        //TODO(NOW): Create SDL Renderer
    }

    BackendMainWindowSDL2::~BackendMainWindowSDL2()
    {
        DestroyWindow();
        //SDL_DestroyWindow(CurrentWindow);
    }
    
    void BackendMainWindowSDL2::SetWindowPosition(glm::ivec2 pos)
    {
        if(GetWindowMode() == Enums::WindowMode::FULLSCREEN)
        {
            // Ignoring setting window position for fullscreen
            return;
        }
        
        SDL_SetWindowPosition(CurrentWindow, pos.x, pos.y);
    }

    glm::ivec2 BackendMainWindowSDL2::GetWindowPosition() const
    {
        glm::ivec2 windowPos;
        
        SDL_GetWindowPosition(CurrentWindow, &windowPos.x, &windowPos.y);
        return windowPos;
    }

    void BackendMainWindowSDL2::GetDecorationOffsets(   glm::ivec2& topLeft, 
                                                        glm::ivec2& bottomRight) const
    {
        int top, left, bottom, right;
        top = left = bottom = right = 0;
        
        int result = SDL_GetWindowBordersSize(CurrentWindow, &top, &left, &bottom, &right);
        
        if(result != 0)
        {
            ssGUI_ERROR(ssGUI_BACKEND_TAG,  "SDL2 is unable to get the window decoration, error: " << 
                                            SDL_GetError());
            
            return;
        }
        
        topLeft = glm::ivec2(top, left);
        bottomRight = glm::ivec2(bottom, right);
        
        //TODO(NOW): Need to check if window borders are in pixels
        glm::vec2 scaling = GetDPIScaling();
        topLeft *= scaling;
        bottomRight *= scaling;
    }

    void BackendMainWindowSDL2::SetWindowSize(glm::ivec2 size)
    {
        //TODO(NOW): Need to check if window borders are in pixels
        glm::vec2 scaling = GetDPIScaling();
        size /= scaling;
        
        if(GetWindowMode() == Enums::WindowMode::FULLSCREEN)
        {
            SDL_DisplayMode current;
            int result = SDL_GetWindowDisplayMode(CurrentWindow, &current);
            int index = SDL_GetWindowDisplayIndex(CurrentWindow);
            
            if(result < 0 || index < 0)
            {
                ssGUI_ERROR(ssGUI_BACKEND_TAG,  "SDL2 is unable to get the display mode or index"
                                                ", error: " << SDL_GetError());
                return;
            }
            
            ssGUI_DEBUG(ssGUI_BACKEND_TAG,  "Display mode is " << 
                                            SDL_BITSPERPIXEL(current.format) << " bits per pixel" <<
                                            "\t" << SDL_GetPixelFormatName(current.format) <<
                                            "\t" << current.w << " x " << current.h);

            SDL_DisplayMode desired;
            SDL_DisplayMode closest;
            
            desired.w = size.x;
            desired.h = size.y;
            desired.format = current.format;
            desired.refresh_rate = current.refresh_rate;
            desired.driverdata = 0; /* hardware-specific, initialize to 0 */
            
            if(SDL_GetClosestDisplayMode(index, &desired, &closest) == NULL)
            {
                ssGUI_ERROR(ssGUI_BACKEND_TAG,  "SDL2 is unable to get the closest display mode"
                                                ", error: " << SDL_GetError());
                return;
            }
            
            SDL_SetWindowDisplayMode(CurrentWindow, &closest);
            return;
        }
        
        glm::ivec2 decorationTopLeft;
        glm::ivec2 decorationBottomRight;
        GetDecorationOffsets(decorationTopLeft, decorationBottomRight);
        
        size -= decorationTopLeft;
        size -= decorationBottomRight;
        
        SDL_SetWindowSize(CurrentWindow, size.x, size.y);
    }

    glm::ivec2 BackendMainWindowSDL2::GetWindowSize() const
    {
        glm::ivec2 size;
        SDL_GetWindowSizeInPixels(CurrentWindow, &size.x, &size.y);
        
        glm::ivec2 decorationTopLeft;
        glm::ivec2 decorationBottomRight;
        GetDecorationOffsets(decorationTopLeft, decorationBottomRight);
        
        size += decorationTopLeft;
        size += decorationBottomRight;
        
        return size;
    }

    void BackendMainWindowSDL2::SetRenderSize(glm::ivec2 size)
    {
        glm::vec2 scaling = GetDPIScaling();
        size /= scaling;
        
        SDL_SetWindowSize(CurrentWindow, size.x, size.y);
    }
    
    glm::ivec2 BackendMainWindowSDL2::GetRenderSize() const
    {
        glm::ivec2 size;
        SDL_GetWindowSizeInPixels(CurrentWindow, &size.x, &size.y);
        return size;
    }

    bool BackendMainWindowSDL2::IsClosed() const
    {
        return WindowClosed;
    }

    void BackendMainWindowSDL2::Close()
    {
        for(int i = 0; i < OnCloseCallback.size(); i++)
        {
            if(OnCloseCallback[i] != nullptr)
                OnCloseCallback[i]();

            if(WindowClosingAborted)
            {
                WindowClosingAborted = false;
                return;
            }
        }
        
        WindowClosed = true;
        DestroyWindow();
    }
    
    void BackendMainWindowSDL2::AbortClosing()
    {
        WindowClosingAborted = true;
    }

    int BackendMainWindowSDL2::AddOnCloseEvent(std::function<void()> func)
    {
        OnCloseCallback.push_back(func);
        return OnCloseCallback.size() - 1;
    }

    void BackendMainWindowSDL2::RemoveOnCloseEvent(int index)
    {
        if(index < 0 || index >= OnCloseCallback.size())
            return;

        OnCloseCallback.at(index) = nullptr;
    }

    void BackendMainWindowSDL2::SetTitle(std::u32string title)
    {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> codec;
        std::string titleUtf8 = codec.to_bytes(title);
        SDL_SetWindowTitle(CurrentWindow, titleUtf8.c_str());
    }
    
    void BackendMainWindowSDL2::SetTitle(std::string title)
    {
        SDL_SetWindowTitle(CurrentWindow, title.c_str());
    }

    void BackendMainWindowSDL2::GetTitle(std::u32string& title) const
    {
        std::string utf8Title;
        GetTitle(utf8Title);
        
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> codec;
        title = codec.from_bytes(utf8Title);
    }

    void BackendMainWindowSDL2::GetTitle(std::string& title) const
    {
        std::string utf8Title;
        const char* windowTitle = SDL_GetWindowTitle(CurrentWindow);
        utf8Title = std::string(windowTitle);
    }

    void BackendMainWindowSDL2::SetIcon(const ssGUI::Backend::BackendImageInterface& iconImage)
    {
        //TODO(NOW)
    }

    void BackendMainWindowSDL2::SetVisible(bool visible)
    {
        WindowHidden = visible;
        
        if(visible)
            SDL_ShowWindow(CurrentWindow);
        else
            SDL_HideWindow(CurrentWindow);
    }

    bool BackendMainWindowSDL2::IsVisible() const
    {
        return WindowHidden;
    }

    void BackendMainWindowSDL2::SetVSync(bool vSync)
    {
        WindowVsync = vSync;
        SDL_RenderSetVSync(CurrentSDL_Renderer, vSync);
    }

    bool BackendMainWindowSDL2::IsVSync() const
    {
        return WindowVsync;
    }

    void BackendMainWindowSDL2::SetFocus(bool focus, bool externalByUser)
    {
        if(focus && !externalByUser)
            SDL_RaiseWindow(CurrentWindow);
        else
        {
            for(int i = 0; i < ExternalFocusChangedCallback.size(); i++)
            {
                if(ExternalFocusChangedCallback[i] != nullptr)
                    ExternalFocusChangedCallback[i](focus);
            }
        }
    }
    
    bool BackendMainWindowSDL2::IsFocused() const
    {
        return SDL_GetKeyboardFocus() == CurrentWindow;
    }

    int BackendMainWindowSDL2::AddFocusChangedByUserEvent(std::function<void(bool focused)> func)
    {
        ExternalFocusChangedCallback.push_back(func);
        return ExternalFocusChangedCallback.size() - 1;
    }

    void BackendMainWindowSDL2::RemoveFocusChangedByUserEvent(int id)
    {
        if(id < 0 || id >= ExternalFocusChangedCallback.size())
            return;

        ExternalFocusChangedCallback.at(id) = nullptr;
    }

    void BackendMainWindowSDL2::SetAntiAliasingLevel(int level)
    {
        //NOTE: Anti-aliasing not supported by SDL2
    }

    int BackendMainWindowSDL2::GetAntiAliasingLevel() const
    {
        //NOTE: Anti-aliasing not supported by SDL2
        return 0;
    }

    void BackendMainWindowSDL2::SetResizable(bool resizable)
    {
        WindowResizable = resizable;
        SDL_SetWindowResizable(CurrentWindow, static_cast<SDL_bool>(resizable));
    }

    bool BackendMainWindowSDL2::IsResizable() const
    {
        //TODO(NOW): Check fullscreen
        return WindowResizable;
    }

    void BackendMainWindowSDL2::SetCloseButton(bool closeButton)
    {
        //NOTE: Not supported by SDL2
    }

    bool BackendMainWindowSDL2::HasCloseButton() const
    {
        //NOTE: Not supported by SDL2
        return true;
    }

    void BackendMainWindowSDL2::SetWindowMode(Enums::WindowMode windowMode)
    {
        static_assert(  static_cast<int>(Enums::WindowMode::COUNT) == 3, 
                        "WindowMode is changed, check this function");
        
        switch (windowMode) 
        {
            case Enums::WindowMode::NORMAL:
                SDL_SetWindowFullscreen(CurrentWindow, 0);
                SDL_SetWindowBordered(CurrentWindow, SDL_TRUE);
                break;
            case Enums::WindowMode::BORDERLESS:
                SDL_SetWindowFullscreen(CurrentWindow, 0);
                SDL_SetWindowBordered(CurrentWindow, SDL_FALSE);
                break;
            case Enums::WindowMode::FULLSCREEN:
                SDL_SetWindowFullscreen(CurrentWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
                break;
        }
        
        CurrentWindowMode = windowMode;
    }

    ssGUI::Enums::WindowMode BackendMainWindowSDL2::GetWindowMode() const
    {
        return CurrentWindowMode;
    }

    bool BackendMainWindowSDL2::SetDrawingContext()
    {
        return true;
    }
    
    BackendMainWindowInterface* BackendMainWindowSDL2:: Clone()
    {
        return nullptr;
    }

    void* BackendMainWindowSDL2::GetRawHandle() const
    {
        return nullptr;
    }
    
    void BackendMainWindowSDL2::Internal_SetBackendDrawing(BackendDrawingSDL2* backendDrawing)
    {
        BackendDrawing = backendDrawing;
    }
    
    SDL_Renderer* BackendMainWindowSDL2::Internal_GetSDLRenderer() const
    {
        return CurrentSDL_Renderer;
    }
}

}