#ifndef SSGUI_FUNCTION_KEY_H
#define SSGUI_FUNCTION_KEY_H

#include "ssGUI/HelperClasses/EnumToStringMacro.hpp"

#include <string>
#include <cstdint>

namespace ssGUI
{

//namespace: ssGUI::Enums
namespace Enums
{
    using GenericButtonAndKeyInput = uint16_t;

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
    
    //function: InputToString
    inline std::string InputToString(FunctionKey input)
    {
        static_assert((int)FunctionKey::COUNT == 12, "ToString");
        switch(input)
        {
            RETURN_ENUM_STRING(FunctionKey::F1);
            RETURN_ENUM_STRING(FunctionKey::F2);
            RETURN_ENUM_STRING(FunctionKey::F3);
            RETURN_ENUM_STRING(FunctionKey::F4);
            RETURN_ENUM_STRING(FunctionKey::F5);
            RETURN_ENUM_STRING(FunctionKey::F6);
            RETURN_ENUM_STRING(FunctionKey::F7);
            RETURN_ENUM_STRING(FunctionKey::F8);
            RETURN_ENUM_STRING(FunctionKey::F9);
            RETURN_ENUM_STRING(FunctionKey::F10);
            RETURN_ENUM_STRING(FunctionKey::F11);
            RETURN_ENUM_STRING(FunctionKey::F12);
            RETURN_ENUM_STRING(FunctionKey::COUNT);
        }
        
        return "";
    }
    
    //function: InputIsFunctionKey
    inline bool InputIsFunctionKey(ssGUI::Enums::GenericButtonAndKeyInput input)
    {
        return input >= (uint16_t)FunctionKey::F1 && input < (uint16_t)FunctionKey::F1 + (uint16_t)FunctionKey::COUNT;
    }
}

}

#endif