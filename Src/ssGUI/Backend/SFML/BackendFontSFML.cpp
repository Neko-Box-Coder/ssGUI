#include "ssGUI/Backend/SFML/BackendFontSFML.hpp"

#include "ssLogger/ssLog.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendFontSFML::BackendFontSFML(BackendFontSFML const& other)
    {
        Font = sf::Font(other.Font);
        SFFontValid = other.SFFontValid;
    }
    
    BackendFontSFML::BackendFontSFML() :    Font(),
                                            SFFontValid(false)
    {
    }
    BackendFontSFML::~BackendFontSFML()
    {
    }

    sf::Font* BackendFontSFML::GetSFMLFont()
    {
        return SFFontValid ? &Font : nullptr;
    }

    bool BackendFontSFML::IsValid() const
    {
        return SFFontValid;
    }

    ssGUI::CharacterRenderInfo BackendFontSFML::GetCharacterRenderInfo(wchar_t charUnicode, float charSize)
    {
        ssGUI::CharacterRenderInfo info;

        if(!SFFontValid)
            return info;
        
        sf::Glyph glyph = Font.getGlyph(charUnicode, charSize, false);

        info.Advance = glyph.advance;
        info.DrawOffset = glm::vec2(glyph.bounds.left, glyph.bounds.top);
        info.Size = glm::vec2(glyph.textureRect.width, glyph.textureRect.height);
        info.Rendered = true;   //This is handled in Text.cpp
        info.Valid = true;

        return info;
    }

    bool BackendFontSFML::IsCharacterSupported(wchar_t charUnicode)
    {
        if(!SFFontValid)
            return false;

        return Font.hasGlyph(charUnicode);   
    }

    float BackendFontSFML::GetKerning(wchar_t charUnicode, wchar_t secondCharUnicode, float charSize)
    {
        if(!SFFontValid)
            return -1;

        return Font.getKerning(charUnicode, secondCharUnicode, charSize);
    }
    
    float BackendFontSFML::GetLineSpacing(float charSize)
    {
        if(!SFFontValid)
            return -1;
        
        return Font.getLineSpacing(charSize);
    }
    
    float BackendFontSFML::GetUnderlineOffset(float charSize)
    {
        if(!SFFontValid)
            return -1;
        
        return Font.getUnderlinePosition(charSize);
    }

    float BackendFontSFML::GetUnderlineThickness(float charSize)
    {
        if(!SFFontValid)
            return -1;
        
        return Font.getUnderlineThickness(charSize);
    }

    bool BackendFontSFML::LoadFromPath(std::string path)
    {
        if(Font.loadFromFile(path))
        {
            SFFontValid = true;
            return true;
        }
        else
            return false;
    }

    bool BackendFontSFML::LoadFromMemory(void* dataPtr, int lengthInBytes)
    {
        if(Font.loadFromMemory(dataPtr, lengthInBytes))
        {
            SFFontValid = true;
            return true;
        }
        else
            return false;
    }

    bool BackendFontSFML::GetFixedAvailableFontSizes(std::vector<float>& fontSizes)
    {
        if(!SFFontValid)
            return false;
            
        return true;
    }
    
    bool BackendFontSFML::GetCharacterImage(wchar_t charUnicode, float charSize, ssGUI::ImageData& characterImage)
    {
        if(!SFFontValid)
            return false;
        
        sf::Glyph glyph = Font.getGlyph(charUnicode, charSize, false);
        sf::Texture characterTextAtlas = Font.getTexture(charSize);
        sf::Image characterAtlasImg = characterTextAtlas.copyToImage();
        const uint8_t* charTexture =    characterAtlasImg.getPixelsPtr() + 
                                        (glyph.textureRect.getPosition().y * characterAtlasImg.getSize().x + 
                                        glyph.textureRect.getPosition().x) * 4;
        
        ssGUI::ImageFormat format;
        
        return characterImage.LoadRawFromMemory(charTexture, ssGUI::ImageFormat(), glm::ivec2(glyph.textureRect.width, glyph.textureRect.height));   
    }

    void* BackendFontSFML::GetRawHandle()
    {
        return &Font;
    }

    ssGUI::Backend::BackendFontInterface* BackendFontSFML::Clone()
    {
        return new BackendFontSFML(*this);
    }

}

}