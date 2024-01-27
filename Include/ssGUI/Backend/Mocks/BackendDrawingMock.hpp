#ifndef SSGUI_BACKEND_DRAWING_MOCK_H
#define SSGUI_BACKEND_DRAWING_MOCK_H

//#define SSGUI_MOCK_ENABLE_LOG
#include "ssGUI/Backend/Mocks/MockMacro.hpp"

#include "ssGUI/Backend/Interfaces/DrawingInterface.hpp"
#include <unordered_set>

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendDrawingMock
    class BackendDrawingMock : public DrawingInterface
    {
        private:
            DrawingInterface* UnderlyingInterface;
            int SavedStateCount;
            int CurrentDrawingBuffer;
            
            std::vector<ssGUI::DrawingEntity> Entities[2];
            glm::u8vec3 ClearedColor[2];
            std::unordered_set<ImageInterface*> CachedImage;

            BackendDrawingMock& operator=(BackendDrawingMock const& other);

        protected:
            BackendDrawingMock( BackendDrawingMock const& other,
                                MainWindowInterface* otherMainWindow);
        
        public:
            BackendDrawingMock( DrawingInterface* drawingInterface,
                                MainWindowInterface* mainWindowInterface);
            ~BackendDrawingMock() override;
            
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(DrawingInterface*, UnderlyingInterface)
            
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(std::unordered_set<ImageInterface*>, CachedImage);
            
            //function: GetStateCounter
            int GetStateCounter() const;
            
            //function: GetDrawnProperties
            const std::vector<DrawingEntity>& GetDrawnEntities() const;
            
            //function: GetRenderedProperties
            const std::vector<DrawingEntity>& GetRenderedEntities() const;

            //function: GetDrawnClearedColor
            const glm::u8vec3& GetDrawnClearedColor() const;
            
            //function: GetRenderedClearedColor
            const glm::u8vec3& GetRenderedClearedColor() const;
            
            //function: SaveState
            //See <DrawingInterface::SaveState>
            void SaveDrawingState() override;

            //function: RestoreDrawingState
            //See <DrawingInterface::RestoreDrawingState>
            void RestoreDrawingState() override;

            //function: DrawEntities
            //See <DrawingInterface::DrawEntities>
            bool CreateDrawingEntities(  const std::vector<DrawingEntity>& entities) override;

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