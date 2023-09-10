#include "ssGUI/Extensions/Border.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Extensions
{
    Border::Border() :  Container(nullptr),
                        Enabled(true),
                        BorderColor(0, 0, 0, 255),
                        BorderWidth(1),
                        BorderSides(15)
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
        ssGUI_LOG_FUNC();
        
        glm::vec2 drawPosition = Container->GetGlobalPosition();
        
        int width = GetBorderWidth();
        glm::u8vec4 colour = GetBorderColor();

        std::vector<ssGUI::DrawingEntity>& drawingEntities = Container->Extension_GetDrawingEntities();

        //Top
        if(IsBorderTopShowing())
        {
            ssGUI::DrawingEntity entity;
            entity.EntityName = BORDER_TOP_SHAPE_NAME;
            entity.Vertices.push_back(drawPosition + glm::vec2(0,                      0));
            entity.Vertices.push_back(drawPosition + glm::vec2(Container->GetSize().x, 0));
            entity.Vertices.push_back(drawPosition + glm::vec2(Container->GetSize().x, width));
            entity.Vertices.push_back(drawPosition + glm::vec2(0,                      width));
        
            entity.Colors.push_back(colour);
            entity.Colors.push_back(colour);
            entity.Colors.push_back(colour);
            entity.Colors.push_back(colour);

            drawingEntities.push_back(entity);
        }

        //Right
        if(IsBorderRightShowing())
        {
            ssGUI::DrawingEntity entity;
            entity.EntityName = BORDER_RIGHT_SHAPE_NAME;
            entity.Vertices.push_back(drawPosition + glm::vec2(Container->GetSize().x - width, (IsBorderTopShowing() ? width : 0)));
            entity.Vertices.push_back(drawPosition + glm::vec2(Container->GetSize().x,         (IsBorderTopShowing() ? width : 0)));
            entity.Vertices.push_back(drawPosition + glm::vec2(Container->GetSize().x,         Container->GetSize().y + (IsBorderBottomShowing() ? -width : 0)));
            entity.Vertices.push_back(drawPosition + glm::vec2(Container->GetSize().x - width, Container->GetSize().y + (IsBorderBottomShowing() ? -width : 0)));

            entity.Colors.push_back(colour);
            entity.Colors.push_back(colour);
            entity.Colors.push_back(colour);
            entity.Colors.push_back(colour);

            drawingEntities.push_back(entity);
        }

        //Bottom
        if(IsBorderBottomShowing())
        {
            ssGUI::DrawingEntity entity;
            entity.EntityName = BORDER_BOTTOM_SHAPE_NAME;
            entity.Vertices.push_back(drawPosition + glm::vec2(0,                      Container->GetSize().y - width));
            entity.Vertices.push_back(drawPosition + glm::vec2(Container->GetSize().x, Container->GetSize().y - width));
            entity.Vertices.push_back(drawPosition + glm::vec2(Container->GetSize().x, Container->GetSize().y));
            entity.Vertices.push_back(drawPosition + glm::vec2(0,                      Container->GetSize().y));

            entity.Colors.push_back(colour);
            entity.Colors.push_back(colour);
            entity.Colors.push_back(colour);
            entity.Colors.push_back(colour);

            drawingEntities.push_back(entity);
        }

        //Left
        if(IsBorderLeftShowing())
        {
            ssGUI::DrawingEntity entity;
            entity.EntityName = BORDER_LEFT_SHAPE_NAME;
            entity.Vertices.push_back(drawPosition + glm::vec2(0,      IsBorderTopShowing() ? width : 0));
            entity.Vertices.push_back(drawPosition + glm::vec2(width,  IsBorderTopShowing() ? width : 0));
            entity.Vertices.push_back(drawPosition + glm::vec2(width,  Container->GetSize().y + (IsBorderBottomShowing() ? -width : 0)));
            entity.Vertices.push_back(drawPosition + glm::vec2(0,      Container->GetSize().y + (IsBorderBottomShowing() ? -width : 0)));

            entity.Colors.push_back(colour);
            entity.Colors.push_back(colour);
            entity.Colors.push_back(colour);
            entity.Colors.push_back(colour);

            drawingEntities.push_back(entity); 
        }
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

    void Border::Internal_Update(   bool isPreUpdate, 
                                    ssGUI::Backend::BackendSystemInputInterface* inputInterface, 
                                    ssGUI::InputStatus& currentInputStatus, 
                                    const ssGUI::InputStatus& lastInputStatus, 
                                    ssGUI::GUIObject* mainWindow)
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

    Border* Border::Clone()
    {
        Border* temp = new Border(*this);
        return temp;
    }
}

}