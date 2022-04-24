#ifndef SSGUI_BUTTON
#define SSGUI_BUTTON

#include "ssGUI/GUIObjectClasses/Widget.hpp"
#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/Extensions/Border.hpp"
#include "ssGUI/Enums/ButtonState.hpp"
#include "ssGUI/EventCallbacks/ButtonStateChangedEventCallback.hpp"
#include "glm/vec4.hpp"

//namespace: ssGUI
namespace ssGUI
{
    /*class: Button
    A class that provides on click functionality when being clicked by the cursor.
    
    Variables & Constructor:
    ============================== C++ ==============================
    private:
        ssGUI::Enums::ButtonState CurrentState;
        ssGUI::EventCallbacks::ButtonStateChangedEventCallback* StateChangedEventCallback;
    =================================================================
    ============================== C++ ==============================
    Button::Button() : CurrentState(ssGUI::Enums::ButtonState::NORMAL), StateChangedEventCallback(nullptr)
    {
        SetBackgroundColor(glm::u8vec4(100,100,100,255)); //Gray background colour for button (For now)
        StateChangedEventCallback = new ssGUI::EventCallbacks::ButtonStateChangedEventCallback();
        StateChangedEventCallback->AddEventListener(
            [](ssGUI::GUIObject* obj)
            {
                ssGUI::Button* btn = static_cast<ssGUI::Button*>(obj);
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
                        btn->SetBackgroundColor(bgcolor);
                        break;
                    case ssGUI::Enums::ButtonState::DISABLED:
                        bgcolor.a = 50;
                        btn->SetBackgroundColor(bgcolor);
                        break;
                }
            }
        );
        
        AddEventCallback(StateChangedEventCallback);
        AddExtension(new ssGUI::Extensions::Border());
    }
    =================================================================
    */
    class Button : public Widget
    {
        private:
            Button& operator=(Button const& other);
        
        protected:
            ssGUI::Enums::ButtonState CurrentState;
            ssGUI::EventCallbacks::ButtonStateChangedEventCallback* StateChangedEventCallback;
            glm::u8vec4 ButtonColor;
            
            Button(Button const& other);

            virtual void ConstructRenderInfo() override;
            virtual void SetButtonState(ssGUI::Enums::ButtonState state);
            
        public:
            Button();
            virtual ~Button() override;
            
            //function: NotifyButtonEventCallbackManually
            //If you are updating the button apperance in the event callback, 
            //call this function after adding the event callback in order to "repaint" the button.
            virtual void NotifyButtonEventCallbackManually(); 
            
            //function: GetButtonState
            virtual ssGUI::Enums::ButtonState GetButtonState() const;

            //function: SetButtonColor
            //Sets the button color that can be referenced from the ButtonStateChangedEventCallback
            virtual void SetButtonColor(glm::u8vec4 color);
            
            //function: GetButtonColor
            //Gets the button color that can be referenced from the ButtonStateChangedEventCallback
            virtual glm::u8vec4 GetButtonColor() const;

            //TODO : Programmically triggering button method
            //TODO : Programmically triggering button with GUI method

            //Overriding widget
            //function: GetType
            //See <Widget::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;
            
            //function: SetInteractable
            //See <Widget::SetInteractable>
            virtual void SetInteractable(bool interactable) override;

            //function: Internal_Update
            //See <Widget::Internal_Update>
            virtual void Internal_Update(ssGUI::Backend::BackendSystemInputInterface *inputInterface, ssGUI::InputStatus &globalInputStatus, ssGUI::InputStatus &windowInputStatus, ssGUI::GUIObject* mainWindow) override;

            //function: Clone
            //See <Widget::Clone>
            virtual Button* Clone(bool cloneChildren) override;

    };
}


#endif