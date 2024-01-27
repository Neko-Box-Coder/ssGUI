#ifndef SSGUI_BACKEND_SDL2_INPUT_CONVERTER_HPP
#define SSGUI_BACKEND_SDL2_INPUT_CONVERTER_HPP

#include "ssGUI/HeaderGroups/InputGroup.hpp"
#include "ssGUI/Enums/MouseButton.hpp"

#include "SDL.h"

namespace ssGUI
{
    
namespace Backend
{
    //class: ssGUI::Backend::SDL2InputConverter
    class SDL2InputConverter
    {
        public:
            //function: ConvertButtonAndKeys
            //This converts SDL2 key events into <ssGUI::Enums::GenericInput>
            static Enums::GenericInput ConvertKeys(SDL_Keycode keycode);
            
            //function: ConvertMouseButton
            //This converts SDL2 mouse button events into <ssGUI::Enums::MouseButton>
            static Enums::GenericInput ConvertMouseButton(uint8_t button);
    };
}

}


#endif