#include "ssGUI/Backend/SFML/BackendImageSFML.hpp"

namespace ssGUI::Backend
{
    BackendImageSFML::BackendImageSFML(BackendImageSFML const& other)
    {
        GPUTexture = sf::Texture(other.GPUTexture);
        GPUTextureValid = other.GPUTextureValid;
    }
    
    BackendImageSFML::BackendImageSFML() : GPUTexture(), GPUTextureValid(false)
    {}

    BackendImageSFML::~BackendImageSFML()
    {}

    // sf::Texture* BackendImageSFML::GetGPUTextureP()
    // {
    //     return GPUTextureValid ? &GPUTexture : nullptr;
    // }

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
        if(GPUTexture.loadFromFile(path))
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

    bool BackendImageSFML::LoadRawFromMemory(void const * dataPtr, int width, int height)
    {
        sf::Image img;
        img.create(width, height, (sf::Uint8*)dataPtr);

        if(GPUTexture.loadFromImage(img))
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

    ssGUI::Backend::BackendImageInterface* BackendImageSFML::Clone()
    {
        return new BackendImageSFML(*this);
    }
}

