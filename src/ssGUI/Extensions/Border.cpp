#include "ssGUI/Extensions/Border.hpp"


namespace ssGUI::Extensions
{
    Border::Border()
    {}
    
    Border::~Border()
    {}

    Border::Border(Border const& other)
    {
        Container = nullptr;
        Enabled = other.IsEnabled();
        BorderColor = other.GetBorderColor();
        BorderWidth = other.GetBorderWidth();
        BorderSides = other.BorderSides;
    }

    void Border::DrawBorder()
    {        
        FUNC_DEBUG_ENTRY();
        
        glm::vec2 drawPosition = Container->GetGlobalPosition();
        
        int width = GetBorderWidth();
        glm::u8vec4 colour = GetBorderColor();

        std::vector<glm::vec2>& drawingVerticies = Container->Extension_GetDrawingVertices();
        std::vector<glm::vec2>& drawingUVs = Container->Extension_GetDrawingUVs();
        std::vector<glm::u8vec4>& drawingColours = Container->Extension_GetDrawingColours();
        std::vector<int>& drawingCounts = Container->Extension_GetDrawingCounts();
        std::vector<ssGUI::DrawingProperty>& drawingProperties = Container->Extension_GetDrawingProperties();

        //Top
        if(IsBorderTopShowing())
        {
            drawingVerticies.push_back(drawPosition + glm::vec2(0,                      0));
            drawingVerticies.push_back(drawPosition + glm::vec2(Container->GetSize().x, 0));
            drawingVerticies.push_back(drawPosition + glm::vec2(Container->GetSize().x, width));
            drawingVerticies.push_back(drawPosition + glm::vec2(0,                      width));

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
        }

        //Right
        if(IsBorderRightShowing())
        {
            drawingVerticies.push_back(drawPosition + glm::vec2(Container->GetSize().x - width, (IsBorderTopShowing() ? width : 0)));
            drawingVerticies.push_back(drawPosition + glm::vec2(Container->GetSize().x,         (IsBorderTopShowing() ? width : 0)));
            drawingVerticies.push_back(drawPosition + glm::vec2(Container->GetSize().x,         Container->GetSize().y + (IsBorderBottomShowing() ? -width : 0)));
            drawingVerticies.push_back(drawPosition + glm::vec2(Container->GetSize().x - width, Container->GetSize().y + (IsBorderBottomShowing() ? -width : 0)));

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
        }

        //Bottom
        if(IsBorderBottomShowing())
        {
            drawingVerticies.push_back(drawPosition + glm::vec2(0,                      Container->GetSize().y - width));
            drawingVerticies.push_back(drawPosition + glm::vec2(Container->GetSize().x, Container->GetSize().y - width));
            drawingVerticies.push_back(drawPosition + glm::vec2(Container->GetSize().x, Container->GetSize().y));
            drawingVerticies.push_back(drawPosition + glm::vec2(0,                      Container->GetSize().y));

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
        }

        //Left
        if(IsBorderLeftShowing())
        {
            drawingVerticies.push_back(drawPosition + glm::vec2(0,      IsBorderTopShowing() ? width : 0));
            drawingVerticies.push_back(drawPosition + glm::vec2(width,  IsBorderTopShowing() ? width : 0));
            drawingVerticies.push_back(drawPosition + glm::vec2(width,  Container->GetSize().y + (IsBorderBottomShowing() ? -width : 0)));
            drawingVerticies.push_back(drawPosition + glm::vec2(0,      Container->GetSize().y + (IsBorderBottomShowing() ? -width : 0)));

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
        }

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
    
    const std::string Border::EXTENSION_NAME = "Border";
    
    glm::u8vec4 Border::GetBorderColor() const
    {
        return BorderColor;
    }
    
    void Border::SetBorderColor(glm::u8vec4 colour)
    {
        BorderColor = colour;

        if(Container != nullptr)
            Container->RedrawObject();
    }

    float Border::GetBorderWidth() const
    {
        return BorderWidth;
    }
    
    void Border::SetBorderWidth(float width)
    {
        BorderWidth = width;

        if(Container != nullptr)
            Container->RedrawObject();
    }

    void Border::ShowBorderLeft(bool show)
    {
        BorderSides = show ? BorderSides | 1 << 0 : BorderSides & (1 << 1 | 1 << 2 | 1 << 3);
    }

    void Border::ShowBorderTop(bool show)
    {
        BorderSides = show ? BorderSides | 1 << 1 : BorderSides & (1 << 0 | 1 << 2 | 1 << 3);
    }

    void Border::ShowBorderRight(bool show)
    {
        BorderSides = show ? BorderSides | 1 << 2 : BorderSides & (1 << 0 | 1 << 1 | 1 << 3);
    }

    void Border::ShowBorderBottom(bool show)
    {
        BorderSides = show ? BorderSides | 1 << 3 : BorderSides & (1 << 0 | 1 << 1 | 1 << 2);
    }

    bool Border::IsBorderLeftShowing() const
    {
        return ((BorderSides & (1 << 0)) > 0);
    }

    bool Border::IsBorderTopShowing() const
    {
        return ((BorderSides & (1 << 1)) > 0);
    }

    bool Border::IsBorderRightShowing() const
    {
        return ((BorderSides & (1 << 2)) > 0);
    }

    bool Border::IsBorderBottomShowing() const
    {
        return ((BorderSides & (1 << 3)) > 0);
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

    void Border::Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, ssGUI::GUIObject* mainWindow)
    {
        //Do nothing
    }
    
    void Border::Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        if(isPreRender || Container == nullptr || Container->GetType() == ssGUI::Enums::GUIObjectType::MAIN_WINDOW || !Enabled)
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
        Enabled = border->IsEnabled();
        BorderColor = border->GetBorderColor();
        BorderWidth = border->GetBorderWidth();
        BorderSides = border->BorderSides;
    }

    ObjectsReferences* Border::Internal_GetObjectsReferences()
    {
        return nullptr;
    }

    Border* Border::Clone(ssGUI::GUIObject* newContainer)
    {
        Border* temp = new Border(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
}