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
    }
    

    int Border::GetBorderWidth() const
    {
        return BorderWidth;
    }
    
    void Border::SetBorderWidth(int width)
    {
        BorderWidth = width;
    }

    void Border::DrawBorder(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {        
        glm::ivec2 drawPosition = Container->GetGlobalPosition();
        
        int width = GetBorderWidth();
        glm::u8vec4 colour = GetBorderColour();

        std::vector<glm::ivec2>& drawingVerticies = Container->Extension_GetDrawingVerticies();
        std::vector<glm::ivec2>& drawingUVs = Container->Extension_GetDrawingUVs();
        std::vector<glm::u8vec4>& drawingColours = Container->Extension_GetDrawingColours();
        std::vector<int>& drawingCounts = Container->Extension_GetDrawingCounts();
        std::vector<ssGUI::DrawingProperty>& drawingProperties = Container->Extension_GetDrawingProperties();

        //Top
        drawingVerticies.push_back(drawPosition + glm::ivec2(0, -width));
        drawingVerticies.push_back(drawPosition + glm::ivec2(Container->GetSize().x, -width));
        drawingVerticies.push_back(drawPosition + glm::ivec2(Container->GetSize().x, 0));
        drawingVerticies.push_back(drawPosition + glm::ivec2(0, 0));

        drawingColours.push_back(colour);
        drawingColours.push_back(colour);
        drawingColours.push_back(colour);
        drawingColours.push_back(colour);

        drawingUVs.push_back(glm::ivec2());
        drawingUVs.push_back(glm::ivec2());
        drawingUVs.push_back(glm::ivec2());
        drawingUVs.push_back(glm::ivec2());

        drawingCounts.push_back(4);
        drawingProperties.push_back(ssGUI::DrawingProperty());

        //Right
        drawingVerticies.push_back(drawPosition + glm::ivec2(Container->GetSize().x, -width));
        drawingVerticies.push_back(drawPosition + glm::ivec2(Container->GetSize().x + width, -width));
        drawingVerticies.push_back(drawPosition + glm::ivec2(Container->GetSize().x + width, Container->GetSize().y + width));
        drawingVerticies.push_back(drawPosition + glm::ivec2(Container->GetSize().x, Container->GetSize().y + width));

        drawingColours.push_back(colour);
        drawingColours.push_back(colour);
        drawingColours.push_back(colour);
        drawingColours.push_back(colour);

        drawingUVs.push_back(glm::ivec2());
        drawingUVs.push_back(glm::ivec2());
        drawingUVs.push_back(glm::ivec2());
        drawingUVs.push_back(glm::ivec2());

        drawingCounts.push_back(4);
        drawingProperties.push_back(ssGUI::DrawingProperty());

        //Bottom
        drawingVerticies.push_back(drawPosition + glm::ivec2(0, Container->GetSize().y));
        drawingVerticies.push_back(drawPosition + glm::ivec2(Container->GetSize().x, Container->GetSize().y));
        drawingVerticies.push_back(drawPosition + glm::ivec2(Container->GetSize().x, Container->GetSize().y + width));
        drawingVerticies.push_back(drawPosition + glm::ivec2(0, Container->GetSize().y + width));

        drawingColours.push_back(colour);
        drawingColours.push_back(colour);
        drawingColours.push_back(colour);
        drawingColours.push_back(colour);

        drawingUVs.push_back(glm::ivec2());
        drawingUVs.push_back(glm::ivec2());
        drawingUVs.push_back(glm::ivec2());
        drawingUVs.push_back(glm::ivec2());

        drawingCounts.push_back(4);
        drawingProperties.push_back(ssGUI::DrawingProperty());

        //Left
        drawingVerticies.push_back(drawPosition + glm::ivec2(-width, -width));
        drawingVerticies.push_back(drawPosition + glm::ivec2(0, -width));
        drawingVerticies.push_back(drawPosition + glm::ivec2(0, Container->GetSize().y + width));
        drawingVerticies.push_back(drawPosition + glm::ivec2(-width, Container->GetSize().y + width));

        drawingColours.push_back(colour);
        drawingColours.push_back(colour);
        drawingColours.push_back(colour);
        drawingColours.push_back(colour);

        drawingUVs.push_back(glm::ivec2());
        drawingUVs.push_back(glm::ivec2());
        drawingUVs.push_back(glm::ivec2());
        drawingUVs.push_back(glm::ivec2());

        drawingCounts.push_back(4);
        drawingProperties.push_back(ssGUI::DrawingProperty()); 
    }

    void Border::SetEnabled(bool enabled)
    {
        Enabled = enabled;
    }

    bool Border::IsEnabled() const
    {
        return Enabled;
    }

    void Border::Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        //Do nothing
    }
    
    void Border::Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {
        if(IsPreRender || Container == nullptr || Container->GetType() == ssGUI::Enums::GUIObjectType::MAIN_WINDOW || !Enabled)
            return;
        
        DrawBorder(drawingInterface, mainWindowP, mainWindowPositionOffset);
    }
    
    std::string Border::GetExtensionName()
    {
        return EXTENSION_NAME;
    }
    
    void Border::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;
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
    
    Extension* Border::Clone(ssGUI::GUIObject* newContainer)
    {
        Border* temp = new Border(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
}