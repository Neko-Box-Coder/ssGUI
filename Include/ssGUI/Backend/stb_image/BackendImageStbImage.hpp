#ifndef SSGUI_BACKEND_IMAGE_STB_IMAGE_H
#define SSGUI_BACKEND_IMAGE_STB_IMAGE_H

#include "ssGUI/Backend/Interfaces/BackendImageInterface.hpp"

#include <vector>


namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    /*class: ssGUI::Backend::BackendImageStbImage
    For functions explainations, please see <BackendDrawingInterface>. Normally you don't need to deal with this class
    
    Variables & Constructor:
    ======================== C++ =======================
    private:
        uint8_t* ImageBuffer;                                                           //See <GetPixelPtr>
        size_t ImageSizeInBytes;                                                        //(Internal variable) This can be calculated using CurrentImageFormat, ImageWidth and ImageHeight
        ImageFormat CurrentImageFormat;                                                 //See <GetPixelPtr>
        uint16_t ImageWidth;                                                            //See <GetSize>
        uint16_t ImageHeight;                                                           //See <GetSize>

        std::vector<ssGUI::Backend::BackendDrawingInterface*> LinkedBackendDrawing;     //See <AddBackendDrawingLinking>
    ====================================================
    ======================== C++ =======================
    BackendImageStbImage::BackendImageStbImage() :  ImageBuffer(nullptr),
                                                    ImageSizeInBytes(0),
                                                    CurrentImageFormat(),
                                                    ImageWidth(0),
                                                    ImageHeight(0)
    {
    }
    ====================================================
    */
    class BackendImageStbImage : public BackendImageInterface
    {
        private:
            uint8_t* ImageBuffer;                                                           //See <GetPixelPtr>
            size_t ImageSizeInBytes;                                                        //(Internal variable) This can be calculated using CurrentImageFormat, ImageWidth and ImageHeight
            ImageFormat CurrentImageFormat;                                                 //See <GetPixelPtr>
            uint16_t ImageWidth;                                                            //See <GetSize>
            uint16_t ImageHeight;                                                           //See <GetSize>

            std::vector<ssGUI::Backend::BackendDrawingInterface*> LinkedBackendDrawing;     //See <AddBackendDrawingLinking>
        
            BackendImageStbImage& operator=(BackendImageStbImage const& other);

        protected:
            BackendImageStbImage(BackendImageStbImage const& other);

        public:
            BackendImageStbImage();
            ~BackendImageStbImage() override;

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