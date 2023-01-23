#ifndef SSGUI_BACKEND_DRAWING_X11_OPEN_GL_3_3_H
#define SSGUI_BACKEND_DRAWING_X11_OPEN_GL_3_3_H

#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"

#include "glad/glad_glx.h"
#include "glad/glad.h"

#include <unordered_map>

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    class BackendMainWindowInterface;

    /*class: ssGUI::Backend::BackendDrawingX11_OpenGL3_3
    For functions explainations, please see <BackendDrawingInterface>. Normally you don't need to deal with this class
    
    Variables & Constructor:
    ======================== C++ =======================
    protected:
        int BackendIndex;                                                                       //(Internal variable) This is used to check if we are drawing on the correct MainWindow
        glm::ivec2 LastMainWindowSize;                                                          //(Internal variable) This is used to check if mainWindow size has changed to update viewport
        std::unordered_map<uint32_t, GLuint> CharTextures;                                      //(Internal variable) This is used to keep track of all the character textures 
        std::unordered_map<ssGUI::Backend::BackendImageInterface*, GLuint> ImageTextures;       //(Internal variable) This is used to keep track of all the image textures 
    ====================================================
    ======================== C++ =======================
    BackendDrawingX11_OpenGL3_3::BackendDrawingX11_OpenGL3_3() :    BackendIndex(0),
                                                                    LastMainWindowSize(-1, -1),
                                                                    CharTextures(),
                                                                    ImageTextures()
    {
        ssGUI::Backend::BackendManager::AddDrawingInterface(static_cast<ssGUI::Backend::BackendDrawingInterface*>(this));   
    }
    ====================================================
    */
    class BackendDrawingX11_OpenGL3_3 : public BackendDrawingInterface
    {
        private:
            BackendDrawingX11_OpenGL3_3& operator=(BackendDrawingX11_OpenGL3_3 const& other);

        protected:
            int BackendIndex;                                                                       //(Internal variable) This is used to check if we are drawing on the correct MainWindow
            glm::ivec2 LastMainWindowSize;                                                          //(Internal variable) This is used to check if mainWindow size has changed to update viewport
            std::unordered_map<uint32_t, GLuint> CharTextures;                                      //(Internal variable) This is used to keep track of all the character textures 
            std::unordered_map<ssGUI::Backend::BackendImageInterface*, GLuint> ImageTextures;       //(Internal variable) This is used to keep track of all the image textures 

        
            BackendDrawingX11_OpenGL3_3(BackendDrawingX11_OpenGL3_3 const& other);
        
            ssGUI::Backend::BackendMainWindowInterface* GetMainWindow();
            void UpdateViewPortAndModelViewIfNeeded();
        
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