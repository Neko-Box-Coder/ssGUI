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

//From: https://stackoverflow.com/questions/20834838/using-tuple-in-unordered-map
// function has to live in the std namespace 
// so that it is picked up by argument-dependent name lookup (ADL).
namespace std{
    namespace
    {

        // Code from boost
        // Reciprocal of the golden ratio helps spread entropy
        //     and handles duplicates.
        // See Mike Seymour in magic-numbers-in-boosthash-combine:
        //     https://stackoverflow.com/questions/4948780

        template <class T>
        inline void hash_combine(std::size_t& seed, T const& v)
        {
            seed ^= hash<T>()(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }

        // Recursive template code derived from Matthieu M.
        template <class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
        struct HashValueImpl
        {
          static void apply(size_t& seed, Tuple const& tuple)
          {
            HashValueImpl<Tuple, Index-1>::apply(seed, tuple);
            hash_combine(seed, get<Index>(tuple));
          }
        };

        template <class Tuple>
        struct HashValueImpl<Tuple,0>
        {
          static void apply(size_t& seed, Tuple const& tuple)
          {
            hash_combine(seed, get<0>(tuple));
          }
        };
    }

    template <typename ... TT>
    struct hash<std::tuple<TT...>> 
    {
        size_t
        operator()(std::tuple<TT...> const& tt) const
        {                                              
            size_t seed = 0;                             
            HashValueImpl<std::tuple<TT...> >::apply(seed, tt);    
            return seed;                                 
        }                                              

    };
}

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

            BackendDrawingSFML& operator=(BackendDrawingSFML const& other);

        protected:
            BackendDrawingSFML(BackendDrawingSFML const& other);

        public:
            BackendDrawingSFML();
            ~BackendDrawingSFML() override;

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
            //Non index variants exist only for legacy purposes
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