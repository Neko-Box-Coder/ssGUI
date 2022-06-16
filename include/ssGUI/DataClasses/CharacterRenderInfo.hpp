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
            float Advance = 0;

            //var: DrawOffset
            //Offset for getting the top-left corner of the character
            glm::vec2 DrawOffset = glm::vec2();
            
            //var: UVOrigin
            //The uv texture origin for this character
            glm::vec2 UVOrigin = glm::vec2();
            
            //var: Size
            //The texture size corresponding to UVOrigin
            glm::vec2 Size = glm::vec2();

            //var: RenderPosition
            //This is the left-most position of the character on the horizontal line (In other words bottom left corner). 
            //To get the top-left corner of the character, add <DrawOffset> to this 
            glm::vec2 RenderPosition = glm::vec2();

            //var: CharacterAtNewline
            //True if this character is on a newline
            bool CharacterAtNewline = false;

            //var: Rendered
            //True if this character is being rendered. Otherwise this character is not being rendered.
            //Note that characters such as space and tab is not "rendered" since it has no texture to be rendered,
            //but it is still "present" in the text.
            bool Rendered = false;

            //var: LineMinY
            //Minimum Y bound of the current line (Relative to RenderPosition).
            //Maximum Y bound of the current line is always the same as RenderPosition.y.
            float LineMinY = 0;
    };
}

#endif