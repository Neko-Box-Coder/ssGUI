#include "ssGUI/BaseClasses/Widget.hpp"

namespace ssGUI
{
    Widget::Widget() :  Focused(false), Interactable(true), BlockInput(true) 
    {}

    Widget::~Widget()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
    }

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

    void Widget::Internal_Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {
        FUNC_DEBUG_ENTRY();
        
        if(!IsVisible())
        {
            FUNC_DEBUG_EXIT();
            return;
        }
        
        for(auto extension : Extensions)
            extension.second->Internal_Draw(true, drawingInterface, mainWindowP, mainWindowPositionOffset);
        
        //Internal_Draw background by default
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
            extension.second->Internal_Draw(false, drawingInterface, mainWindowP, mainWindowPositionOffset);

        drawingInterface->DrawEntities(DrawingVerticies, DrawingUVs, DrawingColours, DrawingCounts, DrawingProperties);
        DrawingVerticies.clear();
        DrawingUVs.clear();
        DrawingColours.clear();
        DrawingCounts.clear();
        DrawingProperties.clear();

        FUNC_DEBUG_EXIT();
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
        
        for(auto extension : Extensions)
            extension.second->Update(true, inputInterface, globalInputStatus, windowInputStatus, mainWindow);

        //It will only block when BlockInput flag is true OR is interactable
        if(!IsBlockInput() || IsInteractable())
            goto endOfUpdate;

        //Mouse Input blocking
        if(windowInputStatus.MouseInputBlocked || globalInputStatus.MouseInputBlocked)
            goto endOfUpdate;

        //Mouse Input blocking
        {
        glm::ivec2 currentMousePos = inputInterface->GetCurrentMousePosition(mainWindow);

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

        endOfUpdate:;

        for(auto extension : Extensions)
            extension.second->Update(false, inputInterface, globalInputStatus, windowInputStatus, mainWindow);

        DEBUG_LINE("object "<<this<<" checking validity");
        Internal_GetObjectsReferences()->CheckObjectsReferencesValidity();

        FUNC_DEBUG_EXIT();
    }

    GUIObject* Widget::Clone(bool cloneChildren)
    {
        Widget* temp = new Widget(*this);

        // for(auto extension : Extensions)
        // {
        //     if(!temp->IsExtensionExist(extension.second->GetExtensionName()))
        //         temp->AddExtension(extension.second->Clone(this));
        // }

        // for(auto eventCallback : EventCallbacks)
        // {
        //     std::vector<ssGUI::GUIObject*> tempVec = std::vector<ssGUI::GUIObject*>();

        //     if(!temp->IsEventCallbackExist(eventCallback.second->GetEventCallbackName()))
        //         temp->AddEventCallback(eventCallback.second->Clone(this, originalObjs, tempVec));
        // }

        return temp;
    }
}
