#ifndef SSGUI_BACKEND_DRAWING_INTERFACE_HPP
#define SSGUI_BACKEND_DRAWING_INTERFACE_HPP

#include "ssGUI/Backend/Interfaces/MainWindowInterface.hpp"
#include "ssGUI/Backend/Interfaces/ImageInterface.hpp"
#include "ssGUI/Backend/Interfaces/FontInterface.hpp"
#include "ssGUI/DataClasses/DrawingEntity.hpp"

#include <vector>

namespace ssGUI
{ 

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::DrawingInterface
    class DrawingInterface
    {        
        public:
            using CharSize = uint16_t;
            using CharCode = uint32_t;
            using CharTextureIdentifier = std::tuple<   FontInterface*, 
                                                        CharSize, 
                                                        CharCode>;
        
            DrawingInterface(){}
            virtual ~DrawingInterface() = 0;

            //function: Initialize
            virtual bool Initialize(MainWindowInterface* mainWindowInterface) = 0;

            //function: SaveDrawingState
            //Saves the drawing context state.
            //This should be called before the state is modified for custom rendering
            virtual void SaveDrawingState() = 0;

            //function: RestoreDrawingState
            //Restores the drawing context state.
            //This should be called after custom rendering and handing rendering back to ssGUI.
            virtual void RestoreDrawingState() = 0;

            /*
            function: QueueDrawingEntities
            Draws the entities based on which backend is populated in <ssGUI::DrawingEntity>. 
            Returns true if created successfully. 

            *Note that if you are not using <ssGUIManager>, 
            you need to call <Render> at the end in order to render it*.
            */
            virtual bool QueueDrawingEntities(const std::vector<DrawingEntity>& entities) = 0;
            
            /*
            function: UploadDrawingEntitiesGroup
            Uploads the drawing entities group that can be ququed to be drawn 
              with the returned group id. 
            
            Failed to upload if returned id is negative.
            */
            virtual int UploadDrawingEntitiesGroup(const std::vector<DrawingEntity>& group) = 0;
            
            //function: DiscardDrawingEntitiesGroup
            //Discards the drawing entities group that is uploaded with the specified group id.
            virtual void DiscardDrawingEntitiesGroup(int groupId) = 0;
            
            //function: QueueDrawingEntitiesGroups
            //Queues the uploaded drawing entities groups to be drawn, 
            //similar to <QueueDrawingEntities>.
            virtual bool QueueDrawingEntitiesGroups(const std::vector<int> groupsIds) = 0;
            
            //function: DrawToBackBuffer
            //This draws all the ququed drawing entities to the back buffer.
            //This is also automatically called by <Render>.
            virtual void DrawToBackBuffer() = 0;

            //function: Render
            //Renders every entity that are drawn to the back buffer to the <MainWindow>. 
            //This will automatically swap and clear the back buffer. 
            //If you are using <ssGUIManager>, this will be automatically called.
            virtual void Render(glm::u8vec3 clearColor) = 0;

            //function: ClearBackBuffer
            //Clears the back buffer manually. If you are using <ssGUIManager>, 
            //This is called automatically by render.
            virtual void ClearBackBuffer(glm::u8vec3 clearColor) = 0;
            
            /*
            function: AddImageCache
            Add the backend image to cache (this can be uploading the image to GPU memory) 
              or system memory for drawing
            
            Call with images that are cached will update the cache.
            */
            virtual void AddImageCache(ImageInterface* backendImage) = 0;
            
            //function: RemoveImageCache
            //This removes the backend image from the cache.
            virtual void RemoveImageCache(ImageInterface* backendImage) = 0;
            
            /*
            function: GetRawImageCacheHandle
            This returns the handle of the image cache. What is returned is backend independent.
            You can use the returned handle to modify the cached image.
            If no cache is found, it will return nullptr.
            */
            virtual void* GetRawImageCacheHandle(ImageInterface* backendImage) const = 0;
    };

    //Pure virtual destructor needs to be defined
    inline DrawingInterface::~DrawingInterface(){}
} 

}

#endif