#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardButton.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

namespace ssGUI
{
    StandardButton::StandardButton(StandardButton const& other) : Button(other)
    {
        ButtonText = other.ButtonText;
        AdaptiveButtonTextColor = other.IsAdaptiveButtonTextColor();
        ButtonTextColorDifference = other.GetAdaptiveButtonTextColorDifference();
        AdaptiveButtonTextContrast = other.IsAdaptiveButtonTextContrast();
    }
    
    void StandardButton::UpdateButtonText()
    {
        FUNC_DEBUG_ENTRY();
        auto buttonTextObj = CurrentObjectsReferences.GetObjectReference(ButtonText);
        if(buttonTextObj == nullptr)
        {
            FUNC_DEBUG_EXIT();
            return;
        }
        
        ssGUI::Extensions::AdvancedSize* as;
        
        if(!buttonTextObj->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME))
            buttonTextObj->AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::AdvancedSize>());

        as = static_cast<ssGUI::Extensions::AdvancedSize*>(buttonTextObj->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        as->SetHorizontalUsePercentage(true);
        as->SetHorizontalPercentage(1);
        as->SetVerticalUsePercentage(true);
        as->SetVerticalPercentage(1);

        static_cast<ssGUI::Text*>(buttonTextObj)->SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::CENTER);
        static_cast<ssGUI::Text*>(buttonTextObj)->SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::CENTER);
        FUNC_DEBUG_EXIT();
    }

    StandardButton::StandardButton() : ButtonText(-1), AdaptiveButtonTextColor(true), ButtonTextColorDifference(glm::ivec4(0, 0, 0, 0)), AdaptiveButtonTextContrast(true)
    {
        FUNC_DEBUG_ENTRY();
        SetSize(glm::vec2(50, 50));

        //Add visual extensions
        RemoveExtension(ssGUI::Extensions::Border::EXTENSION_NAME);

        auto boxShadow = ssGUI::Factory::Create<ssGUI::Extensions::BoxShadow>();
        boxShadow->SetShadowColor(glm::u8vec4(0, 0, 0, 127));
        AddExtension(boxShadow);

        auto roundedCorners = ssGUI::Factory::Create<ssGUI::Extensions::RoundedCorners>();
        roundedCorners->SetRoundedCornersRadius(5);
        AddExtension(roundedCorners);

        auto outline = ssGUI::Factory::Create<ssGUI::Extensions::Outline>();
        outline->SetSimpleOutline(false);
        outline->SetOutlineColor(glm::u8vec4(0, 0, 0, 127));
        outline->SetOutlineThickness(2);
        AddExtension(outline);

        //Add button text
        auto buttonText = new ssGUI::Text();
        buttonText->SetUserCreated(false);
        buttonText->SetHeapAllocated(true);
        buttonText->SetParent(this);
        buttonText->SetMinSize(glm::vec2(5, 5));
        buttonText->SetTextColor(glm::u8vec4(255, 255, 255, 255));
        ButtonText = CurrentObjectsReferences.AddObjectReference(buttonText);
        SetAdaptiveButtonTextColor(true);   //Update the text color

        //Add button text clean-up
        ssGUI::EventCallbacks::OnObjectDestroyEventCallback* callback = nullptr;
        if(IsEventCallbackExist(ssGUI::EventCallbacks::OnObjectDestroyEventCallback::EVENT_NAME))
        {
            callback = static_cast<ssGUI::EventCallbacks::OnObjectDestroyEventCallback*>
                (GetEventCallback(ssGUI::EventCallbacks::OnObjectDestroyEventCallback::EVENT_NAME));
        }
        else
        {
            callback = ssGUI::Factory::Create<ssGUI::EventCallbacks::OnObjectDestroyEventCallback>();
            AddEventCallback(callback);
        }
        
        callback->AddEventListener(
            [](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* references)
            {
                auto buttonText = static_cast<ssGUI::StandardButton*>(container)->GetButtonTextObject();

                if(buttonText != nullptr && buttonText->GetParent() != container && !buttonText->Internal_IsDeleted())
                    buttonText->Delete();
            });
        
        //Change button callback
        auto buttonEventCallback = GetEventCallback(ssGUI::EventCallbacks::ButtonStateChangedEventCallback::EVENT_NAME);
        buttonEventCallback->RemoveEventListener(0);
        buttonEventCallback->AddEventListener(
            [](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* refs)
            {
                ssGUI::StandardButton* btn = static_cast<ssGUI::StandardButton*>(container);
                int buttonReactAmount = 20;
                glm::u8vec4 bgcolor = btn->GetButtonColor();
                switch(btn->GetButtonState())
                {
                    case ssGUI::Enums::ButtonState::NORMAL:
                        btn->SetBackgroundColor(bgcolor);
                        break;
                    case ssGUI::Enums::ButtonState::HOVER:
                        bgcolor.r = bgcolor.r - buttonReactAmount < 0 ? 0 : bgcolor.r - buttonReactAmount;
                        bgcolor.g = bgcolor.g - buttonReactAmount < 0 ? 0 : bgcolor.g - buttonReactAmount;
                        bgcolor.b = bgcolor.b - buttonReactAmount < 0 ? 0 : bgcolor.b - buttonReactAmount;
                        btn->SetBackgroundColor(bgcolor);
                        break;
                    case ssGUI::Enums::ButtonState::ON_CLICK:
                        break;
                    case ssGUI::Enums::ButtonState::CLICKING:
                        bgcolor.r = bgcolor.r - buttonReactAmount * 2 < 0 ? 0 : bgcolor.r - buttonReactAmount * 2;
                        bgcolor.g = bgcolor.g - buttonReactAmount * 2 < 0 ? 0 : bgcolor.g - buttonReactAmount * 2;
                        bgcolor.b = bgcolor.b - buttonReactAmount * 2 < 0 ? 0 : bgcolor.b - buttonReactAmount * 2;
                        btn->SetBackgroundColor(bgcolor);
                        break;
                    case ssGUI::Enums::ButtonState::CLICKED:
                        break;
                    case ssGUI::Enums::ButtonState::DISABLED:
                        bgcolor.r = bgcolor.r + buttonReactAmount < 0 ? 0 : bgcolor.r - buttonReactAmount * 3;
                        bgcolor.g = bgcolor.g + buttonReactAmount < 0 ? 0 : bgcolor.g - buttonReactAmount * 3;
                        bgcolor.b = bgcolor.b + buttonReactAmount < 0 ? 0 : bgcolor.b - buttonReactAmount * 3;
                        btn->SetBackgroundColor(bgcolor);
                        break;
                }
            }); 

        SetBackgroundColor(GetButtonColor());
        UpdateButtonText();
        
        FUNC_DEBUG_EXIT();
    }

    StandardButton::~StandardButton()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
    }

    void StandardButton::SetButtonTextObject(ssGUI::Text* text)
    {
        std::wstring oldText = L"";
        auto oldTextObj = static_cast<ssGUI::Text*>(CurrentObjectsReferences.GetObjectReference(ButtonText));
        if(oldTextObj != nullptr)
        {
            oldText = oldTextObj->GetText();
            if(!oldTextObj->IsUserCreated())
                oldTextObj->Delete();
            else
                oldTextObj->SetParent(nullptr);
        }
        
        if(text == nullptr)
        {
            ButtonText = -1;
            return;
        }

        text->SetParent(this);

        ssGUIObjectIndex newTextIndex = CurrentObjectsReferences.GetObjectIndex(text);

        if(newTextIndex != -1)
            ButtonText = newTextIndex;
        else
            ButtonText = CurrentObjectsReferences.AddObjectReference(text);

        text->SetText(oldText);

        UpdateButtonText();
    }

    ssGUI::Text* StandardButton::GetButtonTextObject() const
    {
        return static_cast<ssGUI::Text*>(CurrentObjectsReferences.GetObjectReference(ButtonText));
    }

    void StandardButton::SetAdaptiveButtonTextColor(bool adaptive)
    {
        if(CurrentObjectsReferences.GetObjectReference(ButtonText) == nullptr)
            return;

        AdaptiveButtonTextColor = adaptive;
        auto buttonTextObj = dynamic_cast<ssGUI::Text*>(CurrentObjectsReferences.GetObjectReference(ButtonText));
        SetAdaptiveButtonTextColorDifference(buttonTextObj->GetTextColor() - GetButtonColor());

        SetButtonColor(GetButtonColor());   //Setting the button color to trigger the event callback
    }

    bool StandardButton::IsAdaptiveButtonTextColor() const
    {
        return AdaptiveButtonTextColor;
    }

    void StandardButton::SetAdaptiveButtonTextContrast(bool contrast)
    {
        AdaptiveButtonTextContrast = contrast;

        if(CurrentObjectsReferences.GetObjectReference(ButtonText) == nullptr)
            return;

        SetButtonColor(GetButtonColor());   //Setting the button color to trigger the event callback
    }

    bool StandardButton::IsAdaptiveButtonTextContrast() const
    {
        return AdaptiveButtonTextContrast;
    }

    void StandardButton::SetAdaptiveButtonTextColorDifference(glm::ivec4 difference)
    {
        ButtonTextColorDifference = difference;
    }

    glm::ivec4 StandardButton::GetAdaptiveButtonTextColorDifference() const
    {
        return ButtonTextColorDifference;
    }

    void StandardButton::SetButtonColor(glm::u8vec4 color)
    {
        Button::SetButtonColor(color);
                    
        if(GetButtonTextObject() == nullptr)
            return;

        if(!IsAdaptiveButtonTextColor())
        {
            GetButtonTextObject()->RedrawObject();
            return;    
        }

        glm::ivec4 textResult = (glm::ivec4)color + GetAdaptiveButtonTextColorDifference();

        if(IsAdaptiveButtonTextContrast())
        {
            if(textResult.r < 0 || textResult.r > 255)
                textResult.r = color.r - ButtonTextColorDifference.r;
            
            if(textResult.g < 0 || textResult.g > 255)
                textResult.g = color.g - ButtonTextColorDifference.g;

            if(textResult.b < 0 || textResult.b > 255)
                textResult.b = color.b - ButtonTextColorDifference.b;
        
            textResult.r = textResult.r < 0 ? 0 : textResult.r;
            textResult.r = textResult.r > 255 ? 255 : textResult.r;
            textResult.g = textResult.g < 0 ? 0 : textResult.g;
            textResult.g = textResult.g > 255 ? 255 : textResult.g;
            textResult.b = textResult.b > 255 ? 255 : textResult.b;
            textResult.b = textResult.b < 0 ? 0 : textResult.b;
            textResult.a = textResult.a < 0 ? 0 : textResult.a;
            textResult.a = textResult.a > 255 ? 255 : textResult.a;
        }
        else
        {
            textResult.r = textResult.r < 0 ? 0 : textResult.r;
            textResult.r = textResult.r > 255 ? 255 : textResult.r;
            textResult.g = textResult.g < 0 ? 0 : textResult.g;
            textResult.g = textResult.g > 255 ? 255 : textResult.g;
            textResult.b = textResult.b > 255 ? 255 : textResult.b;
            textResult.b = textResult.b < 0 ? 0 : textResult.b;
            textResult.a = textResult.a < 0 ? 0 : textResult.a;
            textResult.a = textResult.a > 255 ? 255 : textResult.a;
        }

        GetButtonTextObject()->SetTextColor((glm::u8vec4)textResult);
    }

    //You don't have to override this. If you do want to return your own type, 
    //just edit the GUIObjectType and return it along with WIDGET
    ssGUI::Enums::GUIObjectType StandardButton::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::WIDGET | ssGUI::Enums::GUIObjectType::BUTTON |
            ssGUI::Enums::GUIObjectType::STANDARD_BUTTON;
    }

    //You will always need to override this in order to call the copy constructor
    StandardButton* StandardButton::Clone(bool cloneChildren)
    {
        FUNC_DEBUG_ENTRY();
        StandardButton* temp = new StandardButton(*this);
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