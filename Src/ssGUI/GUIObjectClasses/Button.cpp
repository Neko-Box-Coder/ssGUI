#include "ssGUI/GUIObjectClasses/Button.hpp"
#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

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

    void Button::SetButtonState(ssGUI::Enums::ButtonState state)
    {
        if(CurrentState == state)
            return;
        
        CurrentState = state;

        if(IsEventCallbackExist(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED))
            GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED)->Notify(static_cast<ssGUI::GUIObject*>(this));
    }

    void Button::MainLogic( ssGUI::Backend::SystemInputInterface* inputInterface, 
                            ssGUI::InputStatus& currentInputStatus, 
                            ssGUI::InputStatus& lastInputStatus, 
                            ssGUI::GUIObject* mainWindow)
    {
        if(currentInputStatus.MouseInputBlockedData.GetBlockDataType() == ssGUI::Enums::BlockDataType::NONE && IsBlockInput())
        {
            //On mouse down
            glm::ivec2 currentMousePos = inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)->GetBackendWindowInterface());
            if (inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) && !inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::LEFT))
            {
                //User pressing down on button
                if (currentMousePos.x >= GetGlobalPosition().x && currentMousePos.x <= GetGlobalPosition().x + GetSize().x &&
                    currentMousePos.y >= GetGlobalPosition().y && currentMousePos.y <= GetGlobalPosition().y + GetSize().y)
                {
                    currentInputStatus.MouseInputBlockedData.SetBlockData(this);
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
                currentInputStatus.MouseInputBlockedData.SetBlockData(this);
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
                currentInputStatus.MouseInputBlockedData.SetBlockData(this);
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
                    currentInputStatus.MouseInputBlockedData.SetBlockData(this);
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
            if (GetButtonState() != ssGUI::Enums::ButtonState::NOT_INTERACTABLE)
                SetButtonState(ssGUI::Enums::ButtonState::NOT_INTERACTABLE);
        }
    }

    const std::string Button::ListenerKey = "Button";

    Button::Button() :  CurrentState(ssGUI::Enums::ButtonState::NORMAL),
                        ButtonColor(100, 100, 100, 255)
    {
        SetSize(glm::vec2(25, 25));
        auto stateChangedEventCallback = AddEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED);
        stateChangedEventCallback->AddEventListener(
            ListenerKey, this,
            [](ssGUI::EventInfo& info)
            {
                ssGUI::Button* btn = static_cast<ssGUI::Button*>(info.EventSource);
                glm::u8vec4 btnColor = btn->GetButtonColor();
                
                static_assert((int)ssGUI::Enums::ButtonState::COUNT == 6, "Make sure this is updated");
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
                    case ssGUI::Enums::ButtonState::NOT_INTERACTABLE:
                        btnColor.a = 50;
                        btn->SetBackgroundColor(btnColor);
                        break;
                }
            }
        ); 
        
        AddExtension<ssGUI::Extensions::Border>();
        SetBackgroundColor(GetButtonColor());
    }

    Button::~Button()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
    }

    void Button::NotifyButtonEventCallbackManually()
    {   
        if(IsEventCallbackExist(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED))
            GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED)->Notify(static_cast<ssGUI::GUIObject*>(this));
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
            SetButtonState(ssGUI::Enums::ButtonState::NOT_INTERACTABLE);

        ssGUI::Widget::SetInteractable(interactable);
    }

    Button* Button::Clone(bool cloneChildren)
    {
        ssGUI_LOG_FUNC();
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

