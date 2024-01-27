#ifndef SSGUI_BACKEND_MAIN_WINDOW_X11_OPEN_GL_3_3_H
#define SSGUI_BACKEND_MAIN_WINDOW_X11_OPEN_GL_3_3_H

#include "ssGUI/Backend/Interfaces/MainWindowInterface.hpp"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <vector>

#include "glad/glad_glx.h"
#include "glad/glad.h"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //struct ssGUI::Backend::X11RawHandle
    struct X11RawHandle
    {
        //var: WindowDisplay
        ::Display* WindowDisplay;
        
        //var: WindowColormap
        ::Colormap WindowColormap;
        
        //var: WindowId
        ::Window WindowId;
        
        //var: WindowContext
        GLXContext WindowContext;
        
        //var: WindowCloseEventId
        ::Atom WindowCloseEventId;
        
        //var: XInputContext
        XIC XInputContext;
    };

    /*class: ssGUI::Backend::MainWindowInterface
    For functions explainations, please see <MainWindowInterface>. Normally you don't need to deal with this class
    
    Variables & Constructor:
    ======================== C++ =======================
    private:
        Display* WindowDisplay;                                                         //(Internal variable) The display server we have connected to, not to be confused by monitors
        Colormap WindowColormap;                                                        //(Internal variable) The colormap associated with the window
        Window WindowId;                                                                //(Internal variable) The window identifier
        GLXContext WindowContext;                                                       //(Internal variable) The window context
        XIM XInputManager;                                                              //(Internal variable) Input Manager object for X
        XIC XInputContext;                                                              //(Internal variable) Input context associated with XInputManager and the window for X
        unsigned short OriginalResolutionId;                                            //(Internal variable) Resolution ID before going into fullscreen
        bool OriginalResolutionSet;                                                     //(Internal variable) Is the above ID valid?
        bool IsClosingAborted;                                                          //(Internal variable) Flag to stop closing operation, see <AbortClosing>
        bool Closed;                                                                    //See <IsClosed>
        X11RawHandle CurrentHandle;                                                     //See <GetRawHandle>
        int MsaaLevel;                                                                  //See <GetAntiAliasingLevel>
        Atom WindowCloseEventId;                                                        //(Internal variable) Atom used for close event notification
        bool Titlebar;                                                                  //See <HasTitlebar>
        bool Resizable;                                                                 //See <IsResizable>
        bool CloseButton;                                                               //See <HasCloseButton>
        std::wstring Title;                                                             //See <GetTitle>
        ssGUI::Enums::WindowMode CurrentWindowMode;                                     //See <GetWindowMode>
        bool Visible;                                                                   //See <IsVisible>
        glm::ivec2 LastPositionBeforeHidden;                                            //(Internal variable) Used to set the position back to original when coming out from hidden state
        std::vector<std::function<void()>> OnCloseCallback;                             //See <AddOnCloseEvent>
        std::vector<std::function<void(bool focused)>> ExternalFocusChangedCallback;    //See <AddFocusChangedByUserEvent>
        bool VSync;                                                                     //See <IsVSync>
    ====================================================
    ======================== C++ =======================
    BackendMainWindowX11_OpenGL3_3::BackendMainWindowX11_OpenGL3_3() :  WindowDisplay(nullptr),
                                                                        WindowColormap(0),
                                                                        WindowId(0),
                                                                        WindowContext(),
                                                                        XInputManager(),
                                                                        XInputContext(),
                                                                        OriginalResolutionId(0),
                                                                        OriginalResolutionSet(false),
                                                                        IsClosingAborted(false),
                                                                        Closed(false),
                                                                        CurrentHandle(),
                                                                        MsaaLevel(0),
                                                                        WindowCloseEventId(),
                                                                        Titlebar(true),
                                                                        Resizable(true),
                                                                        CloseButton(true),
                                                                        Title(L""),
                                                                        CurrentWindowMode(ssGUI::Enums::WindowMode::NORMAL),
                                                                        Visible(true),
                                                                        LastPositionBeforeHidden(),
                                                                        OnCloseCallback(),
                                                                        ExternalFocusChangedCallback(),
                                                                        VSync(false)
    {        
        BackendMainWindowX11_OpenGL3_3::ssGUI_CreateWindow();
        
        ssGUI::Backend::BackendManager::AddMainWindowInterface(static_cast<ssGUI::Backend::MainWindowInterface*>(this));
    }
    ====================================================       
    */
    class BackendMainWindowX11_OpenGL3_3 : public MainWindowInterface
    {
        private:
            //https://stackoverflow.com/questions/5134297/xlib-how-does-this-removing-window-decoration-work
            struct MwmHintsData
            {
                unsigned long flags;
                unsigned long functions;
                unsigned long decorations;
                long input_mode;
                unsigned long status;
            };
        
            //https://gist.github.com/jimmiebergmann/5518391
            enum MWM_HINTS
            {
                MWM_HINTS_FUNCTIONS = (1L << 0),
                MWM_HINTS_DECORATIONS =  (1L << 1),
                MWM_HINTS_INPUT_MODE = (1L << 2),
                MWM_HINTS_STATUS = (1L << 3)
            };
            
            enum MWM_FUNC
            {
                MWM_FUNC_ALL = (1L << 0),
                MWM_FUNC_RESIZE = (1L << 1),
                MWM_FUNC_MOVE = (1L << 2),
                MWM_FUNC_MINIMIZE = (1L << 3),
                MWM_FUNC_MAXIMIZE = (1L << 4),
                MWM_FUNC_CLOSE = (1L << 5)
            };
            
            enum MWM_DECOR
            {
                MWM_DECOR_ALL = (1L << 0),
                MWM_DECOR_BORDER = (1L << 1),
                MWM_DECOR_RESIZEH = (1L << 2),
                MWM_DECOR_TITLE = (1L << 3),
                MWM_DECOR_MENU = (1L << 4),
                MWM_DECOR_MINIMIZE = (1L << 5),
                MWM_DECOR_MAXIMIZE = (1L << 6)
            };
            
            enum MWM_INPUT
            {
                MWM_INPUT_MODELESS = 0,
                MWM_INPUT_PRIMARY_APPLICATION_MODAL = 1,
                MWM_INPUT_SYSTEM_MODAL = 2,
                MWM_INPUT_FULL_APPLICATION_MODAL = 3
            };
            
            //#define PROP_MWM_HINTS_ELEMENTS             5
            //#define PROP_MWM_HINTS_ELEMENTS_MIN         4
    
        private:
            Display* WindowDisplay;                                                         //(Internal variable) The display server we have connected to, not to be confused by monitors
            Colormap WindowColormap;                                                        //(Internal variable) The colormap associated with the window
            Window WindowId;                                                                //(Internal variable) The window identifier
            GLXContext WindowContext;                                                       //(Internal variable) The window context
            XIM XInputManager;                                                              //(Internal variable) Input Manager object for X
            XIC XInputContext;                                                              //(Internal variable) Input context associated with XInputManager and the window for X
            unsigned short OriginalResolutionId;                                            //(Internal variable) Resolution ID before going into fullscreen
            bool OriginalResolutionSet;                                                     //(Internal variable) Is the above ID valid?
            bool IsClosingAborted;                                                          //(Internal variable) Flag to stop closing operation, see <AbortClosing>
            bool Closed;                                                                    //See <IsClosed>
            mutable X11RawHandle CurrentHandle;                                                     //See <GetRawHandle>
            int MsaaLevel;                                                                  //See <GetAntiAliasingLevel>
            Atom WindowCloseEventId;                                                        //(Internal variable) Atom used for close event notification
            bool Titlebar;                                                                  //See <HasTitlebar>
            bool Resizable;                                                                 //See <IsResizable>
            bool CloseButton;                                                               //See <HasCloseButton>
            std::wstring Title;                                                             //See <GetTitle>
            ssGUI::Enums::WindowMode CurrentWindowMode;                                     //See <GetWindowMode>
            bool Visible;                                                                   //See <IsVisible>
            glm::ivec2 LastPositionBeforeHidden;                                            //(Internal variable) Used to set the position back to original when coming out from hidden state
            std::vector<std::function<void()>> OnCloseCallback;                             //See <AddOnCloseEvent>
            std::vector<std::function<void(bool focused)>> ExternalFocusChangedCallback;    //See <AddFocusChangedByUserEvent>
            bool VSync;                                                                     //See <IsVSync>
            
            BackendMainWindowX11_OpenGL3_3& operator=(BackendMainWindowX11_OpenGL3_3 const& other);

            bool ssGUI_CreateWindow();
            void ssGUI_DestroyWindow();

            void UpdateWindowCapability();
            void UpdateWindowDecor();
            
            void GetWindowDecor(int& topExtent, int& rightExtent, int& botExtent, int& leftExtent) const;
            bool SetScreenResolution(glm::ivec2 targetResolution);
            void ReturnToOriginalResolution();
            
            //TODO: Maybe remove this
            ////Helper functions for manipulating _NET_WM_ALLOWED_ACTIONS atoms
            //bool GetAllowedWindowActions(Atom** atoms, int& count);
            //bool AddAllowedWindowActionIfNotExists(const char* actionName);
            //bool RemoveAllowedWindowActionIfExists(const char* actionName);

        protected:
            BackendMainWindowX11_OpenGL3_3(BackendMainWindowX11_OpenGL3_3 const& other);

        public:        
            BackendMainWindowX11_OpenGL3_3();
            ~BackendMainWindowX11_OpenGL3_3() override;

            //function: SetWindowPosition
            //See <MainWindowInterface::SetWindowPosition>
            void SetWindowPosition(glm::ivec2 pos) override;

            //function: GetWindowPosition
            //See <MainWindowInterface::GetWindowPosition>
            glm::ivec2 GetWindowPosition() const override;

            //function: GetPositionOffset
            //See <MainWindowInterface::GetPositionOffset>
            glm::ivec2 GetPositionOffset() const override;

            //function: SetWindowSize
            //See <MainWindowInterface::SetWindowSize>
            void SetWindowSize(glm::ivec2 size) override;

            //function: GetWindowSize
            //See <MainWindowInterface::GetWindowSize>
            glm::ivec2 GetWindowSize() const override;

            //function: SetRenderSize
            //See <MainWindowInterface::SetRenderSize>
            void SetRenderSize(glm::ivec2 size) override;

            //function: GetRenderSize
            //See <MainWindowInterface::GetRenderSize>
            glm::ivec2 GetRenderSize() const override;

            //function: IsClosed
            //See <MainWindowInterface::IsClosed>
            bool IsClosed() const override;

            //function: Close
            //See <MainWindowInterface::Close>
            void Close() override;
            
            //function: AbortClosing
            //See <MainWindowInterface::AbortClosing>
            void AbortClosing() override;

            //function: AddOnCloseEvent
            //See <MainWindowInterface::AddOnCloseEvent>
            int AddOnCloseEvent(std::function<void()> func) override;

            //function: RemoveOnCloseEvent
            //See <MainWindowInterface::RemoveOnCloseEvent>
            void RemoveOnCloseEvent(int index) override;

            //function: SetTitle
            //See <MainWindowInterface::SetTitle>
            void SetTitle(std::wstring title) override;

            //function: GetTitle
            //See <MainWindowInterface::GetTitle>
            std::wstring GetTitle() const override;

            //function: SetIcon
            //See <MainWindowInterface::SetIcon>
            void SetIcon(const ssGUI::Backend::ImageInterface& iconImage) override;

            //function: SetVisible
            //See <MainWindowInterface::SetVisible>
            void SetVisible(bool visible) override;

            //function: IsVisible
            //See <MainWindowInterface::IsVisible>
            bool IsVisible() const override;

            //function: SetVSync
            //See <MainWindowInterface::SetVSync>
            void SetVSync(bool vSync) override;

            //function: IsVSync
            //See <MainWindowInterface::IsVSync>
            bool IsVSync() const override;

            //function: SetFocus
            //See <MainWindowInterface::SetFocus>
            void SetFocus(bool focus, bool externalByUser) override;
            
            //function: IsFocused
            //See <MainWindowInterface::IsFocused>
            bool IsFocused() const override;

            //function: AddFocusChangedByUserEvent
            //See <MainWindowInterface::AddFocusChangedByUserEvent>
            int AddFocusChangedByUserEvent(std::function<void(bool focused)> func) override;

            //function: RemoveFocusChangedByUserEvent
            //See <MainWindowInterface::RemoveFocusChangedByUserEvent>
            void RemoveFocusChangedByUserEvent(int index) override;

            //function: SetMSAA
            //See <MainWindowInterface::SetMSAA>
            void SetAntiAliasingLevel(int level) override;

            //function: GetAntiAliasingLevel
            //See <MainWindowInterface::GetAntiAliasingLevel>
            int GetAntiAliasingLevel() const override;

            //function: SetTitlebar
            //See <MainWindowInterface::SetTitlebar>
            void SetTitlebar(bool titlebar) override;

            //function: HasTitlebar
            //See <MainWindowInterface::HasTitlebar>
            bool HasTitlebar() const override;

            //function: SetResizable
            //See <MainWindowInterface::SetResizable>
            void SetResizable(bool resizable) override;

            //function: IsResizable
            //See <MainWindowInterface::IsResizable>
            bool IsResizable() const override;

            //function: SetCloseButton
            //See <MainWindowInterface::SetCloseButton>
            void SetCloseButton(bool closeButton) override;

            //function: HasCloseButton
            //See <MainWindowInterface::HasCloseButton>
            bool HasCloseButton() const override;

            //function: SetWindowMode
            //See <MainWindowInterface::SetWindowMode>
            void SetWindowMode(ssGUI::Enums::WindowMode windowMode) override;

            //function: GetWindowMode
            //See <MainWindowInterface::GetWindowMode>
            ssGUI::Enums::WindowMode GetWindowMode() const override;

            //function: SetGLContext
            //See <MainWindowInterface::SetGLContext>
            bool SetDrawingContext() override;
            
            //function: Clone
            //See <MainWindowInterface::Clone>
            MainWindowInterface* Clone() override;

            //function: GetRawHandle
            //See <MainWindowInterface::GetRawHandle>
            void* GetRawHandle() const override;
    };
}

}

#endif