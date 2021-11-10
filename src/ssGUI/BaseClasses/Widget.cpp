#include "ssGUI/BaseClasses/Widget.hpp"

namespace ssGUI
{
    Widget::Widget() :  Focused(false), Interactable(true), BlockInput(true) 
    {}

    Widget::~Widget()
    {}

    bool Widget::IsFocused() const
    {
        return Focused;
    }

    void Widget::SetFocus(bool focus)
    {
        Focused = focus;
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
        return ssGUI::Enums::GUIObjectType::BASE_WIDGET;
    }

    void Widget::Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {
        if(!IsVisible())
            return;
        
        for(auto extension : Extensions)
            extension->Draw(true, drawingInterface, mainWindowP, mainWindowPositionOffset);
        
        //Draw background by default
        glm::ivec2 drawPosition = GetGlobalPosition();

        //TODO: Some optimisation maybe possible
        DrawingVerticies.push_back(drawPosition);
        DrawingUVs.push_back(glm::ivec2());
        DrawingColours.push_back(GetBackgroundColour());

        DrawingVerticies.push_back(drawPosition + glm::ivec2(GetSize().x, 0));
        DrawingUVs.push_back(glm::ivec2());
        DrawingColours.push_back(GetBackgroundColour());

        DrawingVerticies.push_back(drawPosition + glm::ivec2(GetSize().x, GetSize().y));
        DrawingUVs.push_back(glm::ivec2());
        DrawingColours.push_back(GetBackgroundColour());

        DrawingVerticies.push_back(drawPosition + glm::ivec2(0, GetSize().y));
        DrawingUVs.push_back(glm::ivec2());
        DrawingColours.push_back(GetBackgroundColour());

        DrawingCounts.push_back(4);
        DrawingProperties.push_back(ssGUI::DrawingProperty());

        for(auto extension : Extensions)
            extension->Draw(false, drawingInterface, mainWindowP, mainWindowPositionOffset);

        drawingInterface->DrawEntities(DrawingVerticies, DrawingUVs, DrawingColours, DrawingCounts, DrawingProperties);
        DrawingVerticies.clear();
        DrawingUVs.clear();
        DrawingColours.clear();
        DrawingCounts.clear();
        DrawingProperties.clear();
    }

    void Widget::Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        //If it is not visible, don't even update/draw it
        if(!IsVisible())
            return;
        
        for(auto extension : Extensions)
            extension->Update(true, inputInterface, globalInputStatus, windowInputStatus, mainWindow);

        //It will only block when BlockInput flag is true OR is interactable
        if(!IsBlockInput() || IsInteractable())
            goto endOfUpdate;

        //Mouse Input blocking
        if(windowInputStatus.MouseInputBlocked || globalInputStatus.MouseInputBlocked)
            goto endOfUpdate;

        //On mouse down
        {
        glm::ivec2 currentMousePos = inputInterface->GetCurrentMousePosition(mainWindow);
        if(inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) && !inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::LEFT))
        {
            bool mouseInWindowBoundX = false;
            bool mouseInWindowBoundY = false;
            
            if(currentMousePos.x >= GetGlobalPosition().x && currentMousePos.x <= GetGlobalPosition().x + GetSize().x)
                mouseInWindowBoundX = true;

            if(currentMousePos.y >= GetGlobalPosition().y && currentMousePos.y <= GetGlobalPosition().y + GetSize().y)
                mouseInWindowBoundY = true;
            
            //Input blocking
            if(mouseInWindowBoundX && mouseInWindowBoundY)
                windowInputStatus.MouseInputBlocked = true;
        }
        }

        endOfUpdate:;

        for(auto extension : Extensions)
            extension->Update(false, inputInterface, globalInputStatus, windowInputStatus, mainWindow);
    }

    GUIObject* Widget::Clone(std::vector<GUIObject*>& originalObjs, bool cloneChildren)
    {
        Widget* temp = new Widget(*this);

        for(auto extension : Extensions)
        {
            if(!temp->IsExtensionExist(extension->GetExtensionName()))
                temp->AddExtension(extension->Clone(this));
        }

        for(auto eventCallback : EventCallbacks)
        {
            std::vector<ssGUI::GUIObject*> tempVec = std::vector<ssGUI::GUIObject*>();

            if(!temp->IsEventCallbackExist(eventCallback->GetEventCallbackName()))
                temp->AddEventCallback(eventCallback->Clone(this, originalObjs, tempVec));
        }

        return temp;
    }
}
