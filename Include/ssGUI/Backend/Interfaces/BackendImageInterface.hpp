#ifndef SSGUI_IMAGE_INTERFACE_HPP
#define SSGUI_IMAGE_INTERFACE_HPP

#include "ssGUI/DataClasses/ImageFormat.hpp"

#include <string>

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
            virtual void* GetRawHandle() const = 0;

            //function: IsValid
            //Returns true if image is loaded
            virtual bool IsValid() const = 0;

            //function: LoadFromPath
            //Loads the image from relative or absolute path
            virtual bool LoadFromPath(std::string path) = 0;

            //function: LoadImgFileFromMemory
            //Loads an image file from memory with specified size in bytes
            virtual bool LoadImgFileFromMemory( const void* dataPtr, 
                                                std::size_t size) = 0;

            //function: LoadRawFromMemory
            //This loads an image with specified image format in memory 
            virtual bool LoadRawFromMemory( const void* dataPtr, 
                                            ImageFormat format, 
                                            glm::ivec2 imageSize) = 0;

            //function: GetSize
            //Returns the size of the image
            virtual glm::ivec2 GetSize() const = 0;

            //function: GetPixelPtr
            //Returns the pixel data pointer of the image and also the format of the image
            virtual void* GetPixelPtr(ImageFormat& format) const = 0;
            
            //function: UpdateCache
            //Updates the drawing interfaces' cache with the current image
            virtual void UpdateCache() = 0;
            
            using DrawingInterface = ssGUI::Backend::BackendDrawingInterface;
            
            //function: Internal_AddBackendDrawingRecord
            //(Internal ssGUI function) Adds a linking record of indicating 
            //  this image is stored in backend drawing.
            //By default no linking record will be cloned when <Clone> is called.
            virtual void Internal_AddBackendDrawingRecord(DrawingInterface* backendDrawing) = 0;
            
            //function: Internal_RemoveBackendDrawingRecord
            //(Internal ssGUI function) Removes a linking record of indicating 
            //  this image is stored in backend drawing.
            //By default no linking record will be cloned when <Clone> is called.
            virtual void Internal_RemoveBackendDrawingRecord(DrawingInterface* backendDrawing) = 0;

            //function: Clone
            //Clones the backend image
            virtual BackendImageInterface* Clone() = 0;
    };

    //Pure virtual destructor needs to be defined
    inline BackendImageInterface::~BackendImageInterface(){}
}

}

#endif