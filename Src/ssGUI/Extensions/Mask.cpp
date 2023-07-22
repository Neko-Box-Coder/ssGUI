#include "ssGUI/Extensions/Mask.hpp"

#include "ssGUI/Extensions/MaskEnforcer.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

#include <cmath>
#include <algorithm>    // std::sort

namespace ssGUI
{

namespace Extensions
{
    bool Mask::IsPointContainedInShape(glm::vec2 point, const std::vector<glm::vec2>& shapeVertices) const
    {
        for(int i = 0; i < shapeVertices.size(); i++)
        {
            int nextIndex = (i + 1) % shapeVertices.size();
            glm::vec3 curLine = glm::vec3(shapeVertices[i] - point, 0);
            glm::vec3 nextLine = glm::vec3(shapeVertices[nextIndex] - point, 0);

            float cross = glm::cross(curLine, nextLine).z;
            if(cross < 0)
                return false;
        }

        return true;
    }
    
    bool Mask::IsPointContainedInMask(glm::vec2 point, glm::vec2 min, glm::vec2 max) const
    {
        bool isXIn = point.x >= min.x && point.x <= max.x;

        if(!isXIn)
            return false;
        
        bool isYIn = point.y >= min.y && point.y <= max.y;

        if(isYIn)
            return true;
        
        return false;
    }

    bool Mask::GetAxesValues(glm::vec2 axis, glm::vec2 axis2, glm::vec2 samplePoint, float& axisValue, float& axis2Value)
    {
        //Credit: https://stackoverflow.com/questions/13937782/calculating-the-point-of-intersection-of-two-lines
        //Return the multiplier from a to reach intersection of ab and cd
        auto findIntersectMultiplier = [](glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d) -> float
        {
            float denom = (d.y - c.y) * (b.x - a.x) - (d.x - c.x) * (b.y - a.y);
            if(denom == 0)  
                return 0; 

            return ((d.x - c.x) * (a.y - c.y) - (d.y - c.y) * (a.x - c.x)) / denom;
        };

        // (The two axes can be at any angle, but CD is always perpendcular to the axis we are projecting to)

        //      │
        //      │
        //axis 2│   x samplePoint (C)
        //      │   │
        //      └───┼───────────
        //      A   │ axis     B             
        //          │
        //          D
        
        axisValue = findIntersectMultiplier(glm::vec2(0, 0), axis, samplePoint, samplePoint - axis2);

        //      B
        // axis2│
        //      │
        //D─────┼───x samplePoint (C)
        //      │   
        //      └────────────
        //      A    axis                  
        //          

        axis2Value = findIntersectMultiplier(glm::vec2(0, 0), axis2, samplePoint, samplePoint - axis);

        return true;
    }

    bool Mask::IsAABBOverlap(const std::vector<glm::vec2>& shapeVerticies, glm::vec2 maskMin, glm::vec2 maskMax,
                            glm::vec2& shapeMin, glm::vec2& shapeMax)
    {
        ssLOG_FUNC_ENTRY();
        
        shapeMin = shapeVerticies[0];
        shapeMax = shapeVerticies[0];
        for(int j = 1; j < shapeVerticies.size(); j++)
        {
            if(shapeVerticies[j].x < shapeMin.x)
                shapeMin.x = shapeVerticies[j].x;
            else if(shapeVerticies[j].x > shapeMax.x)
                shapeMax.x = shapeVerticies[j].x;
            if(shapeVerticies[j].y < shapeMin.y)
                shapeMin.y = shapeVerticies[j].y;
            else if(shapeVerticies[j].y > shapeMax.y)
                shapeMax.y = shapeVerticies[j].y;
        }
        
        if(!CheckLinesOverlapping(shapeMin.x, shapeMax.x, maskMin.x, maskMax.x))
        {
            ssLOG_FUNC_EXIT();
            return false;
        }
        
        if(!CheckLinesOverlapping(shapeMin.y, shapeMax.y, maskMin.y, maskMax.y))
        {
            ssLOG_FUNC_EXIT();
            return false;
        }

        ssLOG_FUNC_EXIT();
        return true;
    }

    void Mask::GetIntersections(std::vector<glm::vec2>& intersections, 
                                std::vector<int>& shapeIntersectIndices, 
                                std::vector<int>& maskIntersectIndices,
                                const std::vector<glm::vec2>& shapeVerticies, 
                                std::vector<glm::vec2>& maskVerticies)
    {
        ssLOG_FUNC_ENTRY();
        //For each vertices for this shape
        for(int currentShapeVertexIndex = 0; currentShapeVertexIndex < shapeVerticies.size(); currentShapeVertexIndex++)
        {
            int nextShapeVertexIndex = GetNextIndex(0, shapeVerticies.size(),currentShapeVertexIndex);
            //Iterate every line on the mask and see which intersects
            for(int currentMaskIndex = 0; currentMaskIndex < maskVerticies.size(); currentMaskIndex++)
            {
                glm::vec2 intersection;
                int nextMaskIndex = GetNextIndex(0, maskVerticies.size(), currentMaskIndex);
                if(LineToLineIntersection(  shapeVerticies[currentShapeVertexIndex], 
                                            shapeVerticies[nextShapeVertexIndex], 
                                            maskVerticies[currentMaskIndex], 
                                            maskVerticies[nextMaskIndex], 
                                            intersection))
                {
                    intersections.push_back(intersection);
                    shapeIntersectIndices.push_back(currentShapeVertexIndex);
                    maskIntersectIndices.push_back(currentMaskIndex);
                }
            }   
        }
        ssLOG_FUNC_EXIT();
    }

    void Mask::FormNewShapeWithIntersections(   ssGUI::DrawingEntity& currentEntity,
                                                std::vector<bool>& currentVertexChanged,
                                                const ssGUI::DrawingEntity& originalEntity,
                                                glm::vec2 maskMin, 
                                                glm::vec2 maskMax, 
                                                std::vector<glm::vec2>& maskVerticies, 
                                                std::vector<glm::vec2>& intersections,
                                                std::vector<int>& shapeIntersectIndices, 
                                                std::vector<int>& maskIntersectIndices)
    {
        ssLOG_FUNC_ENTRY();
        
        //Lambda function of adding new vertex infomation
        auto addNewVertexInfo = []( ssGUI::DrawingEntity& currentEntity, 
                                    glm::vec2 newVertex, 
                                    const ssGUI::DrawingEntity& originalEntity,
                                    std::vector<bool>& currentVertexChanged,
                                    bool changed,
                                    int originalEntityVertexIndex)
        {
            // ssGUI_DEBUG(ssGUI_EXT_TAG,"newVertex: "<<newVertex.x<<", "<<newVertex.y);
            currentEntity.Vertices.push_back(newVertex);
            currentEntity.Colors.push_back(originalEntity.Colors.at(originalEntityVertexIndex));
            
            if(!originalEntity.TexCoords.empty())
            {
                currentEntity.BackendImage = originalEntity.BackendImage;
                currentEntity.BackendFont = originalEntity.BackendFont;
                currentEntity.TexCoords.push_back(originalEntity.TexCoords.at(originalEntityVertexIndex));
            }
            currentVertexChanged.push_back(changed);
            
            //Sanity check
            if( currentEntity.Vertices.size() != currentEntity.Colors.size() || 
                (originalEntity.BackendImage != nullptr && currentEntity.Colors.size() != currentEntity.TexCoords.size()))
            {
                ssGUI_ERROR(ssGUI_EXT_TAG, "Number of vertices or colors or texcoords not matching. This should not happen.");
            }
        };

        //Get all the mask vertices that are contained within the shape
        std::vector<int> containedMaskVertices;
        for(int i = 0; i < maskVerticies.size(); i++)
        {
            if(IsPointContainedInShape(maskVerticies[i], originalEntity.Vertices))
                containedMaskVertices.push_back(i);
        }

        // ssGUI_DEBUG(ssGUI_EXT_TAG,"start");
        //Iterating each vertex in the shape
        for(int currentShapeVertexIndex = 0; currentShapeVertexIndex < originalEntity.Vertices.size(); currentShapeVertexIndex++)
        {
            // ssGUI_DEBUG(ssGUI_EXT_TAG,"currentShapeVertexIndex: "<<currentShapeVertexIndex);

            bool isCurVertexContained = IsPointContainedInMask(originalEntity.Vertices[currentShapeVertexIndex], maskMin, maskMax);
            std::vector<int> curIntersectionIndices;

            //Get all the related intersections
            for(int i = 0; i < shapeIntersectIndices.size(); i++)
            {
                if(shapeIntersectIndices[i] == currentShapeVertexIndex)
                    curIntersectionIndices.push_back(i);
            }

            if(isCurVertexContained)
            {
                // ssGUI_DEBUG(ssGUI_EXT_TAG,"in");
                //If current vertex is inside the mask, add it to new shape
                addNewVertexInfo(   currentEntity, 
                                    originalEntity.Vertices[currentShapeVertexIndex], 
                                    originalEntity, 
                                    currentVertexChanged, 
                                    false,
                                    currentShapeVertexIndex);

                //If there's any intersection, add it
                if(!curIntersectionIndices.empty())
                {
                    addNewVertexInfo(   currentEntity,
                                        intersections[curIntersectionIndices[0]],
                                        originalEntity,
                                        currentVertexChanged,
                                        true,
                                        currentShapeVertexIndex);

                    //Find if there's any mask vertices contained in the shape, if so add them
                    if(!containedMaskVertices.empty())
                    {
                        int curMaskIndex = maskIntersectIndices[curIntersectionIndices[0]];
                        int maskVertexCounter = 0;

                        for(int loopCounter = 0; loopCounter < 100; loopCounter++)
                        {
                            curMaskIndex = GetNextIndex(0, maskVerticies.size(), curMaskIndex);
                            bool found = false;
                            for(int i = 0; i < containedMaskVertices.size(); i++)
                            {
                                if(containedMaskVertices[i] == curMaskIndex)
                                {
                                    addNewVertexInfo(   currentEntity,
                                                        maskVerticies[curMaskIndex],
                                                        originalEntity,
                                                        currentVertexChanged,
                                                        true, 
                                                        currentShapeVertexIndex);
                                    maskVertexCounter++;
                                    found = true;
                                    break;
                                }
                            }

                            if(!found || maskVertexCounter >= maskVerticies.size())
                                break;
                                
                            if(loopCounter == 99)
                            {
                                ssGUI_ERROR(ssGUI_EXT_TAG, "Failed to find vertex");
                                return;
                            }
                        }
                    }
                }
            }
            else
            {
                // ssGUI_DEBUG(ssGUI_EXT_TAG,"out");
                //If there are intersections, check how many are there
                if(!curIntersectionIndices.empty())
                {
                    if(curIntersectionIndices.size() > 1)
                    {
                        //Find the closing intersection first
                        if(glm::distance2(originalEntity.Vertices[currentShapeVertexIndex], intersections[curIntersectionIndices.at(0)]) > 
                            glm::distance2(originalEntity.Vertices[currentShapeVertexIndex], intersections[curIntersectionIndices.at(1)]))
                        {
                            auto temp = curIntersectionIndices[0];
                            curIntersectionIndices[0] = curIntersectionIndices[1];
                            curIntersectionIndices[1] = temp; 
                        }   
                    }

                    //Add closing intersection first
                    addNewVertexInfo(   currentEntity, 
                                        intersections[curIntersectionIndices[0]],
                                        originalEntity,
                                        currentVertexChanged,
                                        true,
                                        currentShapeVertexIndex);

                    //Add opening intersection (if there's any)
                    if(curIntersectionIndices.size() > 1)
                    {
                        addNewVertexInfo(   currentEntity,
                                            intersections[curIntersectionIndices[1]], 
                                            originalEntity,
                                            currentVertexChanged,
                                            true,
                                            currentShapeVertexIndex);
                        
                        //Find if there's any mask vertices contained in the shape, if so add them
                        if(!containedMaskVertices.empty())
                        {
                            int curMaskIndex = maskIntersectIndices[curIntersectionIndices[1]];
                            int maskVertexCounter = 0;

                            while (true)
                            {
                                curMaskIndex = GetNextIndex(0, maskVerticies.size(), curMaskIndex);
                                bool found = false;
                                for(int i = 0; i < containedMaskVertices.size(); i++)
                                {
                                    if(containedMaskVertices[i] == curMaskIndex)
                                    {
                                        addNewVertexInfo(   currentEntity,
                                                            maskVerticies[curMaskIndex], 
                                                            originalEntity,
                                                            currentVertexChanged,
                                                            true,
                                                            currentShapeVertexIndex);
                                        
                                        maskVertexCounter++;
                                        found = true;
                                        break;
                                    }
                                }

                                if(!found || maskVertexCounter >= maskVerticies.size())
                                    break;
                            }
                        }
                    }
                }
            }
        }

        ssLOG_FUNC_EXIT();
    }

    void Mask::SampleNewUVsAndColoursForShapes( const std::vector<DrawingEntity>& originalEntities, 
                                                std::vector<DrawingEntity>& newEntities, 
                                                std::vector<std::vector<bool>>& changed)
    {
        ssLOG_FUNC_ENTRY();
        
        //Sample the new UVs and colours
        if(changed.size() != newEntities.size())
        {
            ssGUI_ERROR(ssGUI_EXT_TAG, "size for entities changed not matching size of new entities.");
            return;
        }

        for(int shapeIndex = 0; shapeIndex < newEntities.size(); shapeIndex++)
        {
            const ssGUI::DrawingEntity& curOriginalShape = originalEntities[shapeIndex];
            ssGUI::DrawingEntity& curNewShape = newEntities[shapeIndex];
            
            for(int vertexIndex = 0; vertexIndex < curNewShape.Vertices.size(); vertexIndex++)
            {
                assert(changed[shapeIndex].size() == curNewShape.Vertices.size());

                if(!changed[shapeIndex][vertexIndex])
                    continue;

                int closestIndicies[] = {0, 0, 0};

                if(!GetSampleIndicesFromShape(curOriginalShape.Vertices, closestIndicies, curNewShape.Vertices[vertexIndex]))
                    break;
                
                glm::vec2 axis = curOriginalShape.Vertices[closestIndicies[1]] - curOriginalShape.Vertices[closestIndicies[0]];
                glm::vec2 axis2 = curOriginalShape.Vertices[closestIndicies[2]] - curOriginalShape.Vertices[closestIndicies[0]];
                float axisValue = 0;
                float axis2Value = 0;

                if(!GetAxesValues(axis, axis2, curNewShape.Vertices[vertexIndex] - curOriginalShape.Vertices[closestIndicies[0]], axisValue, axis2Value))  
                    break;

                if(!curOriginalShape.TexCoords.empty())
                {
                    glm::vec2 uvAxis = curOriginalShape.TexCoords[closestIndicies[1]] - curOriginalShape.TexCoords[closestIndicies[0]];
                    glm::vec2 uvAxis2 = curOriginalShape.TexCoords[closestIndicies[2]] - curOriginalShape.TexCoords[closestIndicies[0]];
                    
                    curNewShape.TexCoords[vertexIndex] =    (curOriginalShape.TexCoords[closestIndicies[0]]) +
                                                            (uvAxis) * axisValue +
                                                            (uvAxis2) * axis2Value;
                }
                
                glm::vec4 colourAxis = (glm::vec4)curOriginalShape.Colors[closestIndicies[1]] - (glm::vec4)curOriginalShape.Colors[closestIndicies[0]];
                glm::vec4 colourAxis2 = (glm::vec4)curOriginalShape.Colors[closestIndicies[2]] - (glm::vec4)curOriginalShape.Colors[closestIndicies[0]];

                curNewShape.Colors[vertexIndex] =   glm::vec4(curOriginalShape.Colors[closestIndicies[0]]) +
                                                    glm::vec4(colourAxis) * axisValue +
                                                    glm::vec4(colourAxis2) * axis2Value;
            }
        }
        ssLOG_FUNC_EXIT();
    }

    bool Mask::LineToLineIntersection(  glm::vec2 linePointA, glm::vec2 linePointB, 
                                        glm::vec2 linePointC, glm::vec2 linePointD, glm::vec2& intersection)
    {
        //AABB check
        //X axis
        float minX_AB;
        float maxX_AB;
        float minX_CD;
        float maxX_CD;
        if(linePointA.x < linePointB.x)
        {
            minX_AB = linePointA.x;
            maxX_AB = linePointB.x;
        }
        else
        {
            minX_AB = linePointB.x;
            maxX_AB = linePointA.x;
        }

        if(linePointC.x < linePointD.x)
        {
            minX_CD = linePointC.x;
            maxX_CD = linePointD.x;
        }
        else
        {
            minX_CD = linePointD.x;
            maxX_CD = linePointC.x;
        }

        if(!CheckLinesOverlapping(minX_AB, maxX_AB, minX_CD, maxX_CD))
            return false;

        //Y axis
        float minY_AB;
        float maxY_AB;
        float minY_CD;
        float maxY_CD;
        if(linePointA.y < linePointB.y)
        {
            minY_AB = linePointA.y;
            maxY_AB = linePointB.y;
        }
        else
        {
            minY_AB = linePointB.y;
            maxY_AB = linePointA.y;
        }
        if(linePointC.y < linePointD.y)
        {
            minY_CD = linePointC.y;
            maxY_CD = linePointD.y;
        }
        else
        {
            minY_CD = linePointD.y;
            maxY_CD = linePointC.y;
        }

        if(!CheckLinesOverlapping(minY_AB, maxY_AB, minY_CD, maxY_CD))
            return false;
        
        

        //Radius check
        /*
        glm::ivec2 midPointAB = glm::ivec2((minX_AB + maxX_AB) * 0.5, (minY_AB + maxY_AB) * 0.5);
        glm::ivec2 midPointCD = glm::ivec2((minX_CD + maxX_CD) * 0.5, (minY_CD + maxY_CD) * 0.5);

        glm::ivec2 diffAB = (linePointA - midPointAB);
        glm::ivec2 diffCD = (linePointC - midPointCD);
        int radiusAB = sqrt(diffAB.x * diffAB.x + diffAB.y * diffAB.y);
        int radiusCD = sqrt(diffCD.x * diffCD.x + diffCD.y * diffCD.y);

        if(radiusAB <= 0.0001 || radiusCD <= 0.0001)
            return false;

        glm::ivec2 diffABCD = midPointAB - midPointCD;

        //https://stackoverflow.com/questions/1736734/circle-circle-collision
        if(diffABCD.x * diffABCD.x + diffABCD.y * diffABCD.y > (radiusAB + radiusCD) * (radiusAB + radiusCD))
            return false; 

        //Check if AB and CD is parellel or not (1 = parallel)
        //https://www.mathsisfun.com/algebra/vectors-dot-product.html
        if(abs((diffAB.x * diffCD.x + diffAB.y * diffCD.y) / (radiusAB * radiusCD)) >= 0.9999)
            return false;
        */


        //Find intersection of AB and CD
        //https://www.codeproject.com/Tips/862988/Find-the-Intersection-Point-of-Two-Line-Segments

        glm::vec2 ba = linePointB - linePointA;
        glm::vec2 dc = linePointD - linePointC;
        glm::vec2 ca = linePointC - linePointA;
        
        float ba_cross_dc = glm::cross(glm::vec3(ba, 0), glm::vec3(dc, 0)).z;
        float ca_cross_ba = glm::cross(glm::vec3(ca, 0), glm::vec3(ba, 0)).z;
        float ca_cross_dc = glm::cross(glm::vec3(ca, 0), glm::vec3(dc, 0)).z;

        /* std::cout<<"ba: "<<ba.x<<", "<<ba.y<<"\n";
        std::cout<<"dc: "<<dc.x<<", "<<dc.y<<"\n";
        std::cout<<"ca: "<<ca.x<<", "<<ca.y<<"\n";
        std::cout<<"crosses: "<<ba_cross_dc<<", "<<ca_cross_ba<<", "<<ca_cross_dc<<"\n";
 */
        if(ba_cross_dc == 0)
            return false;
        
        //TODO: Add checks for reaching max limit
        float t = ca_cross_dc / ba_cross_dc;
        float u = ca_cross_ba / ba_cross_dc;

        if(t >= 0 && t <= 1 && u >= 0 && u <= 1)
            intersection = linePointA + glm::vec2(ba.x * t, ba.y * t);
        else
            return false;

        return true;
    }

/*     bool Mask::LineToShapeIntersection( glm::ivec2 linePointA, glm::ivec2 linePointB,
                                        std::vector<glm::ivec2>& shape, int& shapeIntersectionIndex, glm::ivec2& intersection)
    {
        for(int i = 0; i < shape.size(); i++)
        {
            if(!LineToLineIntersection(linePointA, linePointB, shape[i], shape[GetNextIndex(0, shape.size(), i)], intersection))
                continue;
            
            return true;
        }
    } */

    bool Mask::CheckLinesOverlapping(float minA, float maxA, float minB, float maxB)
    {        
        //[====a====] [====b====]
        if(maxA < minB)
            return false;

        //[====b====] [====a====]
        if(maxB < minA)
            return false;

        //[====a====]           OR      [======a======]
        //      [====b====]                [====b====]
        if(maxA >= minB && minA <= minB)
            return true;
        
        //[====b====]           OR      [======b======]
        //      [====a====]                [====a====]
        if(maxB >= minA && minB <= minA)
            return true;

        //Should never be able to reach this point
        ssGUI_WARNING(ssGUI_EXT_TAG, "Unexpected input. minA: "<<minA<<", maxA: "<<maxA<<", minB: "<<minB<<", maxB: "<<maxB);
        return false;
    }

/*     int Mask::GetIndicesDistance(int count, int startIndex, int endIndex)
    {
        if(endIndex >= startIndex)
            return endIndex - startIndex;
        
        return endIndex + count - 1 - startIndex;
    } */

    int Mask::GetNextIndex(int indexOffset, int count, int currentIndex)
    {
        if(currentIndex + 1 > indexOffset + count - 1)
            return indexOffset;
        
        return ++currentIndex;
    }

    int Mask::GetPreviousIndex(int indexOffset, int count, int currentIndex)
    {
        if(currentIndex <= indexOffset)
            return indexOffset + count - 1;
        
        return --currentIndex;
    }

    bool Mask::GetSampleIndicesFromShape(const std::vector<glm::vec2>& vertices, int closestIndices[], glm::vec2 samplePoint)
    {
        ssLOG_FUNC_ENTRY();
        
        if(vertices.size() < 3)
        {
            ssGUI_ERROR(ssGUI_EXT_TAG, "We are trying to sample a shape that has less than 3 vertices, this is invalid");
            ssLOG_FUNC_EXIT();
            return false;
        }

        closestIndices[0] = -1;
        closestIndices[1] = -1;
        closestIndices[2] = -1;

        //float firstDist = glm::distance2(glm::vec2(samplePoint), glm::vec2(vertices[0]));
        float closestDistances[3] = {-1, -1, -1};

        for(int i = 0; i < vertices.size(); i++)
        {
            float curDist = glm::distance2(glm::vec2(samplePoint), glm::vec2(vertices[i]));   

            if(closestIndices[0] == -1)
            {
                closestDistances[0] = curDist;
                closestIndices[0] = i;
                continue;
            }
            else if(closestIndices[1] == -1)
            {
                if(curDist < closestDistances[0])
                {
                    closestIndices[1] = closestIndices[0];
                    closestDistances[1] = closestDistances[0];
                    closestIndices[0] = i;
                    closestDistances[0] = curDist;
                }
                else
                {
                    closestDistances[1] = curDist;
                    closestIndices[1] = i;
                }
                continue;
            }
            else if(closestIndices[2] == -1)
            {
                if(curDist < closestDistances[0])
                {
                    closestIndices[2] = closestIndices[1];
                    closestDistances[2] = closestDistances[1];
                    closestIndices[1] = closestIndices[0];
                    closestDistances[1] = closestDistances[0];
                    closestIndices[0] = i;
                    closestDistances[0] = curDist;
                }
                else if(curDist < closestDistances[1])
                {
                    closestIndices[2] = closestIndices[1];
                    closestDistances[2] = closestDistances[1];
                    closestIndices[1] = i;
                    closestDistances[1] = curDist;
                }
                else
                {
                    closestDistances[2] = curDist;
                    closestIndices[2] = i;
                }
                continue;
            }

            if((curDist < closestDistances[2] && curDist > closestDistances[1]))
            {
                closestDistances[2] = curDist;
                closestIndices[2] = i;
            }
            else if(curDist < closestDistances[1] && curDist > closestDistances[0])
            {
                closestDistances[2] = closestDistances[1];
                closestIndices[2] = closestIndices[1];
                closestDistances[1] = curDist;
                closestIndices[1] = i;
            }
            else if(curDist < closestDistances[0])
            {
                closestDistances[2] = closestDistances[1];
                closestIndices[2] = closestIndices[1];
                closestDistances[1] = closestDistances[0];
                closestIndices[1] = closestIndices[0];
                closestDistances[0] = curDist;
                closestIndices[0] = i;
            }
        }
        
        ssLOG_FUNC_EXIT();
        return true;
    }

    void Mask::AddMaskEnforcerToChildren(ssGUI::GUIObject* parent, bool includeParent)
    {
        ssLOG_FUNC_ENTRY();
        
        if(Container == nullptr)
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        std::queue<ssGUI::GUIObject*> children;

        if(!includeParent)
        {
            Container->StashChildrenIterator();
            Container->MoveChildrenIteratorToFirst();
            while (!Container->IsChildrenIteratorEnd())
            {
                children.push(Container->GetCurrentChild());
                Container->MoveChildrenIteratorNext();
            }
            Container->PopChildrenIterator();
        }
        else
            children.push(parent);
        
        while (!children.empty())
        {
            ssGUI::GUIObject* child = children.front();

            //Don't mask container's composite children
            Container->StashChildrenIterator();
            if(Container->MoveChildrenIteratorToChild(child) && Container->IsCurrentChildComposite())
            {
                Container->PopChildrenIterator();
                children.pop();
                continue;
            }
            Container->PopChildrenIterator();

            if(!child->IsExtensionExist(ssGUI::Extensions::MaskEnforcer::EXTENSION_NAME))
            {
                child   ->AddExtension<ssGUI::Extensions::MaskEnforcer>()
                        ->AddTargetMaskObject(Container);
            }
            else
            {
                ssGUI::Extensions::MaskEnforcer* maskEnforcer = static_cast<ssGUI::Extensions::MaskEnforcer*>(
                    child->GetExtension(ssGUI::Extensions::MaskEnforcer::EXTENSION_NAME));
                
                if(!maskEnforcer->HasTargetMaskObject(Container))
                    maskEnforcer->AddTargetMaskObject(Container);
            }

            child->StashChildrenIterator();
            child->MoveChildrenIteratorToFirst();
            while (!child->IsChildrenIteratorEnd())
            {
                children.push(child->GetCurrentChild());   
                child->MoveChildrenIteratorNext();
            }
            child->StashChildrenIterator();

            children.pop();
        }

        ssLOG_FUNC_EXIT();
    }

    void Mask::RemoveMaskEnforcerToChildren(ssGUI::GUIObject* parent, bool includeParent)
    {
        ssLOG_FUNC_ENTRY();

        if(Container == nullptr)
        {
            ssLOG_FUNC_EXIT();
            return;
        }
        
        std::queue<ssGUI::GUIObject*> children;
        if(!includeParent)
        {            
            Container->StashChildrenIterator();
            Container->MoveChildrenIteratorToFirst();
            while (!Container->IsChildrenIteratorEnd())
            {
                children.push(Container->GetCurrentChild());
                Container->MoveChildrenIteratorNext();
            }
            Container->PopChildrenIterator();
        }
        else
            children.push(parent);
        
        while (!children.empty())
        {
            ssGUI::GUIObject* child = children.front();

            if(child->IsExtensionExist(ssGUI::Extensions::MaskEnforcer::EXTENSION_NAME))
            {
                ssGUI::Extensions::MaskEnforcer* enforcer = dynamic_cast<ssGUI::Extensions::MaskEnforcer*>
                    (child->GetExtension(ssGUI::Extensions::MaskEnforcer::EXTENSION_NAME));
                
                //Check if it has more than 1 Target Mask GUI Object, if just remove Target Mask GUI Object
                if(enforcer->HasTargetMaskObject(Container))
                {
                    if(enforcer->GetTargetMaskObjects().size() == 1)
                        child->RemoveExtension(ssGUI::Extensions::MaskEnforcer::EXTENSION_NAME);
                    else
                        enforcer->RemoveTargetMaskObject(Container);
                }
            }
            
            child->StashChildrenIterator();
            child->MoveChildrenIteratorToFirst();
            while (!child->IsChildrenIteratorEnd())
            {
                children.push(child->GetCurrentChild());   
                child->MoveChildrenIteratorNext();
            }
            child->PopChildrenIterator();

            children.pop();
        }

        ssLOG_FUNC_EXIT();
    }

    void Mask::ConstructRenderInfo()
    {}

    void Mask::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {}

    Mask::Mask() :  Container(nullptr),
                    Enabled(true),
                    MaskChildren(true),
                    MaskContainer(false),
                    FollowContainer(true),
                    FollowPositionOffset(1, 1),
                    FollowSizePadding(-2, -2),
                    GlobalPosition(),
                    Size()
    {}

    Mask::~Mask()
    {
        if(Container != nullptr)
        {
            auto eventCallbackCleanUp = [&](ssGUI::GUIObject* target, ssGUI::Enums::EventType eventType)
            {
                if(!target->IsEventCallbackExist(eventType))
                    return;

                target->GetEventCallback(eventType)->RemoveEventListener(EXTENSION_NAME);
            
                if(target->GetEventCallback(eventType)->GetEventListenerCount() == 0)
                    target->RemoveEventCallback(eventType);
            };

            eventCallbackCleanUp(Container, ssGUI::Enums::EventType::RECURSIVE_CHILD_ADDED);
            eventCallbackCleanUp(Container, ssGUI::Enums::EventType::RECURSIVE_CHILD_REMOVED);
        }
    } 

    Mask::Mask(Mask const& other)
    {
        Container = nullptr;
        Enabled = other.IsEnabled();
        MaskChildren = other.GetMaskChildren();
        MaskContainer = other.IsMaskContainer();
        FollowContainer = other.GetFollowContainer();
        FollowPositionOffset = other.GetFollowPositionOffset();
        FollowSizePadding = other.GetFollowSizePadding();
        GlobalPosition = other.GetGlobalPosition();
        Size = other.GetSize();
    }

    const std::string Mask::EXTENSION_NAME = "Mask";

    void Mask::SetMaskChildren(bool maskChildren)
    {
        ssLOG_FUNC_ENTRY();
        
        MaskChildren = maskChildren;

        if(Container == nullptr)
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        std::queue<ssGUI::GUIObject*> children;

        Container->StashChildrenIterator();
        Container->MoveChildrenIteratorToFirst();
        while (!Container->IsChildrenIteratorEnd())
        {
            children.push(Container->GetCurrentChild());
            Container->MoveChildrenIteratorNext();
        }
        Container->PopChildrenIterator();

        if(MaskChildren)
        {
            AddMaskEnforcerToChildren(Container, IsMaskContainer());      
            if(Container->IsEventCallbackExist(ssGUI::Enums::EventType::RECURSIVE_CHILD_ADDED))
            {
                Container->GetEventCallback(ssGUI::Enums::EventType::RECURSIVE_CHILD_ADDED)->AddEventListener
                (
                    EXTENSION_NAME,
                    [this](ssGUI::EventInfo& info){Internal_RecursiveChildAdded(info.EventSource);}         //TODO: Use ObjectsReferences instead of this
                    // std::bind(&ssGUI::Extensions::Mask::Internal_RecursiveChildAdded, this, std::placeholders::_1)
                );
            }
            else
            {
                auto* event = Container->AddEventCallback(ssGUI::Enums::EventType::RECURSIVE_CHILD_ADDED);

                event->AddEventListener
                (
                    EXTENSION_NAME,
                    [this](ssGUI::EventInfo& info){Internal_RecursiveChildAdded(info.EventSource);}         //TODO: Use ObjectsReferences instead of this
                    // std::bind(&ssGUI::Extensions::Mask::Internal_RecursiveChildAdded, this, std::placeholders::_1)
                );
            }

            if(Container->IsEventCallbackExist(ssGUI::Enums::EventType::RECURSIVE_CHILD_REMOVED))
            {
                Container->GetEventCallback(ssGUI::Enums::EventType::RECURSIVE_CHILD_REMOVED)->AddEventListener
                (
                    EXTENSION_NAME,
                    [this](ssGUI::EventInfo& info){Internal_RecursiveChildRemoved(info.EventSource);}         //TODO: Use ObjectsReferences instead of this
                    // std::bind(&ssGUI::Extensions::Mask::Internal_RecursiveChildRemoved, this, std::placeholders::_1)
                );
            }
            else
            {
                auto* event = Container->AddEventCallback(ssGUI::Enums::EventType::RECURSIVE_CHILD_REMOVED);

                event->AddEventListener
                (
                    EXTENSION_NAME,
                    [this](ssGUI::EventInfo& info){Internal_RecursiveChildRemoved(info.EventSource);}         //TODO: Use ObjectsReferences instead of this
                    // std::bind(&ssGUI::Extensions::Mask::Internal_RecursiveChildRemoved, this, std::placeholders::_1)
                );
            }
        }
        else
        {
            RemoveMaskEnforcerToChildren(Container, IsMaskContainer());
            if(Container->IsEventCallbackExist(ssGUI::Enums::EventType::RECURSIVE_CHILD_ADDED))
            {
                Container->GetEventCallback(ssGUI::Enums::EventType::RECURSIVE_CHILD_ADDED)->
                    RemoveEventListener(EXTENSION_NAME);
                
            }

            if(Container->IsEventCallbackExist(ssGUI::Enums::EventType::RECURSIVE_CHILD_REMOVED))
            {
                Container->GetEventCallback(ssGUI::Enums::EventType::RECURSIVE_CHILD_REMOVED)->
                    RemoveEventListener(EXTENSION_NAME);
                
            }
        }

        ssLOG_FUNC_EXIT();
    }

    bool Mask::GetMaskChildren() const
    {
        return MaskChildren;
    }

    void Mask::SetMaskContainer(bool maskContainer)
    {
        ssLOG_FUNC_ENTRY();
        MaskContainer = maskContainer;

        if(Container == nullptr)
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        if(maskContainer)
        {
            if(!Container->IsExtensionExist(ssGUI::Extensions::MaskEnforcer::EXTENSION_NAME))
            {
                Container   ->AddExtension<ssGUI::Extensions::MaskEnforcer>()
                            ->AddTargetMaskObject(Container);
            }
            else
            {
                ssGUI::Extensions::MaskEnforcer* enforcer = static_cast<ssGUI::Extensions::MaskEnforcer*>
                    (Container->GetExtension(ssGUI::Extensions::MaskEnforcer::EXTENSION_NAME));
                
                if(!enforcer->HasTargetMaskObject(Container))
                    enforcer->AddTargetMaskObject(Container);
            }
        }
        else
        {
            if(Container->IsExtensionExist(ssGUI::Extensions::MaskEnforcer::EXTENSION_NAME) && static_cast<ssGUI::Extensions::MaskEnforcer*>
                (Container->GetExtension(ssGUI::Extensions::MaskEnforcer::EXTENSION_NAME))->HasTargetMaskObject(Container))
            {
                ssGUI::Extensions::MaskEnforcer* enforcer = static_cast<ssGUI::Extensions::MaskEnforcer*>
                    (Container->GetExtension(ssGUI::Extensions::MaskEnforcer::EXTENSION_NAME));
                
                if(enforcer->GetTargetMaskObjects().size() == 1)
                    Container->RemoveExtension(ssGUI::Extensions::MaskEnforcer::EXTENSION_NAME);
                else
                    enforcer->RemoveTargetMaskObject(Container);
            }
        }

        ssLOG_FUNC_EXIT();
    }

    bool Mask::IsMaskContainer() const
    {
        return MaskContainer;
    }

    void Mask::SetFollowContainer(bool followContainer)
    {
        FollowContainer = followContainer;
        if(Container != nullptr)
            Container->RedrawObject();
    }

    bool Mask::GetFollowContainer() const
    {
        return FollowContainer;
    }

    void Mask::SetFollowPositionOffset(glm::vec2 positionOffset)
    {
        FollowPositionOffset = positionOffset;

        if(Container != nullptr)
            Container->RedrawObject();
    }

    glm::vec2 Mask::GetFollowPositionOffset() const
    {
        return FollowPositionOffset;
    }

    void Mask::SetFollowSizePadding(glm::vec2 sizePadding)
    {
        FollowSizePadding = sizePadding;

        if(Container != nullptr)
            Container->RedrawObject();
    }

    glm::vec2 Mask::GetFollowSizePadding() const
    {
        return FollowSizePadding;
    }

    void Mask::SetGlobalPosition(glm::vec2 globalPosition)
    {
        if(Container != nullptr && GlobalPosition != globalPosition)
            Container->RedrawObject();

        GlobalPosition = globalPosition;
    }

    glm::vec2 Mask::GetGlobalPosition() const
    {
        return GlobalPosition;
    }

    void Mask::SetSize(glm::vec2 size)
    {
        if(Container != nullptr && Size != size)
            Container->RedrawObject();
        
        Size = size;
    }

    glm::vec2 Mask::GetSize() const
    {
        return Size;
    }

    bool Mask::IsPointContainedInMask(glm::vec2 point) const
    {
        return IsPointContainedInMask(point, GetGlobalPosition(), GetGlobalPosition() + GetSize());
    }

    void Mask::Internal_RecursiveChildAdded(ssGUI::GUIObject* child)
    {
        ssLOG_FUNC_ENTRY();
        AddMaskEnforcerToChildren(child, true);
        ssLOG_FUNC_EXIT();
    }

    void Mask::Internal_RecursiveChildRemoved(ssGUI::GUIObject* child)
    {
        ssLOG_FUNC_ENTRY();
        RemoveMaskEnforcerToChildren(child, true);
        ssLOG_FUNC_EXIT();
    }

    void Mask::MaskObject(ssGUI::GUIObject* obj, glm::vec2 maskOffset, const std::vector<ssGUI::TargetShape>& applyShapes)
    {
        ssLOG_FUNC_ENTRY();
        
        std::vector<glm::vec2> maskShape;
        
        const std::vector<ssGUI::DrawingEntity>& originalEntities = obj->Extension_GetDrawingEntities();
        std::vector<ssGUI::DrawingEntity> newEntities;
        std::vector<std::vector<bool>> changed;
        
        glm::vec2 maskMin = GetGlobalPosition() + maskOffset;
        glm::vec2 maskMax = maskMin + GetSize();

        maskShape.push_back(maskMin);
        maskShape.push_back(maskMin + glm::vec2(GetSize().x, 0));
        maskShape.push_back(maskMax);
        maskShape.push_back(maskMin + glm::vec2(0, GetSize().y));

        //Iterating each shape
        for(int shapeIndex = 0; shapeIndex < originalEntities.size(); shapeIndex++)
        {
            if(!applyShapes.empty())
            {
                //Check if the current shape needs to be masked or not
                bool applyMask = false;
                for(int i = 0; i < applyShapes.size(); i++)
                {
                    if( applyShapes[i].ShapeIndex != NO_SHAPE_INDEX && 
                        applyShapes[i].ShapeIndex + obj->Extension_GetGUIObjectFirstShapeIndex() == shapeIndex)
                    {
                        applyMask = true;
                        break;
                    }
                    else if(applyShapes[i].ShapeName == originalEntities[shapeIndex].EntityName)
                    {
                        applyMask = true;
                        break;
                    }
                }

                //If we don't need to apply any masks, just go to next shape
                if(!applyMask)
                {
                    changed.push_back(std::vector<bool>(originalEntities.at(shapeIndex).Vertices.size(), false));
                    newEntities.push_back(originalEntities.at(shapeIndex));
                    continue;
                }
            }

            //Do AABB test for the shape and the mask
            glm::vec2 shapeMin;
            glm::vec2 shapeMax;
            if(!IsAABBOverlap(originalEntities.at(shapeIndex).Vertices, maskMin, maskMax, shapeMin, shapeMax))
            {
                changed.push_back(std::vector<bool>());
                newEntities.push_back(ssGUI::DrawingEntity());
                continue;
            }
                
            { //Extra bracket to create scope for the vectors below

                //Find all the intersections (Order of the intersections are not guaranteed)
                std::vector<glm::vec2> intersections;
                std::vector<int> shapeIntersectIndices;
                std::vector<int> maskIntersectIndices;

                GetIntersections(   intersections, 
                                    shapeIntersectIndices, 
                                    maskIntersectIndices, 
                                    originalEntities.at(shapeIndex).Vertices, 
                                    maskShape);

                // ssGUI_DEBUG(ssGUI_EXT_TAG, "intersections count: "<<intersections.size());

                ssGUI::DrawingEntity curEntity;
                //std::vector<bool> currentVertexChanged;

                //If there's no intersection, check if the mask is contained in the shape instead
                if(intersections.empty() && shapeMin.x < maskMin.x && shapeMax.x > maskMax.x && shapeMin.y < maskMin.y && shapeMax.y > maskMax.y)
                {
                    //Change the shape into mask
                    curEntity.EntityName = originalEntities.at(shapeIndex).EntityName;
                    curEntity.Vertices.push_back(maskMin);
                    curEntity.Vertices.push_back(maskMin + glm::vec2(GetSize().x, 0));
                    curEntity.Vertices.push_back(maskMin + GetSize());
                    curEntity.Vertices.push_back(maskMin + glm::vec2(0, GetSize().y));
                    curEntity.Colors.assign(4, glm::u8vec4());
                    
                    if(!originalEntities.at(shapeIndex).TexCoords.empty())
                    {
                        curEntity.BackendImage = originalEntities.at(shapeIndex).BackendImage;
                        curEntity.BackendFont = originalEntities.at(shapeIndex).BackendFont;
                        curEntity.TexCoords.assign(4, glm::vec2());
                    }

                    //currentVertexChanged.assign(4, true);
                    changed.push_back(std::vector<bool>(4, true));
                }
                //If there's intersection, form a new shape with intersections
                else if(!intersections.empty())
                {
                    changed.push_back(std::vector<bool>());
                    FormNewShapeWithIntersections(  curEntity, 
                                                    changed.back(), 
                                                    originalEntities.at(shapeIndex), 
                                                    maskMin,
                                                    maskMax,
                                                    maskShape,
                                                    intersections,
                                                    shapeIntersectIndices,
                                                    maskIntersectIndices);
                    
                    curEntity.EntityName = originalEntities.at(shapeIndex).EntityName;
                }
                //If the first vertex is outside of the mask, this means the shape is outside the mask
                else if(!IsPointContainedInMask(originalEntities.at(shapeIndex).Vertices.front(), maskMin, maskMax))
                {
                    changed.push_back(std::vector<bool>());
                    newEntities.push_back(ssGUI::DrawingEntity());
                    continue;
                }
                //Otherwise there's no intersection and that means the shape is inside the mask
                else
                {
                    newEntities.push_back(originalEntities.at(shapeIndex));
                    changed.push_back(std::vector<bool>(originalEntities.at(shapeIndex).Vertices.size(), false));
                    continue;
                }

                //If there's intersection, append the newly formed shape
                newEntities.push_back(curEntity);
            }   //Extra bracket to create scope to tell compiler the variables are not created if nextShape label is used
        }
        
        if(newEntities.size() != originalEntities.size())
        {
            ssGUI_ERROR("We are missing entity when forming a new shape, internal error");
            return;
        }

        //Sample the new UVs and colours
        SampleNewUVsAndColoursForShapes(originalEntities, newEntities, changed);

        //Copy the new entities to the original entities
        for(int i = newEntities.size() - 1; i >= 0; i--)
        {
            if(newEntities[i].Vertices.empty())
                newEntities.erase(newEntities.begin() + i);
        }
        
        obj->Extension_GetDrawingEntities() = newEntities;

        ssLOG_FUNC_EXIT();
    }

    void Mask::SetEnabled(bool enabled)
    {
        Enabled = enabled;

        if(Container != nullptr)
            Container->RedrawObject();
    }

    bool Mask::IsEnabled() const
    {
        return Enabled;
    }
    
    //Extension methods
    void Mask::Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, ssGUI::GUIObject* mainWindow)
    {
        ssLOG_FUNC_ENTRY();
        
        if(isPreUpdate || Container == nullptr || !Enabled)
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        if(FollowContainer)
        {
            if(Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW)
            {
                ssGUI::Window* window = static_cast<ssGUI::Window*>(Container);
                
                //Position
                SetGlobalPosition(Container->GetGlobalPosition() + 
                                    (window->HasTitlebar() ? glm::vec2(0, window->GetTitlebarHeight()) : glm::vec2()) +
                                    FollowPositionOffset);
                
                //Size
                SetSize(Container->GetSize() +
                        (window->HasTitlebar() ? glm::vec2(0, -window->GetTitlebarHeight()) : glm::vec2()) +
                        FollowSizePadding);
            }
            else
            {
                //Position
                SetGlobalPosition(Container->GetGlobalPosition() + FollowPositionOffset);
                
                //Size
                SetSize(Container->GetSize() + FollowSizePadding);
            }
        }

        ssLOG_FUNC_EXIT();
    }

    void Mask::Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        //Call mask function
    }

    std::string Mask::GetExtensionName()
    {
        return EXTENSION_NAME;
    }

    void Mask::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;
        
        if(GetMaskChildren())
            SetMaskChildren(true);  //Setting it to true in order to add mask enforcer extension as well as event callbacks
        
        if(IsMaskContainer())
            SetMaskContainer(true); //Same thing here
    }

    void Mask::Copy(ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        
        ssGUI::Extensions::Mask* mask = static_cast<ssGUI::Extensions::Mask*>(extension);
        
        Enabled = mask->IsEnabled();
        MaskChildren = mask->GetMaskChildren();
        MaskContainer = mask->IsMaskContainer();
        FollowContainer = mask->GetFollowContainer();
        FollowPositionOffset = mask->GetFollowPositionOffset();
        FollowSizePadding = mask->GetFollowSizePadding();
        GlobalPosition = mask->GetGlobalPosition();
        Size = mask->GetSize();
    }

    ObjectsReferences* Mask::Internal_GetObjectsReferences()
    {
        return nullptr;
    }

    Mask* Mask::Clone()
    {
        Mask* temp = new Mask(*this);
        return temp;
    }
}

}