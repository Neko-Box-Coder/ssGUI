#ifndef SSGUI_REALTIME_INPUT_INFO_H
#define SSGUI_REALTIME_INPUT_INFO_H

//#include "ssGUI/Enums/MouseButton.hpp"
#include "glm/vec2.hpp"
#include <vector>
#include <codecvt>

namespace ssGUI 
{ 

//namespace: ssGUI::Enums
namespace Enums
{
    //var: GenericButtonAndKeyInput
    using GenericButtonAndKeyInput = uint16_t;

    //const: NO_INPUT
    const uint16_t NO_INPUT = 0; 
}

}

//namespace: ssGUI
namespace ssGUI
{
    //struct: RealtimeInputInfo
    //An input info object that records the input status for each input changes within a frame
    struct RealtimeInputInfo
    {
        //var: CurrentKeyChanged
        ssGUI::Enums::GenericButtonAndKeyInput CurrentButtonAndKeyChanged = ssGUI::Enums::NO_INPUT;

        //var: MouseMoved
        bool MouseMoved = false;

        //var: CurrentMousePosition
        glm::ivec2 CurrentMousePosition = glm::ivec2();
    
        //var: CurrentCharacterEntered
        wchar_t CurrentCharacterEntered = ' ';
      
        //var: CharacterEntered
        bool CharacterEntered = false;
    };
}

#endif
