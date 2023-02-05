#ifndef SSGUI_FONT_H
#define SSGUI_FONT_H

#include "ssGUI/Backend/Interfaces/BackendFontInterface.hpp"

//namespace: ssGUI
namespace ssGUI
{
    /*class: ssGUI::Font
    
    Variables & Constructor:
    ============================== C++ ==============================
    private:
        ssGUI::Backend::BackendFontInterface* BackendFont;  //See <GetBackendFontInterface>
    =================================================================
    ============================== C++ ==============================
    Font::Font()
    {
        BackendFont = ssGUI::Backend::BackendFactory::CreateBackendFontInterface();
    }
    =================================================================
    */
    class Font
    {
        private:
            ssGUI::Backend::BackendFontInterface* BackendFont;  //See <GetBackendFontInterface>

            Font(Font const& other);
            Font& operator=(Font const& other);
        
        public:
            Font();
            virtual ~Font();
            
            //function: GetBackendFontInterface
            virtual ssGUI::Backend::BackendFontInterface* GetBackendFontInterface();

            //function: IsCharacterSupported
            //Returns true if the character is supported by the font
            virtual bool IsCharacterSupported(wchar_t charUnicode);

            //function:GetKerning
            //Kerning represents the drawing position offset when drawn after certain characters. (i.e. AV has a negative kerning for the characte V)
            virtual int GetKerning(uint32_t charUnicode, uint32_t secondCharUnicode, int charSize);
            
            //function: GetLineSpacing
            virtual int GetLineSpacing(int charSize);
            
            //function: GetUnderlineOffset
            virtual int GetUnderlineOffset(int charSize);
            
            //function: GetUnderlineThickness
            virtual int GetUnderlineThickness(int charSize);

            //TODO: Proxy functions to BackendFontInterface

            //function: Clone
            virtual Font* Clone();
    };
}


#endif