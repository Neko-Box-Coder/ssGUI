#ifndef SSGUI_IMAGE_CONVERSION_H
#define SSGUI_IMAGE_CONVERSION_H

#include "ssGUI/DataClasses/ImageFormat.hpp"

#include "glm/vec2.hpp"

#include <cstdint>
#include <functional>
#include <limits>

#undef max //std::numeric_limits<T>::max() is colliding with max macro in windows.h

namespace ssGUI
{
    class ImageConversion
    {
        public:
        template<typename T>
        static uint8_t GetLevelInByte(T val)
        { 
            return  static_cast<float>(val) / static_cast<float>(std::numeric_limits<T>::max()) *   //Gets the value, divide by its own max,
                    static_cast<float>(std::numeric_limits<uint8_t>::max()) + 0.5f;                 //Multiply by 255 in order to scale to 255
                                                                                                    //and add 0.5 for rounding when casting back to uint8_t
        }
        
        template<typename T>
        static uint8_t GetReversedPreMultipliedLevelInByte(T val, float alpha)
        {
            return  static_cast<float>(val) / static_cast<float>(alpha) /               //Reverse the premultiplied alpha first
                    static_cast<float>(std::numeric_limits<T>::max()) *                 //Then we scale it from 0 to 1
                    static_cast<float>(std::numeric_limits<uint8_t>::max()) + 0.5f;     //Finally scale it back to 255
        }

        //TODO: Remove template
        template<typename T>
        //outImg must be allocated already before passing to this function
        static bool ConvertToRGBA32(void* outImg, void const * dataPtr, ssGUI::ImageFormat format, 
                                    glm::ivec2 imageSize)
        {
            uint8_t const* imgPtr = static_cast<uint8_t const*>(dataPtr);
            uint8_t* outImgBytePtr = static_cast<uint8_t*>(outImg);

            int bytePerPixel = format.BitPerPixel / 8;                                      //Calculate how many bytes each pixel takes
            int rowNumOfBytes = bytePerPixel * imageSize.x + format.RowPaddingInBytes;      //Calculate how many bytes each row takes, normally called pitch
        
            std::function<void(uint8_t const*, int, int)> conversionFunc;                   //function pointer for doing conversion
            
            //First assign the conversion function depending on the image type and alpha
            //Grayscale
            if(format.ImgType == ssGUI::Enums::ImageType::MONO)
            {
                if(format.HasAlpha)
                {
                    //If the alpha index is not specified, we assume black is transparent
                    if(format.IndexA < 0)
                    {
                        conversionFunc =    [&](uint8_t const* curBytePtr, int x, int y)
                                            {
                                                T const* curPixelPtr = reinterpret_cast<T const*>(curBytePtr);    
                                                uint8_t level = GetLevelInByte(curPixelPtr[0]);
                                                outImgBytePtr[(imageSize.x * y + x) * 4] = 255;
                                                outImgBytePtr[(imageSize.x * y + x) * 4 + 1] = 255;
                                                outImgBytePtr[(imageSize.x * y + x) * 4 + 2] = 255;
                                                outImgBytePtr[(imageSize.x * y + x) * 4 + 3] = level;  
                                            };
                    }
                    //Otherwise if alpha index is specified, use it
                    else
                    {
                        conversionFunc =    [&](uint8_t const* curBytePtr, int x, int y)
                                            {
                                                T const* curPixelPtr = reinterpret_cast<T const*>(curBytePtr);    
                                                uint8_t level = GetLevelInByte(curPixelPtr[format.IndexMono]);
                                                outImgBytePtr[(imageSize.x * y + x) * 4] = level;
                                                outImgBytePtr[(imageSize.x * y + x) * 4 + 1] = level;
                                                outImgBytePtr[(imageSize.x * y + x) * 4 + 2] = level;
                                                outImgBytePtr[(imageSize.x * y + x) * 4 + 3] = GetLevelInByte(curPixelPtr[format.IndexA]);
                                            };
                    }
                }
                else
                {
                    conversionFunc =    [&](uint8_t const* curBytePtr, int x, int y)
                                        {
                                            T const* curPixelPtr = reinterpret_cast<T const*>(curBytePtr);    
                                            uint8_t level = GetLevelInByte(curPixelPtr[0]);
                                            outImgBytePtr[(imageSize.x * y + x) * 4] = level;
                                            outImgBytePtr[(imageSize.x * y + x) * 4 + 1] = level;
                                            outImgBytePtr[(imageSize.x * y + x) * 4 + 2] = level;
                                            outImgBytePtr[(imageSize.x * y + x) * 4 + 3] = 255;
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
                                                outImgBytePtr[(imageSize.x * y + x) * 4] = GetLevelInByte(curPixelPtr[format.IndexR]);
                                                outImgBytePtr[(imageSize.x * y + x) * 4 + 1] = GetLevelInByte(curPixelPtr[format.IndexG]);
                                                outImgBytePtr[(imageSize.x * y + x) * 4 + 2] = GetLevelInByte(curPixelPtr[format.IndexB]);
                                                outImgBytePtr[(imageSize.x * y + x) * 4 + 3] = GetLevelInByte(curPixelPtr[format.IndexA]);
                                            };
                    }
                    else
                    {
                        conversionFunc =    [&](uint8_t const* curBytePtr, int x, int y)
                                            {
                                                T const* curPixelPtr = reinterpret_cast<T const*>(curBytePtr);
                                                
                                                uint8_t alpha = GetLevelInByte(curPixelPtr[format.IndexA]);
                                                float alphaInFloat =    static_cast<float>(curPixelPtr[format.IndexA]) / 
                                                                        static_cast<float>(std::numeric_limits<T>::max());

                                                outImgBytePtr[(imageSize.x * y + x) * 4] = GetReversedPreMultipliedLevelInByte(curPixelPtr[format.IndexR], alphaInFloat);
                                                outImgBytePtr[(imageSize.x * y + x) * 4 + 1] = GetReversedPreMultipliedLevelInByte(curPixelPtr[format.IndexG], alphaInFloat);
                                                outImgBytePtr[(imageSize.x * y + x) * 4 + 2] = GetReversedPreMultipliedLevelInByte(curPixelPtr[format.IndexB], alphaInFloat);
                                                outImgBytePtr[(imageSize.x * y + x) * 4 + 3] = alpha;
                                            };
                    }
                
                }
                else
                {
                    conversionFunc =    [&](uint8_t const* curBytePtr, int x, int y)
                                        {
                                            T const* curPixelPtr = reinterpret_cast<T const*>(curBytePtr);
                                            outImgBytePtr[(imageSize.x * y + x) * 4] = GetLevelInByte(curPixelPtr[format.IndexR]);
                                            outImgBytePtr[(imageSize.x * y + x) * 4 + 1] = GetLevelInByte(curPixelPtr[format.IndexG]);
                                            outImgBytePtr[(imageSize.x * y + x) * 4 + 2] = GetLevelInByte(curPixelPtr[format.IndexB]);
                                            outImgBytePtr[(imageSize.x * y + x) * 4 + 3] = 255;
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