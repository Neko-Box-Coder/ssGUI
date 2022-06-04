#include "ssGUI/GUIObjectClasses/Widget.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

namespace ssGUI
{
    void Widget::ConstructRenderInfo()
    {
        //Internal_Draw background by default
        glm::vec2 drawPosition = GetGlobalPosition();

        //TODO: Some optimisation maybe possible
        DrawingVerticies.push_back(drawPosition);
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingVerticies.push_back(drawPosition + glm::vec2(GetSize().x, 0));
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingVerticies.push_back(drawPosition + glm::vec2(GetSize().x, GetSize().y));
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingVerticies.push_back(drawPosition + glm::vec2(0, GetSize().y));
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingCounts.push_back(4);
        DrawingProperties.push_back(ssGUI::DrawingProperty());
    }

    void Widget::MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, 
                ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        //Mouse Input blocking
        if(windowInputStatus.MouseInputBlocked || globalInputStatus.MouseInputBlocked)
            return;

        //Mouse Input blocking
        {
            glm::ivec2 currentMousePos = inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow));

            bool mouseInWindowBoundX = false;
            bool mouseInWindowBoundY = false;
            
            if(currentMousePos.x >= GetGlobalPosition().x && currentMousePos.x <= GetGlobalPosition().x + GetSize().x)
                mouseInWindowBoundX = true;

            if(currentMousePos.y >= GetGlobalPosition().y && currentMousePos.y <= GetGlobalPosition().y + GetSize().y)
                mouseInWindowBoundY = true;
            
            //Input blocking
            if(mouseInWindowBoundX && mouseInWindowBoundY)
                windowInputStatus.MouseInputBlocked = true;

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
    
    Widget::Widget() : Interactable(true), BlockInput(true) 
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

    void Widget::Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();
        
        //If it is not visible, don't even update/draw it
        if(!IsVisible())
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        for(auto extension : ExtensionsUpdateOrder)
        {
            //Guard against extension being deleted by other extensions
            if(!IsExtensionExist(extension))
                continue;

            Extensions.at(extension)->Internal_Update(true, inputInterface, globalInputStatus, windowInputStatus, mainWindow);
        }

        //It will only block when BlockInput flag is true
        if(!IsBlockInput())
            goto endOfUpdate;

        CheckRightClickMenu(inputInterface, globalInputStatus, windowInputStatus, mainWindow);
        MainLogic(inputInterface, globalInputStatus, windowInputStatus, mainWindow);

        endOfUpdate:;
        for(auto extension : ExtensionsUpdateOrder)
        {
            //Guard against extension being deleted by other extensions
            if(!IsExtensionExist(extension))
                continue;

            Extensions.at(extension)->Internal_Update(false, inputInterface, globalInputStatus, windowInputStatus, mainWindow);
        }
        
        //Check position different for redraw
        if(GetGlobalPosition() != LastGlobalPosition)
            RedrawObject();

        LastGlobalPosition = GetGlobalPosition();

        FUNC_DEBUG_EXIT();
    }

    Widget* Widget::Clone(bool cloneChildren)
    {
        FUNC_DEBUG_ENTRY();
        Widget* temp = new Widget(*this);
        CloneExtensionsAndEventCallbacks(temp);   
        
        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
            {
                FUNC_DEBUG_EXIT();
                return nullptr;
            }
        }

        FUNC_DEBUG_EXIT();
        return temp;
    }
}
