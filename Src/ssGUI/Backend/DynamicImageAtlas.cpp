#include "ssGUI/Backend/DynamicImageAtlas.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Backend
{

    glm::ivec2 DynamicImageAtlas::GetAllocatedImageSize(ImageAtlasImageInfo info)
    {
        return info.HasMipmap ? glm::ivec2(info.ImageSizeInPixel.x * 1.5f, info.ImageSizeInPixel.y) : info.ImageSizeInPixel;
    }
    
    void DynamicImageAtlas::OccupyCells(std::multimap<CellsCount, FreeCellsInfo>::iterator it, glm::ivec2 occupyCellsCountIn2D)
    {
        FreeCellsInfo freeCellInfo = it->second;
        
        if(occupyCellsCountIn2D == freeCellInfo.CellsCountIn2D)
            FreeCells.erase(it);
        else if(occupyCellsCountIn2D.x == freeCellInfo.CellsCountIn2D.x)
        {
            FreeCellsInfo newCellsInfo;
            newCellsInfo.CellsCountIn2D.x = occupyCellsCountIn2D.x;
            newCellsInfo.CellsCountIn2D.y = freeCellInfo.CellsCountIn2D.y - occupyCellsCountIn2D.y;
            newCellsInfo.CellsPositionIndex = freeCellInfo.CellsPositionIndex + glm::ivec3(0, occupyCellsCountIn2D.y, 0);
            FreeCells.insert(std::make_pair(newCellsInfo.CellsCountIn2D.x * newCellsInfo.CellsCountIn2D.y, newCellsInfo));
            FreeCells.erase(it);
        }
        else if(occupyCellsCountIn2D.y == freeCellInfo.CellsCountIn2D.y)
        {
            FreeCellsInfo newCellsInfo;
            newCellsInfo.CellsCountIn2D.x = freeCellInfo.CellsCountIn2D.x - occupyCellsCountIn2D.x;
            newCellsInfo.CellsCountIn2D.y = occupyCellsCountIn2D.y;
            newCellsInfo.CellsPositionIndex = freeCellInfo.CellsPositionIndex + glm::ivec3(occupyCellsCountIn2D.x, 0, 0);
            FreeCells.insert(std::make_pair(newCellsInfo.CellsCountIn2D.x * newCellsInfo.CellsCountIn2D.y, newCellsInfo));
            FreeCells.erase(it);
        }
        else
        {
            FreeCellsInfo newCellsInfoX;
            newCellsInfoX.CellsCountIn2D.x = freeCellInfo.CellsCountIn2D.x - occupyCellsCountIn2D.x;
            newCellsInfoX.CellsCountIn2D.y = occupyCellsCountIn2D.y;
            newCellsInfoX.CellsPositionIndex = freeCellInfo.CellsPositionIndex + glm::ivec3(occupyCellsCountIn2D.x, 0, 0);
            FreeCells.insert(std::make_pair(newCellsInfoX.CellsCountIn2D.x * newCellsInfoX.CellsCountIn2D.y, newCellsInfoX));

            FreeCellsInfo newCellsInfoY;
            newCellsInfoY.CellsCountIn2D.x = freeCellInfo.CellsCountIn2D.x;
            newCellsInfoY.CellsCountIn2D.y = freeCellInfo.CellsCountIn2D.y - occupyCellsCountIn2D.y;
            newCellsInfoY.CellsPositionIndex = freeCellInfo.CellsPositionIndex + glm::ivec3(0, occupyCellsCountIn2D.y, 0);
            FreeCells.insert(std::make_pair(newCellsInfoY.CellsCountIn2D.x * newCellsInfoY.CellsCountIn2D.y, newCellsInfoY));

            FreeCells.erase(it);
        }
    }
    
    bool DynamicImageAtlas::AreCellsOverlapped(FreeCellsInfo info)
    {
        for(auto it = FreeCells.begin(); it != FreeCells.end(); it++)
        {
            if(info.CellsPositionIndex.z != it->second.CellsPositionIndex.z)
                continue;
            
            bool xInside = false;
            bool yInside = false;
            
            if( info.CellsPositionIndex.x >= it->second.CellsPositionIndex.x &&
                info.CellsPositionIndex.x < it->second.CellsPositionIndex.x + it->second.CellsCountIn2D.x)
            {
                xInside = true;
            }
            
            if( info.CellsPositionIndex.y >= it->second.CellsPositionIndex.y &&
                info.CellsPositionIndex.y < it->second.CellsPositionIndex.y + it->second.CellsCountIn2D.y)
            {
                yInside = true;
            }

            if(xInside && yInside)
                return true;
        }
        
        return false;
    }
    
    void DynamicImageAtlas::AddCells(CellsCount count, FreeCellsInfo info)
    {
        assert(!AreCellsOverlapped(info));
        FreeCells.insert(std::make_pair(count, info));
    }
    
    std::multimap<DynamicImageAtlas::CellsCount, DynamicImageAtlas::FreeCellsInfo>::iterator DynamicImageAtlas::FindCells(glm::ivec2 cellsNeeded, int recursiveCount)
    {
        const int MAX_RECURSIVE_COUNT = 10;
        if(recursiveCount >= MAX_RECURSIVE_COUNT)
        {
            ssGUI_ERROR(ssGUI_BACKEND_TAG, "recursiveCount has reached MAX_RECURSIVE_COUNT!");
            return FreeCells.end();
        }
    
        //Get the max free cells count
        CellsCount maxFreeCellsCount = FreeCells.rbegin()->first;
        
        //ssLOG_LINE("maxFreeCellsCount: "<<maxFreeCellsCount);
        //ssLOG_LINE("FreeCells.rbegin()->second.CellsCountIn2D: "<<FreeCells.rbegin()->second.CellsCountIn2D.x<<", "<<FreeCells.rbegin()->second.CellsCountIn2D.y);
        //ssLOG_LINE("cellsNeeded: "<<cellsNeeded.x <<", "<<cellsNeeded.y);
        
        //Check if size needed exceed this max free cells count
        if(cellsNeeded.x * cellsNeeded.y > maxFreeCellsCount)
        {
            //If so, request another atlas
            if(!RequestNewAtlas())
                return FreeCells.end();
            
            maxFreeCellsCount = FreeCells.rbegin()->first;
        }
        
        std::multimap<CellsCount, FreeCellsInfo>::iterator it = FreeCells.end();
        
        //Starting from size needed (converted to cells count) until max free cells count
        for(int curCellCount = cellsNeeded.x * cellsNeeded.y; curCellCount <= maxFreeCellsCount; ++curCellCount)
        {
            //ssLOG_LINE("curCellCount: "<<curCellCount);
            it = FreeCells.find(curCellCount);
            
            if(it != FreeCells.end())
                break;
        }
        
        const int MAX_LOOP = FreeCells.size();
        
        //If we found cells that match with cells count needed
        if(it != FreeCells.end())
        {
            //We record found cells info
            auto foundIt = it;
            int foundSize = it->first;
            
            //We iterate backward first (and find suitable cells info)
            for(int i = 0; i < MAX_LOOP; i++)
            {
                //ssLOG_LINE("back i: "<<i);
            
                //Until the free cells count is not the same
                if(it->first != foundSize)
                    break;
            
                if(it->second.CellsCountIn2D.x >= cellsNeeded.x && it->second.CellsCountIn2D.y >= cellsNeeded.y)
                    return it;
                
                if(it == FreeCells.begin())
                    break;

                it--;
            }
            
            //Then we iterate forward from found cells info 
            it = foundIt;

            //Until we are able to find the suitable cells info
            for(int i = 0; i < MAX_LOOP; i++)
            {
                //ssLOG_LINE("forward i: "<<i);
                
                if(it == FreeCells.end())
                    break;

                if(it->second.CellsCountIn2D.x >= cellsNeeded.x && it->second.CellsCountIn2D.y >= cellsNeeded.y)
                    return it;
            
                it++;
            }
        }
     
        //If we are unable to find one, request another atlas
        if(RequestNewAtlas())
            return FindCells(cellsNeeded, ++recursiveCount);
        else
            return FreeCells.end();
    }
    
    bool DynamicImageAtlas::RequestNewAtlas()
    {
        if(OnRequestNewAtlasCallback())
        {
            FreeCellsInfo cellsInfo;
            cellsInfo.CellsPositionIndex = glm::ivec3(0, 0, ++MaxAtlasIndex);
            cellsInfo.CellsCountIn2D = CellsCountInGrid;
            FreeCells.insert(std::make_pair(CellsCountInGrid.x * CellsCountInGrid.y, cellsInfo));
            return true;
        }
        
        return false;
    }
    
    void DynamicImageAtlas::AddOnRequestNewAtlasCallback(std::function<bool(void)> callback)
    {
        OnRequestNewAtlasCallback = callback;
    }
    
    glm::ivec2 DynamicImageAtlas::GetAtlasSize()
    {
        return AtlasSizeInPixel;
    }
    
    
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
    
    void DynamicImageAtlas::RemoveImage(int id)
    {
        if(ImageInfos.find(id) == ImageInfos.end())
            return;
        
        //Remove from ImageInfos
        ImageAtlasImageInfo imgInfo = ImageInfos.at(id);
        ImageInfos.erase(id);
        
        //AddCells back
        FreeCellsInfo cellsInfo;
        cellsInfo.CellsCountIn2D = glm::vec2(GetAllocatedImageSize(imgInfo)) / glm::vec2(CellSizeInPixel) + 0.99f; 
        cellsInfo.CellsPositionIndex = imgInfo.LocationInPixel / glm::ivec3(CellSizeInPixel, 1);
        
        AddCells(cellsInfo.CellsCountIn2D.x * cellsInfo.CellsCountIn2D.y, cellsInfo);
    }
    
    bool DynamicImageAtlas::GetImageInfo(int id, ImageAtlasImageInfo& returnInfo)
    {
        if(ImageInfos.find(id) == ImageInfos.end())
            return false;
        
        returnInfo = ImageInfos.at(id);
        return true;
    }
}

}