#ifndef SSGUI_CHARACTER_INFO
#define SSGUI_CHARACTER_INFO

#include "glm/vec2.hpp"

//namespace: ssGUI
namespace ssGUI
{
    //struct: CharacterInfo
    struct CharacterInfo
    {
        public:
            //var: Advance
            //How many pixels this character takes horizontally
            int Advance;

            //var: DrawOffset
            //Offset for drawing this character
            glm::ivec2 DrawOffset;
            
            //var: UVOrigin
            //The uv texture origin for this cahracter
            glm::ivec2 UVOrigin;
            
            //var: Size
            //The texture size corresponding to UVOrigin
            glm::ivec2 Size;
    };
}

#endif