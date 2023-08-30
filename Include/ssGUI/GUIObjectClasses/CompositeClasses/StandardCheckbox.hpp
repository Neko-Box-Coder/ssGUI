#ifndef SSGUI_STANDARD_CHECKBOX_HPP
#define SSGUI_STANDARD_CHECKBOX_HPP

#include "ssGUI/GUIObjectClasses/Text.hpp"
#include "ssGUI/GUIObjectClasses/Checkbox.hpp"

//namespace: ssGUI
namespace ssGUI
{
    /*class: StandardCheckbox
    A standard checkbox that contains a title text and a checkbox object.
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUIObjectIndex CheckboxTitleTextObject;
        ssGUIObjectIndex CheckboxObject;
    =================================================================
    ============================== C++ ==============================
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
    =================================================================
    */
    class StandardCheckbox : public Widget
    {
        private:
            StandardCheckbox& operator=(StandardCheckbox const& other) = default;
        
        protected:
            ssGUIObjectIndex CheckboxTitleTextObject;
            ssGUIObjectIndex CheckboxObject;
        
            StandardCheckbox(StandardCheckbox const& other);
    
            void ForwardCheckboxEvents();
    
            virtual void ConstructRenderInfo() override;

        public:
            //string: ListenerKey
            static const std::string ListenerKey;

            StandardCheckbox();
            virtual ~StandardCheckbox() override;

            /*function: SetTitleTextObject
            Sets the title text GUI Object. 
            The text content of the old text GUI Object will be copied to the new one.
            The old text GUI Object will be deleted and the new text GUI Object will be inserted at the same place as the old one.
            Passing nullptr will unset the text GUI object.*/
            virtual void SetTitleTextObject(ssGUI::Text* text);
            
            //function: GetTitleTextObject
            //Gets the title text GUI Object. 
            virtual ssGUI::Text* GetTitleTextObject() const;
            
            /*function: SetCheckboxObject
            Sets the actual checkbox GUI object.
            The old checkbox GUI Object will be deleted and the new checkbox GUI Object will be inserted at the same place as the old one.
            Passing nullptr will unset the checkbox GUI object.*/
            virtual void SetCheckboxObject(ssGUI::Checkbox* checkbox);
            
            //function: GetCheckboxObject
            //Gets the actual checkbox GUI object.
            virtual ssGUI::Checkbox* GetCheckboxObject() const;

            //function: SetInteractable
            //See <Widget::SetInteractable>
            virtual void SetInteractable(bool interactable) override;
            
            //function: SetBlockInput
            //See <Widget::SetBlockInput>
            virtual void SetBlockInput(bool blockInput) override;

            //function: GetType
            //See <Widget::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: Clone
            //See <Widget::Clone>
            virtual StandardCheckbox* Clone(bool cloneChildren) override;
        
    };
}

#endif