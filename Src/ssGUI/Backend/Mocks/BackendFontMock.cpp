#include "ssGUI/Backend/Mocks/BackendFontMock.hpp"

#include "ssGUI/Backend/Mocks/MockMacro.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendFontMock::BackendFontMock(BackendFontMock const& other)                                                                        
    {
        if(other.UnderlyingInterface != nullptr)
        {
            UnderlyingInterface = other.UnderlyingInterface->Clone();
            Valid = UnderlyingInterface->IsValid();
        }
        else
        {
            UnderlyingInterface = nullptr;
            Valid = other.Valid;
        }
    }

    BackendFontMock::BackendFontMock(ssGUI::Backend::BackendFontInterface* fontInterface)
    {
        UnderlyingInterface = fontInterface;
        if(UnderlyingInterface != nullptr)
            Valid = UnderlyingInterface->IsValid();
        else
            Valid = false;
    }

    BackendFontMock::~BackendFontMock()
    {
        if(UnderlyingInterface != nullptr)
            delete UnderlyingInterface;
    }
    
    bool BackendFontMock::IsValid() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(IsValid());
        return Valid;
    }

    ssGUI::CharacterRenderInfo BackendFontMock::GetCharacterRenderInfo(wchar_t charUnicode, float charSize)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetCharacterRenderInfo(charUnicode, charSize));
        return ssGUI::CharacterRenderInfo();
    }
    
    bool BackendFontMock::IsCharacterSupported(wchar_t charUnicode)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(IsCharacterSupported(charUnicode));
        return true;
    }
    
    float BackendFontMock::GetKerning(wchar_t charUnicode, wchar_t secondCharUnicode, float charSize)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetKerning(charUnicode, secondCharUnicode, charSize));
        return 0;
    }
    
    float BackendFontMock::GetLineSpacing(float charSize)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetLineSpacing(charSize));
        return 0;
    }
    
    float BackendFontMock::GetUnderlineOffset(float charSize)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetUnderlineOffset(charSize));
        return 0;
    }
    
    float BackendFontMock::GetUnderlineThickness(float charSize)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetUnderlineThickness(charSize));
        return 0;
    }

    bool BackendFontMock::LoadFromPath(std::string path)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(LoadFromPath(path));
        return true;
    }

    bool BackendFontMock::LoadFromMemory(void* dataPtr, int lengthInBytes)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(LoadFromMemory(dataPtr, lengthInBytes));
        return true;
    }
    
    bool BackendFontMock::GetFixedAvailableFontSizes(std::vector<float>& fontSizes)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetFixedAvailableFontSizes(fontSizes));
        return true;
    }
    
    bool BackendFontMock::GetCharacterImage(wchar_t charUnicode, float charSize, ssGUI::ImageData& characterImage)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetCharacterImage(charUnicode, charSize, characterImage));
        return true;
    }

    void* BackendFontMock::GetRawHandle()
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetRawHandle());
        return nullptr;
    }

    BackendFontInterface* BackendFontMock::Clone()
    {
        return new BackendFontMock(*this);
    }
}

}