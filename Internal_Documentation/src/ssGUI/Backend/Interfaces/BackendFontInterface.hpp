#ifndef SSGUI_BACKEND_FONT_INTERFACE
#define SSGUI_BACKEND_FONT_INTERFACE

#include <string>
#include "ssGUI/BaseClasses/CharacterInfo.hpp"

//namespace: ssGUI::Backend
namespace ssGUI::Backend
{
    //class: BackendFontInterface
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

            //function: GetCharacterInfo
            virtual ssGUI::CharacterInfo GetCharacterInfo(wchar_t charUnicode, int charSize) = 0;
            
            //function: IsCharacterSupported
            virtual bool IsCharacterSupported(wchar_t charUnicode) = 0;
            
            //function: GetKerning
            virtual int GetKerning(wchar_t charUnicode, wchar_t secondCharUnicode, int charSize) = 0;
            
            //function: GetLineSpacing
            virtual int GetLineSpacing(int charSize) = 0;
            
            //function: GetUnderlineOffset
            virtual int GetUnderlineOffset(int charSize) = 0;
            
            //function: GetUnderlineThickness
            virtual int GetUnderlineThickness(int charSize) = 0;


            /*function: LoadFromPath
            Loads a font file. Supported font format is dependent on the backend.
            
            _path_ can be either absolute or relative. Please see <BackendFontSFML::LoadFromPath> for supported font format.
            */
            virtual bool LoadFromPath(std::string path) = 0;

            //function: LoadFromMemory
            //Please see <BackendFontSFML::LoadFromMemory> for supported font format.
            virtual bool LoadFromMemory(void* dataPtr, int lengthInBytes) = 0;

            //function: Clone
            virtual BackendFontInterface* Clone() = 0;

            //TODO : Add GetRawHandle()

    };
}


#endif