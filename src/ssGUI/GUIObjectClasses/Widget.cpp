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

    void Widget::Delete()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
        ssGUI::BaseGUIObject::Delete();
    }

    void Widget::Internal_Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        FUNC_DEBUG_ENTRY();
        
        if(!IsVisible())
        {
            FUNC_DEBUG_EXIT();
            return;
        }
        
        if(Redraw)
        {
            DisableRedrawObjectRequest();

            for(auto extension : ExtensionsDrawOrder)
                Extensions.at(extension)->Internal_Draw(true, drawingInterface, mainWindow, mainWindowPositionOffset);
            
            UpdateGUIObjectVertexAndShapeIndex();

            ConstructRenderInfo();

            for(auto extension : ExtensionsDrawOrder)
                Extensions.at(extension)->Internal_Draw(false, drawingInterface, mainWindow, mainWindowPositionOffset);

            EnableRedrawObjectRequest();

            drawingInterface->DrawEntities(DrawingVerticies, DrawingUVs, DrawingColours, DrawingCounts, DrawingProperties);
            CacheRendering();
            DrawingVerticies.clear();
            DrawingUVs.clear();
            DrawingColours.clear();
            DrawingCounts.clear();
            DrawingProperties.clear();
            Redraw = false;
        }
        else
            drawingInterface->DrawEntities(LastDrawingVerticies, LastDrawingUVs, LastDrawingColours, LastDrawingCounts, LastDrawingProperties);

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
        
        for(auto extension : ExtensionsUpdateOrder)
        {
            //Guard against extension being deleted by other extensions
            if(!IsExtensionExist(extension))
                continue;

            Extensions.at(extension)->Internal_Update(true, inputInterface, globalInputStatus, windowInputStatus, mainWindow);
        }

        //It will only block when BlockInput flag is true OR is interactable
        if(!IsBlockInput() || IsInteractable())
            goto endOfUpdate;

        //Mouse Input blocking
        if(windowInputStatus.MouseInputBlocked || globalInputStatus.MouseInputBlocked)
            goto endOfUpdate;

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
        }

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

    GUIObject* Widget::Clone(bool cloneChildren)
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
