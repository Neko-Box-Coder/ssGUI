#include "ssGUI/GUIObjectClasses/Button.hpp"
#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

namespace ssGUI
{
    Button::Button(Button const& other) : Widget(other)
    {
        CurrentState = other.GetButtonState();
        ButtonColor = other.GetButtonColor();
    }

    void Button::ConstructRenderInfo()
    {
        glm::vec2 drawPosition = GetGlobalPosition();

        //Background
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

    void Button::SetButtonState(ssGUI::Enums::ButtonState state)
    {
        if(CurrentState == state)
            return;
        
        CurrentState = state;

        if(IsAnyEventCallbackExist<ssGUI::EventCallbacks::ButtonStateChangedEventCallback>())
            GetAnyEventCallback<ssGUI::EventCallbacks::ButtonStateChangedEventCallback>()->Notify(static_cast<ssGUI::GUIObject*>(this));
    }

    void Button::MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                            ssGUI::GUIObject* mainWindow)
    {
        if(inputStatus.MouseInputBlockedObject == nullptr && IsBlockInput())
        {
            //On mouse down
            glm::ivec2 currentMousePos = inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow));
            if (inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) && !inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::LEFT))
            {
                //User pressing down on button
                if (currentMousePos.x >= GetGlobalPosition().x && currentMousePos.x <= GetGlobalPosition().x + GetSize().x &&
                    currentMousePos.y >= GetGlobalPosition().y && currentMousePos.y <= GetGlobalPosition().y + GetSize().y)
                {
                    inputStatus.MouseInputBlockedObject = this;
                    if(IsInteractable())
                    {
                        SetFocus(true);
                        SetButtonState(ssGUI::Enums::ButtonState::ON_CLICK);
                        inputInterface->SetCursorType(ssGUI::Enums::CursorType::HAND);
                    }
                }
            }
            //On mouse hold
            else if (inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) &&
                    (GetButtonState() == ssGUI::Enums::ButtonState::ON_CLICK ||
                    GetButtonState() == ssGUI::Enums::ButtonState::CLICKING))
            {
                inputStatus.MouseInputBlockedObject = this;
                if(IsInteractable())
                {
                    SetFocus(true);
                    inputInterface->SetCursorType(ssGUI::Enums::CursorType::HAND);
                    if (GetButtonState() == ssGUI::Enums::ButtonState::ON_CLICK)
                        SetButtonState(ssGUI::Enums::ButtonState::CLICKING);
                }
            }
            //On mouse up
            else if (!inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) && inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::LEFT) &&
                    (GetButtonState() == ssGUI::Enums::ButtonState::ON_CLICK || GetButtonState() == ssGUI::Enums::ButtonState::CLICKING) &&
                    currentMousePos.x >= GetGlobalPosition().x && currentMousePos.x <= GetGlobalPosition().x + GetSize().x &&
                    currentMousePos.y >= GetGlobalPosition().y && currentMousePos.y <= GetGlobalPosition().y + GetSize().y)
            {
                inputStatus.MouseInputBlockedObject = this;
                if(IsInteractable())
                {
                    SetFocus(true);
                    inputInterface->SetCursorType(ssGUI::Enums::CursorType::HAND);
                    SetButtonState(ssGUI::Enums::ButtonState::CLICKED);
                }
            }
            //Otherwise check normal/hover state
            else
            {
                if (currentMousePos.x >= GetGlobalPosition().x && currentMousePos.x <= GetGlobalPosition().x + GetSize().x &&
                    currentMousePos.y >= GetGlobalPosition().y && currentMousePos.y <= GetGlobalPosition().y + GetSize().y)
                {
                    inputStatus.MouseInputBlockedObject = this;
                    if(IsInteractable())
                    {
                        inputInterface->SetCursorType(ssGUI::Enums::CursorType::HAND);
                        SetButtonState(ssGUI::Enums::ButtonState::HOVER);
                    }
                }
                else
                {
                    if(IsInteractable())
                        SetButtonState(ssGUI::Enums::ButtonState::NORMAL);
                }
            }
        }
        else if(IsInteractable())
        {
            SetButtonState(ssGUI::Enums::ButtonState::NORMAL);
        }
        else
        {            
            if (GetButtonState() != ssGUI::Enums::ButtonState::DISABLED)
                SetButtonState(ssGUI::Enums::ButtonState::DISABLED);
        }
    }

    const std::string Button::ListenerKey = "Button";

    Button::Button() :  CurrentState(ssGUI::Enums::ButtonState::NORMAL),
                        ButtonColor(100, 100, 100, 255)
    {
        SetSize(glm::vec2(25, 25));
        auto stateChangedEventCallback = ssGUI::Factory::Create<ssGUI::EventCallbacks::ButtonStateChangedEventCallback>();
        stateChangedEventCallback->AddEventListener(
            ListenerKey, this,
            [](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* refs)
            {
                ssGUI::Button* btn = static_cast<ssGUI::Button*>(src);
                glm::u8vec4 btnColor = btn->GetButtonColor();
                switch(btn->GetButtonState())
                {
                    case ssGUI::Enums::ButtonState::NORMAL:
                        btnColor.a = 255;
                        btn->SetBackgroundColor(btnColor);
                        break;
                    case ssGUI::Enums::ButtonState::HOVER:
                        btnColor.a = 200;
                        btn->SetBackgroundColor(btnColor);
                        break;
                    case ssGUI::Enums::ButtonState::ON_CLICK:
                    case ssGUI::Enums::ButtonState::CLICKING:
                        btnColor.a = 100;
                        btn->SetBackgroundColor(btnColor);
                        break;
                    case ssGUI::Enums::ButtonState::CLICKED:
                    case ssGUI::Enums::ButtonState::DISABLED:
                        btnColor.a = 50;
                        btn->SetBackgroundColor(btnColor);
                        break;
                }
            }
        ); 
        
        AddEventCallback(stateChangedEventCallback);
        AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Border>());
        SetBackgroundColor(GetButtonColor());
    }

    Button::~Button()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
    }

    void Button::NotifyButtonEventCallbackManually()
    {   
        if(IsAnyEventCallbackExist<ssGUI::EventCallbacks::ButtonStateChangedEventCallback>())
            GetAnyEventCallback<ssGUI::EventCallbacks::ButtonStateChangedEventCallback>()->Notify(static_cast<ssGUI::GUIObject*>(this));
        //SetButtonState(GetButtonState());
    }

    ssGUI::Enums::ButtonState Button::GetButtonState() const
    {
        return CurrentState;
    }

    void Button::SetButtonColor(glm::u8vec4 color)
    {
        ButtonColor = color;
        SetBackgroundColor(GetButtonColor());
    }
    
    glm::u8vec4 Button::GetButtonColor() const
    {
        return ButtonColor;
    }

    //Overriding widget
    ssGUI::Enums::GUIObjectType Button::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::BUTTON | ssGUI::Enums::GUIObjectType::WIDGET;
    }

    void Button::SetInteractable(bool interactable)
    {
        if(interactable)
            SetButtonState(ssGUI::Enums::ButtonState::NORMAL);
        else
            SetButtonState(ssGUI::Enums::ButtonState::DISABLED);

        ssGUI::Widget::SetInteractable(interactable);
    }

    Button* Button::Clone(bool cloneChildren)
    {
        FUNC_DEBUG_ENTRY();
        Button* temp = new Button(*this);
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

