#ifndef SSGUI_NUMBER_KEY
#define SSGUI_NUMBER_KEY

#include <cstdint>

//namespace: ssGUI::Enums
namespace ssGUI::Enums
{
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
}


#endif