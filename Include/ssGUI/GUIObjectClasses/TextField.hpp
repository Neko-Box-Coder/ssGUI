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
        uint64_t LastBlinkTime;                     //(Internal variable) Used to do the caret blinking animation
        int32_t BlinkDuration;                      //(Internal variable) Used for the duration of the caret blinking
        bool BlinkCaret;                            //(Internal variable) Flag for controlling the blinking of the caret
        int LastStartSelectionIndex;                //(Internal variable) Used for setting the caret to the correct position
        int LastEndSelectionIndex;                  //(Internal variable) Used for setting the caret to the correct position
        int CaretPosition;                          //(Internal variable) Used for setting position of carret

        uint64_t LastArrowNavStartTime;             //(Internal variable) Timestamp when arrow key is pressed down
        int ArrowNavPauseDuration;                  //(Internal variable) Time in millisecond for holding arrow key to be registered
        uint64_t LastArrowNavTime;                  //(Internal variable) Last timestamp when navigating with arrow key
        int ArrowNavInterval;                       //(Internal variable) How frequent the caret is moved when holding down arrow
        
        uint64_t LastInputTime;                     //(Internal variable) Timestamp when last character input
        bool FinishedChangingNotified;              //(Internal variable) Have we notified for text content finished changing
        bool LastIsFocused;                         //(Internal variable) IsFocused for last frame
        int FinishedChangingTimeThresholdInMs;      //(Internal variable) How much time to wait after the last character input 
                                                    //                      to fire the text content finished changing event
    =================================================================
    ============================== C++ ==============================
    TextField::TextField() :    LastBlinkTime(0),
                                BlinkDuration(500),
                                BlinkCaret(false),
                                LastStartSelectionIndex(-1),
                                LastEndSelectionIndex(-1),
                                CaretPosition(-1),
                                LastArrowNavStartTime(0),
                                ArrowNavPauseDuration(500),
                                LastArrowNavTime(0),
                                ArrowNavInterval(20),
                                LastInputTime(0),
                                FinishedChangingNotified(true),
                                LastIsFocused(false),
                                FinishedChangingTimeThresholdInMs(500)
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
            uint64_t LastBlinkTime;                     //(Internal variable) Used to do the caret blinking animation
            int32_t BlinkDuration;                      //(Internal variable) Used for the duration of the caret blinking
            bool BlinkCaret;                            //(Internal variable) Flag for controlling the blinking of the caret
            int LastStartSelectionIndex;                //(Internal variable) Used for setting the caret to the correct position
            int LastEndSelectionIndex;                  //(Internal variable) Used for setting the caret to the correct position
            int CaretPosition;                          //(Internal variable) Used for setting position of carret

            uint64_t LastArrowNavStartTime;             //(Internal variable) Timestamp when arrow key is pressed down
            int ArrowNavPauseDuration;                  //(Internal variable) Time in millisecond for holding arrow key to be registered
            uint64_t LastArrowNavTime;                  //(Internal variable) Last timestamp when navigating with arrow key
            int ArrowNavInterval;                       //(Internal variable) How frequent the caret is moved when holding down arrow
            
            uint64_t LastInputTime;                     //(Internal variable) Timestamp when last character input
            bool FinishedChangingNotified;              //(Internal variable) Have we notified for text content finished changing
            bool LastIsFocused;                         //(Internal variable) IsFocused for last frame
            int FinishedChangingTimeThresholdInMs;      //(Internal variable) How much time to wait after the last character input 
                                                        //                      to fire the text content finished changing event

            TextField(TextField const& other);

            virtual void TextInputUpdate(std::wstring& textInput, bool& refreshBlinkTimer, bool& wordMode);

            virtual void CaretNavigationUpdate(ssGUI::Backend::BackendSystemInputInterface* inputInterface, bool& refreshBlinkTimer, bool& blockKeys, bool& wordMode);

            virtual void ConstructRenderInfo() override;

            virtual void MainLogic( ssGUI::Backend::BackendSystemInputInterface* inputInterface, 
                                    ssGUI::InputStatus& currentInputStatus, 
                                    ssGUI::InputStatus& lastInputStatus, 
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