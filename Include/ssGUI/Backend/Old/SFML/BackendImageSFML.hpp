#ifndef SSGUI_BACKEND_IMAGE_SFML_H
#define SSGUI_BACKEND_IMAGE_SFML_H

#include "ssGUI/Backend/Interfaces/ImageInterface.hpp"
#include "SFML/Graphics.hpp"

namespace ssGUI
{ 

//namespace: ssGUI::Backend
namespace Backend
{
    /*class: ssGUI::Backend::BackendImageSFML
    For functions explainations, please see <ImageInterface>. Normally you don't need to deal with this class
    
    Variables & Constructor:
    ========================== C++ ==========================
    private:
        sf::Texture GPUTexture; //See <GetRawHandle>
        sf::Image MemoryImage;  //Underlying copy in host memory, which can get the pixel pointer with <GetPixelPtr>
        bool GPUTextureValid;   //See <IsValid>
    =========================================================
    ========================== C++ ==========================
    BackendImageSFML::BackendImageSFML() :  GPUTexture(),
                                            MemoryImage(),
                                            GPUTextureValid(false)
    {}
    =========================================================
    */
    class BackendImageSFML : public ImageInterface
    {
        private:
            sf::Texture GPUTexture; //See <GetRawHandle>
            sf::Image MemoryImage;  //Underlying copy in host memory, which can get the pixel pointer with <GetPixelPtr>
            bool GPUTextureValid;   //See <IsValid>

            BackendImageSFML& operator=(BackendImageSFML const& other);
            
            

        protected:
            BackendImageSFML(BackendImageSFML const& other);

        public:
            BackendImageSFML();
            ~BackendImageSFML() override;
            
            //function: GetGPUTextureP
            //Gets the SFML Texture object
            // sf::Texture* GetGPUTextureP();

            //function: GetRawHandle
            //See <ImageInterface::GetRawHandle>
            void* GetRawHandle() override;

            //function: IsValid
            //See <ImageInterface::IsValid>
            bool IsValid() const override;
            
            //function: LoadFromPath
            //See <ImageInterface::LoadFromPath>
            //The supported image formats are bmp, png, tga, jpg, gif, psd, hdr and pic. Some format options are not supported, like progressive jpeg.
            bool LoadFromPath(std::string path) override;
            
            //function: LoadImgFileFromMemory
            //See <ImageInterface::LoadImgFileFromMemory>
            //The supported image formats are bmp, png, tga, jpg, gif, psd, hdr and pic. Some format options are not supported, like progressive jpeg.
            bool LoadImgFileFromMemory(const void * dataPtr, std::size_t size) override;
            
            //function: LoadRawFromMemory
            //See <ImageInterface::LoadRawFromMemory>
            bool LoadRawFromMemory(const void * dataPtr, ssGUI::ImageFormat format, glm::ivec2 imageSize) override;
            
            //function: GetSize
            //See <ImageInterface::GetSize>
            glm::ivec2 GetSize() const override;

            //function: GetPixelPtr
            //SFML does not support editting pixels from pointer directly.
            //This will return nullptr
            void* GetPixelPtr(ssGUI::ImageFormat& format) const override;

            //function: UpdateCache 
            //This is not used, SFML handles image memory internally
            void UpdateCache() override;
            
            //function: Internal_AddBackendDrawingRecord 
            //This is not used, SFML handles image memory internally
            void Internal_AddBackendDrawingRecord(ssGUI::Backend::DrawingInterface* backendDrawing) override;
            
            //function: Internal_RemoveBackendDrawingRecord 
            //This is not used, SFML handles image memory internally
            void Internal_RemoveBackendDrawingRecord(ssGUI::Backend::DrawingInterface* backendDrawing) override;

            //function: Clone
            ssGUI::Backend::ImageInterface* Clone() override;
    };
}

}

#endif