#ifndef SSGUI_BACKEND_DRAWING_MOCK_H
#define SSGUI_BACKEND_DRAWING_MOCK_H

//#define SSGUI_MOCK_ENABLE_LOG
#include "ssGUI/Backend/Mocks/MockMacro.hpp"

#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"
#include <unordered_set>

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendDrawingMock
    class BackendDrawingMock : public BackendDrawingInterface
    {
        private:
            BackendDrawingInterface* UnderlyingInterface;
            int SavedStateCount;
            int CurrentDrawingBuffer;
            
            std::vector<ssGUI::DrawingEntity> Entities[2];
            glm::u8vec3 ClearedColor[2];
            std::unordered_set<BackendImageInterface*> CachedImage;

            BackendDrawingMock& operator=(BackendDrawingMock const& other);

        protected:
            BackendDrawingMock( BackendDrawingMock const& other,
                                BackendMainWindowInterface* otherMainWindow);
        
        public:
            BackendDrawingMock( BackendDrawingInterface* drawingInterface,
                                BackendMainWindowInterface* mainWindowInterface);
            ~BackendDrawingMock() override;
            
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(BackendDrawingInterface*, UnderlyingInterface)
            
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(std::unordered_set<BackendImageInterface*>, CachedImage);
            
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
            //See <BackendDrawingInterface::SaveState>
            void SaveDrawingState() override;

            //function: RestoreDrawingState
            //See <BackendDrawingInterface::RestoreDrawingState>
            void RestoreDrawingState() override;

            //function: DrawEntities
            //See <BackendDrawingInterface::DrawEntities>
            bool CreateDrawingEntities(  const std::vector<DrawingEntity>& entities) override;

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