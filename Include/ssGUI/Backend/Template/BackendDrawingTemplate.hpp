#ifndef SSGUI_BACKEND_DRAWING_TEMPLATE
#define SSGUI_BACKEND_DRAWING_TEMPLATE

#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendDrawingTemplate
    class BackendDrawingTemplate : public BackendDrawingInterface
    {
        private:
            BackendDrawingTemplate& operator=(BackendDrawingTemplate const& other);

        protected:
            BackendDrawingTemplate(BackendDrawingTemplate const& other);
        
        public:
            BackendDrawingTemplate();
            ~BackendDrawingTemplate() override;

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