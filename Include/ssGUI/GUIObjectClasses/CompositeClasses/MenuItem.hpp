#ifndef SSGUI_MENU_ITEM_H
#define SSGUI_MENU_ITEM_H

#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardButton.hpp"
#include "glm/vec4.hpp"

//namespace: ssGUI
namespace ssGUI
{
    /*class: ssGUI::MenuItem
    A wrapped <ssGUI::StandardButton> class 
    
    Variables & Constructor:
    ============================== C++ ==============================
    =================================================================
    ============================== C++ ==============================
    MenuItem::MenuItem()
    {
        ssGUI_LOG_FUNC();

        RemoveExtension<ssGUI::Extensions::BoxShadow>();
        RemoveExtension<ssGUI::Extensions::RoundedCorners>();
        RemoveExtension<ssGUI::Extensions::Outline>();

        GetButtonTextObject()->SetNewTextFontSize(15);
        GetButtonTextObject()->SetTextHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::LEFT);
        GetButtonTextObject()->SetTextHorizontalPadding(15);

        auto buttonEventCallback = GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED);
        buttonEventCallback->ClearEventListeners();
        buttonEventCallback->AddEventListener
        (
            ListenerKey, this,
            [](ssGUI::EventInfo& info)
            {
                ssGUI::StandardButton* btn = static_cast<ssGUI::StandardButton*>(info.EventCallbackContainer);
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
                    case ssGUI::Enums::ButtonState::NOT_INTERACTABLE:
                        bgcolor.r = bgcolor.r + buttonReactAmount > 255 ? 255 : bgcolor.r + buttonReactAmount;
                        bgcolor.g = bgcolor.g + buttonReactAmount > 255 ? 255 : bgcolor.g + buttonReactAmount;
                        bgcolor.b = bgcolor.b + buttonReactAmount > 255 ? 255 : bgcolor.b + buttonReactAmount;
                        btn->SetBackgroundColor(bgcolor);
                        auto textColor = btn->GetButtonTextObject()->GetNewTextColor();
                        textColor.r = (uint8_t)(textColor.r + buttonReactAmount * 4 & 255);
                        textColor.g = (uint8_t)(textColor.g + buttonReactAmount * 4 & 255);
                        textColor.b = (uint8_t)(textColor.b + buttonReactAmount * 4 & 255);
                        btn->GetButtonTextObject()->SetNewTextColor(textColor);
                        btn->GetButtonTextObject()->ApplyNewTextSettingsToExistingText();
                        break;
                }
            }
        );
    }
    =================================================================
    */
    class MenuItem : public StandardButton
    {
        private:
            MenuItem& operator=(MenuItem const& other) = default;

        protected:
            MenuItem(MenuItem const& other);

            virtual void MainLogic( ssGUI::Backend::BackendSystemInputInterface* inputInterface, 
                                    ssGUI::InputStatus& currentInputStatus, 
                                    const ssGUI::InputStatus& lastInputStatus, 
                                    ssGUI::GUIObject* mainWindow) override;

        public:
            //string: ListenerKey
            static const std::string ListenerKey;

            MenuItem();
            virtual ~MenuItem() override;
            
            //function: GetType
            //See <StandardButton::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;
            
            //function: Clone
            //See <StandardButton::Clone>
            virtual MenuItem* Clone(bool cloneChildren) override;
    };
}


#endif