#ifndef SSGUI_BACKEND_FONT_FREE_TYPE_HPP
#define SSGUI_BACKEND_FONT_FREE_TYPE_HPP

#include "ssGUI/Backend/Interfaces/FontInterface.hpp"

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

    //class: ssGUI::Backend::FontFreeType
    class FontFreeType : public FontInterface
    {   
        private:
            //static StaticDefaultWrapper<FT_Library> FreeTypeLib;
            static FT_Library FreeTypeLib;

            FT_Face FontFace;
            bool Valid;
            mutable float CurrentSize;
            FT_Int32 FontFlags;

            mutable FreeTypeHandles RawHandle;

            uint8_t* FontMemory;
            size_t FontMemoryLength;
            std::string FontPath;
            

            bool SetSizeIfDifferent(float size) const;
            FontFreeType& operator=(FontFreeType const& other);

        protected:
            FontFreeType(FontFreeType const& other);

        public:
            FontFreeType();
            ~FontFreeType() override;
            
            //function: GetCurrentGlyph
            //This returns the loaded freetype glyph
            FT_GlyphSlot GetCurrentGlyph() const;

            static bool InitializeFreeType();
            static bool CleanupFreeType();

            //function: Initialize
            virtual bool Initialize() override;

            //function: IsValid
            //See <FontInterface::IsValid>
            bool IsValid() const override;

            //function: GetCharacterRenderInfo
            //See <FontInterface::GetCharacterRenderInfo>
            CharacterRenderInfo GetCharacterRenderInfo( char32_t charUnicode, 
                                                        float charSize) const override;
            
            //function: IsCharacterSupported
            //For fixed size font, this will return the closest fixed size font 
            //and populate the TargetSizeMultiplier field.
            //
            //See <FontInterface::IsCharacterSupported>
            bool IsCharacterSupported(char32_t charUnicode) const override;
            
            //function: GetKerning
            //See <FontInterface::GetKerning>
            float GetKerning(   char32_t charUnicode, 
                                char32_t secondCharUnicode, 
                                float charSize) const override;
            
            //function: GetLineSpacing
            //For fixed size font, 
            //this will return always return the corresponding line spacing to charSize
            //See <FontInterface::GetLineSpacing>
            float GetLineSpacing(float charSize) const override;
            
            //function: GetUnderlineOffset
            //See <FontInterface::GetUnderlineOffset>
            //For fixed size font, 
            //this will return always return the corresponding underline offset to charSize
            float GetUnderlineOffset(float charSize) const override;
            
            //function: GetUnderlineThickness
            //For fixed size font, 
            //this will return always return the corresponding underline thickness to charSize
            //See <FontInterface::GetUnderlineThickness>
            float GetUnderlineThickness(float charSize) const override;

            //function: LoadFromPath
            //See <FontInterface::LoadFromPath>
            bool LoadFromPath(std::string path) override;

            //function: LoadFromMemory
            //This mainly supports and tested on TTF, other font formats are not tested.
            //See <FontInterface::LoadFromMemory>
            bool LoadFromMemory(const void* dataPtr, int lengthInBytes) override;

            //function: GetFixedAvailableFontSizes
            //See <FontInterface::GetFixedAvailableFontSizes>
            bool GetFixedAvailableFontSizes(std::vector<float>& fontSizes) const override;

            //function: GetCharacterImage
            //See <FontInterface::GetCharacterImage>
            bool GetCharacterImage( char32_t charUnicode, 
                                    float charSize, 
                                    ImageInterface& characterImage) const override;

            //function: GetRawHandle
            void* GetRawHandle() const override;

            //function: Clone
            //Clones the backend font object
            //Seems like freetype currently doesn't support font face cloning
            //https://gitlab.freedesktop.org/freetype/freetype/-/issues/1040
            //
            //Therefore, this will just load the same font instead of copying
            //See <FontInterface::Clone>
            FontInterface* Clone() override;
    };   
}

}


#endif