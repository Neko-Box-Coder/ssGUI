#include "ssGUI/Backend/Mocks/BackendFontMock.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"
#include "ssGUI/HelperClasses/OutputStreamUtil.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"

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
        
        OverrideObject = other.OverrideObject;
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
        SO_RETURN_IF_FOUND(OverrideObject, IsValid(), bool);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(IsValid(), bool);
        return true;
    }

    ssGUI::CharacterRenderInfo BackendFontMock::GetCharacterRenderInfo(wchar_t charUnicode, float charSize)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL((int)charUnicode, charSize);
        SO_RETURN_IF_FOUND(OverrideObject, GetCharacterRenderInfo(wchar_t, float), ssGUI::CharacterRenderInfo, charUnicode, charSize);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetCharacterRenderInfo(charUnicode, charSize), ssGUI::CharacterRenderInfo);
        return ssGUI::CharacterRenderInfo();
    }
    
    bool BackendFontMock::IsCharacterSupported(wchar_t charUnicode)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL((int)charUnicode);
        SO_RETURN_IF_FOUND(OverrideObject, IsCharacterSupported(wchar_t), bool, charUnicode);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(IsCharacterSupported(charUnicode), bool);
        return true;
    }
    
    float BackendFontMock::GetKerning(wchar_t charUnicode, wchar_t secondCharUnicode, float charSize)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL((int)charUnicode, (int)secondCharUnicode, charSize);
        SO_RETURN_IF_FOUND(OverrideObject,
                            GetKerning(wchar_t, wchar_t, float), float,
                            charUnicode, secondCharUnicode, charSize);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetKerning(charUnicode, secondCharUnicode, charSize), float);
        return 0;
    }
    
    float BackendFontMock::GetLineSpacing(float charSize)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL(charSize);
        SO_RETURN_IF_FOUND(OverrideObject, GetLineSpacing(float), float, charSize);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetLineSpacing(charSize), float);
        return 0;
    }
    
    float BackendFontMock::GetUnderlineOffset(float charSize)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL(charSize);
        SO_RETURN_IF_FOUND(OverrideObject, GetUnderlineOffset(float), float, charSize);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetUnderlineOffset(charSize), float);
        return 0;
    }
    
    float BackendFontMock::GetUnderlineThickness(float charSize)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL(charSize);
        SO_RETURN_IF_FOUND(OverrideObject, GetUnderlineThickness(float), float, charSize);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetUnderlineThickness(charSize), float);
        return 0;
    }

    bool BackendFontMock::LoadFromPath(std::string path)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL(path);
        SO_RETURN_IF_FOUND(OverrideObject, LoadFromPath(std::string), bool, path);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(LoadFromPath(path), bool);
        return true;
    }

    bool BackendFontMock::LoadFromMemory(void* dataPtr, int lengthInBytes)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL(dataPtr, lengthInBytes);
        SO_RETURN_IF_FOUND(OverrideObject, LoadFromMemory(void*, int), bool, dataPtr, lengthInBytes);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(LoadFromMemory(dataPtr, lengthInBytes), bool);
        return true;
    }
    
    bool BackendFontMock::GetFixedAvailableFontSizes(std::vector<float>& fontSizes)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL(fontSizes);
        SO_MODIFY_ARGUMENTS_AND_RETURN_IF_FOUND(OverrideObject, true, GetFixedAvailableFontSizes(std::vector<float>&), fontSizes);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetFixedAvailableFontSizes(fontSizes), bool);
        return true;
    }
    
    bool BackendFontMock::GetCharacterImage(wchar_t charUnicode, float charSize, ssGUI::ImageData& characterImage)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL((int)charUnicode, charSize, characterImage);
        SO_MODIFY_ARGUMENTS_AND_RETURN_IF_FOUND(OverrideObject,
                                                true, 
                                                GetCharacterImage(wchar_t, float, ssGUI::ImageData&), 
                                                charUnicode, 
                                                charSize, 
                                                (SO_NonComparableCopyable<ssGUI::ImageData>&)characterImage);
        
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetCharacterImage(charUnicode, charSize, characterImage), bool);
        return true;
    }

    void* BackendFontMock::GetRawHandle()
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        SO_RETURN_IF_FOUND(OverrideObject, GetRawHandle(), void*);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetRawHandle(), void*);
        return nullptr;
    }

    BackendFontInterface* BackendFontMock::Clone()
    {
        return new BackendFontMock(*this);
    }
}

}