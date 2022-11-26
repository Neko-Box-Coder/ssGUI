#include "ssGUI/Backend/SFML/BackendImageSFML.hpp"
#include "ssGUI/HelperClasses/SFMLImageConversion.hpp"

#include "ssLogger/ssLog.hpp"

#include <functional>
#include <limits>

namespace ssGUI
{

namespace Backend
{
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

    bool BackendImageSFML::LoadRawFromMemory(void const * dataPtr, ssGUI::ImageFormat format, glm::ivec2 imageSize)
    {
        bool conversionNeeded = !(format.HasAlpha && 
                                format.BitDepthPerChannel == 8 && 
                                format.ImgType == ssGUI::Enums::ImageType::RGB &&
                                format.IndexR == 0 &&
                                format.IndexG == 1 &&
                                format.IndexB == 2 &&
                                format.IndexA == 3 &&
                                format.RowPaddingInBytes == 0);
    
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
                    result = ssGUI::SFMLImageConversion::ConvertToRGBA32<uint8_t>(MemoryImage, dataPtr, format, imageSize);
                    break;
                case 16:
                    result = ssGUI::SFMLImageConversion::ConvertToRGBA32<uint16_t>(MemoryImage, dataPtr, format, imageSize);
                    break;
                case 32:
                    result = ssGUI::SFMLImageConversion::ConvertToRGBA32<uint32_t>(MemoryImage, dataPtr, format, imageSize);
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

    void BackendImageSFML::AddBackendDrawingLinking(ssGUI::Backend::BackendDrawingInterface* backendDrawing)
    {
        //This is only used natively with SFML, no need to link to backend drawing
    }

    void BackendImageSFML::RemoveBackendDrawingLinking(ssGUI::Backend::BackendDrawingInterface* backendDrawing)
    {
        //This is only used natively with SFML, no need to link to backend drawing
    }

    ssGUI::Backend::BackendImageInterface* BackendImageSFML::Clone()
    {
        return new BackendImageSFML(*this);
    }
}

}