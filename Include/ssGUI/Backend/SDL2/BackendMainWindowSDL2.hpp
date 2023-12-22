#ifndef SSGUI_BACKEND_MAIN_WINDOW_SDL2_HPP
#define SSGUI_BACKEND_MAIN_WINDOW_SDL2_HPP

#include "ssGUI/Backend/Interfaces/BackendMainWindowInterface.hpp"

#include "SDL.h"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    class BackendDrawingSDL2;

    //class: ssGUI::Backend::BackendMainWindowSDL2
    class BackendMainWindowSDL2 : public BackendMainWindowInterface
    {
        private:
            SDL_Window* CurrentWindow;
            bool WindowClosed;
            bool WindowClosingAborted;
            bool WindowHidden;
            bool WindowVsync;
            bool WindowResizable;
            BackendDrawingSDL2* BackendDrawing;
            Enums::WindowMode CurrentWindowMode;
            SDL_Renderer* CurrentSDL_Renderer;
            
            std::vector<std::function<void()>> OnCloseCallback;
            std::vector<std::function<void(bool focused)>> ExternalFocusChangedCallback;
        
            BackendMainWindowSDL2& operator=(BackendMainWindowSDL2 const& other);
            
            void CreateWindow();
            void DestroyWindow();
            glm::vec2 GetDPIScaling() const;
            

        protected:
            BackendMainWindowSDL2(BackendMainWindowSDL2 const& other);

        public:
            BackendMainWindowSDL2();
            ~BackendMainWindowSDL2() override;

            //function: SetWindowPosition
            //See <BackendMainWindowInterface::SetWindowPosition>
            void SetWindowPosition(glm::ivec2 pos) override;

            //function: GetWindowPosition
            //See <BackendMainWindowInterface::GetWindowPosition>
            glm::ivec2 GetWindowPosition() const override;

            //function: GetDecorationOffsets
            //See <BackendMainWindowInterface::GetDecorationOffsets>
            void GetDecorationOffsets(glm::ivec2& topLeft, glm::ivec2& bottomRight) const override;

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
            void RemoveOnCloseEvent(int id) override;

            //function: SetTitle
            //See <BackendMainWindowInterface::SetTitle>
            void SetTitle(std::u32string title) override;

            //function: SetTitle
            //See <BackendMainWindowInterface::SetTitle>
            void SetTitle(std::string title) override;

            //function: GetTitle
            //See <BackendMainWindowInterface::GetTitle>
            void GetTitle(std::u32string& title) const override;

            //function: GetTitle
            //See <BackendMainWindowInterface::GetTitle>
            void GetTitle(std::string& title) const override;

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
            void RemoveFocusChangedByUserEvent(int id) override;

            //function: SetAntiAliasingLevel
            //See <BackendMainWindowInterface::SetAntiAliasingLevel>
            void SetAntiAliasingLevel(int level) override;

            //function: GetAntiAliasingLevel
            //See <BackendMainWindowInterface::GetAntiAliasingLevel>
            int GetAntiAliasingLevel() const override;

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
            void SetWindowMode(Enums::WindowMode windowMode) override;

            //function: GetWindowMode
            //See <BackendMainWindowInterface::GetWindowMode>
            Enums::WindowMode GetWindowMode() const override;

            //function: SetDrawingContext
            //See <BackendMainWindowInterface::SetDrawingContext>
            bool SetDrawingContext() override;
            
            //function: Clone
            //See <BackendMainWindowInterface::Clone>
            BackendMainWindowInterface* Clone() override;

            //function: GetRawHandle
            //See <BackendMainWindowInterface::GetRawHandle>
            void* GetRawHandle() const override;
            
            void Internal_SetBackendDrawing(BackendDrawingSDL2* backendDrawing);
            
            SDL_Renderer* Internal_GetSDLRenderer() const;
    };
}

}

#endif