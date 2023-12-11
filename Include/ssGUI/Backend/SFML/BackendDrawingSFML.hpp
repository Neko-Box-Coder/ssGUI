#ifndef SSGUI_BACKEND_DRAWING_SFML_H
#define SSGUI_BACKEND_DRAWING_SFML_H

#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"
#include "ssGUI/Backend/BackendManager.hpp"

#ifdef SSGUI_FONT_BACKEND_SFML
#include "ssGUI/Backend/SFML/BackendFontSFML.hpp"
#endif

#include "SFML/Graphics.hpp"

#include <functional>
#include <cmath>
#include <unordered_map>
#include <queue>
#include <utility>

namespace ssGUI
{ 
    
//namespace: ssGUI.Backend
namespace Backend
{
    /*class: ssGUI::Backend::BackendDrawingSFML
    For functions explainations, please see <BackendDrawingInterface>. Normally you don't need to deal with this class
    
    Variables & Constructor:
    ======================== C++ =======================
    private:
        int BackendIndex;                                                                       //(Internal variable) This is used to check if we are drawing on the correct MainWindow

        using CharSize = uint16_t;
        using CharCode = uint32_t;
        std::queue<std::pair<CharSize, CharCode>> CharTexturesQueue;                            //(Internal variable) This is used to keep track of all the character textures    

        #ifndef SSGUI_FONT_BACKEND_SFML
        using CharTextureIdentifier =                                                           //(Internal variable) This is the key for getting the character texture
            std::tuple<ssGUI::Backend::BackendFontInterface*, CharSize, CharCode>; 
        
        std::unordered_map<CharTextureIdentifier, sf::Texture> CharTextures;                    //(Internal variable) Hashmap for getting character textures
        #endif
        
        #ifndef SSGUI_IMAGE_BACKEND_SFML
        std::unordered_map<ssGUI::Backend::BackendImageInterface*, sf::Texture> ImageTextures;  //See <RemoveImageLinking>
        #endif
        
        void* endVar = nullptr;
    ====================================================
    ======================== C++ =======================
    BackendDrawingSFML::BackendDrawingSFML() :  BackendIndex(0),
                                                CharTexturesQueue(),
                                                #ifndef SSGUI_FONT_BACKEND_SFML
                                                    CharTextures(),
                                                #endif
                                                #ifndef SSGUI_IMAGE_BACKEND_SFML
                                                    ImageTextures(),
                                                #endif
                                                endVar(nullptr)
    {
        ssGUI::Backend::BackendManager::AddDrawingInterface(static_cast<ssGUI::Backend::BackendDrawingInterface*>(this));
    }
    ====================================================
    */
    class BackendDrawingSFML : public BackendDrawingInterface
    {        
        private:
            int BackendIndex;                                                                       //(Internal variable) This is used to check if we are drawing on the correct MainWindow

            #ifndef SSGUI_FONT_BACKEND_SFML
            std::unordered_map<CharTextureIdentifier, sf::Texture> CharTextures;                    //(Internal variable) Hashmap for getting character textures
            #endif
            
            #ifndef SSGUI_IMAGE_BACKEND_SFML
            std::unordered_map<ssGUI::Backend::BackendImageInterface*, sf::Texture> ImageTextures;  //See <RemoveImageLinking>
            #endif
            
            void* endVar = nullptr;

            BackendDrawingSFML& operator=(BackendDrawingSFML const& other);

        protected:
            BackendDrawingSFML(BackendDrawingSFML const& other);

        public:
            BackendDrawingSFML();
            ~BackendDrawingSFML() override;

            //function: SaveState
            //See <BackendDrawingInterface::SaveState>
            void SaveDrawingState() override;

            //function: RestoreDrawingState
            //See <BackendDrawingInterface::RestoreDrawingState>
            void RestoreDrawingState() override;

            //function: DrawEntities
            //See <BackendDrawingInterface::DrawEntities>
            bool DrawEntities(const std::vector<ssGUI::DrawingEntity>& entities) override;

            //function: DrawToBackBuffer
            //See <BackendDrawingInterface::DrawToBackBuffer>
            void DrawToBackBuffer() override;
            
            //function: Render
            //See <BackendDrawingInterface::Render>
            void Render(glm::u8vec3 clearColor) override;

            //function: ClearBackBuffer
            //See <BackendDrawingInterface::ClearBackBuffer>
            void ClearBackBuffer(glm::u8vec3 clearColor) override;

            //function: AddImageCache
            //See <BackendDrawingInterface::AddImageCache>
            void AddImageCache(ssGUI::Backend::BackendImageInterface* backendImage) override;
            
            //function: RemoveImageCache
            //See <BackendDrawingInterface::RemoveImageCache>
            void RemoveImageCache(ssGUI::Backend::BackendImageInterface* backendImage) override;
            
            //function: GetRawImageCacheHandle
            //See <BackendDrawingInterface::GetRawImageCacheHandle>
            void* GetRawImageCacheHandle(ssGUI::Backend::BackendImageInterface* backendImage) override;

        protected:
            //Non index variants exist only for legacy purposes
            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            const uint32_t character,
                            const ssGUI::Backend::BackendFontInterface& font,
                            int characterSize);

            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            const ssGUI::Backend::BackendImageInterface& image);

            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::u8vec4>& colors);
    };
}

}


#endif