#ifndef SSGUI_BACKEND_FONT_INTERFACE_HPP
#define SSGUI_BACKEND_FONT_INTERFACE_HPP

#include "ssGUI/DataClasses/CharacterRenderInfo.hpp"

#include <string>
#include <vector>

namespace ssGUI
{ 


//namespace: ssGUI::Backend
namespace Backend
{
    class ImageInterface;
    
    //class: ssGUI::Backend::FontInterface
    class FontInterface
    {   
        public:
            FontInterface(){}
            virtual ~FontInterface() = 0;
            
            //function: Initialize
            virtual bool Initialize() = 0;
            
            //function: IsValid
            //Returns true if the font is loaded.
            virtual bool IsValid() const = 0;

            //function: GetCharacterRenderInfo
            //Gets the rendering infomation of a character.
            //For fixed size font behaviour, please see the backend you are using.
            virtual CharacterRenderInfo GetCharacterRenderInfo( char32_t charUnicode, 
                                                                float charSize) const = 0;
            
            //function: IsCharacterSupported
            //Returns true if the character is supported
            virtual bool IsCharacterSupported(char32_t charUnicode) const = 0;
            
            /*
            function: GetKerning
            Kerning represents the drawing position offset when drawn after certain characters. 
            The offset return is for the second character.
            (i.e. AV can return a negative kerning for the character V)
            */
            virtual float GetKerning(   char32_t charUnicode, 
                                        char32_t secondCharUnicode, 
                                        float charSize) const = 0;
            
            //function: GetLineSpacing
            //Returns how much a line space there is based on the font size.
            //For fixed size font behaviour, please see the backend you are using.
            virtual float GetLineSpacing(float charSize) const = 0;
            
            //function: GetUnderlineOffset
            //Gets the vertical offset for underline based on the font size.
            //For fixed size font behaviour, please see the backend you are using.
            virtual float GetUnderlineOffset(float charSize) const = 0;
            
            //function: GetUnderlineThickness
            //Gets the thickness for underline based on the font size 
            //For fixed size font behaviour, please see the backend you are using.
            virtual float GetUnderlineThickness(float charSize) const = 0;

            //function: LoadFromPath
            //Loads a font file. Supported font format is dependent on the backend.
            //_path_ can be either absolute or relative.
            virtual bool LoadFromPath(std::string path) = 0;

            //function: LoadFromMemory
            //Load the raw font file in memory, which is copied and stored internally.
            virtual bool LoadFromMemory(const void* dataPtr, int lengthInBytes) = 0;

            //function: GetFixedAvailableFontSizes
            //If the font is not scalable, this will return all the fixed sizes. Otherwise empty vector.
            virtual bool GetFixedAvailableFontSizes(std::vector<float>& fontSizes) const = 0;

            //function: GetCharacterImage
            //Returns the character image
            //For fixed size font behaviour, please see the backend you are using.
            virtual bool GetCharacterImage( char32_t charUnicode, 
                                            float charSize, 
                                            ImageInterface& characterImage) const = 0;

            //function: GetRawHandle
            //Returns the raw pointer to the underlying backend implmentation object
            virtual void* GetRawHandle() const = 0;

            //function: Clone
            //Clones the backend font object
            virtual FontInterface* Clone() = 0;
    };
    
    //Pure virtual destructor needs to be defined
    inline FontInterface::~FontInterface(){}
}

}

#endif