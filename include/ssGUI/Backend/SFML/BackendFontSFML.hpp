#ifndef SSGUI_BACKEND_FONT_SFML
#define SSGUI_BACKEND_FONT_SFML

#include "ssGUI/Backend/Interfaces/BackendFontInterface.hpp"
#include "SFML/Graphics.hpp"

//namespace: ssGUI::Backend
namespace ssGUI::Backend
{
    //class: BackendFontSFML
    //For functions explainations, please see <BackendFontInterface>. Normally you don't need to deal with this class
    class BackendFontSFML : public BackendFontInterface
    {
        private:
            sf::Font Font;
            bool SFFontValid;
            BackendFontSFML& operator=(BackendFontSFML const& other);

        protected:
            BackendFontSFML(BackendFontSFML const& other);

        public:
            BackendFontSFML();
            virtual ~BackendFontSFML();
            
            //function: GetSFMLFont
            sf::Font* GetSFMLFont();
            
            //function: IsValid
            bool IsValid() override;
            
            //function: GetCharacterInfo
            ssGUI::CharacterInfo GetCharacterInfo(wchar_t charUnicode, int charSize) override;
            
            //function: IsCharacterSupported
            bool IsCharacterSupported(wchar_t charUnicode) override;
            
            //function: GetKerning
            int GetKerning(wchar_t charUnicode, wchar_t secondCharUnicode, int charSize) override;
            
            //function: GetLineSpacing
            int GetLineSpacing(int charSize) override;
            
            //function: GetUnderlineOffset
            int GetUnderlineOffset(int charSize) override;
            
            //function: GetUnderlineThickness
            int GetUnderlineThickness(int charSize) override;

            //function: LoadFromPath
            //SFML supports: TrueType, Type 1, CFF, OpenType, SFNT, X11 PCF, Windows FNT, BDF, PFR and Type 42
            bool LoadFromPath(std::string path) override;
            
            //function: LoadFromMemory
            //SFML supports: TrueType, Type 1, CFF, OpenType, SFNT, X11 PCF, Windows FNT, BDF, PFR and Type 42
            bool LoadFromMemory(void* dataPtr, int lengthInBytes) override;

            //function: Clone
            ssGUI::Backend::BackendFontInterface* Clone() override;
    };
}

#endif