#include "ssGUI/Backend/SFML/BackendImageSFML.hpp"
#include "ssGUI/HelperClasses/ImageUtil.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

#include <functional>
#include <limits>

namespace ssGUI
{

namespace Backend
{
    BackendImageSFML::BackendImageSFML(BackendImageSFML const& other) : GPUTexture(),
                                                                        MemoryImage(),
                                                                        GPUTextureValid(false)
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
            bool result = false;
            
            uint8_t* convertedRawImg = new uint8_t[imageSize.x * imageSize.y * 4];

            result = ssGUI::ImageUtil::ConvertToRGBA32(convertedRawImg, dataPtr, format, imageSize);
            if(!result)
            {
                delete[] convertedRawImg;
                return false;
            }
            else
            {
                MemoryImage.create(sf::Vector2u(imageSize.x, imageSize.y), convertedRawImg);
                delete[] convertedRawImg;
            }
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