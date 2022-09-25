#ifndef SSGUI_TEXT_WRAPPING
#define SSGUI_TEXT_WRAPPING

//namespace: ssGUI::Enums
namespace ssGUI::Enums
{
    /*enum: TextWrapping

    NO_WRAPPING         - No wrapping. Meaning the text will go outside the text widget size.
    WORD_WRAPPING       - Word wrapping. Meaning the text will wrap to next line for preventing overflowing the width of the text widget.
    CHARACTER_WRAPPING  - Character wrapping. Meaning the character will wrap to next line for preventing overflowing the width of the text widget.
    */
    enum class TextWrapping
    {
        NO_WRAPPING,
        WORD_WRAPPING,
        CHARACTER_WRAPPING
    };
}


#endif