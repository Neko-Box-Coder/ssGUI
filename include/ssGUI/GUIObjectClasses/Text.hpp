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
#include <string>
#include <locale>
#include <codecvt>

//namespace: ssGUI
namespace ssGUI
{
    /*class: ssGUI::Text
    A class for showing text with different options such as font size, text alignment, fonts, etc.
    Text can be added by just setting the text, which is the simplest.
    
    If you want the ablitiy to adjust each characters settings such as font, font size, etc. 
    You can add <ssGUI::CharacterDetails> as OverrideCharacterDetails by setting the object variables.
    Please note that if you are using OverrideCharacterDetails, text set by <SetText> will be *ignored*.

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        std::wstring CurrentText;

        bool RecalculateTextNeeded;
        std::vector<ssGUI::CharacterDetails> OverrideCharactersDetails;
        std::vector<ssGUI::CharacterRenderInfo> CharactersRenderInfos;
        std::vector<ssGUI::CharacterDetails> CurrentCharacterDetails;

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
        std::vector<ssGUI::Font*> LastDefaultFonts;

        static std::vector<ssGUI::Font*> DefaultFonts;
    =================================================================
    ============================== C++ ==============================
    Text::Text() :  CurrentText(), RecalculateTextNeeded(false), OverrideCharactersDetails(), 
                    CharactersRenderInfos(), CurrentCharacterDetails(), Overflow(false), FontSize(20), TextColor(glm::u8vec4(0, 0, 0, 255)), 
                    TextUnderline(false), MultilineAllowed(true), WrappingMode(ssGUI::Enums::TextWrapping::NO_WRAPPING), 
                    HorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::LEFT), VerticalAlignment(ssGUI::Enums::TextAlignmentVertical::TOP), 
                    CurrentFonts(), HorizontalPadding(5), VerticalPadding(5), CharacterSpace(0), LineSpace(0), TabSize(4), LastDefaultFonts()
    {
        SetBackgroundColor(glm::ivec4(255, 255, 255, 0));
        SetBlockInput(false);
        SetInteractable(false);

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
            std::wstring CurrentText;

            bool RecalculateTextNeeded;
            std::vector<ssGUI::CharacterDetails> OverrideCharactersDetails;
            std::vector<ssGUI::CharacterRenderInfo> CharactersRenderInfos;
            std::vector<ssGUI::CharacterDetails> CurrentCharacterDetails;

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
            std::vector<ssGUI::Font*> LastDefaultFonts;

            static std::vector<ssGUI::Font*> DefaultFonts;

            Text(Text const& other);

            virtual void ConstructCharacterDetails();

            virtual void AssignSupportedFont();

            virtual void DrawCharacter(glm::vec2 positionOffset, ssGUI::CharacterRenderInfo info, ssGUI::CharacterDetails details);
            
            virtual void FormatNewlinesCharacters();

            virtual void ConstructRenderInfosForWordWrapping();

            virtual void ConstructRenderInfosForCharacterWrapping();

            virtual void ConstructRenderInfosForNoWrapping();

            virtual void ApplyFontLineSpacing();

            virtual void ApplyTextAlignment();

            virtual void ApplyTextUnderline();

            virtual void ConstructRenderInfo() override;

            virtual void MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, 
                ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;

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
            
            //function: GetText
            //Gets the text being shown
            virtual std::wstring GetText() const;
            
            //function: GetCharacterCount
            //Gets the number of characters for the text being shown
            virtual int GetCharacterCount() const;
            
            //function: GetCharacterRenderInfo
            //Gets the character render info of the character at the index position
            virtual ssGUI::CharacterRenderInfo GetCharacterRenderInfo(int index) const;

            //function: SetOverrideCharacterDetails
            //Sets the override character details of the character at the index position
            virtual void SetOverrideCharacterDetails(int index, ssGUI::CharacterDetails details);

            //function: GetOverrideCharacterDetails
            //Gets the override character details of the character at the index position
            virtual ssGUI::CharacterDetails GetOverrideCharacterDetails(int index) const;

            //function: GetOverrideCharactersDetailsCount
            //Returns the number of override characater details
            virtual int GetOverrideCharactersDetailsCount() const;

            //function: AddOverrideCharacterDetails
            //Add the override character details of the character at the index position
            virtual void AddOverrideCharacterDetails(int index, ssGUI::CharacterDetails details);

            //function: AddOverrideCharacterDetails
            //Add the override character details of the character to the end
            virtual void AddOverrideCharacterDetails(ssGUI::CharacterDetails details);

            //function: RemoveOverrideCharacterDetails
            //Removes the override character details of the character at the index position
            virtual void RemoveOverrideCharacterDetails(int index);

            //function: ClearAllOverrideCharacterDetails
            //Removes all override character details
            virtual void ClearAllOverrideCharacterDetails();

            //function: GetCharacterGlobalPosition
            //Gets the global position of the character
            //If topLeftCorner is true, this will return the top-left corner of the character,
            //Otherwise this will return the left-most position of the character on the horizontal line (Bottom Left corner).
            virtual glm::vec2 GetCharacterGlobalPosition(int index, bool topLeftCorner);
            
            //function: IsOverflow
            //Returns true if the text is overflowing the text widget
            virtual bool IsOverflow() const;
            
            //function: SetFontSize
            //Sets the size of the font being used (when there's no override character details)
            virtual void SetFontSize(float size);
            
            //function: GetFontSize
            //Returns the size of the font being used (when there's no override character details)
            virtual float GetFontSize() const;

            //function: SetTextColor
            //Sets the text color being used (when there's no override character details)
            virtual void SetTextColor(glm::u8vec4 color);

            //function: GetTextColor
            //Gets the text color being used (when there's no override character details)
            virtual glm::u8vec4 GetTextColor() const;

            //function: SetTextUnderlined
            //Sets if text is underlined (when there's no override character details)
            virtual void SetTextUnderlined(bool underline);

            //function: IsTextUnderlined
            //Returns if text is underlined (when there's no override character details)
            virtual bool IsTextUnderlined() const;
            
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

            //function: GetContainedCharacterIndexFromPos
            //Gets the character index if the passed in position is contained inside a character
            virtual int GetContainedCharacterIndexFromPos(glm::vec2 pos);
            
            //funciton: GetNearestCharacterIndexFromPos
            //Gets the character index that is closest to the passed in position
            virtual int GetNearestCharacterIndexFromPos(glm::vec2 pos);

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