#ifndef SSGUI_TEXT_WRAPPING_H
#define SSGUI_TEXT_WRAPPING_H

#include "ssGUI/HelperClasses/EnumToStringMacro.hpp"
#include <string>
namespace ssGUI
{

//namespace: ssGUI::Enums
namespace Enums
{
    /*enum: TextWrapping

    NO_WRAPPING         - No wrapping. Meaning the text will go outside the text widget size.
    WORD_WRAPPING       - Word wrapping. Meaning the text will wrap to next line for preventing overflowing the width of the text widget.
    CHARACTER_WRAPPING  - Character wrapping. Meaning the character will wrap to next line for preventing overflowing the width of the text widget.
    COUNT               - Count
    */
    enum class TextWrapping
    {
        NO_WRAPPING,
        WORD_WRAPPING,
        CHARACTER_WRAPPING,
        COUNT
    };
    
    //function: TextWrappingToString
    inline std::string TextWrappingToString(TextWrapping wrapping)
    {
        static_assert((int)TextWrapping::COUNT == 3, "ToString");
        switch(wrapping)
        {
            RETURN_ENUM_STRING(TextWrapping::NO_WRAPPING);
            RETURN_ENUM_STRING(TextWrapping::WORD_WRAPPING);
            RETURN_ENUM_STRING(TextWrapping::CHARACTER_WRAPPING);
            RETURN_ENUM_STRING(TextWrapping::COUNT);
        }
        
        return "";
    }
}

}

#endif