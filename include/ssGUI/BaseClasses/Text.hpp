#ifndef SSGUI_TEXT
#define SSGUI_TEXT

#include "ssGUI/BaseClasses/Font.hpp"
#include "ssGUI/BaseClasses/Widget.hpp"
#include "ssGUI/BaseClasses/CharacterInfo.hpp"
#include "ssGUI/Enums/TextWrapping.hpp"
#include "ssGUI/Extensions/Border.hpp"
#include <string>
#include "ssGUI/Backend/BackendFactory.hpp"

//namespace: ssGUI
namespace ssGUI
{
    //TODO : Add original text
    //TODO : allow to get show text and original text

    
    
    //class: Text
    class Text : public Widget
    {
        private:
            std::wstring CurrentText;

            bool CurrentTextChanged;
            std::vector<glm::ivec2> CharactersPosition;
            std::vector<ssGUI::CharacterInfo> CharactersInfos;

            bool WrappingOverflow;
            int FontSize;
            bool MultilineAllowed;
            ssGUI::Enums::TextWrapping WrappingMode;           
            ssGUI::Font* CurrentFont;

            int CharacterSpace;
            int LineSpace;
            float TabSize;

            Text& operator=(Text const& other);

        protected:
            Text(Text const& other);

            virtual void DrawCharacter(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset,
                                        wchar_t charcterToDraw, glm::ivec2 position, ssGUI::CharacterInfo info);

        public:
            Text();
            virtual ~Text() override;

            //function: ComputeCharactersPositionAndSize
            virtual void ComputeCharactersPositionAndSize();
            
            //function: SetText
            virtual void SetText(std::wstring text);
            
            //function: GetText
            virtual std::wstring GetText() const;
            
            //function: GetCharacterCount
            virtual int GetCharacterCount() const;
            
            //function: GetCharacterGlobalPosition
            virtual glm::ivec2 GetCharacterGlobalPosition(int index) const;
            
            //function: GetCharacterInfo
            virtual ssGUI::CharacterInfo GetCharacterInfo(int index) const;
            
            //function: IsWrappingOverflow
            virtual bool IsWrappingOverflow() const;
            
            //function: SetFontSize
            virtual void SetFontSize(int size);
            
            //function: GetFontSize
            virtual int GetFontSize() const;
            
            //function: SetMultilineAllowed
            virtual void SetMultilineAllowed(bool multiline);
            
            //function: IsMultilineAllowed
            virtual bool IsMultilineAllowed() const;
            
            //function: SetWrappingMode
            virtual void SetWrappingMode(ssGUI::Enums::TextWrapping wrappingMode);
            
            //function: GetWrappingMode
            virtual ssGUI::Enums::TextWrapping GetWrappingMode() const;
            
            //function: SetFont
            virtual void SetFont(ssGUI::Font* font);
            
            //function: GetFont
            virtual ssGUI::Font* GetFont();
            
            //function: SetCharacterSpace
            virtual void SetCharacterSpace(int charSpace);
            
            //function: GetCharacterSpace
            virtual int GetCharacterSpace() const;
            
            //function: SetLineSpace
            virtual void SetLineSpace(int lineSpace);
            
            //function: GetLineSpace
            virtual int GetLineSpace() const;
            
            //function: SetTabSize
            virtual void SetTabSize(float tabSize);
            
            //function: GetTabSize
            virtual float GetTabSize() const;
            
            //function: GetType
            virtual ssGUI::Enums::GUIObjectType GetType() const override;
            
            //function: Draw
            virtual void Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset) override;
            //virtual void Internal_Update(ssGUI::BackendSystemInputInterface& inputInterface, bool& blockAllInput, bool& blockInputInWindow, ssGUI::GUIObject* mainWindow) override;
            
            //function: Clone
            virtual GUIObject* Clone(std::vector<GUIObject*>& originalObjs, bool cloneChildren) override;
    };
}


#endif