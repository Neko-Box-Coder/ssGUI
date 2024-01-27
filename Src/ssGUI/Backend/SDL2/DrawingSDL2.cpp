#include "ssGUI/Backend/SDL2/DrawingSDL2.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Backend
{
    DrawingSDL2::DrawingSDL2( DrawingSDL2 const& other,
                                            MainWindowInterface* otherMainWindow) : 
        DrawingInterface(otherMainWindow)
    {
        //TODO(NOW)
    }

    DrawingSDL2::DrawingSDL2(MainWindowInterface* mainWindowInterface) :
        DrawingInterface(mainWindowInterface)
    {
        //ssGUI_WARNING(ssGUI_BACKEND_TAG, "DrawingSDL2 is being created, is this intended?");
    }

    DrawingSDL2::~DrawingSDL2()
    {
    }
    
    void DrawingSDL2::SaveDrawingState()
    {
    }

    void DrawingSDL2::RestoreDrawingState()
    {
        
    }

    bool DrawingSDL2::CreateDrawingEntities(const std::vector<DrawingEntity>& entities)
    {
        return true;
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