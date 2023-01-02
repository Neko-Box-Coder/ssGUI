#include "ssGUI/Backend/Win32_OpenGL3_3/BackendMainWindowWin32_OpenGL3_3.hpp"
#include "ssGUI/Backend/Win32_OpenGL3_3/BackendSystemInputWin32_OpenGL3_3.hpp"

#include "ssGUI/Backend/BackendManager.hpp"

#include "ssGUI/HelperClasses/ImageUtil.hpp"

#include "ssLogger/ssLog.hpp"
#include <functional>

//Need to apply visual offset because GetWindowRect includes the invisble resize hitbox
#define VISUAL_OFFSET 7


namespace ssGUI
{

namespace Backend
{
    float BackendMainWindowWin32_OpenGL3_3::DeviceUnitToPixels(float unit, HWND hwnd)
    {
        float dpi = GetDpiForWindow(hwnd);
        return unit * (dpi / 96.0f);
    }

    float BackendMainWindowWin32_OpenGL3_3::PixelsToDeviceUnit(float unit, HWND hwnd)
    {
        float dpi = GetDpiForWindow(hwnd);
        return unit / (dpi / 96.0f);
    }
    
    LRESULT CALLBACK BackendMainWindowWin32_OpenGL3_3::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {     
        MSG msg = {hwnd, uMsg, wParam, lParam, 0, 0};

        bool consumed = false;
        
        auto inputInterface = static_cast<ssGUI::Backend::BackendSystemInputWin32_OpenGL3_3*>
                                (ssGUI::Backend::BackendManager::GetInputInterface());
        
        //It is possible to receive messages even before the backend input got initialized, so need nullptr check
        if(inputInterface != nullptr)
            consumed = inputInterface->HandleMessage(msg);

        if(consumed)
            return true;

        //To manually invoke a message, use PostMessage method

        //Pass back unprocessed message
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    void BackendMainWindowWin32_OpenGL3_3::SetWindowStyle()
    {
        LONG result = 0;

        if(!HasTitlebar() || GetWindowMode() == ssGUI::Enums::WindowMode::BORDERLESS)
            result = SetWindowLong(CurrentWindowHandle, GWL_STYLE, WS_POPUP | WS_VISIBLE | WS_SYSMENU);
        else
        {
            LONG windowStyle =  WS_OVERLAPPEDWINDOW | 
                                WS_CLIPSIBLINGS |
                                WS_CLIPCHILDREN;

            windowStyle &= IsResizable() ? windowStyle : ~WS_THICKFRAME;

            if(HasCloseButton())
                EnableMenuItem(GetSystemMenu(CurrentWindowHandle, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);
            else
                EnableMenuItem(GetSystemMenu(CurrentWindowHandle, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);            

            result = SetWindowLong( CurrentWindowHandle, 
                                    GWL_STYLE, 
                                    windowStyle);

            POINT winPos = GetRawPosition();
            POINT winSize = GetRawSize();
            if(!SetWindowPos(CurrentWindowHandle, HWND_TOP, winPos.x, winPos.y, winSize.x, winSize.y, SWP_SHOWWINDOW))
                ssLOG_LINE("Failed to set window position");
        }

        if(!result)
            ssLOG_LINE("Failed to change window style");
    }

    int BackendMainWindowWin32_OpenGL3_3::GetTitlebarHeight() const
    {
        // From firefox
        // mCaptionHeight is the default size of the NC area at
        // the top of the window. If the window has a caption,
        // the size is calculated as the sum of:
        //      SM_CYFRAME        - The thickness of the sizing border
        //                          around a resizable window
        //      SM_CXPADDEDBORDER - The amount of border padding
        //                          for captioned windows
        //      SM_CYCAPTION      - The height of the caption area
        //
        // If the window does not have a caption, mCaptionHeight will be equal to
        // `GetSystemMetrics(SM_CYFRAME)`
        int height = (GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION) +
            GetSystemMetrics(SM_CXPADDEDBORDER));

        return height;
    }

    POINT BackendMainWindowWin32_OpenGL3_3::GetRawPosition() const
    {
        RECT windowRect;

        if(!GetWindowRect(CurrentWindowHandle, &windowRect))
        {
            ssLOG_LINE("Failed to get window position");
            return POINT();
        }

        POINT pt;
        pt.x = windowRect.left;
        pt.y = windowRect.top;
        
        return pt;
    }

    POINT BackendMainWindowWin32_OpenGL3_3::GetRawSize() const
    {
        RECT windowRect;

        if(!GetWindowRect(CurrentWindowHandle, &windowRect))
        {
            ssLOG_LINE("Failed to get window size");
            return POINT();
        }

        POINT pt;
        pt.x = windowRect.right - windowRect.left;
        pt.y = windowRect.bottom - windowRect.top;
        
        return pt;
    }

    void BackendMainWindowWin32_OpenGL3_3::ssGUI_DestroyWindow()
    {
        wglMakeCurrent(NULL, NULL);                     //Release the OpenGL Render Context
        HDC hDC = GetDC(CurrentWindowHandle);
        ReleaseDC(CurrentWindowHandle, hDC);            //Release the Device Context
        wglDeleteContext(CurrentOpenGLContext);         //Deallocate the Render Context
        DestroyWindow(CurrentWindowHandle);             //Cleanup the window   
    }

    void BackendMainWindowWin32_OpenGL3_3::ssGUI_CreateWindow(int fullscreenWidth, int fullscreenHeight, bool generatePfid, bool generatePfDesc, const wchar_t* className)
    {
        HINSTANCE hInstance = GetModuleHandle(NULL);

        if(!SetProcessDPIAware())
            ssLOG_LINE("Failed to set DPI aware");

        if(GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN)
        {
            DEVMODE dmScreenSettings;                                       // Device Mode
            memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));         // Makes Sure Memory's Cleared
            dmScreenSettings.dmSize = sizeof(dmScreenSettings);             // Size Of The Devmode Structure
            dmScreenSettings.dmFields = DM_BITSPERPEL |
                                        DM_PELSWIDTH |
                                        DM_PELSHEIGHT;
            dmScreenSettings.dmPelsWidth    = fullscreenWidth;              // Selected Screen Width
            dmScreenSettings.dmPelsHeight   = fullscreenHeight;             // Selected Screen Height
            dmScreenSettings.dmBitsPerPel   = 32;                           // Selected Bits Per Pixel

            // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
            LONG result = ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN);
            if (result != DISP_CHANGE_SUCCESSFUL)
            {
                ssLOG_LINE("Failed to change to fullscreen: "<<result);
                ssLOG_LINE("falling back to window mode");
                CurrentWindowMode = ssGUI::Enums::WindowMode::NORMAL;
            }
        }

        DWORD exStyle = 0;
        DWORD style = 0;
        bool fullscreen = GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN;
        if(fullscreen)  // Are We Still In Fullscreen Mode?
        {
            exStyle = WS_EX_APPWINDOW;  // Window Extended Style
            style = WS_POPUP |          // Windows Style
                    WS_VISIBLE | 
                    WS_CLIPSIBLINGS | 
                    WS_CLIPCHILDREN | 
                    WS_SYSMENU;   
            //ShowCursor(FALSE);        // Hide Mouse Pointer
        }
        else    //Windowed
        {
            exStyle = WS_EX_LEFT;                       // Window Extended Style, left aligned text
            style = WS_OVERLAPPEDWINDOW |               // Window style, has titlebar and border
                    WS_CLIPSIBLINGS |                   // Diasble clipping so render each window seperately
                    WS_CLIPCHILDREN;    
        }

        std::wstring title = GetTitle();

        // Create the window.
        HWND hwnd = CreateWindowEx
        (
            exStyle,                // Extended Window Styles
            className,              // Window class
            title.c_str(),          // Window title text
            style,

            // Size and position
            CW_USEDEFAULT,                          //X (relative to parent)
            CW_USEDEFAULT,                          //Y (relative to parent)
            fullscreen ? fullscreenWidth : 800,     //Width, in DeviceUnits
            fullscreen ? fullscreenHeight : 800,    //Height, in DeviceUnits

            NULL,                                   // Parent window    
            NULL,                                   // Menu
            hInstance,                              // Instance handle
            NULL                                    // Additional application data
        );

        // Check if window is created sucessfully. If not, you can check with GetLastError
        if (hwnd == NULL)
        {
            ssLOG_LINE("Failed to create window");
            ssLOG_EXIT_PROGRAM();
        }

        //Get the device context for the window
        HDC hDC = GetDC(hwnd);
        if(hDC == NULL)
        {
            ssLOG_LINE("Failed to GetDC");
            ssLOG_EXIT_PROGRAM();
        }

        int fallback = 0;

        fallback:
        if(generatePfDesc)
            GeneratePixelFormatDescriptor(CurrentPictureFormatDescriptor);

        if(generatePfid)
        {
            int pf = 0;
            //Check if the pixel format we requested can be used
            pf = ChoosePixelFormat(hDC, &CurrentPictureFormatDescriptor);
            if(pf == 0)
            {
                ssLOG_LINE("Failed to ChoosePixelFormat");
                ssLOG_EXIT_PROGRAM();
            }
            CurrentPixelFormatId = pf;
        }

        //Set the pixel format for the window
        if(SetPixelFormat(hDC, CurrentPixelFormatId, &CurrentPictureFormatDescriptor) == FALSE)
        {
            ssLOG_LINE("Failed to SetPixelFormat");
            ssLOG_LINE("Falling back...");

            //Decreasing each bitdepth and see what works

            //Seems like no one uses Auxiliary buffer, this could be a problem
            if(CurrentPictureFormatDescriptor.cAuxBuffers == 32)
            {
                CurrentPictureFormatDescriptor.cAuxBuffers = 16;
                ssLOG_LINE("cAuxBuffers: "<<CurrentPictureFormatDescriptor.cAuxBuffers);
                goto fallback;
            }
            else if(CurrentPictureFormatDescriptor.cAuxBuffers == 16)
            {
                CurrentPictureFormatDescriptor.cAuxBuffers = 8;
                ssLOG_LINE("cAuxBuffers: "<<CurrentPictureFormatDescriptor.cAuxBuffers);
                goto fallback;
            }
            else if(CurrentPictureFormatDescriptor.cAuxBuffers == 8)
            {
                CurrentPictureFormatDescriptor.cAuxBuffers = 0;
                ssLOG_LINE("cAuxBuffers: "<<CurrentPictureFormatDescriptor.cAuxBuffers);
                goto fallback;
            }

            //Maybe try turning off accumulative buffer?
            else if(CurrentPictureFormatDescriptor.cAccumBits == 32)
            {
                CurrentPictureFormatDescriptor.cAccumBits = 0;
                ssLOG_LINE("cAccumBits: "<<CurrentPictureFormatDescriptor.cAccumBits);
                goto fallback;
            }
            
            //Maybe the MSAA level is not correct?
            else if(MsaaLevel > 0)
            {
                ssLOG_LINE("Trying to disable MSAA and retry...");
                generatePfid = true;
                MsaaLevel = 0;
                goto fallback;
            }

            //Normally stencil bit can be a problem when above 8 bits
            else if(CurrentPictureFormatDescriptor.cStencilBits == 32)
            {
                CurrentPictureFormatDescriptor.cStencilBits = 16;
                ssLOG_LINE("cStencilBits: "<<CurrentPictureFormatDescriptor.cStencilBits);
                goto fallback;
            }
            else if(CurrentPictureFormatDescriptor.cStencilBits == 16)
            {
                CurrentPictureFormatDescriptor.cStencilBits = 8;
                ssLOG_LINE("cStencilBits: "<<CurrentPictureFormatDescriptor.cStencilBits);
                goto fallback;
            }

            //Very unlikely for depth buffer to have problem but worth a try
            else if(CurrentPictureFormatDescriptor.cDepthBits == 32)
            {
                CurrentPictureFormatDescriptor.cDepthBits = 16;
                ssLOG_LINE("cDepthBits: "<<CurrentPictureFormatDescriptor.cDepthBits);
                goto fallback;
            }
            else if(CurrentPictureFormatDescriptor.cDepthBits == 16)
            {
                CurrentPictureFormatDescriptor.cDepthBits = 8;
                ssLOG_LINE("cDepthBits: "<<CurrentPictureFormatDescriptor.cDepthBits);
                goto fallback;
            }

            ssLOG_LINE("Failed to resolve a pixel format, exited");
            ssLOG_EXIT_PROGRAM();
        }

        //This function can be called to see if there is any changes to our chosen pixel format
        //DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

        //If there's another OpenGL window, the current Device Context can be released for now
        //ReleaseDC(hWnd, hDC);

        HGLRC hRC = wglCreateContext(hDC);        //Creates OpenGL Render Context
        if(hRC == NULL)
        {
            ssLOG_LINE("Failed to wglCreateContext");
            ssLOG_EXIT_PROGRAM();
        }

        if(!wglMakeCurrent(hDC, hRC))           //Select the OpenGL Render Context
        {
            ssLOG_LINE("Failed to wglMakeCurrent");
            ssLOG_EXIT_PROGRAM();
        }

        if(!gladLoadWGL(hDC))
        {
            ssLOG_LINE("Failed to gladLoadWGL");
            ssLOG_EXIT_PROGRAM();
        }

        if (!gladLoadGL())                  //Load Glad
        {
            ssLOG_LINE("Failed to gladLoadGL");
            ssLOG_EXIT_PROGRAM();
        }

        //Check OpenGL version
        if (GLVersion.major < 3 && GLVersion.minor < 3)
        {
            ssLOG_LINE("OpenGL version failed");
            ssLOG_EXIT_PROGRAM();
        }

        if(MsaaLevel > 0)
            glEnable(GL_MULTISAMPLE);
        else
            glDisable(GL_MULTISAMPLE);

        //Display the window
        ShowWindow(hwnd, SW_SHOW);

        /* //For debugging
        const int sampleAttributes[] = {WGL_SAMPLE_BUFFERS_ARB, WGL_SAMPLES_ARB};
        int       sampleValues[2];

        int pfID = GetPixelFormat(hDC);
        if (wglGetPixelFormatAttribivARB(hDC, pfID, PFD_MAIN_PLANE, 2, sampleAttributes, sampleValues) ==
            TRUE)
        {
            //m_settings.antialiasingLevel = static_cast<unsigned int>(sampleValues[0] ? sampleValues[1] : 0);
            ssLOG_LINE("WGL_SAMPLE_BUFFERS_ARB: "<<sampleValues[0]);
            ssLOG_LINE("WGL_SAMPLES_ARB: "<<sampleValues[1]);
        }
        else
            ssLOG_LINE("Failed");
        */

        CurrentWindowHandle = hwnd;
        CurrentOpenGLContext = hRC;

        if(!fullscreen)
            ApplyAllSettingsToWindow();
    }

    void BackendMainWindowWin32_OpenGL3_3::ApplyAllSettingsToWindow()
    {
        std::wstring title = GetTitle();
        SetTitle(title);
        bool visible = IsVisible();
        SetVisible(visible);
        SetWindowStyle();
    }

    bool BackendMainWindowWin32_OpenGL3_3::GetMsaaPixelFormatId(HDC hDC, PIXELFORMATDESCRIPTOR& pfd, int& pfid, int level)
    {
        if(!gladLoadWGL(hDC))
        {
            ssLOG_LINE("Failed to gladLoadWGL");
            return false;
        }

        if(!GLAD_WGL_ARB_multisample)
        {
            ssLOG_LINE("GLAD_WGL_ARB_multisample not supported");
            return false;
        }

        // fill attributes of pixel format that we want to use within our window
        int attributes[] = 
        {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
            WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
            WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB,     32,
            WGL_ALPHA_BITS_ARB,     8,
            WGL_DEPTH_BITS_ARB,     32,
            WGL_STENCIL_BITS_ARB,   8,
            WGL_ACCUM_BITS_ARB,     32,
            WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,                    // Enable multisampling
            WGL_SAMPLES_ARB,        level,                      // Number of samples
            0
        };

        // check if such pixel format is present in our system
        UINT numFormats;
        float fAttributes[] = {0,0};
        bool valid = wglChoosePixelFormatARB(hDC, attributes, fAttributes, 1, &pfid, &numFormats);

        if (valid && numFormats >= 1)
            return true;

        return false;
    }

    void BackendMainWindowWin32_OpenGL3_3::GeneratePixelFormatDescriptor(PIXELFORMATDESCRIPTOR& pfd)
    {
        //there is no guarantee that the contents of the stack that become
        //the pfd are zeroed, therefore _make sure_ to clear these bits.
        memset(&pfd, 0, sizeof(pfd));
        pfd.nSize        = sizeof(pfd);
        pfd.nVersion     = 1;
        pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType   = PFD_TYPE_RGBA;
        pfd.cColorBits   = 32;
        pfd.cAlphaBits   = 8;
        pfd.cAccumBits   = 32;
        pfd.cDepthBits   = 32;
        pfd.cStencilBits = 8;
        pfd.cAuxBuffers  = 32;
    }

    void BackendMainWindowWin32_OpenGL3_3::ssGUI_RegisterClass(const wchar_t* className)
    {
        HINSTANCE hInstance = GetModuleHandle(NULL);
        WNDCLASS wc = { };
        wc.style            = CS_OWNDC;                      //Using own drawing context
        wc.lpfnWndProc      = WindowProc; //Window process callback function
        wc.cbClsExtra       = 0;
        wc.cbWndExtra       = 0;
        wc.hInstance        = hInstance;                       //Instance handle
        wc.hIcon            = LoadIcon(NULL, IDI_WINLOGO);
        wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground    = NULL;
        wc.lpszMenuName     = NULL;
        wc.lpszClassName    = className;                        //Class name

        //Alternatively, you can use the Ex version
        if(!RegisterClass(&wc))
        {
            ssLOG_LINE("Failed to RegisterClass");
            ssLOG_EXIT_PROGRAM();
            return;
        }
    }

    bool BackendMainWindowWin32_OpenGL3_3::GetActiveMonitorPosSize(glm::ivec2& pos, glm::ivec2& size)
    {
        HMONITOR activeMonitor = MonitorFromWindow(CurrentWindowHandle, MONITOR_DEFAULTTONEAREST);
        MONITORINFO info;
        memset(&info, 0, sizeof(info));
        info.cbSize = sizeof(info);
        if(!GetMonitorInfo(activeMonitor, &info))
        {
            ssLOG_LINE("Failed to get active monitor");
            return false;
        }

        pos.x = info.rcMonitor.left;
        pos.y = info.rcMonitor.top;
        size.x = info.rcMonitor.right - info.rcMonitor.left;
        size.y = info.rcMonitor.bottom - info.rcMonitor.top;
        return true;
    }
    
    void BackendMainWindowWin32_OpenGL3_3::SetFullscreenSize(glm::ivec2 size)
    {
        //Iterate the monitor sizes
        DEVMODE fullscreenSettings;
        memset(&fullscreenSettings, 0, sizeof(fullscreenSettings));
        fullscreenSettings.dmSize = sizeof(fullscreenSettings);

        int settingsIndex = 0;

        glm::ivec2 finalizedSize = glm::ivec2();

        while(EnumDisplaySettings(NULL, settingsIndex++, &fullscreenSettings) != FALSE)
        {
            if( fullscreenSettings.dmBitsPerPel >= 32 && 
                size.x <= fullscreenSettings.dmPelsWidth &&
                size.y <= fullscreenSettings.dmPelsHeight)
            {
                finalizedSize = glm::ivec2(fullscreenSettings.dmPelsWidth, fullscreenSettings.dmPelsHeight);
                break;
            }
        }

        if(finalizedSize.x == 0 && finalizedSize.y == 0)
        {
            ssLOG_LINE("Failed to set size: "<<size.x<<", "<<size.y);
            return;
        }

        //ssLOG_LINE("Finalized size: "<<finalizedSize.x<<", "<<finalizedSize.y)

        ssGUI_DestroyWindow();
        ssGUI_CreateWindow(finalizedSize.x, finalizedSize.y, false, false, CLASS_NAME);
    }

    BackendMainWindowWin32_OpenGL3_3::BackendMainWindowWin32_OpenGL3_3(BackendMainWindowWin32_OpenGL3_3 const& other)
    {
        //TODO
        ssLOG_LINE("Not implemented");
        ssLOG_EXIT_PROGRAM();
    }

    BackendMainWindowWin32_OpenGL3_3::BackendMainWindowWin32_OpenGL3_3() :  CurrentWindowHandle(nullptr),
                                                                            CurrentOpenGLContext(NULL),
                                                                            CurrentPictureFormatDescriptor(),
                                                                            CurrentPixelFormatId(0),
                                                                            MsaaLevel(0),
                                                                            OriginalScreenResolution(),
                                                                            Closed(false),
                                                                            Visible(true),
                                                                            OnCloseCallback(),
                                                                            ExternalFocusChangedCallback(),
                                                                            Title(),
                                                                            Titlebar(true),
                                                                            Resizable(true),
                                                                            CloseButton(true),
                                                                            IsClosingAborted(false),
                                                                            PublicHandles(),
                                                                            CurrentWindowMode(ssGUI::Enums::WindowMode::NORMAL)
    {
        // Register the window class in order to create it
        
        //TODO: Bad way of doing it
        static bool oneTimeInit = false;

        if(!oneTimeInit)
        {
            oneTimeInit = true;
            ssGUI_RegisterClass(CLASS_NAME);
        }

        ssGUI_CreateWindow(0, 0, true, true, CLASS_NAME);

        ssGUI::Backend::BackendManager::AddMainWindowInterface(static_cast<ssGUI::Backend::BackendMainWindowInterface*>(this));
    }

    BackendMainWindowWin32_OpenGL3_3::~BackendMainWindowWin32_OpenGL3_3()
    {        
        if(!IsClosed())
        {
            ssGUI_DestroyWindow();
        }

        ssGUI::Backend::BackendManager::RemoveMainWindowInterface(static_cast<ssGUI::Backend::BackendMainWindowInterface*>(this));
    }
    
    void BackendMainWindowWin32_OpenGL3_3::SetWindowPosition(glm::ivec2 pos)
    {
        //Ignore borderless and fullscreen request
        if( GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN ||
            GetWindowMode() == ssGUI::Enums::WindowMode::BORDERLESS )
        {
            return;
        }
        
        POINT windowSize = GetRawSize();
        POINT pt;
        pt.x = pos.x;
        pt.y = pos.y;
        
        //Need to apply visual offset because GetWindowRect includes the invisble resize hitbox
        if(HasTitlebar())
            pt.x = pt.x - VISUAL_OFFSET;

        if(!SetWindowPos(CurrentWindowHandle, HWND_TOP, pt.x, pt.y, windowSize.x, windowSize.y, SWP_SHOWWINDOW))
            ssLOG_LINE("Failed to set window position");
    }

    glm::ivec2 BackendMainWindowWin32_OpenGL3_3::GetWindowPosition() const
    {
        if(GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN)
            return glm::ivec2(0, 0);

        RECT windowRect;

        if(!GetWindowRect(CurrentWindowHandle, &windowRect))
        {
            ssLOG_LINE("Failed to get window position");
            return glm::ivec2();
        }

        //Need to apply visual offset because GetWindowRect includes the invisble resize hitbox
        if(HasTitlebar())
            windowRect.left = windowRect.left + VISUAL_OFFSET;

        return glm::ivec2(windowRect.left, windowRect.top);
    }

    glm::ivec2 BackendMainWindowWin32_OpenGL3_3::GetPositionOffset() const
    {
        if(GetWindowMode() != ssGUI::Enums::WindowMode::NORMAL)
            return glm::ivec2(0, 0);
        
        POINT clientPt;

        clientPt.x = 0;
        clientPt.y = 0;

        if(!ClientToScreen(CurrentWindowHandle, &clientPt))
        {
            ssLOG_LINE("Failed to get position offset");
            return glm::ivec2();
        }
        
        RECT winRect;
        if(!GetWindowRect(CurrentWindowHandle, &winRect))
            ssLOG_LINE("Failed");

        //Hitbox offset
        //return glm::ivec2(clientPt.x - winRect.left, clientPt.y - winRect.top);
        
        //Visual offset
        return glm::ivec2(GetSystemMetrics(SM_CYBORDER), GetTitlebarHeight());
    }


    void BackendMainWindowWin32_OpenGL3_3::SetWindowSize(glm::ivec2 size)
    {
        //Ignore request for borderless winodw
        if(GetWindowMode() == ssGUI::Enums::WindowMode::BORDERLESS)
            return;

        if(GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN)
        {
            SetFullscreenSize(size);
            return;
        }

        POINT windowPos = GetRawPosition();

        if(HasTitlebar())
        {
            size.x += VISUAL_OFFSET * 2;
            size.y += VISUAL_OFFSET;
        }

        if(!SetWindowPos(CurrentWindowHandle, HWND_TOP, windowPos.x, windowPos.y, size.x, size.y, SWP_SHOWWINDOW))
            ssLOG_LINE("Failed to set window size");
    }

    glm::ivec2 BackendMainWindowWin32_OpenGL3_3::GetWindowSize() const
    {
        //This seems to work also for fullscreen

        RECT windowRect;

        if(!GetWindowRect(CurrentWindowHandle, &windowRect))
        {
            ssLOG_LINE("Failed to get window size");
            return glm::ivec2();
        }

        return glm::ivec2(  windowRect.right - windowRect.left - (HasTitlebar() ? VISUAL_OFFSET * 2 : 0), 
                            windowRect.bottom - windowRect.top - (HasTitlebar() ? VISUAL_OFFSET : 0));
    }

    void BackendMainWindowWin32_OpenGL3_3::SetRenderSize(glm::ivec2 size)
    {
        if(GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN)
        {
            SetFullscreenSize(size);
            return;
        }
    
        RECT renderSize;
        if(!GetClientRect(CurrentWindowHandle, &renderSize))
        {
            ssLOG_LINE("Failed to get render size");
            return;
        }

        DWORD dwStyle = (DWORD)GetWindowLong(CurrentWindowHandle, GWL_STYLE);
        renderSize.right = renderSize.left + size.x;
        renderSize.bottom = renderSize.top + size.y;

        if(!AdjustWindowRect(&renderSize, dwStyle, FALSE))
        {
            ssLOG_LINE("Failed to set render size");
            return;
        }
    }

    glm::ivec2 BackendMainWindowWin32_OpenGL3_3::GetRenderSize() const
    {
        RECT renderSize;
        if(!GetClientRect(CurrentWindowHandle, &renderSize))
        {
            ssLOG_LINE("Failed to get render size");
            return glm::ivec2();
        }
        
        return glm::ivec2(renderSize.right - renderSize.left, renderSize.bottom - renderSize.top);
    }

    bool BackendMainWindowWin32_OpenGL3_3::IsClosed() const
    {
        return Closed;
    }

    void BackendMainWindowWin32_OpenGL3_3::Close()
    {
        for(int i = 0; i < OnCloseCallback.size(); i++)
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

        ssGUI_DestroyWindow();
    }

    void BackendMainWindowWin32_OpenGL3_3::AbortClosing()
    {
        IsClosingAborted = true;
    }

    int BackendMainWindowWin32_OpenGL3_3::AddOnCloseEvent(std::function<void()> func)
    {
        OnCloseCallback.push_back(func);
        return OnCloseCallback.size() - 1;
    }

    void BackendMainWindowWin32_OpenGL3_3::RemoveOnCloseEvent(int index)
    {
        OnCloseCallback[index] = nullptr;
    }

    void BackendMainWindowWin32_OpenGL3_3::SetTitle(std::wstring title)
    {   
        if(!SetWindowText(CurrentWindowHandle, Title.c_str()))
        {
            ssLOG_LINE("Failed to set title");
            return;
        }

        Title = title;
    }

    std::wstring BackendMainWindowWin32_OpenGL3_3::GetTitle() const
    {
        return Title;
    }

    void BackendMainWindowWin32_OpenGL3_3::SetIcon(const ssGUI::Backend::BackendImageInterface& iconImage)
    {
        //From https://stackoverflow.com/questions/41533158/create-32-bit-color-icon-programmatically
        ICONINFO iconInfo = 
        {
            TRUE, // fIcon, set to true if this is an icon, set to false if this is a cursor
            NULL, // xHotspot, set to null for icons
            NULL, // yHotspot, set to null for icons
            NULL, // Monochrome bitmap mask, set to null initially
            NULL  // Color bitmap mask, set to null initially
        };

        uint8_t* rgbaImg = new uint8_t[iconImage.GetSize().x * iconImage.GetSize().y * 4];
        ssGUI::ImageFormat format;
        void* imgPtr = iconImage.GetPixelPtr(format);

        //TODO: Move this to somewhere else
        switch(format.BitDepthPerChannel)
        {
            case 8:
                ssGUI::ImageUtil::ConvertToRGBA32<uint8_t>(rgbaImg, imgPtr, format, iconImage.GetSize());
                break;
            case 16:
                ssGUI::ImageUtil::ConvertToRGBA32<uint16_t>(rgbaImg, imgPtr, format, iconImage.GetSize());
                break;
            default:
                ssLOG_LINE("Unsupported bit depth");
                return;
        }

        uint8_t* rawBitmap = new uint8_t[iconImage.GetSize().x * iconImage.GetSize().y * 4];

        //TODO: Move this conversion to ImageConversion
        for(int y = 0; y < iconImage.GetSize().y; y++)
        {
            for(int x = 0; x < iconImage.GetSize().x; x++)
            {
                //BGRA on Win32
                uint32_t curPixel = (y * iconImage.GetSize().x + x) * 4;
                rawBitmap[curPixel] = rgbaImg[curPixel + 2];
                rawBitmap[curPixel + 1] = rgbaImg[curPixel + 1];
                rawBitmap[curPixel + 2] = rgbaImg[curPixel];
                rawBitmap[curPixel + 3] = rgbaImg[curPixel + 3];
            }
        }

        HICON hIcon = NULL;
        bool success = true;
        iconInfo.hbmColor = CreateBitmap(iconImage.GetSize().x, iconImage.GetSize().y, 1, 32, rawBitmap);
        if (iconInfo.hbmColor) 
        {
            iconInfo.hbmMask = CreateCompatibleBitmap(GetDC(CurrentWindowHandle), iconImage.GetSize().x, iconImage.GetSize().y);
            if (iconInfo.hbmMask) 
            {
                hIcon = CreateIconIndirect(&iconInfo);
                if (hIcon == NULL) 
                {
                    ssLOG_LINE("Failed to create icon.");
                    success = false;
                }
                DeleteObject(iconInfo.hbmMask);
            } 
            else 
            {
                success = false;
                ssLOG_LINE("Failed to create color mask.");
            }
            DeleteObject(iconInfo.hbmColor);
        } 
        else 
        {
            success = false;
            ssLOG_LINE("Failed to create bitmap mask.");
        }

        delete[] rawBitmap;
        delete[] rgbaImg;

        if(success)
        {
            SendMessage(CurrentWindowHandle, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            SendMessage(CurrentWindowHandle, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            SendMessage(CurrentWindowHandle, WM_SETICON, ICON_SMALL2, (LPARAM)hIcon);
        }
    }

    void BackendMainWindowWin32_OpenGL3_3::SetVisible(bool visible)
    {
        Visible = visible;

        if(!visible && !ShowWindow(CurrentWindowHandle, SW_HIDE))
            ssLOG_LINE("Hide window failed");
        
        if(visible && !ShowWindow(CurrentWindowHandle, SW_SHOW))
            ssLOG_LINE("Show window failed");
    }

    bool BackendMainWindowWin32_OpenGL3_3::IsVisible() const
    {
        return Visible;
    }

    void BackendMainWindowWin32_OpenGL3_3::SetVSync(bool vSync)
    {
        //For details of this function, check https://registry.khronos.org/OpenGL/extensions/EXT/WGL_EXT_swap_control.txt
        wglSwapIntervalEXT(vSync);
    }

    bool BackendMainWindowWin32_OpenGL3_3::IsVSync() const
    {
        return wglGetSwapIntervalEXT() != FALSE;
    }

    bool BackendMainWindowWin32_OpenGL3_3::IsFocused() const
    {
        return GetActiveWindow() == CurrentWindowHandle;
    }

    void BackendMainWindowWin32_OpenGL3_3::SetFocus(bool focus, bool externalByUser)
    {
        if(externalByUser)
        {
            for(int i = 0; i < ExternalFocusChangedCallback.size(); i++)
            {
                if(ExternalFocusChangedCallback[i] != nullptr)
                    ExternalFocusChangedCallback[i](focus);
            }
        }
        else if(focus)
        {
            if(SetActiveWindow(CurrentWindowHandle) == NULL)
                ssLOG_LINE("Failed to set focus for backend mainwindow");
        }
    }

    int BackendMainWindowWin32_OpenGL3_3::AddFocusChangedByUserEvent(std::function<void(bool focused)> func)
    {
        ExternalFocusChangedCallback.push_back(func);
        return ExternalFocusChangedCallback.size() - 1;
    }

    void BackendMainWindowWin32_OpenGL3_3::RemoveFocusChangedByUserEvent(int index)
    {
        ExternalFocusChangedCallback[index] = nullptr;
    }

    int BackendMainWindowWin32_OpenGL3_3::GetMSAA() const
    {
        return MsaaLevel;
    }

    void BackendMainWindowWin32_OpenGL3_3::SetMSAA(int level)
    {        
        if(level != GetMSAA())
        {
            //PIXELFORMATDESCRIPTOR pfd;
            //GeneratePixelFormatDescriptor(pfd);
            int originalPfid = CurrentPixelFormatId;

            //Setup MSAA
            if(!GetMsaaPixelFormatId(GetDC(CurrentWindowHandle), CurrentPictureFormatDescriptor, CurrentPixelFormatId, level))
            {
                ssLOG_LINE("Failed to get MSAA Pixel Format Id");
                CurrentPixelFormatId = originalPfid;
            }
            else
            {
                MsaaLevel = level;

                glm::ivec2 oriSize = GetWindowSize();
                glm::ivec2 oriPos = GetWindowPosition();

                ssGUI_DestroyWindow();
            
                ssGUI_CreateWindow(oriSize.x, oriSize.y, false, false, CLASS_NAME);
                
                if(GetWindowMode() != ssGUI::Enums::WindowMode::FULLSCREEN)
                {
                    SetWindowPosition(oriPos);
                    SetWindowSize(oriSize);
                }
            }
        }
    }

    void BackendMainWindowWin32_OpenGL3_3::SetTitlebar(bool titlebar)
    {
        if(GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN || GetWindowMode() == ssGUI::Enums::WindowMode::BORDERLESS)
            return;
        
        Titlebar = titlebar;
        SetWindowStyle();
    }

    bool BackendMainWindowWin32_OpenGL3_3::HasTitlebar() const
    {
        if(GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN || GetWindowMode() == ssGUI::Enums::WindowMode::BORDERLESS)
            return false;
        
        //return GetWindowMode() == ssGUI::Enums::WindowMode::NORMAL ? Titlebar : false;
        return Titlebar;
    }

    void BackendMainWindowWin32_OpenGL3_3::SetResizable(bool resizable)
    {
        if(GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN || GetWindowMode() == ssGUI::Enums::WindowMode::BORDERLESS)
            return;
        
        Resizable = resizable;
        SetWindowStyle();
    }

    bool BackendMainWindowWin32_OpenGL3_3::IsResizable() const
    {
        if(GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN || GetWindowMode() == ssGUI::Enums::WindowMode::BORDERLESS)
            return false;
        
        return Resizable;
    }

    void BackendMainWindowWin32_OpenGL3_3::SetCloseButton(bool closeButton)
    {
        if(GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN || GetWindowMode() == ssGUI::Enums::WindowMode::BORDERLESS)
            return;
        
        CloseButton = closeButton;
        SetWindowStyle();
    }

    bool BackendMainWindowWin32_OpenGL3_3::HasCloseButton() const
    {
        if(GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN || GetWindowMode() == ssGUI::Enums::WindowMode::BORDERLESS)
            return false;
        
        return CloseButton;
    }

    void BackendMainWindowWin32_OpenGL3_3::SetWindowMode(ssGUI::Enums::WindowMode windowMode)
    {
        if(CurrentWindowMode == windowMode)
            return;

        ssGUI::Enums::WindowMode oriWindowMode = CurrentWindowMode;
        glm::ivec2 oriSize = GetWindowSize();
        glm::ivec2 oriPos = GetWindowPosition();

        //If we are coming from fullscreen, change the resolution back
        //to the original screen resolution
        if(CurrentWindowMode == ssGUI::Enums::WindowMode::FULLSCREEN)
            SetWindowSize(OriginalScreenResolution);

        CurrentWindowMode = windowMode;

        switch(CurrentWindowMode)
        {
            case ssGUI::Enums::WindowMode::NORMAL:
                ssGUI_DestroyWindow();  //Technically don't need to do this... but just in case      
                ssGUI_CreateWindow(0, 0, false, false, CLASS_NAME);
                break;
            case ssGUI::Enums::WindowMode::BORDERLESS:
            {
                glm::ivec2 pos;
                glm::ivec2 size;
                if(!GetActiveMonitorPosSize(pos, size))
                {
                    ssLOG_LINE("Failed to get monitro info");
                    ssLOG_LINE("Falling back");
                    CurrentWindowMode = oriWindowMode;
                    break;
                }

                ssGUI_DestroyWindow();  //Technically don't need to do this... but just in case      
                ssGUI_CreateWindow(0, 0, false, false, CLASS_NAME);

                SetWindowStyle();

                if(!SetWindowPos(CurrentWindowHandle, HWND_TOP, pos.x, pos.y, size.x, size.y, SWP_SHOWWINDOW))
                    ssLOG_LINE("Failed to set window size");

                break;
            }
            case ssGUI::Enums::WindowMode::FULLSCREEN:
                glm::ivec2 dummy;
                GetActiveMonitorPosSize(dummy, OriginalScreenResolution);
                
                SetWindowSize(oriSize);   //Set Size will handle all the window stuff
                break;
        }
    }

    ssGUI::Enums::WindowMode BackendMainWindowWin32_OpenGL3_3::GetWindowMode() const
    {
        return CurrentWindowMode;
    }

    bool BackendMainWindowWin32_OpenGL3_3::SetGLContext()
    {
        bool success = wglMakeCurrent(GetDC(CurrentWindowHandle), CurrentOpenGLContext) != FALSE;
        return success;
    }

    ssGUI::Backend::BackendMainWindowInterface* BackendMainWindowWin32_OpenGL3_3::Clone()
    {
        return new BackendMainWindowWin32_OpenGL3_3(*this);
    }

    void* BackendMainWindowWin32_OpenGL3_3::GetRawHandle()
    {
        //A bit of a cheat and retarded but it works
        PublicHandles.WindowHandle = CurrentWindowHandle;
        PublicHandles.OpenGLRenderContext = CurrentOpenGLContext;
        return (void*)&PublicHandles;
    }
}

}