#ifndef SSGUI_STANDARD_BUTTON_H
#define SSGUI_STANDARD_BUTTON_H

#include "ssGUI/GUIObjectClasses/Text.hpp"
#include "ssGUI/GUIObjectClasses/Image.hpp"
#include "ssGUI/GUIObjectClasses/Button.hpp"

//namespace: ssGUI
namespace ssGUI
{
    /*class: ssGUI::StandardButton
    A standard button contains button text and with addtional visual extensions added.

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUIObjectIndex ButtonText;                //See <GetButtonTextObject>
        ssGUIObjectIndex ButtonImage;               //See <GetButtonIconObject>
        bool AdaptiveButtonTextColor;               //See <IsAdaptiveButtonTextColor>
        glm::ivec4 ButtonTextColorDifference;       //See <GetAdaptiveButtonTextColorDifference>
        bool AdaptiveButtonTextContrast;            //See <IsAdaptiveButtonTextContrast>
        Mode ButtonMode;                            //See <GetButtonMode>

        ssGUIObjectIndex ButtonImageWrapper;        //(Internal variable) Allow the button image's size to be set by AdvancedSize
    =================================================================
    ============================== C++ ==============================
    StandardButton::StandardButton() :  ButtonText(-1),
                                        ButtonImage(-1),
                                        AdaptiveButtonTextColor(true),
                                        ButtonTextColorDifference(0, 0, 0, 0),
                                        AdaptiveButtonTextContrast(true),
                                        ButtonMode(StandardButton::Mode::TEXT),
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
    =================================================================
    */
    class StandardButton : public Button
    {
        private:
            StandardButton& operator=(StandardButton const& other) = default;

        protected:
            ssGUIObjectIndex ButtonText;                //See <GetButtonTextObject>
            ssGUIObjectIndex ButtonImage;               //See <GetButtonIconObject>
            bool AdaptiveButtonTextColor;               //See <IsAdaptiveButtonTextColor>
            glm::ivec4 ButtonTextColorDifference;       //See <GetAdaptiveButtonTextColorDifference>
            bool AdaptiveButtonTextContrast;            //See <IsAdaptiveButtonTextContrast>

            ssGUIObjectIndex ButtonImageWrapper;        //(Internal variable) Allow the button image's size to be set by AdvancedSize

            StandardButton(StandardButton const& other);

            virtual void UpdateButtonText();
            virtual void UpdateButtonImage();
            virtual glm::u8vec4 GetAdaptiveTextColor() const;

        public:
            //string: ListenerKey
            static const std::string ListenerKey;

            StandardButton();
            virtual ~StandardButton() override;

            //function: SetButtonIconObject
            //Sets the icon image object, by default it will be generated.
            //The new image object will be automatically parent to a wrapper.
            //Passing nullptr will unset the button icon object.
            virtual void SetButtonIconObject(ssGUI::Image* image);
            
            //function: GetButtonIconObject
            //Gets the icon image object, by default it will be generated.
            virtual ssGUI::Image* GetButtonIconObject() const;

            //function: SetButtonTextObject
            //Sets the text object, by default it will be generated.
            //The text content from the old text will be transferred to the new text object.
            //Passing nullptr will unset the button icon object.
            virtual void SetButtonTextObject(ssGUI::Text* text);
            
            //function: GetButtonTextObject
            //Gets the text object, by default it will be generated.
            virtual ssGUI::Text* GetButtonTextObject() const;

            //function: SetAdaptiveButtonTextColor
            //Sets if the button text color "adapts" to the button color dynamically.
            //You need to call this function after setting the button text color to update the color difference stored here.
            virtual void SetAdaptiveButtonTextColor(bool adaptive);

            //function: IsAdaptiveButtonTextColor
            //Returns if the button text color "adapts" to the button color dynamically
            virtual bool IsAdaptiveButtonTextColor() const;

            //function: SetAdaptiveButtonTextContrast
            //Sets if the button text color is opposite to the button color
            virtual void SetAdaptiveButtonTextContrast(bool contrast);

            //function: IsAdaptiveButtonTextContrast
            //Returns if the button text color is opposite to the button color
            virtual bool IsAdaptiveButtonTextContrast() const;

            //function: SetAdaptiveButtonTextColorDifference
            //Sets the button text color difference to button color manually (ButtonTextColor-ButtonColor)
            //This is normally set automatically when you call <SetAdaptiveButtonTextColor>
            virtual void SetAdaptiveButtonTextColorDifference(glm::ivec4 difference);

            //function: GetAdaptiveButtonTextColorDifference
            //Gets the button text color difference to button color (ButtonTextColor-ButtonColor)
            virtual glm::ivec4 GetAdaptiveButtonTextColorDifference() const;

            //function: SetButtonColor
            //See <Button::SetButtonColor>
            virtual void SetButtonColor(glm::u8vec4 color) override;
    
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
            virtual StandardButton* Clone(bool cloneChildren) override;
    };
}

#endif