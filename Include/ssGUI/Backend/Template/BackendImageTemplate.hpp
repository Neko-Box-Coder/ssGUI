#ifndef SSGUI_BACKEND_IMAGE_TEMPLATE_H
#define SSGUI_BACKEND_IMAGE_TEMPLATE_H

#include "ssGUI/Backend/Interfaces/BackendImageInterface.hpp"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendFontTemplate
    class BackendImageTemplate : public BackendImageInterface
    {
        private:
            BackendImageTemplate& operator=(BackendImageTemplate const& other);

        protected:
            BackendImageTemplate(BackendImageTemplate const& other);

        public:
            BackendImageTemplate();
            ~BackendImageTemplate() override;

            //function: GetRawHandle
            //See <BackendImageInterface::GetRawHandle>
            void* GetRawHandle() override;

            //function: IsValid
            //See <BackendImageInterface::IsValid>
            bool IsValid() const override;

            //function: LoadFromPath
            //See <BackendImageInterface::LoadFromPath>
            bool LoadFromPath(std::string path) override;

            //function: LoadImgFileFromMemory
            //See <BackendImageInterface::LoadImgFileFromMemory>
            bool LoadImgFileFromMemory(void const * dataPtr, std::size_t size) override;

            //function: LoadRawFromMemory
            //See <BackendImageInterface::LoadRawFromMemory>
            bool LoadRawFromMemory(void const * dataPtr, ssGUI::ImageFormat format, glm::ivec2 imageSize) override;
            
            //function: GetSize
            //See <BackendImageInterface::GetSize>
            glm::ivec2 GetSize() const override;

            //function: GetPixelPtr
            //See <BackendImageInterface::GetPixelPtr>
            void* GetPixelPtr(ssGUI::ImageFormat& format) const override;
            
            //function: UpdateCache
            //See <BackendImageInterface::UpdateCache>
            void UpdateCache() override;
            
            //function: Internal_AddBackendDrawingRecord
            //See <BackendImageInterface::Internal_AddBackendDrawingRecord>
            void Internal_AddBackendDrawingRecord(ssGUI::Backend::BackendDrawingInterface* backendDrawing) override;

            //function: Internal_RemoveBackendDrawingRecord
            //See <BackendImageInterface::Internal_RemoveBackendDrawingRecord>
            void Internal_RemoveBackendDrawingRecord(ssGUI::Backend::BackendDrawingInterface* backendDrawing) override;

            //function: Clone
            //See <BackendImageInterface::Clone>
            BackendImageInterface* Clone() override;
    };
}

}
#endif