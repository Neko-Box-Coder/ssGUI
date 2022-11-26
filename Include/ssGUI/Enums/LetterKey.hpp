#ifndef SSGUI_LETTER_KEY
#define SSGUI_LETTER_KEY

#include <cstdint>

namespace ssGUI
{

//namespace: ssGUI::Enums
namespace Enums
{
    /*enum: LetterKey
    
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
}

}

#endif