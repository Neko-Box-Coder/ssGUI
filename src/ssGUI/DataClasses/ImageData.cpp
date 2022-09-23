#include "ssGUI/DataClasses/ImageData.hpp"

namespace ssGUI
{
    ImageData::ImageData(ImageData const& other)
    {
        BackendImage = BackendImage->Clone();
    }

    ImageData::ImageData() :    BackendImage(nullptr)
    {
        BackendImage = ssGUI::Backend::BackendFactory::CreateBackendImageInterface();
    }

    ImageData::~ImageData()
    {
        delete BackendImage;
    }

    ssGUI::Backend::BackendImageInterface* ImageData::GetBackendImageInterface() const
    {
        return BackendImage;
    }

    bool ImageData::IsValid() const
    {
        return BackendImage->IsValid();
    }

    bool ImageData::LoadFromPath(std::string path)
    {
        return BackendImage->LoadFromPath(path);
    }

    bool ImageData::LoadImgFileFromMemory(void const * dataPtr, std::size_t size)
    {
        return BackendImage->LoadImgFileFromMemory(dataPtr, size);
    }

    bool ImageData::LoadRawFromMemory(void const * dataPtr, int width, int height)
    {
        return BackendImage->LoadRawFromMemory(dataPtr, width, height);
    }
            
    glm::ivec2 ImageData::GetSize() const
    {
        return BackendImage->GetSize();
    }

    const void* ImageData::GetPixelPtr() const
    {
        return BackendImage->GetPixelPtr();
    }

    ImageData* ImageData::Clone()
    {   
        return new ImageData(*this);
    }
}
