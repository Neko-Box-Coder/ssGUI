#include "ssGUI/GUIObjectClasses/Widget.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{
    Widget::Widget(Widget const& other) :   GUIObject(other),
                                            Interactable(other.Interactable),
                                            BlockInput(other.BlockInput)
    {
    }

    void Widget::ConstructRenderInfo()
    {
        //Internal_Draw background by default
        glm::vec2 drawPosition = GetGlobalPosition();

        ssGUI::DrawingEntity backgroundEntitiy;
        
        backgroundEntitiy.Vertices.push_back(drawPosition);
        backgroundEntitiy.Vertices.push_back(drawPosition + glm::vec2(GetSize().x, 0));
        backgroundEntitiy.Vertices.push_back(drawPosition + glm::vec2(GetSize().x, GetSize().y));
        backgroundEntitiy.Vertices.push_back(drawPosition + glm::vec2(0, GetSize().y));
        
        backgroundEntitiy.Colors.push_back(GetBackgroundColor());
        backgroundEntitiy.Colors.push_back(GetBackgroundColor());
        backgroundEntitiy.Colors.push_back(GetBackgroundColor());
        backgroundEntitiy.Colors.push_back(GetBackgroundColor());
        
        backgroundEntitiy.EntityName = GUI_OBJECT_BG_SHAPE_NAME;
        
        DrawingEntities.push_back(backgroundEntitiy);
    }

    void Widget::MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                            ssGUI::GUIObject* mainWindow)
    {
        //Mouse Input blocking
        if(inputStatus.MouseInputBlockedObject != nullptr)
            return;

        //Mouse Input blocking
        if(IsBlockInput())
        {
            glm::ivec2 currentMousePos = inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)->GetBackendWindowInterface());

            bool mouseInWindowBoundX = false;
            bool mouseInWindowBoundY = false;
            
            if(currentMousePos.x >= GetGlobalPosition().x && currentMousePos.x <= GetGlobalPosition().x + GetSize().x)
                mouseInWindowBoundX = true;

            if(currentMousePos.y >= GetGlobalPosition().y && currentMousePos.y <= GetGlobalPosition().y + GetSize().y)
                mouseInWindowBoundY = true;
            
            //Input blocking
            if(mouseInWindowBoundX && mouseInWindowBoundY)
                inputStatus.MouseInputBlockedObject = this;

            //If mouse click on this, set focus
            if(mouseInWindowBoundX && mouseInWindowBoundY &&
                ((inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) && !inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::LEFT)) ||
                (inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::MIDDLE) && !inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::MIDDLE)) ||
                (inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::RIGHT) && !inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::RIGHT))))
            {
                SetFocus(true);    
            }
        }
    }
    
    Widget::Widget() :  Interactable(true),
                        BlockInput(true)
    {}

    Widget::~Widget()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
    }

    bool Widget::IsInteractable() const
    {
        return Interactable;
    }

    void Widget::SetInteractable(bool interactable)
    {
        Interactable = interactable;
    }

    bool Widget::IsBlockInput() const
    {
        return BlockInput;
    }
    
    void Widget::SetBlockInput(bool blockInput)
    {
        BlockInput = blockInput;
    }

    ssGUI::Enums::GUIObjectType Widget::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::WIDGET;
    }

    Widget* Widget::Clone(bool cloneChildren)
    {
        ssGUI_LOG_FUNC();
        Widget* temp = new Widget(*this);
        CloneExtensionsAndEventCallbacks(temp);   
        
        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
                return nullptr;
        }

        return temp;
    }
}
