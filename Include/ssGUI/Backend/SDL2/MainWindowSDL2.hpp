#ifndef SSGUI_BACKEND_MAIN_WINDOW_SDL2_HPP
#define SSGUI_BACKEND_MAIN_WINDOW_SDL2_HPP

#include "ssGUI/Backend/Interfaces/MainWindowInterface.hpp"

#include "SDL.h"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    class DrawingSDL2;

    //class: ssGUI::Backend::MainWindowSDL2
    class MainWindowSDL2 : public MainWindowInterface
    {
        public:
            struct SDL_RawHandle
            {
                SDL_Window* Window;
                SDL_Renderer* Renderer;
            };
        
        private:
            SDL_Window* CurrentWindow;
            std::u32string WindowTitle;
            mutable glm::ivec2 WindowPosition;
            mutable glm::ivec2 WindowSize;
            bool WindowClosed;
            bool WindowClosingAborted;
            bool WindowHidden;
            bool WindowVsync;
            bool WindowResizable;
            DrawingSDL2* BackendDrawing;
            Enums::WindowMode CurrentWindowMode;
            SDL_Renderer* CurrentSDL_Renderer;
            mutable SDL_RawHandle CurrentSDLHandle;
            
            std::vector<std::function<void(MainWindowInterface* mainWindow)>> OnCloseCallback;
            std::vector<std::function<void( MainWindowInterface* mainWindow, 
                                            bool focused)>> ExternalFocusChangedCallback;
        
            MainWindowSDL2& operator=(MainWindowSDL2 const& other);
            
            bool CreateWindow();
            void DestroyWindow();

        protected:
            MainWindowSDL2(MainWindowSDL2 const& other);

        public:
            MainWindowSDL2();
            ~MainWindowSDL2() override;

            //function: SetDPIScaling
            glm::vec2 GetDPIScaling() const;

            //function: Initialize
            bool Initialize() override;

            //function: SetWindowPosition
            //See <MainWindowInterface::SetWindowPosition>
            void SetWindowPosition(glm::ivec2 pos) override;

            //function: GetWindowPosition
            //See <MainWindowInterface::GetWindowPosition>
            glm::ivec2 GetWindowPosition() const override;

            //function: GetDecorationOffsets
            //See <MainWindowInterface::GetDecorationOffsets>
            void GetDecorationOffsets(glm::ivec2& topLeft, glm::ivec2& bottomRight) const override;

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
            int AddOnCloseEvent(std::function<void(MainWindowInterface* mainWindow)> func) override;

            //function: RemoveOnCloseEvent
            //See <MainWindowInterface::RemoveOnCloseEvent>
            void RemoveOnCloseEvent(int id) override;

            //function: SetTitle
            //See <MainWindowInterface::SetTitle>
            void SetTitle(std::u32string title) override;

            //function: SetTitle
            //See <MainWindowInterface::SetTitle>
            void SetTitle(std::string title) override;

            //function: GetTitle
            //See <MainWindowInterface::GetTitle>
            void GetTitle(std::u32string& title) const override;

            //function: GetTitle
            //See <MainWindowInterface::GetTitle>
            void GetTitle(std::string& title) const override;

            //function: SetIcon
            //See <MainWindowInterface::SetIcon>
            void SetIcon(const ImageInterface& iconImage) override;

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
            int AddFocusChangedByUserEvent(std::function<void(  MainWindowInterface* mainWindow,
                                                                bool focused)> func) override;

            //function: RemoveFocusChangedByUserEvent
            //See <MainWindowInterface::RemoveFocusChangedByUserEvent>
            void RemoveFocusChangedByUserEvent(int id) override;

            //function: SetAntiAliasingLevel
            //See <MainWindowInterface::SetAntiAliasingLevel>
            void SetAntiAliasingLevel(int level) override;

            //function: GetAntiAliasingLevel
            //See <MainWindowInterface::GetAntiAliasingLevel>
            int GetAntiAliasingLevel() const override;

            //function: SetResizable
            //See <MainWindowInterface::SetResizable>
            void SetResizable(bool resizable) override;

            //function: IsResizable
            //See <MainWindowInterface::IsResizable>
            bool IsResizable() const override;

            //function: SetDecorationOptions
            //See <MainWindowInterface::SetDecorationOptions>
            bool SetDecorationOptions(Enums::WindowDecorationOptions options) override;
            
            //function: GetDecorationOptions
            //See <MainWindowInterface::GetDecorationOptions>
            Enums::WindowDecorationOptions GetDecorationOptions() const override;

            //function: SetWindowMode
            //See <MainWindowInterface::SetWindowMode>
            void SetWindowMode(Enums::WindowMode windowMode) override;

            //function: GetWindowMode
            //See <MainWindowInterface::GetWindowMode>
            Enums::WindowMode GetWindowMode() const override;

            //function: Clone
            //See <MainWindowInterface::Clone>
            MainWindowInterface* Clone() override;

            //function: GetRawHandle
            //See <MainWindowInterface::GetRawHandle>
            void* GetRawHandle() const override;
            
            void Internal_SetBackendDrawing(DrawingSDL2* backendDrawing);
            
            SDL_Renderer* Internal_GetSDLRenderer() const;
    };
}

}

#endif