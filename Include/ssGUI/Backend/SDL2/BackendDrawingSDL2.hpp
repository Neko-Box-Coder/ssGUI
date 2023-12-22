#ifndef SSGUI_BACKEND_DRAWING_SDL2_HPP
#define SSGUI_BACKEND_DRAWING_SDL2_HPP

#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"

#include "SDL.h"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendDrawingSDL2
    class BackendDrawingSDL2 : public BackendDrawingInterface
    {
        private:
            //TODO(NOW): Change to sdl2 main window
            BackendMainWindowInterface* MainWindowInterface;
        
            BackendDrawingSDL2& operator=(BackendDrawingSDL2 const& other);

        protected:
            BackendDrawingSDL2( BackendDrawingSDL2 const& other,
                                BackendMainWindowInterface* otherMainWindow);
        
        public:
            BackendDrawingSDL2(BackendMainWindowInterface* mainWindowInterface);
            ~BackendDrawingSDL2() override;

            //function: SaveState
            //See <BackendDrawingInterface::SaveState>
            void SaveDrawingState() override;

            //function: RestoreDrawingState
            //See <BackendDrawingInterface::RestoreDrawingState>
            void RestoreDrawingState() override;

            //function: CreateDrawingEntities
            //See <BackendDrawingInterface::CreateDrawingEntities>
            bool CreateDrawingEntities(const std::vector<DrawingEntity>& entities) override;

            //function: DrawToBackBuffer
            //See <BackendDrawingInterface::DrawToBackBuffer>
            void DrawToBackBuffer() override;

            //function: Render
            //See <BackendDrawingInterface::Render>
            void Render(glm::u8vec3 clearColor) override;

            //function: ClearBackBuffer
            //See <BackendDrawingInterface::ClearBackBuffer>
            void ClearBackBuffer(glm::u8vec3 clearColor) override;

            //function: AddImageCache
            //See <BackendDrawingInterface::AddImageCache>
            void AddImageCache(BackendImageInterface* backendImage) override;
            
            //function: RemoveImageCache
            //See <BackendDrawingInterface::RemoveImageCache>
            void RemoveImageCache(BackendImageInterface* backendImage) override;
            
            //function: GetRawImageCacheHandle
            //See <BackendDrawingInterface::GetRawImageCacheHandle>
            void* GetRawImageCacheHandle(BackendImageInterface* backendImage) const override;
    };
}

}

#endif