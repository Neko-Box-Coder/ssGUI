#ifndef SSGUI_OBJECT_TYPE
#define SSGUI_OBJECT_TYPE

//namespace: ssGUI::Enums
namespace ssGUI::Enums
{
    //About: GUIObjectType enum info
    //This class is a bit enum class. Therefore it can contain multiple enum value at once by using the | operator. For example, for a button is default to:
    //========================= C++ =========================
    //ssGUI::Enums::GUIObjectType type =    ssGUI::Enums::GUIObjectType::BUTTON | 
    //                                      ssGUI::Enums::GUIObjectType::BASE_WIDGET | 
    //                                      ssGUI::Enums::GUIObjectType::BASE_OBJECT;
    //=======================================================
    //
    //If you want to check if a GUI object *is or equal* to a GUI object type, you can just use the == or !=.
    // 
    //For example, the following will return true as Button *is* a widget
    //========================= C++ =========================
    //ssGUI::BaseClasses::Button button;
    //return button == ssGUI::Enums::GUIObjectType::Widget;
    //=======================================================
    //
    //Please note that this operation is *not commutative*. Meaning the follow will not be true since a widget *is not* a button.
    //> return ssGUI::Enums::GUIObjectType::Widget == button; 
    //
    //Further more, the bit & operator is supported as well if you need it.
    
    /*enum: GUIObjectType

    WINDOW          - Window object type. All windows will have this value.
    BASE_WIDGET     - Base widget object type. All widgets will have this value.
    MAIN_WINDOW     - Main window object type
    IMAGE           - Image object type
    TEXT            - Text object type
    BUTTON          - Button object type
    BASE_OBJECT     - Base Object type
    STANDARD_WINDOW - Standard window object type (WIP)
    STANDARD_BUTTON - Standard button object type
    */
    enum class GUIObjectType
    {
        WINDOW =            1 << 0,
        BASE_WIDGET =       1 << 1,
        MAIN_WINDOW =       1 << 2,
        IMAGE =             1 << 3,
        TEXT =              1 << 4,
        BUTTON =            1 << 5,
        BASE_OBJECT =       1 << 6,
        STANDARD_WINDOW =   1 << 7,
        STANDARD_BUTTON =   1 << 8
    };

    inline ssGUI::Enums::GUIObjectType operator|(ssGUI::Enums::GUIObjectType a, ssGUI::Enums::GUIObjectType b)
    {
        return static_cast<ssGUI::Enums::GUIObjectType>(static_cast<int>(a) | static_cast<int>(b));
    };

    inline ssGUI::Enums::GUIObjectType operator&(ssGUI::Enums::GUIObjectType a, ssGUI::Enums::GUIObjectType b)
    {
        return static_cast<ssGUI::Enums::GUIObjectType>(static_cast<int>(a) & static_cast<int>(b));
    };

    inline bool operator==(ssGUI::Enums::GUIObjectType a, ssGUI::Enums::GUIObjectType b)
    {
        return (static_cast<int>(a & b) == static_cast<int>(b));
    };

    inline bool operator!=(ssGUI::Enums::GUIObjectType a, ssGUI::Enums::GUIObjectType b)
    {
        return !(a==b);
    };
}


#endif