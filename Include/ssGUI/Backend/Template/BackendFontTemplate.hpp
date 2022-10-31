#ifndef SSGUI_BACKEND_FONT_TEMPLATE
#define SSGUI_BACKEND_FONT_TEMPLATE

#include "ssGUI/Backend/Interfaces/BackendFontInterface.hpp"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendFontTemplate
    class BackendFontTemplate : public BackendFontInterface
    {   
        private:
        
        public:
            BackendFontTemplate();
            ~BackendFontTemplate() override;
            
            //function: IsValid
            //Returns true if the font is loaded.
            bool IsValid() override;

            //function: GetCharacterRenderInfo
            //Gets the rendering infomation of a character
            ssGUI::CharacterRenderInfo GetCharacterRenderInfo(wchar_t charUnicode, float charSize) override;
            
            //function: IsCharacterSupported
            //Returns true if the character is supported
            bool IsCharacterSupported(wchar_t charUnicode) override;
            
            //function: GetKerning
            //Kerning represents the drawing position offset when drawn after certain characters. (i.e. AV has a negative kerning for the characte V)
            float GetKerning(wchar_t charUnicode, wchar_t secondCharUnicode, float charSize) override;
            
            //function: GetLineSpacing
            //Returns how much a line space is based on the font size
            float GetLineSpacing(float charSize) override;
            
            //function: GetUnderlineOffset
            //Gets the vertical offset for underline based on the font size 
            float GetUnderlineOffset(float charSize) override;
            
            //function: GetUnderlineThickness
            //Gets the thickness for underline based on the font size 
            float GetUnderlineThickness(float charSize) override;

            /*function: LoadFromPath
            Loads a font file. Supported font format is dependent on the backend.
            
            _path_ can be either absolute or relative. Please see <BackendFontSFML::LoadFromPath> for supported font format.
            */
            bool LoadFromPath(std::string path) override;

            //function: LoadFromMemory
            //Please see <BackendFontSFML::LoadFromMemory> for supported font format.
            bool LoadFromMemory(void* dataPtr, int lengthInBytes) override;

            //function: GetRawHandle
            //Returns the raw pointer to the underlying backend implmentation object
            void* GetRawHandle() override;

            //function: Clone
            //Clones the backend font object
            BackendFontInterface* Clone() override;
    };   
}

}


#endif