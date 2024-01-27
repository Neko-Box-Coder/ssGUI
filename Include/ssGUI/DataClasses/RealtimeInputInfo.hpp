#ifndef SSGUI_REALTIME_INPUT_INFO_H
#define SSGUI_REALTIME_INPUT_INFO_H

//#include "ssGUI/Enums/MouseButton.hpp"
#include "ssGUI/HelperClasses/OutputStreamUtil.hpp"

#include "glm/vec2.hpp"
#include <vector>
#include <codecvt>

namespace ssGUI 
{ 

namespace Enums
{
    //var: ssGUI::Enums::GenericInput
    using GenericInput = uint16_t;

    //const: ssGUI::Enums::NO_INPUT
    const uint16_t NO_INPUT = 0; 
    
    /*
    enum: ssGUI::Enums::RealtimeInputType
    This enum determines the type of the realtime input
    
    KEY_BUTTON_INPUT    - A keyboard key or mouse button input event
    MOUSE_POSITION      - A mouse position input event
    MOUSE_WHEEL         - A mouse wheel scroll input event
    TEXT_INPUT          - A text input event
    */
    enum class RealtimeInputType : uint8_t
    {
        KEY_BUTTON_INPUT,
        MOUSE_POSITION, 
        MOUSE_WHEEL_SCROLL,
        TEXT_INPUT,
        COUNT
    };
}

}

namespace ssGUI
{
    //struct: ssGUI::RealtimeInputInfo
    //An input info object that records the input status for each input changes within a frame
    struct RealtimeInputInfo
    {
        //var: InputType
        Enums::RealtimeInputType InputType = Enums::RealtimeInputType::COUNT;
        
        //struct: ssGUI::RealtimeInputInfo::Data
        //Union containing corresponding data for different input types
        union
        {
            //var: KeyButton
            struct
            {
                //var: KeyButtonInput
                Enums::GenericInput Input;
                
                //var: IsDown
                bool IsDown;
                
            } KeyButton;
            
            //var: ssGUI::RealtimeInputInfo::Data::Position
            glm::ivec2 MousePosition;
            
            //var: MouseScrollDelta
            glm::vec2 MouseScrollDelta;
            
            //var: CharacterEntered
            char32_t CharacterEntered = ' ';
        } Data;
        
        inline friend std::ostream& operator<<(std::ostream& stream, const RealtimeInputInfo& other)
        {
            stream << SSGUI_OUTPUT_CLASS_NAME(RealtimeInputInfo);
            
            static_assert((uint8_t)Enums::RealtimeInputType::COUNT == 4, "Not Match");
            switch(other.InputType)
            {
                case Enums::RealtimeInputType::KEY_BUTTON_INPUT:
                    stream  << SSGUI_OUTPUT_VAR(Data.KeyButton.Input)
                            << SSGUI_OUTPUT_LAST_VAR(Data.KeyButton.IsDown);
                    break;
                case Enums::RealtimeInputType::MOUSE_POSITION:
                    stream << SSGUI_OUTPUT_LAST_VAR(Data.MousePosition);
                    break;
                case Enums::RealtimeInputType::MOUSE_WHEEL_SCROLL:
                    stream << SSGUI_OUTPUT_LAST_VAR(Data.MouseScrollDelta);
                    break;
                case Enums::RealtimeInputType::TEXT_INPUT:
                    stream << SSGUI_OUTPUT_LAST_VAR(Data.CharacterEntered);
                    break;
                default:
                    stream << "Unknown input type";
                    break;
            }
            
            return stream;
        }
    };
    
    
    
}

#endif
