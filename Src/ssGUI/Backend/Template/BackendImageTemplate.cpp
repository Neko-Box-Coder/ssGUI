#include "ssGUI/Backend/Template/BackendImageTemplate.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendImageTemplate::BackendImageTemplate(BackendImageTemplate const& other)
    {
    
    }

    BackendImageTemplate::BackendImageTemplate()
    {
        ssGUI_WARNING(ssGUI_TAG_BACKEND, "BackendImageTemplate is being created, is this intended?");
    }

    BackendImageTemplate::~BackendImageTemplate()
    {

    }

    void* BackendImageTemplate::GetRawHandle() const
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

    bool BackendImageTemplate::LoadImgFileFromMemory(const void * dataPtr, std::size_t size)
    {
        return true;
    }

    bool BackendImageTemplate::LoadRawFromMemory(   const void * dataPtr, 
                                                    ImageFormat format, 
                                                    glm::ivec2 imageSize)
    {
        return true;
    }
    
    glm::ivec2 BackendImageTemplate::GetSize() const
    {
        return glm::ivec2();
    }

    void* BackendImageTemplate::GetPixelPtr(ImageFormat& format) const
    {
        return nullptr;
    }

    void BackendImageTemplate::UpdateCache()
    {
        
    }

    void BackendImageTemplate::Internal_AddBackendDrawingRecord(DrawingInterface* backendDrawing)
    {
    }

    void BackendImageTemplate::Internal_RemoveBackendDrawingRecord(DrawingInterface* backendDrawing)
    {
    }

    ImageInterface* BackendImageTemplate::Clone()
    {
        return nullptr;
    }
}

}