#include "ssGUI/BaseClasses/Text.hpp"

namespace ssGUI
{    
    Text::Text(Text const& other) : Widget(other)
    {
        CurrentFont = other.CurrentFont->Clone();

        SetText(other.GetText());
        CurrentTextChanged = true;
        CharactersPosition = other.CharactersPosition;
        CharactersInfos = other.CharactersInfos;

        WrappingOverflow = other.WrappingOverflow;
        SetFontSize(other.GetFontSize());
        SetMultilineAllowed(other.IsMultilineAllowed());
        SetWrappingMode(other.GetWrappingMode());
        SetCharacterSpace(other.GetCharacterSpace());
        SetLineSpace(other.GetLineSpace());
        SetTabSize(other.GetTabSize());
    }

    //TODO: Maybe remove drawingInterface as it is not used anywhere?
    void Text::DrawCharacter(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset,
                        wchar_t charcterToDraw, glm::ivec2 position, ssGUI::CharacterInfo info)
    {
        DrawingVerticies.push_back(position                                 + info.DrawOffset           + glm::ivec2(0, FontSize));
        DrawingVerticies.push_back(position + glm::ivec2(info.Size.x, 0)    + info.DrawOffset           + glm::ivec2(0, FontSize));
        DrawingVerticies.push_back(position + info.Size                     + info.DrawOffset           + glm::ivec2(0, FontSize));
        DrawingVerticies.push_back(position + glm::ivec2(0, info.Size.y)    + info.DrawOffset           + glm::ivec2(0, FontSize));

        DrawingColours.push_back(glm::u8vec4(0, 0, 0, 255));
        DrawingColours.push_back(glm::u8vec4(0, 0, 0, 255));
        DrawingColours.push_back(glm::u8vec4(0, 0, 0, 255));
        DrawingColours.push_back(glm::u8vec4(0, 0, 0, 255));

        DrawingUVs.push_back(info.UVOrigin);
        DrawingUVs.push_back(info.UVOrigin + glm::ivec2(info.Size.x, 0));
        DrawingUVs.push_back(info.UVOrigin + info.Size);
        DrawingUVs.push_back(info.UVOrigin + glm::ivec2(0, info.Size.y));

        DrawingCounts.push_back(4);
        ssGUI::DrawingProperty currentProperty;
        currentProperty.fontP = (GetFont()->GetBackendFontInterface());
        currentProperty.characterSize = GetFontSize();

        DrawingProperties.push_back(currentProperty);
    }
    
    
    Text::Text() :  CurrentText(), CurrentTextChanged(false), CharactersPosition(), 
                    CharactersInfos(), WrappingOverflow(false), FontSize(20), MultilineAllowed(true), 
                    WrappingMode(ssGUI::Enums::TextWrapping::NO_WRAPPING), CurrentFont(nullptr),
                    CharacterSpace(0), LineSpace(0), TabSize(4)
    {
        //AddExtension(new ssGUI::Extensions::Border());
    }

    Text::~Text()
    {}
    
    //TODO : Maybe subdivide this function
    void Text::ComputeCharactersPositionAndSize()
    {
        CurrentTextChanged = false;
        WrappingOverflow = false;

        if(CurrentFont == nullptr)
            return;
        
        int numberOfLines = 1;

        //If multiline is disabled, replace all newline to spaces and remove all \r
        if(!MultilineAllowed)
        {
            for(int i = 0; i < CurrentText.size(); i++)
            {
                if(CurrentText.at(i) == L'\n')
                    CurrentText.at(i) = L' ';
                else if(CurrentText.at(i) == L'\r')
                {
                    CurrentText.erase(1, i);
                    i--;
                }
            }
        }
        //Otherwise find out how many lines and length and remove all \r
        else
        {
            for(int i = 0; i < CurrentText.size(); i++)
            {
                if(CurrentText.at(i) == L'\n')
                    numberOfLines++;
                else if(CurrentText.at(i) == L'\r')
                {
                    CurrentText.erase(1, i);
                    i--;
                }
            }
        }

        //Populate CharactersPosition list and CharactersInfos list
        CharactersPosition.clear();
        CharactersInfos.clear();
        for(int i = 0; i < CurrentText.size(); i++)
        {
            CharactersPosition.push_back(glm::ivec2());
            CharactersInfos.push_back(ssGUI::CharacterInfo());
        }

        ssGUI::Backend::BackendFontInterface* fontInterface = CurrentFont->GetBackendFontInterface();
        int whitespaceWidth = fontInterface->GetCharacterInfo(L' ', FontSize).Advance + GetCharacterSpace();
        
        //If word wrapping mode, find out the lengths of all the words
        if(WrappingMode == ssGUI::Enums::TextWrapping::WORD_WRAPPING)
        {
            int currentWordIndex = 0;
            int currentLineLength = 0;
            int currentWordLength = 0;
            glm::ivec2 currentDrawPos = glm::ivec2();

            wchar_t prevChar = 0;

            for (std::size_t i = 0; i < CurrentText.size(); i++)
            {
                wchar_t curChar = CurrentText.at(i);

                // Apply the kerning offset
                currentDrawPos.x += fontInterface->GetKerning(prevChar, curChar, FontSize) /* GetCharacterSpace()*/;

                prevChar = curChar;
                
                //See if the current word exceeds widget width when a word finishes
                if ((curChar == L' ') || (curChar == L'\n') || (curChar == L'\t') || i == CurrentText.size() - 1)
                {
                    //last character
                    if(i == CurrentText.size() - 1)
                    {
                        ssGUI::CharacterInfo info = fontInterface->GetCharacterInfo(curChar, FontSize);
                        int characterLength = info.Advance;

                        CharactersPosition[i] = currentDrawPos;
                        CharactersInfos[i] = info;

                        currentWordLength += characterLength + GetCharacterSpace();
                    }
                    
                    prevChar = 0;
                    
                    //Check if word width is wider than widget width by itself, it will be on its own line
                    if(currentWordLength > GetSize().x && currentLineLength == 0)
                    {
                        currentWordIndex = i + 1;
                        currentWordLength = 0;
                        CharactersPosition[i] = currentDrawPos;
                        
                        currentDrawPos.y += fontInterface->GetLineSpacing(FontSize) + GetLineSpace(); 
                        currentDrawPos.x = 0;
                    }
                    //Otherwise check if adding current word length to current line length exceeds widget width
                    //If so, reset i and current line length and current word length and go to next line
                    else if(currentWordLength + currentLineLength > GetSize().x)
                    {
                        i = currentWordIndex - 1;
                        currentLineLength = 0;
                        currentWordLength = 0;

                        currentDrawPos.y += fontInterface->GetLineSpacing(FontSize) + GetLineSpace();
                        currentDrawPos.x = 0;
                    }
                    //Otherwise, set word index to next character and reset current word length
                    else 
                    {
                        currentWordIndex = i + 1;
                        currentLineLength += currentWordLength;
                        currentWordLength = 0;
                        CharactersPosition[i] = currentDrawPos;

                        switch (curChar)
                        {
                            case L' ':  
                                currentDrawPos.x += whitespaceWidth;
                                currentLineLength += whitespaceWidth;
                                CharactersInfos[i].Advance = whitespaceWidth; 
                                break;
                            case L'\t': 
                                currentDrawPos.x += whitespaceWidth * TabSize;
                                currentLineLength += whitespaceWidth * TabSize;
                                CharactersInfos[i].Advance = whitespaceWidth * TabSize; 
                                break;
                            case L'\n': 
                                currentDrawPos.y += fontInterface->GetLineSpacing(FontSize) + GetLineSpace(); 
                                currentDrawPos.x = 0;
                                currentLineLength = 0;
                                break;
                        }
                    }
                }
                //Otherwise, add the current character
                else
                {
                    ssGUI::CharacterInfo info = fontInterface->GetCharacterInfo(curChar, FontSize);
                    int characterLength = info.Advance;

                    CharactersPosition[i] = currentDrawPos;
                    CharactersInfos[i] = info;

                    currentWordLength += characterLength + GetCharacterSpace();
                    currentDrawPos.x += characterLength + GetCharacterSpace();
                }
            }

            WrappingOverflow = currentDrawPos.y + fontInterface->GetLineSpacing(FontSize) + GetLineSpace() > GetSize().y ? true : false;
        }
        //Otherwise wrap until it reaches the end of widget
        else if(WrappingMode == ssGUI::Enums::TextWrapping::CHARACTER_WRAPPING)
        {
            int currentLineLength = 0;
            glm::ivec2 currentDrawPos = glm::ivec2();

            wchar_t prevChar = 0;

            for (std::size_t i = 0; i < CurrentText.size(); i++)
            {
                wchar_t curChar = CurrentText.at(i);
                
                // Apply the kerning offset
                currentDrawPos.x += fontInterface->GetKerning(prevChar, curChar, FontSize) /* GetCharacterSpace()*/;

                prevChar = curChar;

                //If space or tab or newline character, just append
                if (curChar == L' ' || curChar == L'\n' || curChar == L'\t')
                {
                    CharactersPosition[i] = currentDrawPos;
                    
                    switch (curChar)
                    {
                        case L' ':  
                            currentDrawPos.x += whitespaceWidth;
                            currentLineLength += whitespaceWidth;
                            CharactersInfos[i].Advance = whitespaceWidth;    
                            break;
                        case L'\t': 
                            currentDrawPos.x += whitespaceWidth * GetTabSize();
                            currentLineLength += whitespaceWidth * GetTabSize();
                            CharactersInfos[i].Advance = whitespaceWidth * GetTabSize(); 
                            break;
                        case L'\n': 
                            currentDrawPos.y += fontInterface->GetLineSpacing(FontSize) + GetLineSpace(); 
                            currentDrawPos.x = 0;
                            currentLineLength = 0;
                            break;
                    }
                }
                else 
                {
                    ssGUI::CharacterInfo info = fontInterface->GetCharacterInfo(curChar, FontSize);
                    int characterLength = info.Advance + GetCharacterSpace();

                    //If one character is taking up the whole line, reset line length and go to next line
                    if(characterLength > GetSize().x && currentLineLength == 0)
                    {
                        CharactersPosition[i] = currentDrawPos;
                        CharactersInfos[i] = info;

                        currentLineLength = 0;
                        currentDrawPos.y += fontInterface->GetLineSpacing(FontSize) + GetLineSpace();
                        currentDrawPos.x = 0;
                    }
                    //If exceed widget width, reset i and line length and go to next line
                    else if(currentLineLength + characterLength > GetSize().x)
                    {
                        currentLineLength = 0;
                        currentDrawPos.y += fontInterface->GetLineSpacing(FontSize) + GetLineSpace();
                        currentDrawPos.x = 0;
                        i--;
                    }
                    else
                    {
                        CharactersPosition[i] = currentDrawPos;
                        CharactersInfos[i] = info;
                        currentLineLength += characterLength;
                        currentDrawPos.x += characterLength;
                    }
                }
            }

            WrappingOverflow = currentDrawPos.y + fontInterface->GetLineSpacing(FontSize) + GetLineSpace() > GetSize().y ? true : false;
        }
        //Otherwise just position text normally
        else
        {
            glm::ivec2 currentDrawPos = glm::ivec2();

            wchar_t prevChar = 0;

            for (std::size_t i = 0; i < CurrentText.size(); i++)
            {
                wchar_t curChar = CurrentText.at(i);
                
                // Apply the kerning offset
                currentDrawPos.x += fontInterface->GetKerning(prevChar, curChar, FontSize) /* GetCharacterSpace()*/;

                prevChar = curChar;

                //If space or tab or newline character, just append
                if (curChar == L' ' || curChar == L'\n' || curChar == L'\t')
                {
                    CharactersPosition[i] = currentDrawPos;
                    
                    switch (curChar)
                    {
                        case L' ':  
                            currentDrawPos.x += whitespaceWidth;
                            CharactersInfos[i].Advance = whitespaceWidth; 
                            break;
                        case L'\t': 
                            currentDrawPos.x += whitespaceWidth * GetTabSize();
                            CharactersInfos[i].Advance = whitespaceWidth * GetTabSize(); 
                            break;
                        case L'\n': 
                            currentDrawPos.y += fontInterface->GetLineSpacing(FontSize) + GetLineSpace(); 
                            currentDrawPos.x = 0;
                            break;
                    }
                }
                else 
                {
                    ssGUI::CharacterInfo info = fontInterface->GetCharacterInfo(curChar, FontSize);
                    int characterLength = info.Advance;

                    CharactersPosition[i] = currentDrawPos;
                    CharactersInfos[i] = info;
                    currentDrawPos.x += characterLength + GetCharacterSpace();

                    if(currentDrawPos.x > GetSize().x)
                        WrappingOverflow = true;                    
                }
            }
        }
    }
    

    void Text::SetText(std::wstring text)
    {
        CurrentTextChanged = true;
        CurrentText = text;
    }

    std::wstring Text::GetText() const
    {
        return CurrentText;
    }

    int Text::GetCharacterCount() const
    {
        return CurrentText.size();
    }

    glm::ivec2 Text::GetCharacterGlobalPosition(int index) const
    {
        return CharactersPosition[index];
    }

    ssGUI::CharacterInfo Text::GetCharacterInfo(int index) const
    {
        return CharactersInfos[index];
    }

    bool Text::IsWrappingOverflow() const
    {
        return WrappingOverflow;
    }

    void Text::SetFontSize(int size)
    {
        FontSize = size;
    }

    int Text::GetFontSize() const
    {
        return FontSize;
    }

    void Text::SetMultilineAllowed(bool multiline)
    {
        MultilineAllowed = multiline;
    }

    bool Text::IsMultilineAllowed() const
    {
        return MultilineAllowed;
    }   

    void Text::SetWrappingMode(ssGUI::Enums::TextWrapping wrappingMode)
    {
        WrappingMode = wrappingMode;
    }

    ssGUI::Enums::TextWrapping Text::GetWrappingMode() const
    {
        return WrappingMode;
    }

    void Text::SetFont(ssGUI::Font* font)
    {
        CurrentFont = font;
    }

    ssGUI::Font* Text::GetFont()
    {
        return CurrentFont;
    }

    void Text::SetCharacterSpace(int charSpace)
    {
        CharacterSpace = charSpace;
    }

    int Text::GetCharacterSpace() const
    {
        return CharacterSpace;
    }

    void Text::SetLineSpace(int lineSpace)
    {
        LineSpace = lineSpace;
    }

    int Text::GetLineSpace() const
    {
        return LineSpace;
    }

    void Text::SetTabSize(float tabSize)
    {
        TabSize = tabSize;
    }
    
    float Text::GetTabSize() const
    {
        return TabSize;
    }

    ssGUI::Enums::GUIObjectType Text::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::BASE_WIDGET | ssGUI::Enums::GUIObjectType::TEXT;
    }

    void Text::Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {
        if(!IsVisible())
            return;
        
        for(auto extension : Extensions)
            extension->Draw(true, drawingInterface, mainWindowP, mainWindowPositionOffset);
        
        glm::ivec2 drawPos = GetGlobalPosition() - mainWindowP->GetGlobalPosition() - mainWindowPositionOffset;

        //TODO: Some optimisation maybe possible
        DrawingVerticies.push_back(drawPos);
        DrawingUVs.push_back(glm::ivec2());
        DrawingColours.push_back(GetBackgroundColour());

        DrawingVerticies.push_back(drawPos + glm::ivec2(GetSize().x, 0));
        DrawingUVs.push_back(glm::ivec2());
        DrawingColours.push_back(GetBackgroundColour());

        DrawingVerticies.push_back(drawPos + glm::ivec2(GetSize().x, GetSize().y));
        DrawingUVs.push_back(glm::ivec2());
        DrawingColours.push_back(GetBackgroundColour());

        DrawingVerticies.push_back(drawPos + glm::ivec2(0, GetSize().y));
        DrawingUVs.push_back(glm::ivec2());
        DrawingColours.push_back(GetBackgroundColour());

        DrawingCounts.push_back(4);
        DrawingProperties.push_back(ssGUI::DrawingProperty());

        if(CurrentFont == nullptr)
            goto endOfDrawing;

        {
        if(CurrentTextChanged)
            ComputeCharactersPositionAndSize();
        
        ssGUI::Backend::BackendFontInterface* fontInterface = CurrentFont->GetBackendFontInterface();

        for (std::size_t i = 0; i < CurrentText.size(); i++)
        {
            wchar_t curChar = CurrentText.at(i);

            if ((curChar == L' ') || (curChar == L'\n') || (curChar == L'\t'))
                continue;

            // Add the glyph to the vertices
            DrawCharacter(drawingInterface, mainWindowP, mainWindowPositionOffset, curChar, drawPos + CharactersPosition[i], CharactersInfos[i]);
        }

        //Border
        //DrawBorder(drawingInterface, mainWindowP, mainWindowPositionOffset);
        }

        endOfDrawing:;

        for(auto extension : Extensions)
            extension->Draw(false, drawingInterface, mainWindowP, mainWindowPositionOffset);

        drawingInterface->DrawEntities(DrawingVerticies, DrawingUVs, DrawingColours, DrawingCounts, DrawingProperties);
        DrawingVerticies.clear();
        DrawingUVs.clear();
        DrawingColours.clear();
        DrawingCounts.clear();
        DrawingProperties.clear();
    }

    GUIObject* Text::Clone(std::vector<GUIObject*>& originalObjs, bool cloneChildren)
    {
        Text* temp = new Text(*this);

        for(auto extension : Extensions)
        {
            if(!temp->IsExtensionExist(extension->GetExtensionName()))
                temp->AddExtension(extension->Clone(this));
        }

        for(auto eventCallback : EventCallbacks)
        {
            std::vector<ssGUI::GUIObject*> tempVec = std::vector<ssGUI::GUIObject*>();

            if(!temp->IsEventCallbackExist(eventCallback->GetEventCallbackName()))
                temp->AddEventCallback(eventCallback->Clone(this, originalObjs, tempVec));
        }

        return temp;
    }

}