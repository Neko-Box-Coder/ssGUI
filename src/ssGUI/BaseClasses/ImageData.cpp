#include "ssGUI/BaseClasses/ImageData.hpp"

namespace ssGUI
{
    ImageData::ImageData(ImageData const& other)
    {
        BackendImage = BackendImage->Clone();
    }

    ImageData::ImageData()
    {
        BackendImage = ssGUI::Backend::BackendFactory::CreateBackendImageInterface();
    }

    ImageData::~ImageData()
    {
        delete BackendImage;
    }

    ssGUI::Backend::BackendImageInterface* ImageData::GetBackendImageInterface()
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

    bool ImageData::LoadImgFileFromMemory(void* dataPtr, std::size_t size)
    {
        return BackendImage->LoadImgFileFromMemory(dataPtr, size);
    }

    bool ImageData::LoadRawFromMemory(void* dataPtr, int width, int height)
    {
        return LoadRawFromMemory(dataPtr, width, height);
    }
            
    glm::ivec2 ImageData::GetSize() const
    {
        return BackendImage->GetSize();
    }

    ImageData* ImageData::Clone()
    {   
        return new ImageData(*this);
    }
}
