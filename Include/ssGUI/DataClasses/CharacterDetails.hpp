#ifndef SSGUI_CHARACTER_DETAILS_H
#define SSGUI_CHARACTER_DETAILS_H

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "ssGUI/DataClasses/Font.hpp"

//namespace: ssGUI
namespace ssGUI
{
    //struct: ssGUI::CharacterDetails
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
            float FontSize = 20;

            //var: CharacterColor
            //The color of the character
            glm::u8vec4 CharacterColor = glm::u8vec4(0, 0, 0, 255);

            //var: Underlined
            //If the character is underlined or not
            bool Underlined = false;
            
            #define FLOAT_EQ(a, b) (a + 0.0001f > b && a - 0.0001f < b)
            inline bool operator==(CharacterDetails const & details)
            {
                return  Character == details.Character &&
                        FontIndex == details.FontIndex &&
                        DefaultFontIndex == details.DefaultFontIndex &&
                        FLOAT_EQ(FontSize, details.FontSize) &&
                        CharacterColor == details.CharacterColor &&
                        Underlined == details.Underlined;
            }
            
            inline bool operator!=(CharacterDetails const & details)
            {
                return !operator==(details);
            }
    };
}

#endif