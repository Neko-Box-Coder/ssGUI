#include "ssGUI/Backend/SFML/BackendImageSFML.hpp"

namespace ssGUI::Backend
{
    BackendImageSFML::BackendImageSFML(BackendImageSFML const& other)
    {
        GPUTexture = sf::Texture(other.GPUTexture);
        MemoryImage = sf::Image(other.MemoryImage);
        GPUTextureValid = other.GPUTextureValid;
    }
    
    BackendImageSFML::BackendImageSFML()
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
        if(!MemoryImage.loadFromFile(std::filesystem::path(path)))
            return false;

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
        if(!MemoryImage.loadFromMemory(dataPtr, size))
            return false;
        
        if(GPUTexture.loadFromMemory(dataPtr, size))
        {
            GPUTextureValid = true;
            return true;
        }
        //If fails to load, texture remains unchanged
        else
            return false;
    }

    bool BackendImageSFML::LoadRawFromMemory(void const * dataPtr, int width, int height)
    {
        MemoryImage.create(width, height, (sf::Uint8*)dataPtr);
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

    const void* BackendImageSFML::GetPixelPtr() const
    {
        return GPUTextureValid ? MemoryImage.getPixelsPtr() : nullptr;
    }

    ssGUI::Backend::BackendImageInterface* BackendImageSFML::Clone()
    {
        return new BackendImageSFML(*this);
    }
}

