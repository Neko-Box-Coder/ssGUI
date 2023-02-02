#include "ssGUI/GUIObjectClasses/CompositeClasses/Slider.hpp"
#include "ssGUI/Extensions/Outline.hpp"
#include "ssGUI/Extensions/RoundedCorners.hpp"
#include "ssGUI/HelperClasses/DrawCircle.hpp"
#include "ssGUI/GUIObjectClasses/Button.hpp"
#include "ssGUI/EventCallbacks/SliderValueChangedEventCallback.hpp"
#include "ssGUI/EventCallbacks/SliderValueFinishedChangingEventCallback.hpp"
#include "ssGUI/EventCallbacks/SliderValueChangedViaGuiEventCallback.hpp"
#include "ssGUI/HeaderGroups/InputGroup.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

#include "ssLogger/ssLog.hpp"

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

        KnobGlobalPosition = other.KnobGlobalPosition;
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
        if(GetSnapInterval() == 0)
            return;
        
        float snapValue = GetSliderValue() / GetSnapInterval();
        SliderValue = round(snapValue) * GetSnapInterval();
    }

    void Slider::UpdateKnobPosition(bool completeUpdate)
    {
        auto knob = static_cast<ssGUI::Button*>(GetKnobObject());
        glm::vec2 curKnobSize = knob == nullptr ? glm::vec2(KnobSize, KnobSize) : knob->GetSize();

        if(!Vertical)
        {
            float leftX = GetGlobalPosition().x + GetEndPadding();
            float rightX = GetGlobalPosition().x + GetSize().x - curKnobSize.x - GetEndPadding();

            KnobGlobalPosition = glm::vec2
            (
                leftX + (rightX - leftX) * (IsReverse() ? (1 - GetSliderValue()) : GetSliderValue()), 
                completeUpdate ? GetGlobalPosition().y + (GetSize().y - curKnobSize.y) * 0.5 : KnobGlobalPosition.y
            );
        }
        else
        {
            float topY = GetGlobalPosition().y + GetEndPadding();
            float botY = GetGlobalPosition().y + GetSize().y - curKnobSize.y - GetEndPadding();
            
            KnobGlobalPosition = glm::vec2
            (
                completeUpdate ? GetGlobalPosition().x + (GetSize().x - curKnobSize.x) * 0.5 : KnobGlobalPosition.x,
                botY - (botY - topY) * (IsReverse() ? (1 - GetSliderValue()) : GetSliderValue())
            );
        }

        if(knob != nullptr)
            knob->SetGlobalPosition(KnobGlobalPosition);
        
        RedrawObject();
    }

    void Slider::UpdateSliderValue()
    {
        ssLOG_FUNC_ENTRY();
        auto knob = static_cast<ssGUI::Button*>(CurrentObjectsReferences.GetObjectReference(KnobObject));
        glm::vec2 curKnobSize = knob == nullptr ? glm::vec2(KnobSize, KnobSize) : knob->GetSize();

        if(!IsVertical())
        {
            float leftX = GetGlobalPosition().x + GetEndPadding();
            float rightX = GetGlobalPosition().x + GetSize().x - curKnobSize.x - GetEndPadding();

            SetSliderValue((KnobGlobalPosition.x - leftX) / (rightX - leftX));
            if(IsReverse())
                SetSliderValue(1 - GetSliderValue());
        }
        else
        {
            float topY = GetGlobalPosition().y + GetEndPadding();
            float botY = GetGlobalPosition().y + GetSize().y - curKnobSize.y - GetEndPadding();

            SetSliderValue((botY - KnobGlobalPosition.y) / (botY - topY));
            if(IsReverse())
                SetSliderValue(1 - GetSliderValue());
        }
        ssLOG_FUNC_EXIT();
    }

    void Slider::ConstructRenderInfo()
    {       
        //Get the top-left position of the widget 
        glm::vec2 drawPosition = GetGlobalPosition();

        //Slider Background
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

        //Slider fill
        auto knob = static_cast<ssGUI::Button*>(CurrentObjectsReferences.GetObjectReference(KnobObject));
        glm::vec2 curKnobSize = knob == nullptr ? glm::vec2(KnobSize, KnobSize) : knob->GetSize();

        if(!IsVertical())
        {
            if(!IsReverse())
            {
                DrawingVerticies.push_back(glm::vec2(drawPosition));
                DrawingVerticies.push_back(glm::vec2(KnobGlobalPosition.x + curKnobSize.x / 2, drawPosition.y));
                DrawingVerticies.push_back(glm::vec2(KnobGlobalPosition.x + curKnobSize.x / 2, drawPosition.y + GetSize().y));
                DrawingVerticies.push_back(glm::vec2(drawPosition.x, drawPosition.y + GetSize().y));
            }
            else
            {
                DrawingVerticies.push_back(glm::vec2(KnobGlobalPosition.x + curKnobSize.x / 2, drawPosition.y));
                DrawingVerticies.push_back(glm::vec2(drawPosition.x + GetSize().x, drawPosition.y));
                DrawingVerticies.push_back(glm::vec2(drawPosition.x + GetSize().x, drawPosition.y + GetSize().y));
                DrawingVerticies.push_back(glm::vec2(KnobGlobalPosition.x + curKnobSize.x / 2, drawPosition.y + GetSize().y));
            }
        }
        else
        {
            if(!IsReverse())
            {
                DrawingVerticies.push_back(glm::vec2(drawPosition.x, KnobGlobalPosition.y + curKnobSize.y / 2));
                DrawingVerticies.push_back(glm::vec2(drawPosition.x + GetSize().x, KnobGlobalPosition.y + curKnobSize.y / 2));
                DrawingVerticies.push_back(glm::vec2(drawPosition + GetSize()));
                DrawingVerticies.push_back(glm::vec2(drawPosition.x, drawPosition.y + GetSize().y));
                
            }
            else
            {
                DrawingVerticies.push_back(glm::vec2(drawPosition));
                DrawingVerticies.push_back(glm::vec2(drawPosition.x + GetSize().x, drawPosition.y));
                DrawingVerticies.push_back(glm::vec2(drawPosition.x + GetSize().x, KnobGlobalPosition.y + curKnobSize.y / 2));
                DrawingVerticies.push_back(glm::vec2(drawPosition.x, KnobGlobalPosition.y + curKnobSize.y / 2));
            }
        }

        for(int i = 0; i < 4; i++)
        {
            DrawingUVs.push_back(glm::vec2());
            DrawingColours.push_back(GetFillColor());
        }
        DrawingCounts.push_back(4);
        DrawingProperties.push_back(ssGUI::DrawingProperty());
    }

    void Slider::MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                            ssGUI::GUIObject* mainWindow)
    {       
        ssLOG_FUNC_ENTRY();

        auto knob = static_cast<ssGUI::Button*>(CurrentObjectsReferences.GetObjectReference(KnobObject));
        glm::vec2 curKnobSize = knob == nullptr ? glm::vec2(KnobSize, KnobSize) : knob->GetSize();

        bool guiInteracted = false;

        if(!inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT))
            SliderDragging = false;

        //If the user clicked on the knob, record the offset
        if(knob != nullptr && knob->GetButtonState() == ssGUI::Enums::ButtonState::ON_CLICK && IsInteractable())
        {
            CursorKnobOffset = 
                IsVertical() ?
                inputInterface->GetCurrentMousePosition(static_cast<ssGUI::MainWindow*>(mainWindow)).y - KnobGlobalPosition.y : 
                inputInterface->GetCurrentMousePosition(static_cast<ssGUI::MainWindow*>(mainWindow)).x - KnobGlobalPosition.x;
        }
        //If the user is dragging the knob, update the position
        else if(knob != nullptr && ((knob->GetButtonState() == ssGUI::Enums::ButtonState::CLICKING && IsInteractable() && IsBlockInput()) || SliderDragging))
        {
            if(IsVertical())
            {
                KnobGlobalPosition = glm::vec2
                (
                    GetGlobalPosition().x + (GetSize().x - curKnobSize.x) * 0.5,
                    inputInterface->GetCurrentMousePosition(static_cast<ssGUI::MainWindow*>(mainWindow)).y - CursorKnobOffset
                );

                float topY = GetGlobalPosition().y + GetEndPadding();
                float botY = GetGlobalPosition().y + GetSize().y - curKnobSize.y - GetEndPadding();

                //Do bound checks
                if(KnobGlobalPosition.y < topY)
                    KnobGlobalPosition = glm::vec2(KnobGlobalPosition.x, topY);
                else if(KnobGlobalPosition.y > botY)
                    KnobGlobalPosition = glm::vec2(KnobGlobalPosition.x, botY);
            }
            else
            {
                KnobGlobalPosition = glm::vec2
                (
                    inputInterface->GetCurrentMousePosition(static_cast<ssGUI::MainWindow*>(mainWindow)).x - CursorKnobOffset,
                    GetGlobalPosition().y + (GetSize().y - curKnobSize.y) * 0.5
                );

                float leftX = GetGlobalPosition().x + GetEndPadding();
                float rightX = GetGlobalPosition().x + GetSize().x - curKnobSize.x - GetEndPadding();

                //Do bound checks
                if(KnobGlobalPosition.x < leftX)
                    KnobGlobalPosition = glm::vec2(leftX, KnobGlobalPosition.y);
                else if(KnobGlobalPosition.x > rightX)
                    KnobGlobalPosition = glm::vec2(rightX, KnobGlobalPosition.y);
            }

            knob->SetGlobalPosition(KnobGlobalPosition);

            knob->SetFocus(true);
            
            float oriSliderValue = GetSliderValue();

            //Update the slider value according to the slider position
            UpdateSliderValue();

            //Record if slider value is changed via GUI
            guiInteracted = oriSliderValue == GetSliderValue() ? false : true;

            //Apply snapping
            ApplySnapping();

            //Reposition the knob
            UpdateKnobPosition(false);

            inputStatus.MouseInputBlockedObject = this;
        }
        else
        {
            //Check if user clicked on the slider instead. If so, move the knob to the cursor
            glm::vec2 mousePos = inputInterface->GetCurrentMousePosition(static_cast<ssGUI::MainWindow*>(mainWindow));
            
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
            UpdateKnobPosition(true);

        LastSliderDragging = SliderDragging;
        LastGlobalPosition = GetGlobalPosition();
        LastSize = GetSize();

        if(guiInteracted)
        {
            if(IsAnyEventCallbackExist<ssGUI::EventCallbacks::SliderValueChangedViaGuiEventCallback>())
                GetAnyEventCallback<ssGUI::EventCallbacks::SliderValueChangedViaGuiEventCallback>()->Notify(static_cast<ssGUI::GUIObject*>(this));
        }

        if(LastSliderValue != GetSliderValue())
        {
            if(IsAnyEventCallbackExist<ssGUI::EventCallbacks::SliderValueChangedEventCallback>())
                GetAnyEventCallback<ssGUI::EventCallbacks::SliderValueChangedEventCallback>()->Notify(static_cast<ssGUI::GUIObject*>(this));

            LastValueChanged = true;
        }
        else if(LastValueChanged)
        {
            if(IsAnyEventCallbackExist<ssGUI::EventCallbacks::SliderValueFinishedChangingEventCallback>())
            {
                GetAnyEventCallback<ssGUI::EventCallbacks::SliderValueFinishedChangingEventCallback>()->
                    Notify(static_cast<ssGUI::GUIObject*>(this));
            }
            
            LastValueChanged = false;
        }

        LastSliderValue = GetSliderValue();

        ssLOG_FUNC_EXIT();
    }

    const std::string Slider::ListenerKey = "Slider";
    
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
                        KnobGlobalPosition(),
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
        auto rc = ssGUI::Factory::Create<ssGUI::Extensions::RoundedCorners>();
        rc->SetRoundedCornersRadius(10);
        rc->AddTargetShape(1);
        AddExtension(rc);

        auto outline = ssGUI::Factory::Create<ssGUI::Extensions::Outline>();
        AddExtension(outline);

        //Create knob object
        auto button = ssGUI::Factory::Create<ssGUI::Button>();
        button->SetMinSize(glm::vec2(5, 5));
        button->SetSize(glm::vec2(KnobSize, KnobSize));
        button->SetButtonColor(GetBackgroundColor());
        
        auto rc2 = ssGUI::Factory::Create<ssGUI::Extensions::RoundedCorners>();
        rc2->SetRoundedCornersRadius(KnobSize);
        button->AddExtension(rc2);

        button->RemoveAnyExtension<ssGUI::Extensions::Border>();
        auto outline2 = ssGUI::Factory::Create<ssGUI::Extensions::Outline>();
        outline2->SetOutlineThickness(1.5);
        button->SetUserCreated(false);
        button->AddExtension(outline2);
        button->SetParent(this, true);

        auto ecb = button->GetAnyEventCallback<ssGUI::EventCallbacks::ButtonStateChangedEventCallback>();
        ecb->ClearEventListeners();
        ecb->AddEventListener
        (
            ListenerKey, this,
            [](ssGUI::EventInfo info)
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
        UpdateKnobPosition(true);
    }

    Slider::~Slider()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
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

    void Slider::SetKnobObject(ssGUI::GUIObject* knob)
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

    ssGUI::GUIObject* Slider::GetKnobObject() const
    {
        return CurrentObjectsReferences.GetObjectReference(KnobObject);
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
        ssLOG_FUNC_ENTRY();
        SliderValue = sliderValue;
        SliderValue = SliderValue < 0 ? 0 : SliderValue;
        SliderValue = SliderValue > 1 ? 1 : SliderValue;

        ApplySnapping();
        UpdateKnobPosition(false);

        ssLOG_FUNC_EXIT();
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

    ssGUI::Enums::GUIObjectType Slider::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::WIDGET | ssGUI::Enums::GUIObjectType::SLIDER;
    }

    Slider* Slider::Clone(bool cloneChildren)
    {
        ssLOG_FUNC_ENTRY();
        Slider* temp = new Slider(*this);
        CloneExtensionsAndEventCallbacks(temp);   
        
        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
            {
                ssLOG_FUNC_EXIT();
                return nullptr;
            }
        }

        ssLOG_FUNC_EXIT();
        return temp;
    }
}