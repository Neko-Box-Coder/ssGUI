#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardButton.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/RoundedCorners.hpp"
#include "ssGUI/Extensions/Outline.hpp"
#include "ssGUI/Extensions/BoxShadow.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/ssGUITags.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{
    StandardButton::StandardButton(StandardButton const& other) : Button(other)
    {
        ButtonText = other.ButtonText;
        ButtonImage = other.ButtonImage;
        AdaptiveButtonTextColor = other.IsAdaptiveButtonTextColor();
        ButtonTextColorDifference = other.GetAdaptiveButtonTextColorDifference();
        AdaptiveButtonTextContrast = other.IsAdaptiveButtonTextContrast();
        //ButtonMode = other.GetButtonMode();
        ButtonImageWrapper = other.ButtonImageWrapper;
    }
    
    void StandardButton::UpdateButtonText()
    {
        ssGUI_LOG_FUNC();
        auto* buttonTextObj = CurrentObjectsReferences.GetObjectReference(ButtonText);
        if(buttonTextObj == nullptr)
            return;
        
        MoveChildToFirst(buttonTextObj);
        
        static_cast<ssGUI::Text*>(buttonTextObj)->SetTextHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
        static_cast<ssGUI::Text*>(buttonTextObj)->SetTextVerticalAlignment(ssGUI::Enums::AlignmentVertical::CENTER);
    }

    void StandardButton::UpdateButtonImage()
    {
        ssGUI_LOG_FUNC();
        auto* buttonImgObj = CurrentObjectsReferences.GetObjectReference(ButtonImage);
        if(buttonImgObj == nullptr)
            return;

        auto* buttonImgWrapper = CurrentObjectsReferences.GetObjectReference(ButtonImageWrapper);
        if(buttonImgWrapper == nullptr)
            return;
        
        MoveChildToFirst(buttonImgWrapper);
        
        ssGUI::Extensions::AdvancedSize* as;
        ssGUI::Extensions::AdvancedPosition* ap;
        
        if(!buttonImgObj->GetExtension<ssGUI::Extensions::AdvancedSize>())
            buttonImgObj->AddExtension<ssGUI::Extensions::AdvancedSize>();

        as = buttonImgObj->GetExtension<ssGUI::Extensions::AdvancedSize>();

        as->SetHorizontalPercentage(0.55);
        as->SetHorizontalPixel(0);
        as->SetVerticalPercentage(0.55);
        as->SetVerticalPixel(0);

        if(!buttonImgObj->GetExtension<ssGUI::Extensions::AdvancedPosition>())
            buttonImgObj->AddExtension<ssGUI::Extensions::AdvancedPosition>();

        ap = buttonImgObj->GetExtension<ssGUI::Extensions::AdvancedPosition>();
        ap->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
        ap->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::CENTER);
    }

    glm::u8vec4 StandardButton::GetAdaptiveTextColor() const
    {
        glm::ivec4 textResult;

        if(IsAdaptiveButtonTextContrast())
        {
            float averageButtonColor = (GetButtonColor().r + GetButtonColor().g + GetButtonColor().b) / (float)3;
            float averageTextDiffColor = (  GetAdaptiveButtonTextColorDifference().r + 
                                            GetAdaptiveButtonTextColorDifference().g + 
                                            GetAdaptiveButtonTextColorDifference().b) / (float)3;
            
            int contrastFactor = averageButtonColor + averageTextDiffColor > 255 ? -1 : 1; 
            textResult = (glm::ivec4)GetButtonColor() + GetAdaptiveButtonTextColorDifference() * contrastFactor;
        }
        else
            textResult = (glm::ivec4)GetButtonColor() + GetAdaptiveButtonTextColorDifference();

        textResult.r = textResult.r < 0 ? 0 : textResult.r;
        textResult.r = textResult.r > 255 ? 255 : textResult.r;
        textResult.g = textResult.g < 0 ? 0 : textResult.g;
        textResult.g = textResult.g > 255 ? 255 : textResult.g;
        textResult.b = textResult.b > 255 ? 255 : textResult.b;
        textResult.b = textResult.b < 0 ? 0 : textResult.b;
        textResult.a = textResult.a < 0 ? 0 : textResult.a;
        textResult.a = textResult.a > 255 ? 255 : textResult.a;
    
        return glm::u8vec4((uint8_t)textResult.r, (uint8_t)textResult.g, (uint8_t)textResult.b, (uint8_t)textResult.a);
    }

    const std::string StandardButton::ListenerKey = "Standard Button";

    StandardButton::StandardButton() :  ButtonText(-1),
                                        ButtonImage(-1),
                                        AdaptiveButtonTextColor(true),
                                        ButtonTextColorDifference(0, 0, 0, 0),
                                        AdaptiveButtonTextContrast(true),
                                        ButtonImageWrapper(-1)
    {
        ssGUI_LOG_FUNC();
        SetSize(glm::vec2(70, 35));

        //Adjust Extensions
        RemoveExtension(ssGUI::Extensions::Border::EXTENSION_NAME);

        auto boxShadow = AddExtension<ssGUI::Extensions::BoxShadow>();

        auto roundedCorners = AddExtension<ssGUI::Extensions::RoundedCorners>();
        roundedCorners->SetRoundedCornersRadius(5);

        auto outline = AddExtension<ssGUI::Extensions::Outline>();
        outline->SetSimpleOutline(false);
        outline->SetOutlineColor(glm::u8vec4(0, 0, 0, 127));
        outline->SetOutlineThickness(1);

        auto layout = AddExtension<ssGUI::Extensions::Layout>();
        layout->SetHorizontalLayout(true);
        layout->SetSpacing(0);
        layout->AddPreferredSizeMultiplier(0.25);
        layout->AddPreferredSizeMultiplier(0.75);

        //Add button text
        auto* buttonText = AddChild<ssGUI::Text>(true);
        buttonText->SetUserCreated(false);
        buttonText->SetMinSize(glm::vec2(5, 5));
        buttonText->SetNewTextColor(glm::u8vec4(255, 255, 255, 255));
        ButtonText = CurrentObjectsReferences.AddObjectReference(buttonText);
        SetAdaptiveButtonTextColor(true);   //Update the text color

        //Add button text clean-up
        if(!IsEventCallbackExist(ssGUI::Enums::EventType::BEFORE_OBJECT_DESTROY))
            AddEventCallback(ssGUI::Enums::EventType::BEFORE_OBJECT_DESTROY);

        //Change button callback
        auto buttonEventCallback = GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED);
        buttonEventCallback->RemoveEventListener(Button::ListenerKey, this);
        buttonEventCallback->AddEventListener
        (
            ListenerKey, this,
            [](ssGUI::EventInfo& info)
            {
                ssGUI::StandardButton* btn = static_cast<ssGUI::StandardButton*>(info.Container);
                auto iconImage = btn->GetButtonIconObject();
                int buttonReactAmount = 20;
                glm::u8vec4 bgcolor = btn->GetButtonColor();
                glm::u8vec4 iconTintColor = iconImage == nullptr ? glm::u8vec4(0,0,0,0) : iconImage->GetImageTint();
                
                static_assert((int)ssGUI::Enums::ButtonState::COUNT == 6, "Make sure this is updated");
                switch(btn->GetButtonState())
                {
                    case ssGUI::Enums::ButtonState::NORMAL:
                        btn->SetBackgroundColor(bgcolor);
                        if(iconImage != nullptr)
                        {
                            iconTintColor.a = 255;
                            iconImage->SetImageTint(iconTintColor);
                        }
                        break;
                    case ssGUI::Enums::ButtonState::HOVER:
                        bgcolor.r = bgcolor.r - buttonReactAmount < 0 ? 0 : bgcolor.r - buttonReactAmount;
                        bgcolor.g = bgcolor.g - buttonReactAmount < 0 ? 0 : bgcolor.g - buttonReactAmount;
                        bgcolor.b = bgcolor.b - buttonReactAmount < 0 ? 0 : bgcolor.b - buttonReactAmount;
                        btn->SetBackgroundColor(bgcolor);
                        if(iconImage != nullptr)
                        {
                            iconTintColor.a = 255 - buttonReactAmount * 2;
                            iconImage->SetImageTint(iconTintColor);
                        }
                        break;
                    case ssGUI::Enums::ButtonState::ON_CLICK:
                        break;
                    case ssGUI::Enums::ButtonState::CLICKING:
                        bgcolor.r = bgcolor.r - buttonReactAmount * 2 < 0 ? 0 : bgcolor.r - buttonReactAmount * 2;
                        bgcolor.g = bgcolor.g - buttonReactAmount * 2 < 0 ? 0 : bgcolor.g - buttonReactAmount * 2;
                        bgcolor.b = bgcolor.b - buttonReactAmount * 2 < 0 ? 0 : bgcolor.b - buttonReactAmount * 2;
                        btn->SetBackgroundColor(bgcolor);
                        if(iconImage != nullptr)
                        {
                            iconTintColor.a = 255 - buttonReactAmount * 4;
                            iconImage->SetImageTint(iconTintColor);
                        }
                        break;
                    case ssGUI::Enums::ButtonState::CLICKED:
                        break;
                    case ssGUI::Enums::ButtonState::NOT_INTERACTABLE:
                        bgcolor.r = bgcolor.r + buttonReactAmount > 255 ? 255 : bgcolor.r + buttonReactAmount;
                        bgcolor.g = bgcolor.g + buttonReactAmount > 255 ? 255 : bgcolor.g + buttonReactAmount;
                        bgcolor.b = bgcolor.b + buttonReactAmount > 255 ? 255 : bgcolor.b + buttonReactAmount;
                        btn->SetBackgroundColor(bgcolor);
                        if(iconImage != nullptr)
                        {
                            iconTintColor.a = 255 - buttonReactAmount * 2;
                            iconImage->SetImageTint(iconTintColor);
                        }
                        auto textColor = btn->GetButtonTextObject()->GetNewTextColor();
                        textColor.r = (uint8_t)((textColor.r + buttonReactAmount * 4) & 255);
                        textColor.g = (uint8_t)((textColor.g + buttonReactAmount * 4) & 255);
                        textColor.b = (uint8_t)((textColor.b + buttonReactAmount * 4) & 255);
                        btn->GetButtonTextObject()->SetNewTextColor(textColor);
                        btn->GetButtonTextObject()->ApplyNewTextSettingsToExistingText();
                        break;
                }
            }
        ); 

        UpdateButtonText();
        UpdateButtonImage();
        NotifyButtonEventCallbackManually();
    }

    StandardButton::~StandardButton()
    {        
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();

        //If the object deallocation is not handled by ssGUIManager
        if(!Internal_IsDeleted())
            Internal_ChildrenManualDeletion(std::vector<ssGUI::ssGUIObjectIndex>{ButtonText, ButtonImage, ButtonImageWrapper});
    }

    void StandardButton::SetButtonIconObject(ssGUI::Image* image)
    {
        auto oldImageObj = static_cast<ssGUI::Image*>(CurrentObjectsReferences.GetObjectReference(ButtonImage));
        if(oldImageObj != nullptr) 
            oldImageObj->Delete();
        
        if(image == nullptr)
        {
            ButtonImage = -1;
            return;
        }

        auto buttonImgWrapper = CurrentObjectsReferences.GetObjectReference(ButtonImageWrapper);
        if(buttonImgWrapper == nullptr)
        {
            buttonImgWrapper = AddChild<ssGUI::GUIObject>(true);
            ButtonImageWrapper = CurrentObjectsReferences.AddObjectReference(buttonImgWrapper);
        }

        image->SetParent(buttonImgWrapper, true);
        ssGUIObjectIndex newImageIndex = CurrentObjectsReferences.GetObjectIndex(image);
        ButtonImage = CurrentObjectsReferences.AddObjectReference(image);
        UpdateButtonImage();
    }

    ssGUI::Image* StandardButton::GetButtonIconObject() const
    {
        return static_cast<ssGUI::Image*>(CurrentObjectsReferences.GetObjectReference(ButtonImage));
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

        glm::vec2 globalPos = text->GetGlobalPosition();
        text->SetParent(this, true);
        text->SetGlobalPosition(globalPos);

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
        SetAdaptiveButtonTextColorDifference(buttonTextObj->GetNewTextColor() - GetButtonColor());

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

        GetButtonTextObject()->SetNewTextColor(GetAdaptiveTextColor());
        GetButtonTextObject()->ApplyNewTextSettingsToExistingText();
    }
    
    void StandardButton::SetInteractable(bool interactable)
    {
        if(GetButtonTextObject() != nullptr)
            GetButtonTextObject()->SetInteractable(interactable);
        
        if(GetButtonIconObject() != nullptr)
            GetButtonIconObject()->SetInteractable(interactable);
        
        Button::SetInteractable(interactable);
    }

    void StandardButton::SetBlockInput(bool blockInput)
    {
        if(GetButtonIconObject() != nullptr)
            GetButtonIconObject()->SetBlockInput(blockInput);
        
        Button::SetBlockInput(blockInput);
    }

    ssGUI::Enums::GUIObjectType StandardButton::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::WIDGET | ssGUI::Enums::GUIObjectType::BUTTON |
            ssGUI::Enums::GUIObjectType::STANDARD_BUTTON;
    }

    StandardButton* StandardButton::Clone(bool cloneChildren)
    {
        ssGUI_LOG_FUNC();
        StandardButton* temp = new StandardButton(*this);
        CloneExtensionsAndEventCallbacks(temp);   
        
        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
                return nullptr;
        }

        return temp;
    }
}