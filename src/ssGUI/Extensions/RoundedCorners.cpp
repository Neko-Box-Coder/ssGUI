#include "ssGUI/Extensions/RoundedCorners.hpp"

#include "ssGUI/BaseClasses/MainWindow.hpp" //For getting mouse position

namespace ssGUI::Extensions
{
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


    double RoundedCorners::GetAngle(glm::ivec2 a, glm::ivec2 b)
    {
        glm::vec3 a3 = glm::vec3(a, 1);
        glm::vec3 b3 = glm::vec3(b, 1);

        return atan2(glm::cross(a3, b3).z, glm::dot(glm::vec2(a), glm::vec2(b)));
    }

    glm::dvec3 RoundedCorners::Barycentric(glm::ivec2 samplePoint, glm::ivec2 a, glm::ivec2 b, glm::ivec2 c)
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
            return glm::dvec3(0, 0, 0);
        
        double v = (double)(d11 * d20 - d01 * d21) / (double)denom;
        double w = (double)(d00 * d21 - d01 * d20) / (double)denom;
        double u = 1.0 - v - w;
        return glm::dvec3(u, v, w);
    }


    void RoundedCorners::PlotArcPoints(glm::ivec2 a, glm::ivec2 b, glm::ivec2 c, std::vector<glm::ivec2>& plottedPoints)
    {
        //Let A be previous vertices and B be current and C be next
        int roundRadius = RoundedCornersRadius;

        //Check if the shortest side is lower than the rounded corner radius.
        //If so, change the radius to that
        double halfABLength = round(glm::distance(glm::vec2(a),glm::vec2(b))*0.5);    
        double halfBCLength = round(glm::distance(glm::vec2(c),glm::vec2(b))*0.5);

        if(halfABLength < roundRadius)
            roundRadius = halfABLength;

        if(halfBCLength < roundRadius)
            roundRadius = halfBCLength;

        glm::vec2 ba = a-b;
        glm::vec2 bc = c-b;
        glm::vec2 midVec = glm::normalize(glm::normalize(ba) + glm::normalize(bc));

        //Find the angle from ba to bc (https://stackoverflow.com/questions/21483999/using-atan2-to-find-angle-between-two-vectors/21486462#21486462)
        //This is in radians
        double angleABC = GetAngle(ba, bc);
        angleABC = angleABC < 0 ? angleABC * -1 : angleABC;

        //Find the rounded corner circle location, let that be CIR
        //https://math.stackexchange.com/questions/797828/calculate-center-of-circle-tangent-to-two-lines-in-space
        glm::dvec2 cird = glm::dvec2(b) + glm::dvec2(midVec) * (roundRadius / sin(angleABC * 0.5)); 
        glm::ivec2 cir = glm::ivec2(round(cird.x), round(cird.y));

        //let that be t1(on ab) and t2(on bc)
        //Find the tangent points by using cross porduct to find the perpendicular line on ab and ac 
        //and add it to the circle origin by radius to find t1 and t2.
        glm::ivec2 t1 = glm::normalize(glm::cross(glm::vec3(ba, 0), glm::vec3(0, 0, -1))) * (float)roundRadius + glm::vec3(cir, 0);
        glm::ivec2 t2 = glm::normalize(glm::cross(glm::vec3(bc, 0), glm::vec3(0, 0, 1))) * (float)roundRadius + glm::vec3(cir, 0);

        //Find the angle between CIR --> t1 to CIR --> t2
        glm::ivec2 cirTot1 = t1 - cir;
        glm::ivec2 cirTot2 = t2 - cir;

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
            DEBUG_LINE("a: "<<a.x<<", "<<a.y);
            DEBUG_LINE("b: "<<b.x<<", "<<b.y);
            DEBUG_LINE("c: "<<c.x<<", "<<c.y);
            DEBUG_LINE("t1: "<<t1.x<<", "<<t1.y);
            DEBUG_LINE("t2: "<<t2.x<<", "<<t2.y);
            DEBUG_EXIT_PROGRAM();
        }

        glm::ivec2 cirOriginline = glm::ivec2(1, 0);
        double originLineToT1Angle = GetAngle(cirOriginline, cirTot1);
        originLineToT1Angle = originLineToT1Angle < 0 ? 2 * pi() + originLineToT1Angle : originLineToT1Angle;

        //https://stackoverflow.com/questions/15525941/find-points-on-circle
        //Using the information with tangent points, angles between them and clockwise information
        //Plot the arc
        //std::vector<glm::ivec2> arcVertices = std::vector<glm::ivec2>();
        for(int i = 0; i < (int)(roundRadius * 1.5) + 2; i++)
        {
            double currentAngle = originLineToT1Angle + angleT1CirT2 * ((double)i / (double)((int)(roundRadius * 1.5) + 1));
            glm::dvec2 plotPoint = glm::dvec2(cos(currentAngle), sin(currentAngle)) * (double)roundRadius;
            plottedPoints.push_back(glm::ivec2(round(plotPoint.x), round(plotPoint.y)) + cir);
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
        VerticesToRound.clear();
        VerticesToRoundPrevVertices.clear();
        VerticesToRoundNextVertices.clear();

        std::vector<glm::ivec2>& drawingVertices = Container->Extension_GetDrawingVertices();
        std::vector<int>& drawingCounts = Container->Extension_GetDrawingCounts();

        int startIndex = 0;
        int endIndex = drawingCounts[0];
        if(!TargetVertices.empty())
        {
            VerticesToRound = TargetVertices;
            for(int i = 0; i < VerticesToRound.size(); i++)
            {
                //Invlaid index check
                if(VerticesToRound[i] >= drawingVertices.size())
                {
                    VerticesToRound.erase(VerticesToRound.begin() + i);
                    i--;
                    continue;
                }

                if(VerticesToRound[i] < startIndex || VerticesToRound[i] >= endIndex)
                    GetStartEndVertexIndex(VerticesToRound[i], startIndex, endIndex, drawingCounts);

                VerticesToRoundPrevVertices.push_back((VerticesToRound[i] == startIndex ? endIndex - 1 : VerticesToRound[i] - 1));
                VerticesToRoundNextVertices.push_back((VerticesToRound[i] == endIndex - 1 ? startIndex : VerticesToRound[i] + 1));
            }
        }
        else
        {
            for(int i = 0; i < TargetShapes.size(); i++)
            {
                //Invalid index check 
                if(TargetShapes[i] >= drawingCounts.size())
                    continue;

                int startIndex = 0;
                for(int j = 0; j < TargetShapes[i]; j++)
                {
                    startIndex += drawingCounts[j];
                }

                for(int j = startIndex; j < startIndex + drawingCounts[TargetShapes[i]]; j++)
                {
                    VerticesToRound.push_back(j);
                    VerticesToRoundPrevVertices.push_back
                        (j == startIndex ? startIndex + drawingCounts[TargetShapes[i]] - 1 : j - 1);
                    VerticesToRoundNextVertices.push_back
                        (j == startIndex + drawingCounts[TargetShapes[i]] - 1 ? startIndex : j + 1);
                }
            }
        }
    }

    void RoundedCorners::ConstructRenderInfo()
    {
        std::vector<glm::ivec2>& drawingVertices = Container->Extension_GetDrawingVertices();
        std::vector<glm::ivec2>& drawingUVs = Container->Extension_GetDrawingUVs();
        std::vector<glm::u8vec4>& drawingColors = Container->Extension_GetDrawingColours();
        std::vector<int>& drawingCounts = Container->Extension_GetDrawingCounts();
        std::vector<ssGUI::DrawingProperty>& drawingProperties = Container->Extension_GetDrawingProperties();

        std::vector<glm::ivec2> originalVertices = drawingVertices;
        std::vector<glm::ivec2> originalUVs = drawingUVs;
        std::vector<glm::u8vec4> originalColors = drawingColors;
        std::vector<int> originalCounts = drawingCounts;
        std::vector<ssGUI::DrawingProperty> originalProperties = drawingProperties;

        std::vector<glm::ivec2> newVertices;    //Lists of new vertices as arc
        std::vector<glm::ivec2> newUVs;         //Associated UVs
        std::vector<glm::u8vec4> newColors;     //Associated colors
        std::vector<int> newCounts;             //The number vertices per arc

        if(drawingCounts.empty())
            return;

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
                glm::dvec3 coord = Barycentric(newVertices[j], drawingVertices[prevIndex], drawingVertices[currentIndex], drawingVertices[nextIndex]);

                newUVs.push_back(glm::dvec2(drawingUVs[prevIndex]) * coord.x + 
                                glm::dvec2(drawingUVs[currentIndex]) * coord.y + 
                                glm::dvec2(drawingUVs[nextIndex]) * coord.z);

                newColors.push_back(glm::dvec4(drawingColors[prevIndex]) * coord.x + 
                                    glm::dvec4(drawingColors[currentIndex]) * coord.y + 
                                    glm::dvec4(drawingColors[nextIndex]) * coord.z);
            }
        }

        DEBUG_LINE();
        DEBUG_LINE();

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
    }

    void RoundedCorners::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {
        ConstructRenderInfo();
    }

    const std::string RoundedCorners::EXTENSION_NAME = "Rounded Corners";

    RoundedCorners::RoundedCorners() : Container(nullptr), Enabled(true), RoundedCornersRadius(20), TargetShapes{0}, TargetVertices(), VerticesToRound(), 
                                        VerticesToRoundPrevVertices(), VerticesToRoundNextVertices()
    {}

    RoundedCorners::~RoundedCorners()
    {}

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

    void RoundedCorners::Internal_Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();

        if(!Enabled || Container == nullptr)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        FUNC_DEBUG_EXIT();
    }

    void RoundedCorners::Internal_Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {
        FUNC_DEBUG_ENTRY();
        
        if(!Enabled || Container == nullptr || IsPreRender)
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

    Extension* RoundedCorners::Clone(ssGUI::GUIObject* newContainer)
    {
        RoundedCorners* temp = new RoundedCorners(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
}