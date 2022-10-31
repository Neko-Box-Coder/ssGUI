#ifndef SSGUI_IMAGE_TEMPLATE
#define SSGUI_IMAGE_TEMPLATE

#include "ssGUI/Backend/Interfaces/BackendImageInterface.hpp"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendFontTemplate
    //This allows transferring the image data from the host memory to the gpu memory
    class BackendImageTemplate : public BackendImageInterface
    {
        private:
        
        public:
            BackendImageTemplate();
            ~BackendImageTemplate() override;

            //function: GetRawHandle
            //Returns the actual backend handle if the image data is valid. Otherwise returns nullptr 
            void* GetRawHandle() override;

            //function: IsValid
            //Returns true if image is loaded
            bool IsValid() const override;

            //function: LoadFromPath
            //Please see <BackendImageSFML::LoadFromPath> for supported image format.
            bool LoadFromPath(std::string path) override;

            //function: LoadImgFileFromMemory
            //Please see <BackendImageSFML::LoadImgFileFromMemory> for supported image format.
            bool LoadImgFileFromMemory(void const * dataPtr, std::size_t size) override;

            //function: LoadRawFromMemory
            //This loads an image in memory in the format of 32-bits rgba.
            bool LoadRawFromMemory(void const * dataPtr, int width, int height) override;
            
            //function: GetSize
            //Returns the size of the image
            glm::ivec2 GetSize() const override;

            //function: GetPixelPtr
            //Returns the pixel data pointer of the image
            const void* GetPixelPtr() const override;

            //function: Clone
            BackendImageInterface* Clone() override;
    };
}

}
#endif