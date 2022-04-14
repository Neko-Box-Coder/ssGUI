#include "ssGUI/Extensions/Shape.hpp" //Or the location of your custom extension header

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

//Remember to add the .cpp file to the cmake list if you are using cmake

namespace ssGUI::Extensions
{
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
            targetShape.Vertices.push_back(glm::vec2(size.x * cos(angle), size.y * sin(angle)) + size);
            targetShape.Colors.push_back(color);
        }

        targetShape.BehindGUI = behindGUIObject;
    }

    void Shape::ConstructRenderInfo()
    {
        //Getting all the rendering details from container
        std::vector<glm::vec2>& drawingVertices = Container->Extension_GetDrawingVertices();
        std::vector<glm::vec2>& drawingUVs = Container->Extension_GetDrawingUVs();
        std::vector<glm::u8vec4>& drawingColors = Container->Extension_GetDrawingColours();
        std::vector<int>& drawingCounts = Container->Extension_GetDrawingCounts();
        std::vector<ssGUI::DrawingProperty>& drawingProperties = Container->Extension_GetDrawingProperties();
        glm::vec2 curPos = Container->GetGlobalPosition();

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
    }

    void Shape::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::vec2 mainWindowPositionOffset)
    {       
        //Otherwise just call this
        ConstructRenderInfo();
    }
    
    //Defining the extension name
    const std::string Shape::EXTENSION_NAME = "Shape";
    
    Shape::Shape() : Container(nullptr), Enabled(true), ExtensionPreRender(true), AdditionalShapes(), GUIObjectShapesToRemove(), NextID(0)
    {}

    Shape::~Shape()
    {}

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

            ConstructAdditionalCircle(AdditionalShapes[i], pos, size, color, behindGUIObject);

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
    void Shape::Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();

        if(!Enabled || Container == nullptr)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        FUNC_DEBUG_EXIT();
    }

    void Shape::Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::vec2 mainWindowPositionOffset)
    {        
        FUNC_DEBUG_ENTRY();
        
        if(!Enabled || Container == nullptr)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        ExtensionPreRender = isPreRender;

        if(Container->IsRedrawNeeded())
            ConstructRenderInfo();
        
        FUNC_DEBUG_EXIT();
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

    Extension* Shape::Clone(ssGUI::GUIObject* newContainer)
    {
        Shape* temp = new Shape(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
}