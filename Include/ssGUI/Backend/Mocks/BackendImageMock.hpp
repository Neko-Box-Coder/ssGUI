#ifndef SSGUI_BACKEND_IMAGE_MOCK_H
#define SSGUI_BACKEND_IMAGE_MOCK_H

//#define SSGUI_MOCK_ENABLE_LOG
#include "ssGUI/Backend/Mocks/MockMacro.hpp"

#include "CppOverride.hpp"
#include "ssGUI/Backend/Interfaces/BackendImageInterface.hpp"
#include <vector>

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendImageMock
    class BackendImageMock : public BackendImageInterface
    {
        private:
            BackendImageInterface* UnderlyingInterface;
            std::vector<BackendDrawingInterface*> LinkedBackendDrawing;     //See <AddBackendDrawingLinking>
            CO_DECLARE_MEMBER_INSTNACE(OverrideObject);
            
            BackendImageMock& operator=(BackendImageMock const& other);

        protected:
            BackendImageMock(BackendImageMock const& other);

        public:
            BackendImageMock(BackendImageInterface* imageInterface);
            ~BackendImageMock() override;

            CO_DECLARE_OVERRIDE_METHODS(OverrideObject)

            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(BackendImageInterface*, UnderlyingInterface)

            //function: GetRawHandle
            //See <BackendImageInterface::GetRawHandle>
            void* GetRawHandle() const override;

            //function: IsValid
            //See <BackendImageInterface::IsValid>
            bool IsValid() const override;

            //function: LoadFromPath
            //See <BackendImageInterface::LoadFromPath>
            bool LoadFromPath(std::string path) override;

            //function: LoadImgFileFromMemory
            //See <BackendImageInterface::LoadImgFileFromMemory>
            bool LoadImgFileFromMemory(const void * dataPtr, std::size_t size) override;

            //function: LoadRawFromMemory
            //See <BackendImageInterface::LoadRawFromMemory>
            bool LoadRawFromMemory( const void * dataPtr, 
                                    ImageFormat format, 
                                    glm::ivec2 imageSize) override;
            
            //function: GetSize
            //See <BackendImageInterface::GetSize>
            glm::ivec2 GetSize() const override;

            //function: GetPixelPtr
            //See <BackendImageInterface::GetPixelPtr>
            void* GetPixelPtr(ImageFormat& format) const override;
            
            //function: UpdateCache
            //See <BackendImageInterface::UpdateCache>
            void UpdateCache() override;
            
            //function: Internal_AddBackendDrawingRecord
            //See <BackendImageInterface::Internal_AddBackendDrawingRecord>
            void Internal_AddBackendDrawingRecord(BackendDrawingInterface* backendDrawing) override;

            //function: Internal_RemoveBackendDrawingRecord
            //See <BackendImageInterface::Internal_RemoveBackendDrawingRecord>
            void Internal_RemoveBackendDrawingRecord(BackendDrawingInterface* backendDrawing) override;

            //function: Clone
            //See <BackendImageInterface::Clone>
            BackendImageInterface* Clone() override;
    };
}

}
#endif