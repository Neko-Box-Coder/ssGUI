#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardButton.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position
#include "ssGUI/EventCallbacks/OnObjectDestroyEventCallback.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/RoundedCorners.hpp"
#include "ssGUI/Extensions/Outline.hpp"
#include "ssGUI/Extensions/BoxShadow.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/ssGUITags.hpp"

#include "ssLogger/ssLog.hpp"

namespace ssGUI
{
    StandardButton::StandardButton(StandardButton const& other) : Button(other)
    {
        ButtonText = other.ButtonText;
        ButtonImage = other.ButtonImage;
        AdaptiveButtonTextColor = other.IsAdaptiveButtonTextColor();
        ButtonTextColorDifference = other.GetAdaptiveButtonTextColorDifference();
        AdaptiveButtonTextContrast = other.IsAdaptiveButtonTextContrast();
        ButtonMode = other.GetButtonMode();
        ButtonImageWrapper = other.ButtonImageWrapper;
    }
    
    void StandardButton::UpdateButtonText()
    {
        ssLOG_FUNC_ENTRY();
        auto buttonTextObj = CurrentObjectsReferences.GetObjectReference(ButtonText);
        if(buttonTextObj == nullptr)
        {
            ssLOG_FUNC_EXIT();
            return;
        }
        
        if(GetButtonMode() == StandardButton::Mode::ICON)
        {
            buttonTextObj->SetEnabled(false);
            ssLOG_FUNC_EXIT();
            return;
        }

        buttonTextObj->SetEnabled(true);
        // if(!buttonTextObj->HasTag(ssGUI::Tags::FLOATING))
            // buttonTextObj->AddTag(ssGUI::Tags::FLOATING);

        // ssGUI::Extensions::AdvancedSize* as;
        
        // if(!buttonTextObj->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME))
        //     buttonTextObj->AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::AdvancedSize>());

        static_cast<ssGUI::Text*>(buttonTextObj)->SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::CENTER);
        static_cast<ssGUI::Text*>(buttonTextObj)->SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::CENTER);
        ssLOG_FUNC_EXIT();
    }

    void StandardButton::UpdateButtonImage()
    {
        ssLOG_FUNC_ENTRY();
        auto buttonImgObj = CurrentObjectsReferences.GetObjectReference(ButtonImage);
        if(buttonImgObj == nullptr)
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        auto buttonImgWrapper = CurrentObjectsReferences.GetObjectReference(ButtonImageWrapper);
        if(buttonImgWrapper == nullptr)
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        if(GetButtonMode() == StandardButton::Mode::TEXT)
        {
            buttonImgWrapper->SetEnabled(false);
            ssLOG_FUNC_EXIT();
            return;
        }
        
        buttonImgWrapper->SetEnabled(true);
        // if(!buttonImgWrapper->HasTag(ssGUI::Tags::FLOATING))
            // buttonImgWrapper->AddTag(ssGUI::Tags::FLOATING);
        
        ssGUI::Extensions::AdvancedSize* as;
        ssGUI::Extensions::AdvancedPosition* ap;
        
        if(!buttonImgObj->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME))
            buttonImgObj->AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::AdvancedSize>());

        as = buttonImgObj->GetAnyExtension<ssGUI::Extensions::AdvancedSize>();

        as->SetHorizontalPercentage(0.55);
        as->SetHorizontalPixel(0);
        as->SetVerticalPercentage(0.55);
        as->SetVerticalPixel(0);

        if(!buttonImgObj->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME))
            buttonImgObj->AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::AdvancedPosition>());

        ap = buttonImgObj->GetAnyExtension<ssGUI::Extensions::AdvancedPosition>();
        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);

        ssLOG_FUNC_EXIT();
    }

    const std::string StandardButton::ListenerKey = "Standard Button";

    StandardButton::StandardButton() :  ButtonText(-1),
                                        ButtonImage(-1),
                                        AdaptiveButtonTextColor(true),
                                        ButtonTextColorDifference(0, 0, 0, 0),
                                        AdaptiveButtonTextContrast(true),
                                        ButtonMode(StandardButton::Mode::TEXT),
                                        ButtonImageWrapper(-1)
    {
        ssLOG_FUNC_ENTRY();
        SetSize(glm::vec2(70, 35));

        //Adjust Extensions
        RemoveExtension(ssGUI::Extensions::Border::EXTENSION_NAME);

        auto boxShadow = ssGUI::Factory::Create<ssGUI::Extensions::BoxShadow>();
        AddExtension(boxShadow);

        auto roundedCorners = ssGUI::Factory::Create<ssGUI::Extensions::RoundedCorners>();
        roundedCorners->SetRoundedCornersRadius(5);
        AddExtension(roundedCorners);

        auto outline = ssGUI::Factory::Create<ssGUI::Extensions::Outline>();
        outline->SetSimpleOutline(false);
        outline->SetOutlineColor(glm::u8vec4(0, 0, 0, 127));
        outline->SetOutlineThickness(1);
        AddExtension(outline);

        auto layout = ssGUI::Factory::Create<ssGUI::Extensions::Layout>();
        layout->SetHorizontalLayout(true);
        layout->SetSpacing(0);
        layout->AddPreferredSizeMultiplier(0.25);
        layout->AddPreferredSizeMultiplier(0.75);
        AddExtension(layout);

        //Add Button Image
        auto wrapper = ssGUI::Factory::Create<ssGUI::Widget>();
        wrapper->SetParent(this, true);
        wrapper->SetBlockInput(false);
        wrapper->SetUserCreated(false);
        wrapper->SetBackgroundColor(glm::u8vec4(0, 0, 0, 25));
        ButtonImageWrapper = CurrentObjectsReferences.AddObjectReference(wrapper);

        auto buttonImage = new ssGUI::Image();
        buttonImage->SetUserCreated(false);
        buttonImage->SetHeapAllocated(true);
        buttonImage->SetParent(wrapper);
        buttonImage->SetMinSize(glm::vec2(5, 5));
        buttonImage->SetBackgroundColor(glm::u8vec4(0, 0, 0, 0));
        buttonImage->SetBlockInput(false);
        ButtonImage = CurrentObjectsReferences.AddObjectReference(buttonImage);

        //Add button text
        auto buttonText = new ssGUI::Text();
        buttonText->SetUserCreated(false);
        buttonText->SetHeapAllocated(true);
        buttonText->SetParent(this, true);
        buttonText->SetMinSize(glm::vec2(5, 5));
        buttonText->SetNewCharacterColor(glm::u8vec4(255, 255, 255, 255));
        ButtonText = CurrentObjectsReferences.AddObjectReference(buttonText);
        SetAdaptiveButtonTextColor(true);   //Update the text color

        //Add button text clean-up
        ssGUI::EventCallbacks::OnObjectDestroyEventCallback* onDestroyCallback = nullptr;
        if(!IsEventCallbackExist(ssGUI::EventCallbacks::OnObjectDestroyEventCallback::EVENT_NAME))
        {
            onDestroyCallback = ssGUI::Factory::Create<ssGUI::EventCallbacks::OnObjectDestroyEventCallback>();
            AddEventCallback(onDestroyCallback);
        }

        //Change button callback
        auto buttonEventCallback = GetEventCallback(ssGUI::EventCallbacks::ButtonStateChangedEventCallback::EVENT_NAME);
        buttonEventCallback->RemoveEventListener(Button::ListenerKey, this);
        buttonEventCallback->AddEventListener
        (
            ListenerKey, this,
            [](ssGUI::EventInfo info)
            {
                ssGUI::StandardButton* btn = static_cast<ssGUI::StandardButton*>(info.EventCallbackContainer);
                auto iconImage = btn->GetButtonIconObject();
                int buttonReactAmount = 20;
                glm::u8vec4 bgcolor = btn->GetButtonColor();
                glm::u8vec4 iconTintColor = iconImage == nullptr ? glm::u8vec4(0,0,0,0) : iconImage->GetImageTint();
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
                    case ssGUI::Enums::ButtonState::DISABLED:
                        bgcolor.r = bgcolor.r + buttonReactAmount > 255 ? 255 : bgcolor.r + buttonReactAmount;
                        bgcolor.g = bgcolor.g + buttonReactAmount > 255 ? 255 : bgcolor.g + buttonReactAmount;
                        bgcolor.b = bgcolor.b + buttonReactAmount > 255 ? 255 : bgcolor.b + buttonReactAmount;
                        btn->SetBackgroundColor(bgcolor);
                        if(iconImage != nullptr)
                        {
                            iconTintColor.a = 255 - buttonReactAmount * 2;
                            iconImage->SetImageTint(iconTintColor);
                        }
                        auto textColor = btn->GetButtonTextObject()->GetNewCharacterColor();
                        textColor.r = (uint8_t)((textColor.r + buttonReactAmount * 4) & 255);
                        textColor.g = (uint8_t)((textColor.g + buttonReactAmount * 4) & 255);
                        textColor.b = (uint8_t)((textColor.b + buttonReactAmount * 4) & 255);
                        btn->GetButtonTextObject()->SetNewCharacterColor(textColor);
                        btn->GetButtonTextObject()->ApplyNewCharacterSettingsToText();
                        break;
                }
            }
        ); 

        UpdateButtonText();
        UpdateButtonImage();
        NotifyButtonEventCallbackManually();

        ssLOG_FUNC_EXIT();
    }

    StandardButton::~StandardButton()
    {        
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();

        //If the object deallocation is not handled by ssGUIManager
        if(!Internal_IsDeleted())
            Internal_ManualDeletion(std::vector<ssGUI::ssGUIObjectIndex>{ButtonText, ButtonImage, ButtonImageWrapper});
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
            ButtonImage = -1;
            ssLOG_FUNC_EXIT();
            return;
        }

        glm::vec2 globalPos = image->GetGlobalPosition();
        image->SetParent(buttonImgWrapper);
        image->SetGlobalPosition(globalPos);

        ssGUIObjectIndex newImageIndex = CurrentObjectsReferences.GetObjectIndex(image);

        if(newImageIndex != -1)
            ButtonImage = newImageIndex;
        else
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
        SetAdaptiveButtonTextColorDifference(buttonTextObj->GetNewCharacterColor() - GetButtonColor());

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

    void StandardButton::SetButtonMode(Mode buttonMode)
    {
        ButtonMode = buttonMode;
        UpdateButtonText();
        UpdateButtonImage();
    }

    StandardButton::Mode StandardButton::GetButtonMode() const
    {
        return ButtonMode;
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

        glm::ivec4 textResult;

        if(IsAdaptiveButtonTextContrast())
        {
            float averageButtonColor = (GetButtonColor().r + GetButtonColor().g + GetButtonColor().b) / (float)3;
            float averageTextDiffColor = (GetAdaptiveButtonTextColorDifference().r + GetAdaptiveButtonTextColorDifference().g + GetAdaptiveButtonTextColorDifference().b) / (float)3;
            int contrastFactor = averageButtonColor + averageTextDiffColor > 255 ? -1 : 1; 
            textResult = (glm::ivec4)GetButtonColor() + GetAdaptiveButtonTextColorDifference() * contrastFactor;
        }
        else
            textResult = (glm::ivec4)color + GetAdaptiveButtonTextColorDifference();

        textResult.r = textResult.r < 0 ? 0 : textResult.r;
        textResult.r = textResult.r > 255 ? 255 : textResult.r;
        textResult.g = textResult.g < 0 ? 0 : textResult.g;
        textResult.g = textResult.g > 255 ? 255 : textResult.g;
        textResult.b = textResult.b > 255 ? 255 : textResult.b;
        textResult.b = textResult.b < 0 ? 0 : textResult.b;
        textResult.a = textResult.a < 0 ? 0 : textResult.a;
        textResult.a = textResult.a > 255 ? 255 : textResult.a;

        GetButtonTextObject()->SetNewCharacterColor(glm::u8vec4((uint8_t)textResult.r, (uint8_t)textResult.g, (uint8_t)textResult.b, (uint8_t)textResult.a));
        GetButtonTextObject()->ApplyNewCharacterSettingsToText();
    }

    ssGUI::Enums::GUIObjectType StandardButton::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::WIDGET | ssGUI::Enums::GUIObjectType::BUTTON |
            ssGUI::Enums::GUIObjectType::STANDARD_BUTTON;
    }

    StandardButton* StandardButton::Clone(bool cloneChildren)
    {
        ssLOG_FUNC_ENTRY();
        StandardButton* temp = new StandardButton(*this);
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