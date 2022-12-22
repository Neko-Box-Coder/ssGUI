#ifndef SSGUI_SFML_INPUT_CONVERTER_H
#define SSGUI_SFML_INPUT_CONVERTER_H

#ifndef UNICODE
#define UNICODE
#define _UNICODE
#endif

#include <windows.h>            /* must include this before GL/gl.h */

#include "ssGUI/DataClasses/RealtimeInputInfo.hpp"
#include "ssGUI/HeaderGroups/KeyGroup.hpp"
#include "ssGUI/Enums/MouseButton.hpp"
#include <vector>

//namespace: ssGUI::Backend
namespace ssGUI 
{
    
namespace Backend
{
    //class: ssGUI::Backend::Win32InputConverter
    //This class converts Win32 events into ssGUI events. Normally you don't need to deal with this class.
    class Win32InputConverter
    {
        public:
            //function: ConvertButtonAndKeys
            //This converts win32 key events into <KeyPresses> 
            static ssGUI::Enums::GenericButtonAndKeyInput ConvertButtonAndKeys(const MSG& msg);

            //function: ConvertMouseButtons
            //This converts win32 mouse button events into <Enums::MouseButton>
            static ssGUI::Enums::GenericButtonAndKeyInput ConvertMouseButtons(const MSG& msg);
    };
}

}

#endif