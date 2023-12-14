#ifndef SSGUI_IMAGE_FORMAT_H
#define SSGUI_IMAGE_FORMAT_H

#include "ssGUI/Enums/ImageType.hpp"
#include <cstdint>
#include <ostream>
#include "ssGUI/HelperClasses/OutputStreamUtil.hpp"

namespace ssGUI
{
    //struct: ssGUI::ImageFormat
    //Default is RGBA 8 bit per channel with non alpha pre-multiplied. 
    struct ImageFormat
    {
        //var: ImgType
        //Indicates the image type
        Enums::ImageType ImgType = Enums::ImageType::RGB;
        
        //var: BitDepthPerChannel
        //How many bits per channel
        int BitDepthPerChannel = 8;
        
        //var: BitPerPixel
        //How many biys per pixel
        int BitPerPixel = 32;
        
        //var: HasAlpha
        //If this image contains alpha.
        //If this value is set to true for type MONO but IndexA is set to -1,
        //then it will treat black as transparent
        bool HasAlpha = true;
        
        //var: PreMultipliedAlpha
        //If this image has pre-multiplied alpha, this means that the color values are already multiplied with the alpha value.
        bool PreMultipliedAlpha = false;
        
        //var: IndexR 
        int IndexR = 0;
        
        //var: IndexG 
        int IndexG = 1;
        
        //var: IndexB
        int IndexB = 2;
        
        //var: IndexMono
        int IndexMono = -1;
        
        //var: IndexA
        int IndexA = 3;
        
        //var: RowPaddingInBytes
        int RowPaddingInBytes = 0;
        
        inline bool operator== (const ImageFormat& other)
        {
            bool monoCheck = ImgType == Enums::ImageType::MONO && HasAlpha ? IndexMono == other.IndexMono : true;
            bool rgbCheck = ImgType == Enums::ImageType::RGB ? 
                            IndexR == other.IndexR && IndexG == other.IndexG && IndexB == other.IndexB :
                            true;
            bool alphaCheck = HasAlpha ? IndexA == other.IndexA && PreMultipliedAlpha == other.PreMultipliedAlpha : true;
        
            return  ImgType == other.ImgType && 
                    BitDepthPerChannel == other.BitDepthPerChannel && 
                    BitPerPixel == other.BitPerPixel &&
                    HasAlpha == other.HasAlpha &&
                    monoCheck &&
                    rgbCheck &&
                    alphaCheck &&
                    RowPaddingInBytes == other.RowPaddingInBytes;
        }
        
        inline bool operator!= (const ImageFormat& other)
        {
            return !operator==(other);
        }
        
        inline friend std::ostream& operator<< (std::ostream& stream, const ImageFormat& other)
        {
            stream  << SSGUI_OUTPUT_CLASS_NAME(ImageFormat)
                    << SSGUI_OUTPUT_VAR(ImgType)
                    << SSGUI_OUTPUT_VAR(BitDepthPerChannel)
                    << SSGUI_OUTPUT_VAR(BitPerPixel)
                    << SSGUI_OUTPUT_VAR(HasAlpha)
                    << SSGUI_OUTPUT_VAR(PreMultipliedAlpha)
                    << SSGUI_OUTPUT_VAR(IndexR)
                    << SSGUI_OUTPUT_VAR(IndexG)
                    << SSGUI_OUTPUT_VAR(IndexB)
                    << SSGUI_OUTPUT_VAR(IndexMono)
                    << SSGUI_OUTPUT_VAR(IndexA)
                    << SSGUI_OUTPUT_LAST_VAR(RowPaddingInBytes);
                    
            return stream;
        }
    };
}

#endif