#ifndef SSGUI_X11_INPUT_CONVERTER_H
#define SSGUI_X11_INPUT_CONVERTER_H



#include "ssGUI/DataClasses/RealtimeInputInfo.hpp"
#include "ssGUI/HeaderGroups/InputGroup.hpp"
#include "ssGUI/Enums/MouseButton.hpp"
#include <X11/Xlib.h>
#include <vector>

//namespace: ssGUI::Backend
namespace ssGUI 
{
    
namespace Backend
{
    //class: ssGUI::Backend::X11InputConverter
    //This class converts X11 events into ssGUI events. Normally you don't need to deal with this class.
    class X11InputConverter
    {
        public:
            //function: ConvertButtonAndKeys
            //This converts X11 key events into <KeyPresses> 
            static ssGUI::Enums::GenericButtonAndKeyInput ConvertButtonAndKeys(KeySym keysym);

            //function: ConvertMouseButtons
            //This converts X11 mouse button events into <Enums::MouseButton>
            static ssGUI::Enums::GenericButtonAndKeyInput ConvertMouseButtons(const XEvent& event);
    };
}

}

#endif