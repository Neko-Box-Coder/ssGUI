#include "ssGUI/Backend/SDL2/BackendDrawingSDL2.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendDrawingSDL2::BackendDrawingSDL2( BackendDrawingSDL2 const& other,
                                            BackendMainWindowInterface* otherMainWindow) : 
        BackendDrawingInterface(otherMainWindow)
    {
        //TODO(NOW)
    }

    BackendDrawingSDL2::BackendDrawingSDL2(BackendMainWindowInterface* mainWindowInterface) :
        BackendDrawingInterface(mainWindowInterface)
    {
        //ssGUI_WARNING(ssGUI_BACKEND_TAG, "BackendDrawingSDL2 is being created, is this intended?");
    }

    BackendDrawingSDL2::~BackendDrawingSDL2()
    {
    }
    
    void BackendDrawingSDL2::SaveDrawingState()
    {
    }

    void BackendDrawingSDL2::RestoreDrawingState()
    {
        
    }

    bool BackendDrawingSDL2::CreateDrawingEntities(const std::vector<DrawingEntity>& entities)
    {
        return true;
    }
    
    void BackendDrawingSDL2::DrawToBackBuffer()
    {
        
    }

    void BackendDrawingSDL2::Render(glm::u8vec3 clearColor)
    {

    }

    void BackendDrawingSDL2::ClearBackBuffer(glm::u8vec3 clearColor) 
    {
        
    }
    
    void BackendDrawingSDL2::AddImageCache(BackendImageInterface* backendImage)
    {
    
    }
    
    void BackendDrawingSDL2::RemoveImageCache(BackendImageInterface* backendImage)
    {
    
    }
    
    void* BackendDrawingSDL2::GetRawImageCacheHandle(BackendImageInterface* backendImage) const
    {
        return nullptr;
    }
}

}