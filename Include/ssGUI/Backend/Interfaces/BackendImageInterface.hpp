#ifndef SSGUI_IMAGE_INTERFACE_H
#define SSGUI_IMAGE_INTERFACE_H

#include <string>
#include "glm/vec2.hpp"
#include "ssGUI/DataClasses/ImageFormat.hpp"

namespace ssGUI
{ 
    
//namespace: ssGUI::Backend
namespace Backend
{
    class BackendDrawingInterface;

    //class: ssGUI::Backend::BackendImageInterface
    //This allows transferring the image data from the host memory to the gpu memory
    class BackendImageInterface
    {
        public:
            BackendImageInterface(){}
            virtual ~BackendImageInterface() = 0;

            //function: GetRawHandle
            //Returns the actual backend handle if the image data is valid. Otherwise returns nullptr.
            virtual void* GetRawHandle() = 0;

            //function: IsValid
            //Returns true if image is loaded
            virtual bool IsValid() const = 0;

            //function: LoadFromPath
            //Loads the image from relative or absolute path
            virtual bool LoadFromPath(std::string path) = 0;

            //function: LoadImgFileFromMemory
            //Loads an image file from memory with specified size in bytes
            virtual bool LoadImgFileFromMemory(void const * dataPtr, std::size_t size) = 0;

            //function: LoadRawFromMemory
            //This loads an image with specified image format in memory 
            virtual bool LoadRawFromMemory(void const * dataPtr, ssGUI::ImageFormat format, glm::ivec2 imageSize) = 0;

            //function: GetSize
            //Returns the size of the image
            virtual glm::ivec2 GetSize() const = 0;

            //function: GetPixelPtr
            //Returns the pixel data pointer of the image and also the format of the image
            virtual void* GetPixelPtr(ssGUI::ImageFormat& format) const = 0;
            
            //function: UpdateCache
            virtual void UpdateCache() = 0;
            
            //function: Internal_AddBackendDrawingRecord
            //(Internal ssGUI function) Adds a linking record of indicating this image is stored in backend drawing.
            virtual void Internal_AddBackendDrawingRecord(ssGUI::Backend::BackendDrawingInterface* backendDrawing) = 0;
            
            //function: Internal_RemoveBackendDrawingRecord
            //(Internal ssGUI function) Removes a linking record of indicating this image is stored in backend drawing.
            virtual void Internal_RemoveBackendDrawingRecord(ssGUI::Backend::BackendDrawingInterface* backendDrawing) = 0;

            //function: Clone
            //Clones the backend image
            virtual BackendImageInterface* Clone() = 0;
    };

    inline BackendImageInterface::~BackendImageInterface(){}   //Pure virtual destructor needs to be defined
}

}

#endif