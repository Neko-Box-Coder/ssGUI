#ifndef SSGUI_IMAGE_UTIL_H
#define SSGUI_IMAGE_UTIL_H

#include "ssGUI/DataClasses/ImageFormat.hpp"
#include "ssLogger/ssLog.hpp"

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

        static bool ConvertToBGRA32(void* outImg, void const * dataPtr, ssGUI::ImageFormat format, 
                                    glm::ivec2 imageSize)
        {
            int indexR = format.IndexR;
            format.IndexR = format.IndexB;
            format.IndexB = indexR;

            switch(format.BitDepthPerChannel)
            {
                case 8:
                    return ssGUI::ImageUtil::ConvertToRGBA32<uint8_t>(outImg, dataPtr, format, imageSize);
                case 16:
                    return ssGUI::ImageUtil::ConvertToRGBA32<uint16_t>(outImg, dataPtr, format, imageSize);
                default:
                    ssLOG_LINE("Unsupported bit depth");
                    return false;
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
            float x_ratio = ((float)(w - 1)) / w2 ;
            float y_ratio = ((float)(h - 1)) / h2 ;
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
                                    
                    //ssLOG_LINE("Pixel["<<i<<"]["<<j<<"]: ("<<red<<", "<<green<<", "<<blue<<", "<<alpha<<")");

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
        static void Resize(const uint8_t* inputPixels, int w, int h, uint8_t* outputPixels, int w2, int h2)
        {
            //temporary image pointers for resizing
            uint8_t* imgPtr = new uint8_t[w * h * 4];
            uint8_t* imgPtr1 = new uint8_t[1];
            uint8_t* imgPtrArr[] = {imgPtr, imgPtr1};

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
                delete[] imgPtrArr[(populatedImg + 1) % 2];
                    
                //Enlarging
                if(currentSize.x < w2)
                {
                    imgPtrArr[(populatedImg + 1) % 2] = new uint8_t[currentSize.x * 2 * currentSize.y * 4];

                    ResizeBilinear
                    (
                        imgPtrArr[populatedImg], 
                        currentSize.x, 
                        currentSize.y,
                        imgPtrArr[(populatedImg + 1) % 2],
                        currentSize.x * 2,
                        currentSize.y
                    );

                    currentSize.x *= 2;
                }
                //Reducing
                else
                {
                    imgPtrArr[(populatedImg + 1) % 2] = new uint8_t[(int)(currentSize.x * 0.5) * currentSize.y * 4];

                    ResizeBilinear
                    (
                        imgPtrArr[populatedImg], 
                        currentSize.x, 
                        currentSize.y,
                        imgPtrArr[(populatedImg + 1) % 2],
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
                delete[] imgPtrArr[(populatedImg + 1) % 2];
                    
                //Enlarging
                if(currentSize.y < h2)
                {
                    imgPtrArr[(populatedImg + 1) % 2] = new uint8_t[currentSize.x * currentSize.y * 2 * 4];

                    ResizeBilinear
                    (
                        imgPtrArr[populatedImg], 
                        currentSize.x, 
                        currentSize.y,
                        imgPtrArr[(populatedImg + 1) % 2],
                        currentSize.x,
                        currentSize.y * 2
                    );

                    currentSize.y *= 2;
                }
                //Reducing
                else
                {
                    imgPtrArr[(populatedImg + 1) % 2] = new uint8_t[currentSize.x * (int)(currentSize.y * 0.5) * 4];

                    ResizeBilinear
                    (
                        imgPtrArr[populatedImg],
                        currentSize.x, 
                        currentSize.y,
                        imgPtrArr[(populatedImg + 1) % 2],
                        currentSize.x,
                        currentSize.y * 0.5
                    );

                    currentSize.y *= 0.5;
                }

                populatedImg = (populatedImg + 1) % 2;
            }

            //Do the final round of resizing
            ResizeBilinear( imgPtrArr[populatedImg], 
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