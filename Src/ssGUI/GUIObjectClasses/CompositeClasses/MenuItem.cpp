#include "ssGUI/GUIObjectClasses/CompositeClasses/MenuItem.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

#include "ssGUI/Extensions/Outline.hpp"
#include "ssGUI/Extensions/BoxShadow.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/Extensions/RoundedCorners.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{
    MenuItem::MenuItem(MenuItem const& other) : StandardButton(other)
    {
    }

    void MenuItem::MainLogic(   ssGUI::Backend::BackendSystemInputInterface* inputInterface, 
                                ssGUI::InputStatus& currentInputStatus, 
                                const ssGUI::InputStatus& lastInputStatus, 
                                ssGUI::GUIObject* mainWindow)
    {
        ssGUI::StandardButton::MainLogic(inputInterface, currentInputStatus, lastInputStatus, mainWindow);

        //Use normal cursor for menu item instead of "click" cursor if hovered
        if( currentInputStatus.MouseInputBlockedData.GetBlockDataType() == ssGUI::Enums::BlockDataType::GUI_OBJECT &&
            currentInputStatus.MouseInputBlockedData.GetBlockData<ssGUI::GUIObject>() == this)
        {
            if(inputInterface->GetCursorType() == ssGUI::Enums::CursorType::HAND)
                inputInterface->SetCursorType(ssGUI::Enums::CursorType::NORMAL);
        }
    }

    const std::string MenuItem::ListenerKey = "Menu Item";
    
    MenuItem::MenuItem()
    {
        ssGUI_LOG_FUNC();

        RemoveExtension<ssGUI::Extensions::BoxShadow>();
        RemoveExtension<ssGUI::Extensions::RoundedCorners>();
        RemoveExtension<ssGUI::Extensions::Outline>();

        GetButtonTextObject()->SetNewTextFontSize(15);
        GetButtonTextObject()->SetTextHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::LEFT);
        GetButtonTextObject()->SetTextHorizontalPadding(15);

        auto buttonEventCallback = GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED);
        buttonEventCallback->ClearEventListeners();
        buttonEventCallback->AddEventListener
        (
            ListenerKey, this,
            [](ssGUI::EventInfo& info)
            {
                ssGUI::StandardButton* btn = static_cast<ssGUI::StandardButton*>(info.Container);
                int buttonReactAmount = 20;
                glm::u8vec4 bgcolor = btn->GetButtonColor();
                
                static_assert((int)ssGUI::Enums::ButtonState::COUNT == 6, "Make sure this is updated");
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
                        textColor.r = (uint8_t)((textColor.r + buttonReactAmount * 4) & 255);
                        textColor.g = (uint8_t)((textColor.g + buttonReactAmount * 4) & 255);
                        textColor.b = (uint8_t)((textColor.b + buttonReactAmount * 4) & 255);
                        btn->GetButtonTextObject()->SetNewTextColor(textColor);
                        btn->GetButtonTextObject()->ApplyNewTextSettingsToExistingText();
                        break;
                }
            }
        );
    }

    MenuItem::~MenuItem()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
        //NOTE: Children resources deallocation is Handled in Slider
    }

    ssGUI::Enums::GUIObjectType MenuItem::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::WIDGET | ssGUI::Enums::GUIObjectType::BUTTON | 
            ssGUI::Enums::GUIObjectType::STANDARD_BUTTON | ssGUI::Enums::GUIObjectType::MENU_ITEM;
    }

    MenuItem* MenuItem::Clone(bool cloneChildren)
    {
        ssGUI_LOG_FUNC();
        MenuItem* temp = new MenuItem(*this);
        CloneExtensionsAndEventCallbacks(temp);   
        
        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
                return nullptr;
        }

        return temp;
    }
}