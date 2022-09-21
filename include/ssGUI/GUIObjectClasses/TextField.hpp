#ifndef SSGUI_TEXT_FIELD
#define SSGUI_TEXT_FIELD

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
        uint64_t LastBlinkTime = 0;             //(Internal variable) Used to do the caret blinking animation
        int32_t BlinkDuration = 500;            //(Internal variable) Used for the duration of the caret blinking
        bool BlinkCaret = false;                //(Internal variable) Flag for controlling the blinking of the caret

        uint64_t LastArrowNavStartTime = 0;     //(Internal variable) Used to control the character navigation with arrow keys
        int ArrowNavPauseDuration = 500;        //(Internal variable) Used to control the character navigation with arrow keys
        uint64_t LastArrowNavTime = 0;          //(Internal variable) Used to control the character navigation with arrow keys
        int ArrowNavInterval = 20;              //(Internal variable) Used to control the character navigation with arrow keys
    =================================================================
    ============================== C++ ==============================
    TextField::TextField() : LastBlinkTime(0), BlinkDuration(500), BlinkCaret(false), LastArrowNavStartTime(0), ArrowNavPauseDuration(500), 
                                LastArrowNavTime(0), ArrowNavInterval(20)
    {
        SetBackgroundColor(glm::ivec4(127, 127, 127, 255));
        
        AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::RoundedCorners>());
        
        auto outlineEx = ssGUI::Factory::Create<ssGUI::Extensions::Outline>();
        outlineEx->SetOutlineColor(glm::u8vec4(0, 0, 0, 127));
        AddExtension(outlineEx);
        
        //Add shadow to window
        auto shadowEx = ssGUI::Factory::Create<ssGUI::Extensions::BoxShadow>();
        shadowEx->SetBlurRadius(20);
        shadowEx->SetSizeOffset(glm::vec2(10, 10));
        AddExtension(shadowEx);
    }
    =================================================================
    */
    class TextField : public Text
    {
        private:
            TextField& operator=(TextField const& other) = default;

        protected:
            uint64_t LastBlinkTime = 0;             //(Internal variable) Used to do the caret blinking animation
            int32_t BlinkDuration = 500;            //(Internal variable) Used for the duration of the caret blinking
            bool BlinkCaret = false;                //(Internal variable) Flag for controlling the blinking of the caret

            uint64_t LastArrowNavStartTime = 0;     //(Internal variable) Used to control the character navigation with arrow keys
            int ArrowNavPauseDuration = 500;        //(Internal variable) Used to control the character navigation with arrow keys
            uint64_t LastArrowNavTime = 0;          //(Internal variable) Used to control the character navigation with arrow keys
            int ArrowNavInterval = 20;              //(Internal variable) Used to control the character navigation with arrow keys

            TextField(TextField const& other);

            virtual int GetEndOfPreviousWord(int curIndex);

            virtual int GetEndOfNextWord(int curIndex);

            virtual void GetCurrentLineStartEndIndex(int curIndex, int& startIndex, int& endIndexInclusive);

            virtual void GetPreviousLineStartEndIndex(int curIndex, int& startIndex, int& endIndexInclusive);

            virtual void GetNextLineStartEndIndex(int curIndex, int& startIndex, int& endIndexInclusive);

            virtual int GetPositionForPreviousLine(int curIndex);

            virtual int GetPositionForNextLine(int curIndex);

            virtual void TextInputUpdate(std::wstring& textInput, bool& refreshBlinkTimer, bool& wordMode);

            virtual void CaretNavigationUpdate(ssGUI::Backend::BackendSystemInputInterface* inputInterface, bool& refreshBlinkTimer, bool& blockKeys, bool& wordMode);

            virtual void ConstructRenderInfo() override;

            virtual void MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                                    ssGUI::GUIObject* mainWindow) override;

        public:
            TextField();
            virtual ~TextField() override;
            
            //function: GetType
            //See <Widget::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: Clone
            //See <Widget::Clone>
            virtual TextField* Clone(bool cloneChildren) override;
    };
}

#endif