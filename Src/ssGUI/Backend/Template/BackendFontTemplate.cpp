#include "ssGUI/Backend/Template/BackendFontTemplate.hpp"

#include "ssLogger/ssLog.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendFontTemplate::BackendFontTemplate(BackendFontTemplate const& other)
    {
    
    }

    BackendFontTemplate::BackendFontTemplate()
    {
        ssLOG_LINE("BackendFontTemplate is being created, is this intended?");
    }
    
    BackendFontTemplate::~BackendFontTemplate()
    {

    }
    
    bool BackendFontTemplate::IsValid() const
    {
        return true;
    }

    ssGUI::CharacterRenderInfo BackendFontTemplate::GetCharacterRenderInfo(wchar_t charUnicode, float charSize)
    {
        return ssGUI::CharacterRenderInfo();
    }
    
    bool BackendFontTemplate::IsCharacterSupported(wchar_t charUnicode)
    {
        return true;
    }
    
    float BackendFontTemplate::GetKerning(wchar_t charUnicode, wchar_t secondCharUnicode, float charSize)
    {
        return 0;
    }
    
    float BackendFontTemplate::GetLineSpacing(float charSize)
    {
        return 0;
    }
    
    float BackendFontTemplate::GetUnderlineOffset(float charSize)
    {
        return 0;
    }
    
    float BackendFontTemplate::GetUnderlineThickness(float charSize)
    {
        return 0;
    }

    bool BackendFontTemplate::LoadFromPath(std::string path)
    {
        return true;
    }

    bool BackendFontTemplate::LoadFromMemory(void* dataPtr, int lengthInBytes)
    {
        return true;
    }
    
    bool BackendFontTemplate::GetFixedAvailableFontSizes(std::vector<float>& fontSizes)
    {
        return true;
    }
    
    bool BackendFontTemplate::GetCharacterImage(wchar_t charUnicode, float charSize, ssGUI::ImageData& characterImage)
    {
        return true;
    }

    void* BackendFontTemplate::GetRawHandle()
    {
        return nullptr;
    }

    BackendFontInterface* BackendFontTemplate::Clone()
    {
        return nullptr;
    }
}

}