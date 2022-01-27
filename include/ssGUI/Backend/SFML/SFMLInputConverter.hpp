#ifndef SSGUI_SFML_INPUT_CONVERTER
#define SSGUI_SFML_INPUT_CONVERTER

#include "SFML/Window.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "ssGUI/HeaderGroups/KeyGroup.hpp"
#include "ssGUI/Enums/MouseButton.hpp"
#include <vector>
#include "ssGUI/BaseClasses/KeyPresses.hpp"


//namespace: ssGUI::Backend
namespace ssGUI::Backend
{
    //class: SFMLInputConverter
    //This class converts SFML events into ssGUI events. Normally you don't need to deal with this class.
    class SFMLInputConverter
    {
        public:
            //function: ConvertKeys
            //This converts sfml key events into <KeyPresses> 
            static ssGUI::KeyPresses ConvertKeys(const std::vector<sf::Event>& events);

            //function: ConvertMouseButtons
            //This converts sfml mouse button events into <Enums::MouseButton>
            static std::vector<ssGUI::Enums::MouseButton> ConvertMouseButtons(const std::vector<sf::Event>& events);

    };
}



#endif