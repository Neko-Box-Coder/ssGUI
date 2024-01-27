#include "ssGUI/Backend/SDL2/MainWindowSDL2.hpp"
#include "ssGUI/Backend/SDL2/DrawingSDL2.hpp"
#include "ssGUI/HelperClasses/ImageUtil.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"


//TODO(NOW): Check if window is closed for every function that needs it

namespace ssGUI
{

namespace Backend
{
    bool MainWindowSDL2::CreateWindow()
    {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> codec;
        std::string titleUtf8 = codec.to_bytes(WindowTitle);
        
        CurrentWindow = SDL_CreateWindow(   titleUtf8.c_str(), 
                                            WindowPosition.x, 
                                            WindowPosition.y, 
                                            WindowSize.x, 
                                            WindowSize.y, 
                                            SDL_WINDOW_SHOWN);

        if(!CurrentWindow)
        {
            ssGUI_ERROR(ssGUI_BACKEND_TAG,  "SDL2 Window couldn't be created, error: " << 
                                            SDL_GetError());

            return false;
        }
        
        SetResizable(WindowResizable);
        SetWindowMode(CurrentWindowMode);
        SetVisible(!WindowHidden);
        
        CurrentSDL_Renderer = SDL_CreateRenderer(CurrentWindow, -1, 0);
        if(!CurrentSDL_Renderer)
        {
            ssGUI_ERROR(ssGUI_BACKEND_TAG,  "SDL2 Renderer couldn't be created, error: " << 
                                            SDL_GetError());

            return false;
        }
        
        //NOTE: SDL does not have support for anti-aliasing
        //SetAntiAliasingLevel(WindowAntiAliasingLevel);
        
        SetVSync(WindowVsync);
        
        return true;
    }
    
    void MainWindowSDL2::DestroyWindow()
    {
        if(CurrentWindow == nullptr || CurrentSDL_Renderer == nullptr)
            return;
        
        SDL_DestroyRenderer(CurrentSDL_Renderer);
        SDL_DestroyWindow(CurrentWindow);
    }

    MainWindowSDL2::MainWindowSDL2(MainWindowSDL2 const& other) :
        CurrentWindow(nullptr),
        WindowTitle(other.WindowTitle),
        WindowPosition(other.WindowPosition),
        WindowSize(other.WindowSize),
        WindowClosed(false),
        WindowClosingAborted(false),
        WindowHidden(other.WindowHidden),
        WindowVsync(other.WindowVsync),
        WindowResizable(other.WindowResizable),
        BackendDrawing(nullptr),
        CurrentWindowMode(other.CurrentWindowMode),
        CurrentSDL_Renderer(nullptr),
        OnCloseCallback(other.OnCloseCallback),
        ExternalFocusChangedCallback(other.ExternalFocusChangedCallback)
    {
    }

    MainWindowSDL2::MainWindowSDL2() :  CurrentWindow(nullptr),
                                        WindowTitle(),
                                        WindowPosition(),
                                        WindowSize(),
                                        WindowClosed(false), 
                                        WindowClosingAborted(false),
                                        WindowHidden(false),
                                        WindowVsync(false),
                                        WindowResizable(true),
                                        BackendDrawing(nullptr),
                                        CurrentWindowMode(Enums::WindowMode::NORMAL),
                                        CurrentSDL_Renderer(nullptr),
                                        OnCloseCallback(),
                                        ExternalFocusChangedCallback()
    {
    }

    MainWindowSDL2::~MainWindowSDL2()
    {
        DestroyWindow();
    }
    
    bool MainWindowSDL2::Initialize()
    {
        return CreateWindow();
    }
    
    glm::vec2 MainWindowSDL2::GetDPIScaling() const
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
    
    void MainWindowSDL2::SetWindowPosition(glm::ivec2 pos)
    {
        if(GetWindowMode() == Enums::WindowMode::FULLSCREEN)
        {
            // Ignoring setting window position for fullscreen
            return;
        }
        
        SDL_SetWindowPosition(CurrentWindow, pos.x, pos.y);
    }

    glm::ivec2 MainWindowSDL2::GetWindowPosition() const
    {
        glm::ivec2 windowPos;
        
        SDL_GetWindowPosition(CurrentWindow, &windowPos.x, &windowPos.y);
        return windowPos;
    }

    void MainWindowSDL2::GetDecorationOffsets(   glm::ivec2& topLeft, 
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

    void MainWindowSDL2::SetWindowSize(glm::ivec2 size)
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

    glm::ivec2 MainWindowSDL2::GetWindowSize() const
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

    void MainWindowSDL2::SetRenderSize(glm::ivec2 size)
    {
        glm::vec2 scaling = GetDPIScaling();
        size /= scaling;
        
        SDL_SetWindowSize(CurrentWindow, size.x, size.y);
    }
    
    glm::ivec2 MainWindowSDL2::GetRenderSize() const
    {
        glm::ivec2 size;
        SDL_GetWindowSizeInPixels(CurrentWindow, &size.x, &size.y);
        return size;
    }

    bool MainWindowSDL2::IsClosed() const
    {
        return WindowClosed;
    }

    void MainWindowSDL2::Close()
    {
        for(int i = 0; i < OnCloseCallback.size(); i++)
        {
            if(OnCloseCallback[i] != nullptr)
                OnCloseCallback[i](this);

            if(WindowClosingAborted)
            {
                WindowClosingAborted = false;
                return;
            }
        }
        
        WindowClosed = true;
        DestroyWindow();
    }
    
    void MainWindowSDL2::AbortClosing()
    {
        WindowClosingAborted = true;
    }

    int MainWindowSDL2::
        AddOnCloseEvent(std::function<void(MainWindowInterface* mainWindow)> func)
    {
        OnCloseCallback.push_back(func);
        return OnCloseCallback.size() - 1;
    }

    void MainWindowSDL2::RemoveOnCloseEvent(int index)
    {
        if(index < 0 || index >= OnCloseCallback.size())
            return;

        OnCloseCallback.at(index) = nullptr;
    }

    void MainWindowSDL2::SetTitle(std::u32string title)
    {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> codec;
        std::string titleUtf8 = codec.to_bytes(title);
        SDL_SetWindowTitle(CurrentWindow, titleUtf8.c_str());
    }
    
    void MainWindowSDL2::SetTitle(std::string title)
    {
        SDL_SetWindowTitle(CurrentWindow, title.c_str());
    }

    void MainWindowSDL2::GetTitle(std::u32string& title) const
    {
        std::string utf8Title;
        GetTitle(utf8Title);
        
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> codec;
        title = codec.from_bytes(utf8Title);
    }

    void MainWindowSDL2::GetTitle(std::string& title) const
    {
        const char* windowTitle = SDL_GetWindowTitle(CurrentWindow);
        title = std::string(windowTitle);
    }

    void MainWindowSDL2::SetIcon(const ImageInterface& iconImage)
    {
        if(!iconImage.IsValid())
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Set icon is passed with invalid iamge");
            return;
        }
        
        auto iconPixels = std::unique_ptr<uint8_t[]>(new uint8_t[   iconImage.GetSize().x * 
                                                                    iconImage.GetSize().y * 4]);
        
        ImageFormat inImgformat;
        void* inPixPtr = iconImage.GetPixelPtr(inImgformat);
        
        if(!ImageUtil::FormatToRGBA32(iconPixels.get(), inPixPtr, inImgformat, iconImage.GetSize()))
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to convert image to rgba32");
            return;
        }
        
        SDL_Surface* cursorSurface = 
            SDL_CreateRGBSurfaceWithFormatFrom( iconPixels.get(), 
                                                iconImage.GetSize().x, 
                                                iconImage.GetSize().y, 
                                                32, 
                                                iconImage.GetSize().x * 32 + 
                                                    inImgformat.RowPaddingInBytes, 
                                                SDL_PIXELFORMAT_RGBA32);
        
        if(cursorSurface == NULL)
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "SDL error while creating icon surface");
            return;
        }
        
        SDL_SetWindowIcon(CurrentWindow, cursorSurface);
        SDL_FreeSurface(cursorSurface);
    }

    void MainWindowSDL2::SetVisible(bool visible)
    {
        WindowHidden = visible;
        
        if(visible)
            SDL_ShowWindow(CurrentWindow);
        else
            SDL_HideWindow(CurrentWindow);
    }

    bool MainWindowSDL2::IsVisible() const
    {
        return WindowHidden;
    }

    void MainWindowSDL2::SetVSync(bool vSync)
    {
        WindowVsync = vSync;
        SDL_RenderSetVSync(CurrentSDL_Renderer, vSync);
    }

    bool MainWindowSDL2::IsVSync() const
    {
        return WindowVsync;
    }

    void MainWindowSDL2::SetFocus(bool focus, bool externalByUser)
    {
        if(focus && !externalByUser)
            SDL_RaiseWindow(CurrentWindow);
        else
        {
            for(int i = 0; i < ExternalFocusChangedCallback.size(); i++)
            {
                if(ExternalFocusChangedCallback[i] != nullptr)
                    ExternalFocusChangedCallback[i](this, focus);
            }
        }
    }
    
    bool MainWindowSDL2::IsFocused() const
    {
        return SDL_GetKeyboardFocus() == CurrentWindow;
    }

    int MainWindowSDL2::
        AddFocusChangedByUserEvent(std::function<void(  MainWindowInterface* mainWindow, 
                                                        bool focused)> func)
    {
        ExternalFocusChangedCallback.push_back(func);
        return ExternalFocusChangedCallback.size() - 1;
    }

    void MainWindowSDL2::RemoveFocusChangedByUserEvent(int id)
    {
        if(id < 0 || id >= ExternalFocusChangedCallback.size())
            return;

        ExternalFocusChangedCallback.at(id) = nullptr;
    }

    void MainWindowSDL2::SetAntiAliasingLevel(int level)
    {
        //NOTE: Anti-aliasing not supported by SDL2
    }

    int MainWindowSDL2::GetAntiAliasingLevel() const
    {
        //NOTE: Anti-aliasing not supported by SDL2
        return 0;
    }

    void MainWindowSDL2::SetResizable(bool resizable)
    {
        WindowResizable = resizable;
        SDL_SetWindowResizable(CurrentWindow, static_cast<SDL_bool>(resizable));
    }

    bool MainWindowSDL2::IsResizable() const
    {
        return CurrentWindowMode == Enums::WindowMode::NORMAL ? WindowResizable : false;
    }

    bool MainWindowSDL2::SetDecorationOptions(Enums::WindowDecorationOptions options)
    {
        //NOTE: SDL2 has no decorations support
        return false;
    }
    
    Enums::WindowDecorationOptions MainWindowSDL2::GetDecorationOptions() const
    {
        return Enums::WindowDecorationOptions::UNKNOWN;
    }

    void MainWindowSDL2::SetWindowMode(Enums::WindowMode windowMode)
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

    ssGUI::Enums::WindowMode MainWindowSDL2::GetWindowMode() const
    {
        return CurrentWindowMode;
    }

    bool MainWindowSDL2::SetDrawingContext()
    {
        return true;
    }
    
    MainWindowInterface* MainWindowSDL2:: Clone()
    {
        MainWindowSDL2* newMainWindow = new MainWindowSDL2();
        if(!newMainWindow->Initialize())
        {
            delete newMainWindow;
            return nullptr;
        }
        
        return newMainWindow;
    }

    void* MainWindowSDL2::GetRawHandle() const
    {
        CurrentSDLHandle.Window = CurrentWindow;
        CurrentSDLHandle.Renderer = CurrentSDL_Renderer;
        return &CurrentSDLHandle;
    }
    
    void MainWindowSDL2::Internal_SetBackendDrawing(DrawingSDL2* backendDrawing)
    {
        BackendDrawing = backendDrawing;
    }
    
    SDL_Renderer* MainWindowSDL2::Internal_GetSDLRenderer() const
    {
        return CurrentSDL_Renderer;
    }
}

}