#include "ssGUI/DataClasses/ImageData.hpp"

#include "ssGUI/Backend/BackendFactory.hpp"

namespace ssGUI
{
    ImageData::ImageData(ImageData const& other)
    {
        BackendImage = other.BackendImage->Clone();
    }

    ImageData::ImageData() :    BackendImage(nullptr),
                                DataChangedCallbacks(),
                                DataChangedCallbacksValid()
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

    int ImageData::AddDataChangedCallback(std::function<void(ssGUI::ImageData*)> callback)
    {
        DataChangedCallbacks.push_back(callback);
        DataChangedCallbacksValid.push_back(true);
        return DataChangedCallbacks.size() - 1;
    }
    
    void ImageData::RemoveDataChangedCallback(int id)
    {
        if(id < 0 || id >= DataChangedCallbacks.size())
            return;
        
        DataChangedCallbacksValid[id] = false;
    }
    
    void ImageData::NotifyImageDataChange()
    {
        for(int i = 0; i < DataChangedCallbacks.size(); i++)
        {
            if(!DataChangedCallbacksValid[i])
                continue;
            
            DataChangedCallbacks[i](this);
        }
    }

    bool ImageData::LoadFromPath(std::string path)
    {
        NotifyImageDataChange();
        return BackendImage->LoadFromPath(path);
    }

    bool ImageData::LoadImgFileFromMemory(void const * dataPtr, std::size_t size)
    {
        NotifyImageDataChange();
        return BackendImage->LoadImgFileFromMemory(dataPtr, size);
    }

    bool ImageData::LoadRawFromMemory(void const * dataPtr, ssGUI::ImageFormat format, glm::ivec2 imageSize)
    {
        NotifyImageDataChange();
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
    
    void ImageData::UpdateCache()
    {
        return BackendImage->UpdateCache();
    }

    ImageData* ImageData::Clone(bool copyCallbacks)
    {   
        ImageData* newImgData = new ImageData(*this);
        if(copyCallbacks)
        {
            newImgData->DataChangedCallbacks = DataChangedCallbacks;
            newImgData->DataChangedCallbacksValid = DataChangedCallbacksValid;
        }
        
        return newImgData;
    }
}
