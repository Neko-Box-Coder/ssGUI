#include "ssGUI/Backend/X11_OpenGL3_3/X11InputConverter.hpp"
#include "ssLogger/ssLog.hpp"

#include <X11/keysym.h>

namespace ssGUI
{

//Based on https://kbdlayout.info/kbdus/virtualkeys
//And https://kbdlayout.info/kbduk/virtualkeys for uk
namespace Backend
{
    //NOTE: Maybe GetKeyboardType is better? Not too sure
    //bool IsUkLayoutKeyboard()
    //{
    //    HKL hkl = GetKeyboardLayout(0);

    //    wchar_t localeData[99] = {0};

    //    //3 Letter country code: https://en.wikipedia.org/wiki/ISO_3166-1_alpha-3
    //    GetLocaleInfo(MAKELCID(hkl, SORT_DEFAULT), LOCALE_SISO3166CTRYNAME2, localeData, 99);

    //    return (wcscmp(localeData, L"GBR") == 0);
    //}

    //https://www.cl.cam.ac.uk/~mgk25/ucs/X11.keysyms.pdf
    ssGUI::Enums::GenericButtonAndKeyInput X11InputConverter::ConvertButtonAndKeys(KeySym keysym)
    {
        switch(keysym)
        {
            case XK_F1:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F1);
            case XK_F2:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F2);
            case XK_F3:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F3);
            case XK_F4:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F4);
            case XK_F5:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F5);
            case XK_F6:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F6);
            case XK_F7:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F7);
            case XK_F8:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F8);
            case XK_F9:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F9);
            case XK_F10:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F10);
            case XK_F11:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F11);
            case XK_F12:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F12);
            case 'a':
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::A);
            case 'b':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::B);
            case 'c':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::C);
            case 'd':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::D);
            case 'e':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::E);
            case 'f':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::F);
            case 'g':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::G);
            case 'h':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::H);
            case 'i':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::I);
            case 'j':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::J);
            case 'k':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::K);
            case 'l':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::L);
            case 'm':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::M);
            case 'n':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::N);
            case 'o':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::O);
            case 'p':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::P);
            case 'q':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::Q);
            case 'r':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::R);
            case 's':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::S);
            case 't':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::T);
            case 'u':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::U);
            case 'v':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::V);
            case 'w':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::W);
            case 'x':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::X);
            case 'y':    
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::Y);
            case 'z':    
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
            case XK_KP_1:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_ONE);
            case XK_KP_2:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_TWO);
            case XK_KP_3:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_THREE);
            case XK_KP_4:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_FOUR);
            case XK_KP_5:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_FIVE);
            case XK_KP_6:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_SIX);
            case XK_KP_7:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_SEVEN);
            case XK_KP_8:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_EIGHT);
            case XK_KP_9:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_NINE);
            case XK_KP_0:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_ZERO);
            case XK_grave:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::BACK_QUOTE);
            case XK_minus:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::MINUS);
            case XK_equal:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::EQUAL);            
            case XK_bracketleft:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::LEFT_BRACKET);
            case XK_bracketright:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::RIGHT_BRACKET);
            case XK_backslash:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::BACKSLASH);
            case XK_semicolon:            
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::SEMICOLON);
            case XK_numbersign:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::HASH_UK);
            case XK_comma:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::COMMA);
            case XK_period:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::PERIOD);
            case XK_slash:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::FORWARD_SLASH);
            case XK_KP_Divide:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::NUMPAD_FORWARD_SLASH);
            case XK_KP_Multiply:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::NUMPAD_MULTIPLY);
            case XK_KP_Subtract:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::NUMPAD_MINUS);
            case XK_KP_Add:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::NUMPAD_PLUS);
            case XK_KP_Decimal:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::NUMPAD_PERIOD);
            case XK_Tab:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::TAB);
            case XK_Caps_Lock:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::CAPS_LOCK);
            case XK_Shift_L:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::LEFT_SHIFT);
            case XK_Control_L:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::LEFT_CTRL);
            case XK_Super_L:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::LEFT_SYSTEM);
            case XK_Alt_L:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::LEFT_ALT);
            case XK_space:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::SPACE);
            case XK_Alt_R:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::RIGHT_ALT);
            case XK_Super_R:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::RIGHT_SYSTEM);
            case XK_Control_R:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::RIGHT_CTRL);
            case XK_Shift_R:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::RIGHT_SHIFT);
            case XK_Return:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::ENTER);
            case XK_BackSpace:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::BACKSPACE);
            case XK_Insert:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::INSERT);
            case XK_Home:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::HOME);
            case XK_Prior:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::PAGE_UP);
            case XK_Delete:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::DELETE);
            case XK_End:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::END);
            case XK_Next:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::PAGE_DOWN);
            case XK_Left:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::LEFT);
            case XK_Up:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::UP);
            case XK_Down:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::DOWN);
            case XK_Right:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::RIGHT);
            case XK_KP_Enter:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::NUMPAD_ENTER);
            default:
                //ssLOG_LINE("Unknown input: "<<keysym);
                return ssGUI::Enums::NO_INPUT;
        }
    }

    ssGUI::Enums::GenericButtonAndKeyInput X11InputConverter::ConvertMouseButtons(const XEvent& event)
    {
        switch(event.xbutton.button)
        {
            case Button1:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::MouseButton::LEFT);
            case Button2:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::MouseButton::MIDDLE);
            case Button3:
                return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::MouseButton::RIGHT);
            default:
                return ssGUI::Enums::NO_INPUT;
        }
    }
}

}
