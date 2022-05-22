#ifndef SSGUI_MENU_ITEM
#define SSGUI_MENU_ITEM

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
        FUNC_DEBUG_ENTRY();

        RemoveAnyExtension<ssGUI::Extensions::BoxShadow>();
        RemoveAnyExtension<ssGUI::Extensions::RoundedCorners>();
        RemoveAnyExtension<ssGUI::Extensions::Outline>();

        SetMinSize(glm::vec2(200, 25));
        SetMaxSize(glm::vec2(200, 25));
        GetButtonTextObject()->SetFontSize(15);
        GetButtonTextObject()->SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::LEFT);
        GetButtonTextObject()->SetHorizontalPadding(15);

        auto buttonEventCallback = GetEventCallback(ssGUI::EventCallbacks::ButtonStateChangedEventCallback::EVENT_NAME);
        buttonEventCallback->ClearEventListeners();
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
                        bgcolor.r = bgcolor.r + buttonReactAmount > 255 ? 255 : bgcolor.r + buttonReactAmount;
                        bgcolor.g = bgcolor.g + buttonReactAmount > 255 ? 255 : bgcolor.g + buttonReactAmount;
                        bgcolor.b = bgcolor.b + buttonReactAmount > 255 ? 255 : bgcolor.b + buttonReactAmount;
                        btn->SetBackgroundColor(bgcolor);
                        auto textColor = btn->GetButtonTextObject()->GetTextColor();
                        textColor.r = (uint8_t)(textColor.r + buttonReactAmount * 4 & 255);
                        textColor.g = (uint8_t)(textColor.g + buttonReactAmount * 4 & 255);
                        textColor.b = (uint8_t)(textColor.b + buttonReactAmount * 4 & 255);
                        btn->GetButtonTextObject()->SetTextColor(textColor);
                        break;
                }
            }); 

        FUNC_DEBUG_EXIT();
    }
    =================================================================
    */
    class MenuItem : public StandardButton
    {
        private:
            MenuItem& operator=(MenuItem const& other) = default;

        protected:
            MenuItem(MenuItem const& other);

        public:
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