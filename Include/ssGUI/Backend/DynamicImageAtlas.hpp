#ifndef SSGUI_DYNAMIC_IMAGE_ATLAS_HPP
#define SSGUI_DYNAMIC_IMAGE_ATLAS_HPP

#include "ssGUI/HelperClasses/OutputStreamUtil.hpp"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <functional>
#include <map>
#include <unordered_map>

namespace ssGUI
{

namespace Backend
{
    /*class: ssGUI::Backend::DynamicImageAtlas
    This is an internal data structure for managing image atlas for OpenGL backends. 
    By default there's 0 layer, each layer has many cells for storing the textures.
    
    You normally don't have to deal with this class.

    Variables & Constructor:
    ============================== C++ ==============================
    public:
        struct ImageAtlasImageInfo
        {
            glm::ivec3 LocationInPixel;
            glm::ivec2 ImageSizeInPixel;
            bool HasMipmap;

            inline friend std::ostream& operator<< (std::ostream& stream, 
                                                    const ImageAtlasImageInfo& other)
            {
                stream  << SSGUI_OUTPUT_CLASS_NAME(ImageAtlasImageInfo)
                        << SSGUI_OUTPUT_VAR(LocationInPixel)
                        << SSGUI_OUTPUT_VAR(ImageSizeInPixel)
                        << SSGUI_OUTPUT_LAST_VAR(HasMipmap);
                        
                return stream;
            }
        };

    private:
        typedef int ImageId;
        typedef int CellsCount;
        
        struct FreeCellsInfo
        {
            glm::ivec3 CellsPositionIndex;
            glm::ivec2 CellsCountIn2D;
        };
        
        //(Internal variable) The size in pixels of each cell in the image atlas
        const glm::ivec2 CellSizeInPixel;
        
        //See <GetAtlasSize>
        const glm::ivec2 AtlasSizeInPixel;

        //(Internal variable) 
        //If the image atlas is created with size not in the multiples of cell size,
        //there will be space not used. 
        //This variable denotes the total space subtracts the space that is not used. 
        glm::ivec2 UsableSizeInPixel;
        
        //(Internal variable) Number of cells in each layer. 
        //Same as UsableSizeInPixel / CellSizeInPixel.
        glm::ivec2 CellsCountInGrid;
        
        //(Internal variable) Used to generate unique image Ids
        ImageId NextImageId;
        
        //See <GetImageInfo>
        std::unordered_map<ImageId, ImageAtlasImageInfo> ImageInfos;
        
        //(Internal variable) Keeps track of all the free cells
        std::multimap<CellsCount, FreeCellsInfo> FreeCells;
        
        //See <AddOnRequestNewAtlasCallback>
        std::function<bool(void)> OnRequestNewAtlasCallback;
        
        //(Internal variable) Keeps track of how many layers of image atlas
        int MaxAtlasIndex;
    =================================================================
    ============================== C++ ==============================
    DynamicImageAtlas::DynamicImageAtlas(   glm::ivec2 atlasSize, 
                                            glm::ivec2 cellSize,
                                            std::function<bool(void)> newAtlasRequestCallback) :    
                                                CellSizeInPixel(cellSize),
                                                AtlasSizeInPixel(atlasSize),
                                                UsableSizeInPixel(),
                                                CellsCountInGrid(),
                                                NextImageId(1),
                                                ImageInfos(),
                                                FreeCells(),
                                                OnRequestNewAtlasCallback(newAtlasRequestCallback),
                                                MaxAtlasIndex(-1)
    {
        //First we calculate the actual atlas texture size we can use
        CellsCountInGrid = AtlasSizeInPixel / CellSizeInPixel;
        UsableSizeInPixel = CellsCountInGrid * CellSizeInPixel;

        //FreeCellsInfo cellsInfo;
        //cellsInfo.CellsPositionIndex = glm::ivec3(0, 0, 0);
        //cellsInfo.CellsCountIn2D = CellsCountInGrid;
        //FreeCells.insert(std::make_pair(CellsCountInGrid.x * CellsCountInGrid.y, cellsInfo));
    }
    =================================================================
    */
    class DynamicImageAtlas
    {
        public:
            struct ImageAtlasImageInfo
            {
                glm::ivec3 LocationInPixel;
                glm::ivec2 ImageSizeInPixel;
                bool HasMipmap;

                inline friend std::ostream& operator<< (std::ostream& stream, 
                                                        const ImageAtlasImageInfo& other)
                {
                    stream  << SSGUI_OUTPUT_CLASS_NAME(ImageAtlasImageInfo)
                            << SSGUI_OUTPUT_VAR(LocationInPixel)
                            << SSGUI_OUTPUT_VAR(ImageSizeInPixel)
                            << SSGUI_OUTPUT_LAST_VAR(HasMipmap);
                            
                    return stream;
                }
            };
    
        private:
            typedef int ImageId;
            typedef int CellsCount;
            
            struct FreeCellsInfo
            {
                glm::ivec3 CellsPositionIndex;
                glm::ivec2 CellsCountIn2D;
            };
            
            //(Internal variable) The size in pixels of each cell in the image atlas
            const glm::ivec2 CellSizeInPixel;
            
            //See <GetAtlasSize>
            const glm::ivec2 AtlasSizeInPixel;

            //(Internal variable) 
            //If the image atlas is created with size not in the multiples of cell size,
            //there will be space not used. 
            //This variable denotes the total space subtracts the space that is not used. 
            glm::ivec2 UsableSizeInPixel;
            
            //(Internal variable) Number of cells in each layer. 
            //Same as UsableSizeInPixel / CellSizeInPixel.
            glm::ivec2 CellsCountInGrid;
            
            //(Internal variable) Used to generate unique image Ids
            ImageId NextImageId;
            
            //See <GetImageInfo>
            std::unordered_map<ImageId, ImageAtlasImageInfo> ImageInfos;
            
            //(Internal variable) Keeps track of all the free cells
            std::multimap<CellsCount, FreeCellsInfo> FreeCells;
            
            //See <AddOnRequestNewAtlasCallback>
            std::function<bool(void)> OnRequestNewAtlasCallback;
            
            //(Internal variable) Keeps track of how many layers of image atlas
            int MaxAtlasIndex;
            
            
            
            glm::ivec2 GetAllocatedImageSize(ImageAtlasImageInfo info);
            
            void OccupyCells(   std::multimap<CellsCount, FreeCellsInfo>::iterator it, 
                                glm::ivec2 occupyCellsCountIn2D);
            
            bool AreCellsOverlapped(FreeCellsInfo info);
            
            void AddCells(CellsCount count, FreeCellsInfo info);
            
            std::multimap<CellsCount, FreeCellsInfo>::iterator FindCells(   glm::ivec2 cellsNeeded, 
                                                                            int recursiveCount = 0);
            
            bool RequestNewAtlas();
            
            //TODO: Auto refragmenting cells
            
        public:
            DynamicImageAtlas(  glm::ivec2 atlasSize, 
                                glm::ivec2 cellSize, 
                                std::function<bool(void)> newAtlasRequestCallback);
            
            /*
            function: RequestImage
            Request an image that WILL BE stored in this image atlas by 
            supplying ImageAtlasImageInfo.
            
            If this function returns true, this means the operation is successful 
            and can store the image by querying the ImageAtlasImageInfo with 
            returnId where it will tell you where to store.
            */
            bool RequestImage(ImageAtlasImageInfo imgInfo, int& returnId);
            
            //function: RemoveImage
            //Removes an existing image that is occupying this image atlas by supplying 
            //the image Id returned by <RequestImage>
            void RemoveImage(int id);
            
            //function: GetImageInfo
            //Returns the infomation on where the image can be stored on the image atlas.
            bool GetImageInfo(int id, ImageAtlasImageInfo& returnInfo);
    
            /*
            function: AddOnRequestNewAtlasCallback
            Registers a callback function which allocates a new layer of image atlas 
            which returns boolean.
            
            If the callback function returns true, it means the new allocation was successful, 
            otherwise failed.
            */
            void AddOnRequestNewAtlasCallback(std::function<bool(void)> callback);
            
            //function: GetAtlasSize
            //The size in pixels for each layer of image atlas
            glm::ivec2 GetAtlasSize();
    
            //function: GetAtlasLayersCount
            //Get how many layers currently
            int GetAtlasLayersCount();
    };
}

}

#endif