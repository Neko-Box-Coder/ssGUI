#ifndef SSGUI_WINDOW_MODE
#define SSGUI_WINDOW_MODE

//namespace: ssGUI::Enums
namespace ssGUI::Enums
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


#endif