#ifndef SSGUI_CHARACTER_RENDER_INFO
#define SSGUI_CHARACTER_RENDER_INFO

#include "glm/vec2.hpp"

//namespace: ssGUI
namespace ssGUI
{
    //struct: ssGUI::CharacterRenderInfo
    struct CharacterRenderInfo
    {
        public:
            //var: Advance
            //How many pixels this character takes horizontally
            int Advance;

            //var: DrawOffset
            //Offset for getting the top-left corner of the character
            glm::vec2 DrawOffset;
            
            //var: UVOrigin
            //The uv texture origin for this character
            glm::vec2 UVOrigin;
            
            //var: Size
            //The texture size corresponding to UVOrigin
            glm::vec2 Size;

            //var: RenderPosition
            //This is the left-most position of the character on the horizontal line. 
            //To get the top-left corner of the character, add <DrawOffset> to this 
            glm::vec2 RenderPosition;

            //var: CharacterAtNewline
            //True if this character is on a newline
            bool CharacterAtNewline = false;

            //var: Valid
            //True if this character is valid and being rendered. Otherwise this character is not being rendered.
            bool Valid = false;
    };
}

#endif