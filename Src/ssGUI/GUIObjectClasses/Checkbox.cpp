#include "ssGUI/GUIObjectClasses/Checkbox.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{
    Checkbox::Checkbox(Checkbox const& other) : Button(other)
    {
        InnerBoxSpace = other.GetInnerBoxSpace();
        Checked = other.IsChecked();
    }
    
    //You only need to override this when you are rendering anything.
    void Checkbox::ConstructRenderInfo()
    {
        if(!IsChecked())
            return;
        
        //Get the top-left position of the widget 
        glm::vec2 drawPosition = GetGlobalPosition();

        auto border = GetAnyExtension<ssGUI::Extensions::Border>();
        int borderWidth = 0;
        if(border != nullptr)
            borderWidth = border->GetBorderWidth();
        float dist = borderWidth + GetInnerBoxSpace();

        ssGUI::DrawingEntity innerFiller;

        innerFiller.Vertices.push_back(drawPosition + glm::vec2(dist, dist));
        innerFiller.Colors.push_back(GetBackgroundColor());

        innerFiller.Vertices.push_back(drawPosition + glm::vec2(GetSize().x - dist, dist));
        innerFiller.Colors.push_back(GetBackgroundColor());

        innerFiller.Vertices.push_back(drawPosition + glm::vec2(GetSize().x - dist, GetSize().y - dist));
        innerFiller.Colors.push_back(GetBackgroundColor());

        innerFiller.Vertices.push_back(drawPosition + glm::vec2(dist, GetSize().y - dist));
        innerFiller.Colors.push_back(GetBackgroundColor());
    
        innerFiller.EntityName = CHECKBOX_INNER_SHAPE_NAME;
        DrawingEntities.push_back(innerFiller);
    }
    
    const std::string Checkbox::ListenerKey = "Checkbox";
    const std::string Checkbox::CHECKBOX_INNER_SHAPE_NAME = "Checkbox Inner";


    Checkbox::Checkbox() :  InnerBoxSpace(2),
                            Checked(false)
    {
        SetBackgroundColor(glm::u8vec4(0, 0, 0, 255));
        auto border = GetAnyExtension<ssGUI::Extensions::Border>();
        border->SetBorderWidth(2);
        border->SetBorderColor(GetBackgroundColor());

        auto buttonEvent = GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED);
        buttonEvent->ClearEventListeners();
        buttonEvent->AddEventListener(
            ListenerKey, this,
            [](ssGUI::EventInfo info)
            {
                ssGUI::Checkbox* btn = static_cast<ssGUI::Checkbox*>(info.EventSource);
                glm::u8vec4 bgcolor = btn->GetBackgroundColor();
                
                static_assert((int)ssGUI::Enums::ButtonState::COUNT == 6, "Make sure this is updated");
                switch(btn->GetButtonState())
                {
                    case ssGUI::Enums::ButtonState::NORMAL:
                        bgcolor.a = 255;
                        btn->SetBackgroundColor(bgcolor);
                        break;
                    case ssGUI::Enums::ButtonState::HOVER:
                        bgcolor.a = 200;
                        btn->SetBackgroundColor(bgcolor);
                        break;
                    case ssGUI::Enums::ButtonState::ON_CLICK:
                        break;
                    case ssGUI::Enums::ButtonState::CLICKING:
                        bgcolor.a = 100;
                        btn->SetBackgroundColor(bgcolor);
                        break;
                    case ssGUI::Enums::ButtonState::CLICKED:
                        btn->SetChecked(!btn->IsChecked());
                        break;
                    case ssGUI::Enums::ButtonState::NOT_INTERACTABLE:
                        bgcolor.a = 50;
                        btn->SetBackgroundColor(bgcolor);
                        break;
                }

                auto border = btn->GetAnyExtension<ssGUI::Extensions::Border>();

                if(border != nullptr)
                    border->SetBorderColor(btn->GetBackgroundColor());
            }
        ); 
    }

    Checkbox::~Checkbox()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
    }

    ssGUI::Enums::GUIObjectType Checkbox::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::WIDGET | ssGUI::Enums::GUIObjectType::BUTTON;
    }

    void Checkbox::SetInnerBoxSpace(float space)
    {
        InnerBoxSpace = space;
        RedrawObject();
    }

    float Checkbox::GetInnerBoxSpace() const
    {
        return InnerBoxSpace;
    }

    void Checkbox::SetChecked(bool checked)
    {
        Checked = checked;
        RedrawObject();
    }

    bool Checkbox::IsChecked() const
    {
        return Checked;
    }

    //You will always need to override this in order to call the copy constructor
    Checkbox* Checkbox::Clone(bool cloneChildren)
    {
        ssLOG_FUNC_ENTRY();
        Checkbox* temp = new Checkbox(*this);
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