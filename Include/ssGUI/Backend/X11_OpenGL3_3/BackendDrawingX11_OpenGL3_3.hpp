#ifndef SSGUI_BACKEND_DRAWING_X11_OPEN_GL_3_3_H
#define SSGUI_BACKEND_DRAWING_X11_OPEN_GL_3_3_H

#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendDrawingX11_OpenGL3_3
    class BackendDrawingX11_OpenGL3_3 : public BackendDrawingInterface
    {
        private:
            BackendDrawingX11_OpenGL3_3& operator=(BackendDrawingX11_OpenGL3_3 const& other);

        protected:
            BackendDrawingX11_OpenGL3_3(BackendDrawingX11_OpenGL3_3 const& other);
        
        public:
            BackendDrawingX11_OpenGL3_3();
            ~BackendDrawingX11_OpenGL3_3() override;

            //function: SaveState
            //See <BackendDrawingInterface::SaveState>
            void SaveState() override;

            //function: RestoreState
            //See <BackendDrawingInterface::RestoreState>
            void RestoreState() override;

            //function: DrawEntities
            //See <BackendDrawingInterface::DrawEntities>
            bool DrawEntities(  const std::vector<glm::vec2>& vertices, 
                                const std::vector<glm::vec2>& texCoords,
                                const std::vector<glm::u8vec4>& colors,
                                const std::vector<int>& counts,
                                const std::vector<ssGUI::DrawingProperty>& properties) override;

            //function: Render
            //See <BackendDrawingInterface::Render>
            void Render(glm::u8vec3 clearColor) override;

            //function: ClearBackBuffer
            //See <BackendDrawingInterface::ClearBackBuffer>
            void ClearBackBuffer(glm::u8vec3 clearColor) override;

            //function: RemoveImageLinking
            //See <BackendDrawingInterface::RemoveImageLinking>
            void RemoveImageLinking(ssGUI::Backend::BackendImageInterface* backendImage) override;
            
        protected:
            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            const uint32_t character,
                            const ssGUI::Backend::BackendFontInterface& font,
                            int characterSize) override;

            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            const ssGUI::Backend::BackendImageInterface& image) override;


            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::u8vec4>& colors) override;

            //NOTE: End index is exclusive
            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            const uint32_t character,
                            int startIndex, int endIndex,
                            const ssGUI::Backend::BackendFontInterface& font,
                            int characterSize) override;

            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            int startIndex, int endIndex,
                            const ssGUI::Backend::BackendImageInterface& image) override;


            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::u8vec4>& colors,
                            int startIndex, int endIndex) override;

    };
}

}

#endif