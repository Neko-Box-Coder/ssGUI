#include "ssGUI/Backend/Template/BackendDrawingTemplate.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendDrawingTemplate::BackendDrawingTemplate(BackendDrawingTemplate const& other)
    {
    
    }

    BackendDrawingTemplate::BackendDrawingTemplate()
    {
        ssGUI_WARNING(ssGUI_BACKEND_TAG, "BackendDrawingTemplate is being created, is this intended?");
    }

    BackendDrawingTemplate::~BackendDrawingTemplate()
    {
    }
    
    void BackendDrawingTemplate::SaveState()
    {
    }

    void BackendDrawingTemplate::RestoreState()
    {
        
    }

    bool BackendDrawingTemplate::DrawEntities(const std::vector<ssGUI::DrawingEntity>& entities)
    {
        return true;
    }

    void BackendDrawingTemplate::Render(glm::u8vec3 clearColor)
    {

    }

    void BackendDrawingTemplate::ClearBackBuffer(glm::u8vec3 clearColor) 
    {
        
    }
    
    void BackendDrawingTemplate::AddImageCache(ssGUI::Backend::BackendImageInterface* backendImage)
    {
    
    }
    
    void BackendDrawingTemplate::RemoveImageCache(ssGUI::Backend::BackendImageInterface* backendImage)
    {
    
    }
    
    void* BackendDrawingTemplate::GetRawImageCacheHandle(ssGUI::Backend::BackendImageInterface* backendImage)
    {
        return nullptr;
    }

    bool BackendDrawingTemplate::DrawShape( const std::vector<glm::vec2>& vertices, 
                                            const std::vector<glm::vec2>& texCoords,
                                            const std::vector<glm::u8vec4>& colors,
                                            const uint32_t character,
                                            const ssGUI::Backend::BackendFontInterface& font,
                                            int characterSize)
    {
        return true;        
    }

    bool BackendDrawingTemplate::DrawShape( const std::vector<glm::vec2>& vertices, 
                                            const std::vector<glm::vec2>& texCoords,
                                            const std::vector<glm::u8vec4>& colors,
                                            const ssGUI::Backend::BackendImageInterface& image)
    {
        return true;        
    }


    bool BackendDrawingTemplate::DrawShape( const std::vector<glm::vec2>& vertices, 
                                            const std::vector<glm::u8vec4>& colors)
    {
        return true;        
    }
}

}