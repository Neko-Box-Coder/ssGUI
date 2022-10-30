#ifndef SSGUI_FUNCTION_KEY
#define SSGUI_FUNCTION_KEY

#include <cstdint>

namespace ssGUI
{

//namespace: ssGUI::Enums
namespace Enums
{
    /*enum: FunctionKey
    
    F1  - F1
    F2  - F2
    F3  - F3
    F4  - F4
    F5  - F5
    F6  - F6
    F7  - F7
    F8  - F8
    F9  - F9
    F10 - F10
    F11 - F11
    F12 - F12
    
    */
    enum class FunctionKey : uint16_t
    {
        F1 = 100,
        F2,F3,F4,F5,F6,F7,F8,F9,F10,F11,F12, 
        COUNT = F12 - F1 + 1
    };
}

}

#endif