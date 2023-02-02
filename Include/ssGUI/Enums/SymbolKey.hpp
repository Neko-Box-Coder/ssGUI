#ifndef SSGUI_SYMBOL_KEY_H
#define SSGUI_SYMBOL_KEY_H

#include "ssGUI/HelperClasses/EnumToStringMacro.hpp"

#include <string>
#include <cstdint>

namespace ssGUI
{

//namespace: ssGUI::Enums
namespace Enums
{
    using GenericButtonAndKeyInput = uint16_t;
    
    /*enum: SymbolKey

    BACK_QUOTE              - Back quote key. (Also know as Tilde key)
    MINUS                   - Minus key (-) 
    EQUAL                   - Equal key (=)
    LEFT_BRACKET            - Left bracket key ([)
    RIGHT_BRACKET           - Right bracket key (])
    BACKSLASH               - Backslash key (\)
    SEMICOLON               - Semicolon key (;)
    QUOTE                   - Quote key (')
    HASH_UK                 - Hash key for UK keyboard layout (#) (SFML backend doesn't support it)
    BACKSLASH_UK            - Backslash key for UK keyboard layout (\) (SFML backend doesn't support it)
    COMMA                   - Comma key (,)
    PERIOD                  - Period key (.)
    FORWARD_SLASH           - Forward slash key (/)
    NUMPAD_FORWARD_SLASH    - Forward slash key for numpad (/)
    NUMPAD_MULTIPLY         - Multiply key for numpad (*)
    NUMPAD_MINUS            - Minus key for numpad (-)
    NUMPAD_PLUS             - Plus key for numpad (+)
    NUMPAD_PERIOD           - Period key for numpad (.)
    */
    enum class SymbolKey : uint16_t
    {
        BACK_QUOTE = 400, 
        MINUS, EQUAL, LEFT_BRACKET, RIGHT_BRACKET, BACKSLASH, SEMICOLON,
        QUOTE, HASH_UK, BACKSLASH_UK, COMMA, PERIOD, FORWARD_SLASH, NUMPAD_FORWARD_SLASH, 
        NUMPAD_MULTIPLY, NUMPAD_MINUS, NUMPAD_PLUS, NUMPAD_PERIOD, 
        COUNT = NUMPAD_PERIOD - BACK_QUOTE + 1
    };
    
    //function: InputToString
    inline std::string InputToString(SymbolKey input)
    {
        static_assert((int)SymbolKey::COUNT == 18, "ToString");
        switch(input)
        {
            RETURN_ENUM_STRING(SymbolKey::BACK_QUOTE);
            RETURN_ENUM_STRING(SymbolKey::MINUS);
            RETURN_ENUM_STRING(SymbolKey::EQUAL);
            RETURN_ENUM_STRING(SymbolKey::LEFT_BRACKET);
            RETURN_ENUM_STRING(SymbolKey::RIGHT_BRACKET);
            RETURN_ENUM_STRING(SymbolKey::BACKSLASH);
            RETURN_ENUM_STRING(SymbolKey::SEMICOLON);
            RETURN_ENUM_STRING(SymbolKey::QUOTE);
            RETURN_ENUM_STRING(SymbolKey::HASH_UK);
            RETURN_ENUM_STRING(SymbolKey::BACKSLASH_UK);
            RETURN_ENUM_STRING(SymbolKey::COMMA);
            RETURN_ENUM_STRING(SymbolKey::PERIOD);
            RETURN_ENUM_STRING(SymbolKey::FORWARD_SLASH);
            RETURN_ENUM_STRING(SymbolKey::NUMPAD_FORWARD_SLASH);
            RETURN_ENUM_STRING(SymbolKey::NUMPAD_MULTIPLY);
            RETURN_ENUM_STRING(SymbolKey::NUMPAD_MINUS);
            RETURN_ENUM_STRING(SymbolKey::NUMPAD_PLUS);
            RETURN_ENUM_STRING(SymbolKey::NUMPAD_PERIOD);
            RETURN_ENUM_STRING(SymbolKey::COUNT);
        }
        
        return "";
    }
    
    //function: InputIsSymbolKey
    inline bool InputIsSymbolKey(ssGUI::Enums::GenericButtonAndKeyInput input)
    {
        return input >= (uint16_t)SymbolKey::BACK_QUOTE && input < (uint16_t)SymbolKey::BACK_QUOTE + (uint16_t)SymbolKey::COUNT;
    }
}

}

#endif