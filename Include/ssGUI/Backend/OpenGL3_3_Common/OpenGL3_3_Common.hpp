#ifndef SSGUI_OPENGL_3_3_COMMON_HPP
#define SSGUI_OPENGL_3_3_COMMON_HPP

#include "ssGUI/Backend/DynamicImageAtlas.hpp"

#include "glad/glad.h"
#include <map>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

//#define SSGUI_DEBUG_OPENGL_MIPMAP
//#define SSGUI_DEBUG_DUMP_MIPMAP


//From: https://stackoverflow.com/questions/20834838/using-tuple-in-unordered-map
// function has to live in the std namespace 
// so that it is picked up by argument-dependent name lookup (ADL).
namespace std
{
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

struct DrawingEntity;

namespace Backend
{
    class BackendMainWindowInterface;
    class BackendFontInterface;
    class BackendImageInterface;
    class BackendDrawingInterface;
    
    class OpenGL3_3_Common
    {
        public:
            using CharSize = uint16_t;
            using CharCode = uint32_t;
            using CharTextureIdentifier = std::tuple<ssGUI::Backend::BackendFontInterface*, CharSize, CharCode>;
    
        private:
            static const std::string VertShader;
            static const std::string FragShader;
            GLuint ProgramId;
            GLuint CachedImages;
            GLuint VAO;
            GLuint VertsVBO;
            GLuint ColorsVBO;
            GLuint TexCoordsVBO;
            GLuint TopLeftTexCoordsVBO;
            GLuint BotRightTexCoordsVBO;
            GLint LastViewportProperties[4];
            
            std::vector<glm::vec2> Vertices;
            std::vector<glm::vec4> Colors;
            std::vector<glm::vec3> TexCoords;
            std::vector<glm::vec3> TextureTopLeftCoords;
            std::vector<glm::vec3> TextureBotRightCoords;
            std::vector<GLuint> Idx;

            BackendMainWindowInterface* CurrentMainWindow;
            
            glm::ivec2 LastMainWindowSize;
            
            DynamicImageAtlas* CurrentImageAtlas;
            std::unordered_map<ssGUI::Backend::BackendImageInterface*, int> MappedImgIds;
            std::unordered_map<CharTextureIdentifier, int> MappedFontIds;
            
            const int VERT_POS_INDEX;
            const int VERT_COLOR_INDEX;
            const int VERT_TEX_COORD_INDEX;
            const int VERT_TEX_TOP_LEFT_INDEX;
            const int VERT_TEX_BOT_RIGHT_INDEX;

            //TODO: Method to generate mipmap

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
            
            
            bool OnNewAtlasRequest();
            
            void SaveLastViewport();
            
            void LoadLastViewport();
            
            template<typename T>
            bool AddDrawingCache(std::unordered_map<T, int>& cachedIds, T key, glm::ivec2 textureSize, const void* rgba32Pixels);
    
            template<typename T>
            bool RemoveDrawingCache(std::unordered_map<T, int>& cachedIds, T key);
    
        public:        
            OpenGL3_3_Common(BackendMainWindowInterface* mainWindow);
            
            ~OpenGL3_3_Common();
        
            glm::mat4x4 UpdateViewPortAndModelView(glm::ivec2 widthHeight);
            void SaveState();
            void RestoreState();
            bool DrawEntities(const std::vector<ssGUI::DrawingEntity>& entities);
            bool AddFontCache(CharTextureIdentifier charTexture);
            bool AddImageCache(ssGUI::Backend::BackendImageInterface* backendImage);
            void RemoveFontCache(CharTextureIdentifier charTexture);
            void RemoveImageCache(ssGUI::Backend::BackendImageInterface* backendImage);
            
            void DrawToBackBuffer();
            //void ClearBackBuffer(glm::u8vec3 clearColor);
            
            //TODO: May not be able to implement
            void* GetRawImageCacheHandle(ssGUI::Backend::BackendImageInterface* backendImage);
    };

}

}

#endif