#include "ssGUI/GUIObjectClasses/CompositeClasses/Slider.hpp"
#include "ssGUI/Extensions/Outline.hpp"
#include "ssGUI/Extensions/RoundedCorners.hpp"
#include "ssGUI/HelperClasses/DrawCircle.hpp"
#include "ssGUI/GUIObjectClasses/Button.hpp"
#include "ssGUI/HeaderGroups/InputGroup.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{
    Slider::Slider(Slider const& other) : Widget(other)
    {
        Reverse = other.Reverse;
        FillColor = other.FillColor;
        KnobObject = other.KnobObject;
        SliderValue = other.SliderValue;
        Vertical = other.Vertical;
        SnapInterval = other.SnapInterval;
        ScrollInternal = other.ScrollInternal;
        KeyInputInterval = other.KeyInputInterval;

        KnobGlobalOffset = other.KnobGlobalOffset;
        CursorKnobOffset = other.CursorKnobOffset;
        LastSliderDragging = false;
        SliderDragging = false;
        LastGlobalPosition = other.LastGlobalPosition;
        LastSize = other.LastSize;

        LastKeyNavStartTime = other.LastKeyNavStartTime;
        KeyNavPauseDuration = other.KeyNavPauseDuration;
        LastKeyNavTime = other.LastKeyNavTime;
        KeyNavInterval = other.KeyNavInterval;
    }

    void Slider::ApplySnapping()
    {
        if(GetSnapInterval() <= 0)
            return;
        
        float snapValue = GetSliderValue() / GetSnapInterval();
        SliderValue = round(snapValue) * GetSnapInterval();
    }

    void Slider::UpdateKnobOffset()
    {
        auto knob = static_cast<ssGUI::Button*>(GetKnobObject());
        glm::vec2 curKnobSize = knob == nullptr ? glm::vec2(KnobSize, KnobSize) : knob->GetSize();

        if(!Vertical)
        {
            float leftX = GetGlobalPosition().x + GetEndPadding();
            float rightX = GetGlobalPosition().x + GetSize().x - curKnobSize.x - GetEndPadding();

            KnobGlobalOffset = glm::vec2
            (
                (rightX - leftX) * (IsReverse() ? (1 - GetSliderValue()) : GetSliderValue()), 
                (GetSize().y - curKnobSize.y) * 0.5
            );
        }
        else
        {
            float topY = GetGlobalPosition().y + GetEndPadding();
            float botY = GetGlobalPosition().y + GetSize().y - curKnobSize.y - GetEndPadding();
            
            KnobGlobalOffset = glm::vec2
            (
                (GetSize().x - curKnobSize.x) * 0.5,
                (botY - topY) * (IsReverse() ? GetSliderValue() : (1 - GetSliderValue()))
            );
        }

        if(knob != nullptr)
            knob->SetGlobalPosition(GetGlobalPosition() + KnobGlobalOffset);
        
        //RedrawObject();
    }

    void Slider::UpdateSliderValueFromCursor(glm::vec2 cursorPos)
    {
        ssGUI_LOG_FUNC();
        auto knob = static_cast<ssGUI::Button*>(CurrentObjectsReferences.GetObjectReference(KnobObject));
        glm::vec2 curKnobSize = knob == nullptr ? glm::vec2(KnobSize, KnobSize) : knob->GetSize();

        cursorPos -= CursorKnobOffset;
        if(!IsVertical())
        {
            float leftX = GetGlobalPosition().x + GetEndPadding();
            float rightX = GetGlobalPosition().x + GetSize().x - curKnobSize.x - GetEndPadding();
            
            cursorPos.x = cursorPos.x < leftX ? leftX : cursorPos.x;
            cursorPos.x = cursorPos.x > rightX ? rightX : cursorPos.x;

            SetSliderValue((cursorPos.x - leftX) / (rightX - leftX));
            if(IsReverse())
                SetSliderValue(1 - GetSliderValue());
        }
        else
        {
            float topY = GetGlobalPosition().y + GetEndPadding();
            float botY = GetGlobalPosition().y + GetSize().y - curKnobSize.y - GetEndPadding();

            cursorPos.y = cursorPos.y < topY ? topY : cursorPos.y;
            cursorPos.y = cursorPos.y > botY ? botY : cursorPos.y;

            SetSliderValue((botY - cursorPos.y) / (botY - topY));
            if(IsReverse())
                SetSliderValue(1 - GetSliderValue());
        }
    }

    void Slider::ConstructRenderInfo()
    {       
        //Get the top-left position of the widget 
        glm::vec2 drawPosition = GetGlobalPosition();

        //Slider Background
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

        //Slider fill
        auto knob = static_cast<ssGUI::Button*>(CurrentObjectsReferences.GetObjectReference(KnobObject));
        glm::vec2 curKnobSize = knob == nullptr ? glm::vec2(KnobSize, KnobSize) : knob->GetSize();
        glm::vec2 knobPos = drawPosition + KnobGlobalOffset;

        ssGUI::DrawingEntity sliderFillEntity;

        if(!IsVertical())
        {
            if(!IsReverse())
            {
                sliderFillEntity.Vertices.push_back(glm::vec2(drawPosition));
                sliderFillEntity.Vertices.push_back(glm::vec2(knobPos.x + curKnobSize.x / 2, drawPosition.y));
                sliderFillEntity.Vertices.push_back(glm::vec2(knobPos.x + curKnobSize.x / 2, drawPosition.y + GetSize().y));
                sliderFillEntity.Vertices.push_back(glm::vec2(drawPosition.x, drawPosition.y + GetSize().y));
            }
            else
            {
                sliderFillEntity.Vertices.push_back(glm::vec2(knobPos.x + curKnobSize.x / 2, drawPosition.y));
                sliderFillEntity.Vertices.push_back(glm::vec2(drawPosition.x + GetSize().x, drawPosition.y));
                sliderFillEntity.Vertices.push_back(glm::vec2(drawPosition.x + GetSize().x, drawPosition.y + GetSize().y));
                sliderFillEntity.Vertices.push_back(glm::vec2(knobPos.x + curKnobSize.x / 2, drawPosition.y + GetSize().y));
            }
        }
        else
        {
            if(!IsReverse())
            {
                sliderFillEntity.Vertices.push_back(glm::vec2(drawPosition.x, knobPos.y + curKnobSize.y / 2));
                sliderFillEntity.Vertices.push_back(glm::vec2(drawPosition.x + GetSize().x, knobPos.y + curKnobSize.y / 2));
                sliderFillEntity.Vertices.push_back(glm::vec2(drawPosition + GetSize()));
                sliderFillEntity.Vertices.push_back(glm::vec2(drawPosition.x, drawPosition.y + GetSize().y));
                
            }
            else
            {
                sliderFillEntity.Vertices.push_back(glm::vec2(drawPosition));
                sliderFillEntity.Vertices.push_back(glm::vec2(drawPosition.x + GetSize().x, drawPosition.y));
                sliderFillEntity.Vertices.push_back(glm::vec2(drawPosition.x + GetSize().x, knobPos.y + curKnobSize.y / 2));
                sliderFillEntity.Vertices.push_back(glm::vec2(drawPosition.x, knobPos.y + curKnobSize.y / 2));
            }
        }

        for(int i = 0; i < 4; i++)
            sliderFillEntity.Colors.push_back(GetFillColor());
    
        sliderFillEntity.EntityName = SLIDER_FILL_SHAPE_NAME;
        DrawingEntities.push_back(sliderFillEntity);
    }

    void Slider::MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                            ssGUI::GUIObject* mainWindow)
    {       
        ssGUI_LOG_FUNC();

        auto knob = static_cast<ssGUI::Button*>(CurrentObjectsReferences.GetObjectReference(KnobObject));
        glm::vec2 curKnobSize = knob == nullptr ? glm::vec2(KnobSize, KnobSize) : knob->GetSize();
        glm::ivec2 mousePos = inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)->GetBackendWindowInterface());

        bool guiInteracted = false;

        if(!inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT))
            SliderDragging = false;

        //If the user clicked on the knob, record the offset
        if(knob != nullptr && knob->GetButtonState() == ssGUI::Enums::ButtonState::ON_CLICK && IsInteractable())
        {
            CursorKnobOffset = 
                IsVertical() ?
                mousePos.y - knob->GetGlobalPosition().y : 
                mousePos.x - knob->GetGlobalPosition().x;
        }
        //If the user is dragging the knob, update the position
        else if(knob != nullptr && ((knob->GetButtonState() == ssGUI::Enums::ButtonState::CLICKING && IsInteractable() && IsBlockInput()) || SliderDragging))
        {
            knob->SetFocus(true);
            
            float oriSliderValue = GetSliderValue();

            //Update the slider value according to the slider position
            UpdateSliderValueFromCursor(mousePos);

            //Record if slider value is changed via GUI
            guiInteracted = oriSliderValue == GetSliderValue() ? false : true;

            //Apply snapping
            ApplySnapping();

            //Reposition the knob
            UpdateKnobOffset();

            inputStatus.MouseInputBlockedObject = this;
        }
        else
        {
            //Check if user clicked on the slider instead. If so, move the knob to the cursor
            bool mouseWithinWidget = 
                mousePos.x >= GetGlobalPosition().x && mousePos.x <= GetGlobalPosition().x + GetSize().x &&
                mousePos.y >= GetGlobalPosition().y && mousePos.y <= GetGlobalPosition().y + GetSize().y;
            
            //TODO: Tidy this
            if
            (
                mouseWithinWidget &&
                (
                    inputStatus.MouseInputBlockedObject == nullptr || 
                    (GetKnobObject() != nullptr && inputStatus.MouseInputBlockedObject == GetKnobObject())
                ) 
                &&
                IsBlockInput()
            )
            {
                if(IsInteractable())
                {
                    inputInterface->SetCursorType(ssGUI::Enums::CursorType::HAND);
                    
                    float oriSliderValue = GetSliderValue();

                    //Handle mouse clicking on the slider
                    if(!inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::LEFT) && 
                        inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT))
                    {
                        SetSliderValue
                        ( 
                            IsVertical() ? 
                            1 - (mousePos.y - GetGlobalPosition().y - GetEndPadding()) / (GetSize().y - GetEndPadding() * 2) : 
                            (mousePos.x - GetGlobalPosition().x - GetEndPadding()) / (GetSize().x - GetEndPadding() * 2)
                        );

                        if(IsReverse())
                            SetSliderValue(1 - GetSliderValue());

                        SliderDragging = true;
                        CursorKnobOffset = IsVertical() ? curKnobSize.y * 0.5 : curKnobSize.x * 0.5;
                    
                        SetFocus(true);
                    }
                    //Handle scrolling on the slider
                    else if(inputInterface->GetCurrentMouseScrollDelta().x != 0 || inputInterface->GetCurrentMouseScrollDelta().y != 0)
                    {
                        SetFocus(true);
                        if(inputInterface->GetCurrentMouseScrollDelta().y != 0)
                        {
                            SetSliderValue
                            (
                                GetSliderValue() + 
                                (
                                    inputInterface->GetCurrentMouseScrollDelta().y > 0 ? 
                                        GetScrollInterval() * 1 :
                                        GetScrollInterval() * -1
                                ) * (IsReverse() ? -1 : 1) * (IsVertical() ? 1 : -1)
                            );
                        }
                        else if(inputInterface->GetCurrentMouseScrollDelta().x != 0)
                        {
                            SetSliderValue
                            (
                                GetSliderValue() + 
                                (
                                    inputInterface->GetCurrentMouseScrollDelta().x > 0 ?
                                        GetScrollInterval() * 1 :
                                        GetScrollInterval() * -1
                                ) * (IsReverse() ? -1 : 1)
                            );
                        }
                    }

                    //Record if slider value is changed via GUI
                    guiInteracted = oriSliderValue == GetSliderValue() ? false : true;
                }

                inputStatus.MouseInputBlockedObject = this;
            }
            
            //Check for keyboard input
            if(inputStatus.KeyInputBlockedObject == NULL && (IsFocused() || mouseWithinWidget) && IsInteractable() && IsBlockInput() && !guiInteracted)
            {
                float oriSliderValue = GetSliderValue();
                
                auto decreaseSliderValue = [&]()
                {
                    SetSliderValue(GetSliderValue() + GetKeyInputMoveInterval() * -1);
                };

                auto increaseSliderValue = [&]()
                {
                    SetSliderValue(GetSliderValue() + GetKeyInputMoveInterval() * 1);
                };
                
                //Start of left/down input
                bool leftKeyDown =  inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::LEFT) &&
                                    !inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::SystemKey::LEFT);
                
                bool downKeyDown =  inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::DOWN) &&
                                    !inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::SystemKey::DOWN);

                bool leftOrDownKeyPressed = inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::LEFT) ||
                                            inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::DOWN);

                bool rightKeyDown = inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::RIGHT) &&
                                    !inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::SystemKey::RIGHT);
                
                bool upKeyDown =    inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::UP) &&
                                    !inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::SystemKey::UP);

                bool rightOrUpKeyPressed =  inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::RIGHT) ||
                                            inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::UP);


                if(leftKeyDown || downKeyDown)
                {
                    LastKeyNavStartTime = inputInterface->GetElapsedTime();
                    IsReverse() ? increaseSliderValue() : decreaseSliderValue();
                    inputStatus.KeyInputBlockedObject = this;
                    SetFocus(true);
                }
                //Continuous input of left/down
                else if(leftOrDownKeyPressed &&
                        inputInterface->GetElapsedTime() - LastKeyNavStartTime > KeyNavPauseDuration &&
                        inputInterface->GetElapsedTime() - LastKeyNavTime > KeyNavInterval)
                {
                    LastKeyNavTime = inputInterface->GetElapsedTime();
                    IsReverse() ? increaseSliderValue() : decreaseSliderValue();
                    inputStatus.KeyInputBlockedObject = this;
                    SetFocus(true);
                }

                //Start of right/up input
                if(rightKeyDown || upKeyDown)
                {
                    LastKeyNavStartTime = inputInterface->GetElapsedTime();
                    IsReverse() ? decreaseSliderValue() : increaseSliderValue();
                    inputStatus.KeyInputBlockedObject = this;
                    SetFocus(true);
                }
                //Continuous input of right/up
                else if(rightOrUpKeyPressed &&
                        inputInterface->GetElapsedTime() - LastKeyNavStartTime > KeyNavPauseDuration &&
                        inputInterface->GetElapsedTime() - LastKeyNavTime > KeyNavInterval)
                {
                    LastKeyNavTime = inputInterface->GetElapsedTime();
                    IsReverse() ? decreaseSliderValue() : increaseSliderValue();
                    inputStatus.KeyInputBlockedObject = this;
                    SetFocus(true);
                }

                //Record if slider value is changed via GUI
                guiInteracted = oriSliderValue == GetSliderValue() ? false : true;
            }
        }

        //If there's any size or position change, we need to update the knob position
        if(GetSize() != LastSize || GetGlobalPosition() != LastGlobalPosition)
            UpdateKnobOffset();

        LastSliderDragging = SliderDragging;
        LastGlobalPosition = GetGlobalPosition();
        LastSize = GetSize();

        if(guiInteracted)
        {
            if(IsEventCallbackExist(ssGUI::Enums::EventType::SLIDER_VALUE_CHANGED_VIA_GUI))
                GetEventCallback(ssGUI::Enums::EventType::SLIDER_VALUE_CHANGED_VIA_GUI)->Notify(static_cast<ssGUI::GUIObject*>(this));
        }

        if(LastSliderValue != GetSliderValue())
        {
            if(IsEventCallbackExist(ssGUI::Enums::EventType::SLIDER_VALUE_CHANGED))
                GetEventCallback(ssGUI::Enums::EventType::SLIDER_VALUE_CHANGED)->Notify(static_cast<ssGUI::GUIObject*>(this));

            LastValueChanged = true;
        }
        else if(LastValueChanged)
        {
            if(IsEventCallbackExist(ssGUI::Enums::EventType::SLIDER_VALUE_FINISHED_CHANGING))
            {
                GetEventCallback(ssGUI::Enums::EventType::SLIDER_VALUE_FINISHED_CHANGING)->
                    Notify(static_cast<ssGUI::GUIObject*>(this));
            }
            
            LastValueChanged = false;
        }

        LastSliderValue = GetSliderValue();
    }

    const std::string Slider::ListenerKey = "Slider";
    const std::string Slider::SLIDER_FILL_SHAPE_NAME = "Slider Fill";
    
    Slider::Slider() :  Reverse(false),
                        FillColor(0, 0, 0, 0),
                        KnobObject(-1),
                        KnobSize(15),
                        SliderValue(0.5),
                        Vertical(false),
                        SnapInterval(0),
                        ScrollInternal(0.05),
                        KeyInputInterval(0.05),
                        EndPadding(0),
                        KnobGlobalOffset(),
                        CursorKnobOffset(0),
                        LastSliderDragging(false),
                        SliderDragging(false),
                        LastGlobalPosition(),
                        LastSize(),
                        LastSliderValue(SliderValue),
                        LastValueChanged(false),
                        LastKeyNavStartTime(0),
                        KeyNavPauseDuration(500),
                        LastKeyNavTime(0),
                        KeyNavInterval(20)
    {
        SetMinSize(glm::vec2(5, 5));
        SetSize(glm::vec2(300, 10));
        auto rc = AddExtension<ssGUI::Extensions::RoundedCorners>();
        rc->SetRoundedCornersRadius(10);
        rc->AddTargetShape(1);

        AddExtension<ssGUI::Extensions::Outline>();

        //Create knob object
        auto button = ssGUI::Factory::Create<ssGUI::Button>();
        button->SetMinSize(glm::vec2(5, 5));
        button->SetSize(glm::vec2(KnobSize, KnobSize));
        button->SetButtonColor(GetBackgroundColor());
        
        button->AddExtension<ssGUI::Extensions::RoundedCorners>()->SetRoundedCornersRadius(KnobSize);

        button->RemoveAnyExtension<ssGUI::Extensions::Border>();
        button->SetUserCreated(false);
        button->AddExtension<ssGUI::Extensions::Outline>()->SetOutlineThickness(1.5);
        button->SetParent(this, true);

        auto ecb = button->GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED);
        ecb->ClearEventListeners();
        ecb->AddEventListener
        (
            ListenerKey, this,
            [](ssGUI::EventInfo& info)
            {
                ssGUI::Button* btn = static_cast<ssGUI::Button*>(info.EventSource);
                glm::u8vec4 btnColor = btn->GetButtonColor();
                int reactAmount = (btnColor.r + btnColor.g + btnColor.b) / 3 > 127 ? -20 : 20;
                
                static_assert((int)ssGUI::Enums::ButtonState::COUNT == 6, "Make sure this is updated");
                switch(btn->GetButtonState())
                {
                    case ssGUI::Enums::ButtonState::NORMAL:
                        btn->SetBackgroundColor(btnColor);
                        break;
                    case ssGUI::Enums::ButtonState::HOVER:
                        btn->SetBackgroundColor(btnColor + glm::u8vec4(reactAmount, reactAmount, reactAmount, 0));
                        break;
                    case ssGUI::Enums::ButtonState::ON_CLICK:
                    case ssGUI::Enums::ButtonState::CLICKING:
                        btn->SetBackgroundColor(btnColor + glm::u8vec4(reactAmount * 2, reactAmount * 2, reactAmount * 2, 0));
                        break;
                    case ssGUI::Enums::ButtonState::CLICKED:
                    case ssGUI::Enums::ButtonState::NOT_INTERACTABLE:
                        btn->SetBackgroundColor(btnColor + glm::u8vec4(reactAmount * 3, reactAmount * 3, reactAmount * 3, 0));
                        break;
                }
            }
        ); 

        KnobObject = CurrentObjectsReferences.GetObjectIndex(button);
        UpdateKnobOffset();
    }

    Slider::~Slider()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
        
        //If the object deallocation is not handled by ssGUIManager
        if(!Internal_IsDeleted())
            Internal_ChildrenManualDeletion(std::vector<ssGUI::ssGUIObjectIndex>{KnobObject});
    }

    void Slider::SetReverse(bool reverse)
    {
        Reverse = reverse;
    }

    bool Slider::IsReverse() const
    {
        return Reverse;
    }

    void Slider::SetFillColor(glm::u8vec4 fillColor)
    {
        FillColor = fillColor;
    }

    glm::u8vec4 Slider::GetFillColor() const
    {
        return FillColor;
    }

    void Slider::SetKnobObject(ssGUI::Button* knob)
    {
        if(KnobObject != -1 && CurrentObjectsReferences.GetObjectReference(KnobObject) != nullptr)
        {
            ssGUI::GUIObject* knobObj = CurrentObjectsReferences.GetObjectReference(KnobObject);
            knobObj->Delete();
        }
        KnobObject = -1;

        if(knob == nullptr)
            return;
        
        int index = CurrentObjectsReferences.AddObjectReference(knob);
        KnobObject = index;

        glm::vec2 globalPos = knob->GetGlobalPosition();
        knob->SetParent(this, true);
        knob->SetGlobalPosition(globalPos);
    }

    ssGUI::Button* Slider::GetKnobObject() const
    {
        return CurrentObjectsReferences.GetObjectReference<ssGUI::Button>(KnobObject);
    }

    void Slider::SetKnobSize(float knobSize, bool updateKnobObject)
    {
        if(updateKnobObject && KnobObject != -1 && CurrentObjectsReferences.GetObjectReference(KnobObject) != nullptr)
        {
            ssGUI::GUIObject* knobObj = CurrentObjectsReferences.GetObjectReference(KnobObject);
            knobObj->SetSize(glm::vec2(knobSize, knobSize));
        }

        KnobSize = knobSize;
    }

    float Slider::GetKnobSize() const
    {
        if(KnobObject != -1 && CurrentObjectsReferences.GetObjectReference(KnobObject) != nullptr)
            return IsVertical() ? GetKnobObject()->GetSize().y : GetKnobObject()->GetSize().x;

        return KnobSize;
    }

    void Slider::SetEndPadding(float padding)
    {
        EndPadding = padding;
    }

    float Slider::GetEndPadding() const
    {
        return EndPadding;
    }

    void Slider::SetSliderValue(float sliderValue)
    {
        ssGUI_LOG_FUNC();
        SliderValue = sliderValue;
        SliderValue = SliderValue < 0 ? 0 : SliderValue;
        SliderValue = SliderValue > 1 ? 1 : SliderValue;

        ApplySnapping();
        UpdateKnobOffset();
    }

    float Slider::GetSliderValue() const
    {
        return SliderValue;
    }

    void Slider::SetVertical(bool vertical, bool swapWidthHeight)
    {
        if(swapWidthHeight && IsVertical() != vertical)
        {
            float temp = GetSize().x;
            SetSize(glm::vec2(GetSize().y, temp));

            if(KnobObject != -1 && CurrentObjectsReferences.GetObjectReference(KnobObject) != nullptr)
            {
                ssGUI::GUIObject* knobObj = CurrentObjectsReferences.GetObjectReference(KnobObject);
                temp = knobObj->GetSize().x;
                knobObj->SetSize(glm::vec2(knobObj->GetSize().y, temp));
            }
        }

        bool oriVertical = IsVertical();
        Vertical = vertical;
        
        if(oriVertical != IsVertical())
            RedrawObject();
    }

    bool Slider::IsVertical() const
    {
        return Vertical;
    }

    void Slider::SetSnapInterval(float interval)
    {
        SnapInterval = interval;
    }

    float Slider::GetSnapInterval() const
    {
        return SnapInterval;
    }

    void Slider::SetScrollInterval(float interval)
    {
        ScrollInternal = interval;
    }

    float Slider::GetScrollInterval() const
    {
        return ScrollInternal;
    }

    void Slider::SetKeyInputMoveInterval(float interval)
    {
        KeyInputInterval = interval;
    }

    float Slider::GetKeyInputMoveInterval() const
    {
        return KeyInputInterval;
    }
    
    void Slider::SetInteractable(bool interactable)
    {
        if(GetKnobObject() != nullptr)
            GetKnobObject()->SetInteractable(interactable);
    
        Widget::SetInteractable(interactable);
    }
    
    void Slider::SetBlockInput(bool blockInput)
    {
        if(GetKnobObject() != nullptr)
            GetKnobObject()->SetBlockInput(blockInput);
    
        Widget::SetBlockInput(blockInput);
    }

    ssGUI::Enums::GUIObjectType Slider::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::WIDGET | ssGUI::Enums::GUIObjectType::SLIDER;
    }

    Slider* Slider::Clone(bool cloneChildren)
    {
        ssGUI_LOG_FUNC();
        Slider* temp = new Slider(*this);
        CloneExtensionsAndEventCallbacks(temp);   
        
        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
                return nullptr;
        }

        return temp;
    }
}