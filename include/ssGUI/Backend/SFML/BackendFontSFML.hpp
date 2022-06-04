#ifndef SSGUI_BACKEND_FONT_SFML
#define SSGUI_BACKEND_FONT_SFML

#include "ssGUI/Backend/Interfaces/BackendFontInterface.hpp"
#include "SFML/Graphics.hpp"

//namespace: ssGUI::Backend
namespace ssGUI::Backend
{
    /*class: ssGUI::Backend::BackendFontSFML
    For functions explainations, please see <BackendFontInterface>. Normally you don't need to deal with this class

    Variables & Constructor:
    =============================== C++ ===============================
    private:
        sf::Font Font;
        bool SFFontValid;
    ===================================================================
    =============================== C++ ===============================
    BackendFontSFML::BackendFontSFML() : Font(), SFFontValid(false)
    {
    }
    ===================================================================
    */
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
            //Gets the pointer of sf::font object
            sf::Font* GetSFMLFont();
            
            //function: IsValid
            //See <BackendFontInterface::IsValid>
            bool IsValid() override;
            
            //function: GetCharacterRenderInfo
            //See <BackendFontInterface::GetCharacterRenderInfo>
            ssGUI::CharacterRenderInfo GetCharacterRenderInfo(wchar_t charUnicode, int charSize) override;
            
            //function: IsCharacterSupported
            //See <BackendFontInterface::IsCharacterSupported>
            bool IsCharacterSupported(wchar_t charUnicode) override;
            
            //function: GetKerning
            //See <BackendFontInterface::GetKerning>
            int GetKerning(wchar_t charUnicode, wchar_t secondCharUnicode, int charSize) override;
            
            //function: GetLineSpacing
            //See <BackendFontInterface::GetLineSpacing>
            int GetLineSpacing(int charSize) override;
            
            //function: GetUnderlineOffset
            //See <BackendFontInterface::GetUnderlineOffset>
            int GetUnderlineOffset(int charSize) override;
            
            //function: GetUnderlineThickness
            //See <BackendFontInterface::GetUnderlineThickness>
            int GetUnderlineThickness(int charSize) override;

            //function: LoadFromPath
            //See <BackendFontInterface::LoadFromPath>
            //SFML supports: TrueType, Type 1, CFF, OpenType, SFNT, X11 PCF, Windows FNT, BDF, PFR and Type 42
            bool LoadFromPath(std::string path) override;
            
            //function: LoadFromMemory
            //See <BackendFontInterface::LoadFromMemory>
            //SFML supports: TrueType, Type 1, CFF, OpenType, SFNT, X11 PCF, Windows FNT, BDF, PFR and Type 42
            bool LoadFromMemory(void* dataPtr, int lengthInBytes) override;

            //function: GetRawHandle
            //See <BackendFontInterface::GetRawHandle>
            void* GetRawHandle() override;

            //function: Clone
            //See <BackendFontInterface::Clone>
            ssGUI::Backend::BackendFontInterface* Clone() override;
    };
}

#endif