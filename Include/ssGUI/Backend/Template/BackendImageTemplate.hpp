#ifndef SSGUI_BACKEND_IMAGE_TEMPLATE_H
#define SSGUI_BACKEND_IMAGE_TEMPLATE_H

#include "ssGUI/Backend/Interfaces/ImageInterface.hpp"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendImageTemplate
    class BackendImageTemplate : public ImageInterface
    {
        private:
            BackendImageTemplate& operator=(BackendImageTemplate const& other);

        protected:
            BackendImageTemplate(BackendImageTemplate const& other);

        public:
            BackendImageTemplate();
            ~BackendImageTemplate() override;

            //function: GetRawHandle
            //See <ImageInterface::GetRawHandle>
            void* GetRawHandle() const override;

            //function: IsValid
            //See <ImageInterface::IsValid>
            bool IsValid() const override;

            //function: LoadFromPath
            //See <ImageInterface::LoadFromPath>
            bool LoadFromPath(std::string path) override;

            //function: LoadImgFileFromMemory
            //See <ImageInterface::LoadImgFileFromMemory>
            bool LoadImgFileFromMemory(const void * dataPtr, std::size_t size) override;

            //function: LoadRawFromMemory
            //See <ImageInterface::LoadRawFromMemory>
            bool LoadRawFromMemory( const void * dataPtr, 
                                    ImageFormat format, 
                                    glm::ivec2 imageSize) override;
            
            //function: GetSize
            //See <ImageInterface::GetSize>
            glm::ivec2 GetSize() const override;

            //function: GetPixelPtr
            //See <ImageInterface::GetPixelPtr>
            void* GetPixelPtr(ImageFormat& format) const override;
            
            //function: UpdateCache
            //See <ImageInterface::UpdateCache>
            void UpdateCache() override;
            
            //function: Internal_AddBackendDrawingRecord
            //See <ImageInterface::Internal_AddBackendDrawingRecord>
            void Internal_AddBackendDrawingRecord(DrawingInterface* backendDrawing) override;

            //function: Internal_RemoveBackendDrawingRecord
            //See <ImageInterface::Internal_RemoveBackendDrawingRecord>
            void Internal_RemoveBackendDrawingRecord(DrawingInterface* backendDrawing) override;

            //function: Clone
            //See <ImageInterface::Clone>
            ImageInterface* Clone() override;
    };
}

}
#endif