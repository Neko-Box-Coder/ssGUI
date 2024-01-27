#include "ssGUI/Backend/SDL2/SDL2InputConverter.hpp"
#include "ssGUI/DataClasses/RealtimeInputInfo.hpp"

namespace ssGUI 
{

namespace Backend
{
    Enums::GenericInput SDL2InputConverter::ConvertKeys(SDL_Keycode keycode)
    {
        switch(keycode)
        {
            case SDLK_F1:
                return static_cast<Enums::GenericInput>(Enums::FunctionKey::F1);
            case SDLK_F2:
                return static_cast<Enums::GenericInput>(Enums::FunctionKey::F2);
            case SDLK_F3:
                return static_cast<Enums::GenericInput>(Enums::FunctionKey::F3);
            case SDLK_F4:
                return static_cast<Enums::GenericInput>(Enums::FunctionKey::F4);
            case SDLK_F5:
                return static_cast<Enums::GenericInput>(Enums::FunctionKey::F5);
            case SDLK_F6:
                return static_cast<Enums::GenericInput>(Enums::FunctionKey::F6);
            case SDLK_F7:
                return static_cast<Enums::GenericInput>(Enums::FunctionKey::F7);
            case SDLK_F8:
                return static_cast<Enums::GenericInput>(Enums::FunctionKey::F8);
            case SDLK_F9:
                return static_cast<Enums::GenericInput>(Enums::FunctionKey::F9);
            case SDLK_F10:
                return static_cast<Enums::GenericInput>(Enums::FunctionKey::F10);
            case SDLK_F11:
                return static_cast<Enums::GenericInput>(Enums::FunctionKey::F11);
            case SDLK_F12:
                return static_cast<Enums::GenericInput>(Enums::FunctionKey::F12);
            
            case SDLK_a:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::A);
            case SDLK_b:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::B);
            case SDLK_c:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::C);
            case SDLK_d:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::D);
            case SDLK_e:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::E);
            case SDLK_f:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::F);
            case SDLK_g:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::G);
            case SDLK_h:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::H);
            case SDLK_i:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::I);
            case SDLK_j:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::J);
            case SDLK_k:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::K);
            case SDLK_l:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::L);
            case SDLK_m:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::M);
            case SDLK_n:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::N);
            case SDLK_o:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::O);
            case SDLK_p:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::P);
            case SDLK_q:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::Q);
            case SDLK_r:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::R);
            case SDLK_s:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::S);
            case SDLK_t:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::T);
            case SDLK_u:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::U);
            case SDLK_v:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::V);
            case SDLK_w:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::W);
            case SDLK_x:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::X);
            case SDLK_y:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::Y);
            case SDLK_z:
                return static_cast<Enums::GenericInput>(Enums::LetterKey::Z);
            case SDLK_0:
                return static_cast<Enums::GenericInput>(Enums::NumberKey::ZERO);
            case SDLK_1:
                return static_cast<Enums::GenericInput>(Enums::NumberKey::ONE);
            case SDLK_2:
                return static_cast<Enums::GenericInput>(Enums::NumberKey::TWO);
            case SDLK_3:
                return static_cast<Enums::GenericInput>(Enums::NumberKey::THREE);
            case SDLK_4:
                return static_cast<Enums::GenericInput>(Enums::NumberKey::FOUR);
            case SDLK_5:
                return static_cast<Enums::GenericInput>(Enums::NumberKey::FIVE);
            case SDLK_6:
                return static_cast<Enums::GenericInput>(Enums::NumberKey::SIX);
            case SDLK_7:
                return static_cast<Enums::GenericInput>(Enums::NumberKey::SEVEN);
            case SDLK_8:
                return static_cast<Enums::GenericInput>(Enums::NumberKey::EIGHT);
            case SDLK_9:
                return static_cast<Enums::GenericInput>(Enums::NumberKey::NINE);
            case SDLK_KP_0:
                return static_cast<Enums::GenericInput>(Enums::NumberKey::NUMPAD_ZERO);
            case SDLK_KP_1:
                return static_cast<Enums::GenericInput>(Enums::NumberKey::NUMPAD_ONE);
            case SDLK_KP_2:
                return static_cast<Enums::GenericInput>(Enums::NumberKey::NUMPAD_TWO);
            case SDLK_KP_3:
                return static_cast<Enums::GenericInput>(Enums::NumberKey::NUMPAD_THREE);
            case SDLK_KP_4:
                return static_cast<Enums::GenericInput>(Enums::NumberKey::NUMPAD_FOUR);
            case SDLK_KP_5:
                return static_cast<Enums::GenericInput>(Enums::NumberKey::NUMPAD_FIVE);
            case SDLK_KP_6:
                return static_cast<Enums::GenericInput>(Enums::NumberKey::NUMPAD_SIX);
            case SDLK_KP_7:
                return static_cast<Enums::GenericInput>(Enums::NumberKey::NUMPAD_SEVEN);
            case SDLK_KP_8:
                return static_cast<Enums::GenericInput>(Enums::NumberKey::NUMPAD_EIGHT);
            case SDLK_KP_9:
                return static_cast<Enums::GenericInput>(Enums::NumberKey::NUMPAD_NINE);
            case SDLK_BACKQUOTE:
                return static_cast<Enums::GenericInput>(Enums::SymbolKey::BACK_QUOTE);
            case SDLK_MINUS:
                return static_cast<Enums::GenericInput>(Enums::SymbolKey::MINUS);
            case SDLK_EQUALS:
                return static_cast<Enums::GenericInput>(Enums::SymbolKey::EQUAL);
            case SDLK_LEFTBRACKET:
                return static_cast<Enums::GenericInput>(Enums::SymbolKey::LEFT_BRACKET);
            case SDLK_RIGHTBRACKET:
                return static_cast<Enums::GenericInput>(Enums::SymbolKey::RIGHT_BRACKET);
            case SDLK_BACKSLASH:
                return static_cast<Enums::GenericInput>(Enums::SymbolKey::BACKSLASH);
            case SDLK_SEMICOLON:
                return static_cast<Enums::GenericInput>(Enums::SymbolKey::SEMICOLON);
            case SDLK_QUOTE:
                return static_cast<Enums::GenericInput>(Enums::SymbolKey::QUOTE);
            //HASH_UK is treated the same as backslash in SDL2
            case SDLK_COMMA:
                return static_cast<Enums::GenericInput>(Enums::SymbolKey::COMMA);
            case SDLK_PERIOD:
                return static_cast<Enums::GenericInput>(Enums::SymbolKey::PERIOD);
            case SDLK_SLASH:
                return static_cast<Enums::GenericInput>(Enums::SymbolKey::FORWARD_SLASH);
            case SDLK_KP_DIVIDE:
                return static_cast<Enums::GenericInput>(Enums::SymbolKey::NUMPAD_FORWARD_SLASH);
            case SDLK_KP_MULTIPLY:
                return static_cast<Enums::GenericInput>(Enums::SymbolKey::NUMPAD_MULTIPLY);
            case SDLK_KP_MINUS:
                return static_cast<Enums::GenericInput>(Enums::SymbolKey::NUMPAD_MINUS);
            case SDLK_KP_PLUS:
                return static_cast<Enums::GenericInput>(Enums::SymbolKey::NUMPAD_PLUS);
            case SDLK_KP_PERIOD:
                return static_cast<Enums::GenericInput>(Enums::SymbolKey::NUMPAD_PERIOD);
            
            case SDLK_TAB:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::TAB);
            case SDLK_CAPSLOCK:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::CAPS_LOCK);
            case SDLK_LSHIFT:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::LEFT_SHIFT);
            case SDLK_LCTRL:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::LEFT_CTRL);
            case SDLK_LGUI:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::LEFT_SYSTEM);
            case SDLK_LALT:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::LEFT_ALT);
            case SDLK_SPACE:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::SPACE);
            case SDLK_RALT:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::RIGHT_ALT);
            case SDLK_RGUI:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::RIGHT_SYSTEM);
            case SDLK_RCTRL:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::RIGHT_CTRL);
            case SDLK_RSHIFT:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::RIGHT_SHIFT);
            case SDLK_RETURN:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::ENTER);
            case SDLK_BACKSPACE:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::BACKSPACE);
            case SDLK_INSERT:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::INSERT);
            case SDLK_HOME:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::HOME);
            case SDLK_PAGEUP:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::PAGE_UP);
            case SDLK_DELETE:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::DELETE);
            case SDLK_END:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::END);
            case SDLK_PAGEDOWN:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::PAGE_DOWN);
            case SDLK_LEFT:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::LEFT);
            case SDLK_UP:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::UP);
            case SDLK_DOWN:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::DOWN);
            case SDLK_RIGHT:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::RIGHT);
            case SDLK_KP_ENTER:
                return static_cast<Enums::GenericInput>(Enums::SystemKey::NUMPAD_ENTER);
            default:
                //ssGUI_WARNING(ssGUI_TAG_BACKEND, "Unknown input: " << keycode);
                return Enums::NO_INPUT;
        }
    }
    
    Enums::GenericInput SDL2InputConverter::ConvertMouseButton(uint8_t button)
    {
        switch(button)
        {
            case SDL_BUTTON_LEFT:
                return static_cast<Enums::GenericInput>(Enums::MouseButton::LEFT);
            case SDL_BUTTON_MIDDLE:
                return static_cast<Enums::GenericInput>(Enums::MouseButton::MIDDLE);
            case SDL_BUTTON_RIGHT:
                return static_cast<Enums::GenericInput>(Enums::MouseButton::RIGHT);
            case SDL_BUTTON_X1:
                return static_cast<Enums::GenericInput>(Enums::MouseButton::EXTRA_1);
            case SDL_BUTTON_X2:
                return static_cast<Enums::GenericInput>(Enums::MouseButton::EXTRA_2);
            default:
                //ssGUI_WARNING(ssGUI_TAG_BACKEND, "Unknown input: " << button);
                return Enums::NO_INPUT;
        }
    }
}

}

