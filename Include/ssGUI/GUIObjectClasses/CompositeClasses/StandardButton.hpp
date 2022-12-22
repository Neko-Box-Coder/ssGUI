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
                        textColor.r = (uint8_t)(textColor.r + buttonReactAmount * 4 & 255);
                        textColor.g = (uint8_t)(textColor.g + buttonReactAmount * 4 & 255);
                        textColor.b = (uint8_t)(textColor.b + buttonReactAmount * 4 & 255);
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
    =================================================================
    */
    class StandardButton : public Button
    {
        public:
            //Enum: ssGUI::StandardButton::Mode
            enum class Mode
            {
                TEXT,
                ICON,
                BOTH
            };

        private:
            StandardButton& operator=(StandardButton const& other) = default;

        protected:
            ssGUIObjectIndex ButtonText;                //See <GetButtonTextObject>
            ssGUIObjectIndex ButtonImage;               //See <GetButtonIconObject>
            bool AdaptiveButtonTextColor;               //See <IsAdaptiveButtonTextColor>
            glm::ivec4 ButtonTextColorDifference;       //See <GetAdaptiveButtonTextColorDifference>
            bool AdaptiveButtonTextContrast;            //See <IsAdaptiveButtonTextContrast>
            Mode ButtonMode;                            //See <GetButtonMode>

            ssGUIObjectIndex ButtonImageWrapper;        //(Internal variable) Allow the button image's size to be set by AdvancedSize

            StandardButton(StandardButton const& other);

            virtual void UpdateButtonText();
            virtual void UpdateButtonImage();

        public:
            //string: ListenerKey
            static const std::string ListenerKey;

            StandardButton();
            virtual ~StandardButton() override;

            //function: SetButtonIconObject
            //Sets the icon image object, by default it will be generated
            virtual void SetButtonIconObject(ssGUI::Image* image);
            
            //function: GetButtonIconObject
            //Gets the icon image object, by default it will be generated
            virtual ssGUI::Image* GetButtonIconObject() const;

            //function: SetButtonTextObject
            //Sets the text object, by default it will be generated
            virtual void SetButtonTextObject(ssGUI::Text* text);
            
            //function: GetButtonTextObject
            //Gets the text object, by default it will be generated
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

            //function: SetButtonMode
            //Sets the button mode for displaying text or icon or both
            virtual void SetButtonMode(Mode buttonMode);

            //function: GetButtonMode
            //Gets the button mode for displaying text or icon or both
            virtual Mode GetButtonMode() const;

            //function: SetButtonColor
            //See <Button::SetButtonColor>
            virtual void SetButtonColor(glm::u8vec4 color) override;
            
            //function: GetType
            //See <Widget::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: Clone
            //See <Widget::Clone>
            virtual StandardButton* Clone(bool cloneChildren) override;
    };
}

#endif