#include "ssGUI/Extensions/Outline.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

#include <cmath>
#include <memory>

namespace ssGUI
{

namespace Extensions
{
    Outline::Outline() :    Container(nullptr),
                            Enabled(true),
                            OutlineThickness(1.1),
                            SimpleOutline(false),
                            InnerOutline(true),
                            OutlineColor(0, 0, 0, 255)
                            //TargetShapes(),
                            //TargetVertices(),
                            //TargetShapesNameSet(),
                            //TargetShapesIndexSet(),
                            //TargetVerticesNameMap(),
                            //TargetVerticesIndexMap()
                            //VerticesToOutline(),
                            //VerticesToOutlinePrevVertices(),
                            //VerticesToOutlineNextVertices(),
                            //VerticesToOutlineNextNextVertices(),
                            //VerticesToOutlineShapeIndex(),
                            //VerticesToOutlineShapeStartFlag()
    {
        ssGUI::TargetShape defaultShape;
        defaultShape.ShapeIndex = 0;
        ModifiedShapes.TargetShapes.push_back(defaultShape);
        //TargetShapes.push_back(defaultShape);
    }

    Outline::~Outline()
    {}
    
    Outline::Outline(Outline const& other)
    {
        Container = nullptr;
        Enabled = other.IsEnabled();

        OutlineThickness = other.GetOutlineThickness();
        SimpleOutline = other.IsSimpleOutline();
        InnerOutline = other.IsInnerOutline();
        OutlineColor = other.GetOutlineColor();
        ModifiedShapes = other.ModifiedShapes;
        //TargetShapes = other.TargetShapes;
        //TargetVertices = other.TargetVertices;

        //TargetShapesNameSet = {};
        //TargetShapesIndexSet = {};
        //TargetVerticesNameMap = {};
        //TargetVerticesIndexMap = {};

        //VerticesToOutline = std::vector<int>();
        //VerticesToOutlinePrevVertices = std::vector<int>();
        //VerticesToOutlineNextVertices = std::vector<int>();
        //VerticesToOutlineNextNextVertices = std::vector<int>();
        //VerticesToOutlineShapeIndex = std::vector<int>();
        //VerticesToOutlineShapeStartFlag = std::vector<bool>();
    }

    void Outline::GetStartEndVertexIndex(int currentIndex, int& startIndex, int& endIndex, std::vector<int>const & drawingCounts, int& shapeIndex)
    {
        startIndex = 0;
        endIndex = 0;
        for(int i = 0; i < drawingCounts.size(); i++)
        {
            if(startIndex + drawingCounts[i] > currentIndex)
            {
                endIndex = startIndex + drawingCounts[i];
                shapeIndex = i;
                break;
            }
            
            startIndex += drawingCounts[i];
        }
    }
    
    #if 0
    void Outline::UpdateVerticesForOutline()
    {
        ssLOG_FUNC_ENTRY();
        
        //VerticesToOutline.clear();
        //VerticesToOutlinePrevVertices.clear();
        //VerticesToOutlineNextVertices.clear();
        //VerticesToOutlineNextNextVertices.clear();
        //VerticesToOutlineShapeIndex.clear();
        //VerticesToOutlineShapeStartFlag.clear();

        //TargetShapesNameSet.clear();
        //TargetShapesIndexSet.clear();
        //TargetVerticesNameMap.clear();
        //TargetVerticesIndexMap.clear();

        //std::vector<ssGUI::DrawingEntity>& drawingEntities = Container->Extension_GetDrawingEntities();


        //if(!TargetVertices.empty())
        //{
        //    for(int i = 0; i < TargetVertices.size(); i++)
        //    {
        //        if(TargetVertices[i].Shape.ShapeIndex != NO_SHAPE_INDEX)
        //        {
        //            std::vector<int>& verts = TargetVerticesIndexMap[TargetVertices[i].Shape.ShapeIndex - Container->Extension_GetGUIObjectFirstShapeIndex()];
        //            verts.insert(verts.end(), TargetVertices[i].VerticesIndices.begin(), TargetVertices[i].VerticesIndices.end());
        //        }
        //        else if(!TargetVertices[i].Shape.ShapeName.empty())
        //        {
        //            std::vector<int>& verts = TargetVerticesNameMap[TargetVertices[i].Shape.ShapeName];
        //            verts.insert(verts.end(), TargetVertices[i].VerticesIndices.begin(), TargetVertices[i].VerticesIndices.end());
        //        }
        //        else
        //            ssGUI_WARNING(ssGUI_EXT_TAG, "TargetVertices' shape is not properly set!!");
        //    }
        //}
        //else
        //{
        //    for(int i = 0; i < TargetShapes.size(); i++)
        //    {
        //        if(TargetShapes[i].ShapeIndex != NO_SHAPE_INDEX)
        //            TargetShapesIndexSet.insert(TargetShapes[i].ShapeIndex - Container->Extension_GetGUIObjectFirstShapeIndex());
        //        else if(!TargetShapes[i].ShapeName.empty())
        //            TargetShapesNameSet.insert(TargetShapes[i].ShapeName);
        //        else
        //            ssGUI_WARNING(ssGUI_EXT_TAG, "TargetShape is not properly set!!");
        //    }
        //}



        #if 0
        //int startIndex = 0;
        //int endIndex = drawingCounts[0];
        if(!TargetVertices.empty())
        {
            int shapeIndex = 0;
            for(int i = 0; i < TargetVertices.size(); i++)
            {
                int currentVertexIndex = VerticesToOutline[i] + Container->Extension_GetGUIObjectFirstVertexIndex();
                
                //Invlaid index check
                if(currentVertexIndex >= drawingVertices.size())
                {
                    VerticesToOutline.erase(VerticesToOutline.begin() + i);
                    i--;
                    continue;
                }

                if(currentVertexIndex < startIndex || currentVertexIndex >= endIndex)
                    GetStartEndVertexIndex(currentVertexIndex, startIndex, endIndex, drawingCounts, shapeIndex);

                //Shape size check
                if(endIndex - startIndex < 2)
                    continue;

                VerticesToOutline.push_back(currentVertexIndex);
                VerticesToOutlineShapeStartFlag.push_back(currentVertexIndex == startIndex);
                VerticesToOutlineShapeIndex.push_back(shapeIndex);

                int prevIndex = currentVertexIndex;
                int loopCount = 0;
                do
                {
                    prevIndex = (prevIndex == startIndex ? endIndex - 1 : prevIndex - 1);
                    loopCount++;
                    if(loopCount > endIndex - startIndex + 1)
                    {
                        ssGUI_WARNING(ssGUI_EXT_TAG, "Failed to construct outline");
                        VerticesToOutline.clear();
                        VerticesToOutlinePrevVertices.clear();
                        VerticesToOutlineNextVertices.clear();
                        return;
                    }
                }
                while(drawingVertices[prevIndex] - drawingVertices[currentVertexIndex] == glm::vec2());
                VerticesToOutlinePrevVertices.push_back(prevIndex);

                int nextIndex = currentVertexIndex;
                loopCount = 0;
                do
                {
                    nextIndex = (nextIndex == endIndex - 1 ? startIndex : nextIndex + 1);
                    loopCount++;
                    if(loopCount > endIndex - startIndex + 1)
                    {
                        ssGUI_WARNING(ssGUI_EXT_TAG, "Failed to construct outline");
                        VerticesToOutline.clear();
                        VerticesToOutlinePrevVertices.clear();
                        VerticesToOutlineNextVertices.clear();
                        return;
                    }
                }
                while(drawingVertices[nextIndex] - drawingVertices[currentVertexIndex] == glm::vec2());
                VerticesToOutlineNextVertices.push_back(nextIndex);

                int nextNextIndex = nextIndex;
                loopCount = 0;
                do
                {
                    nextNextIndex = (nextNextIndex == endIndex - 1 ? startIndex : nextNextIndex + 1);
                    loopCount++;
                    if(loopCount > endIndex - startIndex + 1)
                    {
                        ssGUI_WARNING(ssGUI_EXT_TAG, "Failed to construct outline");
                        VerticesToOutline.clear();
                        VerticesToOutlinePrevVertices.clear();
                        VerticesToOutlineNextVertices.clear();
                        return;
                    }
                }
                while(drawingVertices[nextNextIndex] - drawingVertices[nextIndex] == glm::vec2());
                VerticesToOutlineNextNextVertices.push_back(nextNextIndex);
            }
        }
        else
        {
            for(int i = 0; i < TargetShapes.size(); i++)
            {
                //Invalid index check 
                if(TargetShapes[i] + Container->Extension_GetGUIObjectFirstShapeIndex() >= drawingCounts.size())
                    continue;

                int curShape = TargetShapes[i] + Container->Extension_GetGUIObjectFirstShapeIndex();
                
                //Shape size check
                if(drawingCounts[curShape] < 3)
                    continue;
                
                int startIndex = 0;
                for(int j = 0; j < curShape; j++)
                {
                    startIndex += drawingCounts[j];
                }

                if(drawingCounts[curShape] <= 2)
                    continue;

                for(int j = startIndex; j < startIndex + drawingCounts[curShape]; j++)
                {
                    VerticesToOutline.push_back(j);
                    
                    VerticesToOutlineShapeStartFlag.push_back(j == startIndex);
                    VerticesToOutlineShapeIndex.push_back(curShape);
                    int prevIndex = j;
                    int loopCount = 0;
                    do
                    {
                        prevIndex = (prevIndex == startIndex ? startIndex + drawingCounts[curShape] - 1 : prevIndex - 1);
                        loopCount++;
                        if(loopCount > drawingCounts[curShape])
                        {
                            ssGUI_WARNING(ssGUI_EXT_TAG, "Failed to construct outline");
                            VerticesToOutline.clear();
                            VerticesToOutlinePrevVertices.clear();
                            VerticesToOutlineNextVertices.clear();
                            return;
                        }
                    }
                    while(drawingVertices[prevIndex] - drawingVertices[j] == glm::vec2());
                    VerticesToOutlinePrevVertices.push_back(prevIndex);

                    int nextIndex = j;
                    loopCount = 0;
                    do
                    {
                        nextIndex = (nextIndex == startIndex + drawingCounts[curShape] - 1 ? startIndex : nextIndex + 1);
                        loopCount++;
                        if(loopCount > drawingCounts[curShape])
                        {
                            ssGUI_WARNING(ssGUI_EXT_TAG, "Failed to construct rounded corner");
                            VerticesToOutline.clear();
                            VerticesToOutlinePrevVertices.clear();
                            VerticesToOutlineNextVertices.clear();
                            return;
                        }
                    }
                    while(drawingVertices[nextIndex] - drawingVertices[j] == glm::vec2());
                    VerticesToOutlineNextVertices.push_back(nextIndex);

                    int nextNextIndex = nextIndex;
                    loopCount = 0;
                    do
                    {
                        nextNextIndex = (nextNextIndex == startIndex + drawingCounts[curShape] - 1 ? startIndex : nextNextIndex + 1);
                        loopCount++;
                        if(loopCount > drawingCounts[curShape])
                        {
                            ssGUI_WARNING(ssGUI_EXT_TAG, "Failed to construct rounded corner");
                            VerticesToOutline.clear();
                            VerticesToOutlinePrevVertices.clear();
                            VerticesToOutlineNextVertices.clear();
                            return;
                        }
                    }
                    while(drawingVertices[nextNextIndex] - drawingVertices[nextIndex] == glm::vec2());
                    VerticesToOutlineNextNextVertices.push_back(nextNextIndex);
                }
            }
        }
        #endif
        
        ssLOG_FUNC_EXIT();
    }
    #endif

    double Outline::GetAngle(glm::vec2 a, glm::vec2 b)
    {
        glm::vec3 a3 = glm::vec3(a, 0);
        glm::vec3 b3 = glm::vec3(b, 0);

        return atan2(glm::cross(a3, b3).z, glm::dot(glm::vec2(a), glm::vec2(b)));
    }

    bool Outline::FindInnerOutlinesIntersection(glm::vec2 curVertex, glm::vec2 prevVertex, glm::vec2 nextVertex, float outlineThickness, glm::vec2& intersection)
    {
        /*
                                        │ (p) prevVertex
                                        │
 Interseection of 2 outlines            │
                  (i)   (Opposite)      │
                   ┌────────────────────┤ (b)
                   │\_                  │
                   │  \_                │
                   │    \_              │
                   │      \_            │
                   │        \_          │ (Adjacent)
                   │          \_        │
                   │            \_      │
                   │              \_    │
                   │                \_  │
                   ├─┐                \_│
           ────────┴─┴──────────────────┘(c)
nextVertex (n)    (a)                   curVertex 
                                                                                                                        ▲
            The goal of this method is to find the intersection when 2 outlines meet each other (i)                     │
            Given that we know the 3 vertices (p, c, n), and can calculate the direction of the outline (◄── i - b and  │ i - a, they are both the same distance)
            If we know the distance of bc (b-c), we can calculate where (i) is. (Since b is on pc, we know the direction of it just not the distance)
            We can use trigonometry to find out the distance of bc, if we find out the angle of ∠pci.

            So the final expression would be (Adjacent) = (Opposite) / tan(|∠pci|).
        */

        if(nextVertex - curVertex == glm::vec2() || prevVertex - curVertex == glm::vec2())
        {
            if(nextVertex - curVertex == glm::vec2() && prevVertex - curVertex == glm::vec2())
                intersection = curVertex;
            else if(nextVertex - curVertex == glm::vec2())
                intersection = glm::vec2(glm::normalize(glm::cross(glm::vec3(curVertex - prevVertex, 0), glm::vec3(0, 0, -1)))) * outlineThickness + curVertex;

            return true;
        }

        auto lineAngle = std::abs(GetAngle(prevVertex - curVertex, nextVertex - curVertex)) * 0.5;
        if(lineAngle < 0)
        {
            ssGUI_WARNING(ssGUI_EXT_TAG, "anti-clockwise placements of vertices detected. inner outline failed.");
            return false;
        }
        else if(lineAngle > pi() * 0.5)
        {
            ssGUI_WARNING(ssGUI_EXT_TAG, "Angle between 2 tangents should not be larger than 180 degrees. inner outline failed.");
            return false;
        }
        else if(pi() * 0.5 - lineAngle < 0.001)
        {
            intersection = glm::vec2(glm::normalize(glm::cross(glm::vec3(curVertex - prevVertex, 0), glm::vec3(0, 0, -1)))) * outlineThickness + curVertex;
            return true;
        }
        else if(lineAngle < 0.01)
        {
            intersection = glm::normalize((nextVertex - curVertex) + (prevVertex - curVertex)) * outlineThickness + curVertex;
            return true;
        }

        float tanAngle = tan(lineAngle);
        float adjacent = outlineThickness / tanAngle;
        intersection = glm::vec2(glm::normalize(glm::cross(glm::vec3(curVertex - prevVertex, 0), glm::vec3(0, 0, -1)))) * outlineThickness + 
            curVertex + glm::normalize(prevVertex - curVertex) * adjacent;
        
        return true;
    }

    void Outline::PlotArc(glm::vec2 start, glm::vec2 end, glm::vec2 circlePos, std::vector<glm::vec2>& plottedPoints)
    {
        ssLOG_FUNC_ENTRY();
        glm::vec2 cirOriginline = glm::vec2(1, 0);
        glm::vec2 startDir = (start - circlePos);
        glm::vec2 endDir = (end - circlePos);
        double arcRadius = glm::distance(start, circlePos);
        double startToEndAngle = GetAngle(startDir, endDir);

        bool invalidAngle = false;
        if(startToEndAngle < 0)
        {
            ssGUI_ERROR(ssGUI_EXT_TAG, "anti-clockwise placements of vertices detected. outline failed.");
            invalidAngle = true;
        }
        else if(startToEndAngle > pi())
        {
            ssGUI_ERROR(ssGUI_EXT_TAG, "Angle between 2 tangents should not be larger than 180 degrees. outline failed.");
            invalidAngle = true;
        }

        if(invalidAngle)
        {
            ssGUI_ERROR(ssGUI_EXT_TAG, "startToEndAngle: "<<startToEndAngle);
            ssGUI_ERROR(ssGUI_EXT_TAG, "start: "<<start.x<<", "<<start.y);
            ssGUI_ERROR(ssGUI_EXT_TAG, "end: "<<end.x<<", "<<end.y);
            ssGUI_ERROR(ssGUI_EXT_TAG, "circlePos: "<<circlePos.x<<", "<<circlePos.y);
            ssLOG_EXIT_PROGRAM();
            ssLOG_FUNC_EXIT();
            return;
        }

        double originLineToStartAngle = GetAngle(cirOriginline, startDir);
        originLineToStartAngle = originLineToStartAngle < 0 ? 2 * pi() + originLineToStartAngle : originLineToStartAngle;

        //https://stackoverflow.com/questions/15525941/find-points-on-circle
        //Plot the arc
        //std::vector<glm::ivec2> arcVertices = std::vector<glm::ivec2>();
        // ssGUI_WARNING(ssGUI_EXT_TAG, "points: "<<((int)(arcRadius * startToEndAngle * 1) + 2));        
        for(int i = 0; i <= (int)(arcRadius * startToEndAngle * 1) + 2; i++)
        {
            double currentAngle = originLineToStartAngle + startToEndAngle * ((double)i / (double)(arcRadius * startToEndAngle * 1));
            glm::dvec2 plotPoint = glm::dvec2(cos(currentAngle), sin(currentAngle)) * (double)arcRadius;
            plottedPoints.push_back(/*glm::ivec2(round(plotPoint.x), round(plotPoint.y))*/glm::vec2(plotPoint) + circlePos);
        }
        ssLOG_FUNC_EXIT();
    }

    void Outline::ConstructComplexOutline(bool isInner)
    {        
        ssLOG_FUNC_ENTRY();
        //Getting all the rendering details from container
        //std::vector<glm::vec2>& drawingVertices = Container->Extension_GetDrawingVertices();
        //std::vector<glm::vec2>& drawingUVs = Container->Extension_GetDrawingUVs();
        //std::vector<glm::u8vec4>& drawingColors = Container->Extension_GetDrawingColours();
        //std::vector<int>& drawingCounts = Container->Extension_GetDrawingCounts();
        std::vector<ssGUI::DrawingEntity> newEntities;

        //Copy the original vertices infos
        //std::vector<glm::vec2> originalVertices = Container->Extension_GetDrawingVertices();
        //std::vector<glm::vec2> originalUVs = Container->Extension_GetDrawingUVs();
        //std::vector<glm::u8vec4> originalColors = Container->Extension_GetDrawingColours();
        //std::vector<int> originalCounts = Container->Extension_GetDrawingCounts();
        std::vector<ssGUI::DrawingEntity>& originalEntities = Container->Extension_GetDrawingEntities();

        //Vertices infos of the outline
        //std::vector<glm::vec2> newVertices;                             //Lists of new vertices as arc
        //std::vector<int> newCounts;                                     //The number vertices per arc
        //std::unordered_map<int, std::vector<int>> outlinedShapesMap;    //(originalShapeIndex, (list of outline shapes index on newCounts))

        if(originalEntities.empty())
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        ModifiedShapes.UpdateShapesToBeModified(Container->Extension_GetGUIObjectFirstShapeIndex());

        //int lastShapeStartIndex = -1;
        
        for(int i = 0; i < originalEntities.size(); i++)
        {
            typedef std::unique_ptr<std::vector<int>, std::function<void(std::vector<int>*)>> indexVec;
            indexVec verticesToOutline = nullptr;

            if(!ModifiedShapes.IsModifyWholeShape())
            {
                if(ModifiedShapes.GetVerticesToBeModified(originalEntities[i].EntityName) != nullptr)
                    verticesToOutline = indexVec(ModifiedShapes.GetVerticesToBeModified(originalEntities[i].EntityName), [](...){});
                else if(ModifiedShapes.GetVerticesToBeModified(i) != nullptr)
                    verticesToOutline = indexVec(ModifiedShapes.GetVerticesToBeModified(i), [](...){});
                else
                    continue;
            }
            else
            {
                if(ModifiedShapes.IsShapeModified(originalEntities[i].EntityName) || ModifiedShapes.IsShapeModified(i))
                {
                    verticesToOutline = indexVec(new std::vector<int>(), [](std::vector<int>* vec){ delete vec; });
                    for(int j = 0; j < originalEntities[i].Vertices.size(); j++)
                        verticesToOutline->push_back(j);
                }
                else
                    continue;
            }
            
            for(int j = 0; j < verticesToOutline->size(); j++)
            {
                ssGUI::DrawingEntity outlineShape;
                int curVertexIndexToOutline = verticesToOutline->at(j);
                
                int nextIndex = ModifiedShapes.GetNextIndex(curVertexIndexToOutline, originalEntities.at(i).Vertices);
                int prevIndex = ModifiedShapes.GetPrevIndex(curVertexIndexToOutline, originalEntities.at(i).Vertices);
                glm::vec2 curVertex = originalEntities.at(i).Vertices.at(curVertexIndexToOutline);
                glm::vec2 prevVertex = originalEntities.at(i).Vertices.at(prevIndex);
                glm::vec2 nextVertex = originalEntities.at(i).Vertices.at(nextIndex);
                
                //For each vertex to be outlined, create a outline shape and store it in the new vertices...
                if(!isInner)
                {
                    glm::vec2 curLine = nextVertex - curVertex;
                    glm::vec2 prevLine = prevVertex - curVertex;
                    
                    if(curLine != glm::vec2())
                        curLine = glm::normalize(curLine);
                    
                    if(prevLine != glm::vec2())
                        prevLine = glm::normalize(prevLine);

                    glm::vec2 outlinePos1 = glm::normalize(glm::cross(glm::vec3(-prevLine, 0), glm::vec3(0, 0, 1))) * OutlineThickness + glm::vec3(curVertex, 0);
                    
                    //Draw first arc
                    glm::vec2 outlinePos2 = glm::normalize(glm::cross(glm::vec3(curLine, 0), glm::vec3(0, 0, 1))) * OutlineThickness + glm::vec3(curVertex, 0);

                    if(outlinePos1 != outlinePos2)
                    {
                        PlotArc(outlinePos1, outlinePos2, curVertex, outlineShape.Vertices);

                        //Outline the corner
                        outlineShape.Vertices.push_back(curVertex);
                        
                        for(int k = 0; k < outlineShape.Vertices.size(); k++)
                            outlineShape.Colors.push_back(GetOutlineColor());
                        
                        outlineShape.EntityName = OUTLINE_SHAPES_NAME;
                        newEntities.push_back(outlineShape);
                    }

                    //Link the outline to next vertex if it is present in the verticesToOutline list
                    bool linkingPossible = false;

                    if(!ModifiedShapes.IsModifyWholeShape())
                    {
                        for(int k = 0; k < verticesToOutline->size(); k++)
                        {
                            if(verticesToOutline->at(k) == nextIndex)
                            {
                                linkingPossible = true;
                                break;
                            }
                        }
                    }
                    else
                        linkingPossible = true;

                    if(linkingPossible)
                    {
                        outlineShape = ssGUI::DrawingEntity();
                    
                        outlineShape.Vertices.push_back(outlinePos2);
                        outlineShape.Vertices.push_back(outlinePos2 + (nextVertex - curVertex));
                        outlineShape.Vertices.push_back(nextVertex);
                        outlineShape.Vertices.push_back(curVertex);
                        
                        for(int k = 0; k < outlineShape.Vertices.size(); k++)
                            outlineShape.Colors.push_back(GetOutlineColor());

                        outlineShape.EntityName = OUTLINE_SHAPES_NAME;
                        newEntities.push_back(outlineShape);
                    }
                }
                else
                {
                    glm::vec2 nextNextVertex = originalEntities.at(i).Vertices.at(ModifiedShapes.GetNextIndex(nextIndex, originalEntities.at(i).Vertices));

                    glm::vec2 outlinePos1;
                    glm::vec2 outlinePos2;

                    if(!FindInnerOutlinesIntersection(curVertex, prevVertex, nextVertex, GetOutlineThickness(), outlinePos1))
                    {
                        ssLOG_FUNC_EXIT();
                        return;
                    }
                    
                    if(!FindInnerOutlinesIntersection(nextVertex, curVertex, nextNextVertex, GetOutlineThickness(), outlinePos2))
                    {
                        ssLOG_FUNC_EXIT();
                        return;
                    }

                    //Link the outline to next vertex if it is present in the verticesToOutline list
                    bool linkingPossible = false;

                    if(!ModifiedShapes.IsModifyWholeShape())
                    {
                        for(int k = 0; k < verticesToOutline->size(); k++)
                        {
                            if(verticesToOutline->at(k) == nextIndex)
                            {
                                linkingPossible = true;
                                break;
                            }
                        }
                    }
                    else
                        linkingPossible = true;

                    if(linkingPossible)
                    {
                        outlineShape.Vertices.push_back(curVertex);
                        outlineShape.Vertices.push_back(nextVertex);
                        outlineShape.Vertices.push_back(outlinePos2);
                        outlineShape.Vertices.push_back(outlinePos1);
                        
                        for(int k = 0; k < outlineShape.Vertices.size(); k++)
                            outlineShape.Colors.push_back(GetOutlineColor());

                        outlineShape.EntityName = OUTLINE_SHAPES_NAME;
                        newEntities.push_back(outlineShape);
                    }
                }
            }
        }
        
        //For each vertex to be outlined, create a outline shape and store it in the new vertices...
        //if(!isInner)
        //{
        //    for(int i = 0; i < VerticesToOutline.size(); i++)
        //    {
        //        glm::vec2 curVertex = drawingVertices[VerticesToOutline[i]];

        //        glm::vec2 nextVertex = drawingVertices[VerticesToOutlineNextVertices[i]];
        //        glm::vec2 prevVertex = drawingVertices[VerticesToOutlinePrevVertices[i]];

        //        glm::vec2 curLine = nextVertex - curVertex;
        //        glm::vec2 prevLine = prevVertex - curVertex;

        //        if(curLine != glm::vec2())
        //            curLine = glm::normalize(curLine);
                
        //        if(prevLine != glm::vec2())
        //            prevLine = glm::normalize(prevLine);

        //        glm::vec2 outlinePos1 = glm::normalize(glm::cross(glm::vec3(-prevLine, 0), glm::vec3(0, 0, 1))) * OutlineThickness + glm::vec3(curVertex, 0);
                
        //        //Draw first arc
        //        glm::vec2 outlinePos2 = glm::normalize(glm::cross(glm::vec3(curLine, 0), glm::vec3(0, 0, 1))) * OutlineThickness + glm::vec3(curVertex, 0);
        //        int originalVerticesCount = newVertices.size();

        //        if(outlinePos1 != outlinePos2)
        //        {
        //            PlotArc(outlinePos1, outlinePos2, curVertex, newVertices);

        //            //Outline the corner
        //            newVertices.push_back(curVertex);
        //            newCounts.push_back(newVertices.size() - originalVerticesCount);
        //            outlinedShapesMap[VerticesToOutlineShapeIndex[i]].push_back(newCounts.size() - 1);
        //        }

        //        //Link the outline to next outline vertex if possible
        //        bool linkingPossible = false;

        //        if(i < VerticesToOutline.size() - 1 && VerticesToOutline[i + 1] == VerticesToOutlineNextVertices[i])
        //            linkingPossible = true;
        //        else if(lastShapeStartIndex != -1 && VerticesToOutline[lastShapeStartIndex] == VerticesToOutlineNextVertices[i])
        //            linkingPossible = true;

        //        if(linkingPossible)
        //        {
        //            newVertices.push_back(outlinePos2);
        //            newVertices.push_back(outlinePos2 + (nextVertex - curVertex));
        //            newVertices.push_back(nextVertex);
        //            newVertices.push_back(curVertex);
                    
        //            newCounts.push_back(4);
        //            outlinedShapesMap[VerticesToOutlineShapeIndex[i]].push_back(newCounts.size() - 1);
        //        }

        //        lastShapeStartIndex = VerticesToOutlineShapeStartFlag[i] ? i : lastShapeStartIndex;
        //    }
        //}
        //else
        //{
        //    for(int i = 0; i < VerticesToOutline.size(); i++)
        //    {
        //        glm::vec2 curVertex = drawingVertices[VerticesToOutline[i]];

        //        glm::vec2 nextVertex = drawingVertices[VerticesToOutlineNextVertices[i]];
        //        glm::vec2 nextNextVertex = drawingVertices[VerticesToOutlineNextNextVertices[i]];
        //        glm::vec2 prevVertex = drawingVertices[VerticesToOutlinePrevVertices[i]];

        //        glm::vec2 outlinePos1;
        //        glm::vec2 outlinePos2;
                
        //        if(!FindInnerOutlinesIntersection(curVertex, prevVertex, nextVertex, GetOutlineThickness(), outlinePos1))
        //        {
        //            ssLOG_FUNC_EXIT();
        //            return;
        //        }
                
        //        if(!FindInnerOutlinesIntersection(nextVertex, curVertex, nextNextVertex, GetOutlineThickness(), outlinePos2))
        //        {
        //            ssLOG_FUNC_EXIT();
        //            return;
        //        }

        //        //Link the outline to next outline vertex if possible
        //        bool linkingPossible = false;

        //        if(i < VerticesToOutline.size() - 1 && VerticesToOutline[i + 1] == VerticesToOutlineNextVertices[i])
        //            linkingPossible = true;
        //        else if(lastShapeStartIndex != -1 && VerticesToOutline[lastShapeStartIndex] == VerticesToOutlineNextVertices[i])
        //            linkingPossible = true;

        //        if(linkingPossible)
        //        {
        //            newVertices.push_back(curVertex);
        //            newVertices.push_back(nextVertex);
        //            newVertices.push_back(outlinePos2);
        //            newVertices.push_back(outlinePos1);

        //            newCounts.push_back(4);
        //            outlinedShapesMap[VerticesToOutlineShapeIndex[i]].push_back(newCounts.size() - 1);
        //        }

        //        lastShapeStartIndex = VerticesToOutlineShapeStartFlag[i] ? i : lastShapeStartIndex;
        //    }
        //}

        //drawingVertices.clear();
        //drawingColors.clear();
        //drawingCounts.clear();
        //drawingUVs.clear();
        //drawingProperties.clear();


        originalEntities.insert(originalEntities.end(), newEntities.begin(), newEntities.end());



        //int originalShapeIndex = 0;
        //int currentDrawingCounts = 0;
        ////Merge both original vertices and new ones together (If outline is not inner)
        //for(int i = 0; i < originalVertices.size(); i++)
        //{
        //    drawingVertices.push_back(originalVertices[i]);
        //    drawingColors.push_back(originalColors[i]);
        //    drawingUVs.push_back(originalUVs[i]);

        //    currentDrawingCounts++;
        //    //Last vertex
        //    if(currentDrawingCounts >= originalCounts[originalShapeIndex])
        //    {
        //        drawingCounts.push_back(originalCounts[originalShapeIndex]);
        //        drawingProperties.push_back(originalProperties[originalShapeIndex]);

        //        //Draw outline if needed
        //        if(outlinedShapesMap.find(originalShapeIndex) != outlinedShapesMap.end() && !IsInnerOutline())
        //        {
        //            for(int newShapeIndex : outlinedShapesMap[originalShapeIndex])
        //            {
        //                int startIndex = 0;

        //                for(int j = 0; j < newShapeIndex; j++)
        //                    startIndex += newCounts[j];
                        
        //                int endIndex = startIndex + newCounts[newShapeIndex];

        //                drawingVertices.insert(drawingVertices.end(), newVertices.begin() + startIndex, newVertices.begin() + endIndex);
        //                drawingColors.insert(drawingColors.end(), newCounts[newShapeIndex], GetOutlineColor());
        //                drawingUVs.insert(drawingUVs.end(), newCounts[newShapeIndex], glm::vec2());
        //                drawingCounts.push_back(newCounts[newShapeIndex]);
        //                drawingProperties.push_back(ssGUI::DrawingProperty());
        //            }
        //        }

        //        originalShapeIndex++;
        //        currentDrawingCounts = 0;
        //    }
        //}

        //if(IsInnerOutline())
        //{
        //    drawingVertices.insert(drawingVertices.end(), newVertices.begin(), newVertices.end());

        //    for(int i = 0; i < newCounts.size(); i++)
        //    {
        //        drawingColors.insert(drawingColors.end(), newCounts[i], GetOutlineColor());
        //        drawingUVs.insert(drawingUVs.end(), newCounts[i], glm::vec2());
        //        drawingCounts.push_back(newCounts[i]);
        //        drawingProperties.push_back(ssGUI::DrawingProperty());
        //    }
        //}
        ssLOG_FUNC_EXIT();
    }

    void Outline::ConstructSimpleOutline()
    {        
        ssLOG_FUNC_ENTRY();
        //Getting all the rendering details from container
        //std::vector<glm::vec2>& drawingVertices = Container->Extension_GetDrawingVertices();
        //std::vector<glm::vec2>& drawingUVs = Container->Extension_GetDrawingUVs();
        //std::vector<glm::u8vec4>& drawingColors = Container->Extension_GetDrawingColours();
        //std::vector<int>& drawingCounts = Container->Extension_GetDrawingCounts();
        //std::vector<ssGUI::DrawingProperty>& drawingProperties = Container->Extension_GetDrawingProperties();

        //std::vector<glm::vec2> newVertices;     //Lists of new vertices as arc
        //std::vector<int> newCounts;             //The number vertices per arc

        std::vector<ssGUI::DrawingEntity> newEntities;
        std::vector<ssGUI::DrawingEntity>& originalEntities = Container->Extension_GetDrawingEntities();


        if(originalEntities.empty())
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        ModifiedShapes.UpdateShapesToBeModified(Container->Extension_GetGUIObjectFirstShapeIndex());
        for(int i = 0; i < originalEntities.size(); i++)
        {
            typedef std::unique_ptr<std::vector<int>, std::function<void(std::vector<int>*)>> indexVec;
            indexVec verticesToOutline = nullptr;

            if(!ModifiedShapes.IsModifyWholeShape())
            {
                if(ModifiedShapes.GetVerticesToBeModified(originalEntities[i].EntityName) != nullptr)
                {
                    verticesToOutline = indexVec(ModifiedShapes.GetVerticesToBeModified(originalEntities[i].EntityName), [](...){});
                }
                else if(ModifiedShapes.GetVerticesToBeModified(i) != nullptr)
                {
                    verticesToOutline = indexVec(ModifiedShapes.GetVerticesToBeModified(i), [](...){});
                }
                else
                    continue;
            }
            else
            {
                if(ModifiedShapes.IsShapeModified(originalEntities[i].EntityName) || ModifiedShapes.IsShapeModified(i))
                {
                    verticesToOutline = indexVec(new std::vector<int>(), [](std::vector<int>* vec){ delete vec; });
                    for(int j = 0; j < originalEntities[i].Vertices.size(); j++)
                        verticesToOutline->push_back(j);
                }
                else
                    continue;
            }
        
            ssGUI::DrawingEntity outlineShape;
            
            for(int j = 0; j < verticesToOutline->size(); j++)
            {
                int curVertexIndexToOutline = verticesToOutline->at(j);
                glm::vec2 curVertex = originalEntities.at(i).Vertices.at(curVertexIndexToOutline);
                glm::vec2 prevVertex = originalEntities.at(i).Vertices.at(ModifiedShapes.GetPrevIndex(curVertexIndexToOutline, originalEntities.at(i).Vertices));
                glm::vec2 nextVertex = originalEntities.at(i).Vertices.at(ModifiedShapes.GetNextIndex(curVertexIndexToOutline, originalEntities.at(i).Vertices));

                glm::vec2 curLine = nextVertex - curVertex;
                glm::vec2 prevLine = prevVertex - curVertex;

                if(curLine != glm::vec2())
                    curLine = glm::normalize(curLine);
                
                if(prevLine != glm::vec2())
                    prevLine = glm::normalize(prevLine);

                glm::vec2 outlinePos1 = glm::normalize(glm::cross(glm::vec3(-prevLine, 0), glm::vec3(0, 0, 1))) * OutlineThickness + glm::vec3(curVertex, 0);
                        
                //Draw first arc
                glm::vec2 outlinePos2 = glm::normalize(glm::cross(glm::vec3(curLine, 0), glm::vec3(0, 0, 1))) * OutlineThickness + glm::vec3(curVertex, 0);

                if(glm::distance(outlinePos1, outlinePos2) > 3)
                    PlotArc(outlinePos1, outlinePos2, curVertex, outlineShape.Vertices);
                else
                    outlineShape.Vertices.push_back(outlinePos1);
            }
            
            for(int k = 0; k < outlineShape.Vertices.size(); k++)
                outlineShape.Colors.push_back(GetOutlineColor());
            
            outlineShape.EntityName = OUTLINE_SHAPES_NAME;
            newEntities.push_back(outlineShape);
        }
        
        originalEntities.insert(originalEntities.begin() + Container->Extension_GetGUIObjectFirstShapeIndex(), newEntities.begin(), newEntities.end());
        ssLOG_FUNC_EXIT();
    }

    void Outline::ConstructRenderInfo()
    {
        if(IsInnerOutline())
            ConstructComplexOutline(true);
        else
        {
            if(SimpleOutline)
                ConstructSimpleOutline();
            else
                ConstructComplexOutline(false);
        }
        
    }

    void Outline::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        //If you need the drawingInterface or mainWindow, do it here.

        //Otherwise just call this
        ConstructRenderInfo();
    }

    //Defining the extension name
    const std::string Outline::EXTENSION_NAME = "Outline";
    const std::string Outline::OUTLINE_SHAPES_NAME = "Outline";

    void Outline::SetOutlineThickness(float thickness)
    {
        OutlineThickness = thickness;
        if(Container != nullptr)
            Container->RedrawObject();
    }

    float Outline::GetOutlineThickness() const
    {
        return OutlineThickness;
    }

    void Outline::SetInnerOutline(bool inner)
    {
        InnerOutline = inner;
        if(Container != nullptr)
            Container->RedrawObject();
    }

    bool Outline::IsInnerOutline() const
    {
        return InnerOutline;
    }

    void Outline::SetSimpleOutline(bool simpleOutline)
    {
        SimpleOutline = simpleOutline;
        if(Container != nullptr)
            Container->RedrawObject();
    }

    bool Outline::IsSimpleOutline() const
    {
        return SimpleOutline;
    }

    void Outline::SetOutlineColor(glm::u8vec4 color)
    {
        OutlineColor = color;
        if(Container != nullptr)
            Container->RedrawObject();
    }

    glm::u8vec4 Outline::GetOutlineColor() const
    {
        return OutlineColor;
    }

    int Outline::AddTargetShape(ssGUI::TargetShape targetShape)
    {
        int index = ModifiedShapes.AddTargetShape(targetShape);
        
        if(Container != nullptr)
            Container->RedrawObject();
        
        return index;
    }

    ssGUI::TargetShape Outline::GetTargetShape(int location) const
    {
        return ModifiedShapes.TargetShapes[location];
    }

    void Outline::SetTargetShape(int location, ssGUI::TargetShape targetShape)
    {
        ModifiedShapes.SetTargetShape(location, targetShape);

        if(Container != nullptr)
            Container->RedrawObject();
    }

    int Outline::GetTargetShapesCount() const
    {
        return ModifiedShapes.GetTargetShapesCount();
    }

    void Outline::RemoveTargetShape(int location)
    {
        ModifiedShapes.RemoveTargetShape(location);

        if(Container != nullptr)
            Container->RedrawObject();
    }

    void Outline::ClearTargetShapes()
    {
        ModifiedShapes.ClearTargetShapes();

        if(Container != nullptr)
            Container->RedrawObject();
    }

    int Outline::AddTargetVertex(ssGUI::TargetShape targetShape, int vertexIndex)
    {
        int index = ModifiedShapes.AddTargetVertex(targetShape, vertexIndex);
    
        if(Container != nullptr)
            Container->RedrawObject();
        
        return index;
    }

    VerticesIndicesForShape Outline::GetTargetVertices(int location) const
    {
        return ModifiedShapes.GetTargetVertices(location);
    }
    
    VerticesIndicesForShape Outline::GetTargetVertices(ssGUI::TargetShape targetShape) const
    {
        return ModifiedShapes.GetTargetVertices(targetShape);
    }
    
    void Outline::SetTargetVertices(ssGUI::TargetShape targetShape, const std::vector<int>& vertices)
    {
        ModifiedShapes.SetTargetVertices(targetShape, vertices);

        if(Container != nullptr)
            Container->RedrawObject();
    }

    void Outline::SetTargetVertices(int location, const std::vector<int>& vertices)
    {
        ModifiedShapes.SetTargetVertices(location, vertices);
        
        if(Container != nullptr)
            Container->RedrawObject();
    }

    int Outline::GetTargetVerticesCount() const
    {
        return ModifiedShapes.GetTargetVerticesCount();
    }

    void Outline::RemoveTargetVertex(int location)
    {
        ModifiedShapes.RemoveTargetVertex(location);
    
        if(Container != nullptr)
            Container->RedrawObject();
    }

    void Outline::ClearTargetVertices()
    {
        ModifiedShapes.ClearTargetVertices();

        if(Container != nullptr)
            Container->RedrawObject();
    }

    void Outline::SetEnabled(bool enabled)
    {
        Enabled = enabled;

        //Redrawing the object if this affects the rendering
        if(Container != nullptr)
            Container->RedrawObject();
    }

    bool Outline::IsEnabled() const
    {
        return Enabled;
    }

    //Extension methods
    void Outline::Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, ssGUI::GUIObject* mainWindow)
    {
        ssLOG_FUNC_ENTRY();

        if(!Enabled || Container == nullptr)
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        ssLOG_FUNC_EXIT();
    }

    void Outline::Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        ssLOG_FUNC_ENTRY();

        if(!Enabled || Container == nullptr || isPreRender)
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        if(Container->IsRedrawNeeded())
            ConstructRenderInfo();

        ssLOG_FUNC_EXIT();
    }

    std::string Outline::GetExtensionName()
    {
        return EXTENSION_NAME;
    }

    void Outline::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;
    }

    void Outline::Copy(ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;

        ssGUI::Extensions::Outline* outline = static_cast<ssGUI::Extensions::Outline*>(extension);

        Enabled = outline->IsEnabled();

        OutlineThickness = outline->GetOutlineThickness();
        SimpleOutline = outline->IsSimpleOutline();
        InnerOutline = outline->IsInnerOutline();
        OutlineColor = outline->GetOutlineColor();
        ModifiedShapes = outline->ModifiedShapes;
        //VerticesToOutline = std::vector<int>();
        //VerticesToOutlinePrevVertices = std::vector<int>();
        //VerticesToOutlineNextVertices = std::vector<int>();
        //VerticesToOutlineNextNextVertices = std::vector<int>();
        //VerticesToOutlineShapeIndex = std::vector<int>();
        //VerticesToOutlineShapeStartFlag = std::vector<bool>();
    }

    ObjectsReferences* Outline::Internal_GetObjectsReferences()
    {
        return nullptr;
    }

    Outline* Outline::Clone()
    {
        Outline* temp = new Outline(*this);
        return temp;
    }
}

}