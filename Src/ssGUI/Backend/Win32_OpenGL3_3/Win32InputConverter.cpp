#include "ssGUI/Backend/Win32_OpenGL3_3/Win32InputConverter.hpp"

namespace ssGUI
{

//Based on https://kbdlayout.info/kbdus/virtualkeys
//And https://kbdlayout.info/kbduk/virtualkeys for uk
namespace Backend
{
    //NOTE: Maybe GetKeyboardType is better? Not too sure
    bool IsUkLayoutKeyboard()
    {
        HKL hkl = GetKeyboardLayout(0);

        wchar_t localeData[99] = {0};

        //3 Letter country code: https://en.wikipedia.org/wiki/ISO_3166-1_alpha-3
        GetLocaleInfo(MAKELCID(hkl, SORT_DEFAULT), LOCALE_SISO3166CTRYNAME2, localeData, 99);

        return (wcscmp(localeData, L"GBR") == 0);
    }

    ssGUI::Enums::GenericButtonAndKeyInput Win32InputConverter::ConvertButtonAndKeys(const MSG& msg)
    {
        switch(msg.wParam)
        {
            case VK_F1:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F1);
            case VK_F2:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F2);
            case VK_F3:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F3);
            case VK_F4:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F4);
            case VK_F5:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F5);
            case VK_F6:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F6);
            case VK_F7:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F7);
            case VK_F8:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F8);
            case VK_F9:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F9);
            case VK_F10:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F10);
            case VK_F11:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F11);
            case VK_F12:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F12);
            case 'A':
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::A);
            case 'B':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::B);
            case 'C':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::C);
            case 'D':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::D);
            case 'E':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::E);
            case 'F':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::F);
            case 'G':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::G);
            case 'H':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::H);
            case 'I':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::I);
            case 'J':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::J);
            case 'K':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::K);
            case 'L':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::L);
            case 'M':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::M);
            case 'N':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::N);
            case 'O':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::O);
            case 'P':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::P);
            case 'Q':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::Q);
            case 'R':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::R);
            case 'S':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::S);
            case 'T':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::T);
            case 'U':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::U);
            case 'V':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::V);
            case 'W':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::W);
            case 'X':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::X);
            case 'Y':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::Y);
            case 'Z':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::Z);
            case '1':
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::ONE);
            case '2':
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::TWO);
            case '3':
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::THREE);
            case '4':
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::FOUR);
            case '5':
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::FIVE);
            case '6':
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::SIX);
            case '7':
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::SEVEN);
            case '8':
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::EIGHT);
            case '9':
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NINE);
            case '0':
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::ZERO);
            case VK_NUMPAD1:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_ONE);
            case VK_NUMPAD2:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_TWO);
            case VK_NUMPAD3:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_THREE);
            case VK_NUMPAD4:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_FOUR);
            case VK_NUMPAD5:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_FIVE);
            case VK_NUMPAD6:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_SIX);
            case VK_NUMPAD7:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_SEVEN);
            case VK_NUMPAD8:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_EIGHT);
            case VK_NUMPAD9:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_NINE);
            case VK_NUMPAD0:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_ZERO);
            case VK_OEM_8:  //UK
                //In UK, VK_OEM_8 is back quote. US keyboard doesn't (or shouldn't) have VK_OEM_8
                if(IsUkLayoutKeyboard())
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::BACK_QUOTE);
                else
                    return ssGUI::Enums::NO_INPUT;
            case VK_OEM_MINUS:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::MINUS);
            case VK_OEM_PLUS:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::EQUAL);
            case VK_OEM_3:  
                if(IsUkLayoutKeyboard())
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::QUOTE);
                else
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::BACK_QUOTE);                
            case VK_OEM_4:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::LEFT_BRACKET);
            case VK_OEM_6:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::RIGHT_BRACKET);
            case VK_OEM_5:
                if(IsUkLayoutKeyboard())
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::BACKSLASH_UK);
                else
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::BACKSLASH);
            case VK_OEM_102:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::BACKSLASH);
            case VK_OEM_1:            
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::SEMICOLON);
            case VK_OEM_7:
                if(IsUkLayoutKeyboard())
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::HASH_UK);
                else
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::QUOTE);
            case VK_OEM_COMMA:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::COMMA);
            case VK_OEM_PERIOD:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::PERIOD);
            case VK_OEM_2:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::FORWARD_SLASH);
            case VK_DIVIDE:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::NUMPAD_FORWARD_SLASH);
            case VK_MULTIPLY:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::NUMPAD_MULTIPLY);
            case VK_SUBTRACT:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::NUMPAD_MINUS);
            case VK_ADD:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::NUMPAD_PLUS);
            case VK_DECIMAL:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::NUMPAD_PERIOD);
            case VK_TAB:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::TAB);
            case VK_CAPITAL:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::CAPS_LOCK);
            case VK_LSHIFT:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::LEFT_SHIFT);
            case VK_LCONTROL:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::LEFT_CTRL);
            case VK_LWIN:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::LEFT_SYSTEM);
            case VK_LMENU:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::LEFT_ALT);
            case VK_SPACE:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::SPACE);
            case VK_RMENU:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::RIGHT_ALT);
            case VK_RWIN:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::RIGHT_SYSTEM);
            case VK_RCONTROL:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::RIGHT_CTRL);
            case VK_RSHIFT:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::RIGHT_SHIFT);
            case VK_RETURN:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::ENTER);
            case VK_BACK:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::BACKSPACE);
            case VK_INSERT:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::INSERT);
            case VK_HOME:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::HOME);
            case VK_PRIOR:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::PAGE_UP);
            case VK_DELETE:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::DELETE);
            case VK_END:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::END);
            case VK_NEXT:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::PAGE_DOWN);
            case VK_LEFT:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::LEFT);
            case VK_UP:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::UP);
            case VK_DOWN:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::DOWN);
            case VK_RIGHT:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::RIGHT);
            //NUMPAD_ENTER is mapped to VK_RETURN, same as normal Enter. No way to tell if it is numpad enter or not
            //TODO: maybe remove NUMPAD_ENTER?
            default:
                return ssGUI::Enums::NO_INPUT;
        }
    }

    ssGUI::Enums::GenericButtonAndKeyInput Win32InputConverter::ConvertMouseButtons(const MSG& msg)
    {
        switch(msg.wParam)
        {
            case MK_LBUTTON:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::MouseButton::LEFT);
            case MK_MBUTTON:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::MouseButton::MIDDLE);
            case MK_RBUTTON:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::MouseButton::RIGHT);
            default:
                return ssGUI::Enums::NO_INPUT;
        }
    }
}

}
