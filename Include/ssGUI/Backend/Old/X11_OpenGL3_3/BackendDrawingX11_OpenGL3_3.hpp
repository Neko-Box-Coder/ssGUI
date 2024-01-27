#ifndef SSGUI_BACKEND_DRAWING_X11_OPEN_GL_3_3_H
#define SSGUI_BACKEND_DRAWING_X11_OPEN_GL_3_3_H

#include "ssGUI/Backend/Interfaces/DrawingInterface.hpp"

#include "glad/glad_glx.h"
#include "glad/glad.h"
#include "ssGUI/Backend/OpenGL3_3_Common/OpenGL3_3_Common.hpp"

#include <unordered_map>

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    class MainWindowInterface;

    /*class: ssGUI::Backend::BackendDrawingX11_OpenGL3_3
    For functions explainations, please see <DrawingInterface>. Normally you don't need to deal with this class
    
    Variables & Constructor:
    ======================== C++ =======================
    protected:
        int BackendIndex;                                                                       //(Internal variable) This is used to check if we are drawing on the correct MainWindow
        glm::ivec2 LastMainWindowSize;                                                          //(Internal variable) This is used to check if mainWindow size has changed to update viewport
        std::unordered_map<CharTextureIdentifier, GLuint> CharTextures;                         //(Internal variable) This is used to keep track of all the character textures 
        std::unordered_map<ssGUI::Backend::ImageInterface*, GLuint> ImageTextures;       //(Internal variable) This is used to keep track of all the image textures 
        ssGUI::Backend::OpenGL3_3_Common* OpenGLCommon;                                         //(Internal variable) This is used to invoke common OpenGL and mipmap functions
    ====================================================
    ======================== C++ =======================
    BackendDrawingX11_OpenGL3_3::BackendDrawingX11_OpenGL3_3() :    BackendIndex(0),
                                                                    LastMainWindowSize(-1, -1),
                                                                    CharTextures(),
                                                                    ImageTextures(),
                                                                    OpenGLCommon(nullptr)
    {
        ssGUI::Backend::BackendManager::AddDrawingInterface(static_cast<ssGUI::Backend::DrawingInterface*>(this));   
    }
    ====================================================
    */
    class BackendDrawingX11_OpenGL3_3 : public DrawingInterface
    {
        private:
            BackendDrawingX11_OpenGL3_3& operator=(BackendDrawingX11_OpenGL3_3 const& other);

        protected:
            int BackendIndex;                                                                       //(Internal variable) This is used to check if we are drawing on the correct MainWindow
            glm::ivec2 LastMainWindowSize;                                                          //(Internal variable) This is used to check if mainWindow size has changed to update viewport
            std::unordered_map<CharTextureIdentifier, GLuint> CharTextures;                         //(Internal variable) This is used to keep track of all the character textures 
            std::unordered_map<ssGUI::Backend::ImageInterface*, GLuint> ImageTextures;       //(Internal variable) This is used to keep track of all the image textures 
            ssGUI::Backend::OpenGL3_3_Common* OpenGLCommon;                                         //(Internal variable) This is used to invoke common OpenGL and mipmap functions
        
            BackendDrawingX11_OpenGL3_3(BackendDrawingX11_OpenGL3_3 const& other);
        
            ssGUI::Backend::MainWindowInterface* GetMainWindow();
            void InitializeOpenGLCommonIfNeeded();
        
        public:
            BackendDrawingX11_OpenGL3_3();
            ~BackendDrawingX11_OpenGL3_3() override;

            //function: SaveState
            //See <DrawingInterface::SaveState>
            void SaveDrawingState() override;

            //function: RestoreDrawingState
            //See <DrawingInterface::RestoreDrawingState>
            void RestoreDrawingState() override;

            //function: DrawEntities
            //See <DrawingInterface::DrawEntities>
            bool CreateDrawingEntities(const std::vector<ssGUI::DrawingEntity>& entities) override;
            
            //function: DrawToBackBuffer
            //See <DrawingInterface::DrawToBackBuffer>
            void DrawToBackBuffer() override;

            //function: Render
            //See <DrawingInterface::Render>
            void Render(glm::u8vec3 clearColor) override;

            //function: ClearBackBuffer
            //See <DrawingInterface::ClearBackBuffer>
            void ClearBackBuffer(glm::u8vec3 clearColor) override;

            //function: AddImageCache
            //See <DrawingInterface::AddImageCache>
            void AddImageCache(ssGUI::Backend::ImageInterface* backendImage) override;
            
            //function: RemoveImageCache
            //See <DrawingInterface::RemoveImageCache>
            void RemoveImageCache(ssGUI::Backend::ImageInterface* backendImage) override;
            
            //function: GetRawImageCacheHandle
            //See <DrawingInterface::GetRawImageCacheHandle>
            void* GetRawImageCacheHandle(ssGUI::Backend::ImageInterface* backendImage) override;
    };
}

}

#endif