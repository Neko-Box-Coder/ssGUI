#ifndef SSGUI_WIDGET_H
#define SSGUI_WIDGET_H

#include "ssGUI/GUIObjectClasses/GUIObject.hpp"

//namespace: ssGUI
namespace ssGUI
{
    /*class: ssGUI::Widget
    A base class for any interactable or non-window GUI objects

    Variables & Constructor:
    ============================== C++ ==============================
    private:
        //Widget status
        bool Interactable;      //See <IsInteractable>
        bool BlockInput;        //See <IsBlockInput>
    =================================================================
    ============================== C++ ==============================
    Widget::Widget() :  Interactable(true),
                        BlockInput(true)
    {}
    =================================================================
    */
    class Widget : public GUIObject
    {
        private:
            //Widget status
            bool Interactable;      //See <IsInteractable>
            bool BlockInput;        //See <IsBlockInput>

            Widget& operator=(Widget const& other) = default;

        protected:
            Widget(Widget const& other);

            virtual void ConstructRenderInfo() override;
            virtual void MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                                    ssGUI::GUIObject* mainWindow) override;

        public:
            Widget();
            virtual ~Widget() override;
            
            //function: IsInteractable
            virtual bool IsInteractable() const;
            
            //function: SetInteractable
            virtual void SetInteractable(bool interactable);
            
            //function: IsBlockInput
            //Returns if this widget blocks input. If true, the GUI Object behind this widget will not receive the input.
            virtual bool IsBlockInput() const;
            
            //function: SetBlockInput
            //Sets if this widget blocks input. If true, the GUI Object behind this widget will not receive the input.
            virtual void SetBlockInput(bool blockInput);

            //function: GetType
            //See <GUIObject::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: Clone
            //See <GUIObject::Clone>
            virtual Widget* Clone(bool cloneChildren) override;
    };
}

#endif