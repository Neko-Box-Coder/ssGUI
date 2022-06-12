#ifndef SSGUI_BACKEND_FONT_INTERFACE
#define SSGUI_BACKEND_FONT_INTERFACE

#include <string>
#include "ssGUI/DataClasses/CharacterRenderInfo.hpp"

//namespace: ssGUI::Backend
namespace ssGUI::Backend
{
    //class: ssGUI::Backend::BackendFontInterface
    class BackendFontInterface
    {   
        private:
            // BackendFontInterface(const BackendFontInterface&);
            // BackendFontInterface& operator=(const BackendFontInterface&);
        
        public:
            BackendFontInterface(){}
            
            //function: IsValid
            //Returns true if the font is loaded.
            virtual bool IsValid() = 0;

            //function: GetCharacterRenderInfo
            //Gets the rendering infomation of a character
            virtual ssGUI::CharacterRenderInfo GetCharacterRenderInfo(wchar_t charUnicode, float charSize) = 0;
            
            //function: IsCharacterSupported
            //Returns true if the character is supported
            virtual bool IsCharacterSupported(wchar_t charUnicode) = 0;
            
            //function: GetKerning
            //Kerning represents the drawing position offset when drawn after certain characters. (i.e. AV has a negative kerning for the characte V)
            virtual float GetKerning(wchar_t charUnicode, wchar_t secondCharUnicode, float charSize) = 0;
            
            //function: GetLineSpacing
            //Returns how much a line space is based on the font size
            virtual float GetLineSpacing(float charSize) = 0;
            
            //function: GetUnderlineOffset
            //Gets the vertical offset for underline based on the font size 
            virtual float GetUnderlineOffset(float charSize) = 0;
            
            //function: GetUnderlineThickness
            //Gets the thickness for underline based on the font size 
            virtual float GetUnderlineThickness(float charSize) = 0;

            /*function: LoadFromPath
            Loads a font file. Supported font format is dependent on the backend.
            
            _path_ can be either absolute or relative. Please see <BackendFontSFML::LoadFromPath> for supported font format.
            */
            virtual bool LoadFromPath(std::string path) = 0;

            //function: LoadFromMemory
            //Please see <BackendFontSFML::LoadFromMemory> for supported font format.
            virtual bool LoadFromMemory(void* dataPtr, int lengthInBytes) = 0;

            //function: GetRawHandle
            //Returns the raw pointer to the underlying backend implmentation object
            virtual void* GetRawHandle() = 0;

            //function: Clone
            //Clones the backend font object
            virtual BackendFontInterface* Clone() = 0;
    };
}


#endif