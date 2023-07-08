#ifndef SSGUI_BACKEND_FONT_MOCK_H
#define SSGUI_BACKEND_FONT_MOCK_H

//#define SSGUI_MOCK_ENABLE_LOG
#include "ssGUI/Backend/Mocks/MockMacro.hpp"

#include "ssGUI/Backend/Interfaces/BackendFontInterface.hpp"
#include "FunctionOverrides.hpp"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendFontMock
    class BackendFontMock : public BackendFontInterface
    {   
        private:
            FO_DECLARE_INSTNACE(OverrideObject);

            ssGUI::Backend::BackendFontInterface* UnderlyingInterface;

            BackendFontMock& operator=(BackendFontMock const& other);
        
        protected:
            BackendFontMock(BackendFontMock const& other);

        public:
            FO_DECLARE_OVERRIDE_METHODS(OverrideObject)

            BackendFontMock(ssGUI::Backend::BackendFontInterface* fontInterface);
            ~BackendFontMock() override;

            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(ssGUI::Backend::BackendFontInterface*, UnderlyingInterface)
            
            //function: IsValid
            //See <BackendFontInterface::IsValid>
            bool IsValid() const override;

            //function: GetCharacterRenderInfo
            //See <BackendFontInterface::GetCharacterRenderInfo>
            ssGUI::CharacterRenderInfo GetCharacterRenderInfo(wchar_t charUnicode, float charSize) override;
            
            //function: IsCharacterSupported
            //See <BackendFontInterface::IsCharacterSupported>
            bool IsCharacterSupported(wchar_t charUnicode) override;
            
            //function: GetKerning
            //See <BackendFontInterface::GetKerning>
            float GetKerning(wchar_t charUnicode, wchar_t secondCharUnicode, float charSize) override;
            
            //function: GetLineSpacing
            //See <BackendFontInterface::GetLineSpacing>
            float GetLineSpacing(float charSize) override;
            
            //function: GetUnderlineOffset
            //See <BackendFontInterface::GetUnderlineOffset>
            float GetUnderlineOffset(float charSize) override;
            
            //function: GetUnderlineThickness
            //See <BackendFontInterface::GetUnderlineThickness>
            float GetUnderlineThickness(float charSize) override;

            //function: LoadFromPath
            //See <BackendFontInterface::LoadFromPath>
            bool LoadFromPath(std::string path) override;

            //function: LoadFromMemory
            //See <BackendFontInterface::LoadFromMemory>
            bool LoadFromMemory(void* dataPtr, int lengthInBytes) override;

            //function: GetFixedAvailableFontSizes
            //See <BackendFontInterface::GetFixedAvailableFontSizes>
            bool GetFixedAvailableFontSizes(std::vector<float>& fontSizes) override;

            //function: GetCharacterImage
            //See <BackendFontInterface::GetCharacterImage>
            bool GetCharacterImage(wchar_t charUnicode, float charSize, ssGUI::ImageData& characterImage) override;

            //function: GetRawHandle
            //See <BackendFontInterface::GetRawHandle>
            void* GetRawHandle() override;

            //function: Clone
            //See <BackendFontInterface::Clone>
            BackendFontInterface* Clone() override;
    };   
}

}


#endif