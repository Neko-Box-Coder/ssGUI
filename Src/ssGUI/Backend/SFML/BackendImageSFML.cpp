#include "ssGUI/Backend/SFML/BackendImageSFML.hpp"
#include "ssLogger/ssLog.hpp"

#include <functional>
#include <limits>

namespace ssGUI
{

namespace Backend
{
    template<typename T>
    uint8_t BackendImageSFML::GetReversedPreMultipliedLevelInByte(T val, float alpha)
    {
        return  static_cast<float>(val) / static_cast<float>(alpha) /               //Reverse the premultiplied alpha first
                static_cast<float>(std::numeric_limits<T>::max()) *                 //Then we scale it from 0 to 1
                static_cast<float>(std::numeric_limits<uint8_t>::max()) + 0.5f;     //Finally scale it back to 255
    }

    template<typename T>
    uint8_t BackendImageSFML::GetLevelInByte(T val)
    { 
        return  static_cast<float>(val) / static_cast<float>(std::numeric_limits<T>::max()) *   //Gets the value, divide by its own max,
                static_cast<float>(std::numeric_limits<uint8_t>::max()) + 0.5f;                 //Multiply by 255 in order to scale to 255
                                                                                                //and add 0.5 for rounding when casting back to uint8_t
    }

    template<typename T>
    bool BackendImageSFML::ConvertToRGBA32( sf::Image& outImg, void const * dataPtr, ssGUI::ImageFormat format, 
                                            glm::ivec2 imageSize, int rowPaddingInBytes)
    {
        //Set size first
        outImg.create(sf::Vector2u(imageSize.x, imageSize.y));
    
        uint8_t const* imgPtr = static_cast<uint8_t const*>(dataPtr);
        int bytePerPixel = format.BitPerPixel / 8;                              //Calculate how many bytes each pixel takes
        int rowNumOfBytes = bytePerPixel * imageSize.x + rowPaddingInBytes;     //Calculate how many bytes each row takes, normally called pitch
    
        std::function<void(uint8_t const*, int, int)> conversionFunc;           //function pointer for doing conversion
        
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
                                            outImg.setPixel(sf::Vector2u(x, y), sf::Color(level, level, level, level));
                                        };
                }
                //Otherwise if alpha index is specified, use it
                else
                {
                    conversionFunc =    [&](uint8_t const* curBytePtr, int x, int y)
                                        {
                                            T const* curPixelPtr = reinterpret_cast<T const*>(curBytePtr);    
                                            uint8_t level = GetLevelInByte(curPixelPtr[format.IndexMono]);
                                            outImg.setPixel(sf::Vector2u(x, y), sf::Color(level, level, level, GetLevelInByte(curPixelPtr[format.IndexA])));
                                        };
                }
            }
            else
            {
                conversionFunc =    [&](uint8_t const* curBytePtr, int x, int y)
                                    {
                                        T const* curPixelPtr = reinterpret_cast<T const*>(curBytePtr);    
                                        uint8_t level = GetLevelInByte(curPixelPtr[0]);
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
                                                            sf::Color(  GetLevelInByte(curPixelPtr[format.IndexR]), 
                                                                        GetLevelInByte(curPixelPtr[format.IndexG]), 
                                                                        GetLevelInByte(curPixelPtr[format.IndexB]),
                                                                        GetLevelInByte(curPixelPtr[format.IndexA])));
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
                                            
                                            outImg.setPixel(sf::Vector2u(x, y), 
                                                            sf::Color(  GetReversedPreMultipliedLevelInByte(curPixelPtr[format.IndexR], alphaInFloat), 
                                                                        GetReversedPreMultipliedLevelInByte(curPixelPtr[format.IndexG], alphaInFloat), 
                                                                        GetReversedPreMultipliedLevelInByte(curPixelPtr[format.IndexB], alphaInFloat),
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
                                                        sf::Color(  GetLevelInByte(curPixelPtr[format.IndexR]), 
                                                                    GetLevelInByte(curPixelPtr[format.IndexG]), 
                                                                    GetLevelInByte(curPixelPtr[format.IndexB])));
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

    BackendImageSFML::BackendImageSFML(BackendImageSFML const& other)
    {
        GPUTexture = sf::Texture(other.GPUTexture);
        MemoryImage = sf::Image(other.MemoryImage);
        GPUTextureValid = other.GPUTextureValid;
    }
    
    BackendImageSFML::BackendImageSFML() :  GPUTexture(),
                                            MemoryImage(),
                                            GPUTextureValid(false)
    {}

    BackendImageSFML::~BackendImageSFML()
    {}

    void* BackendImageSFML::GetRawHandle()
    {
        return GPUTextureValid ? &GPUTexture : nullptr;
    }

    bool BackendImageSFML::IsValid() const
    {
        return GPUTextureValid;
    }

    bool BackendImageSFML::LoadFromPath(std::string path)
    {
        if(GPUTexture.loadFromFile(std::filesystem::path(path)))
        {
            GPUTextureValid = true;
            return true;
        }
        //If fails to load, texture remains unchanged
        else
            return false;
    }

    bool BackendImageSFML::LoadImgFileFromMemory(void const * dataPtr, std::size_t size)
    {
        if(GPUTexture.loadFromMemory(dataPtr, size))
        {
            GPUTextureValid = true;
            return true;
        }
        //If fails to load, texture remains unchanged
        else
            return false;
    }

    bool BackendImageSFML::LoadRawFromMemory(void const * dataPtr, ssGUI::ImageFormat format, glm::ivec2 imageSize, int rowPaddingInBytes)
    {
        bool conversionNeeded = !(format.HasAlpha && 
                                format.BitDepthPerChannel == 8 && 
                                format.ImgType == ssGUI::Enums::ImageType::RGB &&
                                format.IndexR == 0 &&
                                format.IndexG == 1 &&
                                format.IndexB == 2 &&
                                format.IndexA == 3 &&
                                rowPaddingInBytes == 0);
    
        if(conversionNeeded)
        {
            //Check byte alignment
            if(format.BitDepthPerChannel % 8 != 0 || format.BitPerPixel % 8 != 0)
            {
                ssLOG_LINE("BitDepthPerChannel or BitPerPixel is not byte aligned, this is not supported.");
                ssLOG_LINE("Aborting...");
                return false;
            }
            
            bool result = false;
            

            switch(format.BitDepthPerChannel)
            {
                case 8:
                    result = ConvertToRGBA32<uint8_t>(MemoryImage, dataPtr, format, imageSize, rowPaddingInBytes);
                    break;
                case 16:
                    result = ConvertToRGBA32<uint16_t>(MemoryImage, dataPtr, format, imageSize, rowPaddingInBytes);
                    break;
                case 32:
                    result = ConvertToRGBA32<uint32_t>(MemoryImage, dataPtr, format, imageSize, rowPaddingInBytes);
                    break;
                default:
                    ssLOG_LINE("Not supported BitDepthPerChannel: " << format.BitDepthPerChannel);
                    ssLOG_LINE("Aborting...");
                    return false;
            }
            
            if(!result)
                return false;
        }
        else
        {
            //Creates Host Image
            MemoryImage.create(sf::Vector2u(imageSize.x, imageSize.y), (uint8_t*)dataPtr);
        }
        
        //Creates GPU Image
        if(GPUTexture.loadFromImage(MemoryImage))
        {
            GPUTextureValid = true;
            return true;
        }
        //If fails to load, texture remains unchanged
        else
            return false;
    }

    glm::ivec2 BackendImageSFML::GetSize() const
    {
        if(GPUTextureValid)
        {
            sf::Vector2u size = GPUTexture.getSize();
            return glm::ivec2(size.x, size.y);
        }
        else
            return glm::ivec2();
    }

    void* BackendImageSFML::GetPixelPtr(ssGUI::ImageFormat& format) const
    {
        //TODO: See if there's any other way of doing this
        return GPUTextureValid ? const_cast<uint8_t*>(MemoryImage.getPixelsPtr()) : nullptr;
        //return nullptr;
    }

    ssGUI::Backend::BackendImageInterface* BackendImageSFML::Clone()
    {
        return new BackendImageSFML(*this);
    }
}

}