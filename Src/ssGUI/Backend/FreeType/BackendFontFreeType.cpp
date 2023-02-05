#include "ssGUI/Backend/FreeType/BackendFontFreeType.hpp"

#include "ssLogger/ssLog.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"

#include <cmath>

namespace ssGUI
{

namespace Backend
{
    StaticDefaultWrapper<FT_Library> BackendFontFreeType::FreeTypeLib;

    bool BackendFontFreeType::SetSizeIfDifferent(float size)
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
            float lastDiff = 0;

            int chosenIndex = 0;

            //Select the closest size
            for(int i = 0; i < FontFace->num_fixed_sizes; i++)
            {
                if(FontFace->available_sizes[i].height > size)
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

                lastDiff = size - FontFace->available_sizes[i].height;
                lastIndex = i;
            }

            //If the fixed font size is not really relative close, report it
            if(lastDiff > size * 0.25)
            {
                ssLOG_LINE("Closest font size: " << (chosenIndex == lastIndex ? size + lastDiff : size - lastDiff));
                return false;
            }

            error = FT_Select_Size(FontFace, 0);
        
            //If failed to select size, just don't change anything
            if(error)
            {
                ssLOG_LINE("Failed with error: " << error);
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
                ssLOG_LINE("Failed with error: " << error);
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
                ssLOG_LINE("Failed to copy via memory");
            }
        }
        else if(!other.FontPath.empty())
        {
            if(!LoadFromPath(other.FontPath))
            {
                ssLOG_LINE("Failed to copy via path");
            }
        }
        else
        {
            ssLOG_LINE("What? This is unexpected condition, exiting...");
            ssLOG_LINE("other.IsValid(): " << other.IsValid());
            ssLOG_LINE("other.FontMemory: " << other.FontMemory);
            ssLOG_LINE("other.FontPath: " << other.FontPath);
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
                ssLOG_LINE("Failed to initialize FreeType library with error: " << error);
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

    FT_GlyphSlot BackendFontFreeType::GetCurrentGlyph()
    {
        return FontFace->glyph;
    }

    bool BackendFontFreeType::IsValid() const
    {
        return Valid;
    }

    ssGUI::CharacterRenderInfo BackendFontFreeType::GetCharacterRenderInfo(wchar_t charUnicode, float charSize)
    {
        ssGUI::CharacterRenderInfo info;
        
        if(!Valid)
        {
            info.Valid = false;
            return info;
        }
        
        if(!SetSizeIfDifferent(charSize))
        {
            ssLOG_LINE("Failed to set character size, aborting...");
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
            ssLOG_LINE("Failed to load glyph with error: " << error);
            ssLOG_LINE("Aborting...");
            info.Valid = false;
            return info;
        }

        info.Advance = static_cast<float>(FontFace->glyph->advance.x) / 64.f;
        info.DrawOffset = glm::vec2(FontFace->glyph->bitmap_left, -FontFace->glyph->bitmap_top);
        info.Size = glm::vec2(FontFace->glyph->bitmap.width, FontFace->glyph->bitmap.rows);
        info.Rendered = true;   //This is handled in Text.cpp
        info.Valid = true;
        
        return info;
    }
    
    bool BackendFontFreeType::IsCharacterSupported(wchar_t charUnicode)
    {
        return FT_Get_Char_Index(FontFace, charUnicode) != 0;
    }
    
    float BackendFontFreeType::GetKerning(wchar_t charUnicode, wchar_t secondCharUnicode, float charSize)
    {
        if(!FT_HAS_KERNING(FontFace))
            return 0;

        if(!SetSizeIfDifferent(charSize))
            ssLOG_LINE("Failed to set character size, continuing");

        FT_Vector delta;    //Delta is expressed in 1/64 of points
        FT_Error err = FT_Get_Kerning(FontFace, charUnicode, secondCharUnicode, ft_kerning_default, &delta);
        if(err)
        {
            ssLOG_LINE("Failed to get kerning");
            return 0;
        }

        return static_cast<float>(delta.x) / static_cast<float>(64); 
    }
    
    float BackendFontFreeType::GetLineSpacing(float charSize)
    {
        if(!SetSizeIfDifferent(charSize))
            ssLOG_LINE("Failed to set character size, continuing");
        
        //See https://freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_facerec
        //See https://freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_size_metrics
        return static_cast<float>( FT_MulFix(FontFace->height, FontFace->size->metrics.y_scale) ) / static_cast<float>(64);
    }
    
    float BackendFontFreeType::GetUnderlineOffset(float charSize)
    {
        if(!SetSizeIfDifferent(charSize))
            ssLOG_LINE("Failed to set character size, continuing");

        //See https://freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_facerec
        //See https://freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_size_metrics
        return std::abs(static_cast<float>( FT_MulFix(FontFace->underline_position, FontFace->size->metrics.y_scale)) ) / static_cast<float>(64);;
    }
    
    float BackendFontFreeType::GetUnderlineThickness(float charSize)
    {
        if(!SetSizeIfDifferent(charSize))
            ssLOG_LINE("Failed to set character size, continuing");

        //See https://freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_facerec
        //See https://freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_size_metrics
        return static_cast<float>( FT_MulFix(FontFace->underline_thickness, FontFace->size->metrics.y_scale) ) / static_cast<float>(64);
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
            ssLOG_LINE("Failed to laod font");
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

    bool BackendFontFreeType::LoadFromMemory(void* dataPtr, int lengthInBytes)
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
            ssLOG_LINE("Failed to laod font: "<<error);
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
    
    bool BackendFontFreeType::GetFixedAvailableFontSizes(std::vector<float>& fontSizes)
    {
        if(!Valid)
            return false;
    
        fontSizes.clear();
    
        //Select the closest size
        for(int i = 0; i < FontFace->num_fixed_sizes; i++)
            fontSizes.push_back(FontFace->available_sizes[i].height);
        
        return true;
    }

    bool BackendFontFreeType::GetCharacterImage(wchar_t charUnicode, float charSize, ssGUI::ImageData& characterImage)
    {
        if(!SetSizeIfDifferent(charSize))
            return false;
        
        bool result = false;

        FT_UInt glyphIndex = FT_Get_Char_Index(FontFace, static_cast<FT_ULong>(charUnicode));
        FT_Error error = FT_Load_Glyph(FontFace, glyphIndex, FontFlags);
        
        if(error)
        {
            ssLOG_LINE("Failed to load glyph");
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
            
            //sf::Vector2u size = sf::Vector2u(freeTypeFont->GetCurrentGlyph()->bitmap.width, freeTypeFont->GetCurrentGlyph()->bitmap.rows);
            //Convert to bitmap if there isn't one
            
            if(FontFace->glyph->format != FT_GLYPH_FORMAT_BITMAP)
            {
                error = FT_Render_Glyph(FontFace->glyph, FT_RENDER_MODE_NORMAL);
                if(error)
                {
                    ssLOG_LINE("Failed to FT_Render_Glyph");
                    return false;
                }
            }
            
            if(FontFace->glyph->bitmap.pixel_mode != FT_PIXEL_MODE_BGRA)
            {
                ssLOG_LINE("Invalid pixel mode: " << FontFace->glyph->bitmap.pixel_mode);
                return false;
            }
            
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
                    ssLOG_LINE("Failed to FT_Render_Glyph");
                    return false;
                }
            }
            
            if(FontFace->glyph->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY)
            {
                ssLOG_LINE("Invalid pixel mode: " << FontFace->glyph->bitmap.pixel_mode);
                return false;
            }
        
            result = characterImage.LoadRawFromMemory(  GetCurrentGlyph()->bitmap.buffer, 
                                                        format, 
                                                        glm::ivec2( GetCurrentGlyph()->bitmap.width, 
                                                                    GetCurrentGlyph()->bitmap.rows));
        }
        
        return result;
    }

    void* BackendFontFreeType::GetRawHandle()
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