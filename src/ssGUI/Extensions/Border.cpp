#include "ssGUI/Extensions/Border.hpp"


namespace ssGUI::Extensions
{
    Border::Border(Border const& other)
    {
        Container = nullptr;
        BorderColour = other.GetBorderColour();
        BorderWidth = other.GetBorderWidth();
        Enabled = other.IsEnabled();
    }
    
    const std::string Border::EXTENSION_NAME = "Border";
    
    Border::Border() : BorderColour(glm::u8vec4(0, 0, 0, 255)), BorderWidth(1), Container(nullptr), Enabled(true)
    {}
    
    Border::~Border()
    {}

    glm::u8vec4 Border::GetBorderColour() const
    {
        return BorderColour;
    }
    
    void Border::SetBorderColour(glm::u8vec4 colour)
    {
        BorderColour = colour;

        if(Container != nullptr)
            Container->RedrawObject();
    }

    int Border::GetBorderWidth() const
    {
        return BorderWidth;
    }
    
    void Border::SetBorderWidth(int width)
    {
        BorderWidth = width;

        if(Container != nullptr)
            Container->RedrawObject();
    }

    void Border::DrawBorder()
    {        
        FUNC_DEBUG_ENTRY();
        
        glm::vec2 drawPosition = Container->GetGlobalPosition();
        
        int width = GetBorderWidth();
        glm::u8vec4 colour = GetBorderColour();

        std::vector<glm::vec2>& drawingVerticies = Container->Extension_GetDrawingVertices();
        std::vector<glm::vec2>& drawingUVs = Container->Extension_GetDrawingUVs();
        std::vector<glm::u8vec4>& drawingColours = Container->Extension_GetDrawingColours();
        std::vector<int>& drawingCounts = Container->Extension_GetDrawingCounts();
        std::vector<ssGUI::DrawingProperty>& drawingProperties = Container->Extension_GetDrawingProperties();

        //Top
        drawingVerticies.push_back(drawPosition + glm::vec2(0, -width));
        drawingVerticies.push_back(drawPosition + glm::vec2(Container->GetSize().x, -width));
        drawingVerticies.push_back(drawPosition + glm::vec2(Container->GetSize().x, 0));
        drawingVerticies.push_back(drawPosition + glm::vec2(0, 0));

        drawingColours.push_back(colour);
        drawingColours.push_back(colour);
        drawingColours.push_back(colour);
        drawingColours.push_back(colour);

        drawingUVs.push_back(glm::vec2());
        drawingUVs.push_back(glm::vec2());
        drawingUVs.push_back(glm::vec2());
        drawingUVs.push_back(glm::vec2());

        drawingCounts.push_back(4);
        drawingProperties.push_back(ssGUI::DrawingProperty());

        //Right
        drawingVerticies.push_back(drawPosition + glm::vec2(Container->GetSize().x, -width));
        drawingVerticies.push_back(drawPosition + glm::vec2(Container->GetSize().x + width, -width));
        drawingVerticies.push_back(drawPosition + glm::vec2(Container->GetSize().x + width, Container->GetSize().y + width));
        drawingVerticies.push_back(drawPosition + glm::vec2(Container->GetSize().x, Container->GetSize().y + width));

        drawingColours.push_back(colour);
        drawingColours.push_back(colour);
        drawingColours.push_back(colour);
        drawingColours.push_back(colour);

        drawingUVs.push_back(glm::vec2());
        drawingUVs.push_back(glm::vec2());
        drawingUVs.push_back(glm::vec2());
        drawingUVs.push_back(glm::vec2());

        drawingCounts.push_back(4);
        drawingProperties.push_back(ssGUI::DrawingProperty());

        //Bottom
        drawingVerticies.push_back(drawPosition + glm::vec2(0, Container->GetSize().y));
        drawingVerticies.push_back(drawPosition + glm::vec2(Container->GetSize().x, Container->GetSize().y));
        drawingVerticies.push_back(drawPosition + glm::vec2(Container->GetSize().x, Container->GetSize().y + width));
        drawingVerticies.push_back(drawPosition + glm::vec2(0, Container->GetSize().y + width));

        drawingColours.push_back(colour);
        drawingColours.push_back(colour);
        drawingColours.push_back(colour);
        drawingColours.push_back(colour);

        drawingUVs.push_back(glm::vec2());
        drawingUVs.push_back(glm::vec2());
        drawingUVs.push_back(glm::vec2());
        drawingUVs.push_back(glm::vec2());

        drawingCounts.push_back(4);
        drawingProperties.push_back(ssGUI::DrawingProperty());

        //Left
        drawingVerticies.push_back(drawPosition + glm::vec2(-width, -width));
        drawingVerticies.push_back(drawPosition + glm::vec2(0, -width));
        drawingVerticies.push_back(drawPosition + glm::vec2(0, Container->GetSize().y + width));
        drawingVerticies.push_back(drawPosition + glm::vec2(-width, Container->GetSize().y + width));

        drawingColours.push_back(colour);
        drawingColours.push_back(colour);
        drawingColours.push_back(colour);
        drawingColours.push_back(colour);

        drawingUVs.push_back(glm::vec2());
        drawingUVs.push_back(glm::vec2());
        drawingUVs.push_back(glm::vec2());
        drawingUVs.push_back(glm::vec2());

        drawingCounts.push_back(4);
        drawingProperties.push_back(ssGUI::DrawingProperty()); 

        FUNC_DEBUG_EXIT();
    }

    void Border::ConstructRenderInfo()
    {
        DrawBorder();
    }

    void Border::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        ConstructRenderInfo();
    }

    void Border::SetEnabled(bool enabled)
    {
        Enabled = enabled;

        if(Container != nullptr)
            Container->RedrawObject();
    }

    bool Border::IsEnabled() const
    {
        return Enabled;
    }

    void Border::Internal_Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        //Do nothing
    }
    
    void Border::Internal_Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        if(IsPreRender || Container == nullptr || Container->GetType() == ssGUI::Enums::GUIObjectType::MAIN_WINDOW || !Enabled)
            return;
        
        if(Container->IsRedrawNeeded())
            ConstructRenderInfo();
    }
    
    std::string Border::GetExtensionName()
    {
        return EXTENSION_NAME;
    }
    
    void Border::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;
        Container->RedrawObject();
    }

    void Border::Copy(ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        
        ssGUI::Extensions::Border* border = static_cast<ssGUI::Extensions::Border*>(extension);
        BorderColour = border->GetBorderColour();
        BorderWidth = border->GetBorderWidth();
        Enabled = border->IsEnabled();
    }

    ObjectsReferences* Border::Internal_GetObjectsReferences()
    {
        return nullptr;
    }

    Extension* Border::Clone(ssGUI::GUIObject* newContainer)
    {
        Border* temp = new Border(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
}