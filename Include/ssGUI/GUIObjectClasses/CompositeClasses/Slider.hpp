#ifndef SSGUI_SLIDER_H
#define SSGUI_SLIDER_H

#include "ssGUI/GUIObjectClasses/Widget.hpp"

//namespace: ssGUI
namespace ssGUI
{
    class Button;
    /*class: ssGUI::Slider
    This class represents a slider GUI widget. A slider object can be either horizontal or vertical, 
    and can also start from either end of the slider using the <SetReverse> function.
    This can also be used as a status bar using the fill color functions provided and unassign the knob object.

    The default orientation of the slider is from left (0) to right (1) if horizontal or bottom (0) to top (1) if vertical.

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        bool Reverse;                       //See <IsReverse>
        glm::u8vec4 FillColor;              //See <GetFillColor>
        ssGUIObjectIndex KnobObject;        //See <GetKnobObject>
        float KnobSize;                     //See <GetKnobSize>
        float SliderValue;                  //See <GetSliderValue>
        bool Vertical;                      //See <IsVertical>
        float SnapInterval;                 //See <GetSnapInterval>
        float ScrollInternal;               //See <GetScrollInterval>
        float KeyInputInterval;             //See <GetKeyInputMoveInterval>
        float EndPadding;                   //See <GetEndPadding>

        glm::vec2 KnobGlobalOffset;         //(Internal variable) This is used for syncing the slider value and knob position
        float CursorKnobOffset;             //(Internal variable) Offset between the cursor down position and the knob
        bool LastSliderDragging;            //(Internal variable) Used to see if the slider is being dragged last frame
        bool SliderDragging;                //(Internal variable) Flag if slider is being dragged right now
        glm::vec2 LastGlobalPosition;       //(Internal variable) Used to update the knob position accordingly if the size or position changed
                                            //TODO: Use <ssGUI::GUIObject::LastGlobalPosition> instead maybe?
        glm::vec2 LastSize;                 //(Internal variable) Used to update the knob position accordingly if the size or position changed
        float LastSliderValue;              //(Internal variable) Used to trigger <SliderValueChangedEventCallback>
        bool LastValueChanged;              //(Internal variable) Used to trigger <SliderValueFinishedChangingEventCallback>

        uint64_t LastKeyNavStartTime;       //(Internal variable) Used to control slider with keys
        int KeyNavPauseDuration;            //(Internal variable) Used to control slider with keys
        uint64_t LastKeyNavTime;            //(Internal variable) Used to control slider with keys
        int KeyNavInterval;                 //(Internal variable) Used to control slider with keys
    =================================================================
    ============================== C++ ==============================
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
    =================================================================
    */
    class Slider : public Widget
    {
        private:
            Slider& operator=(Slider const& other) = default;

        protected:
            bool Reverse;                       //See <IsReverse>
            glm::u8vec4 FillColor;              //See <GetFillColor>
            ssGUIObjectIndex KnobObject;        //See <GetKnobObject>
            float KnobSize;                     //See <GetKnobSize>
            float SliderValue;                  //See <GetSliderValue>
            bool Vertical;                      //See <IsVertical>
            float SnapInterval;                 //See <GetSnapInterval>
            float ScrollInternal;               //See <GetScrollInterval>
            float KeyInputInterval;             //See <GetKeyInputMoveInterval>
            float EndPadding;                   //See <GetEndPadding>

            glm::vec2 KnobGlobalOffset;         //(Internal variable) This is used for syncing the slider value and knob position
            float CursorKnobOffset;             //(Internal variable) Offset between the cursor down position and the knob
            bool LastSliderDragging;            //(Internal variable) Used to see if the slider is being dragged last frame
            bool SliderDragging;                //(Internal variable) Flag if slider is being dragged right now
            glm::vec2 LastGlobalPosition;       //(Internal variable) Used to update the knob position accordingly if the size or position changed
                                                //TODO: Use <ssGUI::GUIObject::LastGlobalPosition> instead maybe?
            glm::vec2 LastSize;                 //(Internal variable) Used to update the knob position accordingly if the size or position changed
            float LastSliderValue;              //(Internal variable) Used to trigger <SliderValueChangedEventCallback>
            bool LastValueChanged;              //(Internal variable) Used to trigger <SliderValueFinishedChangingEventCallback>

            uint64_t LastKeyNavStartTime;       //(Internal variable) Used to control slider with keys
            int KeyNavPauseDuration;            //(Internal variable) Used to control slider with keys
            uint64_t LastKeyNavTime;            //(Internal variable) Used to control slider with keys
            int KeyNavInterval;                 //(Internal variable) Used to control slider with keys

            Slider(Slider const& other);

            virtual void ApplySnapping();

            virtual void UpdateKnobOffset();

            virtual void UpdateSliderValueFromCursor(glm::vec2 cursorPos);

            virtual void ConstructRenderInfo() override;

            virtual void MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                                    ssGUI::GUIObject* mainWindow) override;
        public:
            //string: ListenerKey
            static const std::string ListenerKey;
            static const std::string SLIDER_FILL_SHAPE_NAME;

            Slider();
            virtual ~Slider() override;

            //function: SetReverse
            //Sets if the slider value is starting from the opposite side.
            //If true, it will start from right to left or from top to bottom if vertical is true.
            //Vice versa if false.
            virtual void SetReverse(bool reverse);

            //function: IsReverse
            //If true, it will start from right to left or from top to bottom if vertical is true.
            //Vice versa if false.
            virtual bool IsReverse() const;

            //function: SetFillColor
            //Sets the filling color between starting point to the knob object.
            virtual void SetFillColor(glm::u8vec4 fillColor);

            //function: GetFillColor
            //Gets the filling color between starting point to the knob object.
            virtual glm::u8vec4 GetFillColor() const;

            //function: SetKnobObject
            //Sets knob object. This will automatically delete the previous assigned knob object.
            //Setting nullptr is possible for not using any knob object, useful for status bar or something like that.
            virtual void SetKnobObject(ssGUI::Button* knob);

            //function: GetKnobObject
            //Gets knob object. Nullptr if no knob object is assigned.
            virtual ssGUI::Button* GetKnobObject() const;

            //function: SetKnobSize
            //Helper function for setting the diameter of the knob.
            //Equivalent to SetSize(glm::vec2(knobSize, knobSize)) for the knob object and
            //also store locally and used as a substitude value if there's no knob object assigned.
            virtual void SetKnobSize(float knobSize, bool updateKnobObject);

            //function: GetKnobSize
            //Helper function for getting the diameter of the knob.
            //Equivalent to GetSize() for the knob object.
            //This can return local knob size if no knob object is assigned.
            virtual float GetKnobSize() const;

            //function: SetEndPadding
            //Sets the number of pixels padding for both ends of the slider
            virtual void SetEndPadding(float padding);

            //function: GetEndPadding
            //Gets the number of pixels padding for both ends of the slider
            virtual float GetEndPadding() const;

            //function: SetSliderValue
            //Sets the value of the slider, ranging from 0 to 1.
            virtual void SetSliderValue(float sliderValue);

            //function: GetSliderValue
            //Gets the value of the slider, ranging from 0 to 1.
            virtual float GetSliderValue() const;

            //function: SetVertical
            //Sets if the slider is operating vertically or not.
            //There's an extra option of flipping the width height field of the slider.
            virtual void SetVertical(bool vertical, bool swapWidthHeight);

            //function: IsVertical
            //Returns if the slider is operating vertically or not.
            virtual bool IsVertical() const;

            //function: SetSnapInterval
            //Sets the interval of snapping. 0 or less to disable.
            //It is possible for <GetSliderValue> to have a slight offset depending on the interval, 
            //using rounding function is advised when using the value of it. 
            virtual void SetSnapInterval(float interval);

            //function: GetSnapInterval
            //Gets the interval of snapping. 0 or less to disable.
            //It is possible for <GetSliderValue> to have a slight offset depending on the interval, 
            //using rounding function is advised when using the value of it. 
            virtual float GetSnapInterval() const;

            //function: SetScrollInterval
            //Sets the interval for each scroll input. 
            //This only limits to move in an interval so this is different from snapping.
            virtual void SetScrollInterval(float interval);

            //function: GetScrollInterval
            //Gets the interval for each scroll input. 
            //This only limits to move in an interval so this is different from snapping.
            virtual float GetScrollInterval() const;

            //function: SetKeyInputMoveInterval
            //Sets the interval for each arrow or wasd key input. 
            //This only limits to move in an interval so this is different from snapping.
            virtual void SetKeyInputMoveInterval(float interval);

            //function: GetKeyInputMoveInterval
            //Gets the interval for each arrow or wasd key input. 
            //This only limits to move in an interval so this is different from snapping.
            virtual float GetKeyInputMoveInterval() const;

            //function: SetInteractable
            //See <Widget::SetInteractable>
            virtual void SetInteractable(bool interactable) override;

            //function: SetBlockInput
            //See <Widget::SetBlockInput>
            virtual void SetBlockInput(bool blockInput) override;

            //function: GetType
            //See <Widget::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: Clone
            //See <Widget::Clone>
            virtual Slider* Clone(bool cloneChildren) override;
    };
}

#endif