#ifndef SSGUI_BACKEND_FONT_MOCK_H
#define SSGUI_BACKEND_FONT_MOCK_H

//#define SSGUI_MOCK_ENABLE_LOG
#include "ssGUI/Backend/Mocks/MockMacro.hpp"

#include "ssGUI/Backend/Interfaces/BackendFontInterface.hpp"
#include "CppOverride.hpp"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendFontMock
    class BackendFontMock : public BackendFontInterface
    {   
        private:
            CO_DECLARE_MEMBER_INSTNACE(OverrideObject);

            BackendFontInterface* UnderlyingInterface;

            BackendFontMock& operator=(BackendFontMock const& other);
        
        protected:
            BackendFontMock(BackendFontMock const& other);

        public:
            CO_DECLARE_OVERRIDE_METHODS(OverrideObject)

            BackendFontMock(BackendFontInterface* fontInterface);
            ~BackendFontMock() override;

            SSGUI_MOCK_DECLARE_VARIABLE_GETTER( BackendFontInterface*, 
                                                UnderlyingInterface)
            
            //function: IsValid
            //See <BackendFontInterface::IsValid>
            bool IsValid() const override;

            //function: GetCharacterRenderInfo
            //See <BackendFontInterface::GetCharacterRenderInfo>
            CharacterRenderInfo GetCharacterRenderInfo( char32_t charUnicode, 
                                                        float charSize) const override;
            
            //function: IsCharacterSupported
            //See <BackendFontInterface::IsCharacterSupported>
            bool IsCharacterSupported(char32_t charUnicode) const override;
            
            //function: GetKerning
            //See <BackendFontInterface::GetKerning>
            float GetKerning(   char32_t charUnicode, 
                                char32_t secondCharUnicode, 
                                float charSize) const override;
            
            //function: GetLineSpacing
            //See <BackendFontInterface::GetLineSpacing>
            float GetLineSpacing(float charSize) const override;
            
            //function: GetUnderlineOffset
            //See <BackendFontInterface::GetUnderlineOffset>
            float GetUnderlineOffset(float charSize) const override;
            
            //function: GetUnderlineThickness
            //See <BackendFontInterface::GetUnderlineThickness>
            float GetUnderlineThickness(float charSize) const override;

            //function: LoadFromPath
            //See <BackendFontInterface::LoadFromPath>
            bool LoadFromPath(std::string path) override;

            //function: LoadFromMemory
            //See <BackendFontInterface::LoadFromMemory>
            bool LoadFromMemory(const void* dataPtr, int lengthInBytes) override;

            //function: GetFixedAvailableFontSizes
            //See <BackendFontInterface::GetFixedAvailableFontSizes>
            bool GetFixedAvailableFontSizes(std::vector<float>& fontSizes) const override;

            //function: GetCharacterImage
            //See <BackendFontInterface::GetCharacterImage>
            bool GetCharacterImage( char32_t charUnicode, 
                                    float charSize, 
                                    BackendImageInterface& characterImage) const override;

            //function: GetRawHandle
            //See <BackendFontInterface::GetRawHandle>
            void* GetRawHandle() const override;

            //function: Clone
            //See <BackendFontInterface::Clone>
            BackendFontInterface* Clone() override;
    };   
}

}


#endif