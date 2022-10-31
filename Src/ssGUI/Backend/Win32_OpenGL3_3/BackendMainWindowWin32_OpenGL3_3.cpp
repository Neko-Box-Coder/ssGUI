#include "ssGUI/Backend/Win32_OpenGL3_3/BackendMainWindowWin32_OpenGL3_3.hpp"

#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>            /* must include this before GL/gl.h */
#include "glad/glad.h"
#include "ssLogger/ssLog.hpp"

namespace ssGUI
{

namespace Backend
{
    float BackendMainWindowWin32_OpenGL3_3::deviceUnitToPixels(float unit, HWND hwnd)
    {
        float dpi = GetDpiForWindow(hwnd);
        return unit * (dpi / 96.0f);
    }

    float BackendMainWindowWin32_OpenGL3_3::PixelsToDeviceUnit(float unit, HWND hwnd)
    {
        float dpi = GetDpiForWindow(hwnd);
        return unit / (dpi / 96.0f);
    }
    
    BackendMainWindowWin32_OpenGL3_3::BackendMainWindowWin32_OpenGL3_3()
    {
        HINSTANCE hInstance = GetModuleHandle(NULL);
        // Register the window class in order to create it
        const wchar_t CLASS_NAME[]  = L"ssGUI MainWindow";
        WNDCLASS wc = { };
        wc.style         = CS_OWNDC;                                        //Using own drawing context
        wc.lpfnWndProc   = BackendMainWindowWin32_OpenGL3_3::WindowProc;    //Window process callback function
        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = 0;
        wc.hInstance     = hInstance;                       //Instance handle
        wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = NULL;
        wc.lpszMenuName  = NULL;
        wc.lpszClassName = CLASS_NAME;                        //Class name

        //Alternatively, you can use the Ex version
        if(!RegisterClass(&wc))
        {
            ssLOG_LINE("Failed to RegisterClass");
            ssLOG_EXIT_PROGRAM();
            return;
        }

        // Create the window.
        HWND hwnd = CreateWindowEx
        (
            WS_EX_LEFT,                     // Extended Window Styles, left aligned text
            CLASS_NAME,                     // Window class
            L"Learn to Program Windows",    // Window title text
            WS_OVERLAPPEDWINDOW |           // Window style, has titlebar and border
            WS_CLIPSIBLINGS     |           // Diasble clipping so render each window seperately
            WS_CLIPCHILDREN,

            // Size and position
            CW_USEDEFAULT,                  //X (relative to parent)
            CW_USEDEFAULT,                  //Y (relative to parent)
            800,                            //Width, in DeviceUnits
            600,                            //Height, in DeviceUnits

            NULL,                           // Parent window    
            NULL,                           // Menu
            hInstance,                      // Instance handle
            NULL                            // Additional application data
        );

        // Check if window is created sucessfully. If not, you can check with GetLastError
        if (hwnd == NULL)
            return -1;

        //Get the device context for the window
        hDC = GetDC(hWnd);

        //there is no guarantee that the contents of the stack that become
        //the pfd are zeroed, therefore _make sure_ to clear these bits.
        PIXELFORMATDESCRIPTOR pfd;
        memset(&pfd, 0, sizeof(pfd));
        pfd.nSize        = sizeof(pfd);
        pfd.nVersion     = 1;
        pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | flags;
        pfd.iPixelType   = PFD_TYPE_RGBA;
        pfd.cColorBits   = 32;

        //Check if the pixel format we requested can be used
        int pf = ChoosePixelFormat(hDC, &pfd);
        if(pf == 0)
            return -1;

        //Set the pixel format for the window
        if(SetPixelFormat(hDC, pf, &pfd) == FALSE)
            return -1;

        //This function can be called to see if there is any changes to our chosen pixel format
        //DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

        //If there's another OpenGL window, the current Device Context can be released for now
        //ReleaseDC(hWnd, hDC);

        hRC = wglCreateContext(hDC);        //Creates OpenGL Render Context
        wglMakeCurrent(hDC, hRC);           //Select the OpenGL Render Context

        if (!gladLoadGL())                  //Load Glad
            return -1;

        //Check OpenGL version
        if (GLVersion.major < 3 && GLVersion.minor < 3)
            return -1;

        //Display the window
        ShowWindow(hwnd, nCmdShow);

        // Run the message loop.
        //Get message for all the windows for this thread, with no priority on what types input comes first
        //GetMessage will return zero if recieves WM_QUIT message, or -1 if there's an error
        MSG msg = { };
        while (GetMessage(&msg, NULL, 0, 0) > 0)
        {
            TranslateMessage(&msg);             //Translates any keyboard/text related inputs (if any)
            DispatchMessage(&msg);              //Dispatches the message to the right window(s) (i.e. WM_PAINT)
        }

        wglMakeCurrent(NULL, NULL);     //Release the OpenGL Render Context
        ReleaseDC(hWnd, hDC);           //Release the Device Context
        wglDeleteContext(hRC);          //Deallocate the Render Context
        DestroyWindow(hWnd);            //Cleanup the window

        //ssGUI::Backend::BackendManager::AddMainWindowInterface(static_cast<ssGUI::Backend::BackendMainWindowInterface*>(this));
    }

    BackendMainWindowWin32_OpenGL3_3::~BackendMainWindowWin32_OpenGL3_3()
    {
        //ssGUI::Backend::BackendManager::RemoveMainWindowInterface(static_cast<ssGUI::Backend::BackendMainWindowInterface*>(this));
    }
    
    void BackendMainWindowWin32_OpenGL3_3::SetPosition(glm::ivec2 pos)
    {
        //CurrentWindow.setPosition(sf::Vector2i(pos.x, pos.y));
    }

    glm::ivec2 BackendMainWindowWin32_OpenGL3_3::GetPosition() const
    {
        //sf::Vector2i curPos = CurrentWindow.getPosition();
        //return glm::ivec2(curPos.x, curPos.y);
        return glm::ivec2();
    }

    void BackendMainWindowWin32_OpenGL3_3::SetSize(glm::ivec2 size)
    {
        //CurrentWindow.setSize(sf::Vector2u(size.x, size.y));

        //View is now set in BackendSystemInputSFML
        // CurrentWindow.setView(sf::View(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f((float)size.x, (float)size.y))));
    }

    glm::ivec2 BackendMainWindowWin32_OpenGL3_3::GetSize() const
    {
        //sf::Vector2u curSize = CurrentWindow.getSize();
        //return glm::ivec2(curSize.x, curSize.y);
        return glm::ivec2();
    }

    bool BackendMainWindowWin32_OpenGL3_3::IsClosed() const
    {
        //return Closed;
        return true;
    }

    void BackendMainWindowWin32_OpenGL3_3::Close()
    {
        /*for(int i = 0; i < OnCloseCallback.size(); i++)
        {
            if(OnCloseCallback[i] != nullptr)
                OnCloseCallback[i]();

            if(IsClosingAborted)
            {
                IsClosingAborted = false;
                return;
            }
        }

        Closed = true;

        CurrentWindow.close();*/
    }

    void BackendMainWindowWin32_OpenGL3_3::AbortClosing()
    {
        //IsClosingAborted = true;
    }

    int BackendMainWindowWin32_OpenGL3_3::AddOnCloseEvent(std::function<void()> func)
    {
        //OnCloseCallback.push_back(func);
        //return OnCloseCallback.size() - 1;
        return 0;
    }

    void BackendMainWindowWin32_OpenGL3_3::RemoveOnCloseEvent(int index)
    {
        //OnCloseCallback[index] = nullptr;
    }

    void BackendMainWindowWin32_OpenGL3_3::SetTitle(std::wstring title)
    {
        //Title = title;
        //CurrentWindow.setTitle(Title);
    }

    std::wstring BackendMainWindowWin32_OpenGL3_3::GetTitle() const
    {
        //return Title;

        return L"";
    }

    void BackendMainWindowWin32_OpenGL3_3::SetIcon(const ssGUI::Backend::BackendImageInterface& iconImage)
    {
        //ssGUI::Backend::BackendImageSFML& castedIcon = (ssGUI::Backend::BackendImageSFML&)iconImage;
        //sf::Image sfImg = (*static_cast<sf::Texture*>(castedIcon.GetRawHandle())).copyToImage();

        //CurrentWindow.setIcon(sf::Vector2u(sfImg.getSize().x, sfImg.getSize().y), sfImg.getPixelsPtr());
    }

    void BackendMainWindowWin32_OpenGL3_3::SetVisible(bool visible)
    {
        //CurrentWindow.setVisible(visible);
        //Visible = visible;
    }

    bool BackendMainWindowWin32_OpenGL3_3::IsVisible() const
    {
        //return Visible;
        return true;
    }

    void BackendMainWindowWin32_OpenGL3_3::SetVSync(bool vSync)
    {
        //CurrentWindow.setVerticalSyncEnabled(vSync);
        //VSync = vSync;
    }

    bool BackendMainWindowWin32_OpenGL3_3::IsVSync() const
    {
        //return VSync;
        return true;
    }

    bool BackendMainWindowWin32_OpenGL3_3::IsFocused() const
    {
        //return CurrentWindow.hasFocus();
        return true;
    }

    void BackendMainWindowWin32_OpenGL3_3::SetFocus(bool focus, bool externalByUser)
    {
        /*if(externalByUser)
        {
            for(int i = 0; i < ExternalFocusChangedCallback.size(); i++)
            {
                if(ExternalFocusChangedCallback[i] != nullptr)
                    ExternalFocusChangedCallback[i](focus);
            }
        }
        else if(focus)
            CurrentWindow.requestFocus();*/
    }

    int BackendMainWindowWin32_OpenGL3_3::AddFocusChangedByUserEvent(std::function<void(bool focused)> func)
    {
        //ExternalFocusChangedCallback.push_back(func);
        //return ExternalFocusChangedCallback.size() - 1;
        return 0;
    }

    void BackendMainWindowWin32_OpenGL3_3::RemoveFocusChangedByUserEvent(int index)
    {
        //ExternalFocusChangedCallback[index] = nullptr;
    }

    int BackendMainWindowWin32_OpenGL3_3::GetMSAA() const
    {
        //return CurrentWindow.getSettings().antialiasingLevel;
        return 0;
    }

    void BackendMainWindowWin32_OpenGL3_3::SetMSAA(int level)
    {
        //ResetWindow(GetWindowMode(), IsResizable(), HasTitlebar(), HasCloseButton(), level);
    }

    void BackendMainWindowWin32_OpenGL3_3::SetTitlebar(bool titlebar)
    {
        //ResetWindow(GetWindowMode(), IsResizable(), titlebar, HasCloseButton(), GetMSAA());
        //Titlebar = titlebar;
    }

    bool BackendMainWindowWin32_OpenGL3_3::HasTitlebar() const
    {
        //return GetWindowMode() == ssGUI::Enums::WindowMode::NORMAL ? Titlebar : false;
        return true;
    }

    void BackendMainWindowWin32_OpenGL3_3::SetResizable(bool resizable)
    {
        //ResetWindow(GetWindowMode(), resizable, HasTitlebar(), HasCloseButton(), GetMSAA());
        //Resizable = resizable;
    }

    bool BackendMainWindowWin32_OpenGL3_3::IsResizable() const
    {
        //return GetWindowMode() == ssGUI::Enums::WindowMode::NORMAL ? Resizable : false;
        return true;
    }

    void BackendMainWindowWin32_OpenGL3_3::SetCloseButton(bool closeButton)
    {
        //ResetWindow(GetWindowMode(), IsResizable(), HasTitlebar(), closeButton, GetMSAA());
        //CloseButton = closeButton;
    }

    bool BackendMainWindowWin32_OpenGL3_3::HasCloseButton() const
    {
        //return GetWindowMode() == ssGUI::Enums::WindowMode::NORMAL ? CloseButton : false;
        return true;
    }

    void BackendMainWindowWin32_OpenGL3_3::SetWindowMode(ssGUI::Enums::WindowMode WindowMode)
    {
        //ResetWindow(WindowMode, IsResizable(), HasTitlebar(), HasCloseButton(), GetMSAA());
        //CurrentWindowMode = WindowMode;
    }

    ssGUI::Enums::WindowMode BackendMainWindowWin32_OpenGL3_3::GetWindowMode() const
    {
        //return CurrentWindowMode;
        return ssGUI::Enums::WindowMode::NORMAL;
    }

    bool BackendMainWindowWin32_OpenGL3_3::SetGLContext()
    {
        //return CurrentWindow.setActive();
        return true;
    }

    ssGUI::Backend::BackendMainWindowInterface* BackendMainWindowWin32_OpenGL3_3::Clone()
    {
        //return new BackendMainWindowWin32_OpenGL3_3(*this);
        return nullptr;
    }

    void* BackendMainWindowWin32_OpenGL3_3::GetRawHandle()
    {
        //return static_cast<void*>(&CurrentWindow);
        return nullptr;
    }
}

}