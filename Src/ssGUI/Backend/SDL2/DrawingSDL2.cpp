#include "ssGUI/Backend/SDL2/DrawingSDL2.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Backend
{
    //TODO(NOW): OpenGL?
    //TODO(NOW): SDL_GL_SetAttribute before creating the window
    //TODO(NOW): SDL_RenderGeometry
    //TODO(NOW): glEnable(GL_MULTISAMPLE);
    
    DrawingSDL2::DrawingSDL2(   DrawingSDL2 const& other,
                                MainWindowInterface* otherMainWindow)
    {
        //TODO(NOW)
    }

    DrawingSDL2::DrawingSDL2()
    {
        //ssGUI_WARNING(ssGUI_TAG_BACKEND, "DrawingSDL2 is being created, is this intended?");
    }

    DrawingSDL2::~DrawingSDL2()
    {
    }
    
    bool DrawingSDL2::Initialize(MainWindowInterface* mainWindowInterface)
    {
        return true;
    }
    
    void DrawingSDL2::SaveDrawingState()
    {
    }

    void DrawingSDL2::RestoreDrawingState()
    {
        
    }

    bool DrawingSDL2::QueueDrawingEntities(const std::vector<DrawingEntity>& entities)
    {
        return true;
    }
    
    int DrawingSDL2::UploadDrawingEntitiesGroup(const std::vector<DrawingEntity>& group)
    {
        return -1;
    }
    
    void DrawingSDL2::DiscardDrawingEntitiesGroup(int groupId)
    {
        
    }
    
    bool DrawingSDL2::QueueDrawingEntitiesGroups(const std::vector<int> groupsIds)
    {
        return false;
    }
    
    void DrawingSDL2::DrawToBackBuffer()
    {
        
    }

    void DrawingSDL2::Render(glm::u8vec3 clearColor)
    {

    }

    void DrawingSDL2::ClearBackBuffer(glm::u8vec3 clearColor) 
    {
        
    }
    
    void DrawingSDL2::AddImageCache(ImageInterface* backendImage)
    {
    
    }
    
    void DrawingSDL2::RemoveImageCache(ImageInterface* backendImage)
    {
    
    }
    
    void* DrawingSDL2::GetRawImageCacheHandle(ImageInterface* backendImage) const
    {
        return nullptr;
    }
}

}