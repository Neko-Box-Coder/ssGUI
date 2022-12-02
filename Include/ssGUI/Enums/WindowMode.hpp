#ifndef SSGUI_WINDOW_MODE_H
#define SSGUI_WINDOW_MODE_H

namespace ssGUI 
{ 

//namespace: ssGUI::Enums
namespace Enums
{
    /*enum: WindowMode

    NORMAL      - Normal window, with toolbar, close buttons, etc.. by default
    BORDERLESS  - A window without any decoration. Note that window can be with any size
    FULLSCREEN  - Fullscreen window
    */
    enum class WindowMode
    {
        NORMAL,
        BORDERLESS,
        FULLSCREEN
    };
}

}

#endif