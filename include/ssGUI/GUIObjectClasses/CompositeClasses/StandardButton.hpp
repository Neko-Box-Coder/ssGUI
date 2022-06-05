#ifndef SSGUI_STANDARD_BUTTPM
#define SSGUI_STANDARD_BUTTPM

#include "ssGUI/GUIObjectClasses/Text.hpp"
#include "ssGUI/GUIObjectClasses/Image.hpp"
#include "ssGUI/GUIObjectClasses/Button.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/RoundedCorners.hpp"
#include "ssGUI/Extensions/Outline.hpp"
#include "ssGUI/Extensions/BoxShadow.hpp"

//namespace: ssGUI
namespace ssGUI
{
    /*class: ssGUI::StandardButton
    A standard button contains button text and with addtional visual extensions added.

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUIObjectIndex ButtonText;
        ssGUIObjectIndex ButtonImage;
        bool AdaptiveButtonTextColor;
        glm::ivec4 ButtonTextColorDifference;
        bool AdaptiveButtonTextContrast;
        bool IconButtonMode;
    =================================================================
    ============================== C++ ==============================
    StandardButton::StandardButton() : ButtonText(-1), ButtonImage(-1), AdaptiveButtonTextColor(true), ButtonTextColorDifference(glm::ivec4(0, 0, 0, 0)), 
                                        AdaptiveButtonTextContrast(true), IconButtonMode(false)
    {
        FUNC_DEBUG_ENTRY();
        SetSize(glm::vec2(50, 50));

        //Add visual extensions
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
        ssGUI::EventCallbacks::OnObjectDestroyEventCallback* onDestroyCallback = nullptr;
        if(!IsEventCallbackExist(ssGUI::EventCallbacks::OnObjectDestroyEventCallback::EVENT_NAME))
        {
            onDestroyCallback = ssGUI::Factory::Create<ssGUI::EventCallbacks::OnObjectDestroyEventCallback>();
            AddEventCallback(onDestroyCallback);
        }

        onDestroyCallback = GetAnyEventCallback<ssGUI::EventCallbacks::OnObjectDestroyEventCallback>();
        onDestroyCallback->AddEventListener
        (
            ListenerKey, this,
            [](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* references)
            {
                auto buttonText = static_cast<ssGUI::StandardButton*>(container)->GetButtonTextObject();

                if(buttonText != nullptr && buttonText->GetParent() != container && !buttonText->Internal_IsDeleted())
                    buttonText->Delete();
            }
        );
        
        //Change button callback
        auto buttonEventCallback = GetEventCallback(ssGUI::EventCallbacks::ButtonStateChangedEventCallback::EVENT_NAME);
        buttonEventCallback->RemoveEventListener(Button::ListenerKey, this);
        buttonEventCallback->AddEventListener
        (
            ListenerKey, this,
            [](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* refs)
            {
                ssGUI::StandardButton* btn = static_cast<ssGUI::StandardButton*>(container);
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
                        auto textColor = btn->GetButtonTextObject()->GetTextColor();
                        textColor.r = (uint8_t)(textColor.r + buttonReactAmount * 4 & 255);
                        textColor.g = (uint8_t)(textColor.g + buttonReactAmount * 4 & 255);
                        textColor.b = (uint8_t)(textColor.b + buttonReactAmount * 4 & 255);
                        btn->GetButtonTextObject()->SetTextColor(textColor);
                        break;
                }
            }
        ); 

        //Add Button Image
        auto buttonImage = new ssGUI::Image();
        buttonImage->SetUserCreated(false);
        buttonImage->SetHeapAllocated(true);
        buttonImage->SetParent(this);
        buttonImage->SetMinSize(glm::vec2(5, 5));
        buttonImage->SetBackgroundColor(glm::u8vec4(0, 0, 0, 0));
        buttonImage->SetBlockInput(false);
        ButtonImage = CurrentObjectsReferences.AddObjectReference(buttonImage);

        UpdateButtonText();
        UpdateButtonImage();
        NotifyButtonEventCallbackManually();

        FUNC_DEBUG_EXIT();
    }
    =================================================================
    */
    class StandardButton : public Button
    {
        private:
            StandardButton& operator=(StandardButton const& other) = default;

        protected:
            ssGUIObjectIndex ButtonText;
            ssGUIObjectIndex ButtonImage;
            bool AdaptiveButtonTextColor;
            glm::ivec4 ButtonTextColorDifference;
            bool AdaptiveButtonTextContrast;
            bool IconButtonMode;

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

            //function: SetIconButtonMode
            //Sets if the button displays text or icon
            virtual void SetIconButtonMode(bool icon);

            //function: IsIconButtonMode
            //Returns if the button displays text or icon
            virtual bool IsIconButtonMode() const;

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