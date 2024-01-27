#ifndef SSGUI_LETTER_KEY_H
#define SSGUI_LETTER_KEY_H

#include "ssGUI/HelperClasses/EnumToStringMacro.hpp"

#include <string>
#include <cstdint>

namespace ssGUI
{

//namespace: ssGUI::Enums
namespace Enums
{
    using GenericInput = uint16_t;
    
    /*
    enum: LetterKey
    
    A   - Respective letter.
    B   - Respective letter.
    C   - Respective letter.
    D   - Respective letter.
    E   - Respective letter.
    F   - Respective letter.
    G   - Respective letter.
    H   - Respective letter.
    I   - Respective letter.
    J   - Respective letter.
    K   - Respective letter.
    L   - Respective letter.
    M   - Respective letter.
    N   - Respective letter.
    O   - Respective letter.
    P   - Respective letter.
    Q   - Respective letter.
    R   - Respective letter.
    S   - Respective letter.
    T   - Respective letter.
    U   - Respective letter.
    V   - Respective letter.
    W   - Respective letter.
    X   - Respective letter.
    Y   - Respective letter.
    Z   - Respective letter.   
    */
    enum class LetterKey : uint16_t
    {
        A = 200,
        B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z, 
        COUNT = Z - A + 1
    };
    
    //function: InputToString
    inline std::string InputToString(LetterKey input)
    {
        static_assert((int)LetterKey::COUNT == 26, "ToString");
        switch(input)
        {
            RETURN_ENUM_STRING(LetterKey::A);
            RETURN_ENUM_STRING(LetterKey::B);
            RETURN_ENUM_STRING(LetterKey::C);
            RETURN_ENUM_STRING(LetterKey::D);
            RETURN_ENUM_STRING(LetterKey::E);
            RETURN_ENUM_STRING(LetterKey::F);
            RETURN_ENUM_STRING(LetterKey::G);
            RETURN_ENUM_STRING(LetterKey::H);
            RETURN_ENUM_STRING(LetterKey::I);
            RETURN_ENUM_STRING(LetterKey::J);
            RETURN_ENUM_STRING(LetterKey::K);
            RETURN_ENUM_STRING(LetterKey::L);
            RETURN_ENUM_STRING(LetterKey::M);
            RETURN_ENUM_STRING(LetterKey::N);
            RETURN_ENUM_STRING(LetterKey::O);
            RETURN_ENUM_STRING(LetterKey::P);
            RETURN_ENUM_STRING(LetterKey::Q);
            RETURN_ENUM_STRING(LetterKey::R);
            RETURN_ENUM_STRING(LetterKey::S);
            RETURN_ENUM_STRING(LetterKey::T);
            RETURN_ENUM_STRING(LetterKey::U);
            RETURN_ENUM_STRING(LetterKey::V);
            RETURN_ENUM_STRING(LetterKey::W);
            RETURN_ENUM_STRING(LetterKey::X);
            RETURN_ENUM_STRING(LetterKey::Y);
            RETURN_ENUM_STRING(LetterKey::Z);
            RETURN_ENUM_STRING(LetterKey::COUNT);
        }
        
        return "";
    }
    
    //function: InputIsLetterKey
    inline bool InputIsLetterKey(ssGUI::Enums::GenericInput input)
    {
        return input >= (uint16_t)LetterKey::A && input < (uint16_t)LetterKey::A + (uint16_t)LetterKey::COUNT;
    }
}

}

#endif