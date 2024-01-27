#ifndef SSGUI_BACKEND_FONT_MOCK_H
#define SSGUI_BACKEND_FONT_MOCK_H

//#define SSGUI_MOCK_ENABLE_LOG
#include "ssGUI/Backend/Mocks/MockMacro.hpp"

#include "ssGUI/Backend/Interfaces/FontInterface.hpp"
#include "CppOverride.hpp"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendFontMock
    class BackendFontMock : public FontInterface
    {   
        private:
            CO_DECLARE_MEMBER_INSTNACE(OverrideObject);

            FontInterface* UnderlyingInterface;

            BackendFontMock& operator=(BackendFontMock const& other);
        
        protected:
            BackendFontMock(BackendFontMock const& other);

        public:
            CO_DECLARE_OVERRIDE_METHODS(OverrideObject)

            BackendFontMock(FontInterface* fontInterface);
            ~BackendFontMock() override;

            SSGUI_MOCK_DECLARE_VARIABLE_GETTER( FontInterface*, 
                                                UnderlyingInterface)
            
            //function: IsValid
            //See <FontInterface::IsValid>
            bool IsValid() const override;

            //function: GetCharacterRenderInfo
            //See <FontInterface::GetCharacterRenderInfo>
            CharacterRenderInfo GetCharacterRenderInfo( char32_t charUnicode, 
                                                        float charSize) const override;
            
            //function: IsCharacterSupported
            //See <FontInterface::IsCharacterSupported>
            bool IsCharacterSupported(char32_t charUnicode) const override;
            
            //function: GetKerning
            //See <FontInterface::GetKerning>
            float GetKerning(   char32_t charUnicode, 
                                char32_t secondCharUnicode, 
                                float charSize) const override;
            
            //function: GetLineSpacing
            //See <FontInterface::GetLineSpacing>
            float GetLineSpacing(float charSize) const override;
            
            //function: GetUnderlineOffset
            //See <FontInterface::GetUnderlineOffset>
            float GetUnderlineOffset(float charSize) const override;
            
            //function: GetUnderlineThickness
            //See <FontInterface::GetUnderlineThickness>
            float GetUnderlineThickness(float charSize) const override;

            //function: LoadFromPath
            //See <FontInterface::LoadFromPath>
            bool LoadFromPath(std::string path) override;

            //function: LoadFromMemory
            //See <FontInterface::LoadFromMemory>
            bool LoadFromMemory(const void* dataPtr, int lengthInBytes) override;

            //function: GetFixedAvailableFontSizes
            //See <FontInterface::GetFixedAvailableFontSizes>
            bool GetFixedAvailableFontSizes(std::vector<float>& fontSizes) const override;

            //function: GetCharacterImage
            //See <FontInterface::GetCharacterImage>
            bool GetCharacterImage( char32_t charUnicode, 
                                    float charSize, 
                                    ImageInterface& characterImage) const override;

            //function: GetRawHandle
            //See <FontInterface::GetRawHandle>
            void* GetRawHandle() const override;

            //function: Clone
            //See <FontInterface::Clone>
            FontInterface* Clone() override;
    };   
}

}


#endif