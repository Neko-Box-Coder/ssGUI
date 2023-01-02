#ifndef SSGUI_BACKEND_MAIN_WINDOW_X11_OPEN_GL_3_3_H
#define SSGUI_BACKEND_MAIN_WINDOW_X11_OPEN_GL_3_3_H

#include "ssGUI/Backend/Interfaces/BackendMainWindowInterface.hpp"

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
    struct X11RawHandle
    {
        Display* WindowDisplay;
        Colormap WindowColormap;
        Window WindowId;
        GLXContext WindowContext;
        Atom WindowCloseEventId;
    };

    //class: ssGUI::Backend::BackendMainWindowInterface
    class BackendMainWindowX11_OpenGL3_3 : public BackendMainWindowInterface
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
            BackendMainWindowX11_OpenGL3_3& operator=(BackendMainWindowX11_OpenGL3_3 const& other);

            Display* WindowDisplay;
            Colormap WindowColormap;
            Window WindowId;
            GLXContext WindowContext;
            bool Closed = false;
            X11RawHandle CurrentHandle;
            int MsaaLevel = 0;                                                                  //See <GetMSAA>
            Atom WindowCloseEventId;
            bool Titlebar = true;                                                                  //See <HasTitlebar>
            bool Resizable = true;                                                                 //See <IsResizable>
            bool CloseButton = true;                                                               //See <HasCloseButton>
            std::wstring Title = L"";                                                             //See <GetTitle>
            ssGUI::Enums::WindowMode CurrentWindowMode = Enums::WindowMode::NORMAL;                                     //See <GetWindowMode>
            bool Visible = true;                                                                   //See <IsVisible>
            glm::ivec2 LastPositionBeforeHidden;
            unsigned short OriginalResolutionId = 0;
            bool OriginalResolutionSet = false;

            std::vector<std::function<void()>> OnCloseCallback;                             //See <AddOnCloseEvent>
            std::vector<std::function<void(bool focused)>> ExternalFocusChangedCallback;    //See <AddFocusChangedByUserEvent>
            bool IsClosingAborted = false;                                                          //(Internal variable) Flag to stop closing operation, see <AbortClosing>
            
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
            //See <BackendMainWindowInterface::SetWindowPosition>
            void SetWindowPosition(glm::ivec2 pos) override;

            //function: GetWindowPosition
            //See <BackendMainWindowInterface::GetWindowPosition>
            glm::ivec2 GetWindowPosition() const override;

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