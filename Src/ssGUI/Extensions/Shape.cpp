#include "ssGUI/Extensions/Shape.hpp" //Or the location of your custom extension header

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position
#include "ssLogger/ssLog.hpp"

#include <cmath>

namespace ssGUI
{

namespace Extensions
{
    Shape::Shape() :    Container(nullptr),
                        Enabled(true),
                        ExtensionPreRender(true),
                        AdditionalShapes(),
                        GUIObjectShapesToRemove(),
                        NextID(0)
    {}

    Shape::~Shape()
    {}
    
    Shape::Shape(Shape const& other)
    {
        Container = nullptr;
        Enabled = other.IsEnabled();

        ExtensionPreRender = other.ExtensionPreRender;
        AdditionalShapes = other.AdditionalShapes;
        GUIObjectShapesToRemove = other.GUIObjectShapesToRemove;
        NextID = other.NextID;
    }

    void Shape::ConstructAdditionalPolygon(AdditionalShape& targetShape, std::vector<glm::vec2>const & vertices, std::vector<glm::u8vec4>const & colors, bool behindGUIObject)
    {
        targetShape.Vertices = vertices;
        targetShape.Colors = colors;
        targetShape.BehindGUI = behindGUIObject;
        targetShape.Type = ShapeType::POLYGON;
    }

    void Shape::ConstructAdditionalRectangle(AdditionalShape& targetShape, glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject)
    {
        targetShape.Vertices.clear();
        targetShape.Colors.clear();

        targetShape.Vertices.push_back(pos);
        targetShape.Vertices.push_back(pos + glm::vec2(size.x, 0));
        targetShape.Vertices.push_back(pos + size);
        targetShape.Vertices.push_back(pos + glm::vec2(0, size.y));

        targetShape.Colors.push_back(color);
        targetShape.Colors.push_back(color);
        targetShape.Colors.push_back(color);
        targetShape.Colors.push_back(color);

        targetShape.Type = ShapeType::RECTANGLE;
        targetShape.Data.Rectangle.Pos = pos;
        targetShape.Data.Rectangle.Size = size;
        targetShape.Data.Rectangle.Color = color;     

        targetShape.BehindGUI = behindGUIObject;
    }

    void Shape::ConstructAdditionalCircle(AdditionalShape& targetShape, glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject)
    {
        size *= 0.5;

        targetShape.Vertices.clear();
        targetShape.Colors.clear();

        for(int i = 0; i < (int)(glm::length(size) * 2.f * pi()); i++)
        {
            float angle = 2.f * pi() * ((float)i / (float)((int)(glm::length(size) * 2.f * pi())));
            targetShape.Vertices.push_back(pos + glm::vec2(size.x * cos(angle), size.y * sin(angle)) + size);
            targetShape.Colors.push_back(color);
        }

        targetShape.Type = ShapeType::CIRCLE;
        targetShape.Data.Circle.Pos = pos;        
        targetShape.Data.Circle.Size = size;
        targetShape.Data.Circle.Color = color;

        targetShape.BehindGUI = behindGUIObject;
    }

    void Shape::ConstructAdditionalLine(AdditionalShape& targetShape, glm::vec2 start, glm::vec2 end, float startThickness, float endThickness, glm::u8vec4 startColor, glm::u8vec4 endColor, bool behindGUIObject)
    {
        /*
          a ┌─────────────┐ b
            │             │
            │             │
            │       start │
            │      │      │
            │      │      │
            │      │      │
            │      │      │
            │      │      │
            │      │      │
            │      │ width│
            │      │◄────►│
            │      │      │
            │      │      │
            │      │      │
            │      ▼      │
            │       end   │
            │             │
            │             │
            └─────────────┘
           d               c
        */

        targetShape.Vertices.clear();
        targetShape.Colors.clear();
        
        glm::vec2 curLine = end - start;

        glm::vec2 curLineNormalized;
        glm::vec2 perpendcularVector;

        //Special case of checking of the line has any length
        if(curLine.x == 0 && curLine.y == 0)
        {
            curLineNormalized = glm::vec2(0, 1);
            perpendcularVector = glm::vec2(1, 0);
        }
        else
        {
            curLineNormalized = glm::normalize(curLine);
            
            //This vector will have a direction of a ◄─── b  (b to a)
            perpendcularVector = glm::normalize(glm::cross(glm::vec3(curLine, 0), glm::vec3(0, 0, 1)));
        }


        //Find the position of all vertices based on the thickness
        //Get a and b
        glm::vec2 a = start + perpendcularVector * startThickness - curLineNormalized * startThickness;
        glm::vec2 b = start - perpendcularVector * startThickness - curLineNormalized * startThickness;

        //Get c and d
        glm::vec2 c = end - perpendcularVector * endThickness + curLineNormalized * endThickness;
        glm::vec2 d = end + perpendcularVector * endThickness + curLineNormalized * endThickness;

        targetShape.Vertices.push_back(a) ;
        targetShape.Vertices.push_back(b);
        targetShape.Vertices.push_back(c);
        targetShape.Vertices.push_back(d);

        //Apply color
        targetShape.Colors.push_back(startColor);
        targetShape.Colors.push_back(startColor);
        targetShape.Colors.push_back(endColor);
        targetShape.Colors.push_back(endColor);

        targetShape.Type = ShapeType::LINE;
        targetShape.Data.Line.StartPos = start;        
        targetShape.Data.Line.StartSize = startThickness;        
        targetShape.Data.Line.StartColor = startColor;        
        targetShape.Data.Line.EndPos = end;        
        targetShape.Data.Line.EndSize = endThickness;        
        targetShape.Data.Line.EndColor = endColor;

        targetShape.BehindGUI = behindGUIObject;
    }


    void Shape::ConstructRenderInfo()
    {
        ssLOG_FUNC_ENTRY();
        //Getting all the rendering details from container
        std::vector<glm::vec2>& drawingVertices = Container->Extension_GetDrawingVertices();
        std::vector<glm::vec2>& drawingUVs = Container->Extension_GetDrawingUVs();
        std::vector<glm::u8vec4>& drawingColors = Container->Extension_GetDrawingColours();
        std::vector<int>& drawingCounts = Container->Extension_GetDrawingCounts();
        std::vector<ssGUI::DrawingProperty>& drawingProperties = Container->Extension_GetDrawingProperties();

        using GUIType = ssGUI::Enums::GUIObjectType;
        glm::vec2 curPos = Container->GetType() == GUIType::WINDOW && Container->GetType() != GUIType::MAIN_WINDOW ? 
            Container->GetGlobalPosition() + glm::vec2(0, static_cast<ssGUI::Window*>(Container)->GetTitlebarHeight()) :
            Container->GetGlobalPosition();

        for(int i = 0; i < AdditionalShapes.size(); i++)
        {
            if(AdditionalShapes[i].BehindGUI != ExtensionPreRender)
                continue;

            for(int j = 0; j < AdditionalShapes[i].Vertices.size(); j++)
                drawingVertices.push_back(AdditionalShapes[i].Vertices[j] + curPos);
            
            drawingUVs.insert(drawingUVs.end(), AdditionalShapes[i].Vertices.size(), glm::vec2());
            drawingColors.insert(drawingColors.end(), AdditionalShapes[i].Colors.begin(), AdditionalShapes[i].Colors.end());
            drawingCounts.push_back(AdditionalShapes[i].Vertices.size());
            drawingProperties.push_back(ssGUI::DrawingProperty());
        }

        if(!ExtensionPreRender && !GUIObjectShapesToRemove.empty())
        {
            int originalIndex = 0;
            for(int i = 0; i < drawingCounts.size(); i++)
            {
                //For each shape, check if this shape needs to be removed
                if(GUIObjectShapesToRemove.find(originalIndex - Container->Extension_GetGUIObjectFirstShapeIndex()) 
                    != GUIObjectShapesToRemove.end())
                {
                    int startIndex = 0;

                    for(int j = 0; j < i; j++)
                        startIndex += drawingCounts[j];
                    
                    int endIndex = startIndex + drawingCounts[i];

                    drawingVertices.erase(drawingVertices.begin() + startIndex, drawingVertices.begin() + endIndex);
                    drawingUVs.erase(drawingUVs.begin() + startIndex, drawingUVs.begin() + endIndex);
                    drawingColors.erase(drawingColors.begin() + startIndex, drawingColors.begin() + endIndex);
                    drawingCounts.erase(drawingCounts.begin() + i);
                    drawingProperties.erase(drawingProperties.begin() + i);

                    i--;
                }

                //This keeps track of the original index of the shapes even when the shapes are removed
                originalIndex++;
            }
        }

        ssLOG_FUNC_EXIT();
    }

    void Shape::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::vec2 mainWindowPositionOffset)
    {       
        //Otherwise just call this
        ConstructRenderInfo();
    }
    
    //Defining the extension name
    const std::string Shape::EXTENSION_NAME = "Shape";
    
    int Shape::AddAdditionalPolygon()
    {
        AdditionalShapes.push_back(AdditionalShape());
        AdditionalShapes[AdditionalShapes.size() - 1].ID = NextID;
        AdditionalShapes[AdditionalShapes.size() - 1].Type = ShapeType::POLYGON;
        return NextID++;
    }

    int Shape::AddAdditionalPolygon(std::vector<glm::vec2>const & vertices, std::vector<glm::u8vec4>const & colors, bool behindGUIObject)
    {
        AdditionalShapes.push_back(AdditionalShape());
        ConstructAdditionalPolygon(AdditionalShapes[AdditionalShapes.size() - 1], vertices, colors, behindGUIObject);
        AdditionalShapes[AdditionalShapes.size() - 1].ID = NextID;

        if(Container != nullptr)
            Container->RedrawObject();

        return NextID++;
    }

    int Shape::AddAdditionalPolygon(std::vector<glm::vec2>const & vertices, std::vector<glm::u8vec4>const & colors, bool behindGUIObject, int index)
    {
        if(index < 0 || index > AdditionalShapes.size())
            return -1;
        
        AdditionalShape additionalShape;
        AdditionalShapes.insert(AdditionalShapes.begin() + index, additionalShape);
        ConstructAdditionalPolygon(AdditionalShapes[index], vertices, colors, behindGUIObject);
        AdditionalShapes[index].ID = NextID;

        if(Container != nullptr)
            Container->RedrawObject();

        return NextID++;
    }

    int Shape::AddAdditionalRectangle(glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject)
    {
        AdditionalShapes.push_back(AdditionalShape());
        ConstructAdditionalRectangle(AdditionalShapes[AdditionalShapes.size() - 1], pos, size, color, behindGUIObject);
        AdditionalShapes[AdditionalShapes.size() - 1].ID = NextID;

        if(Container != nullptr)
            Container->RedrawObject();

        return NextID++;
    }

    int Shape::AddAdditionalRectangle(glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject, int index)
    {
        if(index < 0 || index > AdditionalShapes.size())
            return -1;
        
        AdditionalShape additionalShape;
        AdditionalShapes.insert(AdditionalShapes.begin() + index, additionalShape);
        ConstructAdditionalRectangle(AdditionalShapes[index], pos, size, color, behindGUIObject);
        AdditionalShapes[index].ID = NextID;

        if(Container != nullptr)
            Container->RedrawObject();

        return NextID++;
    }

    int Shape::AddAdditionalRectangle()
    {
        return AddAdditionalPolygon();
    }

    int Shape::AddAdditionalCircle(glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject)
    {
        AdditionalShapes.push_back(AdditionalShape());
        ConstructAdditionalCircle(AdditionalShapes[AdditionalShapes.size() - 1], pos, size, color, behindGUIObject);
        AdditionalShapes[AdditionalShapes.size() - 1].ID = NextID;

        if(Container != nullptr)
            Container->RedrawObject();

        return NextID++;
    }

    int Shape::AddAdditionalCircle(glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject, int index)
    {
        if(index < 0 || index > AdditionalShapes.size())
            return -1;
        
        AdditionalShape additionalShape;
        AdditionalShapes.insert(AdditionalShapes.begin() + index, additionalShape);
        ConstructAdditionalCircle(AdditionalShapes[index], pos, size, color, behindGUIObject);
        AdditionalShapes[index].ID = NextID;

        if(Container != nullptr)
            Container->RedrawObject();

        return NextID++;
    }

    int Shape::AddAdditionalCircle()
    {
        return AddAdditionalPolygon();
    }

    int Shape::AddAdditionalLine(glm::vec2 start, glm::vec2 end, float startThickness, float endThickness, glm::u8vec4 startColor, glm::u8vec4 endColor, bool behindGUIObject)
    {
        AdditionalShapes.push_back(AdditionalShape());
        ConstructAdditionalLine(AdditionalShapes[AdditionalShapes.size() - 1], start, end, startThickness, endThickness, startColor, endColor, behindGUIObject);
        AdditionalShapes[AdditionalShapes.size() - 1].ID = NextID;

        if(Container != nullptr)
            Container->RedrawObject();

        return NextID++;
    }

    int Shape::AddAdditionalLine(glm::vec2 start, glm::vec2 end, float startThickness, float endThickness, glm::u8vec4 startColor, glm::u8vec4 endColor, bool behindGUIObject, int index)
    {
        if(index < 0 || index > AdditionalShapes.size())
            return -1;
        
        AdditionalShape additionalShape;
        AdditionalShapes.insert(AdditionalShapes.begin() + index, additionalShape);
        ConstructAdditionalLine(AdditionalShapes[index], start, end, startThickness, endThickness, startColor, endColor, behindGUIObject);
        AdditionalShapes[index].ID = NextID;

        if(Container != nullptr)
            Container->RedrawObject();

        return NextID++;
    }

    int Shape::AddAdditionalLine()
    {
        return AddAdditionalPolygon();
    }

    void Shape::SetAdditionalPolygon(int id, std::vector<glm::vec2>const & vertices, std::vector<glm::u8vec4>const & colors, bool behindGUIObject)
    {
        for(int i = 0; i < AdditionalShapes.size(); i++)
        {
            if(AdditionalShapes[i].ID != id)
                continue;

            ConstructAdditionalPolygon(AdditionalShapes[i], vertices, colors, behindGUIObject);

            if(Container != nullptr)
                Container->RedrawObject();

            return;
        }
    }

    void Shape::SetAdditionalRectangle(int id, glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject)
    {
        for(int i = 0; i < AdditionalShapes.size(); i++)
        {
            if(AdditionalShapes[i].ID != id)
                continue;

            if( AdditionalShapes[i].Type == ShapeType::RECTANGLE && 
                pos == AdditionalShapes[i].Data.Rectangle.Pos &&
                size == AdditionalShapes[i].Data.Rectangle.Size &&
                color == AdditionalShapes[i].Data.Rectangle.Color)
            {
                if(AdditionalShapes[i].BehindGUI != behindGUIObject)
                {
                    AdditionalShapes[i].BehindGUI = behindGUIObject;
                    if(Container != nullptr)
                        Container->RedrawObject();        
                }
                
                return;
            }

            ConstructAdditionalRectangle(AdditionalShapes[i], pos, size, color, behindGUIObject);

            if(Container != nullptr)
                Container->RedrawObject();

            return;
        }
    }

    void Shape::SetAdditionalCircle(int id, glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject)
    {
        for(int i = 0; i < AdditionalShapes.size(); i++)
        {
            if(AdditionalShapes[i].ID != id)
                continue;

            if( AdditionalShapes[i].Type == ShapeType::CIRCLE && 
                pos == AdditionalShapes[i].Data.Circle.Pos &&
                size == AdditionalShapes[i].Data.Circle.Size &&
                color == AdditionalShapes[i].Data.Circle.Color)
            {
                if(AdditionalShapes[i].BehindGUI != behindGUIObject)
                {
                    AdditionalShapes[i].BehindGUI = behindGUIObject;
                    if(Container != nullptr)
                        Container->RedrawObject();        
                }
                
                return;
            }

            ConstructAdditionalCircle(AdditionalShapes[i], pos, size, color, behindGUIObject);

            if(Container != nullptr)
                Container->RedrawObject();

            return;
        }
    }
    
    void Shape::SetAdditionalLine(int id, glm::vec2 start, glm::vec2 end, float startThickness, float endThickness, glm::u8vec4 startColor, glm::u8vec4 endColor, bool behindGUIObject)
    {
        for(int i = 0; i < AdditionalShapes.size(); i++)
        {
            if(AdditionalShapes[i].ID != id)
                continue;

            if( AdditionalShapes[i].Type == ShapeType::LINE && 
                start == AdditionalShapes[i].Data.Line.StartPos &&
                end == AdditionalShapes[i].Data.Line.EndPos &&
                startThickness == AdditionalShapes[i].Data.Line.StartSize &&
                endThickness == AdditionalShapes[i].Data.Line.EndSize &&
                startColor == AdditionalShapes[i].Data.Line.StartColor &&
                endColor == AdditionalShapes[i].Data.Line.EndColor)
            {
                if(AdditionalShapes[i].BehindGUI != behindGUIObject)
                {
                    AdditionalShapes[i].BehindGUI = behindGUIObject;
                    if(Container != nullptr)
                        Container->RedrawObject();        
                }
                
                return;
            }

            ConstructAdditionalLine(AdditionalShapes[i], start, end, startThickness, endThickness, startColor, endColor, behindGUIObject);

            if(Container != nullptr)
                Container->RedrawObject();

            return;
        }
    }

    std::vector<glm::vec2>* Shape::GetAdditionalShapeVerticesWithIndex(int index)
    {
        if(index < 0 || index > AdditionalShapes.size())
            return nullptr;

        return &AdditionalShapes[index].Vertices;
    }

    std::vector<glm::vec2>* Shape::GetAdditionalShapeVerticesWithID(int id)
    {
        for(int i = 0; i < AdditionalShapes.size(); i++)
        {
            if(AdditionalShapes[i].ID != id)
                continue;

            return &AdditionalShapes[i].Vertices;
        }
        
        return nullptr;
    }

    std::vector<glm::u8vec4>* Shape::GetAdditionalShapeColorsWithIndex(int index)
    {
        if(index < 0 || index > AdditionalShapes.size())
            return nullptr;

        return &AdditionalShapes[index].Colors;
    }

    std::vector<glm::u8vec4>* Shape::GetAdditionalShapeColorsWithID(int id)
    {
        for(int i = 0; i < AdditionalShapes.size(); i++)
        {
            if(AdditionalShapes[i].ID != id)
                continue;

            return &AdditionalShapes[i].Colors;
        }
        
        return nullptr;
    }

    int Shape::GetAdditionalShapesCount() const
    {
        return AdditionalShapes.size();
    }

    void Shape::ClearAllAdditionalShapes()
    {
        AdditionalShapes.clear();
    }

    void Shape::RemoveAdditionalShapeWithIndex(int index)
    {
        if(index < 0 || index > AdditionalShapes.size())
            return;

        AdditionalShapes.erase(AdditionalShapes.begin() + index);

        if(Container != nullptr)
            Container->RedrawObject();
    }

    void Shape::RemoveAdditionalShapeWithID(int id)
    {
        for(int i = 0; i < AdditionalShapes.size(); i++)
        {
            if(AdditionalShapes[i].ID != id)
                continue;

            AdditionalShapes.erase(AdditionalShapes.begin() + i);

            if(Container != nullptr)
                Container->RedrawObject();

            return;
        }
    }

    void Shape::RemoveGUIObjectShape(int index)
    {
        GUIObjectShapesToRemove.insert(index);

        if(Container != nullptr)
            Container->RedrawObject();
    }

    void Shape::RestoreGUIObjectShape(int index)
    {
        if(GUIObjectShapesToRemove.find(index) != GUIObjectShapesToRemove.end())
            GUIObjectShapesToRemove.erase(index);

        if(Container != nullptr)
            Container->RedrawObject();
    }

    void Shape::SetEnabled(bool enabled)
    {
        Enabled = enabled;

        //Redrawing the object if this affects the rendering
        if(Container != nullptr)
            Container->RedrawObject();
    }

    bool Shape::IsEnabled() const
    {
        return Enabled;
    }
        
    //Extension methods
    void Shape::Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, ssGUI::GUIObject* mainWindow)
    {
        ssLOG_FUNC_ENTRY();

        if(!Enabled || Container == nullptr)
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        ssLOG_FUNC_EXIT();
    }

    void Shape::Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::vec2 mainWindowPositionOffset)
    {        
        ssLOG_FUNC_ENTRY();
        
        if(!Enabled || Container == nullptr)
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        ExtensionPreRender = isPreRender;

        if(Container->IsRedrawNeeded())
            ConstructRenderInfo();
        
        ssLOG_FUNC_EXIT();
    }

    std::string Shape::GetExtensionName()
    {
        return EXTENSION_NAME;
    }

    void Shape::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;
    }

    void Shape::Copy(ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        
        ssGUI::Extensions::Shape* Shape = static_cast<ssGUI::Extensions::Shape*>(extension);
        
        Enabled = Shape->IsEnabled();
        ExtensionPreRender = Shape->ExtensionPreRender;
        AdditionalShapes = Shape->AdditionalShapes;
        GUIObjectShapesToRemove = Shape->GUIObjectShapesToRemove;
        NextID = Shape->NextID;
    }

    ObjectsReferences* Shape::Internal_GetObjectsReferences()
    {
        return nullptr;
    }

    Shape* Shape::Clone(ssGUI::GUIObject* newContainer)
    {
        Shape* temp = new Shape(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
}

}