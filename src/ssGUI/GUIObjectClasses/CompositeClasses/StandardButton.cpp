#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardButton.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

namespace ssGUI
{
    StandardButton::StandardButton(StandardButton const& other) : Button(other)
    {
        ButtonText = other.ButtonText;
        ButtonColor = other.ButtonColor;
    }
    
    void StandardButton::UpdateButtonText()
    {
        auto buttonTextObj = CurrentObjectsReferences.GetObjectReference(ButtonText);
        if(buttonTextObj == nullptr)
            return;
        
        ssGUI::Extensions::AdvancedSize* as;
        
        if(!buttonTextObj->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME))
            buttonTextObj->AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::AdvancedSize>());

        as = static_cast<ssGUI::Extensions::AdvancedSize*>(buttonTextObj->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        as->SetHorizontalUsePercentage(true);
        as->SetHorizontalPercentage(1);
        as->SetVerticalUsePercentage(true);
        as->SetVerticalPercentage(1);

        static_cast<ssGUI::Text*>(buttonTextObj)->SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::CENTER);
        static_cast<ssGUI::Text*>(buttonTextObj)->SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::CENTER);
    }

    StandardButton::StandardButton() : ButtonText(-1), ButtonColor(glm::u8vec4(255, 255, 255, 255))
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
        outline->SetOutlineColor(glm::u8vec4(255, 255, 255, 255));
        outline->SetOutlineThickness(2);
        AddExtension(outline);
        SetBackgroundColor(ButtonColor);

        //Add button text
        auto buttonText = new ssGUI::Text();
        buttonText->SetUserCreated(false);
        buttonText->SetHeapAllocated(true);
        buttonText->SetParent(this);
        buttonText->SetMinSize(glm::vec2(5, 5));
        //TODO : Change text color
        ButtonText = CurrentObjectsReferences.AddObjectReference(buttonText);

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
                    case ssGUI::Enums::ButtonState::CLICKED:
                    case ssGUI::Enums::ButtonState::ON_CLICK:
                    case ssGUI::Enums::ButtonState::CLICKING:
                        bgcolor.r = bgcolor.r - buttonReactAmount * 2 < 0 ? 0 : bgcolor.r - buttonReactAmount * 2;
                        bgcolor.g = bgcolor.g - buttonReactAmount * 2 < 0 ? 0 : bgcolor.g - buttonReactAmount * 2;
                        bgcolor.b = bgcolor.b - buttonReactAmount * 2 < 0 ? 0 : bgcolor.b - buttonReactAmount * 2;
                        btn->SetBackgroundColor(bgcolor);
                        break;
                    case ssGUI::Enums::ButtonState::DISABLED:
                        bgcolor.r = bgcolor.r + buttonReactAmount < 0 ? 0 : bgcolor.r - buttonReactAmount * 3;
                        bgcolor.g = bgcolor.g + buttonReactAmount < 0 ? 0 : bgcolor.g - buttonReactAmount * 3;
                        bgcolor.b = bgcolor.b + buttonReactAmount < 0 ? 0 : bgcolor.b - buttonReactAmount * 3;
                        btn->SetBackgroundColor(bgcolor);
                        break;
                }
            }); 

        UpdateButtonText();
    }

    StandardButton::~StandardButton()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
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

        text->SetParent(this);

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

    void StandardButton::SetButtonColor(glm::u8vec4 color)
    {
        ButtonColor = color;
    }
    
    glm::u8vec4 StandardButton::GetButtonColor()
    {
        return ButtonColor;
    }

    //You don't have to override this. If you do want to return your own type, 
    //just edit the GUIObjectType and return it along with WIDGET
    ssGUI::Enums::GUIObjectType StandardButton::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::WIDGET | ssGUI::Enums::GUIObjectType::BUTTON |
            ssGUI::Enums::GUIObjectType::STANDARD_BUTTON;
    }

    //You don't have to override this. But if you have any claen-up put it here as well.
    void StandardButton::Delete()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
        ssGUI::Button::Delete();
    }

    //You will always need to override this in order to call the copy constructor
    StandardButton* StandardButton::Clone(bool cloneChildren)
    {
        FUNC_DEBUG_ENTRY();
        StandardButton* temp = new StandardButton(*this);
        CloneExtensionsAndEventCallbacks(temp);   
        
        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
            {
                FUNC_DEBUG_EXIT();
                return nullptr;
            }
        }

        FUNC_DEBUG_EXIT();
        return temp;
    }
}