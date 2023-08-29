#ifndef SSGUI_STANDARD_SLIDER_HPP
#define SSGUI_STANDARD_SLIDER_HPP

#include "ssGUI/GUIObjectClasses/CompositeClasses/Slider.hpp"
#include "ssGUI/GUIObjectClasses/Text.hpp"
#include "ssGUI/GUIObjectClasses/TextField.hpp"

//namespace: ssGUI
namespace ssGUI
{
    /*class: StandardSlider
    A standard slider that contains a title text, slider object and value text.
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUIObjectIndex SliderTitleTextObject;             //See <GetTitleTextObject>
        ssGUIObjectIndex SliderObject;                      //See <GetSliderObject>
        ssGUIObjectIndex SliderDisplayValueTextObject;      //See <GetDisplayValueTextObject>

        float MinDisplayValue;                              //See <GetMinDisplayValue>
        float MaxDisplayValue;                              //See <GetMaxDisplayValue>
        bool DisplayInteger;                                //See <IsDisplayIntegerValue>
        int DisplayDecimalPlaces;                           //See <GetDisplayDecimalPlaces>
    =================================================================
    ============================== C++ ==============================
    StandardSlider::StandardSlider() :  SliderTitleTextObject(-1),
                                        SliderObject(-1),
                                        SliderDisplayValueTextObject(-1),
                                        MinDisplayValue(0.f),
                                        MaxDisplayValue(1.f),
                                        DisplayInteger(false),
                                        DisplayDecimalPlaces(3)
    {
        //Add layout
        auto* layout = AddExtension<ssGUI::Extensions::Layout>();
        layout->SetUpdateContainerMinMaxSize(false);
        layout->SetHorizontalLayout(true);
        
        SetMinSize(glm::vec2(200, 30));
        SetSize(glm::vec2(300, 30));

        //Set layout preferred sizes
        layout->AddPreferredSizeMultipliers(0.25f, 0.5f, 0.15f);
        
        //Add components
        auto* sliderTitle = AddChild<ssGUI::Text>();
        sliderTitle->SetText("Slider:");
        SliderTitleTextObject = CurrentObjectsReferences.AddObjectReference(sliderTitle);
        
        auto* slider = AddChildWithWrapper<ssGUI::Slider>(  ssGUI::Enums::AlignmentHorizontal::CENTER, 
                                                            ssGUI::Enums::AlignmentVertical::CENTER,
                                                            true);
                                                            
        float sliderHeight = slider->GetSize().y;
        auto* sliderAS = slider->AddExtension<ssGUI::Extensions::AdvancedSize>();
        sliderAS->SetHorizontalPercentage(0.9f);
        sliderAS->SetVerticalPixel(sliderHeight);
        SliderObject = CurrentObjectsReferences.AddObjectReference(slider);

        auto* sliderValue = AddChild<ssGUI::TextField>();
        sliderValue->RemoveExtension<ssGUI::Extensions::Outline>();
        sliderValue->RemoveExtension<ssGUI::Extensions::BoxShadow>();
        sliderValue->RemoveExtension<ssGUI::Extensions::RoundedCorners>();
        sliderValue->SetBackgroundColor(glm::u8vec4());
        sliderValue->SetText(ValueToString(DisplayInteger, DisplayDecimalPlaces, slider->GetSliderValue()));
        SliderDisplayValueTextObject = CurrentObjectsReferences.AddObjectReference(sliderValue);
   
        AddDisplayValueEventCallback();
        AddTextModifiedEventCallback();
    }
    =================================================================
    */
    class StandardSlider : public Widget
    {
        private:
            StandardSlider& operator=(StandardSlider const& other) = default;
        
        protected:
            ssGUIObjectIndex SliderTitleTextObject;             //See <GetTitleTextObject>
            ssGUIObjectIndex SliderObject;                      //See <GetSliderObject>
            ssGUIObjectIndex SliderDisplayValueTextObject;      //See <GetDisplayValueTextObject>

            float MinDisplayValue;                              //See <GetMinDisplayValue>
            float MaxDisplayValue;                              //See <GetMaxDisplayValue>
            bool DisplayInteger;                                //See <IsDisplayIntegerValue>
            int DisplayDecimalPlaces;                           //See <GetDisplayDecimalPlaces>
            
            StandardSlider(StandardSlider const& other);
    
            virtual void ConstructRenderInfo() override;

            virtual void AddDisplayValueEventCallback();
            virtual void AddTextModifiedEventCallback();
            //virtual void RemoveDisplayValueEventCallback();
            virtual void UpdateDisplayTextContent();

        public:
            static const std::string ListenerKey;

            StandardSlider();
            virtual ~StandardSlider() override;

            /*function: SetTitleTextObject
            Sets the title text GUI Object. 
            The text content of the old text GUI Object will be copied to the new one.
            The old text GUI Object will be deleted and the new text GUI Object will be inserted at the same place as the old one.
            Passing nullptr will unset the text GUI object.*/
            virtual void SetTitleTextObject(ssGUI::Text* text);
            
            //function: GetTitleTextObject
            //Gets the title text GUI Object. 
            virtual ssGUI::Text* GetTitleTextObject() const;
            
            /*function: SetSliderObject
            Sets the actual slider GUI object.
            The old slider GUI Object will be deleted and the new slider GUI Object will be inserted at the same place as the old one.
            Passing nullptr will unset the slider GUI object.*/
            virtual void SetSliderObject(ssGUI::Slider* slider);
            
            //function: GetSliderObject
            //Gets the actual slider GUI object.
            virtual ssGUI::Slider* GetSliderObject() const;
            
            /*function: SetDisplayValueTextObject
            Sets the display value text for showing the value of the slider.
            The old slider GUI Object will be deleted and the new text GUI Object will be inserted at the same place as the old one.
            Passing nullptr will unset the display value text GUI object.*/
            virtual void SetDisplayValueTextObject(ssGUI::TextField* text);
            
            //function: GetDisplayValueTextObject
            //Gets the display value text for showing the value of the slider.
            virtual ssGUI::TextField* GetDisplayValueTextObject() const;

            //function: SetMinDisplayValue
            //Sets the minimum display value that is mapped to 0 for the actual slider value.
            virtual void SetMinDisplayValue(float min);
            
            //function: GetMinDisplayValue
            //Gets the minimum display value that is mapped to 0 for the actual slider value.
            virtual float GetMinDisplayValue() const;

            //function: SetMaxDisplayValue
            //Sets the maximum display value that is mapped to 1 for the actual slider value.
            virtual void SetMaxDisplayValue(float max);
            
            //function: GetMaxDisplayValue
            //Gets the maximum display value that is mapped to 1 for the actual slider value.
            virtual float GetMaxDisplayValue() const;

            //function: SetMinMaxDisplayValue
            //See <SetMinDisplayValue> and <SetMaxDisplayValue>
            virtual void SetMinMaxDisplayValue(float min, float max);
            
            //function: SetDisplayValue
            //Sets the display value which is translated to <ssGUI::Slider::SetSliderValue> from 
            //the mapped minimum and maximum display value.
            virtual void SetDisplayValue(float displayValue);
            
            //function: GetDisplayValue
            //Gets the display value which is translated from <ssGUI::Slider::GetSliderValue> from 
            //the mapped minimum and maximum display value.
            virtual float GetDisplayValue() const;
            
            //function: SetDisplayStepValue
            //Sets the stepping value which is translated to <ssGUI::Slider::SetSnapInterval> from 
            //the mapped minimum and maximum display value.
            virtual void SetDisplayStepValue(float step);
            
            //function: GetDisplayStepValue
            //Gets the stepping value which is translated from <ssGUI::Slider::GetSnapInterval> from 
            //the mapped minimum and maximum display value.
            virtual float GetDisplayStepValue() const;
            
            /*function: SetDisplayIntegerValue
            Sets if integer is shown instead of float.
            Please note that the underlying display value is still float, 
            but just rounded to integer when displaying in value text GUI Object.*/
            virtual void SetDisplayIntegerValue(bool displayInteger);
            
            //function: IsDisplayIntegerValue
            //Returns if integer is shown instead of float.
            virtual bool IsDisplayIntegerValue() const;
            
            /*function: SetDisplayDecimalPlaces
            Sets how many decimal places are shown. 
            This has no effect if <IsDisplayIntegerValue> is false.
            Please note that the underlying display value is not affected by this, 
            but just discarding the extra decimals when displaying in value text GUI Object.*/
            virtual void SetDisplayDecimalPlaces(int decimalPlaces);
            
            //function: GetDisplayDecimalPlaces
            //Returns how many decimal places are shown. 
            virtual int GetDisplayDecimalPlaces() const;

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
            virtual StandardSlider* Clone(bool cloneChildren) override;
        
    };
}

#endif