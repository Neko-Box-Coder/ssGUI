#ifndef SSGUI_TEXT_H
#define SSGUI_TEXT_H

#include "ssGUI/DataClasses/Font.hpp"
#include "ssGUI/GUIObjectClasses/Widget.hpp"
#include "ssGUI/DataClasses/CharacterDetails.hpp"
#include "ssGUI/DataClasses/CharacterRenderInfo.hpp"
#include "ssGUI/Enums/TextWrapping.hpp"
#include "ssGUI/Enums/AlignmentHorizontal.hpp"
#include "ssGUI/Enums/AlignmentVertical.hpp"
#include "ssGUI/Extensions/Border.hpp"
#include "ssGUI/DataClasses/SegmentedVector.hpp"
#include <string>

//namespace: ssGUI
namespace ssGUI
{
    /*class: ssGUI::Text
    A class for showing text with different options such as font size, text alignment, fonts, etc.
    Text can be added by just setting/adding the text, which is the simplest.
    
    If you want the ablitiy to adjust each characters settings such as font, font size, etc. 
    You can set <ssGUI::CharacterDetails> for individual characters.

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        bool RecalculateTextNeeded;                                                         //(Internal variable) Flag to recalculate all the cahracters locations
                                                                                            //TODO: Maybe just use redraw flag?
        mutable ssGUI::SegmentedVector<ssGUI::CharacterDetails> CurrentCharactersDetails;   //See <GetCharacterDetails>

        std::vector<ssGUI::CharacterRenderInfo> CharactersRenderInfos;                      //(Internal variable) Vertices for rendering characters
        std::unordered_map<int, ssGUI::CharacterDetails> ProcessedCharacterDetails;         //(Internal variable) Valid characters for rendering

        bool Overflow;                                                                      //See <IsOverflow>
        float FontSize;                                                                     //See <GetNewTextFontSize>
        glm::u8vec4 TextColor;                                                              //See <GetNewTextColor>
        bool TextUnderline;                                                                 //See <IsNewTextUnderlined>
        bool MultilineAllowed;                                                              //See <IsMultilineAllowed>
        ssGUI::Enums::TextWrapping WrappingMode;                                            //See <GetWrappingMode>
        ssGUI::Enums::AlignmentHorizontal CurrentHorizontalAlignment;                       //See <GetTextHorizontalAlignment>
        ssGUI::Enums::AlignmentVertical CurrentVerticalAlignment;                           //See <GetTextVerticalAlignment>
        std::vector<ssGUI::Font*> CurrentFonts;                                             //See <GetFont>

        float HorizontalPadding;                                                            //See <GetTextHorizontalPadding>
        float VerticalPadding;                                                              //See <GetTextVerticalPadding>
        float CharacterSpace;                                                               //See <GetCharacterSpace>
        float LineSpace;                                                                    //See <GetLineSpace>
        float TabSize;                                                                      //See <GetTabSize>
        bool SelectionAllowed;                                                              //See <IsTextSelectionAllowed>
        int StartSelectionIndex;                                                            //See <GetStartSelectionIndex>
        int EndSelectionIndex;                                                              //See <GetEndSelectionIndex>
        bool DeselectWhenFocusLost;                                                         //See <IsDeselectWhenFocusLost>

        glm::u8vec4 SelectionColor;                                                         //See <GetSelectionColor>
        glm::u8vec4 TextSelectedColor;                                                      //See <GetTextSelectedColor>

        uint32_t LastDefaultFontsID;                                                        //(Internal variable) Used to keep track if there's any changes to the default fonts

        bool TextContentChanged;                                                            //(Internal variable) Used to keep track if there's any changes to the text content

        static int TextObjectCount;                                                         //(Internal variable) Used for deallocating default resources
        static std::vector<ssGUI::Font*> DefaultFonts;                                      //See <GetDefaultFont>
        static uint32_t DefaultFontsChangeID;                                               //(Internal variable) Used to track default font changes
    =================================================================
    ============================== C++ ==============================
    Text::Text() :  RecalculateTextNeeded(false),
                    CurrentCharactersDetails(),
                    CharactersRenderInfos(),
                    ProcessedCharacterDetails(),
                    Overflow(false),
                    FontSize(17),
                    TextColor(0, 0, 0, 255),
                    TextUnderline(false),
                    MultilineAllowed(true),
                    WrappingMode(ssGUI::Enums::TextWrapping::NO_WRAPPING),
                    CurrentHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER),
                    CurrentVerticalAlignment(ssGUI::Enums::AlignmentVertical::CENTER),
                    CurrentFonts(),
                    HorizontalPadding(5),
                    VerticalPadding(5),
                    CharacterSpace(0),
                    LineSpace(0),
                    TabSize(4),
                    SelectionAllowed(true),
                    StartSelectionIndex(-1),
                    EndSelectionIndex(-1),
                    DeselectWhenFocusLost(true),
                    SelectionColor(51, 153, 255, 255),
                    TextSelectedColor(255, 255, 255, 255),
                    LastDefaultFontsID(0),
                    TextContentChanged(false)
    {
        SetBackgroundColor(glm::ivec4(255, 255, 255, 0));
        SetBlockInput(false);
        SetInteractable(true);
        InitiateDefaultResources();

        auto sizeChangedCallback = AddEventCallback(ssGUI::Enums::EventType::SIZE_CHANGED);
        sizeChangedCallback->AddEventListener
        (
            ListenerKey, this,
            [](ssGUI::EventInfo& info)
            {
                static_cast<ssGUI::Text*>(info.EventSource)->RecalculateTextNeeded = true;
            }
        );
        
        TextObjectCount++;
        
        AddEventCallback(ssGUI::Enums::EventType::BEFORE_OBJECT_DESTROY)->AddEventListener
        (
            ListenerKey,
            this,
            [](ssGUI::EventInfo& info)
            {
                auto* text = static_cast<ssGUI::Text*>(info.Container);
                
                ssGUI::Text::TextObjectCount--;
                
                if(ssGUI::Text::TextObjectCount == 0)
                    text->CleanUpDefaultResources();
            }
        );
    }

    int Text::TextObjectCount = 0;
    std::vector<ssGUI::Font*> Text::DefaultFonts = std::vector<ssGUI::Font*>();
    uint32_t Text::DefaultFontsChangeID = 1;
    =================================================================
    */
    class Text : public Widget
    {
        private:
            Text& operator=(Text const& other);

        protected:
            bool RecalculateTextNeeded;                                                         //(Internal variable) Flag to recalculate all the cahracters locations
                                                                                                //TODO: Maybe just use redraw flag?
            mutable ssGUI::SegmentedVector<ssGUI::CharacterDetails> CurrentCharactersDetails;   //See <GetCharacterDetails>

            std::vector<ssGUI::CharacterRenderInfo> CharactersRenderInfos;                      //(Internal variable) Vertices for rendering characters
            std::unordered_map<int, ssGUI::CharacterDetails> ProcessedCharacterDetails;         //(Internal variable) Valid characters for rendering

            bool Overflow;                                                                      //See <IsOverflow>
            float FontSize;                                                                     //See <GetNewTextFontSize>
            glm::u8vec4 TextColor;                                                              //See <GetNewTextColor>
            bool TextUnderline;                                                                 //See <IsNewTextUnderlined>
            bool MultilineAllowed;                                                              //See <IsMultilineAllowed>
            ssGUI::Enums::TextWrapping WrappingMode;                                            //See <GetWrappingMode>
            ssGUI::Enums::AlignmentHorizontal CurrentHorizontalAlignment;                       //See <GetTextHorizontalAlignment>
            ssGUI::Enums::AlignmentVertical CurrentVerticalAlignment;                           //See <GetTextVerticalAlignment>
            std::vector<ssGUI::Font*> CurrentFonts;                                             //See <GetFont>

            float HorizontalPadding;                                                            //See <GetTextHorizontalPadding>
            float VerticalPadding;                                                              //See <GetTextVerticalPadding>
            float CharacterSpace;                                                               //See <GetCharacterSpace>
            float LineSpace;                                                                    //See <GetLineSpace>
            float TabSize;                                                                      //See <GetTabSize>
            bool SelectionAllowed;                                                              //See <IsTextSelectionAllowed>
            int StartSelectionIndex;                                                            //See <GetStartSelectionIndex>
            int EndSelectionIndex;                                                              //See <GetEndSelectionIndex>
            bool DeselectWhenFocusLost;                                                         //See <IsDeselectWhenFocusLost>

            glm::u8vec4 SelectionColor;                                                         //See <GetSelectionColor>
            glm::u8vec4 TextSelectedColor;                                                      //See <GetTextSelectedColor>

            uint32_t LastDefaultFontsID;                                                        //(Internal variable) Used to keep track if there's any changes to the default fonts

            bool TextContentChanged;                                                            //(Internal variable) Used to keep track if there's any changes to the text content

            static int TextObjectCount;                                                         //(Internal variable) Used for deallocating default resources
            static std::vector<ssGUI::Font*> DefaultFonts;                                      //See <GetDefaultFont>
            static uint32_t DefaultFontsChangeID;                                               //(Internal variable) Used to track default font changes


            Text(Text const& other);

            virtual ssGUI::CharacterDetails& GetInternalCharacterDetail(int index);

            virtual void ConstructCharacterDetails(std::wstring& s, std::vector<ssGUI::CharacterDetails>& details);

            virtual void AssignSupportedFont();

            virtual void DrawCharacter(glm::vec2 positionOffset, ssGUI::CharacterRenderInfo info, ssGUI::CharacterDetails details);
            
            virtual void FormatNewlinesCharacters();

            virtual float GetClosestTabSpace(float startPos, float tabSpace, float targetPos);

            virtual void ConstructRenderInfosForWordWrapping();

            virtual void ConstructRenderInfosForCharacterWrapping();

            virtual void ConstructRenderInfosForNoWrapping(bool checkValid);

            virtual void ApplyFontLineSpacing();

            virtual void ApplyTextAlignment();

            virtual void ApplyTextHighlight();

            virtual void ApplyTextUnderline();

            virtual void DrawAllCharacters();

            virtual void ConstructRenderInfo() override;

            virtual void MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                                    ssGUI::GUIObject* mainWindow) override;

        public:
            //string: ListenerKey
            static const std::string ListenerKey;
            static const std::string TEXT_CHARACTER_SHAPE_NAME;
            static const std::string TEXT_HIGHLIGHT_SHAPE_NAME;
            static const std::string TEXT_UNDERLINE_SHAPE_NAME;

            Text();
            virtual ~Text() override;

            //function: ComputeCharactersPositionAndSize
            //Computes all the characters' positions and sizes. This is called automatically when there's any changes to the text.
            virtual void ComputeCharactersPositionAndSize();
            
            //function: SetText
            //Sets the text to show
            virtual void SetText(std::wstring text);

            //function: SetText
            //Sets the text to show
            virtual void SetText(std::string text);

            //function: AddText
            //Append text to the end
            virtual void AddText(std::wstring text);

            //function: AddText
            //Append text to the end
            virtual void AddText(std::string text);

            //function: AddText
            //Insert text at index position
            virtual void AddText(std::wstring text, int index);

            //function: AddText
            //Insert text at index position
            virtual void AddText(std::string text, int index);

            //function: RemoveText
            //Remove text in range
            virtual void RemoveText(int startIndex, int exclusiveEndIndex);

            //function: GetText
            //Gets the text being shown
            virtual void GetText(std::wstring& retText) const;
            
            //function: GetText
            //Gets the text being shown
            virtual void GetText(std::string& retText) const;
            
            //function: GetText
            //Gets the text being shown
            template<typename T>
            T GetText() const
            {
                T retString;
                GetText(retString);
                return retString;
            }

            //function: GetText (deprecated)
            //Gets the text being shown. This is deprecated, use the template version instead.
            virtual std::wstring GetText() const;
            
            //function: GetCharacterCount
            //Gets the number of characters for the text being shown
            virtual int GetCharacterCount() const;
            
            //function: GetCharacterRenderInfo
            //Gets the character render info of the character at the index position
            virtual ssGUI::CharacterRenderInfo GetCharacterRenderInfo(int index) const;

            //function: SetCharacterDetails
            //Sets the character details at the index position
            virtual void SetCharacterDetails(int index, ssGUI::CharacterDetails details);

            //function: GetCharacterDetails
            //Gets the character details at the index position
            virtual ssGUI::CharacterDetails GetCharacterDetails(int index);

            //function: GetCharactersDetailsCount
            //Returns the number of characater details, same as calling <GetCharacterCount>
            virtual int GetCharactersDetailsCount() const;

            //function: AddCharacterDetails
            //Add the character details at the index position
            virtual void AddCharacterDetails(int index, ssGUI::CharacterDetails details);

            //function: AddCharacterDetails
            //Add the character details to the end
            virtual void AddCharacterDetails(ssGUI::CharacterDetails details);

            //function: AddCharacterDetails
            //Add a number of character details at the index position
            virtual void AddCharacterDetails(int index, std::vector<ssGUI::CharacterDetails>& details);
            
            //function: AddCharacterDetails
            //Add a number of character details to the end
            virtual void AddCharacterDetails(std::vector<ssGUI::CharacterDetails>& details);

            //function: RemoveCharacterDetails
            //Removes the character details at the index position
            virtual void RemoveCharacterDetails(int index);

            //function: RemoveCharacterDetails
            //Removes a range of character details
            virtual void RemoveCharacterDetails(int startIndex, int exclusiveEndIndex);

            //function: ClearAllCharacterDetails
            //Removes all override character details
            virtual void ClearAllCharacterDetails();

            //function: GetCharacterGlobalPosition
            //Gets the global position of the character
            //If topLeftCorner is true, this will return the top-left corner of the character,
            //Otherwise this will return the left-most position of the character on the baseline (Bottom Left corner).
            virtual glm::vec2 GetCharacterGlobalPosition(int index, bool topLeftCorner);
            
            //function: IsOverflow
            //Returns true if the text is overflowing the text widget
            virtual bool IsOverflow() const;
            
            //function: SetNewTextFontSize
            //Sets the size of the font being used for new text being added or set
            virtual void SetNewTextFontSize(float size);
            
            //function: GetNewTextFontSize
            //Returns the size of the font being used for new text being added or set
            virtual float GetNewTextFontSize() const;

            //function: SetNewTextColor
            //Sets the text color being used for new text being added or set
            virtual void SetNewTextColor(glm::u8vec4 color);

            //function: GetNewTextColor
            //Gets the text color being used for new text being added or set
            virtual glm::u8vec4 GetNewTextColor() const;

            //function: SetNewTextUnderlined
            //Sets if text is underlined for new text being added or set
            virtual void SetNewTextUnderlined(bool underline);

            //function: IsNewTextUnderlined
            //Returns if text is underlined for new text being added or set
            virtual bool IsNewTextUnderlined() const;
            
            //function: ApplyNewTextSettingsToExistingText
            //Applies the new character settings (color, font size, underline, etc...) to all characters
            virtual void ApplyNewTextSettingsToExistingText();

            //function: SetMultilineAllowed
            //If true, newlines will be allowed
            virtual void SetMultilineAllowed(bool multiline);
            
            //function: IsMultilineAllowed
            //If true, newlines will be allowed
            virtual bool IsMultilineAllowed() const;
            
            //function: SetWrappingMode
            //Sets the wrapping mode of this text widget
            virtual void SetWrappingMode(ssGUI::Enums::TextWrapping wrappingMode);
            
            //function: GetWrappingMode
            //Gets the wrapping mode of this text widget
            virtual ssGUI::Enums::TextWrapping GetWrappingMode() const;

            //function: SetTextHorizontalAlignment
            virtual void SetTextHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal align);

            //function: GetTextHorizontalAlignment
            virtual ssGUI::Enums::AlignmentHorizontal GetTextHorizontalAlignment() const;

            //function: SetTextVerticalAlignment
            virtual void SetTextVerticalAlignment(ssGUI::Enums::AlignmentVertical align);

            //function: GetTextVerticalAlignment
            virtual ssGUI::Enums::AlignmentVertical GetTextVerticalAlignment() const;
            
            //function: SetTextAlignment
            virtual void SetTextAlignment(ssGUI::Enums::AlignmentHorizontal hori, ssGUI::Enums::AlignmentVertical vert);

            //function: AddFont
            //Adds the font to the end for this text object. Multiple fonts can be added as "fall back" if the character is not supported by it.
            virtual void AddFont(ssGUI::Font* font);

            //function: AddFont
            //Adds the font to the index position for this text object. Multiple fonts can be added as "fall back" if the character is not supported by it.
            virtual void AddFont(ssGUI::Font* font, int index);
            
            //function: GetFont
            virtual ssGUI::Font* GetFont(int index) const;

            //function: SetFont
            virtual void SetFont(ssGUI::Font* font, int index);

            //function: RemoveFont
            virtual void RemoveFont(int index);

            //function: GetFontsCount
            virtual int GetFontsCount() const;

            //function: SetTextHorizontalPadding
            //Sets the horizontal padding for the beginning and the end of the text
            virtual void SetTextHorizontalPadding(float padding);

            //function: GetTextHorizontalPadding
            //Gets the horizontal padding for the beginning and the end of the text
            virtual float GetTextHorizontalPadding() const;

            //function: SetTextVerticalPadding
            //Sets the vertical padding for the beginning and the end of the text
            virtual void SetTextVerticalPadding(float padding);

            //function: GetTextVerticalPadding
            //Sets the vertical padding for the beginning and the end of the text
            virtual float GetTextVerticalPadding() const;
            
            //function: SetCharacterSpace
            //Sets the additional space between each character
            virtual void SetCharacterSpace(float charSpace);
            
            //function: GetCharacterSpace
            //Gets the additional space between each character
            virtual float GetCharacterSpace() const;
            
            //function: SetLineSpace
            //Sets the additional space between each line
            virtual void SetLineSpace(float lineSpace);
            
            //function: GetLineSpace
            //Gets the additional space between each line
            virtual float GetLineSpace() const;
            
            //function: SetTabSize
            //Sets how many space each tab is
            virtual void SetTabSize(float tabSize);
            
            //function: GetTabSize
            //Gets how many space each tab is
            virtual float GetTabSize() const;

            //function: SetTextSelectionAllowed
            //Sets if text selection is allowed
            virtual void SetTextSelectionAllowed(bool allowed);

            //function: IsTextSelectionAllowed
            //Returns if text selection is allowed
            virtual bool IsTextSelectionAllowed() const;

            //function: SetStartSelectionIndex
            //Sets the start selection index. 
            //If start and end index is the same, it just indicates the caret position and nothing is selected.
            //If start index is before the end index, characters in the range of start (inclusive) and end (exclusive) index are selected.
            //If start index is after the end index, characters in the range of start (exclusive) and end (inclusive) index are selected.
            //Note that it is possible for the start/end selection index to be equal to the total characters count.
            //In that case, the caret is placed after the last character.
            virtual void SetStartSelectionIndex(int index);

            //function: GetStartSelectionIndex
            //Gets the start selection index. Returns -1 if selection is not allowed or invalid
            //For explaination on how selection works, see <SetStartSelectionIndex>
            virtual int GetStartSelectionIndex() const;

            //function: SetEndSelectionIndex
            //Sets the end selection index
            //For explaination on how selection works, see <SetStartSelectionIndex>
            virtual void SetEndSelectionIndex(int index);

            //function: GetEndSelectionIndex
            //Gets the end selection index. Returns -1 if selection is not allowed or invalid
            virtual int GetEndSelectionIndex() const;

            //function: SetSelectionColor
            //Sets the selection color
            virtual void SetSelectionColor(glm::u8vec4 color);

            //function: GetSelectionColor
            //Gets the selection color
            virtual glm::u8vec4 GetSelectionColor() const;

            //function: SetTextSelectedColor
            //Sets the text color when being selected
            virtual void SetTextSelectedColor(glm::u8vec4 color);

            //function: GetTextSelectedColor
            //Gets the text color when being selected
            virtual glm::u8vec4 GetTextSelectedColor() const;

            //function: SetDeselectWhenFocusLost
            //Sets the text to deselect when its focus is lost or not
            virtual void SetDeselectWhenFocusLost(bool deselectWhenFocusLost);
            
            //function: IsDeselectWhenFocusLost
            //Returns the text to deselect when its focus is lost or not
            virtual bool IsDeselectWhenFocusLost() const;

            //function: GetContainedCharacterIndexFromPos
            //Gets the character index if the passed in position is contained inside a character
            virtual int GetContainedCharacterIndexFromPos(glm::vec2 pos);
            
            //funciton: GetNearestCharacterIndexFromPos
            //Gets the character index that is closest to the passed in position.
            //If useLeftEdge, it will use the left side of the character instead of the center of it.
            virtual int GetNearestCharacterIndexFromPos(glm::vec2 pos, bool useLeftEdge);

            //function: GetFirstValidCharacterIndex
            //Return the index of the first valid character (<ssGUI::CharacterRenderInfo::Valid>) 
            virtual int GetFirstValidCharacterIndex();

            //function: GetLastValidCharacterIndex
            //Return the index of the last valid character (<ssGUI::CharacterRenderInfo::Valid>)
            virtual int GetLastValidCharacterIndex();

            //function: IsPosAfterLastCharacter
            //Returns if the x position is before or after the right side of the last character
            virtual bool IsPosAfterLastCharacter(glm::vec2 pos);

            //function: AddDefaultFont
            //Adds the font to the end of default fonts. Multiple fonts can be added as "fall back" if the character is not supported by it.
            static ssGUI::Font* AddDefaultFont();

            //TODO: Add ability to add default font from another default font

            //function: AddDefaultFont
            //Adds the font to the index position of default fonts. Multiple fonts can be added as "fall back" if the character is not supported by it.
            static ssGUI::Font* AddDefaultFont(int index);

            //function: GetDefaultFont
            static ssGUI::Font* GetDefaultFont(int index);

            //function: RemoveDefaultFont
            static void RemoveDefaultFont(int index);

            //function: GetDefaultFontsCount 
            static int GetDefaultFontsCount();
            
            //function: GetType
            //See <GUIObject::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: Clone
            //See <GUIObject::Clone>
            virtual Text* Clone(bool cloneChildren) override;
            
            //function: InitiateDefaultResources
            //See <GUIObject::InitiateDefaultResources>
            virtual void InitiateDefaultResources() override;
            
            //function: CleanUpDefaultResources
            //See <GUIObject::CleanUpDefaultResources>
            virtual void CleanUpDefaultResources() override;
    };
}


#endif