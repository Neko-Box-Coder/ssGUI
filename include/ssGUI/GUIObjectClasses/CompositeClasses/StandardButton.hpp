#ifndef SSGUI_STANDARD_BUTTPM
#define SSGUI_STANDARD_BUTTPM

#include "ssGUI/GUIObjectClasses/Text.hpp"
#include "ssGUI/GUIObjectClasses/Button.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/RoundedCorners.hpp"
#include "ssGUI/Extensions/Outline.hpp"
#include "ssGUI/Extensions/BoxShadow.hpp"

//namespace: ssGUI
namespace ssGUI
{
    /*class: StandardButton
    A standard button contains button text and with addtional visual extensions added.

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUIObjectIndex ButtonText;
        bool AdaptiveButtonTextColor;
        glm::ivec4 ButtonTextColorDifference;
        bool AdaptiveButtonTextContrast;
    =================================================================
    ============================== C++ ==============================
    StandardButton::StandardButton() : ButtonText(-1), AdaptiveButtonTextColor(true), ButtonTextColorDifference(glm::ivec4(0, 0, 0, 0)), AdaptiveButtonTextContrast(true)
    {
        SetMinSize(glm::vec2(50, 50));

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
                    case ssGUI::Enums::ButtonState::CLICKING:
                        bgcolor.r = bgcolor.r - buttonReactAmount * 2 < 0 ? 0 : bgcolor.r - buttonReactAmount * 2;
                        bgcolor.g = bgcolor.g - buttonReactAmount * 2 < 0 ? 0 : bgcolor.g - buttonReactAmount * 2;
                        bgcolor.b = bgcolor.b - buttonReactAmount * 2 < 0 ? 0 : bgcolor.b - buttonReactAmount * 2;
                        btn->SetBackgroundColor(bgcolor);
                        break;
                    case ssGUI::Enums::ButtonState::CLICKED:
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
    }
    =================================================================
    */
    class StandardButton : public Button
    {
        private:
            StandardButton& operator=(StandardButton const& other) = default;

        protected:
            ssGUIObjectIndex ButtonText;
            bool AdaptiveButtonTextColor;
            glm::ivec4 ButtonTextColorDifference;
            bool AdaptiveButtonTextContrast;

            StandardButton(StandardButton const& other);

            virtual void UpdateButtonText();

        public:
            StandardButton();
            virtual ~StandardButton() override;

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
            //Sets the button text color difference to button color manually (ButtonTextColor - ButtonColor)
            //This is normally set automatically when you call <SetAdaptiveButtonTextColor>
            virtual void SetAdaptiveButtonTextColorDifference(glm::ivec4 difference);

            //function: GetAdaptiveButtonTextColorDifference
            //Gets the button text color difference to button color (ButtonTextColor - ButtonColor)
            virtual glm::ivec4 GetAdaptiveButtonTextColorDifference() const;

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