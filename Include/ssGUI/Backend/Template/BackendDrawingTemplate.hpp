#ifndef SSGUI_BACKEND_DRAWING_TEMPLATE_H
#define SSGUI_BACKEND_DRAWING_TEMPLATE_H

#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendDrawingTemplate
    class BackendDrawingTemplate : public BackendDrawingInterface
    {
        private:
            BackendDrawingTemplate& operator=(BackendDrawingTemplate const& other);

        protected:
            BackendDrawingTemplate(BackendDrawingTemplate const& other);
        
        public:
            BackendDrawingTemplate();
            ~BackendDrawingTemplate() override;

            //function: SaveState
            //See <BackendDrawingInterface::SaveState>
            void SaveDrawingState() override;

            //function: RestoreDrawingState
            //See <BackendDrawingInterface::RestoreDrawingState>
            void RestoreDrawingState() override;

            //function: DrawEntities
            //See <BackendDrawingInterface::DrawEntities>
            bool DrawEntities(const std::vector<ssGUI::DrawingEntity>& entities) override;

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
            void AddImageCache(ssGUI::Backend::BackendImageInterface* backendImage) override;
            
            //function: RemoveImageCache
            //See <BackendDrawingInterface::RemoveImageCache>
            void RemoveImageCache(ssGUI::Backend::BackendImageInterface* backendImage) override;
            
            //function: GetRawImageCacheHandle
            //See <BackendDrawingInterface::GetRawImageCacheHandle>
            void* GetRawImageCacheHandle(ssGUI::Backend::BackendImageInterface* backendImage) override;
    };
}

}

#endif