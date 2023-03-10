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

            //function: GetCharacterRenderInfo
            //See <ssGUI::Backend::BackendFontInterface::GetCharacterRenderInfo>
            virtual ssGUI::CharacterRenderInfo GetCharacterRenderInfo(wchar_t charUnicode, float charSize);

            //function: IsCharacterSupported
            //See <ssGUI::Backend::BackendFontInterface::IsCharacterSupported>
            virtual bool IsCharacterSupported(wchar_t charUnicode);

            //function: GetKerning
            //See <ssGUI::Backend::BackendFontInterface::GetKerning>
            virtual int GetKerning(uint32_t charUnicode, uint32_t secondCharUnicode, int charSize);
            
            //function: GetLineSpacing
            //See <ssGUI::Backend::BackendFontInterface::GetLineSpacing>
            virtual int GetLineSpacing(int charSize);
            
            //function: GetUnderlineOffset
            //See <ssGUI::Backend::BackendFontInterface::GetUnderlineOffset>
            virtual int GetUnderlineOffset(int charSize);
            
            //function: GetUnderlineThickness
            //See <ssGUI::Backend::BackendFontInterface::GetUnderlineThickness>
            virtual int GetUnderlineThickness(int charSize);

            //function: LoadFromPath
            //See <ssGUI::Backend::BackendFontInterface::LoadFromPath>
            virtual bool LoadFromPath(std::string path);
            
            //function: LoadFromMemory
            //See <ssGUI::Backend::BackendFontInterface::LoadFromMemory>
            virtual bool LoadFromMemory(void* dataPtr, int lengthInBytes);
            
            //function: GetFixedAvailableFontSizes
            //See <ssGUI::Backend::BackendFontInterface::GetFixedAvailableFontSizes>
            virtual bool GetFixedAvailableFontSizes(std::vector<float>& fontSizes);
            
            //function: GetCharacterImage
            //See <ssGUI::Backend::BackendFontInterface::GetCharacterImage>
            virtual bool GetCharacterImage(wchar_t charUnicode, float charSize, ssGUI::ImageData& characterImage);
            
            //function: GetRawHandle
            //See <ssGUI::Backend::BackendFontInterface::GetRawHandle>
            virtual void* GetRawHandle();

            //function: Clone
            //Clones the font object
            virtual Font* Clone();
    };
}


#endif