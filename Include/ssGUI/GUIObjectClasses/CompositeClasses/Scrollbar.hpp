#ifndef SSGUI_SCROLLBAR_H
#define SSGUI_SCROLLBAR_H

#include "ssGUI/GUIObjectClasses/Widget.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/Slider.hpp"

//namespace: ssGUI
namespace ssGUI
{
    /*class: ssGUI::Scrollbar
    Scrollbar class which can be used to for scrolling an element.

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        float ScrollbarSize;    //See <GetScrollbarSize>
        float SidePadding;      //See <GetSidePadding>
    =================================================================
    ============================== C++ ==============================
    Scrollbar::Scrollbar() :    ScrollbarSize(0.25),
                                SidePadding(2)
    {
        SetKnobSize(10, false);
        SetEndPadding(5);
        float diff = 50;
        SetBackgroundColor(GetBackgroundColor() - glm::u8vec4(diff, diff, diff, 0));

        GetKnobObject()->RemoveExtension<ssGUI::Extensions::Outline>();
        auto btn = static_cast<ssGUI::StandardButton*>(GetKnobObject());
        glm::u8vec4 bgColor = GetBackgroundColor();
        bgColor.a = 255;

        btn->SetButtonColor(bgColor);
        btn->SetButtonColor
        (
            (btn->GetButtonColor().r + btn->GetButtonColor().g + btn->GetButtonColor().b) / 3.f > 128 ? 
            btn->GetButtonColor() - glm::u8vec4(diff, diff, diff, 0) : 
            btn->GetButtonColor() + glm::u8vec4(diff, diff, diff, 0)
        );

        UpdateKnobSize();
        SetVertical(true, true);
        SetReverse(true);
    }
    =================================================================
    */
    class Scrollbar : public Slider
    {
        private:
            Scrollbar& operator=(Scrollbar const& other) = default;

        protected:
            float ScrollbarSize;    //See <GetScrollbarSize>
            float SidePadding;      //See <GetSidePadding>

            virtual void UpdateKnobSize();
            Scrollbar(Scrollbar const& other);

        public:
            Scrollbar();
            virtual ~Scrollbar() override;
            
            //function: SetScrollbarSize
            //Sets the length of the scrollbar (button),
            virtual void SetScrollbarSize(float size);

            //function: GetScrollbarSize
            //Gets the length of the scrollbar (button),
            virtual float GetScrollbarSize() const;

            //function: SetScrollbarValue
            //Sets the value of the scrollbar (button), ranging from 0 to 1.
            virtual void SetScrollbarValue(float scrollbarValue);

            //function: GetScrollbarValue
            //Gets the value of the scrollbar (button), ranging from 0 to 1.
            virtual float GetScrollbarValue() const;

            //function: SetSidePadding
            //Sets the number of pixels padding for the side of the scrollbar (button)
            virtual void SetSidePadding(float sidePadding);

            //function: GetSidePadding
            //Gets the number of pixels padding for the side of the scrollbar (button)
            virtual float GetSidePadding() const;

            //function: SetVertical
            //See <Slider::SetVertical>
            virtual void SetVertical(bool vertical, bool swapWidthHeight) override;

            //function: GetType
            //See <Widget::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: Clone
            //See <Widget::Clone>
            virtual Scrollbar* Clone(bool cloneChildren) override;
    };
}

#endif