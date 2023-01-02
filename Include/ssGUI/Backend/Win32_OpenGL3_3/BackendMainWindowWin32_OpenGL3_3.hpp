#ifndef SSGUI_BACKEND_MAIN_WINDOW_WIN32_OPEN_GL_3_3_H
#define SSGUI_BACKEND_MAIN_WINDOW_WIN32_OPEN_GL_3_3_H

#include "ssGUI/Backend/Interfaces/BackendMainWindowInterface.hpp"

#ifndef UNICODE
#define UNICODE
#define _UNICODE
#endif

#include <windows.h>            /* must include this before GL/gl.h */
#include "glad/glad.h"
#include "glad/glad_wgl.h"
#include "wglext.h"

#include <vector>
#include <string>

namespace ssGUI 
{ 
    
//namespace: ssGUI::Backend
namespace Backend
{
    //struct: ssGUI::Backend::Win32_OpenGL_Handles
    struct Win32_OpenGL_Handles
    {
        HWND WindowHandle;                                  //Win32 window handle
        HGLRC OpenGLRenderContext;                          //OpenGL render context, created by wglCreateContext
        PIXELFORMATDESCRIPTOR PictureFormatDescriptor;      //Win32 pixel format descriptor used for this window
        int PixelFormatId;                                  //Pixel format handle (per mainWindow) created by ChoosePixelFormat
    };

    /*class: ssGUI::Backend::BackendMainWindowInterface
    For functions explainations, please see <BackendMainWindowInterface>. Normally you don't need to deal with this class
    
    Variables & Constructor:
    ======================== C++ =======================
    private:
        HWND CurrentWindowHandle;                                                       //See <GetRawHandle>
        HGLRC CurrentOpenGLContext;                                                     //See <GetRawHandle>
        PIXELFORMATDESCRIPTOR CurrentPictureFormatDescriptor;                           //See <GetRawHandle>
        int CurrentPixelFormatId;                                                       //See <GetRawHandle>
        int MsaaLevel;                                                                  //See <GetMSAA>
        glm::ivec2 OriginalScreenResolution;                                            //(Internal variable) Used to record the screen resolution before going to fullscreen (with a different resolution)

        bool Closed;                                                                    //See <IsClosed>
        bool Visible;                                                                   //See <IsVisible>

        std::vector<std::function<void()>> OnCloseCallback;                             //See <AddOnCloseEvent>
        std::vector<std::function<void(bool focused)>> ExternalFocusChangedCallback;    //See <AddFocusChangedByUserEvent>
        
        std::wstring Title;                                                             //See <GetTitle>
        bool Titlebar;                                                                  //See <HasTitlebar>
        bool Resizable;                                                                 //See <IsResizable>
        bool CloseButton;                                                               //See <HasCloseButton>
        bool IsClosingAborted;                                                          //(Internal variable) Flag to stop closing operation, see <AbortClosing>
        Win32_OpenGL_Handles PublicHandles;                                             //See <GetRawHandle>
        ssGUI::Enums::WindowMode CurrentWindowMode;                                     //See <GetWindowMode>

        const wchar_t* CLASS_NAME = L"ssGUI MainWindow";                                //(Internal variable) Const C wide string to register this process
    ====================================================
    ======================== C++ =======================
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
    ====================================================
    */
    class BackendMainWindowWin32_OpenGL3_3 : public BackendMainWindowInterface
    {
        private:
            HWND CurrentWindowHandle;                                                       //See <GetRawHandle>
            HGLRC CurrentOpenGLContext;                                                     //See <GetRawHandle>
            PIXELFORMATDESCRIPTOR CurrentPictureFormatDescriptor;                           //See <GetRawHandle>
            int CurrentPixelFormatId;                                                       //See <GetRawHandle>
            int MsaaLevel;                                                                  //See <GetMSAA>
            glm::ivec2 OriginalScreenResolution;                                            //(Internal variable) Used to record the screen resolution before going to fullscreen (with a different resolution)

            bool Closed;                                                                    //See <IsClosed>
            bool Visible;                                                                   //See <IsVisible>

            std::vector<std::function<void()>> OnCloseCallback;                             //See <AddOnCloseEvent>
            std::vector<std::function<void(bool focused)>> ExternalFocusChangedCallback;    //See <AddFocusChangedByUserEvent>
            
            std::wstring Title;                                                             //See <GetTitle>
            bool Titlebar;                                                                  //See <HasTitlebar>
            bool Resizable;                                                                 //See <IsResizable>
            bool CloseButton;                                                               //See <HasCloseButton>
            bool IsClosingAborted;                                                          //(Internal variable) Flag to stop closing operation, see <AbortClosing>
            Win32_OpenGL_Handles PublicHandles;                                             //See <GetRawHandle>
            ssGUI::Enums::WindowMode CurrentWindowMode;                                     //See <GetWindowMode>

            const wchar_t* CLASS_NAME = L"ssGUI MainWindow";                                //(Internal variable) Const C wide string to register this process
        
            static float DeviceUnitToPixels(float unit, HWND hwnd);

            static float PixelsToDeviceUnit(float unit, HWND hwnd);

            static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

            void SetWindowStyle();

            int GetTitlebarHeight() const;

            POINT GetRawPosition() const;

            POINT GetRawSize() const;

            void ssGUI_DestroyWindow();

            void ssGUI_CreateWindow(int fullscreenWidth, int fullscreenHeight, bool generatePfid, bool generatePfDesc, const wchar_t* className);

            void ApplyAllSettingsToWindow();

            //From http://www.sccg.sk/~samuelcik/opengl/opengl_10.pdf
            //And also https://nehe.gamedev.net/tutorial/fullscreen_antialiasing/16008/
            bool GetMsaaPixelFormatId(HDC hDC, PIXELFORMATDESCRIPTOR& pfd, int& pfid, int level);

            void GeneratePixelFormatDescriptor(PIXELFORMATDESCRIPTOR& pfd);

            void ssGUI_RegisterClass(const wchar_t* className);

            bool GetActiveMonitorPosSize(glm::ivec2& pos, glm::ivec2& size);
            
            void SetFullscreenSize(glm::ivec2 size);
        
            BackendMainWindowWin32_OpenGL3_3& operator=(BackendMainWindowWin32_OpenGL3_3 const& other);
        
        protected:
            BackendMainWindowWin32_OpenGL3_3(BackendMainWindowWin32_OpenGL3_3 const& other);

        public:
            BackendMainWindowWin32_OpenGL3_3();
            ~BackendMainWindowWin32_OpenGL3_3() override;

            //function: SetWindowPosition
            //See <BackendMainWindowInterface::SetWindowPosition>
            void SetWindowPosition(glm::ivec2 pos) override;

            //function: GetWindowPosition
            //See <BackendMainWindowInterface::GetWindowPosition>
            glm::ivec2 GetWindowPosition() const override;

            //function: SyncPositionOffset
            //See <BackendMainWindowInterface::SyncPositionOffset>
            void SyncPositionOffset() override;

            //function: GetPositionOffset
            //See <BackendMainWindowInterface::GetPositionOffset>
            glm::ivec2 GetPositionOffset() const override;

            //function: SetWindowSize
            //See <BackendMainWindowInterface::SetWindowSize>
            void SetWindowSize(glm::ivec2 size) override;

            //function: GetWindowSize
            //See <BackendMainWindowInterface::GetWindowSize>
            glm::ivec2 GetWindowSize() const override;

            //function: SetRenderSize
            //See <BackendMainWindowInterface::SetRenderSize>
            void SetRenderSize(glm::ivec2 size) override;

            //function: GetRenderSize
            //See <BackendMainWindowInterface::GetRenderSize>
            glm::ivec2 GetRenderSize() const override;

            //function: IsClosed
            //See <BackendMainWindowInterface::IsClosed>
            bool IsClosed() const override;

            //function: Close
            //See <BackendMainWindowInterface::Close>
            void Close() override;
            
            //function: AbortClosing
            //See <BackendMainWindowInterface::AbortClosing>
            void AbortClosing() override;

            //function: AddOnCloseEvent
            //See <BackendMainWindowInterface::AddOnCloseEvent>
            int AddOnCloseEvent(std::function<void()> func) override;

            //function: RemoveOnCloseEvent
            //See <BackendMainWindowInterface::RemoveOnCloseEvent>
            void RemoveOnCloseEvent(int index) override;

            //function: SetTitle
            //See <BackendMainWindowInterface::SetTitle>
            void SetTitle(std::wstring title) override;

            //function: GetTitle
            //See <BackendMainWindowInterface::GetTitle>
            std::wstring GetTitle() const override;

            //function: SetIcon
            //See <BackendMainWindowInterface::SetIcon>
            void SetIcon(const ssGUI::Backend::BackendImageInterface& iconImage) override;

            //function: SetVisible
            //See <BackendMainWindowInterface::SetVisible>
            void SetVisible(bool visible) override;

            //function: IsVisible
            //See <BackendMainWindowInterface::IsVisible>
            bool IsVisible() const override;

            //function: SetVSync
            //See <BackendMainWindowInterface::SetVSync>
            void SetVSync(bool vSync) override;

            //function: IsVSync
            //See <BackendMainWindowInterface::IsVSync>
            bool IsVSync() const override;

            //function: SetFocus
            //See <BackendMainWindowInterface::SetFocus>
            void SetFocus(bool focus, bool externalByUser) override;
            
            //function: IsFocused
            //See <BackendMainWindowInterface::IsFocused>
            bool IsFocused() const override;

            //function: AddFocusChangedByUserEvent
            //See <BackendMainWindowInterface::AddFocusChangedByUserEvent>
            int AddFocusChangedByUserEvent(std::function<void(bool focused)> func) override;

            //function: RemoveFocusChangedByUserEvent
            //See <BackendMainWindowInterface::RemoveFocusChangedByUserEvent>
            void RemoveFocusChangedByUserEvent(int index) override;

            //function: SetMSAA
            //See <BackendMainWindowInterface::SetMSAA>
            void SetMSAA(int level) override;

            //function: GetMSAA
            //See <BackendMainWindowInterface::GetMSAA>
            int GetMSAA() const override;

            //function: SetTitlebar
            //See <BackendMainWindowInterface::SetTitlebar>
            void SetTitlebar(bool titlebar) override;

            //function: HasTitlebar
            //See <BackendMainWindowInterface::HasTitlebar>
            bool HasTitlebar() const override;

            //function: SetResizable
            //See <BackendMainWindowInterface::SetResizable>
            void SetResizable(bool resizable) override;

            //function: IsResizable
            //See <BackendMainWindowInterface::IsResizable>
            bool IsResizable() const override;

            //function: SetCloseButton
            //See <BackendMainWindowInterface::SetCloseButton>
            void SetCloseButton(bool closeButton) override;

            //function: HasCloseButton
            //See <BackendMainWindowInterface::HasCloseButton>
            bool HasCloseButton() const override;

            //function: SetWindowMode
            //See <BackendMainWindowInterface::SetWindowMode>
            void SetWindowMode(ssGUI::Enums::WindowMode windowMode) override;

            //function: GetWindowMode
            //See <BackendMainWindowInterface::GetWindowMode>
            ssGUI::Enums::WindowMode GetWindowMode() const override;

            //function: SetGLContext
            //See <BackendMainWindowInterface::SetGLContext>
            bool SetGLContext() override;
            
            //function: Clone
            //See <BackendMainWindowInterface::Clone>
            BackendMainWindowInterface* Clone() override;

            //function: GetRawHandle
            //See <BackendMainWindowInterface::GetRawHandle>
            void* GetRawHandle() override;
    };
} 

}

#endif