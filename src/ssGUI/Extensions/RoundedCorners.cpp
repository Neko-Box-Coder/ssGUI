#include "ssGUI/Extensions/RoundedCorners.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

namespace ssGUI::Extensions
{
    RoundedCorners::RoundedCorners() : Container(nullptr), Enabled(true), RoundedCornersRadius(10), TargetShapes{0}, TargetVertices(), VerticesToRound(), 
                                        VerticesToRoundPrevVertices(), VerticesToRoundNextVertices()
    {}

    RoundedCorners::~RoundedCorners()
    {}
    
    RoundedCorners::RoundedCorners(RoundedCorners const& other)
    {
        Container = nullptr;
        Enabled = other.IsEnabled();
        RoundedCornersRadius = other.GetRoundedCornersRadius();
        TargetShapes = other.TargetShapes;
        TargetVertices = other.TargetVertices;
        VerticesToRound = other.VerticesToRound;
        VerticesToRoundPrevVertices = other.VerticesToRoundPrevVertices;
        VerticesToRoundNextVertices = other.VerticesToRoundNextVertices;
    }

    double RoundedCorners::GetAngle(glm::vec2 a, glm::vec2 b)
    {
        glm::vec3 a3 = glm::vec3(a, 0);
        glm::vec3 b3 = glm::vec3(b, 0);

        return atan2(glm::cross(a3, b3).z, glm::dot(glm::vec2(a), glm::vec2(b)));
    }

    glm::vec3 RoundedCorners::Barycentric(glm::vec2 samplePoint, glm::vec2 a, glm::vec2 b, glm::vec2 c)
    {
        glm::vec2 v0 = b - a;
        glm::vec2 v1 = c - a;
        glm::vec2 v2 = samplePoint - a;

        float d00 = glm::dot(v0, v0);
        float d01 = glm::dot(v0, v1);
        float d11 = glm::dot(v1, v1);
        float d20 = glm::dot(v2, v0);
        float d21 = glm::dot(v2, v1);
        float denom = d00 * d11 - d01 * d01;

        //All points are the same or not valid triangle
        if(denom == 0)
            return glm::vec3(0, 0, 0);
        
        float v = (d11 * d20 - d01 * d21) / denom;
        float w = (d00 * d21 - d01 * d20) / denom;
        float u = 1.0 - v - w;
        return glm::vec3(u, v, w);
    }


    void RoundedCorners::PlotArcPoints(glm::vec2 a, glm::vec2 b, glm::vec2 c, std::vector<glm::vec2>& plottedPoints)
    {
        //Let A be previous vertices and B be current and C be next
        int roundRadius = RoundedCornersRadius;

        //Check if the shortest side is lower than the rounded corner radius.
        //If so, change the radius to that
        double halfABLength = round(glm::distance(a, b) * 0.5);    
        double halfBCLength = round(glm::distance(c, b) * 0.5);

        if(halfABLength < roundRadius)
            roundRadius = halfABLength;

        if(halfBCLength < roundRadius)
            roundRadius = halfBCLength;

        glm::vec2 ba = a-b;
        glm::vec2 bc = c-b;
        glm::vec2 nba = glm::normalize(ba);
        glm::vec2 nbc = glm::normalize(bc);

        //Vertices are either at the same place or on a line
        if(nba + nbc == glm::vec2())
        {
            DEBUG_LINE("Vertices at same place or on a line");
            return;
        }

        glm::vec2 midVec = glm::normalize(nba + nbc);

        //Find the angle from ba to bc (https://stackoverflow.com/questions/21483999/using-atan2-to-find-angle-between-two-vectors/21486462#21486462)
        //This is in radians
        double angleABC = GetAngle(ba, bc);
        angleABC = angleABC < 0 ? angleABC * -1 : angleABC;

        //Find the rounded corner circle location, let that be CIR
        //https://math.stackexchange.com/questions/797828/calculate-center-of-circle-tangent-to-two-lines-in-space
        glm::dvec2 cird = glm::dvec2(b) + glm::dvec2(midVec) * (roundRadius / sin(angleABC * 0.5)); 
        glm::vec2 cir = glm::vec2(cird);

        //let that be t1(on ab) and t2(on bc)
        //Find the tangent points by using cross porduct to find the perpendicular line on ab and ac 
        //and add it to the circle origin by radius to find t1 and t2.
        glm::vec2 t1 = glm::normalize(glm::cross(glm::vec3(ba, 0), glm::vec3(0, 0, -1))) * (float)roundRadius + glm::vec3(cir, 0);
        glm::vec2 t2 = glm::normalize(glm::cross(glm::vec3(bc, 0), glm::vec3(0, 0, 1))) * (float)roundRadius + glm::vec3(cir, 0);

        //Find the angle between CIR --> t1 to CIR --> t2
        glm::vec2 cirTot1 = t1 - cir;
        glm::vec2 cirTot2 = t2 - cir;

        double angleT1CirT2 = GetAngle(cirTot1, cirTot2);

        bool invalidAngle = false;
        if(angleT1CirT2 < 0)
        {
            DEBUG_LINE("anti-clockwise placements of vertices detected. Rounded corners failed.");
            invalidAngle = true;
        }
        else if(angleT1CirT2 > pi())
        {
            DEBUG_LINE("Angle between 2 tangents should not be larger than 180 degrees. Rounded corners failed.");
            invalidAngle = true;
        }

        if(invalidAngle)
        {
            DEBUG_LINE("angleT1CirT2: "<<angleT1CirT2);
            DEBUG_LINE("a: "<<a.x<<", "<<a.y);
            DEBUG_LINE("b: "<<b.x<<", "<<b.y);
            DEBUG_LINE("c: "<<c.x<<", "<<c.y);
            DEBUG_LINE("t1: "<<t1.x<<", "<<t1.y);
            DEBUG_LINE("t2: "<<t2.x<<", "<<t2.y);
            DEBUG_LINE("cir: "<<cir.x<<", "<<cir.y);
            DEBUG_EXIT_PROGRAM();
            return;
        }

        glm::vec2 cirOriginline = glm::vec2(1, 0);
        double originLineToT1Angle = GetAngle(cirOriginline, cirTot1);
        originLineToT1Angle = originLineToT1Angle < 0 ? 2 * pi() + originLineToT1Angle : originLineToT1Angle;

        //https://stackoverflow.com/questions/15525941/find-points-on-circle
        //Using the information with tangent points, angles between them and clockwise information
        //Plot the arc
        //std::vector<glm::ivec2> arcVertices = std::vector<glm::ivec2>();
        // DEBUG_LINE("points: "<<((int)(roundRadius * angleT1CirT2 * 1) + 2));
        int minSamples = 10;
        int sampleCount = (int)(roundRadius * angleT1CirT2 * 1) + 2;
        int finalSampleCount = sampleCount < 10 ? minSamples : sampleCount;
        for(int i = 1; i < finalSampleCount; i++)
        {
            double currentAngle = originLineToT1Angle + angleT1CirT2 * ((double)i / (double)finalSampleCount);
            glm::dvec2 plotPoint = glm::dvec2(cos(currentAngle), sin(currentAngle)) * (double)roundRadius;
            plottedPoints.push_back(/*glm::ivec2(round(plotPoint.x), round(plotPoint.y))*/glm::vec2(plotPoint) + cir);
        }
    }

    void RoundedCorners::GetStartEndVertexIndex(int currentIndex, int& startIndex, int& endIndex, std::vector<int> const & drawingCounts)
    {
        startIndex = 0;
        endIndex = 0;
        for(int i = 0; i < drawingCounts.size(); i++)
        {
            if(startIndex + drawingCounts[i] > currentIndex)
            {
                endIndex = startIndex + drawingCounts[i];
                break;
            }
            
            startIndex += drawingCounts[i];
        }
    }

    void RoundedCorners::UpdateVerticesForRounding()
    {
        FUNC_DEBUG_ENTRY();
        VerticesToRound.clear();
        VerticesToRoundPrevVertices.clear();
        VerticesToRoundNextVertices.clear();

        std::vector<glm::vec2>& drawingVertices = Container->Extension_GetDrawingVertices();
        std::vector<int>& drawingCounts = Container->Extension_GetDrawingCounts();

        int startIndex = 0;
        int endIndex = drawingCounts[0];
        if(!TargetVertices.empty())
        {
            for(int i = 0; i < TargetVertices.size(); i++)
            {
                int currentVertexIndex = TargetVertices[i] + Container->Extension_GetGUIObjectFirstVertexIndex();

                //Invlaid index check
                if(currentVertexIndex >= drawingVertices.size())
                {
                    VerticesToRound.erase(VerticesToRound.begin() + i);
                    i--;
                    continue;
                }

                if(currentVertexIndex < startIndex || currentVertexIndex >= endIndex)
                    GetStartEndVertexIndex(currentVertexIndex, startIndex, endIndex, drawingCounts);

                //Shape size check
                if(endIndex - startIndex < 2)
                    continue;

                VerticesToRound.push_back(currentVertexIndex);
                int prevIndex = currentVertexIndex;
                int loopCount = 0;
                do
                {
                    prevIndex = (prevIndex == startIndex ? endIndex - 1 : prevIndex - 1);
                    loopCount++;
                    if(loopCount > endIndex - startIndex + 1)
                    {
                        DEBUG_LINE("Failed to construct rounded corner");
                        VerticesToRound.clear();
                        VerticesToRoundPrevVertices.clear();
                        VerticesToRoundNextVertices.clear();
                        return;
                    }
                }
                while(drawingVertices[prevIndex] - drawingVertices[currentVertexIndex] == glm::vec2());
                VerticesToRoundPrevVertices.push_back(prevIndex);

                int nextIndex = currentVertexIndex;
                loopCount = 0;
                do
                {
                    nextIndex = (nextIndex == endIndex - 1 ? startIndex : nextIndex + 1);
                    loopCount++;
                    if(loopCount > endIndex - startIndex + 1)
                    {
                        DEBUG_LINE("Failed to construct rounded corner");
                        VerticesToRound.clear();
                        VerticesToRoundPrevVertices.clear();
                        VerticesToRoundNextVertices.clear();
                        return;
                    }
                }
                while(drawingVertices[nextIndex] - drawingVertices[currentVertexIndex] == glm::vec2());
                VerticesToRoundNextVertices.push_back(nextIndex);
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
                for(int j = 0; j < TargetShapes[i] + Container->Extension_GetGUIObjectFirstShapeIndex(); j++)
                {
                    startIndex += drawingCounts[j];
                }

                if(drawingCounts[curShape] <= 2)
                    continue;

                for(int j = startIndex; j < startIndex + drawingCounts[curShape]; j++)
                {
                    VerticesToRound.push_back(j);

                    int prevIndex = j;
                    int loopCount = 0;
                    do
                    {
                        prevIndex = (prevIndex == startIndex ? startIndex + drawingCounts[curShape] - 1 : prevIndex - 1);
                        loopCount++;
                        if(loopCount > drawingCounts[curShape])
                        {
                            DEBUG_LINE("Failed to construct rounded corner");
                            VerticesToRound.clear();
                            VerticesToRoundPrevVertices.clear();
                            VerticesToRoundNextVertices.clear();
                            return;
                        }
                    }
                    while(drawingVertices[prevIndex] - drawingVertices[j] == glm::vec2());
                    VerticesToRoundPrevVertices.push_back(prevIndex);

                    int nextIndex = j;
                    loopCount = 0;
                    do
                    {
                        nextIndex = (nextIndex == startIndex + drawingCounts[curShape] - 1 ? startIndex : nextIndex + 1);
                        loopCount++;
                        if(loopCount > drawingCounts[curShape])
                        {
                            DEBUG_LINE("Failed to construct rounded corner");
                            VerticesToRound.clear();
                            VerticesToRoundPrevVertices.clear();
                            VerticesToRoundNextVertices.clear();
                            return;
                        }
                    }
                    while(drawingVertices[nextIndex] - drawingVertices[j] == glm::vec2());
                    VerticesToRoundNextVertices.push_back(nextIndex);
                }
            }
        }
        FUNC_DEBUG_EXIT();
    }

    void RoundedCorners::ConstructRenderInfo()
    {
        FUNC_DEBUG_ENTRY();
        std::vector<glm::vec2>& drawingVertices = Container->Extension_GetDrawingVertices();
        std::vector<glm::vec2>& drawingUVs = Container->Extension_GetDrawingUVs();
        std::vector<glm::u8vec4>& drawingColors = Container->Extension_GetDrawingColours();
        std::vector<int>& drawingCounts = Container->Extension_GetDrawingCounts();
        std::vector<ssGUI::DrawingProperty>& drawingProperties = Container->Extension_GetDrawingProperties();

        std::vector<glm::vec2> originalVertices = drawingVertices;
        std::vector<glm::vec2> originalUVs = drawingUVs;
        std::vector<glm::u8vec4> originalColors = drawingColors;
        std::vector<int> originalCounts = drawingCounts;
        std::vector<ssGUI::DrawingProperty> originalProperties = drawingProperties;

        std::vector<glm::vec2> newVertices;    //Lists of new vertices as arc
        std::vector<glm::vec2> newUVs;         //Associated UVs
        std::vector<glm::u8vec4> newColors;     //Associated colors
        std::vector<int> newCounts;             //The number vertices per arc

        if(drawingCounts.empty())
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        UpdateVerticesForRounding();

        //Iterate via each vertices
        for(int i = 0; i < VerticesToRound.size(); i++)
        {
            //For each vertices, plot and save the arc to a list
            int currentIndex = VerticesToRound[i];
            int prevIndex = VerticesToRoundPrevVertices[i];
            int nextIndex = VerticesToRoundNextVertices[i];

            int prevNewVerticesCount = newVertices.size();
            PlotArcPoints(drawingVertices[prevIndex], drawingVertices[currentIndex], drawingVertices[nextIndex], newVertices);
            newCounts.push_back(newVertices.size() - prevNewVerticesCount);

            //For each arc points, sample the UV and colors
            for(int j = prevNewVerticesCount; j < newVertices.size(); j++)
            {                
                glm::vec3 coord = Barycentric(newVertices[j], drawingVertices[prevIndex], drawingVertices[currentIndex], drawingVertices[nextIndex]);

                newUVs.push_back((drawingUVs[prevIndex]) * coord.x + 
                                (drawingUVs[currentIndex]) * coord.y + 
                                (drawingUVs[nextIndex]) * coord.z);

                newColors.push_back(glm::vec4(drawingColors[prevIndex]) * coord.x + 
                                    glm::vec4(drawingColors[currentIndex]) * coord.y + 
                                    glm::vec4(drawingColors[nextIndex]) * coord.z);
            }
        }

        drawingVertices.clear();
        drawingUVs.clear();
        drawingColors.clear();
        drawingCounts.clear();
        drawingProperties.clear();
        
        //Hashmap for holding the original vertex index as key and index in newVertices as value
        std::map<int, int> verticesToRoundMap;
        for(int i = 0; i < VerticesToRound.size(); i++)
            verticesToRoundMap[VerticesToRound[i]] = i;
        
        //Reconstruct vertices, uvs, etc.
        int shapeIndex = 0;
        int shapeStartIndex = 0;
        int shapeEndIndex = originalCounts[0];
        int currentDrawingCounts = 0;
        for(int i = 0; i < originalVertices.size(); i++)
        {
            //Updates current shape index
            if(i >= shapeEndIndex)
            {
                shapeStartIndex += originalCounts[shapeIndex];
                shapeIndex++;
                shapeEndIndex += originalCounts[shapeIndex];
                currentDrawingCounts = 0;
            }
            
            //If adding original vertices
            if(verticesToRoundMap.find(i) == verticesToRoundMap.end())
            {
                drawingVertices.push_back(originalVertices[i]);
                drawingUVs.push_back(originalUVs[i]);
                drawingColors.push_back(originalColors[i]);
                currentDrawingCounts++;      
            }
            //If adding arc vertices
            else
            {
                int arcIndex = verticesToRoundMap[i];
                int startIndex = 0;
                for(int j = 0; j < arcIndex; j++)
                    startIndex += newCounts[j];
                
                int endIndex = startIndex + newCounts[arcIndex];

                for(int j = startIndex; j < endIndex; j++)
                {
                    drawingVertices.push_back(newVertices[j]);
                    drawingUVs.push_back(newUVs[j]);
                    drawingColors.push_back(newColors[j]);
                }

                currentDrawingCounts += newCounts[arcIndex];
            }

            //Updates drawing counts
            if(i+1 >= shapeEndIndex)
            {
                drawingCounts.push_back(currentDrawingCounts);
                drawingProperties.push_back(originalProperties[shapeIndex]);
            }
        }
        FUNC_DEBUG_EXIT();
    }

    void RoundedCorners::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        ConstructRenderInfo();
    }

    const std::string RoundedCorners::EXTENSION_NAME = "Rounded Corners";

    void RoundedCorners::SetRoundedCornersRadius(int radius)
    {
        RoundedCornersRadius = radius;
        if(Container != nullptr)
            Container->RedrawObject();
    }
            
    int RoundedCorners::GetRoundedCornersRadius() const
    {
        return RoundedCornersRadius;
    }

    void RoundedCorners::AddTargetShape(int shapeIndex)
    {
        TargetShapes.push_back(shapeIndex);
        if(Container != nullptr)
            Container->RedrawObject();
    }

    int RoundedCorners::GetTargetShape(int location) const
    {
        return TargetShapes[location];
    }

    void RoundedCorners::SetTargetShape(int location, int shapeIndex)
    {
        TargetShapes[location] = shapeIndex;
        if(Container != nullptr)
            Container->RedrawObject();
    }

    int RoundedCorners::GetTargetShapesCount() const
    {
        return TargetShapes.size();
    }

    void RoundedCorners::RemoveTargetShape(int location)
    {
        TargetShapes.erase(TargetShapes.begin() + location);
        if(Container != nullptr)
            Container->RedrawObject();
    }

    void RoundedCorners::ClearTargetShapes()
    {
        TargetShapes.clear();
        if(Container != nullptr)
            Container->RedrawObject();
    }

    void RoundedCorners::AddTargetVertex(int vertexIndex)
    {
        TargetVertices.push_back(vertexIndex);
        if(Container != nullptr)
            Container->RedrawObject();
    }

    int RoundedCorners::GetTargetVertex(int location) const
    {
        return TargetVertices[location];
    }

    void RoundedCorners::SetTargetVertex(int location, int vertexIndex)
    {
        TargetVertices[location] = vertexIndex;
        if(Container != nullptr)
            Container->RedrawObject();
    }

    int RoundedCorners::GetTargetVerticesCount() const
    {
        return TargetVertices.size();
    }

    void RoundedCorners::RemoveTargetVertex(int location)
    {
        TargetVertices.erase(TargetVertices.begin() + location);
        if(Container != nullptr)
            Container->RedrawObject();
    }

    void RoundedCorners::ClearTargetVertices()
    {
        TargetVertices.clear();
        if(Container != nullptr)
            Container->RedrawObject();
    }

    void RoundedCorners::SetEnabled(bool enabled)
    {
        Enabled = enabled;
        Container->RedrawObject();
    }

    bool RoundedCorners::IsEnabled() const
    {
        return Enabled;
    }

    void RoundedCorners::Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();

        if(!Enabled || Container == nullptr)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        FUNC_DEBUG_EXIT();
    }

    void RoundedCorners::Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        FUNC_DEBUG_ENTRY();
        
        if(!Enabled || Container == nullptr || isPreRender)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        if(Container->IsRedrawNeeded())
            ConstructRenderInfo();
        
        FUNC_DEBUG_EXIT();
    }

    std::string RoundedCorners::GetExtensionName()
    {
        return EXTENSION_NAME;
    }

    void RoundedCorners::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;
    }

    void RoundedCorners::Copy(ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        
        ssGUI::Extensions::RoundedCorners* roundedCorners = static_cast<ssGUI::Extensions::RoundedCorners*>(extension);
        
        Enabled = roundedCorners->IsEnabled();
        RoundedCornersRadius = roundedCorners->GetRoundedCornersRadius();
        TargetShapes = roundedCorners->TargetShapes;
        TargetVertices = roundedCorners->TargetVertices;
        VerticesToRound = roundedCorners->VerticesToRound;
        VerticesToRoundPrevVertices = roundedCorners->VerticesToRoundPrevVertices;
        VerticesToRoundNextVertices = roundedCorners->VerticesToRoundNextVertices;
    }

    ObjectsReferences* RoundedCorners::Internal_GetObjectsReferences()
    {
        return nullptr;
    }

    RoundedCorners* RoundedCorners::Clone(ssGUI::GUIObject* newContainer)
    {
        RoundedCorners* temp = new RoundedCorners(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
}