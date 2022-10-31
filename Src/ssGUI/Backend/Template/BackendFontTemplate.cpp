#include "ssGUI/Backend/Template/BackendFontTemplate.hpp"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    BackendFontTemplate::BackendFontTemplate()
    {

    }
    
    BackendFontTemplate::~BackendFontTemplate()
    {

    }
    
    bool BackendFontTemplate::IsValid()
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