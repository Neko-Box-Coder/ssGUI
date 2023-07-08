#ifndef SSGUI_REALTIME_INPUT_INFO_H
#define SSGUI_REALTIME_INPUT_INFO_H

//#include "ssGUI/Enums/MouseButton.hpp"
#include "glm/vec2.hpp"
#include <vector>
#include <codecvt>
#include "ssGUI/HelperClasses/OutputStreamUtil.hpp"

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
        
        inline friend std::ostream& operator<<(std::ostream& stream, const RealtimeInputInfo& other)
        {
            stream  << SSGUI_OUTPUT_CLASS_NAME(RealtimeInputInfo)
                    << SSGUI_OUTPUT_VAR(CurrentButtonAndKeyChanged)
                    << SSGUI_OUTPUT_VAR(MouseMoved)
                    << SSGUI_OUTPUT_VAR(CurrentMousePosition)
                    << SSGUI_OUTPUT_VAR(CurrentCharacterEntered)
                    << SSGUI_OUTPUT_LAST_VAR(CharacterEntered);
            
            return stream;
        }
    };
    
    
    
}

#endif
