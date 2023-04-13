#ifndef SSGUI_SFML_INPUT_CONVERTER_H
#define SSGUI_SFML_INPUT_CONVERTER_H

#include "SFML/Window.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "ssGUI/DataClasses/RealtimeInputInfo.hpp"
#include "ssGUI/HeaderGroups/InputGroup.hpp"
#include "ssGUI/Enums/MouseButton.hpp"
#include <vector>

//namespace: ssGUI::Backend
namespace ssGUI 
{
    
namespace Backend
{
    //class: ssGUI::Backend::SFMLInputConverter
    //This class converts SFML events into ssGUI events. Normally you don't need to deal with this class.
    class SFMLInputConverter
    {
        public:
            //function: ConvertButtonAndKeys
            //This converts sfml key events into <ssGUI::Enums::GenericButtonAndKeyInput> 
            static ssGUI::Enums::GenericButtonAndKeyInput ConvertButtonAndKeys(const sf::Event& event);

            //function: ConvertMouseButtons
            //This converts sfml mouse button events into <Enums::MouseButton>
            static ssGUI::Enums::GenericButtonAndKeyInput ConvertMouseButtons(const sf::Event& event);
    };
}

}

#endif