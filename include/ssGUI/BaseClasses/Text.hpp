#ifndef SSGUI_TEXT
#define SSGUI_TEXT

#include "ssGUI/BaseClasses/Font.hpp"
#include "ssGUI/BaseClasses/Widget.hpp"
#include "ssGUI/BaseClasses/CharacterInfo.hpp"
#include "ssGUI/Enums/TextWrapping.hpp"
#include "ssGUI/Enums/TextAlignmentHorizontal.hpp"
#include "ssGUI/Enums/TextAlignmentVertical.hpp"
#include "ssGUI/Extensions/Border.hpp"
#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/EventCallbacks/OnFontChangeEventCallback.hpp"
#include <string>
#include <locale>
#include <codecvt>

//namespace: ssGUI
namespace ssGUI
{
    //class: Text
    class Text : public Widget
    {
        private:
            Text& operator=(Text const& other);

        protected:
            std::wstring CurrentText;

            bool RecalculateTextNeeded;
            std::vector<glm::ivec2> CharactersPosition;
            std::vector<ssGUI::CharacterInfo> CharactersInfos;

            bool WrappingOverflow;
            int FontSize;
            bool MultilineAllowed;
            ssGUI::Enums::TextWrapping WrappingMode;
            ssGUI::Enums::TextAlignmentHorizontal HorizontalAlignment;
            ssGUI::Enums::TextAlignmentVertical VerticalAlignment;
            ssGUI::Font* CurrentFont;

            int HorizontalPadding;
            int VerticalPadding;
            int CharacterSpace;
            int LineSpace;
            float TabSize;
            ssGUI::Font* LastDefaultFont;

            static ssGUI::Font* DefaultFont;

            Text(Text const& other);

            virtual void DrawCharacter(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset,
                                        wchar_t charcterToDraw, glm::ivec2 position, ssGUI::CharacterInfo info);

        public:
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
            
            //function: GetCharacterGlobalPosition
            //Gets the global position of the character
            virtual glm::ivec2 GetCharacterGlobalPosition(int index) const;
            
            //function: GetCharacterInfo
            //Gets the info of the character
            virtual ssGUI::CharacterInfo GetCharacterInfo(int index) const;
            
            //function: IsWrappingOverflow
            //Returns true if the text is overflowing the text widget
            virtual bool IsWrappingOverflow() const;
            
            //function: SetFontSize
            //Sets the size of the font being used
            virtual void SetFontSize(int size);
            
            //function: GetFontSize
            //Returns the size of the font being used
            virtual int GetFontSize() const;
            
            //function: SetMultilineAllowed
            //If true, the text being shown will be wrapped to multiple lines if needed
            virtual void SetMultilineAllowed(bool multiline);
            
            //function: IsMultilineAllowed
            //If true, the text being shown will be wrapped to multiple lines if needed
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
            virtual ssGUI::Enums::TextAlignmentHorizontal GetHorizontalAlignment();

            //function: SetVerticalAlignment
            virtual void SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical align);

            //function: GetVerticalAlignment
            virtual ssGUI::Enums::TextAlignmentVertical GetVerticalAlignment();

            //function: SetFont
            virtual void SetFont(ssGUI::Font* font);
            
            //function: GetFont
            virtual ssGUI::Font* GetFont();

            //function: SetHorizontalPadding
            //Sets the horizontal padding for the beginning and the end of the text
            virtual void SetHorizontalPadding(int padding);

            //function: GetHorizontalPadding
            //Gets the horizontal padding for the beginning and the end of the text
            virtual int GetHorizontalPadding();

            //function: SetVerticalPadding
            //Sets the vertical padding for the beginning and the end of the text
            virtual void SetVerticalPadding(int padding);

            //function: GetVerticalPadding
            //Sets the vertical padding for the beginning and the end of the text
            virtual int GetVerticalPadding();
            
            //function: SetCharacterSpace
            //Sets the space between each character
            virtual void SetCharacterSpace(int charSpace);
            
            //function: GetCharacterSpace
            //Gets the space between each character
            virtual int GetCharacterSpace() const;
            
            //function: SetLineSpace
            //Sets the space between each line
            virtual void SetLineSpace(int lineSpace);
            
            //function: GetLineSpace
            //Gets the space between each line
            virtual int GetLineSpace() const;
            
            //function: SetTabSize
            //Sets how many space each tab is
            virtual void SetTabSize(float tabSize);
            
            //function: GetTabSize
            //Gets how many space each tab is
            virtual float GetTabSize() const;

            //function: SetDefaultFont
            //Sets the default font for all text widget
            static void SetDefaultFont(ssGUI::Font* font);

            //function: GetDefaultFont
            //Gets the default font for all text widget
            static ssGUI::Font* GetDefaultFont();
            
            //function: GetType
            //See <GUIObject::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;
            
            //function: Delete 
            //See <GUIObject::Delete>
            virtual void Delete() override;

            //function: Internal_Draw
            //See <GUIObject::Internal_Draw>
            virtual void Internal_Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset) override;
            //virtual void Internal_Update(ssGUI::BackendSystemInputInterface& inputInterface, bool& blockAllInput, bool& blockInputInWindow, ssGUI::GUIObject* mainWindow) override;
            
            //function: Clone
            //See <GUIObject::Clone>
            virtual GUIObject* Clone(bool cloneChildren) override;

    };
}


#endif