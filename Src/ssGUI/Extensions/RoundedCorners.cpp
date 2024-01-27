#include "ssGUI/Extensions/RoundedCorners.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

#include <cmath>
#include <memory>

namespace ssGUI
{

namespace Extensions
{
    RoundedCorners::RoundedCorners() :  Container(nullptr),
                                        Enabled(true),
                                        RoundedCornersRadius(10),
                                        ModifiedShapes()
    {
        ModifiedShapes.AddTargetShape(ssGUI::TargetShape(0));
    }

    RoundedCorners::~RoundedCorners()
    {}
    
    RoundedCorners::RoundedCorners(RoundedCorners const& other)
    {
        Container = nullptr;
        Copy(&other);
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
        float roundRadius = RoundedCornersRadius;

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
            ssGUI_WARNING(ssGUI_EXT_TAG, "Vertices at same place or on a line");
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
            ssGUI_WARNING(ssGUI_EXT_TAG, "Container type: "<<(int)Container->GetType());
            ssGUI_WARNING(ssGUI_EXT_TAG, "anti-clockwise placements of vertices detected. Rounded corners failed.");
            invalidAngle = true;
        }
        else if(angleT1CirT2 > pi())
        {
            ssGUI_WARNING(ssGUI_EXT_TAG, "Angle between 2 tangents should not be larger than 180 degrees. Rounded corners failed.");
            invalidAngle = true;
        }

        if(invalidAngle)
        {
            ssGUI_WARNING(ssGUI_EXT_TAG, "angleT1CirT2: "<<angleT1CirT2);
            ssGUI_WARNING(ssGUI_EXT_TAG, "a: "<<a.x<<", "<<a.y);
            ssGUI_WARNING(ssGUI_EXT_TAG, "b: "<<b.x<<", "<<b.y);
            ssGUI_WARNING(ssGUI_EXT_TAG, "c: "<<c.x<<", "<<c.y);
            ssGUI_WARNING(ssGUI_EXT_TAG, "t1: "<<t1.x<<", "<<t1.y);
            ssGUI_WARNING(ssGUI_EXT_TAG, "t2: "<<t2.x<<", "<<t2.y);
            ssGUI_WARNING(ssGUI_EXT_TAG, "cir: "<<cir.x<<", "<<cir.y);
            // ssLOG_EXIT_PROGRAM();
            return;
        }

        glm::vec2 cirOriginline = glm::vec2(1, 0);
        double originLineToT1Angle = GetAngle(cirOriginline, cirTot1);
        originLineToT1Angle = originLineToT1Angle < 0 ? 2 * pi() + originLineToT1Angle : originLineToT1Angle;

        //https://stackoverflow.com/questions/15525941/find-points-on-circle
        //Using the information with tangent points, angles between them and clockwise information
        //Plot the arc
        //std::vector<glm::ivec2> arcVertices = std::vector<glm::ivec2>();
        // ssGUI_DEBUG(ssGUI_EXT_TAG, "points: "<<((int)(roundRadius * angleT1CirT2 * 1) + 2));
        int minSamples = 5;
        int sampleCount = (int)(roundRadius * angleT1CirT2 * 1) + 2;
        int finalSampleCount = sampleCount < minSamples ? minSamples : sampleCount;
        //ssGUI_DEBUG(ssGUI_EXT_TAG, "originLineToT1Angle: "<<originLineToT1Angle);
        //ssGUI_DEBUG(ssGUI_EXT_TAG, "angleT1CirT2: "<<angleT1CirT2);
        for(int i = 1; i < finalSampleCount; i++)
        {
            //ssGUI_DEBUG(ssGUI_EXT_TAG, "i: "<<i<<"/"<<finalSampleCount);
            //ssGUI_DEBUG(ssGUI_EXT_TAG, "((double)i / (double)finalSampleCount): "<<((double)i / (double)finalSampleCount));
            double currentAngle = originLineToT1Angle + angleT1CirT2 * ((double)i / (double)finalSampleCount);
            //ssGUI_DEBUG(ssGUI_EXT_TAG, "currentAngle: "<<currentAngle);
            glm::dvec2 plotPoint = glm::dvec2(cos(currentAngle), sin(currentAngle)) * (double)roundRadius;
            //ssGUI_DEBUG(ssGUI_EXT_TAG, "plotPoint: "<<plotPoint.x<<", "<<plotPoint.y);
            plottedPoints.push_back(/*glm::ivec2(round(plotPoint.x), round(plotPoint.y))*/glm::vec2(plotPoint) + cir);
        }
    }

    void RoundedCorners::GetStartEndVertexIndex(int currentIndex, int& startIndex, int& endIndex, std::vector<int> const & drawingCounts)
    {
        ssGUI_LOG_FUNC();
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
    
    void RoundedCorners::ConstructRenderInfo()
    {
        ssGUI_LOG_FUNC();
        std::vector<ssGUI::DrawingEntity> drawingEntities;
        std::vector<ssGUI::DrawingEntity>& originalEntities = Container->Extension_GetDrawingEntities();

        if(originalEntities.empty())
            return;

        //UpdateVerticesForRounding();
        ModifiedShapes.UpdateShapesToBeModified(Container->Extension_GetGUIObjectFirstShapeIndex());

        //Iterate via each vertices
        for(int i = 0; i < originalEntities.size(); i++)
        {
            std::unordered_set<int> verticesToBeRounded;

            if(!ModifiedShapes.IsModifyWholeShape())
            {
                if(ModifiedShapes.GetVerticesToBeModified(originalEntities[i].EntityName) != nullptr)
                {
                    for(int j = 0; j < ModifiedShapes.GetVerticesToBeModified(originalEntities[i].EntityName)->size(); j++)
                        verticesToBeRounded.insert(ModifiedShapes.GetVerticesToBeModified(originalEntities[i].EntityName)->at(j));
                }
                else if(ModifiedShapes.GetVerticesToBeModified(i) != nullptr)
                {
                    for(int j = 0; j < ModifiedShapes.GetVerticesToBeModified(i)->size(); j++)
                        verticesToBeRounded.insert(ModifiedShapes.GetVerticesToBeModified(i)->at(j));
                }
                else
                {
                    drawingEntities.push_back(originalEntities[i]);
                    continue;
                }
            }
            else
            {
                if(ModifiedShapes.IsShapeModified(originalEntities[i].EntityName) || ModifiedShapes.IsShapeModified(i))
                {
                    for(int j = 0; j < originalEntities[i].Vertices.size(); j++)
                        verticesToBeRounded.insert(j);
                }
                else
                {
                    drawingEntities.push_back(originalEntities[i]);
                    continue;
                }
            }
            
            drawingEntities.push_back(originalEntities[i]);
            drawingEntities[i].Colors.clear();
            drawingEntities[i].TexCoords.clear();
            drawingEntities[i].Vertices.clear();
            
            for(int j = 0; j < originalEntities[i].Vertices.size();)
            {
                int prevJ = j;
            
                if(verticesToBeRounded.find(j) == verticesToBeRounded.end())
                {
                    drawingEntities[i].Vertices.push_back(originalEntities[i].Vertices[j]);
                    if(!originalEntities[i].TexCoords.empty())
                        drawingEntities[i].TexCoords.push_back(originalEntities[i].TexCoords[j]);

                    drawingEntities[i].Colors.push_back(originalEntities[i].Colors[j]);
                    
                    j++;
                }
                else
                {
                    //For each vertices, plot and save the arc to a list
                    int currentIndex = j;
                    int prevIndex = ModifiedShapes.GetPrevIndex(j, originalEntities[i].Vertices);
                    int nextIndex = ModifiedShapes.GetNextIndex(j, originalEntities[i].Vertices);
                    int originalNewVerticesCount = drawingEntities[i].Vertices.size();
                    PlotArcPoints(  originalEntities[i].Vertices[prevIndex], 
                                    originalEntities[i].Vertices[currentIndex], 
                                    originalEntities[i].Vertices[nextIndex], 
                                    drawingEntities[i].Vertices);
                    
                    //For each arc points, sample the UV and colors
                    for(int k = originalNewVerticesCount; k < drawingEntities[i].Vertices.size(); k++)
                    {                
                        glm::vec3 coord = Barycentric(  drawingEntities[i].Vertices[k], 
                                                        originalEntities[i].Vertices[prevIndex], 
                                                        originalEntities[i].Vertices[currentIndex], 
                                                        originalEntities[i].Vertices[nextIndex]);

                        if(!originalEntities[i].TexCoords.empty())
                        {
                            drawingEntities[i].TexCoords.push_back( originalEntities[i].TexCoords[prevIndex] * coord.x + 
                                                                    originalEntities[i].TexCoords[currentIndex] * coord.y + 
                                                                    originalEntities[i].TexCoords[nextIndex] * coord.z);
                        }

                        drawingEntities[i].Colors.push_back(glm::vec4(originalEntities[i].Colors[prevIndex]) * coord.x + 
                                                            glm::vec4(originalEntities[i].Colors[currentIndex]) * coord.y + 
                                                            glm::vec4(originalEntities[i].Colors[nextIndex]) * coord.z +
                                                            0.5001f);   //0.5 for the rounding
                    }
                    
                    if(nextIndex < j)
                        break;

                    j = nextIndex;
                }
                
                assert(j > prevJ);
            }
        }
        
        originalEntities = drawingEntities;
    }

    void RoundedCorners::ConstructRenderInfo(ssGUI::Backend::DrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        ConstructRenderInfo();
    }

    const std::string RoundedCorners::EXTENSION_NAME = "Rounded Corners";

    void RoundedCorners::SetRoundedCornersRadius(float radius)
    {
        RoundedCornersRadius = radius;
        if(Container != nullptr)
            Container->RedrawObject();
    }
            
    float RoundedCorners::GetRoundedCornersRadius() const
    {
        return RoundedCornersRadius;
    }

    int RoundedCorners::AddTargetShape(ssGUI::TargetShape targetShape)
    {
        int index = ModifiedShapes.AddTargetShape(targetShape);
        
        if(Container != nullptr)
            Container->RedrawObject();
            
        return index;
    }

    ssGUI::TargetShape RoundedCorners::GetTargetShape(int location) const
    {
        return ModifiedShapes.GetTargetShape(location);
    }

    void RoundedCorners::SetTargetShape(int location, ssGUI::TargetShape targetShape)
    {
        ModifiedShapes.SetTargetShape(location, targetShape);
        
        if(Container != nullptr)
            Container->RedrawObject();
    }

    int RoundedCorners::GetTargetShapesCount() const
    {
        return ModifiedShapes.GetTargetShapesCount();
    }

    void RoundedCorners::RemoveTargetShape(int location)
    {
        ModifiedShapes.RemoveTargetShape(location);
        
        if(Container != nullptr)
            Container->RedrawObject();
    }

    void RoundedCorners::ClearTargetShapes()
    {
        ModifiedShapes.ClearTargetShapes();
    
        if(Container != nullptr)
            Container->RedrawObject();
    }

    int RoundedCorners::AddTargetVertex(ssGUI::TargetShape targetShape, int vertexIndex)
    {
        int index = ModifiedShapes.AddTargetVertex(targetShape, vertexIndex);
        
        if(Container != nullptr)
            Container->RedrawObject();
            
        return index;
    }

    VerticesIndicesForShape RoundedCorners::GetTargetVertices(int location) const
    {
        return ModifiedShapes.GetTargetVertices(location);
    }

    VerticesIndicesForShape RoundedCorners::GetTargetVertices(ssGUI::TargetShape targetShape) const
    {
        return ModifiedShapes.GetTargetVertices(targetShape);
    }

    void RoundedCorners::SetTargetVertices(ssGUI::TargetShape targetShape, const std::vector<int>& vertices)
    {
        ModifiedShapes.SetTargetVertices(targetShape, vertices);
        
        if(Container != nullptr)
            Container->RedrawObject();
    }

    void RoundedCorners::SetTargetVertices(int location, const std::vector<int>& vertices)
    {
        ModifiedShapes.SetTargetVertices(location, vertices);
        
        if(Container != nullptr)
            Container->RedrawObject();
    }

    int RoundedCorners::GetTargetVerticesCount() const
    {
        return ModifiedShapes.GetTargetVerticesCount();
    }

    void RoundedCorners::RemoveTargetVertex(int location)
    {
        ModifiedShapes.RemoveTargetVertex(location);
        
        if(Container != nullptr)
            Container->RedrawObject();
    }

    void RoundedCorners::ClearTargetVertices()
    {
        ModifiedShapes.ClearTargetVertices();
        
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

    void RoundedCorners::Internal_Update(   bool isPreUpdate, 
                                            ssGUI::Backend::SystemInputInterface* inputInterface, 
                                            ssGUI::InputStatus& currentInputStatus, 
                                            ssGUI::InputStatus& lastInputStatus, 
                                            ssGUI::GUIObject* mainWindow)
    {
        ssGUI_LOG_FUNC();
        if(!Enabled || Container == nullptr)
            return;
    }

    void RoundedCorners::Internal_Draw(bool isPreRender, ssGUI::Backend::DrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        ssGUI_LOG_FUNC();
        
        if(!Enabled || Container == nullptr || isPreRender)
            return;

        if(Container->IsRedrawNeeded())
            ConstructRenderInfo();
    }

    std::string RoundedCorners::GetExtensionName() const
    {
        return EXTENSION_NAME;
    }

    void RoundedCorners::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;
    }

    void RoundedCorners::Copy(const ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        
        auto* roundedCorners = static_cast<const ssGUI::Extensions::RoundedCorners*>(extension);
        
        Enabled = roundedCorners->IsEnabled();
        RoundedCornersRadius = roundedCorners->GetRoundedCornersRadius();
        ModifiedShapes = roundedCorners->ModifiedShapes;
    }

    ObjectsReferences* RoundedCorners::Internal_GetObjectsReferences()
    {
        return nullptr;
    }

    RoundedCorners* RoundedCorners::Clone()
    {
        RoundedCorners* temp = new RoundedCorners(*this);
        return temp;
    }
}

}