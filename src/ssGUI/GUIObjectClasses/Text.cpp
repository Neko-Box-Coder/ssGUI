#include "ssGUI/GUIObjectClasses/Text.hpp"
#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

#include "glm/gtx/norm.hpp"
#include <cmath>
#include <locale>
#include <codecvt>

namespace ssGUI
{    
    //TODO: Encapsulate it in class for deallocation
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
    
    Text::Text(Text const& other) : Widget(other)
    {
        RecalculateTextNeeded = true;
        CurrentCharactersDetails = other.CurrentCharactersDetails;
        CharactersRenderInfos = other.CharactersRenderInfos;
        ProcessedCharacterDetails = other.ProcessedCharacterDetails;
        Overflow = other.Overflow;
        FontSize = other.GetNewCharacterFontSize();
        TextColor = other.GetNewCharacterColor();
        TextUnderline = other.IsNewCharacterUnderlined();
        MultilineAllowed = other.IsMultilineAllowed();
        WrappingMode = other.GetWrappingMode();
        HorizontalAlignment = other.GetHorizontalAlignment();
        VerticalAlignment = other.GetVerticalAlignment();
        CurrentFonts = other.CurrentFonts;
        HorizontalPadding = other.GetHorizontalPadding();
        VerticalPadding = other.GetVerticalPadding();
        CharacterSpace = other.GetCharacterSpace();
        LineSpace = other.GetLineSpace();
        TabSize = other.GetTabSize();
        SelectionAllowed = other.IsTextSelectionAllowed();
        StartSelectionIndex = other.GetStartSelectionIndex();
        EndSelectionIndex = other.GetEndSelectionIndex();
        SelectionColor = other.GetSelectionColor();
        TextSelectedColor = other.GetTextSelectedColor();
        LastDefaultFonts = other.LastDefaultFonts;
    }

    ssGUI::CharacterDetails& Text::GetInternalCharacterDetail(int index)
    {
        return ProcessedCharacterDetails.find(index) == ProcessedCharacterDetails.end() ?
            CurrentCharactersDetails[index] : ProcessedCharacterDetails[index];
    }

    void Text::ConstructCharacterDetails(std::wstring& s, std::vector<ssGUI::CharacterDetails>& details)
    {
        for(int i = 0; i < s.size(); i++)
        {
            ssGUI::CharacterDetails detail;
            detail.Character = s[i];
            detail.FontSize = GetNewCharacterFontSize();
            detail.CharacterColor = GetNewCharacterColor();
            detail.Underlined = IsNewCharacterUnderlined();
            details.push_back(detail);
        }
    }

    void Text::AssignSupportedFont()
    {
        FUNC_DEBUG_ENTRY();
        
        for(int i = 0; i < CurrentCharactersDetails.Size(); i++)
        {
            const ssGUI::CharacterDetails& detail = GetInternalCharacterDetail(i);

            ssGUI::CharacterDetails newDetail = detail;
            bool changed = false;
            
            //If null character, we don't care about the font as it will be marked as not rendered
            if(detail.Character == L'\0')
                continue;
            
            //Assign font index if nothing is assigned
            if(detail.FontIndex == -1 && detail.DefaultFontIndex == -1)
            {
                changed = true;
                if(GetFontsCount() > 0)
                    newDetail.FontIndex = 0;
                else
                    newDetail.DefaultFontIndex = 0;
            }
            
            //Just move on if it is newline, tab or space as it will be marked as not rendered
            if(detail.Character != L'\n' && detail.Character != L'\t' && detail.Character != L' ')
            {
                //Check each font if character is supported. If not, fallback to default fonts
                bool fallbackToDefault = false;
                if(newDetail.FontIndex != -1)
                {
                    if(GetFontsCount() == 0 || newDetail.FontIndex >= GetFontsCount())
                    {
                        newDetail.FontIndex = -1;
                        changed = true;
                        fallbackToDefault = true;
                    }
                    else
                    {
                        while (!GetFont(newDetail.FontIndex)->IsCharacterSupported(newDetail.Character))
                        {
                            if(newDetail.FontIndex == GetFontsCount() -1)
                            {
                                newDetail.FontIndex = -1;
                                break;
                            }

                            newDetail.FontIndex++;
                            changed = true;
                        }
                    }
                }
                
                //Default fonts
                if(fallbackToDefault || (newDetail.FontIndex == -1 && newDetail.DefaultFontIndex != -1))
                {
                    if(GetDefaultFontsCount() == 0)
                    {
                        newDetail.DefaultFontIndex = -1;
                        changed = true;
                    }
                    else
                    {
                        while (!GetDefaultFont(newDetail.DefaultFontIndex)->IsCharacterSupported(newDetail.Character))
                        {
                            if(newDetail.DefaultFontIndex == GetDefaultFontsCount() -1)
                            {
                                newDetail.DefaultFontIndex = -1;
                                break;
                            }

                            newDetail.DefaultFontIndex++;
                            changed = true;
                        }
                    }
                }
            }

            //If no supported fonts are found just assign the default font
            if(newDetail.FontIndex == -1 && newDetail.DefaultFontIndex == -1)
            {
                if(GetFontsCount() > 0)
                    newDetail.FontIndex = 0;
                else if(GetDefaultFontsCount() > 0)
                    newDetail.DefaultFontIndex = 0;
            }

            //Check detail is changed, if so record it.
            if(changed)
                ProcessedCharacterDetails[i] = newDetail;
        }

        FUNC_DEBUG_EXIT();
    }

    void Text::DrawCharacter(glm::vec2 positionOffset, ssGUI::CharacterRenderInfo info, ssGUI::CharacterDetails details)
    {
        if(!info.Rendered)
            return;

        glm::vec2 position = positionOffset + info.RenderPosition;
        ssGUI::Font* targetFont = nullptr;
        
        if(details.FontIndex != -1)
            targetFont = GetFont(details.FontIndex);
        else if(details.DefaultFontIndex != -1)
            targetFont = GetDefaultFont(details.DefaultFontIndex);

        if(targetFont == nullptr)
            return;
        
        DrawingVerticies.push_back(position                                 + info.DrawOffset);
        DrawingVerticies.push_back(position + glm::vec2(info.Size.x, 0)     + info.DrawOffset);
        DrawingVerticies.push_back(position + info.Size                     + info.DrawOffset);
        DrawingVerticies.push_back(position + glm::vec2(0, info.Size.y)     + info.DrawOffset);

        DrawingColours.push_back(details.CharacterColor);
        DrawingColours.push_back(details.CharacterColor);
        DrawingColours.push_back(details.CharacterColor);
        DrawingColours.push_back(details.CharacterColor);

        DrawingUVs.push_back(info.UVOrigin);
        DrawingUVs.push_back(info.UVOrigin + glm::vec2(info.Size.x, 0));
        DrawingUVs.push_back(info.UVOrigin + info.Size);
        DrawingUVs.push_back(info.UVOrigin + glm::vec2(0, info.Size.y));

        DrawingCounts.push_back(4);
        ssGUI::DrawingProperty currentProperty;
        currentProperty.fontP = targetFont->GetBackendFontInterface();
        currentProperty.characterSize = details.FontSize;

        DrawingProperties.push_back(currentProperty);
    }

    void Text::FormatNewlinesCharacters()
    {
        FUNC_DEBUG_ENTRY();

        //If multiline is disabled, replace all newline to spaces and remove all \r
        if(!MultilineAllowed)
        {
            for(int i = 0; i < CurrentCharactersDetails.Size(); i++)
            {
                if(CurrentCharactersDetails.At(i).Character == L'\n')
                {
                    ProcessedCharacterDetails[i] = CurrentCharactersDetails[i];
                    ProcessedCharacterDetails[i].Character = L' ';
                }
                else if(CurrentCharactersDetails.At(i).Character == L'\r')
                {
                    ProcessedCharacterDetails[i] = CurrentCharactersDetails[i];
                    ProcessedCharacterDetails[i].Character = L'\0';
                }
            }
        }
        //Otherwise find out how many lines and length and remove all \r
        else
        {
            for(int i = 0; i < CurrentCharactersDetails.Size(); i++)
            {
                if(CurrentCharactersDetails.At(i).Character == L'\r')
                {
                    ProcessedCharacterDetails[i] = CurrentCharactersDetails[i];
                    ProcessedCharacterDetails[i].Character = L'\0';
                }
            }
        }

        FUNC_DEBUG_EXIT();
    }

    float Text::GetClosestTabSpace(float startPos, float tabSpace, float targetPos)
    {
        float tabAmount = (targetPos - startPos) / tabSpace;

        //If at tab
        //https://stackoverflow.com/questions/5796983/checking-if-float-is-an-integer#answer-25274904
        if(fabs(round(tabAmount) - tabAmount) <= 0.00001f)
            return startPos + (round(tabAmount) + 1) * tabSpace;
        else
            return startPos + ceil(tabAmount) * tabSpace;
    }

    void Text::ConstructRenderInfosForWordWrapping()
    {
        FUNC_DEBUG_ENTRY();
        
        int currentWordIndex = 0;
        float currentLineLength = GetHorizontalPadding();
        float currentWordLength = 0;
        float drawXOffset = 0;

        int lastValidIndex = -1;

        //First pass, check which characters are valid. Also record the last valid character index
        for(int i = 0; i < CurrentCharactersDetails.Size(); i++)
        {
            ssGUI::CharacterDetails& curDetail = GetInternalCharacterDetail(i);
            wchar_t curChar = curDetail.Character;

            //If this character is null or fonts are not available, skip
            if(curChar == L'\0' || (curDetail.FontIndex == -1 && curDetail.DefaultFontIndex == -1))
                continue;
            
            CharactersRenderInfos[i].Valid = true;
            lastValidIndex = i;
        }

        //Second pass, Construct render infos as no character wrapping
        ConstructRenderInfosForNoWrapping(false);
        Overflow = false;

        //Third pass, reposition the characters as word wrapping
        for (int i = 0; i < CurrentCharactersDetails.Size(); i++)
        {
            if(!CharactersRenderInfos[i].Valid)
            {
                CharactersRenderInfos[i].RenderPosition.x += drawXOffset;
                continue;
            }

            ssGUI::CharacterDetails& curDetail = GetInternalCharacterDetail(i);
            ssGUI::CharacterRenderInfo& curRenderInfo = CharactersRenderInfos[i];
            wchar_t curChar = curDetail.Character;

            //Check for newline. If so, reset word and line settings
            if(curRenderInfo.CharacterAtNewline)
            {
                currentWordIndex = i;
                currentWordLength = 0;
                currentLineLength = GetHorizontalPadding();
                drawXOffset = 0;
            }

            currentWordLength += curRenderInfo.Advance + GetCharacterSpace();

            //See if the current word exceeds widget width when a word *finishes*
            if ((curChar == L' ') || (curChar == L'\n') || (curChar == L'\t') || i == lastValidIndex)
            {
                //check if adding current word length to current line length exceeds widget width
                if(currentWordLength - GetCharacterSpace() + currentLineLength + GetHorizontalPadding() > GetSize().x)
                {
                    //If the word is already at newline, set overflow
                    if(CharactersRenderInfos[currentWordIndex].CharacterAtNewline)
                        Overflow = true;
                    //Otherwise, reposition the word to the newlne
                    else
                    {
                        float curWordXOffset = GetHorizontalPadding() - CharactersRenderInfos[currentWordIndex].RenderPosition.x;
                        CharactersRenderInfos[currentWordIndex].CharacterAtNewline = true;
                        currentLineLength = GetHorizontalPadding();

                        for(int j = currentWordIndex; j < i; j++)
                            CharactersRenderInfos[j].RenderPosition.x += curWordXOffset;
                        
                        //Needs to total up the offset instead of assigning it.
                        drawXOffset += curWordXOffset;
                    }
                }

                //Reset word settings
                currentLineLength += currentWordLength;
                currentWordIndex = i + 1;
                currentWordLength = 0;
            }

            //Apply offset
            curRenderInfo.RenderPosition.x += drawXOffset;
        }

        FUNC_DEBUG_EXIT();
    }

    void Text::ConstructRenderInfosForCharacterWrapping()
    {
        FUNC_DEBUG_ENTRY();

        float currentLineLength = GetHorizontalPadding();
        float drawXOffset = 0;

        //First pass, Construct render infos as no character wrapping
        ConstructRenderInfosForNoWrapping(true);
        Overflow = false;

        //Second pass, reposition the characters as character wrapping
        for (int i = 0; i < CurrentCharactersDetails.Size(); i++)
        {
            if(!CharactersRenderInfos[i].Valid)
            {
                CharactersRenderInfos[i].RenderPosition.x += drawXOffset;
                continue;
            }

            ssGUI::CharacterDetails& curDetail = GetInternalCharacterDetail(i);
            ssGUI::CharacterRenderInfo& curRenderInfo = CharactersRenderInfos[i];
            float characterLength = curRenderInfo.Advance + GetCharacterSpace();

            //Check for newline. If so, reset word and line settings
            if(curRenderInfo.CharacterAtNewline)
            {
                currentLineLength = GetHorizontalPadding();
                drawXOffset = 0;
            }

            //If exceed widget width
            if(currentLineLength + characterLength + GetHorizontalPadding() > GetSize().x)
            {
                //If this character is already at newline, set overflow
                if(curRenderInfo.CharacterAtNewline)
                    Overflow = true;
                //Otherwise, move character to newline
                else
                {
                    drawXOffset = GetHorizontalPadding() - curRenderInfo.RenderPosition.x;
                    curRenderInfo.CharacterAtNewline = true;
                    currentLineLength = GetHorizontalPadding();
                }
            }
            currentLineLength += characterLength;

            //Apply offset
            curRenderInfo.RenderPosition.x += drawXOffset;
        }

        FUNC_DEBUG_EXIT();
    }

    void Text::ConstructRenderInfosForNoWrapping(bool checkValid)
    {
        FUNC_DEBUG_ENTRY();
        float drawXPos = GetHorizontalPadding();

        wchar_t prevChar = 0;
        Overflow = false;
        bool nextCharIsAtNewline = true;

        for (int i = 0; i < CurrentCharactersDetails.Size(); i++)
        {
            ssGUI::CharacterDetails& curDetail = GetInternalCharacterDetail(i);
            ssGUI::CharacterRenderInfo& curRenderInfo = CharactersRenderInfos[i];

            wchar_t curChar = curDetail.Character;

            if(checkValid)
            {
                //If this character is null or fonts are not available, skip
                if(curChar == L'\0' || (curDetail.FontIndex == -1 && curDetail.DefaultFontIndex == -1))
                {
                    prevChar = 0;
                    curRenderInfo.RenderPosition = glm::vec2(drawXPos, 0);
                    continue;
                }
                else
                    curRenderInfo.Valid = true;
            }
            //Skip if not valid
            else if(!curRenderInfo.Valid)
            {
                prevChar = 0;
                curRenderInfo.RenderPosition = glm::vec2(drawXPos, 0);
                continue;
            }
            
            ssGUI::Backend::BackendFontInterface* fontInterface = nullptr;
            
            if(curDetail.FontIndex != -1)
                fontInterface = GetFont(curDetail.FontIndex)->GetBackendFontInterface();
            else
                fontInterface = GetDefaultFont(curDetail.DefaultFontIndex)->GetBackendFontInterface();
            
            // Apply the kerning offset
            drawXPos += fontInterface->GetKerning(prevChar, curChar, curDetail.FontSize);

            prevChar = curChar;

            //If space or tab or newline character, just append
            if (curChar == L' ' || curChar == L'\n' || curChar == L'\t')
            {
                //Check newline
                if(nextCharIsAtNewline)
                {
                    drawXPos = GetHorizontalPadding();
                    curRenderInfo.CharacterAtNewline = true;
                    nextCharIsAtNewline = false;
                }
                
                curRenderInfo.RenderPosition = glm::vec2(drawXPos, 0);
                float whitespaceWidth = fontInterface->GetCharacterRenderInfo(L' ', curDetail.FontSize).Advance + GetCharacterSpace();

                switch (curChar)
                {
                    case L' ':  
                        drawXPos += whitespaceWidth;
                        CharactersRenderInfos[i].Advance = whitespaceWidth; 
                        break;
                    case L'\t': 
                    {
                        float newPos = GetClosestTabSpace(GetHorizontalPadding(), whitespaceWidth * GetTabSize(), drawXPos);
                        float actualTabSpace = newPos - drawXPos;
                        drawXPos += actualTabSpace;
                        CharactersRenderInfos[i].Advance = actualTabSpace;
                        break;
                    }
                    case L'\n': 
                        nextCharIsAtNewline = true;
                        break;
                }

                if(drawXPos + GetHorizontalPadding() > GetSize().x)
                    Overflow = true;   
            }
            else 
            {
                ssGUI::CharacterRenderInfo info = fontInterface->GetCharacterRenderInfo(curChar, curDetail.FontSize);
                float characterLength = info.Advance;
                bool oriValid = curRenderInfo.Valid;
                curRenderInfo = info;
                curRenderInfo.Valid = oriValid;

                //Check newline
                if(nextCharIsAtNewline)
                {
                    drawXPos = GetHorizontalPadding();
                    curRenderInfo.CharacterAtNewline = true;
                    nextCharIsAtNewline = false;
                }

                curRenderInfo.RenderPosition = glm::vec2(drawXPos, 0);

                drawXPos += characterLength + GetCharacterSpace();

                if(drawXPos + GetHorizontalPadding() > GetSize().x)
                    Overflow = true;                    
            }
        }

        FUNC_DEBUG_EXIT();
    }

    void Text::ApplyFontLineSpacing()
    {
        FUNC_DEBUG_ENTRY();
        if(CharactersRenderInfos.empty())
        {
            FUNC_DEBUG_EXIT();
            return;
        }
        
        float curMaxFontNewline = 0;
        int currentIndex = 0;
        int currentLineIndex = 0;
        float currentOffset = 0;
        int lineCount = 0;
        int lastValidIndex = GetLastValidCharacterIndex();
        ssGUI::Backend::BackendFontInterface* backendFont = nullptr;

        while (currentIndex < CharactersRenderInfos.size())
        {
            const ssGUI::CharacterDetails& curDetail = GetInternalCharacterDetail(currentIndex);

            //Record max font newline height if needed
            if(!CharactersRenderInfos[currentIndex].Valid)
            {
                currentIndex++;
                continue;
            }
            else if(curDetail.FontIndex != -1)
                backendFont = GetFont(curDetail.FontIndex)->GetBackendFontInterface();
            else
                backendFont = GetDefaultFont(curDetail.DefaultFontIndex)->GetBackendFontInterface();

            //When there's a newline, offset the current line
            if(CharactersRenderInfos[currentIndex].CharacterAtNewline)
            {
                //First line doesn't use font line space
                if(lineCount == 1)
                {
                    currentOffset += curMaxFontNewline;
                    
                    for(int i = currentLineIndex; i < currentIndex; i++)
                    {
                        CharactersRenderInfos[i].RenderPosition.y += currentOffset;
                        CharactersRenderInfos[i].LineMinY = -curMaxFontNewline;
                        CharactersRenderInfos[i].LineMaxY = -curMaxFontNewline + backendFont->GetLineSpacing(curMaxFontNewline) + GetLineSpace();
                    }

                    curMaxFontNewline = 0;
                    currentLineIndex = currentIndex;
                    lineCount++;
                }
                //Second line and beyond uses font line space
                else if(lineCount >= 2)
                {
                    currentOffset += backendFont->GetLineSpacing(curMaxFontNewline) + GetLineSpace();
                    
                    for(int i = currentLineIndex; i < currentIndex; i++)
                    {
                        CharactersRenderInfos[i].RenderPosition.y += currentOffset;
                        CharactersRenderInfos[i].LineMinY = -curMaxFontNewline;//-backendFont->GetLineSpacing(curMaxFontNewline) - GetLineSpace();
                        CharactersRenderInfos[i].LineMaxY = -curMaxFontNewline + backendFont->GetLineSpacing(curMaxFontNewline) + GetLineSpace();
                    }

                    curMaxFontNewline = 0;
                    currentLineIndex = currentIndex;
                }
                //First character is always new line, ignore it
                else
                {
                    curMaxFontNewline = 0;
                    currentLineIndex = currentIndex;
                    lineCount++;
                }
            }

            if(curDetail.FontSize > curMaxFontNewline)
                curMaxFontNewline = curDetail.FontSize;
            
            //If this is the last character, offset the current line
            if(currentIndex == lastValidIndex && backendFont != nullptr)
            {
                currentOffset += lineCount == 1 ? curMaxFontNewline : backendFont->GetLineSpacing(curMaxFontNewline) + GetLineSpace();
                
                for(int i = currentLineIndex; i <= currentIndex; i++)
                {
                    CharactersRenderInfos[i].RenderPosition.y += currentOffset;
                    CharactersRenderInfos[i].LineMinY = -curMaxFontNewline;
                    CharactersRenderInfos[i].LineMaxY = -curMaxFontNewline + backendFont->GetLineSpacing(curMaxFontNewline) + GetLineSpace();
                }

                //Update vertical overflow
                if(!IsOverflow() && CharactersRenderInfos[currentIndex].RenderPosition.y + GetVerticalPadding() * 2 > GetSize().y)
                    Overflow = true;

                break;
            }
            
            currentIndex++;
        }

        FUNC_DEBUG_EXIT();
    }

    void Text::ApplyTextAlignment()
    {
        FUNC_DEBUG_ENTRY();
        
        if(CharactersRenderInfos.empty())
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        //Alignment is fine without checking whether the iterating character is valid or not since the render position would be correct
        
        //Text alignment
        float lineStartPos = 0;
        int lineStartIndex = 0;
        float lineEndPos = lineStartPos;
        float currentLineHeight = -1;

        //For each line, find out how long it is and align accordingly
        for(int i = 0; i < CharactersRenderInfos.size(); i++)
        {
            //If this character is on a newline
            if(CharactersRenderInfos[i].RenderPosition.y != currentLineHeight)
            {
                //Align the previous line first
                if(i > 0)
                {
                    lineEndPos = CharactersRenderInfos[i-1].RenderPosition.x + CharactersRenderInfos[i-1].DrawOffset.x +
                        CharactersRenderInfos[i-1].Size.x + GetHorizontalPadding();
                    float alignOffset = 0; 

                    switch (HorizontalAlignment)
                    {
                        case ssGUI::Enums::TextAlignmentHorizontal::LEFT:
                            break;
                    
                        case ssGUI::Enums::TextAlignmentHorizontal::CENTER:
                            alignOffset = (GetSize().x * 0.5 - lineStartPos) - (lineEndPos - lineStartPos) * 0.5;
                            break;
                        
                        case ssGUI::Enums::TextAlignmentHorizontal::RIGHT:
                            alignOffset = GetSize().x - lineEndPos;
                            break;
                    }

                    for(int j = lineStartIndex; j < i; j++)
                        CharactersRenderInfos[j].RenderPosition.x += alignOffset;
                }

                //Then record where the newline starts
                lineStartPos = 0;
                currentLineHeight = CharactersRenderInfos[i].RenderPosition.y;
                lineStartIndex = i;
            }
            
            //End of character
            if(i == CharactersRenderInfos.size() - 1)
            {
                lineEndPos = CharactersRenderInfos[i].RenderPosition.x + CharactersRenderInfos[i].DrawOffset.x +
                        CharactersRenderInfos[i].Size.x + GetHorizontalPadding();
                float alignOffset = 0; 

                switch (HorizontalAlignment)
                {
                    case ssGUI::Enums::TextAlignmentHorizontal::LEFT:
                        break;
                
                    case ssGUI::Enums::TextAlignmentHorizontal::CENTER:
                        alignOffset = (GetSize().x * 0.5 - lineStartPos) - (lineEndPos - lineStartPos) * 0.5;
                        break;
                    
                    case ssGUI::Enums::TextAlignmentHorizontal::RIGHT:
                        alignOffset = GetSize().x - lineEndPos;
                        break;
                }

                for(int j = lineStartIndex; j <= i; j++)
                    CharactersRenderInfos[j].RenderPosition.x += alignOffset;
            }
        }
        
        //Find out how tall all the texts are and align accordingly
        float alignOffset = 0;
        lineStartPos = 0;
        
        for(int i = 0; i < CharactersRenderInfos.size(); i++)
        {
            if(CharactersRenderInfos[i].RenderPosition.y + CharactersRenderInfos[i].DrawOffset.y < lineStartPos)
                lineStartPos = CharactersRenderInfos[i].RenderPosition.y + CharactersRenderInfos[i].DrawOffset.y;
        }

        lineEndPos = CharactersRenderInfos[CharactersRenderInfos.size() - 1].RenderPosition.y + GetVerticalPadding();
        switch (VerticalAlignment)
        {
            case ssGUI::Enums::TextAlignmentVertical::TOP:
                alignOffset = GetVerticalPadding();
                break;
        
            case ssGUI::Enums::TextAlignmentVertical::CENTER:
                alignOffset = (GetSize().y * 0.5 - lineStartPos) - (lineEndPos - lineStartPos) * 0.5;
                break;
            
            case ssGUI::Enums::TextAlignmentVertical::BOTTOM:
                alignOffset = GetSize().y - lineEndPos;
                break;
        }

        for(int i = 0; i < CharactersRenderInfos.size(); i++)
            CharactersRenderInfos[i].RenderPosition.y += alignOffset;

        FUNC_DEBUG_EXIT();
    }
    
    void Text::ApplyTextHighlight()
    {
        FUNC_DEBUG_ENTRY();

        if(StartSelectionIndex == EndSelectionIndex)
        {
            FUNC_DEBUG_EXIT();
            return;
        }
        
        if(StartSelectionIndex < 0 || StartSelectionIndex > CharactersRenderInfos.size() ||
            EndSelectionIndex < 0 || EndSelectionIndex > CharactersRenderInfos.size())
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        auto drawHighlight = [&](int startIndex, int inclusiveEndIndex, glm::u8vec4 highlightColor)
        {
            DrawingVerticies.push_back(CharactersRenderInfos[startIndex].RenderPosition + GetGlobalPosition() +
                glm::vec2(0, CharactersRenderInfos[startIndex].LineMinY));

            DrawingVerticies.push_back(CharactersRenderInfos[inclusiveEndIndex].RenderPosition + GetGlobalPosition() +
                glm::vec2(CharactersRenderInfos[inclusiveEndIndex].Advance, CharactersRenderInfos[inclusiveEndIndex].LineMinY));

            DrawingVerticies.push_back(CharactersRenderInfos[inclusiveEndIndex].RenderPosition + GetGlobalPosition() +
                glm::vec2(CharactersRenderInfos[inclusiveEndIndex].Advance, CharactersRenderInfos[inclusiveEndIndex].LineMaxY));

            DrawingVerticies.push_back(CharactersRenderInfos[startIndex].RenderPosition + GetGlobalPosition() +
                glm::vec2(0, CharactersRenderInfos[startIndex].LineMaxY));

            for(int i = 0; i < 4; i++)
            {
                DrawingUVs.push_back(glm::vec2());
                DrawingColours.push_back(highlightColor);
            }

            DrawingCounts.push_back(4);
            DrawingProperties.push_back(ssGUI::DrawingProperty());
        };

        int startIndex, endIndex;

        if(StartSelectionIndex < EndSelectionIndex)
        {
            startIndex = StartSelectionIndex;
            endIndex = EndSelectionIndex;
        }
        else
        {
            startIndex = EndSelectionIndex;
            endIndex = StartSelectionIndex;
        }

        int lineStartIndex = startIndex;

        for(int i = startIndex; i < endIndex; i++)
        {           
            //Check newline
            if(CharactersRenderInfos[i].CharacterAtNewline && i != 0 && lineStartIndex != i)
            {
                drawHighlight(lineStartIndex, i-1, SelectionColor);
                lineStartIndex = i;
            }

            //Check last character
            if(i == endIndex - 1)
                drawHighlight(lineStartIndex, i, glm::u8vec4(51, 153, 255, 255));
        }

        FUNC_DEBUG_EXIT();
    }

    void Text::ApplyTextUnderline()
    {
        FUNC_DEBUG_ENTRY();

        auto drawUnderline = [&](int startIndex, int inclusiveEndIndex, glm::u8vec4 underlineColor, float thickness, float underlineOffset)
        {
            DrawingVerticies.push_back(CharactersRenderInfos[startIndex].RenderPosition + GetGlobalPosition() +
                glm::vec2(0, underlineOffset));

            DrawingVerticies.push_back(CharactersRenderInfos[inclusiveEndIndex].RenderPosition + GetGlobalPosition() + 
                glm::vec2(CharactersRenderInfos[inclusiveEndIndex].Advance, underlineOffset));

            DrawingVerticies.push_back(CharactersRenderInfos[inclusiveEndIndex].RenderPosition + GetGlobalPosition() + 
                glm::vec2(CharactersRenderInfos[inclusiveEndIndex].Advance, underlineOffset + thickness));

            DrawingVerticies.push_back(CharactersRenderInfos[startIndex].RenderPosition + GetGlobalPosition() + 
                glm::vec2(0, underlineOffset + thickness));

            for(int i = 0; i < 4; i++)
            {
                DrawingUVs.push_back(glm::vec2());
                DrawingColours.push_back(underlineColor);
            }

            DrawingCounts.push_back(4);
            DrawingProperties.push_back(ssGUI::DrawingProperty());
        };

        //This handles font size and color change
        auto drawMultiColoredUnderline = [&](int startIndex, int inclusiveEndIndex)
        {
            std::function<bool(int)> isIndexInSelection;

            //Setup lambda for checking character is highlighted
            if(IsMultilineAllowed() && GetStartSelectionIndex() >= 0 && GetEndSelectionIndex() >= 0)
            {
                if(GetStartSelectionIndex() < GetEndSelectionIndex())
                {
                    isIndexInSelection = [&](int targetIndex)->bool
                    {
                        return targetIndex >= GetStartSelectionIndex() && targetIndex < GetEndSelectionIndex(); 
                    };
                }
                else
                {
                    isIndexInSelection = [&](int targetIndex)->bool
                    {
                        return targetIndex >= GetEndSelectionIndex() && targetIndex < GetStartSelectionIndex(); 
                    };
                }
            }
            else
                isIndexInSelection = [](int targetIndex){return false;};
            
            float maxFontSize = -1;
            int maxFontSizeCharIndex = -1;
            
            //Find max font size
            for (int i = startIndex; i <= inclusiveEndIndex; i++)
            {
                const ssGUI::CharacterDetails& curDetail = GetInternalCharacterDetail(i);
                
                if((curDetail.FontIndex != -1 || curDetail.DefaultFontIndex != -1) && 
                    curDetail.FontSize > maxFontSize)
                {
                    maxFontSize = curDetail.FontSize;
                    maxFontSizeCharIndex = i;
                }
            }

            ssGUI::Backend::BackendFontInterface* fontInterface = nullptr;
            if(GetInternalCharacterDetail(maxFontSizeCharIndex).FontIndex != -1)
                fontInterface = GetFont(GetInternalCharacterDetail(maxFontSizeCharIndex).FontIndex)->GetBackendFontInterface();
            else
                fontInterface = GetDefaultFont(GetInternalCharacterDetail(maxFontSizeCharIndex).DefaultFontIndex)->GetBackendFontInterface();

            //Draw underlines
            glm::u8vec4 currentUnderlineColor = GetInternalCharacterDetail(startIndex).CharacterColor;
            int lastIndex = startIndex;
            bool IsLastCharHighlighted = isIndexInSelection(startIndex);
            for (int i = startIndex; i <= inclusiveEndIndex; i++)
            {
                bool curCharHighlighted = isIndexInSelection(i);
                
                //Check for color change or highlight change
                if(GetInternalCharacterDetail(i).CharacterColor != currentUnderlineColor || curCharHighlighted != IsLastCharHighlighted)
                {           
                    drawUnderline
                    (
                        lastIndex, 
                        i-1, 
                        IsLastCharHighlighted ? TextSelectedColor : currentUnderlineColor, 
                        fontInterface->GetUnderlineThickness(maxFontSize), 
                        fontInterface->GetUnderlineOffset(maxFontSize)    
                    );

                    currentUnderlineColor = GetInternalCharacterDetail(i).CharacterColor;
                    lastIndex = i;
                }

                //last index, draw underline
                if(i == inclusiveEndIndex)
                {
                    drawUnderline
                    (
                        lastIndex, 
                        i, 
                        curCharHighlighted ? TextSelectedColor : currentUnderlineColor, 
                        fontInterface->GetUnderlineThickness(maxFontSize), 
                        fontInterface->GetUnderlineOffset(maxFontSize)    
                    );
                }

                IsLastCharHighlighted = curCharHighlighted;
            }
        };

        int underlineStartIndex = -1;

        ssGUI::Backend::BackendFontInterface* fontInterface = nullptr;
        for (int i = 0; i < CurrentCharactersDetails.Size(); i++)
        {
            ssGUI::CharacterDetails& curDetail = GetInternalCharacterDetail(i);
            wchar_t curChar = curDetail.Character;

            //If current character/font is invalid
            if(!CharactersRenderInfos[i].Valid)
            {               
                continue;
            }
            
            if(curDetail.FontIndex != -1)
                fontInterface = GetFont(curDetail.FontIndex)->GetBackendFontInterface();
            else
                fontInterface = GetDefaultFont(curDetail.DefaultFontIndex)->GetBackendFontInterface();
            
            //If underline started
            if(underlineStartIndex != -1)
            {
                //If current charcter is at newline or
                //If current character doesn't need underline
                if(CharactersRenderInfos[i].CharacterAtNewline || !curDetail.Underlined)
                {
                    //Break underline
                    drawMultiColoredUnderline(underlineStartIndex, i - 1);

                    //Reset status
                    underlineStartIndex = -1;
                }
            }

            //If underline not started
            if(underlineStartIndex == -1)
            {
                //Record underline if needed
                if(curDetail.Underlined)
                    underlineStartIndex = i;
            }

            //Check if last index and underline needed
            if(underlineStartIndex != -1 && i == CurrentCharactersDetails.Size() - 1)
            {
                //Draw underline
                drawMultiColoredUnderline(underlineStartIndex, i);
            }
        }

        FUNC_DEBUG_EXIT();
    }

    void Text::DrawAllCharacters()
    {
        if(!IsTextSelectionAllowed() || StartSelectionIndex < 0 || EndSelectionIndex < 0 || 
            StartSelectionIndex == EndSelectionIndex)
        {
            for (int i = 0; i < CurrentCharactersDetails.Size(); i++)
            {
                const ssGUI::CharacterDetails& curDetail = GetInternalCharacterDetail(i);
                
                // Add the glyph to the vertices
                DrawCharacter(GetGlobalPosition(), CharactersRenderInfos[i], curDetail);
            }
        }
        else
        {
            std::function<bool(int)> isIndexInSelection;
            
            if(GetStartSelectionIndex() < GetEndSelectionIndex())
            {
                isIndexInSelection = [&](int targetIndex)->bool
                {
                    return targetIndex >= GetStartSelectionIndex() && targetIndex < GetEndSelectionIndex(); 
                };
            }
            else
            {
                isIndexInSelection = [&](int targetIndex)->bool
                {
                    return targetIndex >= GetEndSelectionIndex() && targetIndex < GetStartSelectionIndex(); 
                };
            }

            for (int i = 0; i < CurrentCharactersDetails.Size(); i++)
            {
                
                if(!isIndexInSelection(i))
                {
                    // Add the glyph to the vertices
                    DrawCharacter(GetGlobalPosition(), CharactersRenderInfos[i], GetInternalCharacterDetail(i));
                }
                else
                {
                    ssGUI::CharacterDetails curDetail = GetInternalCharacterDetail(i);
                    curDetail.CharacterColor = TextSelectedColor;
                    DrawCharacter(GetGlobalPosition(), CharactersRenderInfos[i], curDetail);
                }
            }
        }
    }

    void Text::ConstructRenderInfo()
    {
        FUNC_DEBUG_ENTRY();
        
        glm::vec2 drawPos = GetGlobalPosition();

        //Drawing background
        DrawingVerticies.push_back(drawPos);
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingVerticies.push_back(drawPos + glm::vec2(GetSize().x, 0));
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingVerticies.push_back(drawPos + glm::vec2(GetSize().x, GetSize().y));
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingVerticies.push_back(drawPos + glm::vec2(0, GetSize().y));
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingCounts.push_back(4);
        DrawingProperties.push_back(ssGUI::DrawingProperty());

        if(GetFontsCount() == 0 && GetDefaultFontsCount() == 0)
        {
            DEBUG_LINE("Failed to find any fonts");
            FUNC_DEBUG_EXIT();
            return;
        }

        //TODO: Some optimization can be done here depending on what characters are changed

        if(RecalculateTextNeeded)
            ComputeCharactersPositionAndSize(); 

        if(IsTextSelectionAllowed())
            ApplyTextHighlight();

        DrawAllCharacters();
        
        ApplyTextUnderline();

        FUNC_DEBUG_EXIT();
    }

    void Text::MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                        ssGUI::GUIObject* mainWindow)
    {
        //Check any changes to default fonts
        //TODO: Maybe need optimization
        bool defaultFontsChanged = false; 
        if(LastDefaultFonts.size() != DefaultFonts.size()) 
            defaultFontsChanged = true; 
        else 
        { 
            for(int i = 0; i < DefaultFonts.size(); i++) 
            { 
                if(LastDefaultFonts[i] != DefaultFonts[i]) 
                { 
                    defaultFontsChanged = true; 
                    break; 
                } 
            } 
        } 
 
        if(defaultFontsChanged) 
        { 
            LastDefaultFonts = DefaultFonts; 
            RecalculateTextNeeded = true; 
            RedrawObject(); 
        }
        
        if(inputStatus.MouseInputBlockedObject == nullptr && IsBlockInput())
        {
            //Mouse Input blocking
            glm::ivec2 currentMousePos = inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow));

            bool mouseInWindowBoundX = false;
            bool mouseInWindowBoundY = false;
            
            if(currentMousePos.x >= GetGlobalPosition().x && currentMousePos.x <= GetGlobalPosition().x + GetSize().x)
                mouseInWindowBoundX = true;

            if(currentMousePos.y >= GetGlobalPosition().y && currentMousePos.y <= GetGlobalPosition().y + GetSize().y)
                mouseInWindowBoundY = true;
            
            //Input blocking
            if(mouseInWindowBoundX && mouseInWindowBoundY)
            {
                inputInterface->SetCursorType(ssGUI::Enums::CursorType::TEXT); 
                inputStatus.MouseInputBlockedObject = this;
            }

            //If mouse click on this, set focus
            if(mouseInWindowBoundX && mouseInWindowBoundY &&
                ((inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) && !inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::LEFT)) ||
                (inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::MIDDLE) && !inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::MIDDLE)) ||
                (inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::RIGHT) && !inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::RIGHT))))
            {
                SetFocus(true);    
            }

            //Text selection with mouse
            if(IsTextSelectionAllowed() && inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) &&
                mouseInWindowBoundX && mouseInWindowBoundY && IsInteractable())
            {
                int closestIndex = GetNearestCharacterIndexFromPos
                (
                    inputInterface->GetCurrentMousePosition(static_cast<ssGUI::MainWindow*>(mainWindow)), true
                );

                if(closestIndex == GetLastValidCharacterIndex() && 
                    IsPosAfterLastCharacter(inputInterface->GetCurrentMousePosition(static_cast<ssGUI::MainWindow*>(mainWindow))))
                {
                    closestIndex += 1;
                }

                //When left click down, set start and end selection index to the closest (exclusive) index
                if(!inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::LEFT))
                {
                    SetStartSelectionIndex(closestIndex);
                    SetEndSelectionIndex(closestIndex);
                }
                //When left clicking, move end selection index to the closest index
                else
                    SetEndSelectionIndex(closestIndex);
            }
        }

        if(inputStatus.KeyInputBlockedObject == nullptr)
        {
            //Text copying
            if(IsTextSelectionAllowed() && GetStartSelectionIndex() >= 0 && GetEndSelectionIndex() >= 0 && 
                (inputInterface->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::LEFT_CTRL) || 
                inputInterface->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::RIGHT_CTRL)) &&
                !inputInterface->GetLastKeyPresses().IsLetterKeyPresent(ssGUI::Enums::LetterKey::C) &&
                inputInterface->GetCurrentKeyPresses().IsLetterKeyPresent(ssGUI::Enums::LetterKey::C))
            {
                std::wstring curText = GetText();

                if(GetStartSelectionIndex() < GetEndSelectionIndex())
                {
                    curText = curText.substr(GetStartSelectionIndex(), GetEndSelectionIndex() - GetStartSelectionIndex());
                    inputInterface->SetClipboardText(curText); 
                }
                else if(GetEndSelectionIndex() < GetStartSelectionIndex())
                {
                    curText = curText.substr(GetEndSelectionIndex(), GetStartSelectionIndex() - GetEndSelectionIndex());
                    inputInterface->SetClipboardText(curText); 
                }

                inputStatus.KeyInputBlockedObject = this;
            }
        }
    }
    
    const std::string Text::ListenerKey = "Text";

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

    Text::~Text()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
    }
    
    void Text::ComputeCharactersPositionAndSize()
    {
        FUNC_DEBUG_ENTRY();
        if(GetFontsCount() == 0 && GetDefaultFontsCount() == 0)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        RedrawObject();
        RecalculateTextNeeded = false;
        Overflow = false;
        
        //Clear and reprocess the character details
        ProcessedCharacterDetails.clear(); 

        FormatNewlinesCharacters();

        AssignSupportedFont();

        CharactersRenderInfos.clear();

        //Empty text guard
        if(CurrentCharactersDetails.Empty())
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        //Populate CharactersRenderInfos list
        for(int i = 0; i < CurrentCharactersDetails.Size(); i++)
            CharactersRenderInfos.push_back(ssGUI::CharacterRenderInfo());
        
        if(WrappingMode == ssGUI::Enums::TextWrapping::WORD_WRAPPING)
            ConstructRenderInfosForWordWrapping();
        else if(WrappingMode == ssGUI::Enums::TextWrapping::CHARACTER_WRAPPING)
            ConstructRenderInfosForCharacterWrapping();
        else
            ConstructRenderInfosForNoWrapping(true);

        ApplyFontLineSpacing();
        ApplyTextAlignment();

        FUNC_DEBUG_EXIT();
    }
    
    void Text::SetText(std::wstring text)
    {
        RecalculateTextNeeded = true;

        CurrentCharactersDetails.Clear();
        ProcessedCharacterDetails.clear();
        std::vector<ssGUI::CharacterDetails> newDetails;
        ConstructCharacterDetails(text, newDetails);
        AddCurrentCharacterDetails(newDetails);
        RedrawObject();
    }

    void Text::SetText(std::string text)
    {
        RecalculateTextNeeded = true;

        CurrentCharactersDetails.Clear();
        ProcessedCharacterDetails.clear();
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring currentText = converter.from_bytes(text);
        std::vector<ssGUI::CharacterDetails> newDetails;
        ConstructCharacterDetails(currentText, newDetails);
        AddCurrentCharacterDetails(newDetails);
        RedrawObject();
    }

    void Text::AddText(std::wstring text)
    {
        std::vector<ssGUI::CharacterDetails> newDetails;
        ConstructCharacterDetails(text, newDetails);
        AddCurrentCharacterDetails(newDetails);
        RedrawObject();
    }

    void Text::AddText(std::string text)
    {
        std::vector<ssGUI::CharacterDetails> newDetails;
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring currentText = converter.from_bytes(text);
        ConstructCharacterDetails(currentText, newDetails);
        AddCurrentCharacterDetails(newDetails);
        RedrawObject();
    }

    void Text::AddText(std::wstring text, int index)
    {
        std::vector<ssGUI::CharacterDetails> newDetails;
        ConstructCharacterDetails(text, newDetails);
        AddCurrentCharacterDetails(index, newDetails);
        RedrawObject();
    }

    void Text::AddText(std::string text, int index)
    {
        std::vector<ssGUI::CharacterDetails> newDetails;
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring currentText = converter.from_bytes(text);
        ConstructCharacterDetails(currentText, newDetails);
        AddCurrentCharacterDetails(index, newDetails);
        RedrawObject();
    }

    void Text::RemoveText(int startIndex, int exclusiveEndIndex)
    {
        RemoveCurrentCharacterDetails(startIndex, exclusiveEndIndex);
        RedrawObject();
    }

    std::wstring Text::GetText()
    {
        std::wstring currentText = L"";
        for(int i = 0; i < CurrentCharactersDetails.Size(); i++)
        {
            currentText += CurrentCharactersDetails[i].Character;
        }
        return currentText;
    }

    int Text::GetCharacterCount() const
    {
        return CurrentCharactersDetails.Size();
    }

    ssGUI::CharacterRenderInfo Text::GetCharacterRenderInfo(int index) const
    {
        if(index < 0 || index >= CharactersRenderInfos.size())
            return ssGUI::CharacterRenderInfo();
        
        return CharactersRenderInfos[index];
    }
    
    void Text::SetCurrentCharacterDetails(int index, ssGUI::CharacterDetails details)
    {
        if(index < 0 || index >= CurrentCharactersDetails.Size())
            return;

        CurrentCharactersDetails[index] = details;

        RecalculateTextNeeded = true;
        RedrawObject();
    }

    ssGUI::CharacterDetails Text::GetCurrentCharacterDetails(int index)
    {
        if(index < 0 || index >= CurrentCharactersDetails.Size())
            return ssGUI::CharacterDetails();
        
        return CurrentCharactersDetails[index];
    }

    int Text::GetCurrentCharactersDetailsCount() const
    {
        return CurrentCharactersDetails.Size();
    }

    void Text::AddCurrentCharacterDetails(int index, ssGUI::CharacterDetails details)
    {
        if(index < 0 || index > CurrentCharactersDetails.Size())
            return;

        CurrentCharactersDetails.Add(details, index);

        RecalculateTextNeeded = true;
        RedrawObject();
    }

    void Text::AddCurrentCharacterDetails(ssGUI::CharacterDetails details)
    {
        CurrentCharactersDetails.Add(details);
        RecalculateTextNeeded = true;
        RedrawObject();
    }
    
    void Text::AddCurrentCharacterDetails(int index, std::vector<ssGUI::CharacterDetails>& details)
    {
        if(index < 0 || index > CurrentCharactersDetails.Size())
            return;

        CurrentCharactersDetails.Add(details, index);

        RecalculateTextNeeded = true;
        RedrawObject();
    }

    void Text::AddCurrentCharacterDetails(std::vector<ssGUI::CharacterDetails>& details)
    {
        CurrentCharactersDetails.Add(details);
    }

    void Text::RemoveCurrentCharacterDetails(int index)
    {
        if(index < 0 || index >= CurrentCharactersDetails.Size())
            return;

        CurrentCharactersDetails.Remove(index);
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    void Text::RemoveCurrentCharacterDetails(int startIndex, int exclusiveEndIndex)
    {
        if(startIndex < 0 || startIndex >= CurrentCharactersDetails.Size())
            return;

        if(exclusiveEndIndex < 0 || exclusiveEndIndex > CurrentCharactersDetails.Size())
            return;

        CurrentCharactersDetails.Remove(startIndex, exclusiveEndIndex);
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    void Text::ClearAllCurrentCharacterDetails()
    {
        CurrentCharactersDetails.Clear();
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    glm::vec2 Text::GetCharacterGlobalPosition(int index, bool topLeftCorner)
    {
        if(index < 0 || index >= CharactersRenderInfos.size())
            return glm::vec2();
        
        return GetGlobalPosition() + 
                (topLeftCorner ? 
                CharactersRenderInfos[index].RenderPosition + CharactersRenderInfos[index].DrawOffset :
                CharactersRenderInfos[index].RenderPosition);
    }

    bool Text::IsOverflow() const
    {
        return Overflow;
    }

    void Text::SetNewCharacterFontSize(float size)
    {
        FontSize = size;
    }

    float Text::GetNewCharacterFontSize() const
    {
        return FontSize;
    }

    void Text::SetNewCharacterColor(glm::u8vec4 color)
    {
        TextColor = color;
    }

    glm::u8vec4 Text::GetNewCharacterColor() const
    {
        return TextColor;
    } 

    void Text::SetNewCharacterUnderlined(bool underline)
    {
        TextUnderline = underline;
    }

    bool Text::IsNewCharacterUnderlined() const
    {
        return TextUnderline;
    }

    void Text::ApplyNewCharacterSettingsToText()
    {
        RecalculateTextNeeded = true;
        
        for(size_t i = 0; i < CurrentCharactersDetails.Size(); i++)
        {
            CurrentCharactersDetails[i].FontSize = GetNewCharacterFontSize();
            CurrentCharactersDetails[i].CharacterColor = GetNewCharacterColor();
            CurrentCharactersDetails[i].Underlined = IsNewCharacterUnderlined();
        }
        
        RedrawObject();
    }

    void Text::SetMultilineAllowed(bool multiline)
    {
        MultilineAllowed = multiline;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    bool Text::IsMultilineAllowed() const
    {
        return MultilineAllowed;
    }   

    void Text::SetWrappingMode(ssGUI::Enums::TextWrapping wrappingMode)
    {
        WrappingMode = wrappingMode;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    ssGUI::Enums::TextWrapping Text::GetWrappingMode() const
    {
        return WrappingMode;
    }

    void Text::SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal align)
    {
        HorizontalAlignment = align;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    ssGUI::Enums::TextAlignmentHorizontal Text::GetHorizontalAlignment() const
    {
        return HorizontalAlignment;
    }

    void Text::SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical align)
    {
        VerticalAlignment = align;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    ssGUI::Enums::TextAlignmentVertical Text::GetVerticalAlignment() const
    {
        return VerticalAlignment;
    }

    void Text::AddFont(ssGUI::Font* font)
    {
        if(font == nullptr)
            return;

        if(IsEventCallbackExist(ssGUI::EventCallbacks::OnFontChangeEventCallback::EVENT_NAME))
            GetEventCallback(ssGUI::EventCallbacks::OnFontChangeEventCallback::EVENT_NAME)->Notify(this);
        
        CurrentFonts.push_back(font);
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    void Text::AddFont(ssGUI::Font* font, int index)
    {
        if(font == nullptr)
            return;
        
        if(IsEventCallbackExist(ssGUI::EventCallbacks::OnFontChangeEventCallback::EVENT_NAME))
            GetEventCallback(ssGUI::EventCallbacks::OnFontChangeEventCallback::EVENT_NAME)->Notify(this);
        
        CurrentFonts.insert(CurrentFonts.begin() + index, font);
        RecalculateTextNeeded = true;
        RedrawObject();
    }
    
    ssGUI::Font* Text::GetFont(int index) const
    {
        if(index < 0 || index >= CurrentFonts.size())
            return nullptr;
        
        return CurrentFonts[index];
    }

    void Text::SetFont(ssGUI::Font* font, int index)
    {
        if(index < 0 || index >= CurrentFonts.size() || font == nullptr)
            return;

        if(font != CurrentFonts[index] && IsEventCallbackExist(ssGUI::EventCallbacks::OnFontChangeEventCallback::EVENT_NAME))
            GetEventCallback(ssGUI::EventCallbacks::OnFontChangeEventCallback::EVENT_NAME)->Notify(this);

        CurrentFonts[index] = font;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    void Text::RemoveFont(int index)
    {
        if(index < 0 || index >= CurrentFonts.size())
            return;

        if(IsEventCallbackExist(ssGUI::EventCallbacks::OnFontChangeEventCallback::EVENT_NAME))
            GetEventCallback(ssGUI::EventCallbacks::OnFontChangeEventCallback::EVENT_NAME)->Notify(this);

        CurrentFonts.erase(CurrentFonts.begin() + index);
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    int Text::GetFontsCount() const
    {
        return CurrentFonts.size();
    }

    void Text::SetHorizontalPadding(float padding)
    {
        HorizontalPadding = padding;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    float Text::GetHorizontalPadding() const
    {
        return HorizontalPadding;
    }

    void Text::SetVerticalPadding(float padding)
    {
        VerticalPadding = padding;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    float Text::GetVerticalPadding() const
    {
        return VerticalPadding;
    }

    void Text::SetCharacterSpace(float charSpace)
    {
        CharacterSpace = charSpace;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    float Text::GetCharacterSpace() const
    {
        return CharacterSpace;
    }

    void Text::SetLineSpace(float lineSpace)
    {
        LineSpace = lineSpace;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    float Text::GetLineSpace() const
    {
        return LineSpace;
    }

    void Text::SetTabSize(float tabSize)
    {
        TabSize = tabSize;
        RecalculateTextNeeded = true;
        RedrawObject();
    }
    
    float Text::GetTabSize() const
    {
        return TabSize;
    }

    void Text::SetTextSelectionAllowed(bool allowed)
    {
        SelectionAllowed = allowed;
        RedrawObject();
    }

    bool Text::IsTextSelectionAllowed() const
    {
        return SelectionAllowed;
    }

    void Text::SetStartSelectionIndex(int index)
    {
        if(IsTextSelectionAllowed())
        {
            StartSelectionIndex = index;
            RedrawObject();
        }
    }

    int Text::GetStartSelectionIndex() const
    {
        return IsTextSelectionAllowed() ? StartSelectionIndex : -1;
    }

    void Text::SetEndSelectionIndex(int index)
    {
        if(IsTextSelectionAllowed())
        {
            EndSelectionIndex = index;
            RedrawObject();
        }
    }

    int Text::GetEndSelectionIndex() const
    {
        return IsTextSelectionAllowed() ? EndSelectionIndex : -1;
    }

    void Text::SetSelectionColor(glm::u8vec4 color)
    {
        SelectionColor = color;
    }

    glm::u8vec4 Text::GetSelectionColor() const
    {
        return SelectionColor;
    }

    void Text::SetTextSelectedColor(glm::u8vec4 color)
    {
        TextSelectedColor = color;
    }

    glm::u8vec4 Text::GetTextSelectedColor() const
    {
        return TextSelectedColor;
    }

    int Text::GetContainedCharacterIndexFromPos(glm::vec2 pos)
    {
        if(CharactersRenderInfos.empty())
            return -1;
        
        //Use binary search to find the character
        int startIndex = 0;
        int endIndex = CharactersRenderInfos.size() - 1;
        int midIndex = 0;

        auto checkWithin = [](glm::vec2 checkPos, glm::vec2 charPos, glm::vec2 charSize)->bool
        {
            return checkPos.x > charPos.x && 
                checkPos.x < charPos.x + charSize.x &&
                checkPos.y > charPos.y &&
                checkPos.y < charPos.y + charSize.y;
        };

        //TODO: Add max search iterations
        while (true)
        {
            //Check start and end index are valid
            while (!CharactersRenderInfos[startIndex].Valid && startIndex < CharactersRenderInfos.size()) 
            {
                startIndex++;
            }
            while (!CharactersRenderInfos[endIndex].Valid && endIndex >= 0) 
            {
                endIndex--;
            }
            
            if(endIndex < startIndex)
                return -1;

            glm::vec2 globalPos = GetGlobalPosition();

            midIndex = (startIndex + endIndex) / 2;
            while (!CharactersRenderInfos[midIndex].Valid && midIndex >= 0) 
            {
                midIndex--;
            }
            
            //Check within
            ssGUI::CharacterRenderInfo& midInfo = CharactersRenderInfos[midIndex];
            if(checkWithin(pos, globalPos + midInfo.RenderPosition + midInfo.DrawOffset, midInfo.Size))
            {
                return midIndex;
            }

            ssGUI::CharacterRenderInfo& startInfo = CharactersRenderInfos[startIndex];
            ssGUI::CharacterRenderInfo& endInfo = CharactersRenderInfos[endIndex];

            //Check if check position is contained
            if(pos.y < globalPos.y + startInfo.LineMinY ||
                pos.y > globalPos.y + endInfo.RenderPosition.y)
            {
                return -1;
            }

            //Check if pos is before midChar or after
            //If above this line
            if(pos.y < globalPos.y + midInfo.RenderPosition.y + midInfo.LineMinY)
                endIndex = midIndex - 1;

            //If below this line
            else if(pos.y > globalPos.y + midInfo.RenderPosition.y + midInfo.LineMaxY)
                startIndex = midIndex + 1;

            //If within this line
            else
            {
                //Before this character
                if(pos.x < globalPos.x + midInfo.RenderPosition.x + midInfo.DrawOffset.x)
                    endIndex = midIndex - 1;

                //After this character
                else if(pos.x > globalPos.x + midInfo.RenderPosition.x + midInfo.DrawOffset.x)
                    startIndex = midIndex + 1;

                //Not contained
                else
                    return -1;
            }
        }
    }

    int Text::GetNearestCharacterIndexFromPos(glm::vec2 pos, bool useLeftEdge)
    {
        if(CharactersRenderInfos.empty())
            return -1;
        
        //Use binary search to find the character
        int startIndex = 0;
        int endIndex = CharactersRenderInfos.size() - 1;
        int midIndex = 0;

        //TODO: Add max search iterations
        while (true)
        {
            //Check start and end index are valid
            while (!CharactersRenderInfos[startIndex].Valid && startIndex < CharactersRenderInfos.size()) 
            {
                startIndex++;
            }
            while (!CharactersRenderInfos[endIndex].Valid && endIndex >= 0) 
            {
                endIndex--;
            }
            
            if(endIndex < startIndex)
                return -1;

            glm::vec2 globalPos = GetGlobalPosition();

            midIndex = (startIndex + endIndex) / 2;
            while (!CharactersRenderInfos[midIndex].Valid && midIndex >= 0) 
            {
                midIndex--;
            }

            ssGUI::CharacterRenderInfo& midInfo = CharactersRenderInfos[midIndex];
            ssGUI::CharacterRenderInfo& startInfo = CharactersRenderInfos[startIndex];
            ssGUI::CharacterRenderInfo& endInfo = CharactersRenderInfos[endIndex];

            //Check closest
            if(startIndex == endIndex && endIndex == midIndex)
                return midIndex;

            if(startIndex == midIndex)
            {                
                //If below the line, just go for the endIndex character
                if(pos.y > globalPos.y + midInfo.RenderPosition.y + midInfo.LineMaxY)
                    return endIndex;
                
                if(!useLeftEdge)
                {
                    return glm::distance2(pos, globalPos + midInfo.RenderPosition + midInfo.DrawOffset + midInfo.Size * 0.5f) < 
                            glm::distance2(pos, globalPos + endInfo.RenderPosition + endInfo.DrawOffset + endInfo.Size * 0.5f) ?
                            midIndex : endIndex;
                }
                else
                {
                    return glm::distance2(pos, globalPos + midInfo.RenderPosition + midInfo.DrawOffset + glm::vec2(0, midInfo.Size.y * 0.5f)) < 
                            glm::distance2(pos, globalPos + endInfo.RenderPosition + endInfo.DrawOffset + glm::vec2(0, endInfo.Size.y * 0.5f)) ?
                            midIndex : endIndex;
                }
            }

            if(endIndex == midIndex)
            {
                //If above the line, just go for the startIndex character
                if(pos.y < globalPos.y + midInfo.RenderPosition.y + midInfo.LineMinY)
                    return startIndex;
                
                if(!useLeftEdge)
                {
                    return glm::distance2(pos, globalPos + midInfo.RenderPosition + midInfo.DrawOffset + midInfo.Size * 0.5f) < 
                            glm::distance2(pos, globalPos + startInfo.RenderPosition + startInfo.DrawOffset + startInfo.Size * 0.5f) ?
                            midIndex : startIndex;
                }
                else
                {
                    return glm::distance2(pos, globalPos + midInfo.RenderPosition + midInfo.DrawOffset + glm::vec2(0, midInfo.Size.y * 0.5f)) < 
                            glm::distance2(pos, globalPos + startInfo.RenderPosition + startInfo.DrawOffset + glm::vec2(0, startInfo.Size.y * 0.5f)) ?
                            midIndex : startIndex;
                }
            }

            //Check if pos is before midChar or after
            //If above this line
            if(pos.y < globalPos.y + midInfo.RenderPosition.y + midInfo.LineMinY)
            {
                endIndex = midIndex;
            }
            //If below this line
            else if(pos.y > globalPos.y + midInfo.RenderPosition.y + midInfo.LineMaxY)
            {
                startIndex = midIndex;
            }
            //If within this line
            else
            {
                //Before this character
                if(pos.x < globalPos.x + midInfo.RenderPosition.x + midInfo.DrawOffset.x)
                    endIndex = midIndex;
                //After this character
                else
                    startIndex = midIndex;
            }
        }
    }

    int Text::GetFirstValidCharacterIndex()
    {
        int firstIndex = 0;

        while (firstIndex < CharactersRenderInfos.size() && !CharactersRenderInfos[firstIndex].Valid) 
        {
            firstIndex++;
        }
        
        return firstIndex;
    }

    int Text::GetLastValidCharacterIndex()
    {
        int lastIndex = CharactersRenderInfos.size() - 1;

        while (lastIndex >= 0 && !CharactersRenderInfos[lastIndex].Valid) 
        {
            lastIndex--;
        }

        if(lastIndex < 0)
            return -1;
        
        return lastIndex;
    }
    
    bool Text::IsPosAfterLastCharacter(glm::vec2 pos)
    {
        if(CharactersRenderInfos.empty())
            return false;

        int lastIndex = GetLastValidCharacterIndex();

        if(lastIndex < 0)
            return false;

        //If cursor is on the same line as last character
        if(pos.y >= GetGlobalPosition().y + CharactersRenderInfos[lastIndex].RenderPosition.y + CharactersRenderInfos[lastIndex].LineMinY &&
            pos.y <= GetGlobalPosition().y + CharactersRenderInfos[lastIndex].RenderPosition.y + CharactersRenderInfos[lastIndex].LineMaxY)
        {
            //If newline, it will be pointing to character before it
            if(GetInternalCharacterDetail(lastIndex).Character == '\n')
                return false;
            //Otherwise check if the cursor is to the right of the character
            else if(pos.x > GetGlobalPosition().x + CharactersRenderInfos[lastIndex].RenderPosition.x + CharactersRenderInfos[lastIndex].Advance)
                return true;
            else
                return false;
        }
        //If cursor is after this line
        else if(pos.y > GetGlobalPosition().y + CharactersRenderInfos[lastIndex].RenderPosition.y + CharactersRenderInfos[lastIndex].LineMaxY)
            return true;
        else
            return false;
    }

    void Text::AddDefaultFont(ssGUI::Font* font)
    {
        if(font == nullptr)
            return;
        
        DefaultFonts.push_back(font);
    }

    void Text::AddDefaultFont(ssGUI::Font* font, int index)
    {
        if(font == nullptr)
            return;
        
        DefaultFonts.insert(DefaultFonts.begin() + index, font);
    }
    
    ssGUI::Font* Text::GetDefaultFont(int index)
    {
        if(index < 0 || index >= DefaultFonts.size())
            return nullptr;
        
        return DefaultFonts[index];
    }

    void Text::SetDefaultFont(ssGUI::Font* font, int index)
    {
        if(index < 0 || index >= DefaultFonts.size() || font == nullptr)
            return;

        DefaultFonts[index] = font;
    }

    void Text::RemoveDefaultFont(int index)
    {
        if(index < 0 || index >= DefaultFonts.size())
            return;

        DefaultFonts.erase(DefaultFonts.begin() + index);
    }

    int Text::GetDefaultFontsCount()
    {
        return DefaultFonts.size();
    }

    ssGUI::Enums::GUIObjectType Text::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::WIDGET | ssGUI::Enums::GUIObjectType::TEXT;
    }

    Text* Text::Clone(bool cloneChildren)
    {
        FUNC_DEBUG_ENTRY();
        Text* temp = new Text(*this);
        CloneExtensionsAndEventCallbacks(temp);
        
        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
            {
                FUNC_DEBUG_EXIT();
                return nullptr;
            }
        }

        FUNC_DEBUG_EXIT();
        return temp;
    }
}