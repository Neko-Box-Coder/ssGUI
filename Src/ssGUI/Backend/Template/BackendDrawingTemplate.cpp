#include "ssGUI/Backend/Template/BackendDrawingTemplate.hpp"

#include "ssLogger/ssLog.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendDrawingTemplate::BackendDrawingTemplate(BackendDrawingTemplate const& other)
    {
    
    }

    BackendDrawingTemplate::BackendDrawingTemplate()
    {
        ssLOG_LINE("BackendDrawingTemplate is being created, is this intended?");
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

    bool BackendDrawingTemplate::DrawEntities(  const std::vector<glm::vec2>& vertices, 
                                const std::vector<glm::vec2>& texCoords,
                                const std::vector<glm::u8vec4>& colors,
                                const std::vector<int>& counts,
                                const std::vector<ssGUI::DrawingProperty>& properties)
    {
        return true;   
    }

    void BackendDrawingTemplate::Render(glm::u8vec3 clearColor)
    {

    }

    void BackendDrawingTemplate::ClearBackBuffer(glm::u8vec3 clearColor) 
    {
        
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

    //NOTE: End index is exclusive
    bool BackendDrawingTemplate::DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            const uint32_t character,
                            int startIndex, int endIndex,
                            const ssGUI::Backend::BackendFontInterface& font,
                            int characterSize)
    {
        return true;        
    }

    bool BackendDrawingTemplate::DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            int startIndex, int endIndex,
                            const ssGUI::Backend::BackendImageInterface& image)
    {
        return true;        
    }


    bool BackendDrawingTemplate::DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::u8vec4>& colors,
                            int startIndex, int endIndex)
    {
        return true;        
    }
}

}