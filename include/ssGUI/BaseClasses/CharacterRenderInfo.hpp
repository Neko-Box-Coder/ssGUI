#ifndef SSGUI_CHARACTER_RENDER_INFO
#define SSGUI_CHARACTER_RENDER_INFO

#include "glm/vec2.hpp"

//namespace: ssGUI
namespace ssGUI
{
    //struct: CharacterRenderInfo
    struct CharacterRenderInfo
    {
        public:
            //var: Advance
            //How many pixels this character takes horizontally
            int Advance;

            //var: DrawOffset
            //Offset for getting the top-left corner of the character
            glm::ivec2 DrawOffset;
            
            //var: UVOrigin
            //The uv texture origin for this character
            glm::ivec2 UVOrigin;
            
            //var: Size
            //The texture size corresponding to UVOrigin
            glm::ivec2 Size;

            //var: RenderPosition
            //This is the position of the bottom-left corner of the character
            glm::ivec2 RenderPosition;

            //var: CharacterAtNewline
            //True if this character is on a newline
            bool CharacterAtNewline = false;

            //var: Valid
            //True if this character is valid and being rendered. Otherwise this character is not being rendered.
            bool Valid = false;
    };
}

#endif