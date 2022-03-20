#ifndef SSGUI_BACKEND_DRAWING_SFML
#define SSGUI_BACKEND_DRAWING_SFML

#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"
#include "ssGUI/Backend/BackendManager.hpp"
#include "ssGUI/Backend/SFML/BackendImageSFML.hpp"
#include "ssGUI/Backend/SFML/BackendFontSFML.hpp"
#include "SFML/Graphics.hpp"
#include <functional>

//namespace: ssGUI.Backend
namespace ssGUI::Backend
{
    /*class: BackendDrawingSFML
    For functions explainations, please see <BackendDrawingInterface>. Normally you don't need to deal with this class
    
    Variables & Constructor:
    ======================== C++ =======================
    private:
        int BackendIndex;
    ====================================================
    ======================== C++ =======================
    BackendDrawingSFML::BackendDrawingSFML() : BackendIndex(0)
    {
        ssGUI::Backend::BackendManager::AddDrawingInterface(static_cast<ssGUI::Backend::BackendDrawingInterface*>(this));
    }
    ====================================================
    */
    class BackendDrawingSFML : public BackendDrawingInterface
    {        
        private:
            int BackendIndex; 

            //Non index variants exist only for legacy purposes
            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colours,
                            const ssGUI::Backend::BackendFontInterface& font,
                            int characterSize) override;

            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colours,
                            const ssGUI::Backend::BackendImageInterface& image) override;

            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::u8vec4>& colours) override;

            //NOTE: End index is exclusive
            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colours,
                            int startIndex, int endIndex,
                            const ssGUI::Backend::BackendFontInterface& font,
                            int CharacterSize) override;

            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colours,
                            int startIndex, int endIndex,
                            const ssGUI::Backend::BackendImageInterface& image) override;


            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::u8vec4>& colours,
                            int startIndex, int endIndex) override;

        protected:
            

        public:
            BackendDrawingSFML();
            virtual ~BackendDrawingSFML();

            //function: DrawEntities
            //See <BackendDrawingInterface::DrawEntities>
            bool DrawEntities(  const std::vector<glm::vec2>& vertices, 
                                const std::vector<glm::vec2>& texCoords,
                                const std::vector<glm::u8vec4>& colours,
                                const std::vector<int>& counts,
                                const std::vector<ssGUI::DrawingProperty>& properties) override;

            //function: Render
            //See <BackendDrawingInterface::Render>
            virtual void Render(glm::u8vec3 clearColor) override;

            //function: ClearBackBuffer
            //See <BackendDrawingInterface::ClearBackBuffer>
            virtual void ClearBackBuffer(glm::u8vec3 clearColor) override;

    };
}


#endif