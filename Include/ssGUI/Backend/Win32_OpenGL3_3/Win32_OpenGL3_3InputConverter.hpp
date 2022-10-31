#ifndef SSGUI_WIN32_OPEN_GL3_3_INPUT_CONVERTER
#define SSGUI_WIN32_OPEN_GL3_3_INPUT_CONVERTER

#include "ssGUI/DataClasses/RealtimeInputInfo.hpp"
#include "ssGUI/HeaderGroups/KeyGroup.hpp"
#include "ssGUI/Enums/MouseButton.hpp"
#include <vector>

namespace ssGUI 
{ 

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::SFMLInputConverter
    //This class converts SFML events into ssGUI events. Normally you don't need to deal with this class.
    class Win32_OpenGL3_3InputConverter
    {
        public:
            //function: ConvertButtonAndKeys
            //This converts sfml key events into <KeyPresses> 
            //static ssGUI::Enums::GenericButtonAndKeyInput ConvertButtonAndKeys(const sf::Event& event);

            //function: ConvertMouseButtons
            //This converts sfml mouse button events into <Enums::MouseButton>
            // static ssGUI::Enums::GenericButtonAndKeyInput ConvertMouseButtons(const sf::Event& event);
    };
}

}

#endif