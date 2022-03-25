#ifndef SSGUI_CHARACTER_DETAILS
#define SSGUI_CHARACTER_DETAILS

#include "glm/vec2.hpp"
#include "ssGUI/BaseClasses/Font.hpp"

//namespace: ssGUI
namespace ssGUI
{
    //struct: CharacterDetails
    struct CharacterDetails
    {
        public:
            //var: Character
            //The character to be rendered
            wchar_t Character = L'\0';

            //var: FontIndex
            //The index for the font in the text object to be used
            int FontIndex = -1;

            //var: DefaultFontIndex
            //The index for the default font in the text object to be used
            int DefaultFontIndex = -1;

            //var: FontSize
            //The font size of the character
            int FontSize = 20;
    };
}

#endif