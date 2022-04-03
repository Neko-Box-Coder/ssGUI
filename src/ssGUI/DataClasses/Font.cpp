#include "ssGUI/DataClasses/Font.hpp"

namespace ssGUI
{
    Font::Font(Font const& other)
    {
        BackendFont = other.BackendFont->Clone();
    }
    
    Font::Font() : BackendFont()
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

    Font* Font::Clone()
    {
        return new Font(*this);
    }

}
    


