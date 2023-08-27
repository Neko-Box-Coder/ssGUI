#ifndef SSGUI_TEXT_FIELD_H
#define SSGUI_TEXT_FIELD_H

#include "ssGUI/GUIObjectClasses/Text.hpp"
#include "ssGUI/GUIObjectClasses/Widget.hpp"

//namespace: ssGUI
namespace ssGUI
{
    /*class: ssGUI::TextField
    Allow editing <ssGUI::Text> with user input.

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        uint64_t LastBlinkTime;             //(Internal variable) Used to do the caret blinking animation
        int32_t BlinkDuration;              //(Internal variable) Used for the duration of the caret blinking
        bool BlinkCaret;                    //(Internal variable) Flag for controlling the blinking of the caret

        uint64_t LastArrowNavStartTime;     //(Internal variable) Used to control the character navigation with arrow keys
        int ArrowNavPauseDuration;          //(Internal variable) Used to control the character navigation with arrow keys
        uint64_t LastArrowNavTime;          //(Internal variable) Used to control the character navigation with arrow keys
        int ArrowNavInterval;               //(Internal variable) Used to control the character navigation with arrow keys
    =================================================================
    ============================== C++ ==============================
    TextField::TextField() :    LastBlinkTime(0),
                                BlinkDuration(500),
                                BlinkCaret(false),
                                LastArrowNavStartTime(0),
                                ArrowNavPauseDuration(500),
                                LastArrowNavTime(0),
                                ArrowNavInterval(20)
    {
        SetBlockInput(true);
        SetMinSize(glm::vec2(35, 35));
        SetBackgroundColor(glm::ivec4(127, 127, 127, 255));

        AddExtension<ssGUI::Extensions::RoundedCorners>();
        
        AddExtension<ssGUI::Extensions::Outline>()->SetOutlineColor(glm::u8vec4(0, 0, 0, 127));
        
        //Add shadow to window
        auto shadowEx = AddExtension<ssGUI::Extensions::BoxShadow>();
        shadowEx->SetBlurRadius(20);
        shadowEx->SetSizeOffset(glm::vec2(10, 10));
    }
    =================================================================
    */
    class TextField : public Text
    {
        private:
            TextField& operator=(TextField const& other) = default;

        protected:
            uint64_t LastBlinkTime;             //(Internal variable) Used to do the caret blinking animation
            int32_t BlinkDuration;              //(Internal variable) Used for the duration of the caret blinking
            bool BlinkCaret;                    //(Internal variable) Flag for controlling the blinking of the caret

            uint64_t LastArrowNavStartTime;     //(Internal variable) Used to control the character navigation with arrow keys
            int ArrowNavPauseDuration;          //(Internal variable) Used to control the character navigation with arrow keys
            uint64_t LastArrowNavTime;          //(Internal variable) Used to control the character navigation with arrow keys
            int ArrowNavInterval;               //(Internal variable) Used to control the character navigation with arrow keys

            TextField(TextField const& other);

            virtual void TextInputUpdate(std::wstring& textInput, bool& refreshBlinkTimer, bool& wordMode);

            virtual void CaretNavigationUpdate(ssGUI::Backend::BackendSystemInputInterface* inputInterface, bool& refreshBlinkTimer, bool& blockKeys, bool& wordMode);

            virtual void ConstructRenderInfo() override;

            virtual void MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                                    ssGUI::GUIObject* mainWindow) override;

        public:
            //string: TEXT_UNDERLINE_SHAPE_NAME
            static const std::string TEXTFIELD_CARET_SHAPE_NAME;
        
            TextField();
            virtual ~TextField() override;

            //====================================================================
            //Group: Overrides
            //====================================================================
            
            //function: GetType
            //See <Widget::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: Clone
            //See <Widget::Clone>
            virtual TextField* Clone(bool cloneChildren) override;
    };
}

#endif