#include "ssGUI/Backend/SFML/SFMLInputConverter.hpp"

namespace ssGUI::Backend
{
    ssGUI::Enums::GenericButtonAndKeyInput SFMLInputConverter::ConvertButtonAndKeys(const sf::Event& event)
    {
        // ssGUI::KeyPresses keys;

        // for(int i = 0; i < events.size(); i++)
        // {
            // switch (events.at(i).key.code)
            switch(event.key.code) 
            {
                case sf::Keyboard::F1:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F1); 
                case sf::Keyboard::F2:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F2);
                case sf::Keyboard::F3:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F3);
                case sf::Keyboard::F4:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F4);
                case sf::Keyboard::F5:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F5);
                case sf::Keyboard::F6:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F6);
                case sf::Keyboard::F7:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F7);
                case sf::Keyboard::F8:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F8);
                case sf::Keyboard::F9:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F9);
                case sf::Keyboard::F10:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F10);
                case sf::Keyboard::F11:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F11);
                case sf::Keyboard::F12:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::FunctionKey::F12);
                case sf::Keyboard::A:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::A);
                case sf::Keyboard::B:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::B);
                case sf::Keyboard::C:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::C);
                case sf::Keyboard::D:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::D);
                case sf::Keyboard::E:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::E);
                case sf::Keyboard::F:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::F);
                case sf::Keyboard::G:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::G);
                case sf::Keyboard::H:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::H);
                case sf::Keyboard::I:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::I);
                case sf::Keyboard::J:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::J);
                case sf::Keyboard::K:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::K);
                case sf::Keyboard::L:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::L);
                case sf::Keyboard::M:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::M);
                case sf::Keyboard::N:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::N);
                case sf::Keyboard::O:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::O);
                case sf::Keyboard::P:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::P);
                case sf::Keyboard::Q:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::Q);
                case sf::Keyboard::R:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::R);
                case sf::Keyboard::S:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::S);
                case sf::Keyboard::T:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::T);
                case sf::Keyboard::U:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::U);
                case sf::Keyboard::V:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::V);
                case sf::Keyboard::W:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::W);
                case sf::Keyboard::X:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::X);
                case sf::Keyboard::Y:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::Y);
                case sf::Keyboard::Z:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::LetterKey::Z);
                case sf::Keyboard::Num1:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::ONE);
                case sf::Keyboard::Num2:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::TWO);
                case sf::Keyboard::Num3:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::THREE);
                case sf::Keyboard::Num4:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::FOUR);
                case sf::Keyboard::Num5:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::FIVE);
                case sf::Keyboard::Num6:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::SIX);
                case sf::Keyboard::Num7:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::SEVEN);
                case sf::Keyboard::Num8:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::EIGHT);
                case sf::Keyboard::Num9:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NINE);
                case sf::Keyboard::Num0:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::ZERO);
                case sf::Keyboard::Numpad1:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_ONE);
                case sf::Keyboard::Numpad2:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_TWO);
                case sf::Keyboard::Numpad3:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_THREE);
                case sf::Keyboard::Numpad4:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_FOUR);
                case sf::Keyboard::Numpad5:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_FIVE);
                case sf::Keyboard::Numpad6:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_SIX);
                case sf::Keyboard::Numpad7:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_SEVEN);
                case sf::Keyboard::Numpad8:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_EIGHT);
                case sf::Keyboard::Numpad9:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_NINE);
                case sf::Keyboard::Numpad0:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::NumberKey::NUMPAD_ZERO);
                case sf::Keyboard::Tilde:   //Even though it is tilde, it is actually backquote button
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::BACK_QUOTE);
                case sf::Keyboard::Hyphen:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::MINUS);
                case sf::Keyboard::Equal:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::EQUAL);
                case sf::Keyboard::LBracket:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::LEFT_BRACKET);
                case sf::Keyboard::RBracket:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::RIGHT_BRACKET);
                case sf::Keyboard::Backslash:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::BACKSLASH);
                case sf::Keyboard::Semicolon:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::SEMICOLON);
                case sf::Keyboard::Quote:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::QUOTE);
                //HASH_UK missing
                //BACKSLASH_UK missing
                case sf::Keyboard::Comma:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::COMMA);
                case sf::Keyboard::Period:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::PERIOD);
                case sf::Keyboard::Slash:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::FORWARD_SLASH);
                case sf::Keyboard::Divide:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::NUMPAD_FORWARD_SLASH);
                case sf::Keyboard::Multiply:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::NUMPAD_MULTIPLY);
                case sf::Keyboard::Subtract:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::NUMPAD_MINUS);
                case sf::Keyboard::Add:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SymbolKey::NUMPAD_PLUS);
                //NUMPAD_PERIOD missing
                case sf::Keyboard::Tab:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::TAB);
                //CAPS_LOCK missing
                case sf::Keyboard::LShift:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::LEFT_SHIFT);
                case sf::Keyboard::LControl:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::LEFT_CTRL);
                case sf::Keyboard::LSystem:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::LEFT_SYSTEM);
                case sf::Keyboard::LAlt:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::LEFT_ALT);
                case sf::Keyboard::Space:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::SPACE);
                case sf::Keyboard::RAlt:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::RIGHT_ALT);
                case sf::Keyboard::RSystem:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::RIGHT_SYSTEM);
                case sf::Keyboard::RControl:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::RIGHT_CTRL);
                case sf::Keyboard::RShift:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::RIGHT_SHIFT);
                case sf::Keyboard::Enter:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::ENTER);
                case sf::Keyboard::Backspace:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::BACKSPACE);
                case sf::Keyboard::Insert:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::INSERT);
                case sf::Keyboard::Home:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::HOME);
                case sf::Keyboard::PageUp:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::PAGE_UP);
                case sf::Keyboard::Delete:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::DELETE);
                case sf::Keyboard::End:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::END);
                case sf::Keyboard::PageDown:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::PAGE_DOWN);
                case sf::Keyboard::Left:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::LEFT);
                case sf::Keyboard::Up:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::UP);
                case sf::Keyboard::Down:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::DOWN);
                case sf::Keyboard::Right:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::SystemKey::RIGHT);
                //NUMPAD_ENTER missing
                //TODO: Add follow keys
                case sf::Keyboard::Menu:
                case sf::Keyboard::Unknown:
                case sf::Keyboard::Escape:
                case sf::Keyboard::F13:
                case sf::Keyboard::F14:
                case sf::Keyboard::F15:
                case sf::Keyboard::Pause:
                case sf::Keyboard::KeyCount:
                    return ssGUI::Enums::NO_INPUT;
                }
        // }
        return ssGUI::Enums::NO_INPUT;
    }

    ssGUI::Enums::GenericButtonAndKeyInput SFMLInputConverter::ConvertMouseButtons(const sf::Event& event)
    {
        // std::vector<ssGUI::Enums::MouseButton> mouseButtons;

        // for(int i = 0; i < events.size(); i++)
        // {
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Left:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::MouseButton::LEFT);
                case sf::Mouse::Middle:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::MouseButton::MIDDLE);
                case sf::Mouse::Right:
                    return static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(ssGUI::Enums::MouseButton::RIGHT);
                case sf::Mouse::XButton1:
                case sf::Mouse::XButton2:
                case sf::Mouse::ButtonCount:
                    return ssGUI::Enums::NO_INPUT;
            }
        // }
        return ssGUI::Enums::NO_INPUT;
    }
}


