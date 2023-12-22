#include "ssGUI/Backend/Mocks/BackendFontMock.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"
#include "ssGUI/HelperClasses/OutputStreamUtil.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendFontMock::BackendFontMock(const BackendFontMock& other)
    {
        if(other.UnderlyingInterface != nullptr)
            UnderlyingInterface = other.UnderlyingInterface->Clone();
        else
            UnderlyingInterface = nullptr;
        
        OverrideObject = other.OverrideObject;
    }

    BackendFontMock::BackendFontMock(BackendFontInterface* fontInterface) : 
        UnderlyingInterface(fontInterface)
    {}

    BackendFontMock::~BackendFontMock()
    {
        if(UnderlyingInterface != nullptr)
            delete UnderlyingInterface;
    }
    
    bool BackendFontMock::IsValid() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_RETURN_IF_FOUND(OverrideObject, IsValid(), bool);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(IsValid(), bool);
        return true;
    }

    ssGUI::CharacterRenderInfo BackendFontMock::GetCharacterRenderInfo( char32_t charUnicode, 
                                                                        float charSize) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL((int)charUnicode, charSize);
        CO_RETURN_IF_FOUND( OverrideObject, 
                            GetCharacterRenderInfo(char32_t, float), 
                            ssGUI::CharacterRenderInfo, charUnicode, charSize);
        
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC( GetCharacterRenderInfo(charUnicode, charSize), 
                                                ssGUI::CharacterRenderInfo);

        return ssGUI::CharacterRenderInfo();
    }
    
    bool BackendFontMock::IsCharacterSupported(char32_t charUnicode) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL((int)charUnicode);
        CO_RETURN_IF_FOUND(OverrideObject, IsCharacterSupported(char32_t), bool, charUnicode);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(IsCharacterSupported(charUnicode), bool);
        return true;
    }
    
    float BackendFontMock::GetKerning(  char32_t charUnicode, 
                                        char32_t secondCharUnicode, 
                                        float charSize) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL((int)charUnicode, (int)secondCharUnicode, charSize);
        CO_RETURN_IF_FOUND( OverrideObject,
                            GetKerning(char32_t, char32_t, float), float,
                            charUnicode, secondCharUnicode, charSize);
        
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC( GetKerning(charUnicode, secondCharUnicode, charSize), 
                                                float);
        return 0;
    }
    
    float BackendFontMock::GetLineSpacing(float charSize) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL(charSize);
        CO_RETURN_IF_FOUND(OverrideObject, GetLineSpacing(float), float, charSize);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetLineSpacing(charSize), float);
        return 0;
    }
    
    float BackendFontMock::GetUnderlineOffset(float charSize) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL(charSize);
        CO_RETURN_IF_FOUND(OverrideObject, GetUnderlineOffset(float), float, charSize);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetUnderlineOffset(charSize), float);
        return 0;
    }
    
    float BackendFontMock::GetUnderlineThickness(float charSize) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL(charSize);
        CO_RETURN_IF_FOUND(OverrideObject, GetUnderlineThickness(float), float, charSize);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetUnderlineThickness(charSize), float);
        return 0;
    }

    bool BackendFontMock::LoadFromPath(std::string path)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL(path);
        CO_RETURN_IF_FOUND(OverrideObject, LoadFromPath(std::string), bool, path);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(LoadFromPath(path), bool);
        return true;
    }

    bool BackendFontMock::LoadFromMemory(const void* dataPtr, int lengthInBytes)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL(dataPtr, lengthInBytes);
        CO_RETURN_IF_FOUND(OverrideObject, LoadFromMemory(void*, int), bool, dataPtr, lengthInBytes);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(LoadFromMemory(dataPtr, lengthInBytes), bool);
        return true;
    }
    
    bool BackendFontMock::GetFixedAvailableFontSizes(std::vector<float>& fontSizes) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL(fontSizes);
        CO_MODIFY_ARGS_AND_RETURN_IF_FOUND( OverrideObject, 
                                            true, 
                                            GetFixedAvailableFontSizes(std::vector<float>&), 
                                            fontSizes);
        
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetFixedAvailableFontSizes(fontSizes), bool);
        return true;
    }
    
    bool BackendFontMock::GetCharacterImage(char32_t charUnicode, 
                                            float charSize, 
                                            BackendImageInterface& characterImage) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL((int)charUnicode, charSize, characterImage);
        CO_MODIFY_ARGS_AND_RETURN_IF_FOUND( OverrideObject,
                                            true, 
                                            GetCharacterImage(char32_t, float, BackendImageInterface&), 
                                            charUnicode, 
                                            charSize, 
                                            characterImage);

        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC( GetCharacterImage(  charUnicode, 
                                                                    charSize, 
                                                                    characterImage), 
                                                bool);
        return true;
    }

    void* BackendFontMock::GetRawHandle() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_RETURN_IF_FOUND(OverrideObject, GetRawHandle(), void*);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetRawHandle(), void*);
        return nullptr;
    }

    BackendFontInterface* BackendFontMock::Clone()
    {
        return new BackendFontMock(*this);
    }
}

}