#include "ssGUI/Backend/Mocks/BackendFontMock.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"
#include "ssGUI/HelperClasses/OutputStreamUtil.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendFontMock::BackendFontMock(BackendFontMock const& other)                                                                        
    {
        if(other.UnderlyingInterface != nullptr)
            UnderlyingInterface = other.UnderlyingInterface->Clone();
        else
            UnderlyingInterface = nullptr;
    }

    BackendFontMock::BackendFontMock(ssGUI::Backend::BackendFontInterface* fontInterface) : UnderlyingInterface(fontInterface)
    {}

    BackendFontMock::~BackendFontMock()
    {
        if(UnderlyingInterface != nullptr)
            delete UnderlyingInterface;
    }
    
    bool BackendFontMock::IsValid() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND_WITHOUT_ARGS(OverrideObject, &BackendFontMock::IsValid, bool);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(IsValid());
        return true;
    }

    ssGUI::CharacterRenderInfo BackendFontMock::GetCharacterRenderInfo(wchar_t charUnicode, float charSize)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, &BackendFontMock::GetCharacterRenderInfo, ssGUI::CharacterRenderInfo, charUnicode, charSize);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetCharacterRenderInfo(charUnicode, charSize));
        return ssGUI::CharacterRenderInfo();
    }
    
    bool BackendFontMock::IsCharacterSupported(wchar_t charUnicode)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, &BackendFontMock::IsCharacterSupported, bool, charUnicode);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(IsCharacterSupported(charUnicode));
        return true;
    }
    
    float BackendFontMock::GetKerning(wchar_t charUnicode, wchar_t secondCharUnicode, float charSize)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, &BackendFontMock::GetKerning, float, charUnicode, secondCharUnicode, charSize);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetKerning(charUnicode, secondCharUnicode, charSize));
        return 0;
    }
    
    float BackendFontMock::GetLineSpacing(float charSize)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, &BackendFontMock::GetLineSpacing, float, charSize);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetLineSpacing(charSize));
        return 0;
    }
    
    float BackendFontMock::GetUnderlineOffset(float charSize)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, &BackendFontMock::GetUnderlineOffset, float, charSize);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetUnderlineOffset(charSize));
        return 0;
    }
    
    float BackendFontMock::GetUnderlineThickness(float charSize)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, &BackendFontMock::GetUnderlineThickness, float, charSize);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetUnderlineThickness(charSize));
        return 0;
    }

    bool BackendFontMock::LoadFromPath(std::string path)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, &BackendFontMock::LoadFromPath, bool, path);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(LoadFromPath(path));
        return true;
    }

    bool BackendFontMock::LoadFromMemory(void* dataPtr, int lengthInBytes)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, &BackendFontMock::LoadFromMemory, bool, dataPtr, lengthInBytes);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(LoadFromMemory(dataPtr, lengthInBytes));
        return true;
    }
    
    bool BackendFontMock::GetFixedAvailableFontSizes(std::vector<float>& fontSizes)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_ARGUMENTS_AND_RETURN_IF_FOUND(true, OverrideObject, &BackendFontMock::GetFixedAvailableFontSizes, fontSizes);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetFixedAvailableFontSizes(fontSizes));
        return true;
    }
    
    bool BackendFontMock::GetCharacterImage(wchar_t charUnicode, float charSize, ssGUI::ImageData& characterImage)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        auto anyWrapper = SimpleOverride::NonCopyable<ssGUI::ImageData>(characterImage);
        FO_ARGUMENTS_AND_RETURN_IF_FOUND(   true, OverrideObject, &BackendFontMock::GetCharacterImage, charUnicode, charSize, anyWrapper);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetCharacterImage(charUnicode, charSize, characterImage));
        return true;
    }

    void* BackendFontMock::GetRawHandle()
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND_WITHOUT_ARGS(OverrideObject, &BackendFontMock::GetRawHandle, void*);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetRawHandle());
        return nullptr;
    }

    BackendFontInterface* BackendFontMock::Clone()
    {
        return new BackendFontMock(*this);
    }
}

}