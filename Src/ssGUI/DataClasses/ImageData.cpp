#include "ssGUI/DataClasses/ImageData.hpp"

#include "ssGUI/Backend/BackendFactory.hpp"

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

    bool ImageData::LoadRawFromMemory(void const * dataPtr, ssGUI::ImageFormat format, glm::ivec2 imageSize)
    {
        return BackendImage->LoadRawFromMemory(dataPtr, format, imageSize);
    }
            
    glm::ivec2 ImageData::GetSize() const
    {
        return BackendImage->GetSize();
    }

    void* ImageData::GetPixelPtr(ssGUI::ImageFormat& format)
    {
        return BackendImage->GetPixelPtr(format);
    }

    ImageData* ImageData::Clone()
    {   
        return new ImageData(*this);
    }
}
