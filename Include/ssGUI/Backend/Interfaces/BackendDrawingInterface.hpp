#ifndef SSGUI_BACKEND_DRAWING_INTERFACE_H
#define SSGUI_BACKEND_DRAWING_INTERFACE_H

#include "ssGUI/Backend/Interfaces/BackendImageInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendFontInterface.hpp"
#include "ssGUI/DataClasses/DrawingEntity.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <string>
#include <vector>

#include <iostream>

namespace ssGUI
{ 

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendDrawingInterface
    class BackendDrawingInterface
    {        
        public:
            using CharSize = uint16_t;
            using CharCode = uint32_t;
            using CharTextureIdentifier = std::tuple<ssGUI::Backend::BackendFontInterface*, CharSize, CharCode>;
        
            BackendDrawingInterface(){}
            virtual ~BackendDrawingInterface() = 0;

            //function: SaveState
            //Saves the OpenGL state
            virtual void SaveState() = 0;

            //function: RestoreState
            //Restores the OpenGL state
            virtual void RestoreState() = 0;

            //function: DrawEntities
            //Draws the entity based on what is set in the _properties_. Returns true if drawn successfully. 
            //Depending on the backend this, the entities might be drawn to internal buffer or
            //  drawn to back buffer directly. For OpenGL backends, it will be drawn to internal buffers.
            //<DrawToBackBuffer> can be called to flush the internal buffer to be drawn on the back buffer.
            //*Note that if you are not using <ssGUIManager>, you need to call <Render> at the end in order to render it*.
            virtual bool DrawEntities(const std::vector<ssGUI::DrawingEntity>& entities) = 0;
            
            //function: DrawToBackBuffer
            //This flushes the internal buffer to the back buffer.
            //Depending on the backend, this might have no effect as <DrawEntities> can draw to back buffer directly.
            //This is also automatically called by <Render>.
            virtual void DrawToBackBuffer() = 0;

            //function: Render
            //Renders every entity that are drawn to the <MainWindow>. This will automatically clear the back buffer. 
            //If you are using <ssGUIManager>, this will be automatically called.
            virtual void Render(glm::u8vec3 clearColor) = 0;

            //function: ClearBackBuffer
            //Clears the back buffer manually. If you are using <ssGUIManager>, this will be automatically called for caching.
            virtual void ClearBackBuffer(glm::u8vec3 clearColor) = 0;
            
            //function: AddImageCache
            //Add the backend image to cache (this can be uploading the image to GPU memory) or system memory for drawing
            //To update the cache, call <BackendImageInterface::UpdateCache>.
            //Calling this fuction multiple times will not update the cache.
            virtual void AddImageCache(ssGUI::Backend::BackendImageInterface* backendImage) = 0;
            
            //function: RemoveImageCache
            //This removes the backend image from the cache.
            virtual void RemoveImageCache(ssGUI::Backend::BackendImageInterface* backendImage) = 0;
            
            //function: GetRawImageCacheHandle
            //This returns the handle of the image cache. What is returned is backend independent.
            //You can use the returned handle to modify the cached image.
            //If no cache is found, it will return nullptr.
            virtual void* GetRawImageCacheHandle(ssGUI::Backend::BackendImageInterface* backendImage) = 0;
    };
    inline BackendDrawingInterface::~BackendDrawingInterface(){}   //Pure virtual destructor needs to be defined
} 

}

#endif