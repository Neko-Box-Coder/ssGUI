#include "ssGUI/Backend/Template/BackendFontTemplate.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendFontTemplate::BackendFontTemplate(BackendFontTemplate const& other)
    {
    
    }

    BackendFontTemplate::BackendFontTemplate()
    {
        ssGUI_WARNING(ssGUI_BACKEND_TAG, "BackendFontTemplate is being created, is this intended?");
    }
    
    BackendFontTemplate::~BackendFontTemplate()
    {

    }
    
    bool BackendFontTemplate::IsValid() const
    {
        return true;
    }

    CharacterRenderInfo BackendFontTemplate::GetCharacterRenderInfo(char32_t charUnicode, 
                                                                    float charSize) const
    {
        return CharacterRenderInfo();
    }
    
    bool BackendFontTemplate::IsCharacterSupported(char32_t charUnicode) const
    {
        return true;
    }
    
    float BackendFontTemplate::GetKerning(  char32_t charUnicode, 
                                            char32_t secondCharUnicode, 
                                            float charSize) const
    {
        return 0;
    }
    
    float BackendFontTemplate::GetLineSpacing(float charSize) const
    {
        return 0;
    }
    
    float BackendFontTemplate::GetUnderlineOffset(float charSize) const
    {
        return 0;
    }
    
    float BackendFontTemplate::GetUnderlineThickness(float charSize) const
    {
        return 0;
    }

    bool BackendFontTemplate::LoadFromPath(std::string path)
    {
        return true;
    }

    bool BackendFontTemplate::LoadFromMemory(const void* dataPtr, int lengthInBytes)
    {
        return true;
    }
    
    bool BackendFontTemplate::GetFixedAvailableFontSizes(std::vector<float>& fontSizes) const
    {
        return true;
    }
    
    bool BackendFontTemplate::GetCharacterImage(char32_t charUnicode, 
                                                float charSize, 
                                                BackendImageInterface& characterImage) const
    {
        return true;
    }

    void* BackendFontTemplate::GetRawHandle() const
    {
        return nullptr;
    }

    BackendFontInterface* BackendFontTemplate::Clone()
    {
        return nullptr;
    }
}

}