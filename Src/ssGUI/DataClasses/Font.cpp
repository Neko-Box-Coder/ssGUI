#include "ssGUI/DataClasses/Font.hpp"

#include "ssGUI/Backend/BackendFactory.hpp"


namespace ssGUI
{
    Font::Font(Font const& other)
    {
        BackendFont = other.BackendFont->Clone();
    }
    
    Font::Font() :  BackendFont(nullptr)
    {
        BackendFont = ssGUI::Backend::BackendFactory::CreateBackendFontInterface();
    }

    Font::~Font()
    {
        delete BackendFont;
    }
        
    ssGUI::Backend::BackendFontInterface* Font::GetBackendFontInterface()
    {
        return BackendFont;
    }
    
    ssGUI::CharacterRenderInfo Font::GetCharacterRenderInfo(wchar_t charUnicode, float charSize)
    {
        return BackendFont->GetCharacterRenderInfo(charUnicode, charSize);
    }

    bool Font::IsCharacterSupported(wchar_t charUnicode)
    {
        return BackendFont->IsCharacterSupported(charUnicode);
    }

    int Font::GetKerning(uint32_t charUnicode, uint32_t secondCharUnicode, int charSize)
    {
        return BackendFont->GetKerning(charUnicode, secondCharUnicode, charSize);
    }
    
    int Font::GetLineSpacing(int charSize)
    {
        return BackendFont->GetLineSpacing(charSize);
    }
    
    int Font::GetUnderlineOffset(int charSize)
    {
        return BackendFont->GetUnderlineOffset(charSize);
    }
    
    int Font::GetUnderlineThickness(int charSize)
    {
        return BackendFont->GetUnderlineThickness(charSize);
    }

    bool Font::LoadFromPath(std::string path)
    {
        return BackendFont->LoadFromPath(path);
    }
    
    bool Font::LoadFromMemory(void* dataPtr, int lengthInBytes)
    {
        return BackendFont->LoadFromMemory(dataPtr, lengthInBytes);
    }
    
    bool Font::GetFixedAvailableFontSizes(std::vector<float>& fontSizes)
    {
        return BackendFont->GetFixedAvailableFontSizes(fontSizes);
    }
    
    bool Font::GetCharacterImage(wchar_t charUnicode, float charSize, ssGUI::ImageData& characterImage)
    {
        return BackendFont->GetCharacterImage(charUnicode, charSize, characterImage);
    }
    
    void* Font::GetRawHandle()
    {
        return BackendFont->GetRawHandle();
    }

    Font* Font::Clone()
    {
        return new Font(*this);
    }

}
    


