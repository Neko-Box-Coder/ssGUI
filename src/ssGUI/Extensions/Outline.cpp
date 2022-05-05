#include "ssGUI/Extensions/Outline.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

//Remember to add the .cpp file to the cmake list if you are using cmake

namespace ssGUI::Extensions
{
    Outline::Outline() : Container(nullptr), Enabled(true), OutlineThickness(1.1), SimpleOutline(false), InnerOutline(true), 
                            OutlineColor(glm::u8vec4(0, 0, 0, 255)), TargetShapes{0}, TargetVertices(), VerticesToOutline(), 
                            VerticesToOutlinePrevVertices(), VerticesToOutlineNextVertices(), VerticesToOutlineNextNextVertices(), 
                            VerticesToOutlineShapeIndex(), VerticesToOutlineShapeStartFlag()
    {}

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
        TargetShapes = other.TargetShapes;
        TargetVertices = other.TargetVertices;

        VerticesToOutline = std::vector<int>();
        VerticesToOutlinePrevVertices = std::vector<int>();
        VerticesToOutlineNextVertices = std::vector<int>();
        VerticesToOutlineNextNextVertices = std::vector<int>();
        VerticesToOutlineShapeIndex = std::vector<int>();
        VerticesToOutlineShapeStartFlag = std::vector<bool>();
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

    void Outline::UpdateVerticesForOutline()
    {
        FUNC_DEBUG_ENTRY();
        VerticesToOutline.clear();
        VerticesToOutlinePrevVertices.clear();
        VerticesToOutlineNextVertices.clear();
        VerticesToOutlineNextNextVertices.clear();
        VerticesToOutlineShapeIndex.clear();
        VerticesToOutlineShapeStartFlag.clear();

        std::vector<glm::vec2>& drawingVertices = Container->Extension_GetDrawingVertices();
        std::vector<int>& drawingCounts = Container->Extension_GetDrawingCounts();

        int startIndex = 0;
        int endIndex = drawingCounts[0];
        if(!TargetVertices.empty())
        {
            VerticesToOutline = TargetVertices;
            int shapeIndex = 0;
            for(int i = 0; i < VerticesToOutline.size(); i++)
            {
                VerticesToOutline[i] += Container->Extension_GetGUIObjectFirstVertexIndex();
                int currentVertexIndex = VerticesToOutline[i];
                
                //Invlaid index check
                if(currentVertexIndex >= drawingVertices.size())
                {
                    VerticesToOutline.erase(VerticesToOutline.begin() + i);
                    i--;
                    continue;
                }

                if(currentVertexIndex < startIndex || currentVertexIndex >= endIndex)
                    GetStartEndVertexIndex(currentVertexIndex, startIndex, endIndex, drawingCounts, shapeIndex);

                VerticesToOutlineShapeStartFlag.push_back(currentVertexIndex == startIndex);
                VerticesToOutlineShapeIndex.push_back(shapeIndex);

                int prevIndex = currentVertexIndex;
                do
                {
                    prevIndex = (prevIndex == startIndex ? endIndex - 1 : prevIndex - 1);   
                }
                while(drawingVertices[prevIndex] - drawingVertices[currentVertexIndex] == glm::vec2());
                VerticesToOutlinePrevVertices.push_back(prevIndex);

                int nextIndex = currentVertexIndex;
                do
                {
                    nextIndex = (nextIndex == endIndex - 1 ? startIndex : nextIndex + 1);
                }
                while(drawingVertices[nextIndex] - drawingVertices[currentVertexIndex] == glm::vec2());
                VerticesToOutlineNextVertices.push_back(nextIndex);

                int nextNextIndex = nextIndex;
                do
                {
                    nextNextIndex = (nextNextIndex == endIndex - 1 ? startIndex : nextNextIndex + 1);
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

                int startIndex = 0;
                int curShape = TargetShapes[i] + Container->Extension_GetGUIObjectFirstShapeIndex();
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
                    do
                    {
                        prevIndex = (prevIndex == startIndex ? startIndex + drawingCounts[curShape] - 1 : prevIndex - 1);
                    }
                    while(drawingVertices[prevIndex] - drawingVertices[j] == glm::vec2());
                    VerticesToOutlinePrevVertices.push_back(prevIndex);

                    int nextIndex = j;
                    do
                    {
                        nextIndex = (nextIndex == startIndex + drawingCounts[curShape] - 1 ? startIndex : nextIndex + 1);
                    }
                    while(drawingVertices[nextIndex] - drawingVertices[j] == glm::vec2());
                    VerticesToOutlineNextVertices.push_back(nextIndex);

                    int nextNextIndex = nextIndex;
                    do
                    {
                        nextNextIndex = (nextNextIndex == startIndex + drawingCounts[curShape] - 1 ? startIndex : nextNextIndex + 1);
                    }
                    while(drawingVertices[nextNextIndex] - drawingVertices[nextIndex] == glm::vec2());
                    VerticesToOutlineNextNextVertices.push_back(nextNextIndex);
                }
            }
        }
        FUNC_DEBUG_EXIT();
    }

    double Outline::GetAngle(glm::vec2 a, glm::vec2 b)
    {
        glm::vec3 a3 = glm::vec3(a, 0);
        glm::vec3 b3 = glm::vec3(b, 0);

        return atan2(glm::cross(a3, b3).z, glm::dot(glm::vec2(a), glm::vec2(b)));
    }

    void Outline::PlotArc(glm::vec2 start, glm::vec2 end, glm::vec2 circlePos, std::vector<glm::vec2>& plottedPoints)
    {
        FUNC_DEBUG_ENTRY();
        glm::vec2 cirOriginline = glm::vec2(1, 0);
        glm::vec2 startDir = (start - circlePos);
        glm::vec2 endDir = (end - circlePos);
        double arcRadius = glm::distance(start, circlePos);
        double startToEndAngle = GetAngle(startDir, endDir);

        bool invalidAngle = false;
        if(startToEndAngle < 0)
        {
            DEBUG_LINE("anti-clockwise placements of vertices detected. outline failed.");
            invalidAngle = true;
        }
        else if(startToEndAngle > pi())
        {
            DEBUG_LINE("Angle between 2 tangents should not be larger than 180 degrees. outline failed.");
            invalidAngle = true;
        }

        if(invalidAngle)
        {
            DEBUG_LINE("startToEndAngle: "<<startToEndAngle);
            DEBUG_LINE("start: "<<start.x<<", "<<start.y);
            DEBUG_LINE("end: "<<end.x<<", "<<end.y);
            DEBUG_LINE("circlePos: "<<circlePos.x<<", "<<circlePos.y);
            DEBUG_EXIT_PROGRAM();
            FUNC_DEBUG_EXIT();
            return;
        }

        double originLineToStartAngle = GetAngle(cirOriginline, startDir);
        originLineToStartAngle = originLineToStartAngle < 0 ? 2 * pi() + originLineToStartAngle : originLineToStartAngle;

        //https://stackoverflow.com/questions/15525941/find-points-on-circle
        //Plot the arc
        //std::vector<glm::ivec2> arcVertices = std::vector<glm::ivec2>();
        // DEBUG_LINE("points: "<<((int)(arcRadius * startToEndAngle * 1) + 2));        
        for(int i = 0; i < (int)(arcRadius * startToEndAngle * 1) + 2; i++)
        {
            double currentAngle = originLineToStartAngle + startToEndAngle * ((double)i / (double)((int)(arcRadius * startToEndAngle * 1) + 1));
            glm::dvec2 plotPoint = glm::dvec2(cos(currentAngle), sin(currentAngle)) * (double)arcRadius;
            plottedPoints.push_back(/*glm::ivec2(round(plotPoint.x), round(plotPoint.y))*/glm::vec2(plotPoint) + circlePos);
        }
        FUNC_DEBUG_EXIT();
    }

    //TODO : add option for inner outline
    void Outline::ConstructComplexOutline(bool isInner)
    {        
        FUNC_DEBUG_ENTRY();
        //Getting all the rendering details from container
        std::vector<glm::vec2>& drawingVertices = Container->Extension_GetDrawingVertices();
        std::vector<glm::vec2>& drawingUVs = Container->Extension_GetDrawingUVs();
        std::vector<glm::u8vec4>& drawingColors = Container->Extension_GetDrawingColours();
        std::vector<int>& drawingCounts = Container->Extension_GetDrawingCounts();
        std::vector<ssGUI::DrawingProperty>& drawingProperties = Container->Extension_GetDrawingProperties();

        //Copy the original vertices infos
        std::vector<glm::vec2> originalVertices = Container->Extension_GetDrawingVertices();
        std::vector<glm::vec2> originalUVs = Container->Extension_GetDrawingUVs();
        std::vector<glm::u8vec4> originalColors = Container->Extension_GetDrawingColours();
        std::vector<int> originalCounts = Container->Extension_GetDrawingCounts();
        std::vector<ssGUI::DrawingProperty> originalProperties = Container->Extension_GetDrawingProperties();

        //Vertices infos of the outline
        std::vector<glm::vec2> newVertices;                             //Lists of new vertices as arc
        std::vector<int> newCounts;                                     //The number vertices per arc
        std::unordered_map<int, std::vector<int>> outlinedShapesMap;    //(originalShapeIndex, (list of outline shapes index on newCounts))

        if(drawingCounts.empty())
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        UpdateVerticesForOutline();

        int lastShapeStartIndex = -1;
        
        //For each vertex to be outlined, create a outline shape and store it in the new vertices...
        if(!isInner)
        {
            for(int i = 0; i < VerticesToOutline.size(); i++)
            {
                glm::vec2 curVertex = drawingVertices[VerticesToOutline[i]];

                glm::vec2 nextVertex = drawingVertices[VerticesToOutlineNextVertices[i]];
                glm::vec2 prevVertex = drawingVertices[VerticesToOutlinePrevVertices[i]];

                glm::vec2 curLine = nextVertex - curVertex;
                glm::vec2 prevLine = prevVertex - curVertex;

                if(curLine != glm::vec2())
                    curLine = glm::normalize(curLine);
                
                if(prevLine != glm::vec2())
                    prevLine = glm::normalize(prevLine);

                glm::vec2 outlinePos1 = glm::normalize(glm::cross(glm::vec3(-prevLine, 0), glm::vec3(0, 0, 1))) * OutlineThickness + glm::vec3(curVertex, 0);
                
                //Draw first arc
                glm::vec2 outlinePos2 = glm::normalize(glm::cross(glm::vec3(curLine, 0), glm::vec3(0, 0, 1))) * OutlineThickness + glm::vec3(curVertex, 0);
                int originalVerticesCount = newVertices.size();

                if(outlinePos1 != outlinePos2)
                {
                    PlotArc(outlinePos1, outlinePos2, curVertex, newVertices);

                    //Outline the corner
                    newVertices.push_back(curVertex);
                    newCounts.push_back(newVertices.size() - originalVerticesCount);
                    outlinedShapesMap[VerticesToOutlineShapeIndex[i]].push_back(newCounts.size() - 1);
                }

                //Link the outline to next outline vertex if possible
                bool linkingPossible = false;

                if(i < VerticesToOutline.size() - 1 && VerticesToOutline[i + 1] == VerticesToOutlineNextVertices[i])
                    linkingPossible = true;
                else if(lastShapeStartIndex != -1 && VerticesToOutline[lastShapeStartIndex] == VerticesToOutlineNextVertices[i])
                    linkingPossible = true;

                if(linkingPossible)
                {
                    newVertices.push_back(outlinePos2);
                    newVertices.push_back(outlinePos2 + (nextVertex - curVertex));
                    newVertices.push_back(nextVertex);
                    newVertices.push_back(curVertex);
                    
                    newCounts.push_back(4);
                    outlinedShapesMap[VerticesToOutlineShapeIndex[i]].push_back(newCounts.size() - 1);
                }

                lastShapeStartIndex = VerticesToOutlineShapeStartFlag[i] ? i : lastShapeStartIndex;
            }
        }
        else
        {
            for(int i = 0; i < VerticesToOutline.size(); i++)
            {
                glm::vec2 curVertex = drawingVertices[VerticesToOutline[i]];

                glm::vec2 nextVertex = drawingVertices[VerticesToOutlineNextVertices[i]];
                glm::vec2 nextNextVertex = drawingVertices[VerticesToOutlineNextNextVertices[i]];
                glm::vec2 prevVertex = drawingVertices[VerticesToOutlinePrevVertices[i]];

                glm::vec2 curLine = nextVertex - curVertex;
                glm::vec2 nextLine = nextNextVertex - nextVertex;
                glm::vec2 prevLine = prevVertex - curVertex;

                if(curLine != glm::vec2())
                    curLine = glm::normalize(curLine);
                
                if(prevLine != glm::vec2())
                    prevLine = glm::normalize(prevLine);

                if(nextLine != glm::vec2())
                    nextLine = glm::normalize(nextLine);

                glm::vec2 outlinePos1 = glm::normalize(curLine + prevLine) * OutlineThickness + curVertex;
                
                glm::vec2 outlinePos2 = glm::normalize(-curLine + nextLine) * OutlineThickness + nextVertex;

                //Link the outline to next outline vertex if possible
                bool linkingPossible = false;

                if(i < VerticesToOutline.size() - 1 && VerticesToOutline[i + 1] == VerticesToOutlineNextVertices[i])
                    linkingPossible = true;
                else if(lastShapeStartIndex != -1 && VerticesToOutline[lastShapeStartIndex] == VerticesToOutlineNextVertices[i])
                    linkingPossible = true;

                if(linkingPossible)
                {
                    newVertices.push_back(curVertex);
                    newVertices.push_back(nextVertex);
                    newVertices.push_back(outlinePos2);
                    newVertices.push_back(outlinePos1);

                    newCounts.push_back(4);
                    outlinedShapesMap[VerticesToOutlineShapeIndex[i]].push_back(newCounts.size() - 1);
                }

                lastShapeStartIndex = VerticesToOutlineShapeStartFlag[i] ? i : lastShapeStartIndex;
            }
        }

        drawingVertices.clear();
        drawingColors.clear();
        drawingCounts.clear();
        drawingUVs.clear();
        drawingProperties.clear();

        int originalShapeIndex = 0;
        int currentDrawingCounts = 0;
        //Merge both original vertices and new ones together (If outline is not inner)
        for(int i = 0; i < originalVertices.size(); i++)
        {
            drawingVertices.push_back(originalVertices[i]);
            drawingColors.push_back(originalColors[i]);
            drawingUVs.push_back(originalUVs[i]);

            currentDrawingCounts++;
            //Last vertex
            if(currentDrawingCounts >= originalCounts[originalShapeIndex])
            {
                drawingCounts.push_back(originalCounts[originalShapeIndex]);
                drawingProperties.push_back(originalProperties[originalShapeIndex]);

                //Draw outline if needed
                if(outlinedShapesMap.find(originalShapeIndex) != outlinedShapesMap.end() && !IsInnerOutline())
                {
                    for(int newShapeIndex : outlinedShapesMap[originalShapeIndex])
                    {
                        int startIndex = 0;

                        for(int j = 0; j < newShapeIndex; j++)
                            startIndex += newCounts[j];
                        
                        int endIndex = startIndex + newCounts[newShapeIndex];

                        drawingVertices.insert(drawingVertices.end(), newVertices.begin() + startIndex, newVertices.begin() + endIndex);
                        drawingColors.insert(drawingColors.end(), newCounts[newShapeIndex], GetOutlineColor());
                        drawingUVs.insert(drawingUVs.end(), newCounts[newShapeIndex], glm::vec2());
                        drawingCounts.push_back(newCounts[newShapeIndex]);
                        drawingProperties.push_back(ssGUI::DrawingProperty());
                    }
                }

                originalShapeIndex++;
                currentDrawingCounts = 0;
            }
        }

        if(IsInnerOutline())
        {
            drawingVertices.insert(drawingVertices.end(), newVertices.begin(), newVertices.end());

            for(int i = 0; i < newCounts.size(); i++)
            {
                drawingColors.insert(drawingColors.end(), newCounts[i], GetOutlineColor());
                drawingUVs.insert(drawingUVs.end(), newCounts[i], glm::vec2());
                drawingCounts.push_back(newCounts[i]);
                drawingProperties.push_back(ssGUI::DrawingProperty());
            }
        }
        FUNC_DEBUG_EXIT();
    }

    void Outline::ConstructSimpleOutline()
    {        
        FUNC_DEBUG_ENTRY();
        //Getting all the rendering details from container
        std::vector<glm::vec2>& drawingVertices = Container->Extension_GetDrawingVertices();
        std::vector<glm::vec2>& drawingUVs = Container->Extension_GetDrawingUVs();
        std::vector<glm::u8vec4>& drawingColors = Container->Extension_GetDrawingColours();
        std::vector<int>& drawingCounts = Container->Extension_GetDrawingCounts();
        std::vector<ssGUI::DrawingProperty>& drawingProperties = Container->Extension_GetDrawingProperties();

        std::vector<glm::vec2> newVertices;     //Lists of new vertices as arc
        std::vector<int> newCounts;             //The number vertices per arc

        if(drawingCounts.empty())
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        UpdateVerticesForOutline();
        for(int i = 0; i < VerticesToOutline.size(); i++)
        {
            glm::vec2 curVertex = drawingVertices[VerticesToOutline[i]];
            glm::vec2 prevVertex = drawingVertices[VerticesToOutlinePrevVertices[i]];
            glm::vec2 nextVertex = drawingVertices[VerticesToOutlineNextVertices[i]];

            glm::vec2 curLine = nextVertex - curVertex;
            glm::vec2 prevLine = prevVertex - curVertex;

            if(curLine != glm::vec2())
                curLine = glm::normalize(curLine);
            
            if(prevLine != glm::vec2())
                prevLine = glm::normalize(prevLine);

            glm::vec2 outlinePos1 = glm::normalize(glm::cross(glm::vec3(-prevLine, 0), glm::vec3(0, 0, 1))) * OutlineThickness + glm::vec3(curVertex, 0);
                    
            //Draw first arc
            glm::vec2 outlinePos2 = glm::normalize(glm::cross(glm::vec3(curLine, 0), glm::vec3(0, 0, 1))) * OutlineThickness + glm::vec3(curVertex, 0);
            int originalVerticesCount = newVertices.size();

            if(outlinePos1 != outlinePos2)
                PlotArc(outlinePos1, outlinePos2, curVertex, newVertices);
            else
                newVertices.push_back(outlinePos1);
        }
        
        newCounts.push_back(newVertices.size());

        drawingVertices.insert(drawingVertices.begin() + Container->Extension_GetGUIObjectFirstVertexIndex(), newVertices.begin(), newVertices.end());
        drawingCounts.insert(drawingCounts.begin() + Container->Extension_GetGUIObjectFirstShapeIndex(), newCounts.begin(), newCounts.end());

        drawingColors.insert(drawingColors.begin() + Container->Extension_GetGUIObjectFirstVertexIndex(), drawingVertices.size() - drawingColors.size(), GetOutlineColor());
        drawingUVs.insert(drawingUVs.begin() + Container->Extension_GetGUIObjectFirstVertexIndex(), drawingVertices.size() - drawingUVs.size(), glm::vec2());
        drawingProperties.insert(drawingProperties.begin() + Container->Extension_GetGUIObjectFirstShapeIndex(), drawingCounts.size() - drawingProperties.size(), ssGUI::DrawingProperty());
        FUNC_DEBUG_EXIT();
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

    void Outline::AddTargetShape(int shapeIndex)
    {
        TargetShapes.push_back(shapeIndex);
        if(Container != nullptr)
            Container->RedrawObject();
    }

    int Outline::GetTargetShape(int location) const
    {
        return TargetShapes[location];
    }

    void Outline::SetTargetShape(int location, int shapeIndex)
    {
        TargetShapes[location] = shapeIndex;
        if(Container != nullptr)
            Container->RedrawObject();
    }

    int Outline::GetTargetShapesCount() const
    {
        return TargetShapes.size();
    }

    void Outline::RemoveTargetShape(int location)
    {
        TargetShapes.erase(TargetShapes.begin() + location);
        if(Container != nullptr)
            Container->RedrawObject();
    }

    void Outline::ClearTargetShapes()
    {
        TargetShapes.clear();
        if(Container != nullptr)
            Container->RedrawObject();
    }

    void Outline::AddTargetVertex(int vertexIndex)
    {
        TargetVertices.push_back(vertexIndex);
        if(Container != nullptr)
            Container->RedrawObject();
    }

    int Outline::GetTargetVertex(int location) const
    {
        return TargetVertices[location];
    }

    void Outline::SetTargetVertex(int location, int vertexIndex)
    {
        TargetVertices[location] = vertexIndex;
        if(Container != nullptr)
            Container->RedrawObject();
    }

    int Outline::GetTargetVerticesCount() const
    {
        return TargetVertices.size();
    }

    void Outline::RemoveTargetVertex(int location)
    {
        TargetVertices.erase(TargetVertices.begin() + location);
        if(Container != nullptr)
            Container->RedrawObject();
    }

    void Outline::ClearTargetVertices()
    {
        TargetVertices.clear();
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
    void Outline::Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();

        if(!Enabled || Container == nullptr)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        FUNC_DEBUG_EXIT();
    }

    void Outline::Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        FUNC_DEBUG_ENTRY();

        if(!Enabled || Container == nullptr || isPreRender)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        if(!SimpleOutline)
        {
            if(Container->IsRedrawNeeded())
                ConstructRenderInfo();
        }
        else
        {
            if(Container->IsRedrawNeeded())
                ConstructRenderInfo();
        }

        FUNC_DEBUG_EXIT();
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
        TargetShapes = outline->TargetShapes;
        TargetVertices = outline->TargetVertices;

        VerticesToOutline = std::vector<int>();
        VerticesToOutlinePrevVertices = std::vector<int>();
        VerticesToOutlineNextVertices = std::vector<int>();
        VerticesToOutlineNextNextVertices = std::vector<int>();
        VerticesToOutlineShapeIndex = std::vector<int>();
        VerticesToOutlineShapeStartFlag = std::vector<bool>();
    }

    ObjectsReferences* Outline::Internal_GetObjectsReferences()
    {
        return nullptr;
    }

    Outline* Outline::Clone(ssGUI::GUIObject* newContainer)
    {
        Outline* temp = new Outline(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
}