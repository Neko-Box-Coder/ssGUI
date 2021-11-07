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
            int Advance;

            //var: DrawOffset
            glm::ivec2 DrawOffset;
            
            //var: UVOrigin
            glm::ivec2 UVOrigin;
            
            //var: Size
            glm::ivec2 Size;
    };
}

#endif