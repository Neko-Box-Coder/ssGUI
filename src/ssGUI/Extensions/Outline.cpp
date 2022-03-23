#include "ssGUI/Extensions/Outline.hpp"

#include "ssGUI/BaseClasses/MainWindow.hpp" //For getting mouse position

//Remember to add the .cpp file to the cmake list if you are using cmake

namespace ssGUI::Extensions
{
    Outline::Outline(Outline const& other)
    {
        Container = nullptr;
        Enabled = other.IsEnabled();

        OutlineThickness = other.GetOutlineThickness();
        SimpleOutline = other.IsSimpleOutline();
        OutlineColor = other.GetOutlineColor();
        TargetShapes = other.TargetShapes;
        TargetVertices = other.TargetVertices;

        VerticesToOutline = std::vector<int>();
        VerticesToOutlinePrevVertices = std::vector<int>();
        VerticesToOutlineNextVertices = std::vector<int>();
        VerticesToOutlineNextNextVertices = std::vector<int>();
    }

    void Outline::GetStartEndVertexIndex(int currentIndex, int& startIndex, int& endIndex, std::vector<int> const & drawingCounts)
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

    void Outline::UpdateVerticesForOutline()
    {
        VerticesToOutline.clear();
        VerticesToOutlinePrevVertices.clear();
        VerticesToOutlineNextVertices.clear();
        VerticesToOutlineNextNextVertices.clear();

        std::vector<glm::vec2>& drawingVertices = Container->Extension_GetDrawingVertices();
        std::vector<int>& drawingCounts = Container->Extension_GetDrawingCounts();

        int startIndex = 0;
        int endIndex = drawingCounts[0];
        if(!TargetVertices.empty())
        {
            VerticesToOutline = TargetVertices;
            for(int i = 0; i < VerticesToOutline.size(); i++)
            {
                //Invlaid index check
                if(VerticesToOutline[i] >= drawingVertices.size())
                {
                    VerticesToOutline.erase(VerticesToOutline.begin() + i);
                    i--;
                    continue;
                }

                if(VerticesToOutline[i] < startIndex || VerticesToOutline[i] >= endIndex)
                    GetStartEndVertexIndex(VerticesToOutline[i], startIndex, endIndex, drawingCounts);

                int prevIndex = VerticesToOutline[i];
                do
                {
                    prevIndex = (prevIndex == startIndex ? endIndex - 1 : prevIndex - 1);   
                }
                while(drawingVertices[prevIndex] - drawingVertices[VerticesToOutline[i]] == glm::vec2());
                VerticesToOutlinePrevVertices.push_back(prevIndex);

                int nextIndex = VerticesToOutline[i];
                do
                {
                    nextIndex = (nextIndex == endIndex - 1 ? startIndex : nextIndex + 1);
                }
                while(drawingVertices[nextIndex] - drawingVertices[VerticesToOutline[i]] == glm::vec2());
                VerticesToOutlineNextVertices.push_back(nextIndex);

                if(!SimpleOutline)
                {
                    int nextNextIndex = nextIndex;
                    do
                    {
                        nextNextIndex = (nextNextIndex == endIndex - 1 ? startIndex : nextNextIndex + 1);
                    }
                    while(drawingVertices[nextNextIndex] - drawingVertices[nextIndex] == glm::vec2());
                    VerticesToOutlineNextNextVertices.push_back(nextNextIndex);
                }
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
                    VerticesToOutline.push_back(j);
                    int prevIndex = j;
                    do
                    {
                        prevIndex = (prevIndex == startIndex ? startIndex + drawingCounts[TargetShapes[i]] - 1 : prevIndex - 1);
                    }
                    while(drawingVertices[prevIndex] - drawingVertices[j] == glm::vec2());
                    VerticesToOutlinePrevVertices.push_back(prevIndex);

                    int nextIndex = j;
                    do
                    {
                        nextIndex = (nextIndex == startIndex + drawingCounts[TargetShapes[i]] - 1 ? startIndex : nextIndex + 1);
                    }
                    while(drawingVertices[nextIndex] - drawingVertices[j] == glm::vec2());
                    VerticesToOutlineNextVertices.push_back(nextIndex);

                    if(!SimpleOutline)
                    {
                        int nextNextIndex = nextIndex;
                        do
                        {
                            nextNextIndex = (nextNextIndex == startIndex + drawingCounts[TargetShapes[i]] - 1 ? startIndex : nextNextIndex + 1);
                        }
                        while(drawingVertices[nextIndex] - drawingVertices[nextNextIndex] == glm::vec2());
                        VerticesToOutlineNextNextVertices.push_back(nextNextIndex);
                    }
                }
            }
        }
    }

    void Outline::ConstructComplexOutline()
    {        
        //Getting all the rendering details from container
        std::vector<glm::vec2>& drawingVertices = Container->Extension_GetDrawingVertices();
        std::vector<glm::vec2>& drawingUVs = Container->Extension_GetDrawingUVs();
        std::vector<glm::u8vec4>& drawingColors = Container->Extension_GetDrawingColours();
        std::vector<int>& drawingCounts = Container->Extension_GetDrawingCounts();
        std::vector<ssGUI::DrawingProperty>& drawingProperties = Container->Extension_GetDrawingProperties();

        std::vector<glm::vec2> newVertices;    //Lists of new vertices as arc
        std::vector<glm::u8vec4> newColors;     //Associated colors
        std::vector<int> newCounts;             //The number vertices per arc

        if(drawingCounts.empty())
            return;

        UpdateVerticesForOutline();

        for(int i = 0; i < VerticesToOutline.size(); i++)
        {
            glm::vec2 curVertex = drawingVertices[VerticesToOutline[i]];

            glm::vec2 nextVertex = drawingVertices[VerticesToOutlineNextVertices[i]];
            glm::vec2 nextNextVertex = drawingVertices[VerticesToOutlineNextNextVertices[i]];
            glm::vec2 prevVertex = drawingVertices[VerticesToOutlinePrevVertices[i]];

            glm::vec2 curLine = nextVertex - curVertex;
            glm::vec2 prevLine = prevVertex - curVertex;
            glm::vec2 nextLine = nextNextVertex - nextVertex;

            if(curLine != glm::vec2())
                curLine = glm::normalize(curLine);
            
            if(prevLine != glm::vec2())
                prevLine = glm::normalize(prevLine);
            
            if(nextLine != glm::vec2())
                nextLine = glm::normalize(nextLine);

            glm::vec2 outlinePos1 = glm::vec2();
            glm::vec2 outlinePos2 = glm::vec2();

            if(curLine + prevLine != glm::vec2())
            {
                glm::vec2 normalDir = glm::normalize((curLine + prevLine));
                
                if(glm::cross(glm::vec3(normalDir, 0), glm::vec3(curLine, 0)).z < 0)
                    outlinePos1 = -normalDir * (float)OutlineThickness + glm::vec2(curVertex);
                else
                    outlinePos1 = normalDir * (float)OutlineThickness + glm::vec2(curVertex);                
            }
            else
            {
                outlinePos1 = glm::normalize(glm::cross(glm::vec3(curLine, 0), glm::vec3(0, 0, 1))) * (float)OutlineThickness + glm::vec3(curVertex, 0);
            }

            if(nextLine - curLine != glm::vec2())
            {
                glm::vec2 normalDir = glm::normalize((nextLine - curLine));
                
                if(glm::cross(glm::vec3(normalDir, 0), glm::vec3(nextLine, 0)).z < 0)
                    outlinePos2  = -normalDir * (float)OutlineThickness + glm::vec2(nextVertex);
                else
                    outlinePos2  = normalDir * (float)OutlineThickness + glm::vec2(nextVertex);
            }
            else
            {
                outlinePos2 = glm::normalize(glm::cross(glm::vec3(nextLine, 0), glm::vec3(0, 0, 1))) * (float)OutlineThickness + glm::vec3(nextVertex, 0);
            }

            newVertices.push_back(outlinePos1);
            newVertices.push_back(outlinePos2);
            newVertices.push_back(nextVertex);
            newVertices.push_back(curVertex);

            newColors.push_back(OutlineColor);
            newColors.push_back(OutlineColor);
            newColors.push_back(OutlineColor);
            newColors.push_back(OutlineColor);

            newCounts.push_back(4);
        }

        // DEBUG_LINE();

        drawingVertices.insert(drawingVertices.end(), newVertices.begin(), newVertices.end());
        drawingColors.insert(drawingColors.end(), newColors.begin(), newColors.end());
        drawingCounts.insert(drawingCounts.end(), newCounts.begin(), newCounts.end());

        drawingUVs.insert(drawingUVs.end(), drawingVertices.size() - drawingUVs.size(), glm::vec2());
        drawingProperties.insert(drawingProperties.end(), drawingCounts.size() - drawingProperties.size(), ssGUI::DrawingProperty());
    }

    void Outline::ConstructSimpleOutline()
    {        
        //Getting all the rendering details from container
        std::vector<glm::vec2>& drawingVertices = Container->Extension_GetDrawingVertices();
        std::vector<glm::vec2>& drawingUVs = Container->Extension_GetDrawingUVs();
        std::vector<glm::u8vec4>& drawingColors = Container->Extension_GetDrawingColours();
        std::vector<int>& drawingCounts = Container->Extension_GetDrawingCounts();
        std::vector<ssGUI::DrawingProperty>& drawingProperties = Container->Extension_GetDrawingProperties();

        std::vector<glm::vec2> newVertices;    //Lists of new vertices as arc
        std::vector<glm::u8vec4> newColors;     //Associated colors
        std::vector<int> newCounts;             //The number vertices per arc

        if(drawingCounts.empty())
            return;

        //TODO : only need to fill current, prev and next vertices instead
        UpdateVerticesForOutline();

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

            glm::vec2 outlinePos1 = glm::vec2();

            if(curLine + prevLine != glm::vec2())
            {
                glm::vec2 normalDir = glm::normalize((curLine + prevLine));
                
                if(glm::cross(glm::vec3(normalDir, 0), glm::vec3(curLine, 0)).z < 0)
                    outlinePos1 = -normalDir * (float)OutlineThickness + glm::vec2(curVertex);
                else
                    outlinePos1 = normalDir * (float)OutlineThickness + glm::vec2(curVertex);
            }
            else
            {
                outlinePos1 = glm::normalize(glm::cross(glm::vec3(curLine, 0), glm::vec3(0, 0, 1))) * (float)OutlineThickness + glm::vec3(curVertex, 0);
            }

            newVertices.push_back(outlinePos1);
            newColors.push_back(OutlineColor);

        }
        
        newCounts.push_back(VerticesToOutline.size());

        drawingVertices.insert(drawingVertices.begin(), newVertices.begin(), newVertices.end());
        drawingColors.insert(drawingColors.begin(), newColors.begin(), newColors.end());
        drawingCounts.insert(drawingCounts.begin(), newCounts.begin(), newCounts.end());

        drawingUVs.insert(drawingUVs.begin(), drawingVertices.size() - drawingUVs.size(), glm::vec2());
        drawingProperties.insert(drawingProperties.begin(), drawingCounts.size() - drawingProperties.size(), ssGUI::DrawingProperty());
    }

    void Outline::ConstructRenderInfo()
    {
        if(SimpleOutline)
            ConstructSimpleOutline();
        else
            ConstructComplexOutline();
    }

    void Outline::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        //If you need the drawingInterface or mainWindow, do it here.

        //Otherwise just call this
        ConstructRenderInfo();
    }

    //Defining the extension name
    const std::string Outline::EXTENSION_NAME = "Outline";

    //Add ObjectsReferences construction if you are using it
    Outline::Outline() : Container(nullptr), Enabled(true), OutlineThickness(1), SimpleOutline(true), OutlineColor(glm::u8vec4(0, 0, 0, 255)), 
                            TargetShapes{0}, TargetVertices(), VerticesToOutline(), VerticesToOutlinePrevVertices(),
                            VerticesToOutlineNextVertices(), VerticesToOutlineNextNextVertices()
    {}

    Outline::~Outline()
    {}

    void Outline::SetOutlineThickness(int thickness)
    {
        OutlineThickness = thickness;
        if(Container != nullptr)
            Container->RedrawObject();
    }

    int Outline::GetOutlineThickness() const
    {
        return OutlineThickness;
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
    void Outline::Internal_Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();

        if(!Enabled || Container == nullptr)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        if(IsPreUpdate)
        {
            //Any changes before running the GUI object update
        }

        if(!IsPreUpdate)
        {
            //Any changes after running the GUI object update
        }

        FUNC_DEBUG_EXIT();
    }

    void Outline::Internal_Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        FUNC_DEBUG_ENTRY();

        if(!Enabled || Container == nullptr || IsPreRender)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        if(!SimpleOutline)
        {
            DEBUG_LINE();
            if(Container->IsRedrawNeeded())
            {
        DEBUG_LINE();
                ConstructRenderInfo();
            }
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
        OutlineColor = outline->GetOutlineColor();
        TargetShapes = outline->TargetShapes;
        TargetVertices = outline->TargetVertices;

        VerticesToOutline = std::vector<int>();
        VerticesToOutlinePrevVertices = std::vector<int>();
        VerticesToOutlineNextVertices = std::vector<int>();
        VerticesToOutlineNextNextVertices = std::vector<int>();
    }

    ObjectsReferences* Outline::Internal_GetObjectsReferences()
    {
        return nullptr;
    }

    Extension* Outline::Clone(ssGUI::GUIObject* newContainer)
    {
        Outline* temp = new Outline(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
}