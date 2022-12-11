#ifndef SSGUI_SFML_IMAGE_CONVERSION_H
#define SSGUI_SFML_IMAGE_CONVERSION_H

#include "ssGUI/HelperClasses/ImageConversion.hpp"


#include "ssGUI/DataClasses/ImageFormat.hpp"
#include "glm/vec2.hpp"
#include "SFML/Graphics.hpp"
#include <cstdint>
#include <functional>
#include <limits>

namespace ssGUI
{

class SFMLImageConversion
{
    public:
        template<typename T>
        static bool ConvertToRGBA32(sf::Image& outImg, void const * dataPtr, ssGUI::ImageFormat format, 
                                    glm::ivec2 imageSize)
        {
            //Set size first
            outImg.create(sf::Vector2u(imageSize.x, imageSize.y));
        
            uint8_t const* imgPtr = static_cast<uint8_t const*>(dataPtr);
            int bytePerPixel = format.BitPerPixel / 8;                                      //Calculate how many bytes each pixel takes
            int rowNumOfBytes = bytePerPixel * imageSize.x + format.RowPaddingInBytes;      //Calculate how many bytes each row takes, normally called pitch
        
            std::function<void(uint8_t const*, int, int)> conversionFunc;                   //function pointer for doing conversion
            
            //First assign the conversion function depending on the image type and alpha
            //Grayscale
            if(format.ImgType == ssGUI::Enums::ImageType::MONO)
            {
                if(format.HasAlpha)
                {
                    //If the alpha index is not specified, we assume black is transparent.
                    //Always use white and use the pixel value for alpha
                    if(format.IndexA < 0)
                    {
                        conversionFunc =    [&](uint8_t const* curBytePtr, int x, int y)
                                            {
                                                T const* curPixelPtr = reinterpret_cast<T const*>(curBytePtr);    
                                                uint8_t level = ssGUI::ImageConversion::GetLevelInByte(curPixelPtr[0]);
                                                outImg.setPixel(sf::Vector2u(x, y), sf::Color(255, 255, 255, level));
                                            };
                    }
                    //Otherwise if alpha index is specified, use it
                    else
                    {
                        conversionFunc =    [&](uint8_t const* curBytePtr, int x, int y)
                                            {
                                                T const* curPixelPtr = reinterpret_cast<T const*>(curBytePtr);    
                                                uint8_t level = ssGUI::ImageConversion::GetLevelInByte(curPixelPtr[format.IndexMono]);
                                                outImg.setPixel(sf::Vector2u(x, y), 
                                                                sf::Color(  level, 
                                                                            level, 
                                                                            level, 
                                                                            ssGUI::ImageConversion::GetLevelInByte(curPixelPtr[format.IndexA])));
                                            };
                    }
                }
                else
                {
                    conversionFunc =    [&](uint8_t const* curBytePtr, int x, int y)
                                        {
                                            T const* curPixelPtr = reinterpret_cast<T const*>(curBytePtr);    
                                            uint8_t level = ssGUI::ImageConversion::GetLevelInByte(curPixelPtr[0]);
                                            outImg.setPixel(sf::Vector2u(x, y), sf::Color(level, level, level));
                                        };
                }
            }
            //RGB
            else
            {
                if(format.HasAlpha)
                {
                    if(!format.PreMultipliedAlpha)
                    {
                        conversionFunc =    [&](uint8_t const* curBytePtr, int x, int y)
                                            {
                                                T const* curPixelPtr = reinterpret_cast<T const*>(curBytePtr);    
                                                outImg.setPixel(sf::Vector2u(x, y), 
                                                                sf::Color(  ssGUI::ImageConversion::GetLevelInByte(curPixelPtr[format.IndexR]), 
                                                                            ssGUI::ImageConversion::GetLevelInByte(curPixelPtr[format.IndexG]), 
                                                                            ssGUI::ImageConversion::GetLevelInByte(curPixelPtr[format.IndexB]),
                                                                            ssGUI::ImageConversion::GetLevelInByte(curPixelPtr[format.IndexA])));
                                            };
                    }
                    else
                    {
                        conversionFunc =    [&](uint8_t const* curBytePtr, int x, int y)
                                            {
                                                T const* curPixelPtr = reinterpret_cast<T const*>(curBytePtr);
                                                
                                                uint8_t alpha = ssGUI::ImageConversion::GetLevelInByte(curPixelPtr[format.IndexA]);
                                                float alphaInFloat =    static_cast<float>(curPixelPtr[format.IndexA]) / 
                                                                        static_cast<float>(std::numeric_limits<T>::max());
                                                
                                                outImg.setPixel(sf::Vector2u(x, y), 
                                                                sf::Color(  ssGUI::ImageConversion::GetReversedPreMultipliedLevelInByte(curPixelPtr[format.IndexR], alphaInFloat), 
                                                                            ssGUI::ImageConversion::GetReversedPreMultipliedLevelInByte(curPixelPtr[format.IndexG], alphaInFloat), 
                                                                            ssGUI::ImageConversion::GetReversedPreMultipliedLevelInByte(curPixelPtr[format.IndexB], alphaInFloat),
                                                                            alpha));
                                            };
                    }
                
                }
                else
                {
                    conversionFunc =    [&](uint8_t const* curBytePtr, int x, int y)
                                        {
                                            T const* curPixelPtr = reinterpret_cast<T const*>(curBytePtr);    
                                            outImg.setPixel(sf::Vector2u(x, y), 
                                                            sf::Color(  ssGUI::ImageConversion::GetLevelInByte(curPixelPtr[format.IndexR]), 
                                                                        ssGUI::ImageConversion::GetLevelInByte(curPixelPtr[format.IndexG]), 
                                                                        ssGUI::ImageConversion::GetLevelInByte(curPixelPtr[format.IndexB])));
                                        };
                }
            }
        
            //Then we call the conversion function for each pixel
            for(int y = 0; y < imageSize.y; y++)
            {
                for(int x = 0; x < imageSize.x; x++)
                {
                    uint8_t const* curBytePtr = imgPtr + (y * rowNumOfBytes) + x * bytePerPixel;
                    conversionFunc(curBytePtr, x, y);
                }
            }
            
            return true;
        }
};

}

#endif