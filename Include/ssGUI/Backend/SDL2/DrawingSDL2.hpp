#ifndef SSGUI_BACKEND_DRAWING_SDL2_HPP
#define SSGUI_BACKEND_DRAWING_SDL2_HPP

#include "ssGUI/Backend/Interfaces/DrawingInterface.hpp"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::DrawingSDL2
    class DrawingSDL2 : public DrawingInterface
    {
        private:
            //TODO(NOW): Change to sdl2 main window
            MainWindowInterface* MainWindow;
        
            DrawingSDL2& operator=(DrawingSDL2 const& other);

        protected:
            DrawingSDL2( DrawingSDL2 const& other,
                                MainWindowInterface* otherMainWindow);
        
        public:
            DrawingSDL2(MainWindowInterface* mainWindowInterface);
            ~DrawingSDL2() override;

            //function: SaveState
            //See <DrawingInterface::SaveState>
            void SaveDrawingState() override;

            //function: RestoreDrawingState
            //See <DrawingInterface::RestoreDrawingState>
            void RestoreDrawingState() override;

            //function: CreateDrawingEntities
            //See <DrawingInterface::CreateDrawingEntities>
            bool CreateDrawingEntities(const std::vector<DrawingEntity>& entities) override;

            //function: DrawToBackBuffer
            //See <DrawingInterface::DrawToBackBuffer>
            void DrawToBackBuffer() override;

            //function: Render
            //See <DrawingInterface::Render>
            void Render(glm::u8vec3 clearColor) override;

            //function: ClearBackBuffer
            //See <DrawingInterface::ClearBackBuffer>
            void ClearBackBuffer(glm::u8vec3 clearColor) override;

            //function: AddImageCache
            //See <DrawingInterface::AddImageCache>
            void AddImageCache(ImageInterface* backendImage) override;
            
            //function: RemoveImageCache
            //See <DrawingInterface::RemoveImageCache>
            void RemoveImageCache(ImageInterface* backendImage) override;
            
            //function: GetRawImageCacheHandle
            //See <DrawingInterface::GetRawImageCacheHandle>
            void* GetRawImageCacheHandle(ImageInterface* backendImage) const override;
    };
}

}

#endif