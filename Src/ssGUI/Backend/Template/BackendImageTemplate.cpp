#include "ssGUI/Backend/Template/BackendImageTemplate.hpp"


namespace ssGUI
{

namespace Backend
{
    BackendImageTemplate::BackendImageTemplate()
    {

    }

    BackendImageTemplate::~BackendImageTemplate()
    {

    }

    //function: GetRawHandle
    //Returns the actual backend handle if the image data is valid. Otherwise returns nullptr 
    void* BackendImageTemplate::GetRawHandle()
    {
        return nullptr;
    }

    //function: IsValid
    //Returns true if image is loaded
    bool BackendImageTemplate::IsValid() const
    {
        return true;
    }

    //function: LoadFromPath
    //Please see <BackendImageSFML::LoadFromPath> for supported image format.
    bool BackendImageTemplate::LoadFromPath(std::string path)
    {
        return true;
    }

    //function: LoadImgFileFromMemory
    //Please see <BackendImageSFML::LoadImgFileFromMemory> for supported image format.
    bool BackendImageTemplate::LoadImgFileFromMemory(void const * dataPtr, std::size_t size)
    {
        return true;
    }

    //function: LoadRawFromMemory
    //This loads an image in memory in the format of 32-bits rgba.
    bool BackendImageTemplate::LoadRawFromMemory(void const * dataPtr, int width, int height)
    {
        return true;
    }
    
    //function: GetSize
    //Returns the size of the image
    glm::ivec2 BackendImageTemplate::GetSize() const
    {
        return glm::ivec2();
    }

    //function: GetPixelPtr
    //Returns the pixel data pointer of the image
    const void* BackendImageTemplate::GetPixelPtr() const
    {
        return nullptr;
    }

    //function: Clone
    BackendImageInterface* BackendImageTemplate::Clone()
    {
        return nullptr;
    }
}

}