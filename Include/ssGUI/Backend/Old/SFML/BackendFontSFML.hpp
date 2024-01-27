#ifndef SSGUI_BACKEND_FONT_SFML_H
#define SSGUI_BACKEND_FONT_SFML_H

#include "ssGUI/Backend/Interfaces/FontInterface.hpp"
#include "SFML/Graphics.hpp"

namespace ssGUI
{ 
    
//namespace: ssGUI::Backend
namespace Backend
{
    /*class: ssGUI::Backend::BackendFontSFML
    For functions explainations, please see <FontInterface>. Normally you don't need to deal with this class

    Variables & Constructor:
    =============================== C++ ===============================
    private:
        sf::Font Font;      //See <GetRawHandle>
        bool SFFontValid;   //See <IsValid>
    ===================================================================
    =============================== C++ ===============================
    BackendFontSFML::BackendFontSFML() :    Font(),
                                            SFFontValid(false)
    {
    }
    ===================================================================
    */
    class BackendFontSFML : public FontInterface
    {
        private:
            sf::Font Font;      //See <GetRawHandle>
            bool SFFontValid;   //See <IsValid>

            BackendFontSFML& operator=(BackendFontSFML const& other);

        protected:
            BackendFontSFML(BackendFontSFML const& other);

        public:
            BackendFontSFML();
            ~BackendFontSFML() override;
            
            //function: GetSFMLFont
            //Gets the pointer of sf::font object
            sf::Font* GetSFMLFont();
            
            //function: IsValid
            //See <FontInterface::IsValid>
            bool IsValid() const override;
            
            //function: GetCharacterRenderInfo
            //See <FontInterface::GetCharacterRenderInfo>
            ssGUI::CharacterRenderInfo GetCharacterRenderInfo(wchar_t charUnicode, float charSize) override;
            
            //function: IsCharacterSupported
            //See <FontInterface::IsCharacterSupported>
            bool IsCharacterSupported(wchar_t charUnicode) override;
            
            //function: GetKerning
            //For fixed size font, this is undefined.
            //See <FontInterface::GetKerning>
            float GetKerning(wchar_t charUnicode, wchar_t secondCharUnicode, float charSize) override;
            
            //function: GetLineSpacing
            //For fixed size font, this is undefined.
            //See <FontInterface::GetLineSpacing>
            float GetLineSpacing(float charSize) override;
            
            //function: GetUnderlineOffset
            //For fixed size font, this is undefined.
            //See <FontInterface::GetUnderlineOffset>
            float GetUnderlineOffset(float charSize) override;
            
            //function: GetUnderlineThickness
            //For fixed size font, this is undefined.
            //See <FontInterface::GetUnderlineThickness>
            float GetUnderlineThickness(float charSize) override;

            //function: LoadFromPath
            //See <FontInterface::LoadFromPath>
            //SFML supports: TrueType, Type 1, CFF, OpenType, SFNT, X11 PCF, Windows FNT, BDF, PFR and Type 42
            bool LoadFromPath(std::string path) override;
            
            //function: LoadFromMemory
            //See <FontInterface::LoadFromMemory>
            //SFML supports: TrueType, Type 1, CFF, OpenType, SFNT, X11 PCF, Windows FNT, BDF, PFR and Type 42
            bool LoadFromMemory(void* dataPtr, int lengthInBytes) override;

            //function: GetFixedAvailableFontSizes
            //SFML doesn't support this, calling this function will have no effect.
            bool GetFixedAvailableFontSizes(std::vector<float>& fontSizes) override;

            //function: GetCharacterImage
            //See <FontInterface::GetCharacterImage>
            bool GetCharacterImage(wchar_t charUnicode, float charSize, ssGUI::ImageData& characterImage) override;

            //function: GetRawHandle
            //See <FontInterface::GetRawHandle>
            void* GetRawHandle() override;

            //function: Clone
            //See <FontInterface::Clone>
            ssGUI::Backend::FontInterface* Clone() override;
    };
}

}

#endif