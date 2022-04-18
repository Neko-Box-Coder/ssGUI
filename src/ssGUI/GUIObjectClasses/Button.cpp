#include "ssGUI/GUIObjectClasses/Button.hpp"
#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

namespace ssGUI
{
    Button::Button(Button const& other) : Widget(other)
    {
        CurrentState = other.GetButtonState();
        StateChangedEventCallback = nullptr;
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

        //TODO : Set it as optional
        if(StateChangedEventCallback != nullptr)
            StateChangedEventCallback->Notify(static_cast<ssGUI::GUIObject*>(this));
    }

    Button::Button() : CurrentState(ssGUI::Enums::ButtonState::NORMAL), StateChangedEventCallback(nullptr)
    {
        SetBackgroundColor(glm::u8vec4(100,100,100,255)); //Gray background colour for button (For now)
        StateChangedEventCallback = ssGUI::Factory::Create<ssGUI::EventCallbacks::ButtonStateChangedEventCallback>();
        StateChangedEventCallback->AddEventListener(
            [](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* refs)
            {
                ssGUI::Button* btn = static_cast<ssGUI::Button*>(src);
                glm::u8vec4 bgcolor = btn->GetBackgroundColor();
                switch(btn->GetButtonState())
                {
                    case ssGUI::Enums::ButtonState::NORMAL:
                        bgcolor.a = 255;
                        btn->SetBackgroundColor(bgcolor);
                        break;
                    case ssGUI::Enums::ButtonState::HOVER:
                        bgcolor.a = 200;
                        btn->SetBackgroundColor(bgcolor);
                        break;
                    case ssGUI::Enums::ButtonState::CLICKED:
                    case ssGUI::Enums::ButtonState::ON_CLICK:
                    case ssGUI::Enums::ButtonState::CLICKING:
                        bgcolor.a = 100;
                        btn->SetBackgroundColor(bgcolor);
                        break;
                    case ssGUI::Enums::ButtonState::DISABLED:
                        bgcolor.a = 50;
                        btn->SetBackgroundColor(bgcolor);
                        break;
                }
            }
        ); 
        
        AddEventCallback(StateChangedEventCallback);
        AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Border>());
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
        return ssGUI::Enums::GUIObjectType::BUTTON | ssGUI::Enums::GUIObjectType::WIDGET;
    }

    void Button::SetInteractable(bool interactable)
    {
        if(interactable)
            CurrentState = ssGUI::Enums::ButtonState::NORMAL;
        else
            CurrentState = ssGUI::Enums::ButtonState::DISABLED;

        ssGUI::Widget::SetInteractable(interactable);
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

        for (auto extension : ExtensionsUpdateOrder)
        {
            //Guard against extension being deleted by other extensions
            if(!IsExtensionExist(extension))
                continue;

            Extensions.at(extension)->Internal_Update(true, inputInterface, globalInputStatus, windowInputStatus, mainWindow);
        }

        if(!globalInputStatus.MouseInputBlocked && !windowInputStatus.MouseInputBlocked)
        {
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
                    (GetButtonState() == ssGUI::Enums::ButtonState::ON_CLICK || GetButtonState() == ssGUI::Enums::ButtonState::CLICKING) &&
                    currentMousePos.x >= GetGlobalPosition().x && currentMousePos.x <= GetGlobalPosition().x + GetSize().x &&
                    currentMousePos.y >= GetGlobalPosition().y && currentMousePos.y <= GetGlobalPosition().y + GetSize().y)
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
                        globalInputStatus.MouseInputBlocked = true;
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
        }        

        endUpdate:;
        for (auto extension : ExtensionsUpdateOrder)
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

    Button* Button::Clone(bool cloneChildren)
    {
        FUNC_DEBUG_ENTRY();
        Button* temp = new Button(*this);
        CloneExtensionsAndEventCallbacks(temp);

        if(temp->IsEventCallbackExist(ssGUI::EventCallbacks::ButtonStateChangedEventCallback::EVENT_NAME))
        {
            temp->StateChangedEventCallback = static_cast<ssGUI::EventCallbacks::ButtonStateChangedEventCallback*>
                (temp->GetEventCallback(ssGUI::EventCallbacks::ButtonStateChangedEventCallback::EVENT_NAME));
        }
        
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

