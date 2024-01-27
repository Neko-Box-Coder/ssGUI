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
    class MainWindowInterface;
    class FontInterface;
    class ImageInterface;
    class DrawingInterface;
    
    /*class: ssGUI::Backend::OpenGL3_3_Common
    This is a common class for OpenGL backend on Linux and Windows. It uses the same shader for rendering everything.
    
    Normally, you don't have to deal with this class.
    
    Variables & Constructor:
    ============================== C++ ==============================
    public:
        using CharSize = uint16_t;
        using CharCode = uint32_t;
        using CharTextureIdentifier = std::tuple<ssGUI::Backend::FontInterface*, CharSize, CharCode>;

    private:
        static const std::string VertShader;                                                //(Internal variable) Vertex shader string
        static const std::string FragShader;                                                //(Internal variable) Fragment shader string
        GLuint ProgramId;                                                                   //(Internal variable) Program Id for drawing GUI
        GLuint CachedImages;                                                                //(Internal variable) 2D texture array for storing the image atlas layers
        GLuint VAO;                                                                         //(Internal variable) VAO for drawing GUI
        GLuint VertsVBO;                                                                    //(Internal variable) VBO for storing vertices for drawing GUI
        GLuint ColorsVBO;                                                                   //(Internal variable) VBO for storing colors for drawing GUI
        GLuint TexCoordsVBO;                                                                //(Internal variable) VBO for storing texture coordinates (0 to image atlas size) for drawing GUI. If non texture, this is not used.
        GLuint TopLeftTexCoordsVBO;                                                         //(Internal variable) VBO for storing top left position texture coordinates (0 to image atlas size) for drawing GUI. 
                                                                                            //                      If non texture, this will be negative to signal this shape is non texture.
        GLuint BotRightTexCoordsVBO;                                                        //(Internal variable) VBO for storing bottom right position texture coordinates (0 to image atlas size) for drawing GUI. 
        GLint LastViewportProperties[4];                                                    //(Internal variable) Used for recording the last viewport when drawing GUI
        
        std::vector<glm::vec2> Vertices;                                                    //(Internal variable) Internal buffer for storing shape vertices
        std::vector<glm::vec4> Colors;                                                      //(Internal variable) Internal buffer for storing color 
        std::vector<glm::vec3> TexCoords;                                                   //(Internal variable) Internal buffer for storing texture coordinates
        std::vector<glm::vec3> TextureTopLeftCoords;                                        //(Internal variable) Internal buffer for storing top left corner for texture in image atlas
        std::vector<glm::vec3> TextureBotRightCoords;                                       //(Internal variable) Internal buffer for storing bottom right corner for texture in image atlas
        std::vector<GLuint> Idx;                                                            //(Internal variable) Internal buffer for storing indices for drawing GUI shapes.

        MainWindowInterface* CurrentMainWindow;                                      //(Internal variable) Used for performing viewport related operations
        
        DynamicImageAtlas* CurrentImageAtlas;                                               //(Internal variable) <DynamicImageAtlas> object for managing image atlas
        std::unordered_map<ssGUI::Backend::ImageInterface*, int> MappedImgIds;       //See <AddImageCache>
        std::unordered_map<CharTextureIdentifier, int> MappedFontIds;                       //See <AddFontCache>
        
        const int VERT_POS_INDEX;                                                           //(Internal variable) Shader layout location for vertex position
        const int VERT_COLOR_INDEX;                                                         //(Internal variable) Shader layout location for color
        const int VERT_TEX_COORD_INDEX;                                                     //(Internal variable) Shader layout location for texture coordinates
        const int VERT_TEX_TOP_LEFT_INDEX;                                                  //(Internal variable) Shader layout location for top left texture coordinates
        const int VERT_TEX_BOT_RIGHT_INDEX;                                                 //(Internal variable) Shader layout location for bot right texture coordinates
        static const int MAX_TEXTURE_LAYER_SIZE;                                            //(Internal variable) Size in pixel for width and height for each layer in image atlas
    =================================================================
    ============================== C++ ==============================
    OpenGL3_3_Common::OpenGL3_3_Common( MainWindowInterface* mainWindow) :   ProgramId(0),
                                                                                    CachedImages(0),
                                                                                    VAO(0),
                                                                                    VertsVBO(0),
                                                                                    ColorsVBO(0),
                                                                                    TexCoordsVBO(0),
                                                                                    TopLeftTexCoordsVBO(0),
                                                                                    BotRightTexCoordsVBO(0),
                                                                                    LastViewportProperties(),
                                                                                    Vertices(),
                                                                                    Colors(),
                                                                                    TexCoords(),
                                                                                    TextureTopLeftCoords(),
                                                                                    TextureBotRightCoords(),
                                                                                    Idx(),
                                                                                    CurrentMainWindow(nullptr),
                                                                                    CurrentImageAtlas(nullptr),
                                                                                    MappedImgIds(),
                                                                                    MappedFontIds(),
                                                                                    VERT_POS_INDEX(0),
                                                                                    VERT_COLOR_INDEX(1),
                                                                                    VERT_TEX_COORD_INDEX(2),
                                                                                    VERT_TEX_TOP_LEFT_INDEX(3),
                                                                                    VERT_TEX_BOT_RIGHT_INDEX(4)
    {
        CurrentMainWindow = mainWindow;

        mainWindow->SetGLContext();
    
        GLint maxTextureSize = 0;
        //GLint maxLayerSize = 0;
        
        GL_CHECK_ERROR( glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize) );
        //GL_CHECK_ERROR( glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxLayerSize) );
        
        //ssLOG_LINE("maxTextureSize: "<<maxTextureSize);
        //ssLOG_LINE("maxLayerSize: "<<maxLayerSize);
        
        //maxTextureSize = 1920;
        maxTextureSize = maxTextureSize > MAX_TEXTURE_LAYER_SIZE ? MAX_TEXTURE_LAYER_SIZE : maxTextureSize;
        
        GLint success = GL_FALSE;
        char infoLog[512] { 0 };
        
        // vertex Shader
        GLuint vertexShaderId = 0;
        GL_CHECK_ERROR( vertexShaderId = glCreateShader(GL_VERTEX_SHADER) );
        const char* vertShaderP = VertShader.c_str();
        GL_CHECK_ERROR( glShaderSource(vertexShaderId, 1, &vertShaderP, NULL) );
        GL_CHECK_ERROR( glCompileShader(vertexShaderId) );
        
        {
            GL_CHECK_ERROR( glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success) );
            if(success != GL_TRUE)
            {
                GL_CHECK_ERROR( glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog) );
                ssGUI_ERROR(ssGUI_TAG_BACKEND, "Failed to compile shaders:");
                ssGUI_ERROR(ssGUI_TAG_BACKEND, infoLog);
                ssLOG_EXIT_PROGRAM(1);
            };
        }
        
        // fragment Shader
        GLuint fragmentShaderId = 0;
        GL_CHECK_ERROR( fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER) );
        const char* fargShaderP = FragShader.c_str();
        GL_CHECK_ERROR( glShaderSource(fragmentShaderId, 1, &fargShaderP, NULL) );
        GL_CHECK_ERROR( glCompileShader(fragmentShaderId) );

        {
            GL_CHECK_ERROR( glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success) );
            if(success != GL_TRUE)
            {
                GL_CHECK_ERROR( glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog) );
                ssGUI_ERROR(ssGUI_TAG_BACKEND, "Failed to compile shaders:");
                ssGUI_ERROR(ssGUI_TAG_BACKEND, infoLog);
                ssLOG_EXIT_PROGRAM(1);
            };
        }

        // shader Program
        GL_CHECK_ERROR( ProgramId = glCreateProgram() );
        
        GL_CHECK_ERROR( glAttachShader(ProgramId, vertexShaderId) );
        GL_CHECK_ERROR( glAttachShader(ProgramId, fragmentShaderId) );
        GL_CHECK_ERROR( glLinkProgram(ProgramId) );
        
        // print linking errors if any
        glGetProgramiv(ProgramId, GL_LINK_STATUS, &success);
        {
            GL_CHECK_ERROR( glGetProgramiv(ProgramId, GL_LINK_STATUS, &success) );
            if(success != GL_TRUE)
            {
                GL_CHECK_ERROR( glGetProgramInfoLog(ProgramId, 512, NULL, infoLog) );
                ssGUI_ERROR(ssGUI_TAG_BACKEND, "Failed to compile shaders:");
                ssGUI_ERROR(ssGUI_TAG_BACKEND, infoLog);
                ssLOG_EXIT_PROGRAM(1);
            };
        }
        
        GL_CHECK_ERROR( glDetachShader(ProgramId, vertexShaderId) );
        GL_CHECK_ERROR( glDetachShader(ProgramId, fragmentShaderId) );
        
        // delete the shaders as they're linked into our program now and no longer necessary
        GL_CHECK_ERROR( glDeleteShader(vertexShaderId) );
        GL_CHECK_ERROR( glDeleteShader(fragmentShaderId) );
        
        //Generate ID for VBOs for vertex pos, colors, UVs and UseUVs flag
        GL_CHECK_ERROR( glGenBuffers(1, &VertsVBO) );
        GL_CHECK_ERROR( glGenBuffers(1, &ColorsVBO) );
        GL_CHECK_ERROR( glGenBuffers(1, &TexCoordsVBO) );
        GL_CHECK_ERROR( glGenBuffers(1, &TopLeftTexCoordsVBO) );
        GL_CHECK_ERROR( glGenBuffers(1, &BotRightTexCoordsVBO) );
        
        //Generate ID for VAO and bind VBOs and EBO created above to this VAO
        GL_CHECK_ERROR( glGenVertexArrays(1, &VAO) );
        
        GL_CHECK_ERROR( glBindAttribLocation(ProgramId, VERT_POS_INDEX, "vertPos") );
        GL_CHECK_ERROR( glBindVertexArray(VAO) );
        GL_CHECK_ERROR( glBindBuffer(GL_ARRAY_BUFFER, VertsVBO) );
        GL_CHECK_ERROR( glVertexAttribPointer(VERT_POS_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0) );
        GL_CHECK_ERROR( glEnableVertexAttribArray(VERT_POS_INDEX) );
        
        GL_CHECK_ERROR( glBindAttribLocation(ProgramId, VERT_COLOR_INDEX, "vertColor") );
        GL_CHECK_ERROR( glBindBuffer(GL_ARRAY_BUFFER, ColorsVBO) );
        GL_CHECK_ERROR( glVertexAttribPointer(VERT_COLOR_INDEX, 4, GL_FLOAT, GL_FALSE, 0, 0) );
        GL_CHECK_ERROR( glEnableVertexAttribArray(VERT_COLOR_INDEX) );
        
        GL_CHECK_ERROR( glBindAttribLocation(ProgramId, VERT_TEX_COORD_INDEX, "vertTexCoord") );
        GL_CHECK_ERROR( glBindBuffer(GL_ARRAY_BUFFER, TexCoordsVBO) );
        GL_CHECK_ERROR( glVertexAttribPointer(VERT_TEX_COORD_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0) );
        GL_CHECK_ERROR( glEnableVertexAttribArray(VERT_TEX_COORD_INDEX) );

        GL_CHECK_ERROR( glBindAttribLocation(ProgramId, VERT_TEX_TOP_LEFT_INDEX, "vertTexTopLeft") );
        GL_CHECK_ERROR( glBindBuffer(GL_ARRAY_BUFFER, TopLeftTexCoordsVBO) );
        GL_CHECK_ERROR( glVertexAttribPointer(VERT_TEX_TOP_LEFT_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0) );
        GL_CHECK_ERROR( glEnableVertexAttribArray(VERT_TEX_TOP_LEFT_INDEX) );

        GL_CHECK_ERROR( glBindAttribLocation(ProgramId, VERT_TEX_BOT_RIGHT_INDEX, "vertTexBotRight") );
        GL_CHECK_ERROR( glBindBuffer(GL_ARRAY_BUFFER, BotRightTexCoordsVBO) );
        GL_CHECK_ERROR( glVertexAttribPointer(VERT_TEX_BOT_RIGHT_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0) );
        GL_CHECK_ERROR( glEnableVertexAttribArray(VERT_TEX_BOT_RIGHT_INDEX) );

        GL_CHECK_ERROR( glBindVertexArray(0) );
        GL_CHECK_ERROR( glBindBuffer(GL_ARRAY_BUFFER, 0) );

        CurrentImageAtlas = new ssGUI::Backend::DynamicImageAtlas(  glm::ivec2(maxTextureSize, maxTextureSize), 
                                                                    glm::ivec2(64, 64), 
                                                                    std::bind(&OpenGL3_3_Common::OnNewAtlasRequest, this));
    }
    
    const std::string OpenGL3_3_Common::VertShader = 
    R"(
        #version 330 core
        
        in vec2 vertPos;
        in vec4 vertColor;
        in vec3 vertTexCoord;
        in vec3 vertTexTopLeft;
        in vec3 vertTexBotRight;

        out vec4 fragColor;
        out vec3 fragTexCoord;
        flat out vec3 fragTexTopLeft;
        flat out vec3 fragTexBotRight;

        uniform mat4 projMatrix;

        void main()
        {
            gl_Position = projMatrix * vec4(vertPos, 0.0, 1.0f);
            
            fragColor = vertColor;
            fragTexCoord = vertTexCoord;
            fragTexTopLeft = vertTexTopLeft;
            fragTexBotRight = vertTexBotRight;
        }
    )";
    
    const std::string OpenGL3_3_Common::FragShader = 
    R"(
        #version 330 core
        
        #extension GL_ARB_texture_query_lod : enable

        out vec4 outColor;
        
        in vec4 fragColor;
        in vec3 fragTexCoord;
        flat in vec3 fragTexTopLeft;
        flat in vec3 fragTexBotRight;

        uniform sampler2DArray ourTexture;

        //From: https://stackoverflow.com/questions/24388346/how-to-access-automatic-mipmap-level-in-glsl-fragment-shader-texture
        // Does not take into account GL_TEXTURE_MIN_LOD/GL_TEXTURE_MAX_LOD/GL_TEXTURE_LOD_BIAS,
        // nor implementation-specific flexibility allowed by OpenGL spec
        float mip_map_level(in vec2 texture_coordinate) // in texel units
        {
            vec2  dx_vtc        = dFdx(texture_coordinate);
            vec2  dy_vtc        = dFdy(texture_coordinate);
            float delta_max_sqr = max(dot(dx_vtc, dx_vtc), dot(dy_vtc, dy_vtc));
            float mml = 0.5 * log2(delta_max_sqr);
            return max( 0, mml ); // Thanks @Nims
        }

        vec2 BoundTexCoord(vec2 texCoord, ivec2 texPos, ivec2 texSize)
        {
            return clamp(texCoord, texPos, texPos + texSize - 1);
        }

        void main()
        {
            //Normalize the color
            vec4 fragColorN = fragColor / 255.f;
            
            //If drawing shape, just output the color
            if(fragTexTopLeft.x < 0)
            {
                outColor = fragColorN;
                return;
            }
            
            vec3 boundedTexCoord = fragTexCoord;
            vec2 imageSize = (fragTexBotRight - fragTexTopLeft).xy;
            boundedTexCoord.xy = BoundTexCoord(fragTexCoord.xy, ivec2(fragTexTopLeft.xy), ivec2(imageSize));
            float mipmapLevel = 0;
            
            #ifdef GL_ARB_texture_query_lod
                mipmapLevel = textureQueryLOD(ourTexture, fragTexCoord.xy / textureSize(ourTexture, 0).xy).y;
            #else
                mipmapLevel = mip_map_level(boundedTexCoord.xy);
            #endif
            
            int lowerDim = min(textureSize(ourTexture, 0).x, textureSize(ourTexture, 0).y);
            const int MAX_LOOP = 99;
            int counter = 1;
            for(int i = 0; i < MAX_LOOP; i++)
            {
                lowerDim = lowerDim >> 1;
                counter++;
                
                if(lowerDim == 0)
                    break;
            }
            
            int roundedMipmapLevel = int(clamp(mipmapLevel, 0, counter));
            //roundedMipmapLevel = 3;
            
            //If mipmap level is 0 (base), don't need to do anything
            if(roundedMipmapLevel == 0)
            {
                outColor = texelFetch(ourTexture, ivec3(boundedTexCoord), 0) * (fragColorN);
                return;
            }
            
            //Otherwise get the localized TexCoord
            vec2 localTexCoord = (boundedTexCoord.xy - fragTexTopLeft.xy);
            vec2 localTexCoordNormalized = localTexCoord / imageSize;
            
            //NOTE: This in theory is pretty good, but doesn't work because mipmap size is rounded down at each level 
            //          and each one is relying on the previous one.
            //Get the mipmap multiplier by doing 0.5 ^ mipmap level
            //float mipmapMultiplier = pow(0.5f, float(roundedMipmapLevel));
            
            //NOTE: Again, this works on paper but because mipmap is rounded down each time, the offset will be wrong.
            //The coordinate of the mipmap in y can be found by using the nth partial sum of a geometric sequence
            //  with this (modified) formula: (1 - 0.5 ^ mipmapLevel) / (1 - 0.5) - 1
            //float mipmapYOffsetMultiplier = (1.f - mipmapMultiplier) / 0.5f - 1.f;
            
            //Get the mipmap size and Y offset
            ivec2 mipmapSize = ivec2(imageSize);
            int mipmapYOffset = -mipmapSize.y; 
            
            for(int i = 0; i < MAX_LOOP; i++)
            {
                if(i == roundedMipmapLevel)
                    break;
                
                mipmapYOffset += mipmapSize.y;
                mipmapSize /= 2;
            }
            
            ivec3 mipmapTopLeft = ivec3(fragTexBotRight.x, fragTexTopLeft.y + mipmapYOffset, fragTexCoord.z);
            vec3 mipmapTexCoord = vec3(localTexCoordNormalized * vec2(mipmapSize), fragTexCoord.z);
            
            mipmapTexCoord += vec3(mipmapTopLeft.xy, 0);
            mipmapTexCoord.xy = BoundTexCoord(mipmapTexCoord.xy, mipmapTopLeft.xy, mipmapSize);
            
            //Nearest
            //outColor = texelFetch(ourTexture, ivec3(mipmapTexCoord.xy, fragTexCoord.z), 0) * (fragColorN);
            //return;
            
            vec2 lowerTexCoord = BoundTexCoord(floor(mipmapTexCoord.xy - vec2(0.5f, 0.5f)) + 0.5f, mipmapTopLeft.xy, mipmapSize);
            vec2 higherTexCoord = BoundTexCoord(floor(mipmapTexCoord.xy + vec2(0.5f, 0.5f)) + 0.5f, mipmapTopLeft.xy, mipmapSize);
            
            mipmapTexCoord.xy = clamp(mipmapTexCoord.xy, lowerTexCoord, higherTexCoord);
            
            float totalMultiplier = (higherTexCoord.x - lowerTexCoord.x) * (higherTexCoord.y - lowerTexCoord.y);
            
            float leftness = (higherTexCoord.x - mipmapTexCoord.x);
            float rightness = (mipmapTexCoord.x - lowerTexCoord.x);
            float topness = (higherTexCoord.y - mipmapTexCoord.y);
            float botness = (mipmapTexCoord.y - lowerTexCoord.y);
            
            
            float topLeftMultiplier = leftness * topness / totalMultiplier;
            float topRightMultiplier = rightness * topness / totalMultiplier;
            float botRightMultiplier = rightness * botness / totalMultiplier;
            float botLeftMultiplier = leftness * botness / totalMultiplier;
            
            vec4 linearColor =  texelFetch(ourTexture, ivec3(lowerTexCoord, fragTexCoord.z), 0) * topLeftMultiplier + 
                                texelFetch(ourTexture, ivec3(vec2(higherTexCoord.x, lowerTexCoord.y), fragTexCoord.z), 0) * topRightMultiplier +
                                texelFetch(ourTexture, ivec3(higherTexCoord, fragTexCoord.z), 0) * botRightMultiplier +
                                texelFetch(ourTexture, ivec3(vec2(lowerTexCoord.x, higherTexCoord.y), fragTexCoord.z), 0) * botLeftMultiplier;
            
            outColor = linearColor * (fragColorN);
        }  
    )";
    
    const int OpenGL3_3_Common::MAX_TEXTURE_LAYER_SIZE = 4096;
    =================================================================
    */
    class OpenGL3_3_Common
    {
        public:
            using CharSize = uint16_t;
            using CharCode = uint32_t;
            using CharTextureIdentifier = std::tuple<ssGUI::Backend::FontInterface*, CharSize, CharCode>;
    
        private:
            static const std::string VertShader;                                                //(Internal variable) Vertex shader string
            static const std::string FragShader;                                                //(Internal variable) Fragment shader string
            GLuint ProgramId;                                                                   //(Internal variable) Program Id for drawing GUI
            GLuint CachedImages;                                                                //(Internal variable) 2D texture array for storing the image atlas layers
            GLuint VAO;                                                                         //(Internal variable) VAO for drawing GUI
            GLuint VertsVBO;                                                                    //(Internal variable) VBO for storing vertices for drawing GUI
            GLuint ColorsVBO;                                                                   //(Internal variable) VBO for storing colors for drawing GUI
            GLuint TexCoordsVBO;                                                                //(Internal variable) VBO for storing texture coordinates (0 to image atlas size) for drawing GUI. If non texture, this is not used.
            GLuint TopLeftTexCoordsVBO;                                                         //(Internal variable) VBO for storing top left position texture coordinates (0 to image atlas size) for drawing GUI. 
                                                                                                //                      If non texture, this will be negative to signal this shape is non texture.
            GLuint BotRightTexCoordsVBO;                                                        //(Internal variable) VBO for storing bottom right position texture coordinates (0 to image atlas size) for drawing GUI. 
            GLint LastViewportProperties[4];                                                    //(Internal variable) Used for recording the last viewport when drawing GUI
            
            std::vector<glm::vec2> Vertices;                                                    //(Internal variable) Internal buffer for storing shape vertices
            std::vector<glm::vec4> Colors;                                                      //(Internal variable) Internal buffer for storing color 
            std::vector<glm::vec3> TexCoords;                                                   //(Internal variable) Internal buffer for storing texture coordinates
            std::vector<glm::vec3> TextureTopLeftCoords;                                        //(Internal variable) Internal buffer for storing top left corner for texture in image atlas
            std::vector<glm::vec3> TextureBotRightCoords;                                       //(Internal variable) Internal buffer for storing bottom right corner for texture in image atlas
            std::vector<GLuint> Idx;                                                            //(Internal variable) Internal buffer for storing indices for drawing GUI shapes.

            MainWindowInterface* CurrentMainWindow;                                      //(Internal variable) Used for performing viewport related operations
            
            DynamicImageAtlas* CurrentImageAtlas;                                               //(Internal variable) <DynamicImageAtlas> object for managing image atlas
            std::unordered_map<ssGUI::Backend::ImageInterface*, int> MappedImgIds;       //See <AddImageCache>
            std::unordered_map<CharTextureIdentifier, int> MappedFontIds;                       //See <AddFontCache>
            
            const int VERT_POS_INDEX;                                                           //(Internal variable) Shader layout location for vertex position
            const int VERT_COLOR_INDEX;                                                         //(Internal variable) Shader layout location for color
            const int VERT_TEX_COORD_INDEX;                                                     //(Internal variable) Shader layout location for texture coordinates
            const int VERT_TEX_TOP_LEFT_INDEX;                                                  //(Internal variable) Shader layout location for top left texture coordinates
            const int VERT_TEX_BOT_RIGHT_INDEX;                                                 //(Internal variable) Shader layout location for bot right texture coordinates
            static const int MAX_TEXTURE_LAYER_SIZE;                                            //(Internal variable) Size in pixel for width and height for each layer in image atlas

            //TODO: Method to generate mipmap

            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            const uint32_t character,
                            const ssGUI::Backend::FontInterface& font,
                            int characterSize);
                            
            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            const ssGUI::Backend::ImageInterface& image);

            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::u8vec4>& colors);
            
            GLuint CreateTexture2DArray(int textureSize, int numOfLayers);
            
            bool OnNewAtlasRequest();
            
            void SaveLastViewport();
            
            void LoadLastViewport();
            
            template<typename T>
            bool AddDrawingCache(std::unordered_map<T, int>& cachedIds, T key, glm::ivec2 textureSize, const void* rgba32Pixels);
    
            template<typename T>
            bool RemoveDrawingCache(std::unordered_map<T, int>& cachedIds, T key);
    
        public:        
            OpenGL3_3_Common(MainWindowInterface* mainWindow);
            
            ~OpenGL3_3_Common();
        
            //function: UpdateViewPortAndModelView
            //Updates glViewport and return the 4 x 4 matrix that can be used to correctly map the window coordinates in shader
            glm::mat4x4 UpdateViewPortAndModelView(glm::ivec2 widthHeight);
            
            //function: SaveState
            //See <DrawingInterface::SaveState>
            void SaveDrawingState();
            
            //function: RestoreDrawingState
            //See <DrawingInterface::RestoreDrawingState>
            void RestoreDrawingState();
            
            //function: DrawEntities
            //See <DrawingInterface::DrawEntities>
            bool CreateDrawingEntities(const std::vector<ssGUI::DrawingEntity>& entities);
            
            //function: AddFontCache
            //Adds a texture cache for displaying a character from a font with corresponding font size
            bool AddFontCache(CharTextureIdentifier charTexture);
            
            //function: AddImageCache
            //See <DrawingInterface::AddImageCache>
            bool AddImageCache(ssGUI::Backend::ImageInterface* backendImage);
            
            //function: RemoveFontCache
            //This removes the backend font from the cache.
            void RemoveFontCache(CharTextureIdentifier charTexture);
            
            //function: RemoveImageCache
            //See <DrawingInterface::RemoveImageCache>
            void RemoveImageCache(ssGUI::Backend::ImageInterface* backendImage);
            
            //function: DrawToBackBuffer
            //See <DrawingInterface::DrawToBackBuffer>
            void DrawToBackBuffer();
            //void ClearBackBuffer(glm::u8vec3 clearColor);
            
            //TODO: May not be able to implement
            //function: GetRawImageCacheHandle
            //This will return nullptr for OpenGL backends.
            //
            //For original purpose, see <DrawingInterface::GetRawImageCacheHandle>
            void* GetRawImageCacheHandle(ssGUI::Backend::ImageInterface* backendImage);
    };

}

}

#endif