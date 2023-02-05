#ifndef SSGUI_BUTTON_H
#define SSGUI_BUTTON_H

#include "ssGUI/GUIObjectClasses/Widget.hpp"
#include "ssGUI/Extensions/Border.hpp"
#include "ssGUI/Enums/ButtonState.hpp"
#include "ssGUI/EventCallbacks/ButtonStateChangedEventCallback.hpp"
#include "glm/vec4.hpp"

//namespace: ssGUI
namespace ssGUI
{
    /*class: ssGUI::Button
    A class that provides on click functionality when being clicked by the cursor.
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUI::Enums::ButtonState CurrentState;     //See <GetButtonState>
        glm::u8vec4 ButtonColor;                    //See <GetButtonColor>
    =================================================================
    ============================== C++ ==============================
    Button::Button() :  CurrentState(ssGUI::Enums::ButtonState::NORMAL),
                        ButtonColor(100, 100, 100, 255)
    {
        SetSize(glm::vec2(25, 25));
        auto stateChangedEventCallback = ssGUI::Factory::Create<ssGUI::EventCallbacks::ButtonStateChangedEventCallback>();
        stateChangedEventCallback->AddEventListener(
            ListenerKey, this,
            [](ssGUI::EventInfo info)
            {
                ssGUI::Button* btn = static_cast<ssGUI::Button*>(info.EventSource);
                glm::u8vec4 btnColor = btn->GetButtonColor();
                switch(btn->GetButtonState())
                {
                    case ssGUI::Enums::ButtonState::NORMAL:
                        btnColor.a = 255;
                        btn->SetBackgroundColor(btnColor);
                        break;
                    case ssGUI::Enums::ButtonState::HOVER:
                        btnColor.a = 200;
                        btn->SetBackgroundColor(btnColor);
                        break;
                    case ssGUI::Enums::ButtonState::ON_CLICK:
                    case ssGUI::Enums::ButtonState::CLICKING:
                        btnColor.a = 100;
                        btn->SetBackgroundColor(btnColor);
                        break;
                    case ssGUI::Enums::ButtonState::CLICKED:
                    case ssGUI::Enums::ButtonState::NOT_INTERACTABLE:
                        btnColor.a = 50;
                        btn->SetBackgroundColor(btnColor);
                        break;
                }
            }
        ); 
        
        AddEventCallback(stateChangedEventCallback);
        AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Border>());
        SetBackgroundColor(GetButtonColor());
    }
    =================================================================
    */
    class Button : public Widget
    {
        private:
            Button& operator=(Button const& other);
        
        protected:
            ssGUI::Enums::ButtonState CurrentState;     //See <GetButtonState>
            glm::u8vec4 ButtonColor;                    //See <GetButtonColor>
            
            Button(Button const& other);

            virtual void ConstructRenderInfo() override;
            virtual void SetButtonState(ssGUI::Enums::ButtonState state);
            virtual void MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                                    ssGUI::GUIObject* mainWindow) override;
            
        public:
            //string: ListenerKey
            static const std::string ListenerKey;

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

            //function: Clone
            //See <Widget::Clone>
            virtual Button* Clone(bool cloneChildren) override;

    };
}


#endif