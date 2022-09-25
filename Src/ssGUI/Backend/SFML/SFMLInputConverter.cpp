#include "ssGUI/Backend/SFML/SFMLInputConverter.hpp"

namespace ssGUI::Backend
{
    ssGUI::KeyPresses SFMLInputConverter::ConvertKeys(const std::vector<sf::Event>& events)
    {
        ssGUI::KeyPresses keys;

        for(int i = 0; i < events.size(); i++)
        {
            switch (events[i].key.code)
            {
                case sf::Keyboard::F1:
                    keys.FunctionKey.push_back(ssGUI::Enums::FunctionKey::F1); 
                    break;
                case sf::Keyboard::F2:
                    keys.FunctionKey.push_back(ssGUI::Enums::FunctionKey::F2); 
                    break;
                case sf::Keyboard::F3:
                    keys.FunctionKey.push_back(ssGUI::Enums::FunctionKey::F3); 
                    break;
                case sf::Keyboard::F4:
                    keys.FunctionKey.push_back(ssGUI::Enums::FunctionKey::F4); 
                    break;
                case sf::Keyboard::F5:
                    keys.FunctionKey.push_back(ssGUI::Enums::FunctionKey::F5); 
                    break;
                case sf::Keyboard::F6:
                    keys.FunctionKey.push_back(ssGUI::Enums::FunctionKey::F6); 
                    break;
                case sf::Keyboard::F7:
                    keys.FunctionKey.push_back(ssGUI::Enums::FunctionKey::F7); 
                    break;
                case sf::Keyboard::F8:
                    keys.FunctionKey.push_back(ssGUI::Enums::FunctionKey::F8); 
                    break;
                case sf::Keyboard::F9:
                    keys.FunctionKey.push_back(ssGUI::Enums::FunctionKey::F9); 
                    break;
                case sf::Keyboard::F10:
                    keys.FunctionKey.push_back(ssGUI::Enums::FunctionKey::F10); 
                    break;
                case sf::Keyboard::F11:
                    keys.FunctionKey.push_back(ssGUI::Enums::FunctionKey::F11); 
                    break;
                case sf::Keyboard::F12:
                    keys.FunctionKey.push_back(ssGUI::Enums::FunctionKey::F12); 
                    break;
                case sf::Keyboard::A:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::A);
                    break;
                case sf::Keyboard::B:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::B);
                    break;
                case sf::Keyboard::C:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::C);
                    break;
                case sf::Keyboard::D:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::D);
                    break;
                case sf::Keyboard::E:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::E);
                    break;
                case sf::Keyboard::F:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::F);
                    break;
                case sf::Keyboard::G:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::G);
                    break;
                case sf::Keyboard::H:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::H);
                    break;
                case sf::Keyboard::I:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::I);
                    break;
                case sf::Keyboard::J:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::J);
                    break;
                case sf::Keyboard::K:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::K);
                    break;
                case sf::Keyboard::L:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::L);
                    break;
                case sf::Keyboard::M:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::M);
                    break;
                case sf::Keyboard::N:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::N);
                    break;
                case sf::Keyboard::O:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::O);
                    break;
                case sf::Keyboard::P:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::P);
                    break;
                case sf::Keyboard::Q:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::Q);
                    break;
                case sf::Keyboard::R:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::R);
                    break;
                case sf::Keyboard::S:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::S);
                    break;
                case sf::Keyboard::T:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::T);
                    break;
                case sf::Keyboard::U:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::U);
                    break;
                case sf::Keyboard::V:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::V);
                    break;
                case sf::Keyboard::W:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::W);
                    break;
                case sf::Keyboard::X:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::X);
                    break;
                case sf::Keyboard::Y:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::Y);
                    break;
                case sf::Keyboard::Z:
                    keys.LetterKey.push_back(ssGUI::Enums::LetterKey::Z);
                    break;
                case sf::Keyboard::Num1:
                    keys.NumberKey.push_back(ssGUI::Enums::NumberKey::ONE);
                    break;
                case sf::Keyboard::Num2:
                    keys.NumberKey.push_back(ssGUI::Enums::NumberKey::TWO);
                    break;
                case sf::Keyboard::Num3:
                    keys.NumberKey.push_back(ssGUI::Enums::NumberKey::THREE);
                    break;
                case sf::Keyboard::Num4:
                    keys.NumberKey.push_back(ssGUI::Enums::NumberKey::FOUR);
                    break;
                case sf::Keyboard::Num5:
                    keys.NumberKey.push_back(ssGUI::Enums::NumberKey::FIVE);
                    break;
                case sf::Keyboard::Num6:
                    keys.NumberKey.push_back(ssGUI::Enums::NumberKey::SIX);
                    break;
                case sf::Keyboard::Num7:
                    keys.NumberKey.push_back(ssGUI::Enums::NumberKey::SEVEN);
                    break;
                case sf::Keyboard::Num8:
                    keys.NumberKey.push_back(ssGUI::Enums::NumberKey::EIGHT);
                    break;
                case sf::Keyboard::Num9:
                    keys.NumberKey.push_back(ssGUI::Enums::NumberKey::NINE);
                    break;
                case sf::Keyboard::Num0:
                    keys.NumberKey.push_back(ssGUI::Enums::NumberKey::ZERO);
                    break;
                case sf::Keyboard::Numpad1:
                    keys.NumberKey.push_back(ssGUI::Enums::NumberKey::NUMPAD_ONE);
                    break;
                case sf::Keyboard::Numpad2:
                    keys.NumberKey.push_back(ssGUI::Enums::NumberKey::NUMPAD_TWO);
                    break;
                case sf::Keyboard::Numpad3:
                    keys.NumberKey.push_back(ssGUI::Enums::NumberKey::NUMPAD_THREE);
                    break;
                case sf::Keyboard::Numpad4:
                    keys.NumberKey.push_back(ssGUI::Enums::NumberKey::NUMPAD_FOUR);
                    break;
                case sf::Keyboard::Numpad5:
                    keys.NumberKey.push_back(ssGUI::Enums::NumberKey::NUMPAD_FIVE);
                    break;
                case sf::Keyboard::Numpad6:
                    keys.NumberKey.push_back(ssGUI::Enums::NumberKey::NUMPAD_SIX);
                    break;
                case sf::Keyboard::Numpad7:
                    keys.NumberKey.push_back(ssGUI::Enums::NumberKey::NUMPAD_SEVEN);
                    break;
                case sf::Keyboard::Numpad8:
                    keys.NumberKey.push_back(ssGUI::Enums::NumberKey::NUMPAD_EIGHT);
                    break;
                case sf::Keyboard::Numpad9:
                    keys.NumberKey.push_back(ssGUI::Enums::NumberKey::NUMPAD_NINE);
                    break;
                case sf::Keyboard::Numpad0:
                    keys.NumberKey.push_back(ssGUI::Enums::NumberKey::NUMPAD_ZERO);
                    break;
                case sf::Keyboard::Tilde:   //Even though it is tilde, it is actually backquote button
                    keys.SymbolKey.push_back(ssGUI::Enums::SymbolKey::BACK_QUOTE);
                    break;
                case sf::Keyboard::Hyphen:
                    keys.SymbolKey.push_back(ssGUI::Enums::SymbolKey::MINUS);
                    break;
                case sf::Keyboard::Equal:
                    keys.SymbolKey.push_back(ssGUI::Enums::SymbolKey::EQUAL);
                    break;
                case sf::Keyboard::LBracket:
                    keys.SymbolKey.push_back(ssGUI::Enums::SymbolKey::LEFT_BRACKET);
                    break;
                case sf::Keyboard::RBracket:
                    keys.SymbolKey.push_back(ssGUI::Enums::SymbolKey::RIGHT_BRACKET);
                    break;
                case sf::Keyboard::Backslash:
                    keys.SymbolKey.push_back(ssGUI::Enums::SymbolKey::BACKSLASH);
                    break;
                case sf::Keyboard::Semicolon:
                    keys.SymbolKey.push_back(ssGUI::Enums::SymbolKey::SEMICOLON);
                    break;
                case sf::Keyboard::Quote:
                    keys.SymbolKey.push_back(ssGUI::Enums::SymbolKey::QUOTE);
                    break;
                //HASH_UK missing
                //BACKSLASH_UK missing
                case sf::Keyboard::Comma:
                    keys.SymbolKey.push_back(ssGUI::Enums::SymbolKey::COMMA);
                    break;
                case sf::Keyboard::Period:
                    keys.SymbolKey.push_back(ssGUI::Enums::SymbolKey::PERIOD);
                    break;
                case sf::Keyboard::Slash:
                    keys.SymbolKey.push_back(ssGUI::Enums::SymbolKey::FORWARD_SLASH);
                    break;
                case sf::Keyboard::Divide:
                    keys.SymbolKey.push_back(ssGUI::Enums::SymbolKey::NUMPAD_FORWARD_SLASH);
                    break;
                case sf::Keyboard::Multiply:
                    keys.SymbolKey.push_back(ssGUI::Enums::SymbolKey::NUMPAD_MULTIPLY);
                    break;
                case sf::Keyboard::Subtract:
                    keys.SymbolKey.push_back(ssGUI::Enums::SymbolKey::NUMPAD_MINUS);
                    break;
                case sf::Keyboard::Add:
                    keys.SymbolKey.push_back(ssGUI::Enums::SymbolKey::NUMPAD_PLUS);
                    break;
                //NUMPAD_PERIOD missing
                case sf::Keyboard::Tab:
                    keys.SystemKey.push_back(ssGUI::Enums::SystemKey::TAB);
                    break;
                //CAPS_LOCK missing
                case sf::Keyboard::LShift:
                    keys.SystemKey.push_back(ssGUI::Enums::SystemKey::LEFT_SHIFT);
                    break;
                case sf::Keyboard::LControl:
                    keys.SystemKey.push_back(ssGUI::Enums::SystemKey::LEFT_CTRL);
                    break;
                case sf::Keyboard::LSystem:
                    keys.SystemKey.push_back(ssGUI::Enums::SystemKey::LEFT_SYSTEM);
                    break;
                case sf::Keyboard::LAlt:
                    keys.SystemKey.push_back(ssGUI::Enums::SystemKey::LEFT_ALT);
                    break;
                case sf::Keyboard::Space:
                    keys.SystemKey.push_back(ssGUI::Enums::SystemKey::SPACE);
                    break;
                case sf::Keyboard::RAlt:
                    keys.SystemKey.push_back(ssGUI::Enums::SystemKey::RIGHT_ALT);
                    break;
                case sf::Keyboard::RSystem:
                    keys.SystemKey.push_back(ssGUI::Enums::SystemKey::RIGHT_SYSTEM);
                    break;
                case sf::Keyboard::RControl:
                    keys.SystemKey.push_back(ssGUI::Enums::SystemKey::RIGHT_CTRL);
                    break;
                case sf::Keyboard::RShift:
                    keys.SystemKey.push_back(ssGUI::Enums::SystemKey::RIGHT_SHIFT);
                    break;
                case sf::Keyboard::Enter:
                    keys.SystemKey.push_back(ssGUI::Enums::SystemKey::ENTER);
                    break;
                case sf::Keyboard::Backspace:
                    keys.SystemKey.push_back(ssGUI::Enums::SystemKey::BACKSPACE);
                    break;
                case sf::Keyboard::Insert:
                    keys.SystemKey.push_back(ssGUI::Enums::SystemKey::INSERT);
                    break;
                case sf::Keyboard::Home:
                    keys.SystemKey.push_back(ssGUI::Enums::SystemKey::HOME);
                    break;
                case sf::Keyboard::PageUp:
                    keys.SystemKey.push_back(ssGUI::Enums::SystemKey::PAGE_UP);
                    break;
                case sf::Keyboard::Delete:
                    keys.SystemKey.push_back(ssGUI::Enums::SystemKey::DELETE);
                    break;
                case sf::Keyboard::End:
                    keys.SystemKey.push_back(ssGUI::Enums::SystemKey::END);
                    break;
                case sf::Keyboard::PageDown:
                    keys.SystemKey.push_back(ssGUI::Enums::SystemKey::PAGE_DOWN);
                    break;
                case sf::Keyboard::Left:
                    keys.SystemKey.push_back(ssGUI::Enums::SystemKey::LEFT);
                    break;
                case sf::Keyboard::Up:
                    keys.SystemKey.push_back(ssGUI::Enums::SystemKey::UP);
                    break;
                case sf::Keyboard::Down:
                    keys.SystemKey.push_back(ssGUI::Enums::SystemKey::DOWN);
                    break;
                case sf::Keyboard::Right:
                    keys.SystemKey.push_back(ssGUI::Enums::SystemKey::RIGHT);
                    break;
                //NUMPAD_ENTER missing
            }
        }

        return keys;
    }



    std::vector<ssGUI::Enums::MouseButton> SFMLInputConverter::ConvertMouseButtons(const std::vector<sf::Event>& events)
    {
        std::vector<ssGUI::Enums::MouseButton> mouseButtons;

        for(int i = 0; i < events.size(); i++)
        {
            switch (events[i].mouseButton.button)
            {
                case sf::Mouse::Left:
                    mouseButtons.push_back(ssGUI::Enums::MouseButton::LEFT);
                    break;
                case sf::Mouse::Middle:
                    mouseButtons.push_back(ssGUI::Enums::MouseButton::MIDDLE);
                    break;
                case sf::Mouse::Right:
                    mouseButtons.push_back(ssGUI::Enums::MouseButton::RIGHT);
                    break;
            }
        }

        return mouseButtons;
    }
}


