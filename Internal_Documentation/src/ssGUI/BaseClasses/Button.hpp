#ifndef SSGUI_BUTTON
#define SSGUI_BUTTON

#include "ssGUI/BaseClasses/Widget.hpp"
#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/Extensions/Border.hpp"
#include "ssGUI/Enums/ButtonState.hpp"
#include "ssGUI/EventCallbacks/ButtonStateChangedEventCallback.hpp"
#include "glm/vec4.hpp"

//namespace: ssGUI
namespace ssGUI
{
    //class: Button
    class Button : public Widget
    {
        private:
            ssGUI::Enums::ButtonState CurrentState;
            ssGUI::EventCallbacks::ButtonStateChangedEventCallback* StateChangedEventCallback;
            Button& operator=(Button const& other);
        
        protected:
            Button(Button const& other);

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

            //TODO : Programmically triggering button method
            //TODO : Programmically triggering button with GUI method

            //Overriding widget
            //function: GetType
            virtual ssGUI::Enums::GUIObjectType GetType() const override;
            
            //function: SetInteractable
            virtual void SetInteractable(bool interactable) override;
            
            //function: Draw
            virtual void Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset) override;
            
            //function: Internal_Update
            virtual void Internal_Update(ssGUI::Backend::BackendSystemInputInterface *inputInterface, ssGUI::InputStatus &globalInputStatus, ssGUI::InputStatus &windowInputStatus) override;

            //function: Clone
            virtual GUIObject* Clone(std::vector<GUIObject*>& originalObjs, bool cloneChildren) override;
    };
}


#endif