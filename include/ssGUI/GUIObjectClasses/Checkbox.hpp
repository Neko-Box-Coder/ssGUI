#ifndef SSGUI_CHECKBOX
#define SSGUI_CHECKBOX

#include "ssGUI/GUIObjectClasses/Button.hpp"

//namespace: ssGUI
namespace ssGUI
{
    /*class: Checkbox
    Basic checkbox widget that inherits the <Button> class.
    You can override how the checkbox behaves by manipulating the <EventCallbacks::ButtonStateChangedEventCallback> class.

    Variables & Constructor:
    //============================== C++ ==============================
    protected:
        float InnerBoxSpace;
        bool Checked;
    //=================================================================
    //============================== C++ ==============================
    Checkbox::Checkbox() : InnerBoxSpace(2), Checked(true)
    {
        SetBackgroundColor(glm::u8vec4(0, 0, 0, 255));
        auto border = GetAnyExtension<ssGUI::Extensions::Border>();
        border->SetBorderWidth(2);
        border->SetBorderColor(GetBackgroundColor());

        auto buttonEvent = GetAnyEventCallback<ssGUI::EventCallbacks::ButtonStateChangedEventCallback>();
        buttonEvent->ClearEventListeners();
        buttonEvent->AddEventListener(
            [](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* refs)
            {
                ssGUI::Checkbox* btn = static_cast<ssGUI::Checkbox*>(src);
                glm::u8vec4 bgcolor = btn->GetBackgroundColor();
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
                    case ssGUI::Enums::ButtonState::CLICKED:
                    case ssGUI::Enums::ButtonState::ON_CLICK:
                    case ssGUI::Enums::ButtonState::CLICKING:
                        bgcolor.a = 100;
                        btn->SetChecked(!btn->IsChecked());
                        btn->SetBackgroundColor(bgcolor);
                        break;
                    case ssGUI::Enums::ButtonState::DISABLED:
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
    //=================================================================
    */
    class Checkbox : public Button
    {
        private:
            Checkbox& operator=(Checkbox const& other) = default;

        protected:
            float InnerBoxSpace;
            bool Checked;

            Checkbox(Checkbox const& other);
            virtual void ConstructRenderInfo() override;

        public:
            Checkbox();
            virtual ~Checkbox() override;
            
            //function: SetInnerBoxSpace
            //Sets the space between the border and the inner "check" box, in pixels
            virtual void SetInnerBoxSpace(float space);

            //function: GetInnerBoxSpace
            //Gets the space between the border and the inner "check" box, in pixels
            virtual float GetInnerBoxSpace() const;

            //function: SetChecked
            //Sets the check status
            virtual void SetChecked(bool checked);

            //function: IsChecked
            //Gets the check status
            virtual bool IsChecked() const;

            //function: GetType
            //See <Widget::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;
    
            //function: Internal_Update
            //See <Widget::Internal_Update>
            // virtual void Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;
            
            //function: Clone
            //See <Widget::Clone>
            virtual Checkbox* Clone(bool cloneChildren) override;
    };
}

#endif