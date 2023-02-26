#ifndef SSGUI_IMAGE_UTIL_H
#define SSGUI_IMAGE_UTIL_H

#include "ssGUI/DataClasses/ImageFormat.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

#include "glm/vec2.hpp"

#include <cstdint>
#include <functional>
#include <limits>

#undef max //std::numeric_limits<T>::max() is colliding with max macro in windows.h

//namespace: ssGUI
namespace ssGUI
{
    //class: ssGUI::ImageUtil
    //This class is for internal use for the time being, and will be "opened" soon.
    class ImageUtil
    {
        private:
        template<typename InputType, typename OutputType>
        static OutputType GetLevelInType(InputType val)
        {                                                                                                   //Example OutputType: uint8_t
            return  static_cast<float>(val) / static_cast<float>(std::numeric_limits<InputType>::max()) *   //Gets the value, divide by its own max,
                    static_cast<float>(std::numeric_limits<OutputType>::max()) + 0.5f;                      //Multiply by 255 in order to scale to 255
                                                                                                            //and add 0.5 for rounding when casting back to uint8_t
        }
        
        template<typename InputType, typename OutputType>
        static OutputType GetReversedPreMultipliedLevelInType(InputType val, float alpha)
        {                                                                                   //Example OutputType: uint8_t
            return  static_cast<float>(val) / static_cast<float>(alpha) /                   //Reverse the premultiplied alpha first
                    static_cast<float>(std::numeric_limits<InputType>::max()) *             //Then we scale it from 0 to 1
                    static_cast<float>(std::numeric_limits<OutputType>::max()) + 0.5f;      //Finally scale it back to 255
        }

        //This is only used by Freetype, it is assumed pixels are arranged from MSB to LSB
        template<typename OutputChannelType>
        static bool ConvertBitmapToMono(void* outImg,
                                        void const * inImg,
                                        glm::ivec2 imageSize,
                                        ssGUI::ImageFormat outputFormat)
        {
            OutputChannelType* outImgPixelPtr = static_cast<OutputChannelType*>(outImg);
            
            if(outputFormat.ImgType != ssGUI::Enums::ImageType::MONO)
            {
                ssGUI_WARNING(ssGUI_IMAGE_UTIL_TAG, "Invalid output format");
                return false;
            }
            
            std::function<void(uint8_t const*, int, int)> conversionFunc;                   //function pointer for doing conversion
            
            if(outputFormat.HasAlpha)
            {
                conversionFunc =    [&](uint8_t const* curPixelPtr, int x, int y)
                                    {
                                        //Each pixel pointer represents 8 pixels in bit map
                                        for(int i = 0; i < 8; i++)
                                        {
                                            uint8_t curPix = curPixelPtr[i] >> (8 - i) & 0x1;
                                            outImgPixelPtr[(imageSize.x * y + x + i) * 2 + outputFormat.IndexMono] = std::numeric_limits<OutputChannelType>::max();
                                            outImgPixelPtr[(imageSize.x * y + x + i) * 2 + outputFormat.IndexA] =  curPix > 0 ? std::numeric_limits<OutputChannelType>::max() : 0;
                                        }
                                    };
            }
            else
            {
                conversionFunc =    [&](uint8_t const* curPixelPtr, int x, int y)
                                    {
                                        //Each pixel pointer represents 8 pixels in bit map
                                        for(int i = 0; i < 8; i++)
                                        {
                                            uint8_t curPix = curPixelPtr[i] >> (8 - i) & 0x1;
                                            ssLOG_LINE("("<<(x+i)<<", "<<y<<"): "<<curPix);
                                            outImgPixelPtr[(imageSize.x * y + x + i) * 1 + outputFormat.IndexMono] = curPix > 0 ? std::numeric_limits<OutputChannelType>::max() : 0;
                                        }
                                    };
            }
            
            //Then we call the conversion function for each pixel
            for(int y = 0; y < imageSize.y; y++)
            {
                int currentRowBytes = y * imageSize.x / 8;
                for(int x = 0; x < imageSize.x; x += 8)
                {
                    uint8_t const* curPixelPtr = reinterpret_cast<uint8_t const*>(inImg);
                    curPixelPtr = curPixelPtr + currentRowBytes + x;
                    conversionFunc(curPixelPtr, x, y);
                }
            }
            
            return true;
        }

        //outImg must be allocated already before passing to this function
        template<typename InputChannelType, typename OutputChannelType>
        static bool ConvertToRGB(   void* outImg, 
                                    void const * inImg, 
                                    ssGUI::ImageFormat format, 
                                    glm::ivec2 imageSize, 
                                    ssGUI::ImageFormat outputFormat)        //For specifying the indices of the RGB(A) channels
        {
            InputChannelType const* imgPtr = static_cast<InputChannelType const*>(inImg);
            OutputChannelType* outImgPixelPtr = static_cast<OutputChannelType*>(outImg);

            int bytePerPixel = format.BitPerPixel / 8;                                      //Calculate how many bytes each pixel takes
            int rowNumOfBytes = bytePerPixel * imageSize.x + format.RowPaddingInBytes;      //Calculate how many bytes each row takes, normally called pitch
        
            std::function<void(InputChannelType const*, int, int)> conversionFunc;                   //function pointer for doing conversion
            
            //Validation
            if( outputFormat.ImgType != ssGUI::Enums::ImageType::RGB ||
                outputFormat.IndexR < 0 ||
                outputFormat.IndexG < 0 ||
                outputFormat.IndexB < 0)
            {
                ssGUI_WARNING(ssGUI_IMAGE_UTIL_TAG, "Invalid output format");
                return false;
            }

            //Need to use a bit of macro to make it a bit shorter :/
            #undef GetLevelInType
            #undef GetReversedPreMultipliedLevelInType
            #define GetLevelInType GetLevelInType<InputChannelType, OutputChannelType>
            #define GetReversedPreMultipliedLevelInType GetReversedPreMultipliedLevelInType<InputChannelType, OutputChannelType>

            //First assign the conversion function depending on the image type and alpha
            //Grayscale
            if(format.ImgType == ssGUI::Enums::ImageType::MONO)
            {
                if(format.HasAlpha)
                {
                    //If the alpha index is not specified, we assume black is transparent
                    if(format.IndexA < 0)
                    {
                        //If we are outputing RGBA
                        if(outputFormat.IndexA >= 0)
                        {
                            conversionFunc =    [&](InputChannelType const* curPixelPtr, int x, int y)
                                                {
                                                    OutputChannelType level = GetLevelInType(curPixelPtr[0]);
                                                    outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexR] = std::numeric_limits<OutputChannelType>::max();
                                                    outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexG] = std::numeric_limits<OutputChannelType>::max();
                                                    outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexB] = std::numeric_limits<OutputChannelType>::max();                            
                                                    outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexA] = level;  
                                                };
                        }
                        //If we are outputing RGB
                        else
                        {
                            conversionFunc =    [&](InputChannelType const* curPixelPtr, int x, int y)
                                                {
                                                    OutputChannelType level = GetLevelInType(curPixelPtr[0]);
                                                    outImgPixelPtr[(imageSize.x * y + x) * 3 + outputFormat.IndexR] = std::numeric_limits<OutputChannelType>::max();
                                                    outImgPixelPtr[(imageSize.x * y + x) * 3 + outputFormat.IndexG] = std::numeric_limits<OutputChannelType>::max();
                                                    outImgPixelPtr[(imageSize.x * y + x) * 3 + outputFormat.IndexB] = std::numeric_limits<OutputChannelType>::max();                            
                                                };
                        }
                    }
                    //Otherwise if alpha index is specified, use it
                    else
                    {
                        //If we are outputing RGBA
                        if(outputFormat.IndexA >= 0)
                        {
                            conversionFunc =    [&](InputChannelType const* curPixelPtr, int x, int y)
                                                {
                                                    OutputChannelType level = GetLevelInType(curPixelPtr[format.IndexMono]);
                                                    outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexR] = level;
                                                    outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexG] = level;
                                                    outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexB] = level;
                                                    outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexA] = GetLevelInType(curPixelPtr[format.IndexA]);
                                                };
                        }
                        //If we are outputing RGB
                        else
                        {
                            conversionFunc =    [&](InputChannelType const* curPixelPtr, int x, int y)
                                                {
                                                    OutputChannelType level = GetLevelInType(curPixelPtr[format.IndexMono]);
                                                    outImgPixelPtr[(imageSize.x * y + x) * 3 + outputFormat.IndexR] = level;
                                                    outImgPixelPtr[(imageSize.x * y + x) * 3 + outputFormat.IndexG] = level;
                                                    outImgPixelPtr[(imageSize.x * y + x) * 3 + outputFormat.IndexB] = level;
                                                };
                        }
                    }
                }
                //If the input mono image doesn't have alpha
                else
                {
                    //If we are outputing RGBA
                    if(outputFormat.IndexA >= 0)
                    {
                        conversionFunc =    [&](InputChannelType const* curPixelPtr, int x, int y)
                                            {
                                                OutputChannelType level = GetLevelInType(curPixelPtr[0]);
                                                outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexR] = level;
                                                outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexG] = level;
                                                outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexB] = level;
                                                outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexA] = std::numeric_limits<OutputChannelType>::max();
                                            };
                    }
                    //If we are outputing RGB
                    else
                    {
                        conversionFunc =    [&](InputChannelType const* curPixelPtr, int x, int y)
                                            {
                                                OutputChannelType level = GetLevelInType(curPixelPtr[0]);
                                                outImgPixelPtr[(imageSize.x * y + x) * 3 + outputFormat.IndexR] = level;
                                                outImgPixelPtr[(imageSize.x * y + x) * 3 + outputFormat.IndexG] = level;
                                                outImgPixelPtr[(imageSize.x * y + x) * 3 + outputFormat.IndexB] = level;
                                            };
                    }
                }
            }
            //RGB
            else
            {
                if(format.HasAlpha)
                {
                    if(!format.PreMultipliedAlpha)
                    {
                        //If we are outputing RGBA
                        if(outputFormat.IndexA >= 0)
                        {
                            conversionFunc =    [&](InputChannelType const* curPixelPtr, int x, int y)
                                                {
                                                    outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexR] = GetLevelInType(curPixelPtr[format.IndexR]);
                                                    outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexG] = GetLevelInType(curPixelPtr[format.IndexG]);
                                                    outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexB] = GetLevelInType(curPixelPtr[format.IndexB]);
                                                    outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexA] = GetLevelInType(curPixelPtr[format.IndexA]);
                                                };
                        }
                        //If we are outputing RGB
                        else
                        {
                            conversionFunc =    [&](InputChannelType const* curPixelPtr, int x, int y)
                                                {
                                                    outImgPixelPtr[(imageSize.x * y + x) * 3 + outputFormat.IndexR] = GetLevelInType(curPixelPtr[format.IndexR]);
                                                    outImgPixelPtr[(imageSize.x * y + x) * 3 + outputFormat.IndexG] = GetLevelInType(curPixelPtr[format.IndexG]);
                                                    outImgPixelPtr[(imageSize.x * y + x) * 3 + outputFormat.IndexB] = GetLevelInType(curPixelPtr[format.IndexB]);
                                                };
                        }
                    }
                    else
                    {
                        //If we are outputing RGBA
                        if(outputFormat.IndexA >= 0)
                        {
                            conversionFunc =    [&](InputChannelType const* curPixelPtr, int x, int y)
                                                {  
                                                    OutputChannelType alpha = GetLevelInType(curPixelPtr[format.IndexA]);
                                                    float alphaInFloat =    static_cast<float>(curPixelPtr[format.IndexA]) / 
                                                                            static_cast<float>(std::numeric_limits<InputChannelType>::max());

                                                    outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexR] = GetReversedPreMultipliedLevelInType(curPixelPtr[format.IndexR], alphaInFloat);
                                                    outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexG] = GetReversedPreMultipliedLevelInType(curPixelPtr[format.IndexG], alphaInFloat);
                                                    outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexB] = GetReversedPreMultipliedLevelInType(curPixelPtr[format.IndexB], alphaInFloat);
                                                    outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexA] = alpha;
                                                };
                        }
                        //If we are outputing RGB
                        else
                        {
                            conversionFunc =    [&](InputChannelType const* curPixelPtr, int x, int y)
                                                {  
                                                    float alphaInFloat =    static_cast<float>(curPixelPtr[format.IndexA]) / 
                                                                            static_cast<float>(std::numeric_limits<InputChannelType>::max());

                                                    outImgPixelPtr[(imageSize.x * y + x) * 3 + outputFormat.IndexR] = GetReversedPreMultipliedLevelInType(curPixelPtr[format.IndexR], alphaInFloat);
                                                    outImgPixelPtr[(imageSize.x * y + x) * 3 + outputFormat.IndexG] = GetReversedPreMultipliedLevelInType(curPixelPtr[format.IndexG], alphaInFloat);
                                                    outImgPixelPtr[(imageSize.x * y + x) * 3 + outputFormat.IndexB] = GetReversedPreMultipliedLevelInType(curPixelPtr[format.IndexB], alphaInFloat);
                                                };
                        }
                    }
                }
                //If the input RGB image doesn't have alpha
                else
                {
                    //If we are outputing RGBA
                    if(outputFormat.IndexA >= 0)
                    {
                        conversionFunc =    [&](InputChannelType const* curPixelPtr, int x, int y)
                                            {
                                                outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexR] = GetLevelInType(curPixelPtr[format.IndexR]);
                                                outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexG] = GetLevelInType(curPixelPtr[format.IndexG]);
                                                outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexB] = GetLevelInType(curPixelPtr[format.IndexB]);
                                                outImgPixelPtr[(imageSize.x * y + x) * 4 + outputFormat.IndexA] = std::numeric_limits<OutputChannelType>::max();
                                            };
                    }
                    //If we are outputing RGB
                    else
                    {
                        conversionFunc =    [&](InputChannelType const* curPixelPtr, int x, int y)
                                            {
                                                outImgPixelPtr[(imageSize.x * y + x) * 3 + outputFormat.IndexR] = GetLevelInType(curPixelPtr[format.IndexR]);
                                                outImgPixelPtr[(imageSize.x * y + x) * 3 + outputFormat.IndexG] = GetLevelInType(curPixelPtr[format.IndexG]);
                                                outImgPixelPtr[(imageSize.x * y + x) * 3 + outputFormat.IndexB] = GetLevelInType(curPixelPtr[format.IndexB]);
                                            };
                    }
                }
            }
        
            //Then we call the conversion function for each pixel
            for(int y = 0; y < imageSize.y; y++)
            {
                int currentRowBytes = y * rowNumOfBytes;
                for(int x = 0; x < imageSize.x; x++)
                {
                    uint8_t const* curPixelPtr = reinterpret_cast<uint8_t const*>(imgPtr);
                    curPixelPtr = curPixelPtr + currentRowBytes + x * bytePerPixel;
                    InputChannelType const* curPixelPtrConverted = reinterpret_cast<InputChannelType const*>(curPixelPtr);
                    conversionFunc(curPixelPtrConverted, x, y);
                }
            }
            
            #undef GetLevelInType
            #undef GetReversedPreMultipliedLevelInType
            
            return true;
        }
        
        #define CONVERT_MONO_IF_NEEDED(dummyBool)\
        if(format.ImgType == ssGUI::Enums::ImageType::MONO && format.BitDepthPerChannel == 1)\
        {\
            ssGUI::ImageFormat outputFormat;\
            outputFormat.ImgType = ssGUI::Enums::ImageType::MONO;\
            outputFormat.BitDepthPerChannel = 8;\
            outputFormat.BitPerPixel = 8;\
            outputFormat.HasAlpha = false;\
            outputFormat.IndexMono = 0;\
            \
            uint8_t* monoImgPtr = new uint8_t[imageSize.x * imageSize.y];\
            bool result = ssGUI::ImageUtil::ConvertBitmapToMono<uint8_t>(monoImgPtr, inImg, imageSize, outputFormat);\
            if(!result)\
            {\
                delete[] monoImgPtr;\
                ssGUI_WARNING(ssGUI_IMAGE_UTIL_TAG, "Failed to convert bitmap to mono");\
                return result; \
            }\
            \
            inPtrReplaced = true;\
            format = outputFormat;\
            inImg = monoImgPtr;\
        }
        
        public:        
        static bool ConvertToRGBA32(void* outImg, void const * inImg, ssGUI::ImageFormat format, 
                                    glm::ivec2 imageSize)
        {
            bool inPtrReplaced = false;
            
            CONVERT_MONO_IF_NEEDED(inPtrReplaced);
            
            ssGUI::ImageFormat outputFormat;
            switch(format.BitDepthPerChannel)
            {
                case 8:
                {
                    bool result = ssGUI::ImageUtil::ConvertToRGB<uint8_t, uint8_t>(outImg, inImg, format, imageSize, outputFormat); 
                    if(inPtrReplaced)
                        delete[] reinterpret_cast<uint8_t*>(const_cast<void*>(inImg));
                    return result; 
                }
                case 16:
                {
                    bool result = ssGUI::ImageUtil::ConvertToRGB<uint16_t, uint8_t>(outImg, inImg, format, imageSize, outputFormat);
                    if(inPtrReplaced)
                        delete[] reinterpret_cast<uint8_t*>(const_cast<void*>(inImg));
                    return result; 
                }
                default:
                {
                    if(inPtrReplaced)
                        delete[] reinterpret_cast<uint8_t*>(const_cast<void*>(inImg));
                    ssGUI_WARNING(ssGUI_IMAGE_UTIL_TAG, "Unsupported bit depth");
                    return false;
                }
            }   
        }
        
        static bool ConvertToBGRA32(void* outImg, void const * inImg, ssGUI::ImageFormat format, 
                                    glm::ivec2 imageSize)
        {
            bool inPtrReplaced = false;
            
            CONVERT_MONO_IF_NEEDED(inPtrReplaced);
        
            ssGUI::ImageFormat outputFormat;
            outputFormat.IndexB = 0;
            outputFormat.IndexR = 1;
            outputFormat.IndexG = 2;
            outputFormat.IndexA = 3;

            switch(format.BitDepthPerChannel)
            {
                case 8:
                {
                    bool result = ssGUI::ImageUtil::ConvertToRGB<uint8_t, uint8_t>(outImg, inImg, format, imageSize, outputFormat);
                    if(inPtrReplaced)
                        delete[] reinterpret_cast<uint8_t*>(const_cast<void*>(inImg));
                    return result;
                }
                case 16:
                {
                    bool result = ssGUI::ImageUtil::ConvertToRGB<uint16_t, uint8_t>(outImg, inImg, format, imageSize, outputFormat);
                    if(inPtrReplaced)
                        delete[] reinterpret_cast<uint8_t*>(const_cast<void*>(inImg));
                    return result;
                }
                default:
                {
                    if(inPtrReplaced)
                        delete[] reinterpret_cast<uint8_t*>(const_cast<void*>(inImg));
                    ssGUI_WARNING(ssGUI_IMAGE_UTIL_TAG, "Unsupported bit depth");
                    return false;
                }
            }
        }
        
        static bool ConvertToARGB32(void* outImg, void const * inImg, ssGUI::ImageFormat format, 
                                    glm::ivec2 imageSize)
        {
            bool inPtrReplaced = false;
            
            CONVERT_MONO_IF_NEEDED(inPtrReplaced);
        
            ssGUI::ImageFormat outputFormat;
            outputFormat.IndexA = 0;
            outputFormat.IndexR = 1;
            outputFormat.IndexG = 2;
            outputFormat.IndexB = 3;

            switch(format.BitDepthPerChannel)
            {
                case 8:
                {
                    bool result = ssGUI::ImageUtil::ConvertToRGB<uint8_t, uint8_t>(outImg, inImg, format, imageSize, outputFormat);
                    if(inPtrReplaced)
                        delete[] reinterpret_cast<uint8_t*>(const_cast<void*>(inImg));
                    return result;
                }
                case 16:
                {
                    bool result = ssGUI::ImageUtil::ConvertToRGB<uint16_t, uint8_t>(outImg, inImg, format, imageSize, outputFormat);
                    if(inPtrReplaced)
                        delete[] reinterpret_cast<uint8_t*>(const_cast<void*>(inImg));
                    return result;
                }
                default:
                {
                    if(inPtrReplaced)
                        delete[] reinterpret_cast<uint8_t*>(const_cast<void*>(inImg));
                    ssGUI_WARNING(ssGUI_IMAGE_UTIL_TAG, "Unsupported bit depth");
                    return false;
                }
            }
        }

        static void ResizeBilinear(const uint8_t* inputPixels, int w, int h, uint8_t* outputPixels, int w2, int h2)
        {
            ssLOG_FUNC_ENTRY();
            const uint8_t* a;
            const uint8_t* b;
            const uint8_t* c;
            const uint8_t* d; 
            int x, y, index;
            float x_ratio = ((float)(w/* - 1*/)) / (float)w2 ;
            float y_ratio = ((float)(h/* - 1*/)) / (float)h2 ;
            float x_diff, y_diff, blue, red, green, alpha;
            int offset = 0;
            bool aValid, bValid, cValid, dValid;
            for (int i = 0; i < h2; i++) 
            {
                for (int j = 0; j < w2; j++) 
                {
                    x =         (int)(x_ratio * j) ;
                    y =         (int)(y_ratio * i) ;
                    x_diff =    (x_ratio * j) - x ;
                    y_diff =    (y_ratio * i) - y ;
                    index =     (y * w + x) ;                
                    a =         &inputPixels[index * 4] ;
                    b =         &inputPixels[(index + 1) * 4] ;
                    c =         &inputPixels[(index + w) * 4] ;
                    d =         &inputPixels[(index + w + 1) * 4] ;

                    //Make sure pixels with 0 alpha are not affecting any of the sampling
                    aValid = *(a + 3) > 0;
                    bValid = *(b + 3) > 0;
                    cValid = *(c + 3) > 0;
                    dValid = *(d + 3) > 0;

                    float inverseWidthAndHight =    (1 - x_diff) * (1 - y_diff);
                    float widthAndInverseHeight =   (x_diff) * (1 - y_diff);
                    float heightAndInverseWidth =   (y_diff) * (1 - x_diff);
                    float widthHeight =             (x_diff * y_diff);
                    float total =   inverseWidthAndHight * aValid+ 
                                    widthAndInverseHeight * bValid+ 
                                    heightAndInverseWidth * cValid+ 
                                    widthHeight * dValid;

                    // red element
                    // Yr = Ar(1-w)(1-h) + Br(w)(1-h) + Cr(h)(1-w) + Dr(wh)
                    red =   *(a + 0) * inverseWidthAndHight * aValid +
                            *(b + 0) * widthAndInverseHeight * bValid +
                            *(c + 0) * heightAndInverseWidth * cValid + 
                            *(d + 0) * widthHeight * dValid;
                    if(red > 0)
                        red /= total;

                    // green element
                    // Yg = Ag(1-w)(1-h) + Bg(w)(1-h) + Cg(h)(1-w) + Dg(wh)
                    green = *(a + 1) * inverseWidthAndHight * aValid +
                            *(b + 1) * widthAndInverseHeight * bValid +
                            *(c + 1) * heightAndInverseWidth * cValid +
                            *(d + 1) * widthHeight * dValid;
                    if(green > 0)
                        green /= total;

                    // blue element
                    // Yb = Ab(1-w)(1-h) + Bb(w)(1-h) + Cb(h)(1-w) + Db(wh)
                    blue =  *(a + 2) * inverseWidthAndHight * aValid +
                            *(b + 2) * widthAndInverseHeight * bValid +
                            *(c + 2) * heightAndInverseWidth * cValid +
                            *(d + 2) * widthHeight * dValid;
                    if(blue > 0)
                        blue /= total;

                    // alpha element
                    //Using nearest neighbour for alpha otherwise it will show the color for pixels we are sampling that have 0 alpha
                    // if(inverseWidthAndHight > 0.25)
                    //     alpha = *(a + 3);
                    // else if(widthAndInverseHeight > 0.25)
                    //     alpha = *(b + 3);
                    // else if(heightAndInverseWidth > 0.25)
                    //     alpha = *(c + 3);
                    // else
                    //     alpha = *(d + 3);

                    //Ya = Aa(1-w)(1-h) + Ba(w)(1-h) + Ca(h)(1-w) + Da(wh)
                    alpha = *(a + 3) * inverseWidthAndHight + 
                            *(b + 3) * widthAndInverseHeight +
                            *(c + 3) * heightAndInverseWidth + 
                            *(d + 3) * widthHeight;
                                    
                    //ssGUI_DEBUG("Pixel["<<i<<"]["<<j<<"]: ("<<red<<", "<<green<<", "<<blue<<", "<<alpha<<")");

                    // range is 0 to 255 thus bitwise AND with 0xff
                    outputPixels[offset * 4] =      (uint8_t)(((int)red) & 0xff);
                    outputPixels[offset * 4 + 1] =  (uint8_t)(((int)green) & 0xff);
                    outputPixels[offset * 4 + 2] =  (uint8_t)(((int)blue) & 0xff);
                    outputPixels[offset * 4 + 3] =  (uint8_t)(((int)alpha) & 0xff);
                    offset++;
                }
            }
            ssLOG_FUNC_EXIT();
        }

        //Assuming is RGBA32
        static void ResizeRGBA(const uint8_t* inputPixels, int w, int h, uint8_t* outputPixels, int w2, int h2)
        {
            //temporary image pointers for resizing
            uint8_t* imgPtr = new uint8_t[w * h * 4];
            uint8_t* imgPtr1 = new uint8_t[1];
            uint8_t** imgPtrArr[] = {&imgPtr, &imgPtr1};
            int allocationCounter[] = {1, 1};

            //Flag for indicating which pointer has just been populated
            int populatedImg = 0;

            //Populate the first temporary image pointer
            for(int i = 0; i < w * h * 4; i++)
                imgPtr[i] = (*(inputPixels + i));

            //Record the current image size
            glm::ivec2 currentSize = glm::ivec2(w, h);

            //Resize width until the new cursor size is within 2x or 0.5x
            while ((currentSize.x < w2 && currentSize.x * 2 < w2) ||
                    (currentSize.x > w2 && (int)(currentSize.x * 0.5) > w2))
            {
                delete[] *imgPtrArr[(populatedImg + 1) % 2];
                allocationCounter[(populatedImg + 1) % 2]--;
                    
                //Enlarging
                if(currentSize.x < w2)
                {
                    *imgPtrArr[(populatedImg + 1) % 2] = new uint8_t[currentSize.x * 2 * currentSize.y * 4];
                    allocationCounter[(populatedImg + 1) % 2]++;

                    ResizeBilinear
                    (
                        *imgPtrArr[populatedImg], 
                        currentSize.x, 
                        currentSize.y,
                        *imgPtrArr[(populatedImg + 1) % 2],
                        currentSize.x * 2,
                        currentSize.y
                    );

                    currentSize.x *= 2;
                }
                //Reducing
                else
                {
                    *imgPtrArr[(populatedImg + 1) % 2] = new uint8_t[(int)(currentSize.x * 0.5) * currentSize.y * 4];
                    allocationCounter[(populatedImg + 1) % 2]++;

                    ResizeBilinear
                    (
                        *imgPtrArr[populatedImg], 
                        currentSize.x, 
                        currentSize.y,
                        *imgPtrArr[(populatedImg + 1) % 2],
                        currentSize.x * 0.5,
                        currentSize.y
                    );

                    currentSize.x *= 0.5;
                }

                populatedImg = (populatedImg + 1) % 2;
            }
                
            //Resize height until the new cursor size is within 2x or 0.5x
            while ((currentSize.y < h2 && currentSize.y * 2 < h2) ||
                    (currentSize.y > h2 && (int)(currentSize.y * 0.5) > h2))
            {
                delete[] *imgPtrArr[(populatedImg + 1) % 2];
                allocationCounter[(populatedImg + 1) % 2]--;
                    
                //Enlarging
                if(currentSize.y < h2)
                {
                    *imgPtrArr[(populatedImg + 1) % 2] = new uint8_t[currentSize.x * currentSize.y * 2 * 4];
                    allocationCounter[(populatedImg + 1) % 2]++;

                    ResizeBilinear
                    (
                        *imgPtrArr[populatedImg], 
                        currentSize.x, 
                        currentSize.y,
                        *imgPtrArr[(populatedImg + 1) % 2],
                        currentSize.x,
                        currentSize.y * 2
                    );

                    currentSize.y *= 2;
                }
                //Reducing
                else
                {
                    *imgPtrArr[(populatedImg + 1) % 2] = new uint8_t[currentSize.x * (int)(currentSize.y * 0.5) * 4];
                    allocationCounter[(populatedImg + 1) % 2]++;

                    ResizeBilinear
                    (
                        *imgPtrArr[populatedImg],
                        currentSize.x, 
                        currentSize.y,
                        *imgPtrArr[(populatedImg + 1) % 2],
                        currentSize.x,
                        currentSize.y * 0.5
                    );

                    currentSize.y *= 0.5;
                }

                populatedImg = (populatedImg + 1) % 2;
            }

            //Do the final round of resizing
            ResizeBilinear( *imgPtrArr[populatedImg], 
                            currentSize.x, 
                            currentSize.y, 
                            outputPixels, 
                            w2, 
                            h2);

            delete[] imgPtr;
            delete[] imgPtr1;
        }
    };
}


#endif