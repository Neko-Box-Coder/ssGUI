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
            
            const glm::ivec2 CellSizeInPixel;
            const glm::ivec2 AtlasSizeInPixel;

            glm::ivec2 UsableSizeInPixel;
            glm::ivec2 CellsCountInGrid;
            
            ImageId NextImageId;
            std::unordered_map<ImageId, ImageAtlasImageInfo> ImageInfos;
            std::multimap<CellsCount, FreeCellsInfo> FreeCells;
            
            std::function<bool(void)> OnRequestNewAtlasCallback;
            int MaxAtlasIndex = 0;
            
            glm::ivec2 GetAllocatedImageSize(ImageAtlasImageInfo info);
            
            void OccupyCells(std::multimap<CellsCount, FreeCellsInfo>::iterator it, glm::ivec2 occupyCellsCountIn2D);
            
            bool AreCellsOverlapped(FreeCellsInfo info);
            
            void AddCells(CellsCount count, FreeCellsInfo info);
            
            std::multimap<CellsCount, FreeCellsInfo>::iterator FindCells(glm::ivec2 cellsNeeded, int recursiveCount = 0);
            
            bool RequestNewAtlas();
            
            //TODO: Auto refragmenting cells
            
        public:
            DynamicImageAtlas(glm::ivec2 atlasSize, glm::ivec2 cellSize, std::function<bool(void)> newAtlasRequestCallback);
            
            //Request Image
            bool RequestImage(ImageAtlasImageInfo imgInfo, int& returnId);
            
            //Remove Image
            void RemoveImage(int id);
            
            //Get Image Info
            bool GetImageInfo(int id, ImageAtlasImageInfo& returnInfo);
    
            void AddOnRequestNewAtlasCallback(std::function<bool(void)> callback);
            
            glm::ivec2 GetAtlasSize();
    };
}

}

#endif