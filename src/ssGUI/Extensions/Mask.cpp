#include "ssGUI/Extensions/Mask.hpp"

#include "ssGUI/Extensions/MaskEnforcer.hpp"


namespace ssGUI::Extensions
{
    bool Mask::IsContained(glm::ivec2 point, glm::ivec2 min, glm::ivec2 max) const
    {
        bool isXIn = point.x >= min.x && point.x <= max.x;

        if(!isXIn)
            return false;
        
        bool isYIn = point.y >= min.y && point.y <= max.y;

        if(isYIn)
            return true;
        
        return false;
    }

    bool Mask::GetAxesValues(    glm::ivec2 axis, glm::ivec2 axis2, glm::ivec2 samplePoint, double& axisValue, double& axis2Value)
    {
        /*
        Note: I am not a mathematician and have no idea why this doesn't work.

        //Find intersection of AB and CD
        //https://www.codeproject.com/Tips/862988/Find-the-Intersection-Point-of-Two-Line-Segments


        //Finding axisValue for samplePoint
    
        //      |
        //      |
        //axis 2|   x samplePoint (C)
        //      |   |
        //      |___|_________
        //      A   | axis     B             
        //          |
        //          D

        glm::ivec2 ba = axis;
        glm::ivec2 dc = samplePoint - axis2;
        glm::ivec2 ca = samplePoint;

        float ba_cross_dc = glm::cross(glm::vec3(ba, 0), glm::vec3(dc, 0)).z;
        float ca_cross_ba = glm::cross(glm::vec3(ca, 0), glm::vec3(ba, 0)).z;
        float ca_cross_dc = glm::cross(glm::vec3(ca, 0), glm::vec3(dc, 0)).z;

        if(ba_cross_dc == 0)
            return false;
        
        //TODO: Add checks for reaching max limit
        axisValue = ca_cross_dc / ba_cross_dc;
        
        return true;

        //Finding axis2Value for samplePoint


        //      B
        // axis2|
        //      |
        //D-----|---x samplePoint (C)
        //      |   
        //      |____________
        //      A    axis                  
        //          
        // 

        ba = axis2;
        dc = samplePoint - axis;

        ba_cross_dc = glm::cross(glm::vec3(ba, 0), glm::vec3(dc, 0)).z;
        ca_cross_ba = glm::cross(glm::vec3(ca, 0), glm::vec3(ba, 0)).z;
        ca_cross_dc = glm::cross(glm::vec3(ca, 0), glm::vec3(dc, 0)).z;

        if(ba_cross_dc == 0)
            return false;

        axis2Value = ca_cross_dc / ba_cross_dc;

        return true;
        */

        //Credit: https://stackoverflow.com/questions/13937782/calculating-the-point-of-intersection-of-two-lines

        //Return the multiplier from a to reach intersection of ab and cd
        auto findIntersectMultiplier = [](glm::ivec2 a, glm::ivec2 b, glm::ivec2 c, glm::ivec2 d) -> double
        {
            int denom = (d.y - c.y) * (b.x - a.x) - (d.x - c.x) * (b.y - a.y);
            if(denom == 0)  
                return 0; 

            return (double)((d.x - c.x) * (a.y - c.y) - (d.y - c.y) * (a.x - c.x)) / (double)denom;
        };

        //      |
        //      |
        //axis 2|   x samplePoint (C)
        //      |   |
        //      |___|_________
        //      A   | axis     B             
        //          |
        //          D
        
        axisValue = findIntersectMultiplier(glm::ivec2(0, 0), axis, samplePoint, samplePoint - axis2);

        //      B
        // axis2|
        //      |
        //D-----|---x samplePoint (C)
        //      |   
        //      |____________
        //      A    axis                  
        //          
        // 

        axis2Value = findIntersectMultiplier(glm::ivec2(0, 0), axis2, samplePoint, samplePoint - axis);

        return true;
    }

    bool Mask::IsAABBOverlap(std::vector<glm::ivec2>& shapeVerticies, int shapeOffset, int shapeVertexCount, glm::ivec2 maskMin, glm::ivec2 maskMax,
                            glm::ivec2& shapeMin, glm::ivec2& shapeMax)
    {
        shapeMin = glm::ivec2(shapeVerticies[shapeOffset]);
        shapeMax = glm::ivec2(shapeVerticies[shapeOffset]);
        for(int j = shapeOffset + 1; j < shapeOffset + shapeVertexCount; j++)
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
            return false;
        
        if(!CheckLinesOverlapping(shapeMin.y, shapeMax.y, maskMin.y, maskMax.y))
            return false;
        
        return true;
    }

    void Mask::GetIntersections(std::vector<glm::ivec2>& intersections, std::vector<int>& shapeIntersectIndices, std::vector<int>& maskIntersectIndices,
                                std::vector<glm::ivec2>& shapeVerticies, int shapeOffset, int shapeVertexCount, std::vector<glm::ivec2>& maskVerticies)
    {
        //For each vertices for this shape
        for(int currentShapeVertexIndex = shapeOffset; currentShapeVertexIndex < shapeOffset + shapeVertexCount; currentShapeVertexIndex++)
        {
            int nextShapeVertexIndex = GetNextIndex(shapeOffset, shapeVertexCount, currentShapeVertexIndex);
            //Iterate every line on the mask and see which intersects
            for(int currentMaskIndex = 0; currentMaskIndex < maskVerticies.size(); currentMaskIndex++)
            {
                glm::ivec2 intersection;
                int nextMaskIndex = GetNextIndex(0, maskVerticies.size(), currentMaskIndex);
                if(LineToLineIntersection(shapeVerticies[currentShapeVertexIndex], shapeVerticies[nextShapeVertexIndex], 
                                            maskVerticies[currentMaskIndex], maskVerticies[nextMaskIndex], intersection))
                {
                    intersections.push_back(intersection);
                    shapeIntersectIndices.push_back(currentShapeVertexIndex);
                    maskIntersectIndices.push_back(currentMaskIndex);

                }
            }   
        }
    }

    void Mask::FromNewShapeWithIntersections(std::vector<glm::ivec2>& currentShapeVertices, std::vector<glm::ivec2>& currentShapeUVs, 
                                                std::vector<glm::u8vec4>& currentShapeColours, std::vector<bool>& currentVertexChanged,
                                                std::vector<glm::ivec2>& originalVerticies, std::vector<glm::u8vec4>& originalColours,
                                                std::vector<glm::ivec2>& originalUVs,
                                                glm::ivec2 maskMin, glm::ivec2 maskMax, int shapeOffset, int shapeVertexCount, 
                                                std::vector<glm::ivec2>& maskVerticies, std::vector<glm::ivec2>& intersections,
                                                std::vector<int>& shapeIntersectIndices, std::vector<int>& maskIntersectIndices)
    {
        FUNC_DEBUG_ENTRY();
        
        //Lambda function of adding new vertex infomation
        auto addNewVertexInfo = [&currentShapeVertices, &currentShapeUVs, &currentShapeColours, &currentVertexChanged]
        (glm::ivec2 newVertex, glm::u8vec4 newColour, bool changed, glm::ivec2 uv = glm::ivec2())
        {
            currentShapeVertices.push_back(newVertex);
            currentShapeUVs.push_back(uv);
            currentShapeColours.push_back(newColour);
            currentVertexChanged.push_back(changed);
        };
        
        int lastOpenIntersectionIndex = -1;     //Intersection index
        int lastCloseIntersectionIndex = -1;    //Intersection index
        //Iterating each vertex in the shape
        for(int currentShapeVertexIndex = shapeOffset; currentShapeVertexIndex < shapeOffset + shapeVertexCount; currentShapeVertexIndex++)
        {
            //If both shape vertices are inside the mask, add the current vertex
            if(IsContained(originalVerticies[currentShapeVertexIndex], maskMin, maskMax) && 
                IsContained(originalVerticies[GetNextIndex(shapeOffset, shapeVertexCount, currentShapeVertexIndex)], maskMin, maskMax))
            {                        
                addNewVertexInfo(originalVerticies[currentShapeVertexIndex], originalColours[shapeOffset], false, originalUVs[currentShapeVertexIndex]);
            }
            //If the line is going outside the mask from inside (Open intersection)
            else if(IsContained(originalVerticies[currentShapeVertexIndex], maskMin, maskMax) && 
                !IsContained(originalVerticies[GetNextIndex(shapeOffset, shapeVertexCount, currentShapeVertexIndex)], maskMin, maskMax))
            {
                //If the closing intersection index is not recorded, then record the current index for open intersection and add the intersection
                if(lastCloseIntersectionIndex == -1)
                {
                    addNewVertexInfo(originalVerticies[currentShapeVertexIndex], originalColours[shapeOffset], false, originalUVs[currentShapeVertexIndex]);

                    //Find the mask intersection index and it
                    int openIntersectionIndex = -1;
                    for(int j = 0; j < intersections.size(); j++)
                    {
                        //This block gets triggered when one of the points are sitting on the mask line
                        if(openIntersectionIndex != -1 && shapeIntersectIndices[j] == currentShapeVertexIndex)
                        {
                            //If the last intersection is closer to the start point of the line, use this intersection instead
                            if(glm::distance2(glm::vec2(originalVerticies[currentShapeVertexIndex]), glm::vec2(intersections[openIntersectionIndex])) <
                                glm::distance2(glm::vec2(originalVerticies[currentShapeVertexIndex]), glm::vec2(intersections[j])))
                            {
                                openIntersectionIndex = j;
                            }
                            break;
                        }
                        else if(shapeIntersectIndices[j] == currentShapeVertexIndex)
                        {
                            openIntersectionIndex = j;
                        }
                    }

                    lastOpenIntersectionIndex = openIntersectionIndex;
                    addNewVertexInfo(intersections[openIntersectionIndex], originalColours[shapeOffset], true);
                }
                //If there's a closing intersection, then add the open intersection until it reaches the close intersection
                else
                {
                    addNewVertexInfo(originalVerticies[currentShapeVertexIndex], originalColours[shapeOffset], false, originalUVs[currentShapeVertexIndex]);

                    int maskCloseIntersectionIndex = maskIntersectIndices[lastCloseIntersectionIndex];
                    int maskOpenIntersectionIndex = -1;

                    //Find the mask intersection indices first
                    int openIntersectionIndex = -1;
                    for(int j = 0; j < intersections.size(); j++)
                    {
                        //This block gets triggered when one of the points are sitting on the mask line
                        if(openIntersectionIndex != -1 && shapeIntersectIndices[j] == currentShapeVertexIndex)
                        {
                            //If the last intersection is closer to the start point of the line, use this intersection instead
                            if(glm::distance2(glm::vec2(originalVerticies[currentShapeVertexIndex]), glm::vec2(intersections[openIntersectionIndex])) <
                                glm::distance2(glm::vec2(originalVerticies[currentShapeVertexIndex]), glm::vec2(intersections[j])))
                            {
                                maskOpenIntersectionIndex = maskIntersectIndices[j];
                                openIntersectionIndex = j;
                            }
                            break;
                        }
                        else if(shapeIntersectIndices[j] == currentShapeVertexIndex)
                        {
                            maskOpenIntersectionIndex = maskIntersectIndices[j];
                            openIntersectionIndex = j;
                        }
                    }

                    //Add open intersection first
                    addNewVertexInfo(intersections[openIntersectionIndex], originalColours[shapeOffset], true);

                    //Then add all the mask vertices until reaches close intersection
                    int currentMaskIndex = GetNextIndex(0, maskVerticies.size(), maskOpenIntersectionIndex);
                    int endMaskIndex = GetNextIndex(0, maskVerticies.size(), maskCloseIntersectionIndex);
                    while (currentMaskIndex != endMaskIndex)
                    {
                        addNewVertexInfo(maskVerticies[currentMaskIndex], originalColours[shapeOffset], true);

                        currentMaskIndex = GetNextIndex(0, maskVerticies.size(), currentMaskIndex);
                    }

                    //Reset intersection index
                    lastOpenIntersectionIndex = -1;
                    lastCloseIntersectionIndex = -1;
                }

            }
            //If the line is going inside the mask from outside (Close intersection)
            else if(!IsContained(originalVerticies[currentShapeVertexIndex], maskMin, maskMax) && 
                IsContained(originalVerticies[GetNextIndex(shapeOffset, shapeVertexCount, currentShapeVertexIndex)], maskMin, maskMax))
            {
                //If the open intersection index is not recorded, then record the current index for close intersection and add the intersection
                if(lastOpenIntersectionIndex == -1)
                {
                    //Find the mask intersection index and add it
                    int closeIntersectionIndex = -1;
                    for(int j = 0; j < intersections.size(); j++)
                    {
                        //This block gets triggered when one of the points are sitting on the mask line
                        if(closeIntersectionIndex != -1 && shapeIntersectIndices[j] == currentShapeVertexIndex)
                        {
                            //If the last intersection is further to the start point of the line, use this intersection instead
                            if(glm::distance2(glm::vec2(originalVerticies[currentShapeVertexIndex]), glm::vec2(intersections[closeIntersectionIndex])) > 
                                glm::distance2(glm::vec2(originalVerticies[currentShapeVertexIndex]), glm::vec2(intersections[j])))
                            {
                                closeIntersectionIndex = j;
                            }
                            break;
                        }
                        else if(shapeIntersectIndices[j] == currentShapeVertexIndex)
                        {
                            closeIntersectionIndex = j;
                        }
                    }

                    lastCloseIntersectionIndex = closeIntersectionIndex;
                    addNewVertexInfo(intersections[closeIntersectionIndex], originalColours[shapeOffset], true);
                }
                //If there's a open intersection, then add the mask indices until it reaches the close intersection
                else
                {                                                        
                    int maskCloseIntersectionIndex = -1;
                    int maskOpenIntersectionIndex = maskIntersectIndices[lastOpenIntersectionIndex];

                    //Find the mask close intersection index first
                    int closeIntersectionIndex = -1;
                    for(int j = 0; j < intersections.size(); j++)
                    {
                        if(closeIntersectionIndex != -1 && shapeIntersectIndices[j] == currentShapeVertexIndex)
                        {
                            //If the last intersection is further to the start point of the line, use this intersection instead
                            if(glm::distance2(glm::vec2(originalVerticies[currentShapeVertexIndex]), glm::vec2(intersections[closeIntersectionIndex])) > 
                                glm::distance2(glm::vec2(originalVerticies[currentShapeVertexIndex]), glm::vec2(intersections[j])))
                            {
                                maskCloseIntersectionIndex = maskIntersectIndices[j];
                                closeIntersectionIndex = j;
                            }
                            break;
                        }
                        else if(shapeIntersectIndices[j] == currentShapeVertexIndex)
                        {
                            maskCloseIntersectionIndex = maskIntersectIndices[j];
                            closeIntersectionIndex = j;
                        }
                    }

                    //Then add all the mask vertices until reaches close intersection
                    int currentMaskIndex = GetNextIndex(0, maskVerticies.size(), maskOpenIntersectionIndex);
                    int endMaskIndex = GetNextIndex(0, maskVerticies.size(), maskCloseIntersectionIndex);
                    while (currentMaskIndex != endMaskIndex)
                    {
                        addNewVertexInfo(maskVerticies[currentMaskIndex], originalColours[shapeOffset], true);
                        currentMaskIndex = GetNextIndex(0, maskVerticies.size(), currentMaskIndex);
                    }

                    //Add close intersection last
                    addNewVertexInfo(intersections[closeIntersectionIndex], originalColours[shapeOffset], true);

                    //Reset intersection index
                    lastOpenIntersectionIndex = -1;
                    lastCloseIntersectionIndex = -1;
                }
            }
            //If the line is totally outside the mask **OR** passing through the mask
            else /*if(!IsContained(originalVerticies[currentShapeVertexIndex], maskMin, maskMax) && 
                !IsContained(originalVerticies[GetNextIndex(currentOffset, originalVerticies.size(), currentShapeVertexIndex)], maskMin, maskMax))*/
            {
                //Check if the line is intersecting at all
                int passThroughIntersectionIndices[2];
                bool intersectionFound = false;

                for(int j = 0; j < intersections.size(); j++)
                {
                    if(shapeIntersectIndices[j] == currentShapeVertexIndex)
                    {
                        if(!intersectionFound)
                        {
                            intersectionFound = true;
                            passThroughIntersectionIndices[0] = j;
                        }
                        else
                        {
                            passThroughIntersectionIndices[1] = j;
                            break;
                        }
                    }
                }

                //If the line is indeed passing through the mask, add both intersections
                if(intersectionFound)
                {
                    //If the first intersection is closer to the starting point of the line
                    if(glm::distance2(glm::vec2(originalVerticies[currentShapeVertexIndex]), glm::vec2(intersections[passThroughIntersectionIndices[0]])) < 
                        glm::distance2(glm::vec2(originalVerticies[currentShapeVertexIndex]), glm::vec2(intersections[passThroughIntersectionIndices[1]])))
                    {   
                        addNewVertexInfo(intersections[passThroughIntersectionIndices[0]], originalColours[shapeOffset], true);
                        addNewVertexInfo(intersections[passThroughIntersectionIndices[1]], originalColours[shapeOffset], true);

                        lastCloseIntersectionIndex = passThroughIntersectionIndices[0];
                        lastOpenIntersectionIndex = passThroughIntersectionIndices[1];
                    }
                    else
                    {
                        addNewVertexInfo(intersections[passThroughIntersectionIndices[1]], originalColours[shapeOffset], true);
                        addNewVertexInfo(intersections[passThroughIntersectionIndices[0]], originalColours[shapeOffset], true);

                        lastCloseIntersectionIndex = passThroughIntersectionIndices[1];
                        lastOpenIntersectionIndex = passThroughIntersectionIndices[0];
                    }
                }

                //Special Case check:
                /*
                            Mask
                            v
                        +-------+
                    +---+-------+---+
                    |   |///////|   |
                    |   |///////|   |
                    |   +-------+   |
                    |               | <-- Shape
                    +---------------+

                */
                if(currentShapeVertexIndex == shapeOffset + shapeVertexCount - 1 &&
                    currentShapeVertices.size() == 2 && lastOpenIntersectionIndex != -1 &&
                    lastCloseIntersectionIndex != -1)
                {
                    int maskCloseIntersectionIndex = maskIntersectIndices[lastCloseIntersectionIndex];
                    int maskOpenIntersectionIndex = maskIntersectIndices[lastOpenIntersectionIndex];

                    //Then add all the mask vertices until reaches close intersection
                    int currentMaskIndex = GetNextIndex(0, maskVerticies.size(), maskOpenIntersectionIndex);
                    int endMaskIndex = GetNextIndex(0, maskVerticies.size(), maskCloseIntersectionIndex);
                    while (currentMaskIndex != endMaskIndex)
                    {
                        addNewVertexInfo(maskVerticies[currentMaskIndex], originalColours[shapeOffset], true);

                        currentMaskIndex = GetNextIndex(0, maskVerticies.size(), currentMaskIndex);
                    }
                }

            }
        }

        FUNC_DEBUG_EXIT();
    }

    void Mask::SampleNewUVsAndColoursForShapes(std::vector<glm::ivec2>& originalVerticies, std::vector<glm::ivec2>& originalUVs, std::vector<glm::u8vec4>& originalColours,
                                                std::vector<int>& verticesCount, std::vector<glm::ivec2>& newVertices, std::vector<glm::ivec2>& newUVs, 
                                                std::vector<glm::u8vec4>& newColours, std::vector<bool>& changed, std::vector<int>& newVerticesCount)
    {
        //Sample the new UVs and colours
        int currentOffset = 0;
        int oldOffset = 0;

        for(int shapeIndex = 0; shapeIndex < newVerticesCount.size(); shapeIndex++)
        {
            std::vector<glm::ivec2> currentShapeVertices;

            currentShapeVertices.assign(originalVerticies.begin() + oldOffset, originalVerticies.begin() + oldOffset + verticesCount[shapeIndex]);
            
            for(int vertexIndex = currentOffset; vertexIndex < currentOffset + newVerticesCount[shapeIndex]; vertexIndex++)
            {
                if(!changed[vertexIndex])
                    continue;

                int closestIndicies[] = {0, 0, 0};

                if(!GetSampleIndicesFromShape(currentShapeVertices, closestIndicies, newVertices[vertexIndex]))
                    break;
                

                glm::ivec2 axis = currentShapeVertices[closestIndicies[1]] - currentShapeVertices[closestIndicies[0]];
                glm::ivec2 axis2 = currentShapeVertices[closestIndicies[2]] - currentShapeVertices[closestIndicies[0]];
                double axisValue = 0;
                double axis2Value = 0;

                if(!GetAxesValues(axis, axis2, newVertices[vertexIndex] - currentShapeVertices[closestIndicies[0]], axisValue, axis2Value))  
                    break;

                glm::ivec2 uvAxis = originalUVs[oldOffset + closestIndicies[1]] - originalUVs[oldOffset + closestIndicies[0]];
                glm::ivec2 uvAxis2 = originalUVs[oldOffset + closestIndicies[2]] - originalUVs[oldOffset + closestIndicies[0]];
                glm::u8vec4 colourAxis = originalColours[oldOffset + closestIndicies[1]] - originalColours[oldOffset + closestIndicies[0]];
                glm::u8vec4 colourAxis2 = originalColours[oldOffset + closestIndicies[2]] - originalColours[oldOffset + closestIndicies[0]];

                newUVs[vertexIndex] = glm::dvec2(originalUVs[oldOffset + closestIndicies[0]]) +
                                        glm::dvec2(uvAxis) * axisValue +
                                        glm::dvec2(uvAxis2) * axis2Value;

                newColours[vertexIndex] = glm::dvec4(originalColours[oldOffset + closestIndicies[0]]) +
                                            glm::dvec4(colourAxis) * axisValue +
                                            glm::dvec4(colourAxis2) * axis2Value;


            }

            currentOffset += newVerticesCount[shapeIndex];
            oldOffset += verticesCount[shapeIndex];
        }
    }



    bool Mask::LineToLineIntersection(  glm::ivec2 linePointA, glm::ivec2 linePointB, 
                                        glm::ivec2 linePointC, glm::ivec2 linePointD, glm::ivec2& intersection)
    {
        //AABB check
        //X axis
        int minX_AB;
        int maxX_AB;
        int minX_CD;
        int maxX_CD;
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
        int minY_AB;
        int maxY_AB;
        int minY_CD;
        int maxY_CD;
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

        glm::ivec2 ba = linePointB - linePointA;
        glm::ivec2 dc = linePointD - linePointC;
        glm::ivec2 ca = linePointC - linePointA;
        
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
            intersection = linePointA + glm::ivec2(ba.x * t, ba.y * t);
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

    bool Mask::CheckLinesOverlapping(int minA, int maxA, int minB, int maxB)
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
        std::cout<<"Unexpected input. minA: "<<minA<<", maxA: "<<maxA<<", minB: "<<minB<<", maxB: "<<maxB<<std::endl;
        return false;
    }

/*     int Mask::GetIndicesDistance(int count, int startIndex, int endIndex)
    {
        if(endIndex >= startIndex)
            return endIndex - startIndex;
        
        return endIndex + count - 1 - startIndex;
    } */

    int Mask::GetNextIndex( int indexOffset, int count, int currentIndex )
    {
        if(currentIndex + 1 > indexOffset + count - 1)
            return indexOffset;
        
        return ++currentIndex;
    }

    int Mask::GetPreviousIndex( int indexOffset, int count, int currentIndex )
    {
        if(currentIndex <= indexOffset)
            return indexOffset + count - 1;
        
        return --currentIndex;
    }

    bool Mask::GetSampleIndicesFromShape(std::vector<glm::ivec2>& vertices, int closestIndices[], glm::ivec2 samplePoint)
    {
        FUNC_DEBUG_ENTRY();
        
        if(vertices.size() < 3)
        {
            FUNC_DEBUG_EXIT();
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
        
        FUNC_DEBUG_EXIT();
        return true;
    }

    void Mask::AddMaskEnforcerToChildren(ssGUI::GUIObject* parent, bool includeParent)
    {
        FUNC_DEBUG_ENTRY();
        
        if(Container == nullptr)
        {
            FUNC_DEBUG_EXIT();
            return;
        }
        
        std::queue<ssGUI::GUIObject*> children;

        if(!includeParent)
        {
            Container->MoveChildrenIteratorToFirst();
            while (!Container->IsChildrenIteratorEnd())
            {
                children.push(Container->GetCurrentChild());
                Container->MoveChildrenIteratorNext();
            }
        }
        else
            children.push(parent);
        
        while (!children.empty())
        {
            ssGUI::GUIObject* child = children.front();

            if(!child->IsExtensionExist(ssGUI::Extensions::MaskEnforcer::EXTENSION_NAME))
            {
                ssGUI::Extensions::MaskEnforcer* enforcer = new MaskEnforcer();
                enforcer->BindToMaskGUIObject(Container);
                child->AddExtension(enforcer);
            }
            else
                static_cast<ssGUI::Extensions::MaskEnforcer*>(child->GetExtension(ssGUI::Extensions::MaskEnforcer::EXTENSION_NAME))->BindToMaskGUIObject(Container);

            child->MoveChildrenIteratorToFirst();
            while (!child->IsChildrenIteratorEnd())
            {
                children.push(child->GetCurrentChild());   
                child->MoveChildrenIteratorNext();
            }
            
            children.pop();
        }

        FUNC_DEBUG_EXIT();
    }

    void Mask::RemoveMaskEnforcerToChildren(ssGUI::GUIObject* parent, bool includeParent)
    {
        FUNC_DEBUG_ENTRY();

        if(Container == nullptr)
        {
            FUNC_DEBUG_EXIT();
            return;
        }
        
        std::queue<ssGUI::GUIObject*> children;
        if(!includeParent)
        {            
            Container->MoveChildrenIteratorToFirst();
            while (!Container->IsChildrenIteratorEnd())
            {
                children.push(Container->GetCurrentChild());
                Container->MoveChildrenIteratorNext();
            }
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
                
                if(enforcer->GetMaskGUIObject() == Container)
                    child->RemoveExtension(ssGUI::Extensions::MaskEnforcer::EXTENSION_NAME);
            }
            
            child->MoveChildrenIteratorToFirst();
            while (!child->IsChildrenIteratorEnd())
            {
                children.push(child->GetCurrentChild());   
                child->MoveChildrenIteratorNext();
            }

            children.pop();
        }

        FUNC_DEBUG_EXIT();
    }

    Mask::Mask(Mask const& other)
    {
        Container = nullptr;
        Enabled = other.IsEnabled();
        MaskChildren = other.GetMaskChildren();
        MaskContainer = other.GetMaskContainer();
        FollowContainer = other.GetFollowContainer();
        FollowPositionOffset = other.GetFollowPositionOffset();
        FollowSizePadding = other.GetFollowSizePadding();
        GlobalPosition = other.GetGlobalPosition();
        Size = other.GetSize();
        ChildAddedEventIndex = -1;
        ChildRemovedEventIndex = -1;
    }

    const std::string Mask::EXTENSION_NAME = "Mask";

    Mask::Mask() :  Container(nullptr), Enabled(true), MaskChildren(true), MaskContainer(false), FollowContainer(true), 
                    FollowPositionOffset(), FollowSizePadding(), GlobalPosition(), Size(),
                    ChildAddedEventIndex(-1), ChildRemovedEventIndex(-1)
    {}

    Mask::~Mask()
    {
        if(Container != nullptr)
        {
            auto eventCallbackCleanUp = [&](ssGUI::GUIObject* target, std::string eventCallbackName, int removeIndex)
            {
                target->GetEventCallback(eventCallbackName)->RemoveEventListener(removeIndex);
            
                if(target->GetEventCallback(eventCallbackName)->GetEventListenerCount() == 0)
                    target->RemoveEventCallback(eventCallbackName);
            };

            eventCallbackCleanUp(Container, ssGUI::EventCallbacks::RecursiveChildrenAddedEventCallback::EVENT_NAME, ChildAddedEventIndex);
            eventCallbackCleanUp(Container, ssGUI::EventCallbacks::RecursiveChildrenRemovedEventCallback::EVENT_NAME, ChildRemovedEventIndex);
        }
    } 

    void Mask::SetMaskChildren(bool maskChildren)
    {
        FUNC_DEBUG_ENTRY();
        
        MaskChildren = maskChildren;

        std::queue<ssGUI::GUIObject*> children;

        Container->MoveChildrenIteratorToFirst();
        while (!Container->IsChildrenIteratorEnd())
        {
            children.push(Container->GetCurrentChild());
            Container->MoveChildrenIteratorNext();
        }

        if(MaskChildren)
        {
            AddMaskEnforcerToChildren(Container, GetMaskContainer());      
            if(Container->IsEventCallbackExist(ssGUI::EventCallbacks::RecursiveChildrenAddedEventCallback::EVENT_NAME))
            {
                if(ChildAddedEventIndex == -1)
                {
                    ChildAddedEventIndex = dynamic_cast<ssGUI::EventCallbacks::RecursiveChildrenAddedEventCallback*>
                        (Container->GetEventCallback(ssGUI::EventCallbacks::RecursiveChildrenAddedEventCallback::EVENT_NAME))->
                        AddEventListener(std::bind(&ssGUI::Extensions::Mask::Internal_OnRecursiveChildAdded, this, std::placeholders::_1));
                }
            }
            else
            {
                ssGUI::EventCallbacks::RecursiveChildrenAddedEventCallback* event = new ssGUI::EventCallbacks::RecursiveChildrenAddedEventCallback();
                Container->AddEventCallback(event);

                ChildAddedEventIndex = event->AddEventListener(std::bind(&ssGUI::Extensions::Mask::Internal_OnRecursiveChildAdded, 
                                                                            this, std::placeholders::_1));
            }

            if(Container->IsEventCallbackExist(ssGUI::EventCallbacks::RecursiveChildrenRemovedEventCallback::EVENT_NAME))
            {
                if(ChildRemovedEventIndex == -1)
                {
                    ChildRemovedEventIndex = dynamic_cast<ssGUI::EventCallbacks::RecursiveChildrenRemovedEventCallback*>
                        (Container->GetEventCallback(ssGUI::EventCallbacks::RecursiveChildrenRemovedEventCallback::EVENT_NAME))->
                        AddEventListener(std::bind(&ssGUI::Extensions::Mask::Internal_OnRecursiveChildRemoved, this, std::placeholders::_1));
                }
            }
            else
            {
                ssGUI::EventCallbacks::RecursiveChildrenRemovedEventCallback* event = new ssGUI::EventCallbacks::RecursiveChildrenRemovedEventCallback();
                Container->AddEventCallback(event);

                ChildRemovedEventIndex = event->AddEventListener(std::bind(&ssGUI::Extensions::Mask::Internal_OnRecursiveChildRemoved, 
                                                                            this, std::placeholders::_1));
            }
        }
        else
        {
            RemoveMaskEnforcerToChildren(Container, GetMaskContainer());
            if(Container->IsEventCallbackExist(ssGUI::EventCallbacks::RecursiveChildrenRemovedEventCallback::EVENT_NAME))
            {
                if(ChildAddedEventIndex != -1)
                {
                    dynamic_cast<ssGUI::EventCallbacks::RecursiveChildrenAddedEventCallback*>
                        (Container->GetEventCallback(ssGUI::EventCallbacks::RecursiveChildrenAddedEventCallback::EVENT_NAME))->
                        RemoveEventListener(ChildAddedEventIndex);
                }
            }

            if(Container->IsEventCallbackExist(ssGUI::EventCallbacks::RecursiveChildrenRemovedEventCallback::EVENT_NAME))
            {
                if(ChildRemovedEventIndex == -1)
                {
                    dynamic_cast<ssGUI::EventCallbacks::RecursiveChildrenRemovedEventCallback*>
                        (Container->GetEventCallback(ssGUI::EventCallbacks::RecursiveChildrenRemovedEventCallback::EVENT_NAME))->
                        RemoveEventListener(ChildRemovedEventIndex);
                }
            }

            ChildAddedEventIndex = -1;
            ChildRemovedEventIndex = -1;
        }

        FUNC_DEBUG_EXIT();
    }

    bool Mask::GetMaskChildren() const
    {
        return MaskChildren;
    }

    void Mask::SetMaskContainer(bool maskContainer)
    {
        MaskContainer = maskContainer;

        if(maskContainer)
        {
            if(!Container->IsExtensionExist(ssGUI::Extensions::MaskEnforcer::EXTENSION_NAME))
            {
                ssGUI::Extensions::MaskEnforcer* enforcer = new MaskEnforcer();
                enforcer->BindToMaskGUIObject(Container);
                Container->AddExtension(enforcer);
            }
            else
                static_cast<ssGUI::Extensions::MaskEnforcer*>(Container->GetExtension(ssGUI::Extensions::MaskEnforcer::EXTENSION_NAME))->BindToMaskGUIObject(Container);
        }
        else
        {
            if(Container->IsExtensionExist(ssGUI::Extensions::MaskEnforcer::EXTENSION_NAME) && static_cast<ssGUI::Extensions::MaskEnforcer*>
                (Container->GetExtension(ssGUI::Extensions::MaskEnforcer::EXTENSION_NAME))->GetMaskGUIObject() == Container)
            {
                Container->RemoveExtension(ssGUI::Extensions::MaskEnforcer::EXTENSION_NAME);
            }
        }
    }

    bool Mask::GetMaskContainer() const
    {
        return MaskContainer;
    }

    void Mask::SetFollowContainer(bool followContainer)
    {
        FollowContainer = followContainer;
    }

    bool Mask::GetFollowContainer() const
    {
        return FollowContainer;
    }

    void Mask::SetFollowPositionOffset(glm::ivec2 positionOffset)
    {
        FollowPositionOffset = positionOffset;
    }

    glm::ivec2 Mask::GetFollowPositionOffset() const
    {
        return FollowPositionOffset;
    }

    void Mask::SetFollowSizePadding(glm::ivec2 sizePadding)
    {
        FollowSizePadding = sizePadding;
    }

    glm::ivec2 Mask::GetFollowSizePadding() const
    {
        return FollowSizePadding;
    }

    void Mask::SetGlobalPosition(glm::ivec2 globalPosition)
    {
        GlobalPosition = globalPosition;
    }

    glm::ivec2 Mask::GetGlobalPosition() const
    {
        return GlobalPosition;
    }

    void Mask::SetSize(glm::ivec2 size)
    {
        Size = size;
    }

    glm::ivec2 Mask::GetSize() const
    {
        return Size;
    }

    bool Mask::IsPointContainedInMask(glm::ivec2 point) const
    {
        return IsContained(point, GetGlobalPosition(), GetGlobalPosition() + GetSize());
    }

    void Mask::Internal_OnRecursiveChildAdded(ssGUI::GUIObject* child)
    {
        FUNC_DEBUG_ENTRY();
        AddMaskEnforcerToChildren(child, true);
        FUNC_DEBUG_EXIT();
    }

    void Mask::Internal_OnRecursiveChildRemoved(ssGUI::GUIObject* child)
    {
        FUNC_DEBUG_ENTRY();
        RemoveMaskEnforcerToChildren(child, true);
        FUNC_DEBUG_EXIT();
    }


    void Mask::MaskObject(ssGUI::GUIObject* obj, glm::ivec2 renderOffset)
    {
        FUNC_DEBUG_ENTRY();
        
        std::vector<glm::ivec2> maskShape;
        std::vector<glm::ivec2>& originalVerticies = obj->Extension_GetDrawingVertices();
        std::vector<bool> isOriginalVerticesInside;
        std::vector<glm::ivec2> newVertices;

        std::vector<glm::ivec2>& originalUVs = obj->Extension_GetDrawingUVs();
        std::vector<glm::ivec2> newUVs;

        std::vector<glm::u8vec4>& originalColours = obj->Extension_GetDrawingColours();
        std::vector<glm::u8vec4> newColours;

        std::vector<bool> changed;

        std::vector<int>& verticesCount = obj->Extension_GetDrawingCounts();
        std::vector<int> newVerticesCount;
        
        int currentOffset = 0;
        glm::ivec2 maskMin = GetGlobalPosition() + renderOffset;
        glm::ivec2 maskMax = maskMin + GetSize();

        maskShape.push_back(maskMin);
        maskShape.push_back(maskMin + glm::ivec2(GetSize().x, 0));
        maskShape.push_back(maskMax);
        maskShape.push_back(maskMin + glm::ivec2(0, GetSize().y));

        //Iterating each shape
        for(int shapeIndex = 0; shapeIndex < verticesCount.size(); shapeIndex++)
        {
            //Do AABB test for the shape and the mask
            glm::ivec2 shapeMin;
            glm::ivec2 shapeMax;
            if(!IsAABBOverlap(originalVerticies, currentOffset, verticesCount[shapeIndex], maskMin, maskMax, shapeMin, shapeMax))
            {
                newVerticesCount.push_back(0);
                goto nextShape;
            }
                
            { //Extra bracket to create scope for the vectors below

                //Find all the intersections (Order of the intersections are not guaranteed)
                std::vector<glm::ivec2> intersections;
                std::vector<int> shapeIntersectIndices;
                std::vector<int> maskIntersectIndices;

                GetIntersections(intersections, shapeIntersectIndices, maskIntersectIndices, originalVerticies, currentOffset, verticesCount[shapeIndex], maskShape);

                /*
                std::cout<<"\n";
                std::cout<<"\n";
                std::cout<<"Mask:\n";
                for(int j = 0; j < maskIntersectionsCount.size(); j++)
                {
                    std::cout<<maskIntersectionsCount[j]<<"\n";
                }
                std::cout<<"\n";

                std::cout<<"Shape:\n";
                for(int j = 0; j < shapeIntersectionsCount.size(); j++)
                {
                    std::cout<<shapeIntersectionsCount[j]<<"\n";
                }
                std::cout<<"\n";
                */

                std::vector<glm::ivec2> currentShapeVertices;
                std::vector<glm::ivec2> currentShapeUVs;
                std::vector<glm::u8vec4> currentShapeColours;
                std::vector<bool> currentVertexChanged;

                //If there's no intersection, check if the mask is contained in the shape instead
                if(intersections.empty() && shapeMin.x < maskMin.x && shapeMax.x > maskMax.x && shapeMin.y < maskMin.y && shapeMax.y > maskMax.y)
                {
                    //Change the shape into mask
                    currentShapeVertices.push_back(maskMin);
                    currentShapeVertices.push_back(maskMin + glm::ivec2(GetSize().x, 0));
                    currentShapeVertices.push_back(maskMin + GetSize());
                    currentShapeVertices.push_back(maskMin + glm::ivec2(0, GetSize().y));
                    currentShapeUVs.assign(4, glm::ivec2());
                    currentShapeColours.assign(4, originalColours[currentOffset]);
                    currentVertexChanged.assign(4, true);
                }
                //If there's intersection, form a new shape with intersections
                else if(!intersections.empty())
                {
                    FromNewShapeWithIntersections(currentShapeVertices, currentShapeUVs, currentShapeColours, currentVertexChanged,
                                                    originalVerticies, originalColours, originalUVs, maskMin, maskMax, currentOffset, 
                                                    verticesCount[shapeIndex], maskShape, intersections, shapeIntersectIndices,
                                                    maskIntersectIndices);
                }
                //If there's no intersection, that means the shape is inside the mask
                else
                {
                    newVertices.insert(     newVertices.end(),      originalVerticies.begin() + currentOffset,  originalVerticies.begin() + currentOffset + verticesCount[shapeIndex]);
                    newUVs.insert(          newUVs.end(),           originalUVs.begin() + currentOffset,        originalUVs.begin() + currentOffset + verticesCount[shapeIndex]);
                    newColours.insert(      newColours.end(),       originalColours.begin() + currentOffset,    originalColours.begin() + currentOffset + verticesCount[shapeIndex]);
                    changed.insert(         changed.end(),          verticesCount[shapeIndex],   false);
                    newVerticesCount.insert(newVerticesCount.end(), verticesCount[shapeIndex]);
                    goto nextShape;
                }

                //If there's intersection, append the newly formed shape
                newVertices.insert(newVertices.end(), currentShapeVertices.begin(), currentShapeVertices.end());
                newUVs.insert(newUVs.end(), currentShapeUVs.begin(), currentShapeUVs.end());
                newColours.insert(newColours.end(), currentShapeColours.begin(), currentShapeColours.end());
                changed.insert(changed.end(), currentVertexChanged.begin(), currentVertexChanged.end());
                newVerticesCount.insert(newVerticesCount.end(), currentShapeVertices.size());
            }   //Extra bracket to create scope to tell compiler the variables are not created if nextShape label is used

            nextShape:;
            //Advance to the next shape
            currentOffset += verticesCount[shapeIndex];
        }

        //Sample the new UVs and colours
        SampleNewUVsAndColoursForShapes(originalVerticies, originalUVs, originalColours, verticesCount, newVertices, newUVs, newColours, changed, newVerticesCount);

        //Copy the new vectices to the original vertices
        originalVerticies.assign(newVertices.begin(), newVertices.end());

        //Copy the new UVs to the original UVs
        originalUVs.assign(newUVs.begin(), newUVs.end());

        //Copy the new colours to the original colours
        originalColours.assign(newColours.begin(), newColours.end());

        verticesCount.assign(newVerticesCount.begin(), newVerticesCount.end());

        FUNC_DEBUG_EXIT();
    }

    void Mask::SetEnabled(bool enabled)
    {
        Enabled = enabled;
    }

    bool Mask::IsEnabled() const
    {
        return Enabled;
    }
    
    //Extension methods
    void Mask::Internal_Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();
        
        if(IsPreUpdate || Container == nullptr || !Enabled)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        if(FollowContainer)
        {
            if(Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW)
            {
                ssGUI::Window* window = static_cast<ssGUI::Window*>(Container);
                
                //Position
                SetGlobalPosition(Container->GetGlobalPosition() + 
                                    (window->HasTitlebar() ? glm::ivec2(0, window->GetTitlebarHeight()) : glm::ivec2()) +
                                    FollowPositionOffset);
                
                //Size
                SetSize(Container->GetSize() +
                        (window->HasTitlebar() ? glm::ivec2(0, -window->GetTitlebarHeight()) : glm::ivec2()) +
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

        FUNC_DEBUG_EXIT();
    }

    void Mask::Internal_Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::ivec2 mainWindowPositionOffset)
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
        SetMaskChildren(true);  //Setting it to true in order to add mask enforcer extension as well as event callbacks
    }

    void Mask::Copy(ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        
        ssGUI::Extensions::Mask* mask = static_cast<ssGUI::Extensions::Mask*>(extension);
        
        Enabled = mask->IsEnabled();
        MaskChildren = mask->GetMaskChildren();
        MaskContainer = mask->GetMaskContainer();
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

    Extension* Mask::Clone(ssGUI::GUIObject* newContainer)
    {
        Mask* temp = new Mask(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
}