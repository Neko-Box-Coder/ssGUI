#ifndef SSGUI_WINDOW_DECORATION_OPTIONS_HPP
#define SSGUI_WINDOW_DECORATION_OPTIONS_HPP


namespace ssGUI 
{ 

//namespace: ssGUI::Enums
namespace Enums
{

    /*
    enum: WindowDecorationOptions
    //TODO(NOW)
    */
    enum class WindowDecorationOptions 
    {
        NONE,
        UNKNOWN,
        ICON,
        TITLE_BAR,
        OMNIPRESENT,
        ALWAYS_ON_TOP,
        ALWAYS_ON_BOTTOM,
        SHADE,
        MINIMIZE,
        MAXIMIZE,
        CLOSE
    };

    inline WindowDecorationOptions operator|(WindowDecorationOptions a, WindowDecorationOptions b)
    {
        return static_cast<WindowDecorationOptions>(static_cast<int>(a) | static_cast<int>(b));
    }
    
    inline WindowDecorationOptions operator&(WindowDecorationOptions a, WindowDecorationOptions b)
    {
        return static_cast<WindowDecorationOptions>(static_cast<int>(a) & static_cast<int>(b));
    }
    
    inline WindowDecorationOptions& operator|=(WindowDecorationOptions& a, WindowDecorationOptions b)
    {
        a = a | b;
        return a;
    }
    
    inline WindowDecorationOptions& operator&=(WindowDecorationOptions& a, WindowDecorationOptions b)
    {
        a = a & b;
        return a;
    }
    
    inline bool operator==(WindowDecorationOptions a, WindowDecorationOptions b)
    {
        return (static_cast<int>(a) & static_cast<int>(b)) == static_cast<int>(b);
    }
    
    inline bool operator!=(WindowDecorationOptions a, WindowDecorationOptions b)
    {
        return !(a == b);
    }
}

}

#endif
