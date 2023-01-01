#include "ssGUI/GUIObjectClasses/CompositeClasses/MenuItem.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

#include "ssGUI/Extensions/Outline.hpp"
#include "ssGUI/Extensions/BoxShadow.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/Extensions/RoundedCorners.hpp"

#include "ssLogger/ssLog.hpp"

namespace ssGUI
{
    MenuItem::MenuItem(MenuItem const& other) : StandardButton(other)
    {
    }

    void MenuItem::MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                            ssGUI::GUIObject* mainWindow)
    {
        ssGUI::StandardButton::MainLogic(inputInterface, inputStatus, mainWindow);

        //Use normal cursor for menu item instead of "click" cursor if hovered
        if(inputStatus.MouseInputBlockedObject == this)
        {
            if(inputInterface->GetCursorType() == ssGUI::Enums::CursorType::HAND)
                inputInterface->SetCursorType(ssGUI::Enums::CursorType::NORMAL);
        }
    }

    const std::string MenuItem::ListenerKey = "Menu Item";
    
    MenuItem::MenuItem()
    {
        ssLOG_FUNC_ENTRY();

        RemoveAnyExtension<ssGUI::Extensions::BoxShadow>();
        RemoveAnyExtension<ssGUI::Extensions::RoundedCorners>();
        RemoveAnyExtension<ssGUI::Extensions::Outline>();

        GetButtonTextObject()->SetNewCharacterFontSize(15);
        GetButtonTextObject()->SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::LEFT);
        GetButtonTextObject()->SetHorizontalPadding(15);

        auto buttonEventCallback = GetEventCallback(ssGUI::EventCallbacks::ButtonStateChangedEventCallback::EVENT_NAME);
        buttonEventCallback->ClearEventListeners();
        buttonEventCallback->AddEventListener
        (
            ListenerKey, this,
            [](ssGUI::EventInfo info)
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
                    case ssGUI::Enums::ButtonState::DISABLED:
                        bgcolor.r = bgcolor.r + buttonReactAmount > 255 ? 255 : bgcolor.r + buttonReactAmount;
                        bgcolor.g = bgcolor.g + buttonReactAmount > 255 ? 255 : bgcolor.g + buttonReactAmount;
                        bgcolor.b = bgcolor.b + buttonReactAmount > 255 ? 255 : bgcolor.b + buttonReactAmount;
                        btn->SetBackgroundColor(bgcolor);
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

        ssLOG_FUNC_EXIT();
    }

    MenuItem::~MenuItem()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
    }

    ssGUI::Enums::GUIObjectType MenuItem::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::WIDGET | ssGUI::Enums::GUIObjectType::BUTTON | 
            ssGUI::Enums::GUIObjectType::STANDARD_BUTTON | ssGUI::Enums::GUIObjectType::MENU_ITEM;
    }

    MenuItem* MenuItem::Clone(bool cloneChildren)
    {
        ssLOG_FUNC_ENTRY();
        MenuItem* temp = new MenuItem(*this);
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