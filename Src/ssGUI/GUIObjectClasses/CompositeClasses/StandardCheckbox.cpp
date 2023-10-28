#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardCheckbox.hpp"
#include "ssGUI/Extensions/Layout.hpp"

namespace ssGUI
{
    StandardCheckbox::StandardCheckbox(StandardCheckbox const& other) : Widget(other),
                                                                        CheckboxTitleTextObject(other.CheckboxTitleTextObject),
                                                                        CheckboxObject(other.CheckboxObject)
    {
    }
    
    void StandardCheckbox::ForwardCheckboxEvents()
    {
        if(GetCheckboxObject() == nullptr)
            return;
        
        GetCheckboxObject()->ForwardEvent(this, ssGUI::Enums::EventType::BUTTON_STATE_CHANGED);
    }

    void StandardCheckbox::ConstructRenderInfo()
    {
    }
    
    const std::string StandardCheckbox::ListenerKey = "Standard Checkbox";
    
    StandardCheckbox::StandardCheckbox() :  CheckboxTitleTextObject(-1),
                                            CheckboxObject(-1)
    {
        //Add layout
        auto* layout = AddExtension<ssGUI::Extensions::Layout>();
        layout->SetUpdateContainerMinMaxSize(false);
        layout->SetHorizontalLayout(true);
        
        SetMinSize(glm::vec2(150, 30));
        SetSize(glm::vec2(150, 30));

        //Set layout preferred sizes
        layout->AddPreferredSizeMultipliers(0.85f, 0.15f);
        
        //Add components
        auto* checkboxTitle = AddChild<ssGUI::Text>();
        checkboxTitle->SetText("Checkbox:");
        CheckboxTitleTextObject = CurrentObjectsReferences.AddObjectReference(checkboxTitle);
        
        auto* checkbox = AddChildWithWrapper<ssGUI::Checkbox>(  ssGUI::Enums::AlignmentHorizontal::CENTER, 
                                                                ssGUI::Enums::AlignmentVertical::CENTER,
                                                                true);
        
        CheckboxObject = CurrentObjectsReferences.AddObjectReference(checkbox);
        ForwardCheckboxEvents();
    }

    StandardCheckbox::~StandardCheckbox()
    {
        
    }
    
    void StandardCheckbox::SetTitleTextObject(ssGUI::Text* text)
    {
        if(text == nullptr)
        {
            if(GetTitleTextObject() != nullptr)
                GetTitleTextObject()->Delete();
            
            return;
        }
    
        std::wstring oldTitle = L"";
    
        if(GetTitleTextObject() != nullptr)
        {
            oldTitle = GetTitleTextObject()->GetText();
            text->SetParent(GetTitleTextObject()->GetParent(), true);
            MoveChildAfterTargetChild(text, GetTitleTextObject());
            GetTitleTextObject()->Delete();
        }
        else
        {
            text->SetParent(this, true);
            MoveChildToFirst(text);
        }
        
        text->SetText(oldTitle);

        CheckboxTitleTextObject = CurrentObjectsReferences.AddObjectReference(text);    
    }
    
    ssGUI::Text* StandardCheckbox::GetTitleTextObject() const
    {
        return CurrentObjectsReferences.GetObjectReference<ssGUI::Text>(CheckboxTitleTextObject);
    }
    
    void StandardCheckbox::SetCheckboxObject(ssGUI::Checkbox* checkbox)
    {
        if(checkbox == nullptr)
        {
            if(GetCheckboxObject() != nullptr)
                GetCheckboxObject()->Delete();
            
            return;
        }
        
        if(GetCheckboxObject() != nullptr)
        {
            checkbox->SetParent(GetCheckboxObject()->GetParent(), true);
            GetCheckboxObject()->Delete();
        }
        else
        {
            ssGUI::GUIObject* wrapper;
            if(GetTitleTextObject() != nullptr)
                wrapper = AddChildAfter<ssGUI::GUIObject>(GetTitleTextObject());
            else
                wrapper = AddChild<ssGUI::GUIObject>();
        
            checkbox->SetParent(wrapper, true);
        }

        CheckboxObject = CurrentObjectsReferences.AddObjectReference(checkbox);
        
        ForwardCheckboxEvents();
    }
    
    ssGUI::Checkbox* StandardCheckbox::GetCheckboxObject() const
    {
        return CurrentObjectsReferences.GetObjectReference<ssGUI::Checkbox>(CheckboxObject);
    }
    
    void StandardCheckbox::SetInteractable(bool interactable)
    {
        if(GetCheckboxObject() != nullptr)
            GetCheckboxObject()->SetInteractable(true);
        
        Widget::SetInteractable(interactable);
    }
    
    void StandardCheckbox::SetBlockInput(bool blockInput)
    {
        if(GetCheckboxObject() != nullptr)
            GetCheckboxObject()->SetBlockInput(true);
        
        Widget::SetBlockInput(blockInput);
    }
    
    ssGUI::Enums::GUIObjectType StandardCheckbox::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::STANDARD_CHECKBOX | ssGUI::Enums::GUIObjectType::WIDGET;
    }
    
    StandardCheckbox* StandardCheckbox::Clone(bool cloneChildren)
    {
        ssGUI_LOG_FUNC();
        StandardCheckbox* temp = new StandardCheckbox(*this);
        CloneExtensionsAndEventCallbacks(temp);   

        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
                return nullptr;
        }

        return temp;
    }
}