#ifndef SSGUI_BACKEND_IMAGE_TEMPLATE
#define SSGUI_BACKEND_IMAGE_TEMPLATE

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
            
            //function: AddBackendDrawingLinking
            //See <BackendImageInterface::AddBackendDrawingLinking>
            void AddBackendDrawingLinking(ssGUI::Backend::BackendDrawingInterface* backendDrawing) override;
            
            //function: RemoveBackendDrawingLinking
            //See <BackendImageInterface::RemoveBackendDrawingLinking>
            void RemoveBackendDrawingLinking(ssGUI::Backend::BackendDrawingInterface* backendDrawing) override;

            //function: Clone
            //See <BackendImageInterface::Clone>
            BackendImageInterface* Clone() override;
    };
}

}
#endif