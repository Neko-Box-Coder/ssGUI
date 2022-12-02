#ifndef SSGUI_CHARACTER_RENDER_INFO_H
#define SSGUI_CHARACTER_RENDER_INFO_H

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
            //Offset for getting the top-left corner of the character from the baseline.
            //In other words, this is the bearing but Y is flipped
            glm::vec2 DrawOffset = glm::vec2();
            
            //var: Size
            //The character size corresponding to UVOrigin
            glm::vec2 Size = glm::vec2();

            //var: BaselinePosition
            //This is the character global position *on the baseline*.
            //To get the top-left corner of the character, add <DrawOffset> to this 
            glm::vec2 BaselinePosition = glm::vec2();

            //var: CharacterAtNewline
            //True if this character is on a newline
            bool CharacterAtNewline = false;

            //var: Rendered
            //When true, this means there's font assigned to this character and not space, tab or newline character.
            bool Rendered = false;

            //var: Valid
            //When false, this means there's no font assigned to this character or is null character
            bool Valid = false;

            //var: LineMinY
            //Minimum Y bound of the current line (Relative to BaselinePosition)
            float LineMinY = 0;

            //var: LineMaxY
            //Maximum Y bound of the current line (Relative to BaselinePosition)
            float LineMaxY = 0;
    };
}

#endif