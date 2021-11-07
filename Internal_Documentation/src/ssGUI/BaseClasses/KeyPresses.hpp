#ifndef SSGUI_KEY_PRESSES
#define SSGUI_KEY_PRESSES

#include <vector>
#include <algorithm>
#include "ssGUI/Enums/Key.hpp"

//namespace: ssGUI
namespace ssGUI
{
    //class: KeyPresses
    class KeyPresses
    {
        public:
            //var: FunctionKey
            std::vector<ssGUI::Enums::FunctionKey> FunctionKey;
            
            //var: LetterKey
            std::vector<ssGUI::Enums::LetterKey> LetterKey;
            
            //var: NumberKey
            std::vector<ssGUI::Enums::NumberKey> NumberKey;
            
            //var: SymbolKey
            std::vector<ssGUI::Enums::SymbolKey> SymbolKey;
            
            //var: SystemKey
            std::vector<ssGUI::Enums::SystemKey> SystemKey;

            //function: IsFunctionKeyPresent
            bool IsFunctionKeyPresent(ssGUI::Enums::FunctionKey key) const{return std::find(FunctionKey.begin(), FunctionKey.end(), key) != FunctionKey.end();};
            
            //function: IsLetterKeyPresent
            bool IsLetterKeyPresent(ssGUI::Enums::LetterKey key) const{return std::find(LetterKey.begin(), LetterKey.end(), key) != LetterKey.end();};
            
            //function: IsNumberKeyPresent
            bool IsNumberKeyPresent(ssGUI::Enums::NumberKey key) const{return std::find(NumberKey.begin(), NumberKey.end(), key) != NumberKey.end();};
            
            //function: IsSymbolKeyPresent
            bool IsSymbolKeyPresent(ssGUI::Enums::SymbolKey key) const{return std::find(SymbolKey.begin(), SymbolKey.end(), key) != SymbolKey.end();};
            
            //function: IsSystemKeyPresent
            bool IsSystemKeyPresent(ssGUI::Enums::SystemKey key) const{return std::find(SystemKey.begin(), SystemKey.end(), key) != SystemKey.end();};
    };
}


#endif