#ifndef SSGUI_BACKEND_IMAGE_SFML
#define SSGUI_BACKEND_IMAGE_SFML

#include "ssGUI/Backend/Interfaces/BackendImageInterface.hpp"
#include "SFML/Graphics.hpp"

//namespace: ssGUI::Backend
namespace ssGUI::Backend
{
    //class: BackendImageSFML
    //For functions explainations, please see <BackendImageInterface>. Normally you don't need to deal with this class
    class BackendImageSFML : public BackendImageInterface
    {
        private:
            sf::Texture GPUTexture;
            bool GPUTextureValid;
            BackendImageSFML& operator=(BackendImageSFML const& other);

        protected:
            BackendImageSFML(BackendImageSFML const& other);

        public:
            BackendImageSFML();
            virtual ~BackendImageSFML();
            
            //function: GetGPUTextureP
            //Gets the SFML Texture object
            sf::Texture* GetGPUTextureP();

            //function: IsValid
            bool IsValid() const override;
            
            //function: LoadImgFileFromMemory
            //The supported image formats are bmp, png, tga, jpg, gif, psd, hdr and pic. Some format options are not supported, like progressive jpeg.
            bool LoadImgFileFromMemory(void* dataPtr, std::size_t size) override;
            
            //function: LoadFromPath
            //The supported image formats are bmp, png, tga, jpg, gif, psd, hdr and pic. Some format options are not supported, like progressive jpeg.
            bool LoadFromPath(std::string path) override;
            
            //function: LoadRawFromMemory
            bool LoadRawFromMemory(void* dataPtr, int width, int height) override;
            
            //function: GetSize
            glm::ivec2 GetSize() const override;

            //function: Clone
            ssGUI::Backend::BackendImageInterface* Clone() override;
    };
}

#endif