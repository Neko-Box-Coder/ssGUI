#ifndef SSGUI_BACKEND_FONT_FREE_TYPE_H
#define SSGUI_BACKEND_FONT_FREE_TYPE_H

#include "ssGUI/Backend/Interfaces/BackendFontInterface.hpp"

#include "ft2build.h" 
#include "ssGUI/HelperClasses/StaticDefaultWrapper.hpp"
#include FT_FREETYPE_H

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //struct: FreeTypeHandles
    struct FreeTypeHandles
    {
        //All of the FT types are actually just pointers internally

        //var: FreeTypeLib
        FT_Library FreeTypeLib = nullptr;
        
        //var: FontFace
        FT_Face FontFace = nullptr;
        
        //var: FontFlags
        FT_Int32 FontFlags = 0;
    };

    //class: ssGUI::Backend::BackendFontFreeType
    class BackendFontFreeType : public BackendFontInterface
    {   
        private:
            static StaticDefaultWrapper<FT_Library> FreeTypeLib;

            FT_Face FontFace;
            bool Valid;
            float CurrentSize;
            FT_Int32 FontFlags;

            FreeTypeHandles RawHandle;

            uint8_t* FontMemory;
            size_t FontMemoryLength;
            std::string FontPath;
            

            bool SetSizeIfDifferent(float size);
            BackendFontFreeType& operator=(BackendFontFreeType const& other);

        protected:
            BackendFontFreeType(BackendFontFreeType const& other);

        public:
            BackendFontFreeType();
            ~BackendFontFreeType() override;
            
            //function: GetCurrentGlyph
            //This returns the loaded freetype glyph
            FT_GlyphSlot GetCurrentGlyph();

            //function: IsValid
            //See <BackendFontInterface::IsValid>
            bool IsValid() const override;

            //function: GetCharacterRenderInfo
            //See <BackendFontInterface::GetCharacterRenderInfo>
            ssGUI::CharacterRenderInfo GetCharacterRenderInfo(wchar_t charUnicode, float charSize) override;
            
            //function: IsCharacterSupported
            //See <BackendFontInterface::IsCharacterSupported>
            bool IsCharacterSupported(wchar_t charUnicode) override;
            
            //function: GetKerning
            //See <BackendFontInterface::GetKerning>
            float GetKerning(wchar_t charUnicode, wchar_t secondCharUnicode, float charSize) override;
            
            //function: GetLineSpacing
            //See <BackendFontInterface::GetLineSpacing>
            float GetLineSpacing(float charSize) override;
            
            //function: GetUnderlineOffset
            //See <BackendFontInterface::GetUnderlineOffset>
            float GetUnderlineOffset(float charSize) override;
            
            //function: GetUnderlineThickness
            //See <BackendFontInterface::GetUnderlineThickness>
            float GetUnderlineThickness(float charSize) override;

            //function: LoadFromPath
            //See <BackendFontInterface::LoadFromPath>
            bool LoadFromPath(std::string path) override;

            //function: LoadFromMemory
            //This mainly supports and tested on TTF, other font formats are not tested.
            //See <BackendFontInterface::LoadFromMemory>
            bool LoadFromMemory(void* dataPtr, int lengthInBytes) override;

            //function: GetFixedAvailableFontSizes
            //See <BackendFontInterface::GetFixedAvailableFontSizes>
            bool GetFixedAvailableFontSizes(std::vector<float>& fontSizes) override;

            //function: GetCharacterImage
            //See <BackendFontInterface::GetCharacterImage>
            bool GetCharacterImage(wchar_t charUnicode, float charSize, ssGUI::ImageData& characterImage) override;

            //function: GetRawHandle
            void* GetRawHandle() override;

            //function: Clone
            //Clones the backend font object
            //Seems like freetype currently doesn't support font face cloning
            //https://gitlab.freedesktop.org/freetype/freetype/-/issues/1040
            //Therefore, this will just load the same font instead of copying
            //See <BackendFontInterface::Clone>
            BackendFontInterface* Clone() override;
    };   
}

}


#endif