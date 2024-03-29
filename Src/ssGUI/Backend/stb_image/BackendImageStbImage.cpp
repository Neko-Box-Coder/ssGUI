#include "ssGUI/Backend/stb_image/BackendImageStbImage.hpp"

#include <fstream>
#include <cstring>

//SFML is using STB Image so we should not use the implementation define here
#ifndef SSGUI_MAIN_BACKEND_SFML
    #define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb_image.h"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendImageStbImage::BackendImageStbImage(BackendImageStbImage const& other)
    {
        if(other.ImageBuffer != nullptr)
        {
            ImageBuffer = static_cast<uint8_t*>(malloc(other.ImageSizeInBytes));
            ImageSizeInBytes = other.ImageSizeInBytes;
            memcpy(ImageBuffer, other.ImageBuffer, ImageSizeInBytes);
        }
        else
        {
            ImageBuffer = nullptr;
            ImageSizeInBytes = 0;
        }
        
        CurrentImageFormat = other.CurrentImageFormat;
        ImageWidth = other.ImageWidth;
        ImageHeight = other.ImageHeight;
        LinkedBackendDrawing = std::vector<ssGUI::Backend::BackendDrawingInterface*>();
    }

    BackendImageStbImage::BackendImageStbImage() :  ImageBuffer(nullptr),
                                                    ImageSizeInBytes(0),
                                                    CurrentImageFormat(),
                                                    ImageWidth(0),
                                                    ImageHeight(0)
    {
    }

    BackendImageStbImage::~BackendImageStbImage()
    {
        if(ImageBuffer != nullptr)
            free(ImageBuffer);
        
        //Remove all linked backend drawing
        std::vector<ssGUI::Backend::BackendDrawingInterface*> backends = LinkedBackendDrawing;
        for(int i = 0; i < backends.size(); i++)
            backends[i]->RemoveImageCache(this);
    }

    void* BackendImageStbImage::GetRawHandle()
    {
        ssGUI_WARNING(ssGUI_BACKEND_TAG, "There's no raw handle for Stb Image");
        return nullptr;
    }

    bool BackendImageStbImage::IsValid() const
    {
        return ImageBuffer != nullptr;
    }

    bool BackendImageStbImage::LoadFromPath(std::string path)
    {    
        std::ifstream inputStream(path, std::ios::in | std::ios::binary | std::ios::ate);
        
        if(!inputStream)
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to open file");
            return false;
        }
        size_t fileSize = inputStream.tellg();
        inputStream.seekg(0, std::ios::beg);

        //Create buffer
        uint8_t* buffer = static_cast<uint8_t*>(malloc(fileSize));

        //Read the file
        inputStream.read(reinterpret_cast<char*>(buffer), fileSize);
    
        if(!inputStream)
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to read the file");
            return false;
        }
        
        bool result = LoadImgFileFromMemory(buffer, fileSize);

        free(buffer);
        
        return result;
    }

    bool BackendImageStbImage::LoadImgFileFromMemory(const void * dataPtr, std::size_t size)
    {
        if(ImageBuffer != nullptr)
        {
            free(ImageBuffer);
            ImageBuffer =  nullptr;
            ImageSizeInBytes = 0;
        }
    
        int width, height, channels;
        bool validImg = false;
        void* data = nullptr;
        
        //If this is a 16 bit image
        if(stbi_is_16_bit_from_memory(static_cast<const stbi_uc*>(dataPtr), size))
        {
            data = stbi_load_16_from_memory(static_cast<const stbi_uc*>(dataPtr), size, &width, &height, &channels, 0);
            if(data != nullptr)
            {
                validImg = true;
                ImageSizeInBytes = width * height * channels * 2;
                CurrentImageFormat.BitDepthPerChannel = 16;
            }
        }
        //Otherwise 8 bit
        else
        {
            data = stbi_load_from_memory(static_cast<const stbi_uc*>(dataPtr), size, &width, &height, &channels, 0);
            if(data != nullptr)
            {
                validImg = true;
                ImageSizeInBytes = width * height * channels;
                CurrentImageFormat.BitDepthPerChannel = 8;
            }
        }
        
        if(validImg)
        {
            ImageBuffer = static_cast<uint8_t*>(malloc(ImageSizeInBytes));
            
            if(ImageBuffer == nullptr)
                return false;
            
            memcpy(ImageBuffer, data, ImageSizeInBytes);
            stbi_image_free(data);
            
            CurrentImageFormat.ImgType = channels <= 2 ? ssGUI::Enums::ImageType::MONO : ssGUI::Enums::ImageType::RGB;
            CurrentImageFormat.BitPerPixel = CurrentImageFormat.BitDepthPerChannel * channels;
            CurrentImageFormat.HasAlpha = channels == 2 || channels == 4;
            CurrentImageFormat.PreMultipliedAlpha = false;      //stb image provides function for converting it to non pre-multiplied 
                                                                //but we have no way to find out. Assuming non pre-multiplied.
            CurrentImageFormat.IndexR = -1;
            CurrentImageFormat.IndexG = -1;
            CurrentImageFormat.IndexB = -1;
            CurrentImageFormat.IndexA = -1;
            CurrentImageFormat.IndexMono = -1;
            CurrentImageFormat.RowPaddingInBytes = 0;
            
            switch(channels)
            {
                case 1:
                    CurrentImageFormat.IndexMono = 0;
                    break;
                case 2:
                    CurrentImageFormat.IndexMono = 0;
                    CurrentImageFormat.IndexA = 1;
                    break;
                case 3:
                    CurrentImageFormat.IndexR = 0;
                    CurrentImageFormat.IndexG = 1;
                    CurrentImageFormat.IndexB = 2;
                    break;
                case 4:
                    CurrentImageFormat.IndexR = 0;
                    CurrentImageFormat.IndexG = 1;
                    CurrentImageFormat.IndexB = 2;
                    CurrentImageFormat.IndexA = 3;
                    break;
                default:
                    ssGUI_WARNING(ssGUI_BACKEND_TAG, "What?");
                    return false;
                    //ssLOG_EXIT_PROGRAM();
            }
            
            ImageWidth = width;
            ImageHeight = height;
            
            return true;
        }
        
        return false;
    }

    bool BackendImageStbImage::LoadRawFromMemory(const void * dataPtr, ssGUI::ImageFormat format, glm::ivec2 imageSize)
    {
        if(ImageBuffer != nullptr)
        {
            free(ImageBuffer);
            ImageBuffer = nullptr;
            ImageSizeInBytes = 0;
        }
        
        ImageSizeInBytes = (format.BitPerPixel / 8 * imageSize.x + format.RowPaddingInBytes) * imageSize.y;
        ImageBuffer = static_cast<uint8_t*>(malloc(ImageSizeInBytes));
        memcpy(ImageBuffer, dataPtr, ImageSizeInBytes);
        
        if(ImageBuffer == nullptr)
            return false;
        
        CurrentImageFormat = format;
        ImageWidth = imageSize.x;
        ImageHeight = imageSize.y;
    
        return true;
    }
    
    glm::ivec2 BackendImageStbImage::GetSize() const
    {
        return glm::ivec2(ImageWidth, ImageHeight);
    }

    void* BackendImageStbImage::GetPixelPtr(ssGUI::ImageFormat& format) const
    {
        format = CurrentImageFormat;
        return ImageBuffer;
    }
    
    void BackendImageStbImage::UpdateCache()
    {
        std::vector<ssGUI::Backend::BackendDrawingInterface*> backends = LinkedBackendDrawing;
        for(int i = 0; i < backends.size(); i++)
        {
            backends[i]->RemoveImageCache(this);
            backends[i]->AddImageCache(this);
        }
    }

    void BackendImageStbImage::Internal_AddBackendDrawingRecord(ssGUI::Backend::BackendDrawingInterface* backendDrawing)
    {
        for(int i = 0; i < LinkedBackendDrawing.size(); i++)
        {
            if(LinkedBackendDrawing[i] == backendDrawing)
                return;
        }
        
        LinkedBackendDrawing.push_back(backendDrawing);
    }

    void BackendImageStbImage::Internal_RemoveBackendDrawingRecord(ssGUI::Backend::BackendDrawingInterface* backendDrawing)
    {
        for(int i = 0; i < LinkedBackendDrawing.size(); i++)
        {
            if(LinkedBackendDrawing[i] == backendDrawing)
            {
                LinkedBackendDrawing.erase(LinkedBackendDrawing.begin() + i);
                return;
            }
        }
    }

    BackendImageInterface* BackendImageStbImage::Clone()
    {
        return new BackendImageStbImage(*this);
    }
}

}