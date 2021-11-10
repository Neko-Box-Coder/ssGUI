#include "ssGUI/BaseClasses/Button.hpp"

namespace ssGUI
{
    Button::Button(Button const& other) : Widget(other)
    {
        SetButtonState(other.GetButtonState());
        Button();
    }

    void Button::SetButtonState(ssGUI::Enums::ButtonState state)
    {
        CurrentState = state;
        StateChangedEventCallback->Notify(static_cast<ssGUI::GUIObject*>(this));
    }

    Button::Button() : CurrentState(ssGUI::Enums::ButtonState::NORMAL), StateChangedEventCallback(nullptr)
    {
        SetBackgroundColour(glm::u8vec4(127,127,127,255)); //Gray background colour for button (For now)
        StateChangedEventCallback = new ssGUI::EventCallbacks::ButtonStateChangedEventCallback();
        StateChangedEventCallback->AddEventListener(
            [](ssGUI::GUIObject* obj)
            {
                ssGUI::Button* btn = static_cast<ssGUI::Button*>(obj);
                glm::u8vec4 bgcolor = btn->GetBackgroundColour();
                switch(btn->GetButtonState())
                {
                    case ssGUI::Enums::ButtonState::NORMAL:
                        bgcolor.a = 255;
                        btn->SetBackgroundColour(bgcolor);
                        break;
                    case ssGUI::Enums::ButtonState::HOVER:
                        bgcolor.a = 200;
                        btn->SetBackgroundColour(bgcolor);
                        break;
                    case ssGUI::Enums::ButtonState::CLICKED:
                    case ssGUI::Enums::ButtonState::ON_CLICK:
                    case ssGUI::Enums::ButtonState::CLICKING:
                        bgcolor.a = 100;
                        btn->SetBackgroundColour(bgcolor);
                        break;
                    case ssGUI::Enums::ButtonState::DISABLED:
                        bgcolor.a = 50;
                        btn->SetBackgroundColour(bgcolor);
                        break;
                }
            }
        );
        
        AddEventCallback(StateChangedEventCallback);
        AddExtension(new ssGUI::Extensions::Border());
    }

    Button::~Button()
    {}

    void Button::NotifyButtonEventCallbackManually()
    {   
        SetButtonState(GetButtonState());
    }

    ssGUI::Enums::ButtonState Button::GetButtonState() const
    {
        return CurrentState;
    }

    //Overriding widget
    ssGUI::Enums::GUIObjectType Button::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::BUTTON | ssGUI::Enums::GUIObjectType::BASE_WIDGET;
    }

    void Button::SetInteractable(bool interactable)
    {
        if(interactable)
            CurrentState = ssGUI::Enums::ButtonState::NORMAL;
        else
            CurrentState = ssGUI::Enums::ButtonState::DISABLED;

        ssGUI::Widget::SetInteractable(interactable);
    }

    void Button::Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {
        for (auto extension : Extensions)
            extension->Draw(true, drawingInterface, mainWindowP, mainWindowPositionOffset);

        if (!IsVisible())
            return;

        glm::ivec2 drawPosition = GetGlobalPosition();

        //Background
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

        for (auto extension : Extensions)
            extension->Draw(false, drawingInterface, mainWindowP, mainWindowPositionOffset);

        drawingInterface->DrawEntities(DrawingVerticies, DrawingUVs, DrawingColours, DrawingCounts, DrawingProperties);
        DrawingVerticies.clear();
        DrawingUVs.clear();
        DrawingColours.clear();
        DrawingCounts.clear();
        DrawingProperties.clear();
    }

    void Button::Internal_Update(ssGUI::Backend::BackendSystemInputInterface *inputInterface, ssGUI::InputStatus &globalInputStatus, ssGUI::InputStatus &windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        //If it is not visible, don't even update/draw it
        if (!IsVisible())
            return;

        for (auto extension : Extensions)
            extension->Update(true, inputInterface, globalInputStatus, windowInputStatus, mainWindow);

        //On mouse down
        glm::ivec2 currentMousePos = inputInterface->GetCurrentMousePosition(mainWindow);
        if (inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) && !inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::LEFT))
        {
            //User pressing down on button
            if (currentMousePos.x >= GetGlobalPosition().x && currentMousePos.x <= GetGlobalPosition().x + GetSize().x &&
                currentMousePos.y >= GetGlobalPosition().y && currentMousePos.y <= GetGlobalPosition().y + GetSize().y)
            {
                globalInputStatus.MouseInputBlocked = true;
                SetButtonState(ssGUI::Enums::ButtonState::ON_CLICK);
            }
        }
        //On mouse hold
        else if (inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) &&
                (GetButtonState() == ssGUI::Enums::ButtonState::ON_CLICK ||
                GetButtonState() == ssGUI::Enums::ButtonState::CLICKING))
        {
            globalInputStatus.MouseInputBlocked = true;
            if (GetButtonState() == ssGUI::Enums::ButtonState::ON_CLICK)
                SetButtonState(ssGUI::Enums::ButtonState::CLICKING);
        }
        //On mouse up
        else if (!inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) && inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::LEFT) &&
                (GetButtonState() == ssGUI::Enums::ButtonState::ON_CLICK ||
                GetButtonState() == ssGUI::Enums::ButtonState::CLICKING))
        {
            SetButtonState(ssGUI::Enums::ButtonState::CLICKED);
        }
        //Otherwise check normal/hover/disabled state
        else
        {
            if(IsInteractable())
            {
                if (currentMousePos.x >= GetGlobalPosition().x && currentMousePos.x <= GetGlobalPosition().x + GetSize().x &&
                    currentMousePos.y >= GetGlobalPosition().y && currentMousePos.y <= GetGlobalPosition().y + GetSize().y)
                {
                    SetButtonState(ssGUI::Enums::ButtonState::HOVER);
                }
                else
                    SetButtonState(ssGUI::Enums::ButtonState::NORMAL);
            }
            else
            {
                if (GetButtonState() != ssGUI::Enums::ButtonState::DISABLED)
                    SetButtonState(ssGUI::Enums::ButtonState::DISABLED);
            }
        }

        endUpdate:;
        
        for (auto extension : Extensions)
            extension->Update(false, inputInterface, globalInputStatus, windowInputStatus, mainWindow);
    }

    GUIObject* Button::Clone(std::vector<GUIObject*>& originalObjs, bool cloneChildren)
    {
        Button* temp = new Button(*this);

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

