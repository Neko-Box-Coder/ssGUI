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
        uint64_t LastBlinkTime;
        int32_t BlinkDuration;
        bool BlinkCaret;

        uint64_t LastArrowNavStartTime;
        int ArrowNavPauseDuration;
        uint64_t LastArrowNavTime;
        int ArrowNavInterval;
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
            uint64_t LastBlinkTime;
            int32_t BlinkDuration;
            bool BlinkCaret;

            uint64_t LastArrowNavStartTime;
            int ArrowNavPauseDuration;
            uint64_t LastArrowNavTime;
            int ArrowNavInterval;

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

            virtual void MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, 
                ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;

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