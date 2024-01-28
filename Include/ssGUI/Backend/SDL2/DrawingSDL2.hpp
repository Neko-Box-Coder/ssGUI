#ifndef SSGUI_BACKEND_DRAWING_SDL2_HPP
#define SSGUI_BACKEND_DRAWING_SDL2_HPP

#include "ssGUI/Backend/Interfaces/DrawingInterface.hpp"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    class MainWindowSDL2;
    
    //class: ssGUI::Backend::DrawingSDL2
    class DrawingSDL2 : public DrawingInterface
    {
        private:
            //TODO(NOW): Change to sdl2 main window
            MainWindowSDL2* MainWindow;
        
            DrawingSDL2& operator=(DrawingSDL2 const& other);

        protected:
            DrawingSDL2( DrawingSDL2 const& other,
                                MainWindowInterface* otherMainWindow);
        
        public:
            DrawingSDL2();
            ~DrawingSDL2() override;

            //function: Initialize
            bool Initialize(MainWindowInterface* mainWindowInterface) override;

            //function: SaveState
            //See <DrawingInterface::SaveState>
            void SaveDrawingState() override;

            //function: RestoreDrawingState
            //See <DrawingInterface::RestoreDrawingState>
            void RestoreDrawingState() override;

            //function: QueueDrawingEntities
            //See <DrawingInterface::QueueDrawingEntities>
            bool QueueDrawingEntities(const std::vector<DrawingEntity>& entities) override;

            //function: UploadDrawingEntitiesGroup
            //See <DrawingInterface::UploadDrawingEntitiesGroup>
            int UploadDrawingEntitiesGroup(const std::vector<DrawingEntity>& group) override;
            
            //function: DiscardDrawingEntitiesGroup
            //See <DrawingInterface::DiscardDrawingEntitiesGroup>
            void DiscardDrawingEntitiesGroup(int groupId) override;
            
            //function: QueueDrawingEntitiesGroups
            //See <DrawingInterface::QueueDrawingEntitiesGroups>
            bool QueueDrawingEntitiesGroups(const std::vector<int> groupsIds) override;

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