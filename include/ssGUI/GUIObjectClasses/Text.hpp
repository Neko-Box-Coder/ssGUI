#ifndef SSGUI_TEXT
#define SSGUI_TEXT

#include "ssGUI/DataClasses/Font.hpp"
#include "ssGUI/GUIObjectClasses/Widget.hpp"
#include "ssGUI/DataClasses/CharacterDetails.hpp"
#include "ssGUI/DataClasses/CharacterRenderInfo.hpp"
#include "ssGUI/Enums/TextWrapping.hpp"
#include "ssGUI/Enums/TextAlignmentHorizontal.hpp"
#include "ssGUI/Enums/TextAlignmentVertical.hpp"
#include "ssGUI/Extensions/Border.hpp"
#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/EventCallbacks/OnFontChangeEventCallback.hpp"
#include "ssGUI/EventCallbacks/SizeChangedEventCallback.hpp"
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
        bool RecalculateTextNeeded;
        ssGUI::SegmentedVector<ssGUI::CharacterDetails> CurrentCharactersDetails;

        std::vector<ssGUI::CharacterRenderInfo> CharactersRenderInfos;
        std::unordered_map<int, ssGUI::CharacterDetails> ProcessedCharacterDetails;

        bool Overflow;
        float FontSize;
        glm::u8vec4 TextColor;
        bool TextUnderline;
        bool MultilineAllowed;
        ssGUI::Enums::TextWrapping WrappingMode;
        ssGUI::Enums::TextAlignmentHorizontal HorizontalAlignment;
        ssGUI::Enums::TextAlignmentVertical VerticalAlignment;            
        std::vector<ssGUI::Font*> CurrentFonts;

        float HorizontalPadding;
        float VerticalPadding;
        float CharacterSpace;
        float LineSpace;
        float TabSize;
        bool SelectionAllowed;
        int StartSelectionIndex;
        int EndSelectionIndex;

        glm::u8vec4 SelectionColor;
        glm::u8vec4 TextSelectedColor;

        std::vector<ssGUI::Font*> LastDefaultFonts;

        static std::vector<ssGUI::Font*> DefaultFonts;
    =================================================================
    ============================== C++ ==============================
    Text::Text() :  RecalculateTextNeeded(false), CurrentCharactersDetails(), 
                    CharactersRenderInfos(), ProcessedCharacterDetails(), Overflow(false), FontSize(15), TextColor(glm::u8vec4(0, 0, 0, 255)), 
                    TextUnderline(false), MultilineAllowed(true), WrappingMode(ssGUI::Enums::TextWrapping::NO_WRAPPING), 
                    HorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::LEFT), VerticalAlignment(ssGUI::Enums::TextAlignmentVertical::TOP), 
                    CurrentFonts(), HorizontalPadding(5), VerticalPadding(5), CharacterSpace(0), LineSpace(0), TabSize(4), SelectionAllowed(true),
                    StartSelectionIndex(-1), EndSelectionIndex(-1), SelectionColor(51, 153, 255, 255), TextSelectedColor(255, 255, 255, 255), 
                    LastDefaultFonts()
    {
        SetBackgroundColor(glm::ivec4(255, 255, 255, 0));
        SetBlockInput(false);
        SetInteractable(true);

        auto sizeChangedCallback = ssGUI::Factory::Create<ssGUI::EventCallbacks::SizeChangedEventCallback>();
        sizeChangedCallback->AddEventListener
        (
            ListenerKey, this,
            [](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* refs)
            {
                static_cast<ssGUI::Text*>(src)->RecalculateTextNeeded = true;
            }
        );

        AddEventCallback(sizeChangedCallback);
    }
    =================================================================

    DefaultFonts initialization
    ============================== C++ ==============================
    std::vector<ssGUI::Font*> Text::DefaultFonts = []()->std::vector<ssGUI::Font*>
    {
        FUNC_DEBUG_ENTRY("LoadDefaultFont");

        std::vector<ssGUI::Font*> defaultFonts;

        auto font = new ssGUI::Font();
        if(!font->GetBackendFontInterface()->LoadFromPath("Resources/NotoSans-Regular.ttf"))
        {
            DEBUG_LINE("Failed to load default font");
            delete font;
            FUNC_DEBUG_EXIT("LoadDefaultFont");
            return defaultFonts;
        }
        else
        {
            defaultFonts.push_back(font);
            FUNC_DEBUG_EXIT("LoadDefaultFont");
            return defaultFonts;
        }
        FUNC_DEBUG_EXIT("LoadDefaultFont");
    }();    //Brackets at the end to call this lambda, pretty cool.
    =================================================================
    */
    class Text : public Widget
    {
        private:
            Text& operator=(Text const& other);

        protected:
            bool RecalculateTextNeeded;
            ssGUI::SegmentedVector<ssGUI::CharacterDetails> CurrentCharactersDetails;

            std::vector<ssGUI::CharacterRenderInfo> CharactersRenderInfos;
            std::unordered_map<int, ssGUI::CharacterDetails> ProcessedCharacterDetails;

            bool Overflow;
            float FontSize;
            glm::u8vec4 TextColor;
            bool TextUnderline;
            bool MultilineAllowed;
            ssGUI::Enums::TextWrapping WrappingMode;
            ssGUI::Enums::TextAlignmentHorizontal HorizontalAlignment;
            ssGUI::Enums::TextAlignmentVertical VerticalAlignment;            
            std::vector<ssGUI::Font*> CurrentFonts;

            float HorizontalPadding;
            float VerticalPadding;
            float CharacterSpace;
            float LineSpace;
            float TabSize;
            bool SelectionAllowed;
            int StartSelectionIndex;
            int EndSelectionIndex;

            glm::u8vec4 SelectionColor;
            glm::u8vec4 TextSelectedColor;

            std::vector<ssGUI::Font*> LastDefaultFonts;

            static std::vector<ssGUI::Font*> DefaultFonts;

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
            virtual std::wstring GetText();
            
            //function: GetCharacterCount
            //Gets the number of characters for the text being shown
            virtual int GetCharacterCount() const;
            
            //function: GetCharacterRenderInfo
            //Gets the character render info of the character at the index position
            virtual ssGUI::CharacterRenderInfo GetCharacterRenderInfo(int index) const;

            //function: SetCurrentCharacterDetails
            //Sets the character details at the index position
            virtual void SetCurrentCharacterDetails(int index, ssGUI::CharacterDetails details);

            //function: GetCurrentCharacterDetails
            //Gets the character details at the index position
            virtual ssGUI::CharacterDetails GetCurrentCharacterDetails(int index);

            //function: GetCurrentCharactersDetailsCount
            //Returns the number of characater details, same as calling <GetCharacterCount>
            virtual int GetCurrentCharactersDetailsCount() const;

            //function: AddCurrentCharacterDetails
            //Add the character details at the index position
            virtual void AddCurrentCharacterDetails(int index, ssGUI::CharacterDetails details);

            //function: AddCurrentCharacterDetails
            //Add the character details to the end
            virtual void AddCurrentCharacterDetails(ssGUI::CharacterDetails details);

            //function: AddCurrentCharacterDetails
            //Add a number of character details at the index position
            virtual void AddCurrentCharacterDetails(int index, std::vector<ssGUI::CharacterDetails>& details);
            
            //function: AddCurrentCharacterDetails
            //Add a number of character details to the end
            virtual void AddCurrentCharacterDetails(std::vector<ssGUI::CharacterDetails>& details);

            //function: RemoveCurrentCharacterDetails
            //Removes the character details at the index position
            virtual void RemoveCurrentCharacterDetails(int index);

            //function: RemoveCurrentCharacterDetails
            //Removes a range of character details
            virtual void RemoveCurrentCharacterDetails(int startIndex, int exclusiveEndIndex);

            //function: ClearAllOverrideCharacterDetails
            //Removes all override character details
            virtual void ClearAllCurrentCharacterDetails();

            //function: GetCharacterGlobalPosition
            //Gets the global position of the character
            //If topLeftCorner is true, this will return the top-left corner of the character,
            //Otherwise this will return the left-most position of the character on the horizontal line (Bottom Left corner).
            virtual glm::vec2 GetCharacterGlobalPosition(int index, bool topLeftCorner);
            
            //function: IsOverflow
            //Returns true if the text is overflowing the text widget
            virtual bool IsOverflow() const;
            
            //function: SetNewCharacterFontSize
            //Sets the size of the font being used for new characters
            virtual void SetNewCharacterFontSize(float size);
            
            //function: GetNewCharacterFontSize
            //Returns the size of the font being used for new characters
            virtual float GetNewCharacterFontSize() const;

            //function: SetNewCharacterColor
            //Sets the text color being used for new characters
            virtual void SetNewCharacterColor(glm::u8vec4 color);

            //function: GetNewCharacterColor
            //Gets the text color being used for new characters
            virtual glm::u8vec4 GetNewCharacterColor() const;

            //function: SetNewCharacterUnderlined
            //Sets if text is underlined for new characters
            virtual void SetNewCharacterUnderlined(bool underline);

            //function: IsNewCharacterUnderlined
            //Returns if text is underlined for new characters
            virtual bool IsNewCharacterUnderlined() const;
            
            //function: ApplyNewCharacterSettingsToText
            //Applies the new character settings (color, font size, underline, etc...) to all characters
            virtual void ApplyNewCharacterSettingsToText();

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

            //function: SetHorizontalAlignment
            virtual void SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal align);

            //function: GetHorizontalAlignment
            virtual ssGUI::Enums::TextAlignmentHorizontal GetHorizontalAlignment() const;

            //function: SetVerticalAlignment
            virtual void SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical align);

            //function: GetVerticalAlignment
            virtual ssGUI::Enums::TextAlignmentVertical GetVerticalAlignment() const;

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

            //function: SetHorizontalPadding
            //Sets the horizontal padding for the beginning and the end of the text
            virtual void SetHorizontalPadding(float padding);

            //function: GetHorizontalPadding
            //Gets the horizontal padding for the beginning and the end of the text
            virtual float GetHorizontalPadding() const;

            //function: SetVerticalPadding
            //Sets the vertical padding for the beginning and the end of the text
            virtual void SetVerticalPadding(float padding);

            //function: GetVerticalPadding
            //Sets the vertical padding for the beginning and the end of the text
            virtual float GetVerticalPadding() const;
            
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
            //Sets the start selection index. If start and end index is the same, nothing is selected.
            //If start index is before the end index, characters in the range of start (inclusive) and end (exclusive) index are selected.
            //If start index is after the end index, characters in the range of start (exclusive) and end (inclusive) index are selected.
            //Note that it is possible for the start/end selection index to be equal to the total characters count.
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
            static void AddDefaultFont(ssGUI::Font* font);

            //function: AddDefaultFont
            //Adds the font to the index position of default fonts. Multiple fonts can be added as "fall back" if the character is not supported by it.
            static void AddDefaultFont(ssGUI::Font* font, int index);

            //function: GetDefaultFont
            static ssGUI::Font* GetDefaultFont(int index);

            //function: SetDefaultFont
            static void SetDefaultFont(ssGUI::Font* font, int index);

            //function: RemoveDefaultFont
            static void RemoveDefaultFont(int index);

            //function:GetDefaultFontsCount 
            static int GetDefaultFontsCount();
            
            //function: GetType
            //See <GUIObject::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: Clone
            //See <GUIObject::Clone>
            virtual Text* Clone(bool cloneChildren) override;

    };
}


#endif