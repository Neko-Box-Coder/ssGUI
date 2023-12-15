#include "ssGUI/Backend/Template/BackendDrawingTemplate.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendDrawingTemplate::BackendDrawingTemplate( BackendDrawingTemplate const& other,
                                                    BackendMainWindowInterface* otherMainWindow) : 
        BackendDrawingInterface(otherMainWindow)
    {
    }

    BackendDrawingTemplate::BackendDrawingTemplate(BackendMainWindowInterface* mainWindowInterface) :
        BackendDrawingInterface(mainWindowInterface)
    {
        ssGUI_WARNING(ssGUI_BACKEND_TAG, "BackendDrawingTemplate is being created, is this intended?");
    }

    BackendDrawingTemplate::~BackendDrawingTemplate()
    {
    }
    
    void BackendDrawingTemplate::SaveDrawingState()
    {
    }

    void BackendDrawingTemplate::RestoreDrawingState()
    {
        
    }

    bool BackendDrawingTemplate::CreateDrawingEntities(const std::vector<DrawingEntity>& entities)
    {
        return true;
    }
    
    void BackendDrawingTemplate::DrawToBackBuffer()
    {
        
    }

    void BackendDrawingTemplate::Render(glm::u8vec3 clearColor)
    {

    }

    void BackendDrawingTemplate::ClearBackBuffer(glm::u8vec3 clearColor) 
    {
        
    }
    
    void BackendDrawingTemplate::AddImageCache(BackendImageInterface* backendImage)
    {
    
    }
    
    void BackendDrawingTemplate::RemoveImageCache(BackendImageInterface* backendImage)
    {
    
    }
    
    void* BackendDrawingTemplate::GetRawImageCacheHandle(BackendImageInterface* backendImage) const
    {
        return nullptr;
    }
}

}