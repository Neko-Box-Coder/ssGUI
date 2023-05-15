#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardSlider.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/Layout.hpp"

namespace 
{
    std::string ValueToString(bool toInt, int decimalPlaces, float value)
    {
        std::string displayString = std::to_string(value);
        int decimalIndex = displayString.find('.');
        
        if(toInt || decimalPlaces <= 0)
            displayString.resize(decimalIndex);
        else
            displayString.resize(decimalIndex + 1 + decimalPlaces, '0');
        
        return displayString;
    }
}

namespace ssGUI
{
    StandardSlider::StandardSlider(StandardSlider const& other) :   SliderTitleTextObject(other.SliderTitleTextObject),
                                                                    SliderObject(other.SliderObject),
                                                                    SliderDisplayValueTextObject(other.SliderDisplayValueTextObject),
                                                                    MinDisplayValue(other.MinDisplayValue),
                                                                    MaxDisplayValue(other.MaxDisplayValue),
                                                                    DisplayInteger(other.DisplayInteger),
                                                                    DisplayDecimalPlaces(other.DisplayDecimalPlaces)
    {
    }
    
    void StandardSlider::ConstructRenderInfo()
    {
        //Doesn't need to render anything
    }
    
    void StandardSlider::AddDisplayValueEventCallback()
    {
        ssGUI::Slider* slider = GetSliderObject();
        if(slider == nullptr)
            return;
    
        auto* ecb = slider->AddEventCallback(ssGUI::Enums::EventType::SLIDER_VALUE_CHANGED);
        ssGUIObjectIndex holderId = ecb->AddObjectReference(this);
        ecb->AddEventListener
        (
            ListenerKey,
            this,
            [holderId, this](ssGUI::EventInfo info)
            {
                auto* slider = static_cast<ssGUI::Slider*>(info.Container);
                auto* standardSlider = info.References->GetObjectReference<ssGUI::StandardSlider>(holderId);

                if(standardSlider == nullptr || standardSlider->GetDisplayValueTextObject() == nullptr)
                {
                    ssGUI::EventCallback* ecb = slider->GetEventCallback(ssGUI::Enums::EventType::SLIDER_VALUE_CHANGED);
                    ecb->RemoveEventListener(ListenerKey, this);
                    return;
                }
                
                standardSlider->GetDisplayValueTextObject()->SetText(ValueToString( standardSlider->IsDisplayIntegerValue(), 
                                                                                    standardSlider->GetDisplayDecimalPlaces(), 
                                                                                    standardSlider->GetDisplayValue()));
            }
        );
        
        //Forward events from slider to this
        slider->ForwardEvent(this, Enums::EventType::SLIDER_VALUE_CHANGED);
        slider->ForwardEvent(this, Enums::EventType::SLIDER_VALUE_CHANGED_VIA_GUI);
        slider->ForwardEvent(this, Enums::EventType::SLIDER_VALUE_FINISHED_CHANGING);
    }
    
    //void StandardSlider::RemoveDisplayValueEventCallback()
    //{
    //    ssGUI::Slider* slider = GetSliderObject();
    //    if(slider == nullptr)
    //        return;
        
    //    ssGUI::EventCallback* ecb = slider->GetEventCallback(ssGUI::Enums::EventType::SLIDER_VALUE_CHANGED);
        
    //    if(ecb != nullptr)
    //        ecb->RemoveEventListener(ListenerKey, this);
    //}
    
    void StandardSlider::UpdateDisplayTextContent()
    {
        ssGUI::Slider* slider = GetSliderObject();
        if(slider == nullptr)
            return;
        
        ssGUI::EventCallback* ecb = slider->GetEventCallback(ssGUI::Enums::EventType::SLIDER_VALUE_CHANGED);
        if(ecb != nullptr)
            ecb->Notify(slider);
    }
    
    const std::string StandardSlider::ListenerKey = "Standard Slider";
    
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
        
        SetMinSize(glm::vec2(200, 10));
        SetSize(glm::vec2(300, 10));

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

        auto* sliderValue = AddChild<ssGUI::Text>();
        sliderValue->SetText(ValueToString(DisplayInteger, DisplayDecimalPlaces, slider->GetSliderValue()));
        SliderDisplayValueTextObject = CurrentObjectsReferences.AddObjectReference(sliderValue);
   
        AddDisplayValueEventCallback();
    }

    StandardSlider::~StandardSlider()
    {
        
    }
    
    void StandardSlider::SetTitleTextObject(ssGUI::Text* text)
    {
        if(text == nullptr)
        {
            if(GetTitleTextObject() != nullptr)
                GetTitleTextObject()->Delete();
            
            return;
        }
    
        std::wstring oldTitle = L"";
    
        if(GetTitleTextObject() != nullptr)
        {
            oldTitle = GetTitleTextObject()->GetText();
            text->SetParent(GetTitleTextObject()->GetParent(), true);
            MoveChildAfterTargetChild(text, GetTitleTextObject());
            GetTitleTextObject()->Delete();
        }
        else
        {
            text->SetParent(this, true);
            MoveChildToFirst(text);
        }
        
        text->SetText(oldTitle);

        SliderTitleTextObject = CurrentObjectsReferences.AddObjectReference(text);   
    }
    
    ssGUI::Text* StandardSlider::GetTitleTextObject() const
    {
        return CurrentObjectsReferences.GetObjectReference<ssGUI::Text>(SliderTitleTextObject);
    }
    
    void StandardSlider::SetSliderObject(ssGUI::Slider* slider)
    {
        if(slider == nullptr)
        {
            if(GetSliderObject() != nullptr)
                GetSliderObject()->Delete();
            
            return;
        }
        
        if(GetSliderObject() != nullptr)
        {
            slider->SetParent(GetSliderObject()->GetParent(), true);
            GetSliderObject()->Delete();
        }
        else
        {
            ssGUI::GUIObject* wrapper;
            if(GetTitleTextObject() != nullptr)
                wrapper = AddChildAfter<ssGUI::GUIObject>(GetTitleTextObject());
            else
                wrapper = AddChild<ssGUI::GUIObject>();
        
            slider->SetParent(wrapper, true);
        }

        SliderObject = CurrentObjectsReferences.AddObjectReference(slider);
        AddDisplayValueEventCallback();
    }
    
    ssGUI::Slider* StandardSlider::GetSliderObject() const
    {
        return CurrentObjectsReferences.GetObjectReference<ssGUI::Slider>(SliderObject);
    }
    
    void StandardSlider::SetDisplayValueTextObject(ssGUI::Text* text)
    {
        if(text == nullptr)
        {
            if(GetDisplayValueTextObject() != nullptr)
                GetDisplayValueTextObject()->Delete();
            
            return;
        } 
    
        if(GetDisplayValueTextObject() != nullptr)
        {
            text->SetParent(GetDisplayValueTextObject()->GetParent(), true);
            MoveChildAfterTargetChild(text, GetDisplayValueTextObject());
            GetDisplayValueTextObject()->Delete();
        }
        else
        {
            text->SetParent(this, true);
            MoveChildToLast(text);
        }

        SliderDisplayValueTextObject = CurrentObjectsReferences.AddObjectReference(text);
        UpdateDisplayTextContent();
    }
    
    ssGUI::Text* StandardSlider::GetDisplayValueTextObject() const
    {
        return CurrentObjectsReferences.GetObjectReference<ssGUI::Text>(SliderDisplayValueTextObject);
    }
    
    void StandardSlider::SetMinDisplayValue(float min)
    {
        if(min >= MaxDisplayValue)
            return;

        MinDisplayValue = min;
        UpdateDisplayTextContent();
    }
    
    float StandardSlider::GetMinDisplayValue() const
    {
        return MinDisplayValue;
    }
    
    void StandardSlider::SetMaxDisplayValue(float max)
    {
        if(max <= MinDisplayValue)
            return;
            
        MaxDisplayValue = max;        
        UpdateDisplayTextContent();
    }
    
    float StandardSlider::GetMaxDisplayValue() const
    {
        return MaxDisplayValue;
    }
    
    void StandardSlider::SetMinMaxDisplayValue(float min, float max)
    {
        if(min >= max)
            return;

        MinDisplayValue = min;
        MaxDisplayValue = max;
        UpdateDisplayTextContent();
    }

    void StandardSlider::SetDisplayValue(float displayValue)
    {
        if(GetSliderObject() == nullptr)
        {
            ssGUI_WARNING(ssGUI_GUI_OBJECT_TAG, "Slider Object is not set");
            return;
        }
        
        displayValue = displayValue > MaxDisplayValue ? MaxDisplayValue : displayValue;
        displayValue = displayValue < MinDisplayValue ? MinDisplayValue : displayValue;
        
        GetSliderObject()->SetSliderValue((displayValue - MinDisplayValue) / (MaxDisplayValue - MinDisplayValue));
    }
    
    float StandardSlider::GetDisplayValue() const
    {
        if(GetSliderObject() == nullptr)
        {
            ssGUI_WARNING(ssGUI_GUI_OBJECT_TAG, "Slider Object is not set");
            return 0.f;
        }

        float length = MaxDisplayValue - MinDisplayValue;
        float displayVal = MinDisplayValue + GetSliderObject()->GetSliderValue() * length;
        return DisplayInteger ? std::round(displayVal) : displayVal;
    }
    
    void StandardSlider::SetDisplayStepValue(float step)
    {
        if(GetSliderObject() == nullptr)
        {
            ssGUI_WARNING(ssGUI_GUI_OBJECT_TAG, "Slider Object is not set");
            return;
        }
        
        if(step > (MaxDisplayValue - MinDisplayValue))
            step = MaxDisplayValue - MinDisplayValue;
        
        GetSliderObject()->SetSnapInterval((step) / (MaxDisplayValue - MinDisplayValue));   
    }
    
    float StandardSlider::GetDisplayStepValue() const
    {
        if(GetSliderObject() == nullptr)
        {
            ssGUI_WARNING(ssGUI_GUI_OBJECT_TAG, "Slider Object is not set");
            return 0.f;
        }
        
        return GetSliderObject()->GetSnapInterval() * (MaxDisplayValue - MinDisplayValue);
    }
    
    void StandardSlider::SetDisplayIntegerValue(bool displayInteger)
    {
        DisplayInteger = displayInteger;
    }
    
    bool StandardSlider::IsDisplayIntegerValue() const
    {
        return DisplayInteger;
    }
    
    void StandardSlider::SetDisplayDecimalPlaces(int decimalPlaces)
    {
        DisplayDecimalPlaces = decimalPlaces;
    }
    
    int StandardSlider::GetDisplayDecimalPlaces() const
    {
        return DisplayDecimalPlaces;
    }
    
    void StandardSlider::SetInteractable(bool interactable)
    {
        if(GetTitleTextObject() != nullptr)
            GetTitleTextObject()->SetInteractable(interactable);
            
        if(GetSliderObject() != nullptr)
            GetSliderObject()->SetInteractable(interactable);
        
        if(GetDisplayValueTextObject() != nullptr)
            GetDisplayValueTextObject()->SetInteractable(interactable);
        
        Widget::SetInteractable(interactable);
    }
    
    void StandardSlider::SetBlockInput(bool blockInput)
    {
        if(GetSliderObject() != nullptr)
            GetSliderObject()->SetBlockInput(blockInput);
        
        Widget::SetBlockInput(blockInput);
    }
    
    ssGUI::Enums::GUIObjectType StandardSlider::GetType() const
    {
        return  ssGUI::Enums::GUIObjectType::STANDARD_SLIDER |
                ssGUI::Enums::GUIObjectType::WIDGET;
    }
    
    StandardSlider* StandardSlider::Clone(bool cloneChildren)
    {
        ssLOG_FUNC_ENTRY();
        StandardSlider* temp = new StandardSlider(*this);
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