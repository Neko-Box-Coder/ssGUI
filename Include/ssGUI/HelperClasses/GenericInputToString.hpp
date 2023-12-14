#ifndef SSGUI_GENERIC_INPUT_TO_STRING_H
#define SSGUI_GENERIC_INPUT_TO_STRING_H

#include "ssGUI/HeaderGroups/InputGroup.hpp"
#include "ssGUI/Enums/MouseButton.hpp"

//namespace: ssGUI
namespace ssGUI
{
    //function: GenericInputToString
    //Returns the string for a given input
    inline std::string GenericInputToString(ssGUI::Enums::GenericInput input)
    {
        if(ssGUI::Enums::InputIsFunctionKey(input))
            return ssGUI::Enums::InputToString((ssGUI::Enums::FunctionKey)input);
        else if(ssGUI::Enums::InputIsLetterKey(input))
            return ssGUI::Enums::InputToString((ssGUI::Enums::LetterKey)input);
        else if(ssGUI::Enums::InputIsMouseButton(input))
            return ssGUI::Enums::InputToString((ssGUI::Enums::MouseButton)input);
        else if(ssGUI::Enums::InputIsNumberKey(input))
            return ssGUI::Enums::InputToString((ssGUI::Enums::NumberKey)input);
        else if(ssGUI::Enums::InputIsSymbolKey(input))
            return ssGUI::Enums::InputToString((ssGUI::Enums::SymbolKey)input);
        else if(ssGUI::Enums::InputIsSystemKey(input))
            return ssGUI::Enums::InputToString((ssGUI::Enums::SystemKey)input);
        else
            return "";
    }
}

#endif