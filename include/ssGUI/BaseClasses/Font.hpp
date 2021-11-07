#ifndef SSGUI_FONT
#define SSGUI_FONT

#include "ssGUI/Backend/BackendFactory.hpp"

//#include "ssGUI/Backend/Interfaces/BackendFontInterface.hpp"

//namespace: ssGUI
namespace ssGUI
{
    //class: Font
    class Font
    {
        private:
            ssGUI::Backend::BackendFontInterface* BackendFont;

            Font(Font const& other);
            Font& operator=(Font const& other);
        
        public:
            Font();
            virtual ~Font();
            
            //function: GetBackendFontInterface
            virtual ssGUI::Backend::BackendFontInterface* GetBackendFontInterface();

            //function:GetKerning
            virtual int GetKerning(uint32_t charUnicode, uint32_t secondCharUnicode, int charSize);
            
            //function: GetLineSpacing
            virtual int GetLineSpacing(int charSize);
            
            //function: GetUnderlineOffset
            virtual int GetUnderlineOffset(int charSize);
            
            //function: GetUnderlineThickness
            virtual int GetUnderlineThickness(int charSize);

            //function: Clone
            virtual Font* Clone();
    };
}


#endif