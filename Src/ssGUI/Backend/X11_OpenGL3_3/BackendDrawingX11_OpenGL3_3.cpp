#include "ssGUI/Backend/X11_OpenGL3_3/BackendDrawingX11_OpenGL3_3.hpp"

#include "ssGUI/Backend/BackendManager.hpp"
#include "ssGUI/Backend/X11_OpenGL3_3/BackendMainWindowX11_OpenGL3_3.hpp"

#include "glad/glad_glx.h"
#include "glad/glad.h"

#include "ssLogger/ssLog.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendDrawingX11_OpenGL3_3::BackendDrawingX11_OpenGL3_3(BackendDrawingX11_OpenGL3_3 const& other)
    {
    
    }

    BackendDrawingX11_OpenGL3_3::BackendDrawingX11_OpenGL3_3()
    {
        ssLOG_LINE("BackendDrawingX11_OpenGL3_3 is being created, is this intended?");
    }

    BackendDrawingX11_OpenGL3_3::~BackendDrawingX11_OpenGL3_3()
    {
    }
    
    void BackendDrawingX11_OpenGL3_3::SaveState()
    {
    }

    void BackendDrawingX11_OpenGL3_3::RestoreState()
    {
        
    }

    bool BackendDrawingX11_OpenGL3_3::DrawEntities(  const std::vector<glm::vec2>& vertices, 
                                const std::vector<glm::vec2>& texCoords,
                                const std::vector<glm::u8vec4>& colors,
                                const std::vector<int>& counts,
                                const std::vector<ssGUI::DrawingProperty>& properties)
    {
        return true;   
    }

    void BackendDrawingX11_OpenGL3_3::Render(glm::u8vec3 clearColor)
    {
        auto* rawHandle = static_cast<ssGUI::Backend::X11RawHandle*>(
                            ssGUI::Backend::BackendManager::GetMainWindowInterface(0)->GetRawHandle());
                
        glXSwapBuffers(rawHandle->WindowDisplay, rawHandle->WindowId);        
    }

    void BackendDrawingX11_OpenGL3_3::ClearBackBuffer(glm::u8vec3 clearColor) 
    {
        glClearColor(0.8, 0.6, 0.7, 1.0);
        
        glClear(GL_COLOR_BUFFER_BIT);
        
    }
    
    void BackendDrawingX11_OpenGL3_3::RemoveImageLinking(ssGUI::Backend::BackendImageInterface* backendImage)
    {
    
    }
    
    bool BackendDrawingX11_OpenGL3_3::DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            const uint32_t character,
                            const ssGUI::Backend::BackendFontInterface& font,
                            int characterSize)
    {
        return true;        
    }

    bool BackendDrawingX11_OpenGL3_3::DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            const ssGUI::Backend::BackendImageInterface& image)
    {
        return true;        
    }


    bool BackendDrawingX11_OpenGL3_3::DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::u8vec4>& colors)
    {
        return true;        
    }

    //NOTE: End index is exclusive
    bool BackendDrawingX11_OpenGL3_3::DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            const uint32_t character,
                            int startIndex, int endIndex,
                            const ssGUI::Backend::BackendFontInterface& font,
                            int characterSize)
    {
        return true;        
    }

    bool BackendDrawingX11_OpenGL3_3::DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            int startIndex, int endIndex,
                            const ssGUI::Backend::BackendImageInterface& image)
    {
        return true;        
    }


    bool BackendDrawingX11_OpenGL3_3::DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::u8vec4>& colors,
                            int startIndex, int endIndex)
    {
        return true;        
    }
}

}