#ifndef SSGUI_NUMBER_KEY_H
#define SSGUI_NUMBER_KEY_H

#include "ssGUI/HelperClasses/EnumToStringMacro.hpp"

#include <string>
#include <cstdint>

namespace ssGUI
{

//namespace: ssGUI::Enums
namespace Enums
{
    using GenericButtonAndKeyInput = uint16_t;
    
    /*enum: NumberKey

    ONE             - 1 key at the top
    TWO             - 2 key at the top
    THREE           - 3 key at the top
    FOUR            - 4 key at the top
    FIVE            - 5 key at the top 
    SIX             - 6 key at the top
    SEVEN           - 7 key at the top
    EIGHT           - 8 key at the top
    NINE            - 9 key at the top
    ZERO            - 0 key at the top
    NUMPAD_ONE      - 1 numpad key on the right
    NUMPAD_TWO      - 2 numpad key on the right
    NUMPAD_THREE    - 3 numpad key on the right 
    NUMPAD_FOUR     - 4 numpad key on the right
    NUMPAD_FIVE     - 5 numpad key on the right
    NUMPAD_SIX      - 6 numpad key on the right
    NUMPAD_SEVEN    - 7 numpad key on the right
    NUMPAD_EIGHT    - 8 numpad key on the right
    NUMPAD_NINE     - 9 numpad key on the right
    NUMPAD_ZERO     - 0 numpad key on the right
    */
    enum class NumberKey : uint16_t
    {
        ONE = 300,
        TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,ZERO,
        NUMPAD_ONE,NUMPAD_TWO,NUMPAD_THREE,NUMPAD_FOUR,
        NUMPAD_FIVE,NUMPAD_SIX,NUMPAD_SEVEN,NUMPAD_EIGHT,NUMPAD_NINE,NUMPAD_ZERO,
        COUNT = NUMPAD_ZERO - ONE + 1
    };
    
    //function: InputToString
    inline std::string InputToString(NumberKey input)
    {
        static_assert((int)NumberKey::COUNT == 20, "ToString");
        switch(input)
        {
            RETURN_ENUM_STRING(NumberKey::ONE);
            RETURN_ENUM_STRING(NumberKey::TWO);
            RETURN_ENUM_STRING(NumberKey::THREE);
            RETURN_ENUM_STRING(NumberKey::FOUR);
            RETURN_ENUM_STRING(NumberKey::FIVE);
            RETURN_ENUM_STRING(NumberKey::SIX);
            RETURN_ENUM_STRING(NumberKey::SEVEN);
            RETURN_ENUM_STRING(NumberKey::EIGHT);
            RETURN_ENUM_STRING(NumberKey::NINE);
            RETURN_ENUM_STRING(NumberKey::ZERO);
            RETURN_ENUM_STRING(NumberKey::NUMPAD_ONE);
            RETURN_ENUM_STRING(NumberKey::NUMPAD_TWO);
            RETURN_ENUM_STRING(NumberKey::NUMPAD_THREE);
            RETURN_ENUM_STRING(NumberKey::NUMPAD_FOUR);
            RETURN_ENUM_STRING(NumberKey::NUMPAD_FIVE);
            RETURN_ENUM_STRING(NumberKey::NUMPAD_SIX);
            RETURN_ENUM_STRING(NumberKey::NUMPAD_SEVEN);
            RETURN_ENUM_STRING(NumberKey::NUMPAD_EIGHT);
            RETURN_ENUM_STRING(NumberKey::NUMPAD_NINE);
            RETURN_ENUM_STRING(NumberKey::NUMPAD_ZERO);
            RETURN_ENUM_STRING(NumberKey::COUNT);
        }
        
        return "";
    }
    
    //function: InputIsNumberKey
    inline bool InputIsNumberKey(ssGUI::Enums::GenericButtonAndKeyInput input)
    {
        return input >= (uint16_t)NumberKey::ONE && input < (uint16_t)NumberKey::ONE + (uint16_t)NumberKey::COUNT;
    }
}

}

#endif