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
        
        public:
            BackendDrawingTemplate();
            ~BackendDrawingTemplate() override;

            //function: SaveState
            //Saves the OpenGL state
            void SaveState() override;

            //function: RestoreState
            //Restores the OpenGL state
            void RestoreState() override;

            //function: DrawEntities
            //Draws the entity based on what is set in the _properties_. Returns true if drawn successfully. *Note that if you are not using <ssGUIManager>, you need to call <Render> at the end in order to render it*.
            bool DrawEntities(  const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::vec2>& texCoords,
                                        const std::vector<glm::u8vec4>& colors,
                                        const std::vector<int>& counts,
                                        const std::vector<ssGUI::DrawingProperty>& properties) override;

            /*function: Render
            Renders every entity that are drawn to the <MainWindow>. This will automatically clear the back buffer. 
            If you are using <ssGUIManager>, this will be automatically called.*/
            void Render(glm::u8vec3 clearColor) override;

            /*function: ClearBackBuffer
            Clears the back buffer manually. If you are using <ssGUIManager>, this will be automatically called for caching.*/
            void ClearBackBuffer(glm::u8vec3 clearColor) override;
            
        protected:
            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                                    const std::vector<glm::vec2>& texCoords,
                                    const std::vector<glm::u8vec4>& colors,
                                    const ssGUI::Backend::BackendFontInterface& font,
                                    int CharacterSize) override;

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
                                    int startIndex, int endIndex,
                                    const ssGUI::Backend::BackendFontInterface& font,
                                    int CharacterSize) override;

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