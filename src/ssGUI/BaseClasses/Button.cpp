#include "ssGUI/BaseClasses/Button.hpp"
#include "ssGUI/BaseClasses/MainWindow.hpp" //For getting mouse position

namespace ssGUI
{
    Button::Button(Button const& other) : Widget(other)
    {
        CurrentState = other.GetButtonState();
        StateChangedEventCallback = nullptr;

        if(other.IsEventCallbackExist(ssGUI::EventCallbacks::ButtonStateChangedEventCallback::EVENT_NAME))
        {
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
        }
    }

    void Button::SetButtonState(ssGUI::Enums::ButtonState state)
    {
        if(CurrentState == state)
            return;
        
        CurrentState = state;

        //TODO : Set it as optional
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
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
    }

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

    void Button::Delete()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
        ssGUI::BaseGUIObject::Delete();
    }

    void Button::SetInteractable(bool interactable)
    {
        if(interactable)
            CurrentState = ssGUI::Enums::ButtonState::NORMAL;
        else
            CurrentState = ssGUI::Enums::ButtonState::DISABLED;

        ssGUI::Widget::SetInteractable(interactable);
    }

    void Button::Internal_Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {
        FUNC_DEBUG_ENTRY();
        
        for (auto extension : Extensions)
            extension.second->Internal_Draw(true, drawingInterface, mainWindowP, mainWindowPositionOffset);

        if (!IsVisible())
        {
            FUNC_DEBUG_EXIT();
            return;
        }

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
            extension.second->Internal_Draw(false, drawingInterface, mainWindowP, mainWindowPositionOffset);

        drawingInterface->DrawEntities(DrawingVerticies, DrawingUVs, DrawingColours, DrawingCounts, DrawingProperties);
        DrawingVerticies.clear();
        DrawingUVs.clear();
        DrawingColours.clear();
        DrawingCounts.clear();
        DrawingProperties.clear();

        FUNC_DEBUG_EXIT();
    }

    void Button::Internal_Update(ssGUI::Backend::BackendSystemInputInterface *inputInterface, ssGUI::InputStatus &globalInputStatus, ssGUI::InputStatus &windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();

        //If it is not visible, don't even update/draw it
        if (!IsVisible())
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        for (auto extension : Extensions)
            extension.second->Update(true, inputInterface, globalInputStatus, windowInputStatus, mainWindow);

        //On mouse down
        glm::ivec2 currentMousePos = inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow));
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
            extension.second->Update(false, inputInterface, globalInputStatus, windowInputStatus, mainWindow);

        FUNC_DEBUG_EXIT();
    }

    GUIObject* Button::Clone(bool cloneChildren)
    {
        Button* temp = new Button(*this);
        CloneExtensionsAndEventCallbacks(temp);   
        
        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
                return nullptr;
        }

        return temp;
    }
}

