#ifndef SSGUI_WIDGET
#define SSGUI_WIDGET

#include "ssGUI/BaseClasses/BaseGUIObject.hpp"

//namespace: ssGUI
namespace ssGUI
{
    //class: Widget
    class Widget : public BaseGUIObject
    {
        private:
            //Widget status
            bool Focused;
            bool Interactable;
            bool BlockInput;
            Widget& operator=(Widget const& other) = default;

        protected:
            Widget(Widget const& other) = default;

        public:
            Widget();
            virtual ~Widget() override;
            
            //function: IsFocused
            virtual bool IsFocused() const;
            
            //function: SetFocus
            virtual void SetFocus(bool focus);
            
            //function: IsInteractable
            virtual bool IsInteractable() const;
            
            //function: SetInteractable
            virtual void SetInteractable(bool interactable);
            
            //function: IsBlockInput
            virtual bool IsBlockInput() const;
            
            //function: SetBlockInput
            virtual void SetBlockInput(bool blockInput);

            //virtual int GetBorderWidth();
            //virtual void SetBorderWidth(int width);

            //virtual glm::u8vec4 GetBorderColor();
            //virtual void SetBorderColor(glm::u8vec4 color);
            
            //function: GetType
            virtual ssGUI::Enums::GUIObjectType GetType() const override;
            
            //function: Draw
            virtual void Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset) override;
            
            //function: Internal_Update
            virtual void Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus) override;
            
            //function: Clone
            virtual GUIObject* Clone(std::vector<GUIObject*>& originalObjs, bool cloneChildren) override;
    };
}

#endif