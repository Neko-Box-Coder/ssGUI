#include "ssGUI/Backend/Template/BackendDrawingTemplate.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendDrawingTemplate::BackendDrawingTemplate( BackendDrawingTemplate const& other,
                                                    MainWindowInterface* otherMainWindow) : 
        DrawingInterface(otherMainWindow)
    {
    }

    BackendDrawingTemplate::BackendDrawingTemplate(MainWindowInterface* mainWindowInterface) :
        DrawingInterface(mainWindowInterface)
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
    
    void BackendDrawingTemplate::AddImageCache(ImageInterface* backendImage)
    {
    
    }
    
    void BackendDrawingTemplate::RemoveImageCache(ImageInterface* backendImage)
    {
    
    }
    
    void* BackendDrawingTemplate::GetRawImageCacheHandle(ImageInterface* backendImage) const
    {
        return nullptr;
    }
}

}