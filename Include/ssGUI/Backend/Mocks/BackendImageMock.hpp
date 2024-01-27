#ifndef SSGUI_BACKEND_IMAGE_MOCK_H
#define SSGUI_BACKEND_IMAGE_MOCK_H

//#define SSGUI_MOCK_ENABLE_LOG
#include "ssGUI/Backend/Mocks/MockMacro.hpp"

#include "CppOverride.hpp"
#include "ssGUI/Backend/Interfaces/ImageInterface.hpp"
#include <vector>

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendImageMock
    class BackendImageMock : public ImageInterface
    {
        private:
            ImageInterface* UnderlyingInterface;
            std::vector<DrawingInterface*> LinkedBackendDrawing;     //See <AddBackendDrawingLinking>
            CO_DECLARE_MEMBER_INSTNACE(OverrideObject);
            
            BackendImageMock& operator=(BackendImageMock const& other);

        protected:
            BackendImageMock(BackendImageMock const& other);

        public:
            BackendImageMock(ImageInterface* imageInterface);
            ~BackendImageMock() override;

            CO_DECLARE_OVERRIDE_METHODS(OverrideObject)

            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(ImageInterface*, UnderlyingInterface)

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