#ifndef SSGUI_FONT_HPP
#define SSGUI_FONT_HPP

#include "ssGUI/Backend/Interfaces/FontInterface.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"

//TODO(NOW): Convert wchar_t to uint32_t

//namespace: ssGUI
namespace ssGUI
{
    /*class: ssGUI::Font
    
    Variables & Constructor:
    ============================== C++ ==============================
    private:
        ssGUI::Backend::FontInterface* BackendFont;  //See <GetFontInterface>
    =================================================================
    ============================== C++ ==============================
    Font::Font()
    {
        BackendFont = ssGUI::Backend::Factory::CreateFontInterface();
    }
    =================================================================
    */
    class Font
    {
        private:
            ssGUI::Backend::FontInterface* BackendFont;  //See <GetFontInterface>

            Font(Font const& other);
            Font& operator=(Font const& other);
        
        public:
            Font();
            virtual ~Font();
            
            //function: GetFontInterface
            virtual ssGUI::Backend::FontInterface* GetFontInterface();

            //function: GetCharacterRenderInfo
            //See <ssGUI::Backend::FontInterface::GetCharacterRenderInfo>
            virtual ssGUI::CharacterRenderInfo GetCharacterRenderInfo(  wchar_t charUnicode, 
                                                                        float charSize);

            //function: IsCharacterSupported
            //See <ssGUI::Backend::FontInterface::IsCharacterSupported>
            virtual bool IsCharacterSupported(wchar_t charUnicode);

            //function: GetKerning
            //See <ssGUI::Backend::FontInterface::GetKerning>
            virtual int GetKerning(uint32_t charUnicode, uint32_t secondCharUnicode, int charSize);
            
            //function: GetLineSpacing
            //See <ssGUI::Backend::FontInterface::GetLineSpacing>
            virtual int GetLineSpacing(int charSize);
            
            //function: GetUnderlineOffset
            //See <ssGUI::Backend::FontInterface::GetUnderlineOffset>
            virtual int GetUnderlineOffset(int charSize);
            
            //function: GetUnderlineThickness
            //See <ssGUI::Backend::FontInterface::GetUnderlineThickness>
            virtual int GetUnderlineThickness(int charSize);

            //function: LoadFromPath
            //See <ssGUI::Backend::FontInterface::LoadFromPath>
            virtual bool LoadFromPath(std::string path);
            
            //TODO(NOW): Convert void* to const void*
            
            //function: LoadFromMemory
            //See <ssGUI::Backend::FontInterface::LoadFromMemory>
            virtual bool LoadFromMemory(void* dataPtr, int lengthInBytes);
            
            //function: GetFixedAvailableFontSizes
            //See <ssGUI::Backend::FontInterface::GetFixedAvailableFontSizes>
            virtual bool GetFixedAvailableFontSizes(std::vector<float>& fontSizes);
            
            //function: GetCharacterImage
            //See <ssGUI::Backend::FontInterface::GetCharacterImage>
            virtual bool GetCharacterImage( wchar_t charUnicode, 
                                            float charSize, 
                                            ImageData& characterImage);
            
            //function: GetRawHandle
            //See <ssGUI::Backend::FontInterface::GetRawHandle>
            virtual void* GetRawHandle();

            //function: Clone
            //Clones the font object
            virtual Font* Clone();
    };
}


#endif