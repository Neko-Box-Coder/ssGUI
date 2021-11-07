#ifndef SSGUI_IMAGE_INTERFACE
#define SSGUI_IMAGE_INTERFACE

#include <string>
#include "glm/vec2.hpp"

//namespace: ssGUI::Backend
namespace ssGUI::Backend
{
    //class: BackendImageInterface
    class BackendImageInterface
    {
        private:
            // BackendImageInterface(const BackendImageInterface&);
            // BackendImageInterface& operator=(const BackendImageInterface&);
        
        public:
            BackendImageInterface(){}
            //function: IsValid
            //Returns true if image is loaded
            virtual bool IsValid() const = 0;

            //function: LoadFromPath
            //Please see <BackendImageSFML::LoadFromPath> for supported image format.
            virtual bool LoadFromPath(std::string path) = 0;

            //function: LoadImgFileFromMemory
            //Please see <BackendImageSFML::LoadImgFileFromMemory> for supported image format.
            virtual bool LoadImgFileFromMemory(void* dataPtr, std::size_t size) = 0;

            //function: LoadRawFromMemory
            //This loads an image in memory in the format of 32-bits rgba.
            virtual bool LoadRawFromMemory(void* dataPtr, int width, int height) = 0;
            
            //function: GetSize
            virtual glm::ivec2 GetSize() const = 0;

            //function: Clone
            virtual BackendImageInterface* Clone() = 0;

            //TODO : Add GetRawHandle()

    };
}

#endif