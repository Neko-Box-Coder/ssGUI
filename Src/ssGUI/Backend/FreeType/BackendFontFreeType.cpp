#include "ssGUI/Backend/FreeType/BackendFontFreeType.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"

#include <cmath>

namespace ssGUI
{

namespace Backend
{
    StaticDefaultWrapper<FT_Library> BackendFontFreeType::FreeTypeLib;

    bool BackendFontFreeType::SetSizeIfDifferent(float size) const
    {
        if(CurrentSize == size)
            return true;
        
        FT_Error error = 0;

        if(!FT_IS_SCALABLE(FontFace) || FT_HAS_FIXED_SIZES(FontFace))
        {
            //This doesn't make sense, invalid font
            if(FontFace->num_fixed_sizes <= 0)
                return false;
            
            int lastIndex = 0;
            float lastDiff = 1000000000;

            int chosenIndex = 0;

            //Select the closest size
            for(int i = 0; i < FontFace->num_fixed_sizes; i++)
            {
                if(FontFace->available_sizes[i].height >= size)
                {
                    float curDiff = FontFace->available_sizes[i].height - size;
                    if(curDiff < lastDiff)
                    {
                        chosenIndex = i;
                        lastDiff = curDiff;
                        break;
                    }
                    else
                    {
                        chosenIndex = lastIndex;
                        break;
                    }
                }
                else
                {
                    float curDiff = size - FontFace->available_sizes[i].height;
                    if(lastDiff < curDiff)
                    {
                        lastDiff = curDiff;
                        lastIndex = i;
                    }
                }
            }

            //If the fixed font size is not really relative close, report it
            if(lastDiff > size * 0.25)
            {
                ssGUI_DEBUG(ssGUI_BACKEND_TAG, "Requested font size: " << size);
                ssGUI_DEBUG(ssGUI_BACKEND_TAG, "Closest font size: " << 
                            (chosenIndex == lastIndex ? size + lastDiff : size - lastDiff));
                //return false;
            }

            error = FT_Select_Size(FontFace, 0);
        
            //If failed to select size, just don't change anything
            if(error)
            {
                ssGUI_DEBUG(ssGUI_BACKEND_TAG, "Failed with error: " << error);
                return false;
            }
            
            CurrentSize = FontFace->available_sizes[chosenIndex].height;
        }
        else
        {
            //Set the height directly
            error = FT_Set_Pixel_Sizes
                    (
                        FontFace,   // handle to face object
                        0,          // pixel_width
                        size        // pixel_height
                    );
            
            //If failed to select size, just don't change anything
            if(error)
            {
                ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed with error: " << error);
                return false;
            }

            CurrentSize = size;
        }
        
        return true;
    }

    BackendFontFreeType::BackendFontFreeType(BackendFontFreeType const& other)
    {
        FontFace = nullptr;
        Valid = false;
        CurrentSize = -1;
        FontFlags = 0;
        FontMemory = nullptr;
        FontMemoryLength = 0;
        FontPath = "";
    
        //If the copy font is invalid, just reinitialize the current font
        if(!other.IsValid())
        {
            if(FontFace != nullptr)
                FT_Done_Face(FontFace);
        
            if(FontMemory != nullptr)
                free(FontMemory);   
        }
        else if(other.FontMemory != nullptr)
        {
            if(!LoadFromMemory(other.FontMemory, other.FontMemoryLength))
            {
                ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to copy via memory");
            }
        }
        else if(!other.FontPath.empty())
        {
            if(!LoadFromPath(other.FontPath))
            {
                ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to copy via path");
            }
        }
        else
        {
            ssGUI_ERROR(ssGUI_BACKEND_TAG, "What? This is unexpected condition, exiting...");
            ssGUI_ERROR(ssGUI_BACKEND_TAG, "other.IsValid(): " << other.IsValid());
            ssGUI_ERROR(ssGUI_BACKEND_TAG, "other.FontMemory: " << other.FontMemory);
            ssGUI_ERROR(ssGUI_BACKEND_TAG, "other.FontPath: " << other.FontPath);
            ssLOG_EXIT_PROGRAM();
        }
    }

    BackendFontFreeType::BackendFontFreeType() :    FontFace(nullptr),
                                                    Valid(false),
                                                    CurrentSize(-1),
                                                    FontFlags(0),
                                                    FontMemory(nullptr),
                                                    FontMemoryLength(0),
                                                    FontPath()
    {
        //First time initialization
        if(FreeTypeLib.Obj == nullptr)
        {
            FreeTypeLib.Obj = ssGUI::Factory::Create<FT_Library>();
            
            FT_Error error = FT_Init_FreeType(FreeTypeLib.Obj);
            if(error)
            {
                ssGUI_ERROR(ssGUI_BACKEND_TAG, 
                            "Failed to initialize FreeType library with error: " << error);
                
                ssLOG_EXIT_PROGRAM();
            }

            FT_Library* lib = FreeTypeLib.Obj;
            FreeTypeLib.CleanUpFunc.push_back
            (
                [lib]()
                {
                    FT_Done_FreeType(*lib);     //Clean up FreeType Library
                    ssGUI::Factory::Dispose(lib);
                }
            );
        }
    }
    
    BackendFontFreeType::~BackendFontFreeType()
    {
        if(FontFace != nullptr)
            FT_Done_Face(FontFace);
        
        if(FontMemory != nullptr)
            free(FontMemory);
    }

    FT_GlyphSlot BackendFontFreeType::GetCurrentGlyph() const
    {
        return FontFace->glyph;
    }

    bool BackendFontFreeType::IsValid() const
    {
        return Valid;
    }

    ssGUI::CharacterRenderInfo BackendFontFreeType::GetCharacterRenderInfo( char32_t charUnicode, 
                                                                            float charSize) const
    {
        ssGUI::CharacterRenderInfo info;
        
        if(!Valid)
        {
            info.Valid = false;
            return info;
        }
        
        if(!SetSizeIfDifferent(charSize))
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to set character size, aborting...");
            info.Valid = false;
            return info;
        }

        FT_UInt glyphIndex = FT_Get_Char_Index(FontFace, charUnicode);

        FT_Error error = FT_Load_Glyph
                        (
                            FontFace,           // handle to face object
                            glyphIndex,         // glyph index
                            FontFlags           // load flags
                        );

        if(error)
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to load glyph with error: " << error);
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Aborting...");
            info.Valid = false;
            return info;
        }

        //For some reason, variable selections is taking spaces. We don't want that
        //https://en.wikipedia.org/wiki/Variation_Selectors_(Unicode_block)
        if(charUnicode < U'\uFE00' || charUnicode > U'\uFE0F')
        {
            info.Advance = static_cast<float>(FontFace->glyph->advance.x) / 64.f;
            info.DrawOffset = glm::vec2(FontFace->glyph->bitmap_left, -FontFace->glyph->bitmap_top);
            info.Size = glm::vec2(FontFace->glyph->bitmap.width, FontFace->glyph->bitmap.rows);
            info.Rendered = true;   //This is handled in Text.cpp
            info.Valid = true;
            info.RenderFontSize = CurrentSize;
            info.TargetSizeMultiplier = charSize / CurrentSize; 
        }
        
        return info;
    }
    
    bool BackendFontFreeType::IsCharacterSupported(char32_t charUnicode) const
    {
        return FT_Get_Char_Index(FontFace, charUnicode) != 0;
    }
    
    float BackendFontFreeType::GetKerning(  char32_t charUnicode, 
                                            char32_t secondCharUnicode, 
                                            float charSize) const
    {
        if(!FT_HAS_KERNING(FontFace))
            return 0;

        if(!SetSizeIfDifferent(charSize))
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to set character size, continuing");

        FT_Vector delta;    //Delta is expressed in 1/64 of points
        FT_Error err = FT_Get_Kerning(  FontFace, 
                                        charUnicode, 
                                        secondCharUnicode, 
                                        ft_kerning_default, 
                                        &delta);
        
        if(err)
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to get kerning");
            return 0;
        }

        return static_cast<float>(delta.x) / static_cast<float>(64); 
    }
    
    float BackendFontFreeType::GetLineSpacing(float charSize) const
    {
        if(!SetSizeIfDifferent(charSize))
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to set character size, continuing");
        
        //See https://freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_facerec
        //See https://freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_size_metrics
        FT_Long rawLineSpace = FT_MulFix(FontFace->height, FontFace->size->metrics.y_scale);
        
        if(rawLineSpace == 0)
            return charSize;
        
        if(charSize == CurrentSize)
            return static_cast<float>(rawLineSpace) / static_cast<float>(64);
        else
        {
            return  static_cast<float>(rawLineSpace) / 
                    static_cast<float>(64) * 
                    (charSize / CurrentSize);
        }
    }
    
    float BackendFontFreeType::GetUnderlineOffset(float charSize) const
    {
        if(!SetSizeIfDifferent(charSize))
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to set character size, continuing");

        //See https://freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_facerec
        //See https://freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_size_metrics
        
        FT_Long underlineOffset = FT_MulFix(FontFace->underline_position, 
                                            FontFace->size->metrics.y_scale);
        
        if(underlineOffset == 0)
            return charSize / 5;
        
        if(charSize == CurrentSize)
            return std::abs(static_cast<float>(underlineOffset) ) / static_cast<float>(64);
        else
        {
            return  std::abs(static_cast<float>(underlineOffset) ) / 
                    static_cast<float>(64) * 
                    (charSize / CurrentSize);
        }
    }
    
    float BackendFontFreeType::GetUnderlineThickness(float charSize) const
    {
        if(!SetSizeIfDifferent(charSize))
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to set character size, continuing");

        //See https://freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_facerec
        //See https://freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_size_metrics
        
        FT_Long underlineThickness = FT_MulFix( FontFace->underline_thickness, 
                                                FontFace->size->metrics.y_scale);
        
        if(underlineThickness == 0)
            return charSize / 25;
        
        if(charSize == CurrentSize)
            return static_cast<float>(underlineThickness) / static_cast<float>(64);
        else
        {
            return  static_cast<float>(underlineThickness) / 
                    static_cast<float>(64) * 
                    (charSize / CurrentSize);
        }
    }

    bool BackendFontFreeType::LoadFromPath(std::string path)
    {
        //Discard current font
        if(FontFace != nullptr)
            FT_Done_Face(FontFace);
        
        if(FontMemory != nullptr)
        {
            free(FontMemory);
            FontMemory = nullptr;
            FontMemoryLength = 0;
        }
        
        if(!FontPath.empty())
            FontPath.clear();
        
        FontPath = path;
        
        //Load new font
        FT_Error error = 0;
        error = FT_New_Face
        ( 
            *(FreeTypeLib.Obj),
            path.c_str(),
            0,                      //First font by default
            &FontFace 
        );

        if(error)
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to laod font");
            Valid = false;
            return false;
        }

        FontFlags = FT_LOAD_FORCE_AUTOHINT | 
                    FT_LOAD_TARGET_NORMAL;
        
        if(FT_HAS_COLOR(FontFace))
            FontFlags |= FT_LOAD_COLOR;
        else
            FontFlags |= FT_LOAD_RENDER;

        CurrentSize = -1;
        Valid = true;
        FontPath = path;

        return true;
    }

    bool BackendFontFreeType::LoadFromMemory(const void* dataPtr, int lengthInBytes)
    {
        //Discard current font
        if(FontFace != nullptr)
            FT_Done_Face(FontFace);
        
        
        if(FontMemory != nullptr)
            free(FontMemory);
        
        if(!FontPath.empty())
            FontPath.clear();
        
        FontMemory = static_cast<uint8_t*>(malloc(static_cast<size_t>(lengthInBytes)));
        memcpy(FontMemory, dataPtr, lengthInBytes);
        FontMemoryLength = lengthInBytes;

        //Load new font
        FT_Error error = 0;
        error = FT_New_Memory_Face
        (
            *(FreeTypeLib.Obj),
            reinterpret_cast<FT_Byte*>(FontMemory), //first byte in memory
            lengthInBytes,                          //size in bytes
            0,                                      //face_index
            &FontFace
        );

        if(error)
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to laod font: "<<error);
            Valid = false;
            return false;
        }

        FontFlags = FT_LOAD_FORCE_AUTOHINT | 
                    FT_LOAD_TARGET_NORMAL;
        
        if(FT_HAS_COLOR(FontFace))
            FontFlags |= FT_LOAD_COLOR;
        else
            FontFlags |= FT_LOAD_RENDER;

        CurrentSize = -1;
        
        Valid = true;
        return true;
    }
    
    bool BackendFontFreeType::GetFixedAvailableFontSizes(std::vector<float>& fontSizes) const
    {
        if(!Valid)
            return false;
    
        fontSizes.clear();
    
        for(int i = 0; i < FontFace->num_fixed_sizes; i++)
            fontSizes.push_back(FontFace->available_sizes[i].height);
        
        return true;
    }

    bool BackendFontFreeType::GetCharacterImage(char32_t charUnicode, 
                                                float charSize, 
                                                BackendImageInterface& characterImage) const
    {
        if(!SetSizeIfDifferent(charSize))
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to set character size, continuing");

        bool result = false;

        FT_UInt glyphIndex = FT_Get_Char_Index(FontFace, static_cast<FT_ULong>(charUnicode));
        FT_Error error = FT_Load_Glyph(FontFace, glyphIndex, FontFlags);
        
        if(error)
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to load glyph");
            return false;
        }
        
        //If this has color, the format is BGRA with 8 bit per channel
        if(FT_HAS_COLOR(FontFace))
        {
            ssGUI::ImageFormat format;
            format.IndexB = 0;
            format.IndexG = 1;
            format.IndexR = 2;
            format.IndexA = 3;
            format.PreMultipliedAlpha = true;
            
            //Convert to bitmap if there isn't one
            if(FontFace->glyph->format != FT_GLYPH_FORMAT_BITMAP)
            {
                error = FT_Render_Glyph(FontFace->glyph, FT_RENDER_MODE_NORMAL);
                if(error)
                {
                    ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to FT_Render_Glyph");
                    return false;
                }
            }
            
            if(FontFace->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
            {
                format.ImgType = ssGUI::Enums::ImageType::MONO;
                format.BitDepthPerChannel = 1;
                format.HasAlpha = false;
                format.IndexMono = 0;
                format.BitPerPixel = 1;
            }
            else if(FontFace->glyph->bitmap.pixel_mode != FT_PIXEL_MODE_BGRA)
            {
                ssGUI_WARNING(  ssGUI_BACKEND_TAG, "Invalid pixel mode: " << 
                                (int)FontFace->glyph->bitmap.pixel_mode);
                
                return false;
            }
            
            //If it is am empty character, it shouldn't be render, so return false
            if(GetCurrentGlyph()->bitmap.width == 0 || GetCurrentGlyph()->bitmap.rows == 0)
                return false;
            
            result = characterImage.LoadRawFromMemory(  GetCurrentGlyph()->bitmap.buffer, 
                                                        format, 
                                                        glm::ivec2( GetCurrentGlyph()->bitmap.width, 
                                                                    GetCurrentGlyph()->bitmap.rows));
        }
        //Otherwise it is just grayscale
        //We will treat black pixels as transparent
        else
        {
            ssGUI::ImageFormat format;
            format.ImgType = ssGUI::Enums::ImageType::MONO;
            format.HasAlpha = true;
            format.IndexMono = 0;
            format.IndexA = -1;
            format.BitPerPixel = 8;
            format.IndexR = -1;
            format.IndexG = -1;
            format.IndexB = -1;

            //Convert to bitmap if there isn't one
            if(FontFace->glyph->format != FT_GLYPH_FORMAT_BITMAP)
            {
                //This uses RGB instead of grayscale
                 error = FT_Render_Glyph(FontFace->glyph, FT_RENDER_MODE_NORMAL);
                if(error)
                {
                    ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to FT_Render_Glyph");
                    return false;
                }
            }
            
            if(FontFace->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
            {
                format.ImgType = ssGUI::Enums::ImageType::MONO;
                format.BitDepthPerChannel = 1;
                format.HasAlpha = false;
                format.IndexMono = 0;
                format.BitPerPixel = 1;
            }
            else if(FontFace->glyph->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY)
            {
                ssGUI_WARNING(  ssGUI_BACKEND_TAG, "Invalid pixel mode: " << 
                                FontFace->glyph->bitmap.pixel_mode);
                
                return false;
            }
            
            //If it is am empty character, it shouldn't be render, so return false
            if(GetCurrentGlyph()->bitmap.width == 0 || GetCurrentGlyph()->bitmap.rows == 0)
                return false;
        
            result = characterImage.LoadRawFromMemory(  GetCurrentGlyph()->bitmap.buffer, 
                                                        format, 
                                                        glm::ivec2( GetCurrentGlyph()->bitmap.width, 
                                                                    GetCurrentGlyph()->bitmap.rows));
        }
        
        return result;
    }

    void* BackendFontFreeType::GetRawHandle() const
    {
        RawHandle.FreeTypeLib = *FreeTypeLib.Obj;
        RawHandle.FontFace = FontFace;
        RawHandle.FontFlags = FontFlags;
        return &RawHandle;
    }

    BackendFontInterface* BackendFontFreeType::Clone()
    {
        return new BackendFontFreeType(*this);
    }
}

}