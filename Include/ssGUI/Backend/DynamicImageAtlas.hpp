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
    By default there's only 1 layer of image atlas, each layer has many cells for storing the textures.
    
    You normally don't have to deal with this class.

    Variables & Constructor:
    ============================== C++ ==============================
    public:
        struct ImageAtlasImageInfo
        {
            glm::ivec3 LocationInPixel;
            glm::ivec2 ImageSizeInPixel;
            bool HasMipmap;

            inline friend std::ostream& operator<< (std::ostream& stream, const ImageAtlasImageInfo& other)
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
        
        const glm::ivec2 CellSizeInPixel;                                   //(Internal variable) The size in pixels of each cell in the image atlas
        const glm::ivec2 AtlasSizeInPixel;                                  //See <GetAtlasSize>

        glm::ivec2 UsableSizeInPixel;                                       //(Internal variable) If the image atlas is created with size not in the multiples of cell size,
                                                                            //                          there will be space not used. 
                                                                            //                          This variable denotes the total space subtracts the space that is not used. 
        glm::ivec2 CellsCountInGrid;                                        //(Internal variable) Number of cells in each layer. Same as UsableSizeInPixel / CellSizeInPixel.
        
        ImageId NextImageId;                                                //(Internal variable) Used to generate unique image Ids
        std::unordered_map<ImageId, ImageAtlasImageInfo> ImageInfos;        //See <GetImageInfo>
        std::multimap<CellsCount, FreeCellsInfo> FreeCells;                 //(Internal variable) Keeps track of all the free cells
        
        std::function<bool(void)> OnRequestNewAtlasCallback;                //See <AddOnRequestNewAtlasCallback>
        int MaxAtlasIndex;                                                  //(Internal variable) Keeps track of how many layers of image atlas
    =================================================================
    ============================== C++ ==============================
    DynamicImageAtlas::DynamicImageAtlas(   glm::ivec2 atlasSize, 
                                            glm::ivec2 cellSize,
                                            std::function<bool(void)> newAtlasRequestCallback) :    CellSizeInPixel(cellSize),
                                                                                                    AtlasSizeInPixel(atlasSize),
                                                                                                    UsableSizeInPixel(),
                                                                                                    CellsCountInGrid(),
                                                                                                    NextImageId(1),
                                                                                                    ImageInfos(),
                                                                                                    FreeCells(),
                                                                                                    OnRequestNewAtlasCallback(newAtlasRequestCallback),
                                                                                                    MaxAtlasIndex(0)
    {
        //First we calculate the actual atlas texture size we can use
        CellsCountInGrid = AtlasSizeInPixel / CellSizeInPixel;
        UsableSizeInPixel = CellsCountInGrid * CellSizeInPixel;

        FreeCellsInfo cellsInfo;
        cellsInfo.CellsPositionIndex = glm::ivec3(0, 0, 0);
        cellsInfo.CellsCountIn2D = CellsCountInGrid;
        FreeCells.insert(std::make_pair(CellsCountInGrid.x * CellsCountInGrid.y, cellsInfo));
    }
    
    bool DynamicImageAtlas::RequestImage(ImageAtlasImageInfo imgInfo, int& returnId)
    {
        glm::ivec2 sizeNeeded = GetAllocatedImageSize(imgInfo);
     
        //Check if size needed exceeds atlas total size
        if(sizeNeeded.x > UsableSizeInPixel.x || sizeNeeded.y > UsableSizeInPixel.y)
            return false;    
        
        glm::ivec2 cellsNeeded = glm::ivec2( glm::vec2(sizeNeeded) / glm::vec2(CellSizeInPixel) + 0.99f );    
     
        //Check if theres any free cells left
        if(FreeCells.empty())
        {
            //Try to create another atlas
            if(!RequestNewAtlas())
                //If unable, return false
                return false;
        }
        
        auto foundIt = FindCells(cellsNeeded);
        
        if(foundIt == FreeCells.end())
            return false;
        
        FreeCellsInfo occupyCellsInfo = foundIt->second;
        
        //After this point, foundIt is no longer valid
        OccupyCells(foundIt, cellsNeeded);
        
        imgInfo.LocationInPixel = occupyCellsInfo.CellsPositionIndex * glm::ivec3(CellSizeInPixel, 1); 
        ImageInfos[NextImageId] = imgInfo;
        
        returnId = NextImageId++;
        return true;
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

                inline friend std::ostream& operator<< (std::ostream& stream, const ImageAtlasImageInfo& other)
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
            
            const glm::ivec2 CellSizeInPixel;                                   //(Internal variable) The size in pixels of each cell in the image atlas
            const glm::ivec2 AtlasSizeInPixel;                                  //See <GetAtlasSize>

            glm::ivec2 UsableSizeInPixel;                                       //(Internal variable) If the image atlas is created with size not in the multiples of cell size,
                                                                                //                          there will be space not used. 
                                                                                //                          This variable denotes the total space subtracts the space that is not used. 
            glm::ivec2 CellsCountInGrid;                                        //(Internal variable) Number of cells in each layer. Same as UsableSizeInPixel / CellSizeInPixel.
            
            ImageId NextImageId;                                                //(Internal variable) Used to generate unique image Ids
            std::unordered_map<ImageId, ImageAtlasImageInfo> ImageInfos;        //See <GetImageInfo>
            std::multimap<CellsCount, FreeCellsInfo> FreeCells;                 //(Internal variable) Keeps track of all the free cells
            
            std::function<bool(void)> OnRequestNewAtlasCallback;                //See <AddOnRequestNewAtlasCallback>
            int MaxAtlasIndex;                                                  //(Internal variable) Keeps track of how many layers of image atlas
            
            glm::ivec2 GetAllocatedImageSize(ImageAtlasImageInfo info);
            
            void OccupyCells(std::multimap<CellsCount, FreeCellsInfo>::iterator it, glm::ivec2 occupyCellsCountIn2D);
            
            bool AreCellsOverlapped(FreeCellsInfo info);
            
            void AddCells(CellsCount count, FreeCellsInfo info);
            
            std::multimap<CellsCount, FreeCellsInfo>::iterator FindCells(glm::ivec2 cellsNeeded, int recursiveCount = 0);
            
            bool RequestNewAtlas();
            
            //TODO: Auto refragmenting cells
            
        public:
            DynamicImageAtlas(glm::ivec2 atlasSize, glm::ivec2 cellSize, std::function<bool(void)> newAtlasRequestCallback);
            
            //function: RequestImage
            //Request an image that WILL BE stored in this image atlas by supplying ImageAtlasImageInfo.
            //If this function returns true, this means the operation is successful 
            //and can store the image by querying the ImageAtlasImageInfo with returnId where it will tell you where to store.
            bool RequestImage(ImageAtlasImageInfo imgInfo, int& returnId);
            
            //function: RemoveImage
            //Removes an existing image that is occupying this image atlas by supplying the image Id returned by <RequestImage>
            void RemoveImage(int id);
            
            //function: GetImageInfo
            //Returns the infomation on where the image can be stored on the image atlas.
            bool GetImageInfo(int id, ImageAtlasImageInfo& returnInfo);
    
            //function: AddOnRequestNewAtlasCallback
            //Registers a callback function which allocates a new layer of image atlas which returns boolean.
            //If the callback function returns true, it means the new allocation was successful, otherwise failed.
            void AddOnRequestNewAtlasCallback(std::function<bool(void)> callback);
            
            //function: GetAtlasSize
            //The size in pixels for each layer of image atlas
            glm::ivec2 GetAtlasSize();
    };
}

}

#endif