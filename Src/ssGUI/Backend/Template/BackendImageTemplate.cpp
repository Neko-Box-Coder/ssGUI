#include "ssGUI/Backend/Template/BackendImageTemplate.hpp"

#include "ssLogger/ssLog.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendImageTemplate::BackendImageTemplate(BackendImageTemplate const& other)
    {
    
    }

    BackendImageTemplate::BackendImageTemplate()
    {
        ssLOG_LINE("BackendImageTemplate is being created, is this intended?");
    }

    BackendImageTemplate::~BackendImageTemplate()
    {

    }

    void* BackendImageTemplate::GetRawHandle()
    {
        return nullptr;
    }

    bool BackendImageTemplate::IsValid() const
    {
        return true;
    }

    bool BackendImageTemplate::LoadFromPath(std::string path)
    {
        return true;
    }

    bool BackendImageTemplate::LoadImgFileFromMemory(void const * dataPtr, std::size_t size)
    {
        return true;
    }

    bool BackendImageTemplate::LoadRawFromMemory(void const * dataPtr, ssGUI::ImageFormat format, glm::ivec2 imageSize, int rowPaddingInBytes)
    {
        return true;
    }
    
    glm::ivec2 BackendImageTemplate::GetSize() const
    {
        return glm::ivec2();
    }

    void* BackendImageTemplate::GetPixelPtr(ssGUI::ImageFormat& format) const
    {
        return nullptr;
    }

    BackendImageInterface* BackendImageTemplate::Clone()
    {
        return nullptr;
    }
}

}