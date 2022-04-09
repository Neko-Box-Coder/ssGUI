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
    }

    void Shape::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::vec2 mainWindowPositionOffset)
    {       
        //Otherwise just call this
        ConstructRenderInfo();
    }
    
    //Defining the extension name
    const std::string Shape::EXTENSION_NAME = "Shape";
    
    //Add ObjectsReferences construction if you are using it 
    Shape::Shape() : Container(nullptr), Enabled(true), ExtensionPreRender(true), AdditionalShapes()
    {}

    Shape::~Shape()
    {}

    void Shape::AddShape(std::vector<glm::vec2>const & vertices, std::vector<glm::u8vec4>const & colors, bool behindGUIObject)
    {
        AdditionalShapes.push_back(AdditionalShape());
        AdditionalShapes[AdditionalShapes.size() - 1].Vertices = vertices;
        AdditionalShapes[AdditionalShapes.size() - 1].Colors = colors;
        AdditionalShapes[AdditionalShapes.size() - 1].BehindGUI = behindGUIObject;
    }

    void Shape::AddShape(std::vector<glm::vec2>const & vertices, std::vector<glm::u8vec4>const & colors, bool behindGUIObject, int index)
    {
        if(index < 0 || index > AdditionalShapes.size())
            return;
        
        AdditionalShape additionalShape;
        AdditionalShapes.insert(AdditionalShapes.begin() + index, additionalShape);

        AdditionalShapes[index].Vertices = vertices;
        AdditionalShapes[index].Colors = colors;
        AdditionalShapes[index].BehindGUI = behindGUIObject;
    }

    void Shape::AddRectangle(glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject)
    {
        AdditionalShapes.push_back(AdditionalShape());
        AdditionalShapes[AdditionalShapes.size() - 1].Vertices.push_back(pos);
        AdditionalShapes[AdditionalShapes.size() - 1].Vertices.push_back(pos + glm::vec2(size.x, 0));
        AdditionalShapes[AdditionalShapes.size() - 1].Vertices.push_back(pos + size);
        AdditionalShapes[AdditionalShapes.size() - 1].Vertices.push_back(pos + glm::vec2(0, size.y));

        AdditionalShapes[AdditionalShapes.size() - 1].Colors.push_back(color);
        AdditionalShapes[AdditionalShapes.size() - 1].Colors.push_back(color);
        AdditionalShapes[AdditionalShapes.size() - 1].Colors.push_back(color);
        AdditionalShapes[AdditionalShapes.size() - 1].Colors.push_back(color);

        AdditionalShapes[AdditionalShapes.size() - 1].BehindGUI = behindGUIObject;
    }

    void Shape::AddRectangle(glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject, int index)
    {
        if(index < 0 || index > AdditionalShapes.size())
            return;
        
        AdditionalShape additionalShape;
        AdditionalShapes.insert(AdditionalShapes.begin() + index, additionalShape);

        AdditionalShapes[index].Vertices.push_back(pos);
        AdditionalShapes[index].Vertices.push_back(pos + glm::vec2(size.x, 0));
        AdditionalShapes[index].Vertices.push_back(pos + size);
        AdditionalShapes[index].Vertices.push_back(pos + glm::vec2(0, size.y));

        AdditionalShapes[index].Colors.push_back(color);
        AdditionalShapes[index].Colors.push_back(color);
        AdditionalShapes[index].Colors.push_back(color);
        AdditionalShapes[index].Colors.push_back(color);

        AdditionalShapes[index].BehindGUI = behindGUIObject;
    }

    void Shape::AddCircle(glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject)
    {
        AdditionalShapes.push_back(AdditionalShape());
        size *= 0.5;

        for(int i = 0; i < (int)(glm::length(size) * 2.f * pi()) + 2; i++)
        {
            float angle = 2.f * pi() * ((float)i / (float)((int)(glm::length(size) * 2.f * pi() + 1)));
            AdditionalShapes[AdditionalShapes.size() - 1].Vertices.push_back(glm::vec2(size.x * cos(angle), size.y * sin(angle)) + size);
            AdditionalShapes[AdditionalShapes.size() - 1].Colors.push_back(color);
        }

        AdditionalShapes[AdditionalShapes.size() - 1].BehindGUI = behindGUIObject;
    }

    void Shape::AddCircle(glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject, int index)
    {
        if(index < 0 || index > AdditionalShapes.size())
            return;
        
        AdditionalShape additionalShape;
        AdditionalShapes.insert(AdditionalShapes.begin() + index, additionalShape);
        size *= 0.5;

        for(int i = 0; i < (int)(glm::length(size) * 2.f * pi()) + 2; i++)
        {
            float angle = 2.f * pi() * ((float)i / (float)((int)(glm::length(size) * 2.f * pi() + 1)));
            AdditionalShapes[index].Vertices.push_back(glm::vec2(size.x * cos(angle), size.y * sin(angle)) + size);
            AdditionalShapes[index].Colors.push_back(color);
        }

        AdditionalShapes[index].BehindGUI = behindGUIObject;
    }
    
    std::vector<glm::vec2>* Shape::GetVertices(int index)
    {
        if(index < 0 || index > AdditionalShapes.size())
            return nullptr;

        return &AdditionalShapes[index].Vertices;
    }

    std::vector<glm::u8vec4>* Shape::GetColors(int index)
    {
        if(index < 0 || index > AdditionalShapes.size())
            return nullptr;

        return &AdditionalShapes[index].Colors;
    }

    void Shape::RemoveShape(int index)
    {
        if(index < 0 || index > AdditionalShapes.size())
            return;

        AdditionalShapes.erase(AdditionalShapes.begin() + index);
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