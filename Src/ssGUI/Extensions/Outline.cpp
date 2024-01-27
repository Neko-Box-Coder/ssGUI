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
                            OutlineColor(0, 0, 0, 255),
                            ModifiedShapes()
    {
        ModifiedShapes.TargetShapes.push_back(ssGUI::TargetShape(0));
    }

    Outline::~Outline()
    {}
    
    Outline::Outline(Outline const& other)
    {
        Container = nullptr;
        Copy(&other);
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

    double Outline::GetAngle(glm::vec2 a, glm::vec2 b)
    {
        glm::vec3 a3 = glm::vec3(a, 0);
        glm::vec3 b3 = glm::vec3(b, 0);

        return atan2(glm::cross(a3, b3).z, glm::dot(glm::vec2(a), glm::vec2(b)));
    }

    bool Outline::FindInnerOutlinesIntersection(glm::vec2 curVertex, glm::vec2 prevVertex, glm::vec2 nextVertex, float outlineThickness, glm::vec2& intersection)
    {
        /*
                   │                    │ (p) prevVertex
                   │                    │
 Interseection of 2 inner outlines      │
                  (i)   (Opposite)      │
───────────────────┼ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─┤ (b)
                    \_                  │
                   │  \_                │
                        \_              │
                   │      \_            │
                            \_          │ (Adjacent)
                   │          \_        │
                                \_      │
                   │              \_    │
                                    \_  │
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
            ssGUI_WARNING(ssGUI_TAG_EXT, "anti-clockwise placements of vertices detected. inner outline failed.");
            return false;
        }
        else if(lineAngle > pi() * 0.5)
        {
            ssGUI_WARNING(ssGUI_TAG_EXT, "Angle between 2 tangents should not be larger than 180 degrees. inner outline failed.");
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
        ssGUI_LOG_FUNC();
        glm::vec2 cirOriginline = glm::vec2(1, 0);
        glm::vec2 startDir = (start - circlePos);
        glm::vec2 endDir = (end - circlePos);
        double arcRadius = glm::distance(start, circlePos);
        double startToEndAngle = GetAngle(startDir, endDir);

        bool invalidAngle = false;
        if(startToEndAngle < 0)
        {
            ssGUI_ERROR(ssGUI_TAG_EXT, "anti-clockwise placements of vertices detected. outline failed.");
            invalidAngle = true;
        }
        else if(startToEndAngle > pi())
        {
            ssGUI_ERROR(ssGUI_TAG_EXT, "Angle between 2 tangents should not be larger than 180 degrees. outline failed.");
            invalidAngle = true;
        }

        if(invalidAngle)
        {
            ssGUI_ERROR(ssGUI_TAG_EXT, "startToEndAngle: "<<startToEndAngle);
            ssGUI_ERROR(ssGUI_TAG_EXT, "start: "<<start.x<<", "<<start.y);
            ssGUI_ERROR(ssGUI_TAG_EXT, "end: "<<end.x<<", "<<end.y);
            ssGUI_ERROR(ssGUI_TAG_EXT, "circlePos: "<<circlePos.x<<", "<<circlePos.y);
            ssLOG_EXIT_PROGRAM();
            return;
        }

        double originLineToStartAngle = GetAngle(cirOriginline, startDir);
        originLineToStartAngle = originLineToStartAngle < 0 ? 2 * pi() + originLineToStartAngle : originLineToStartAngle;

        //https://stackoverflow.com/questions/15525941/find-points-on-circle
        //Plot the arc
        //std::vector<glm::ivec2> arcVertices = std::vector<glm::ivec2>();
        // ssGUI_WARNING(ssGUI_TAG_EXT, "points: "<<((int)(arcRadius * startToEndAngle * 1) + 2));        
        for(int i = 0; i <= (int)(arcRadius * startToEndAngle * 1) + 2; i++)
        {
            double currentAngle = originLineToStartAngle + startToEndAngle * ((double)i / (double)(arcRadius * startToEndAngle * 1));
            glm::dvec2 plotPoint = glm::dvec2(cos(currentAngle), sin(currentAngle)) * (double)arcRadius;
            plottedPoints.push_back(/*glm::ivec2(round(plotPoint.x), round(plotPoint.y))*/glm::vec2(plotPoint) + circlePos);
        }
    }

    void Outline::ConstructComplexOutline(bool isInner)
    {        
        ssGUI_LOG_FUNC();
        //Getting all the rendering details from container
        std::vector<ssGUI::DrawingEntity> newEntities;

        //Copy the original vertices infos
        std::vector<ssGUI::DrawingEntity>& originalEntities = Container->Extension_GetDrawingEntities();

        //Vertices infos of the outline

        if(originalEntities.empty())
            return;

        ModifiedShapes.UpdateShapesToBeModified(Container->Extension_GetGUIObjectFirstShapeIndex());

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
                        return;
                    
                    if(!FindInnerOutlinesIntersection(nextVertex, curVertex, nextNextVertex, GetOutlineThickness(), outlinePos2))
                        return;

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
        
        originalEntities.insert(originalEntities.end(), newEntities.begin(), newEntities.end());
    }

    void Outline::ConstructSimpleOutline()
    {        
        ssGUI_LOG_FUNC();
        
        //Getting all the rendering details from container
        std::vector<ssGUI::DrawingEntity> newEntities;
        std::vector<ssGUI::DrawingEntity>& originalEntities = Container->Extension_GetDrawingEntities();

        if(originalEntities.empty())
            return;

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

    void Outline::ConstructRenderInfo(ssGUI::Backend::DrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
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
    void Outline::Internal_Update(  bool isPreUpdate, 
                                    ssGUI::Backend::SystemInputInterface* inputInterface, 
                                    ssGUI::InputStatus& currentInputStatus, 
                                    ssGUI::InputStatus& lastInputStatus, 
                                    ssGUI::GUIObject* mainWindow)
    {
        ssGUI_LOG_FUNC();

        if(!Enabled || Container == nullptr)
            return;
    }

    void Outline::Internal_Draw(bool isPreRender, ssGUI::Backend::DrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        ssGUI_LOG_FUNC();

        if(!Enabled || Container == nullptr || isPreRender)
            return;

        if(Container->IsRedrawNeeded())
            ConstructRenderInfo();
    }

    std::string Outline::GetExtensionName() const
    {
        return EXTENSION_NAME;
    }

    void Outline::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;
    }

    void Outline::Copy(const ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;

        auto* outline = static_cast<const ssGUI::Extensions::Outline*>(extension);

        Enabled = outline->IsEnabled();

        OutlineThickness = outline->GetOutlineThickness();
        SimpleOutline = outline->IsSimpleOutline();
        InnerOutline = outline->IsInnerOutline();
        OutlineColor = outline->GetOutlineColor();
        ModifiedShapes = outline->ModifiedShapes;
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