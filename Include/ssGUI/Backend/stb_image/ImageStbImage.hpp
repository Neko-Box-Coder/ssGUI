#ifndef SSGUI_BACKEND_IMAGE_STB_IMAGE_HPP
#define SSGUI_BACKEND_IMAGE_STB_IMAGE_HPP

#include "ssGUI/Backend/Interfaces/ImageInterface.hpp"

#include <vector>


namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    /*class: ssGUI::Backend::ImageStbImage
    For functions explainations, please see <DrawingInterface>. Normally you don't need to deal with this class
    
    Variables & Constructor:
    ======================== C++ =======================
    private:
        uint8_t* ImageBuffer;                                                           //See <GetPixelPtr>
        size_t ImageSizeInBytes;                                                        //(Internal variable) This can be calculated using CurrentImageFormat, ImageWidth and ImageHeight
        ImageFormat CurrentImageFormat;                                                 //See <GetPixelPtr>
        uint16_t ImageWidth;                                                            //See <GetSize>
        uint16_t ImageHeight;                                                           //See <GetSize>

        std::vector<ssGUI::Backend::DrawingInterface*> LinkedBackendDrawing;     //See <AddBackendDrawingLinking>
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
    class ImageStbImage : public ImageInterface
    {
        private:
            uint8_t* ImageBuffer;                                                           //See <GetPixelPtr>
            size_t ImageSizeInBytes;                                                        //(Internal variable) This can be calculated using CurrentImageFormat, ImageWidth and ImageHeight
            ImageFormat CurrentImageFormat;                                                 //See <GetPixelPtr>
            uint16_t ImageWidth;                                                            //See <GetSize>
            uint16_t ImageHeight;                                                           //See <GetSize>

            std::vector<DrawingInterface*> LinkedBackendDrawing;     //See <AddBackendDrawingLinking>
        
            ImageStbImage& operator=(ImageStbImage const& other);

        protected:
            ImageStbImage(ImageStbImage const& other);

        public:
            ImageStbImage();
            ~ImageStbImage() override;

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
            ImageStbImage* Clone() override;
    };
}

}
#endif