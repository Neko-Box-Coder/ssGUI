#include "ssGUI/GUIObjectClasses/Text.hpp"
#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

#include "ssGUI/EmbeddedResources.h"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"
#include "ssGUI/HeaderGroups/InputGroup.hpp"
#include "glm/gtx/norm.hpp"
#include <cmath>
#include <locale>
#include <codecvt>

namespace ssGUI
{    
    int Text::TextObjectCount = 0;
    std::vector<ssGUI::Font*> Text::DefaultFonts = std::vector<ssGUI::Font*>();
    uint32_t Text::DefaultFontsChangeID = 1;

    Text::Text(Text const& other) : Widget(other)
    {
        RecalculateTextNeeded = true;
        CurrentCharactersDetails = other.CurrentCharactersDetails;
        CharactersRenderInfos = other.CharactersRenderInfos;
        ProcessedCharacterDetails = other.ProcessedCharacterDetails;
        Overflow = other.Overflow;
        FontSize = other.GetNewTextFontSize();
        TextColor = other.GetNewTextColor();
        TextUnderline = other.IsNewTextUnderlined();
        MultilineAllowed = other.IsMultilineAllowed();
        WrappingMode = other.GetWrappingMode();
        CurrentHorizontalAlignment = other.GetTextHorizontalAlignment();
        CurrentVerticalAlignment = other.GetTextVerticalAlignment();
        CurrentFonts = other.CurrentFonts;
        HorizontalPadding = other.GetTextHorizontalPadding();
        VerticalPadding = other.GetTextVerticalPadding();
        CharacterSpace = other.GetCharacterSpace();
        LineSpace = other.GetLineSpace();
        TabSize = other.GetTabSize();
        SelectionAllowed = other.IsTextSelectionAllowed();
        StartSelectionIndex = other.GetStartSelectionIndex();
        EndSelectionIndex = other.GetEndSelectionIndex();
        DeselectWhenFocusLost = other.IsDeselectWhenFocusLost();
        SelectionColor = other.GetSelectionColor();
        TextSelectedColor = other.GetTextSelectedColor();
        LastDefaultFontsID = other.LastDefaultFontsID;
        
        TextObjectCount++;
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
            detail.FontSize = GetNewTextFontSize();
            detail.CharacterColor = GetNewTextColor();
            detail.Underlined = IsNewTextUnderlined();
            details.push_back(detail);
        }
    }

    void Text::AssignSupportedFont()
    {
        ssLOG_FUNC_ENTRY();
        
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

        ssLOG_FUNC_EXIT();
    }

    void Text::DrawCharacter(glm::vec2 positionOffset, ssGUI::CharacterRenderInfo info, ssGUI::CharacterDetails details)
    {
        if(!info.Rendered)
            return;

        glm::vec2 position = positionOffset + info.BaselinePosition;
        ssGUI::Font* targetFont = nullptr;
        
        if(details.FontIndex != -1)
            targetFont = GetFont(details.FontIndex);
        else if(details.DefaultFontIndex != -1)
            targetFont = GetDefaultFont(details.DefaultFontIndex);

        if(targetFont == nullptr)
            return;
        
        ssGUI::DrawingEntity characterEntity;
        
        characterEntity.Vertices.push_back(position                                                         + info.DrawOffset * info.TargetSizeMultiplier);
        characterEntity.Vertices.push_back(position + glm::vec2(info.Size.x * info.TargetSizeMultiplier, 0) + info.DrawOffset * info.TargetSizeMultiplier);
        characterEntity.Vertices.push_back(position + info.Size * info.TargetSizeMultiplier                 + info.DrawOffset * info.TargetSizeMultiplier);
        characterEntity.Vertices.push_back(position + glm::vec2(0, info.Size.y * info.TargetSizeMultiplier) + info.DrawOffset * info.TargetSizeMultiplier);

        characterEntity.Colors.push_back(details.CharacterColor);
        characterEntity.Colors.push_back(details.CharacterColor);
        characterEntity.Colors.push_back(details.CharacterColor);
        characterEntity.Colors.push_back(details.CharacterColor);

        characterEntity.TexCoords.push_back(glm::vec2());
        characterEntity.TexCoords.push_back(glm::vec2(info.Size.x, 0));
        characterEntity.TexCoords.push_back(info.Size);
        characterEntity.TexCoords.push_back(glm::vec2(0, info.Size.y));

        characterEntity.BackendFont = targetFont->GetBackendFontInterface();
        characterEntity.CharacterSize = details.FontSize;
        characterEntity.Character = details.Character;
        characterEntity.EntityName = TEXT_CHARACTER_SHAPE_NAME;

        DrawingEntities.push_back(characterEntity);
    }

    void Text::FormatNewlinesCharacters()
    {
        ssLOG_FUNC_ENTRY();

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

        ssLOG_FUNC_EXIT();
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
        ssLOG_FUNC_ENTRY();
        
        int currentWordIndex = 0;
        float currentLineLength = GetTextHorizontalPadding();
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
                CharactersRenderInfos[i].BaselinePosition.x += drawXOffset;
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
                currentLineLength = GetTextHorizontalPadding();
                drawXOffset = 0;
            }

            currentWordLength += curRenderInfo.Advance + GetCharacterSpace();

            //See if the current word exceeds widget width when a word *finishes*
            //We do this by setting space, newline, tab or last character as word separator
            if ((curChar == L' ') || (curChar == L'\n') || (curChar == L'\t') || i == lastValidIndex)
            {
                //check if adding current word length to current line length exceeds widget width
                if(currentWordLength - GetCharacterSpace() + currentLineLength + GetTextHorizontalPadding() > GetSize().x)
                {
                    //If the word is already at newline, set overflow
                    if(CharactersRenderInfos[currentWordIndex].CharacterAtNewline)
                        Overflow = true;
                    //Otherwise, reposition the word to the newlne
                    else
                    {
                        float curWordXOffset = GetTextHorizontalPadding() - CharactersRenderInfos[currentWordIndex].BaselinePosition.x;
                        CharactersRenderInfos[currentWordIndex].CharacterAtNewline = true;
                        currentLineLength = GetTextHorizontalPadding();

                        for(int j = currentWordIndex; j < i; j++)
                            CharactersRenderInfos[j].BaselinePosition.x += curWordXOffset;
                        
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
            curRenderInfo.BaselinePosition.x += drawXOffset;
        }

        ssLOG_FUNC_EXIT();
    }

    void Text::ConstructRenderInfosForCharacterWrapping()
    {
        ssLOG_FUNC_ENTRY();

        float currentLineLength = GetTextHorizontalPadding();
        float drawXOffset = 0;

        //First pass, Construct render infos as no character wrapping
        ConstructRenderInfosForNoWrapping(true);
        Overflow = false;

        //Second pass, reposition the characters as character wrapping
        for (int i = 0; i < CurrentCharactersDetails.Size(); i++)
        {
            if(!CharactersRenderInfos[i].Valid)
            {
                CharactersRenderInfos[i].BaselinePosition.x += drawXOffset;
                continue;
            }

            ssGUI::CharacterDetails& curDetail = GetInternalCharacterDetail(i);
            ssGUI::CharacterRenderInfo& curRenderInfo = CharactersRenderInfos[i];
            float characterLength = curRenderInfo.Advance + GetCharacterSpace();

            //Check for newline. If so, reset word and line settings
            if(curRenderInfo.CharacterAtNewline)
            {
                currentLineLength = GetTextHorizontalPadding();
                drawXOffset = 0;
            }

            //If exceed widget width
            if(currentLineLength + characterLength + GetTextHorizontalPadding() > GetSize().x)
            {
                //If this character is already at newline, set overflow
                if(curRenderInfo.CharacterAtNewline)
                    Overflow = true;
                //Otherwise, move character to newline
                else
                {
                    drawXOffset = GetTextHorizontalPadding() - curRenderInfo.BaselinePosition.x;
                    curRenderInfo.CharacterAtNewline = true;
                    currentLineLength = GetTextHorizontalPadding();
                }
            }
            currentLineLength += characterLength;

            //Apply offset
            curRenderInfo.BaselinePosition.x += drawXOffset;
        }

        ssLOG_FUNC_EXIT();
    }

    void Text::ConstructRenderInfosForNoWrapping(bool checkValid)
    {
        ssLOG_FUNC_ENTRY();
        float drawXPos = GetTextHorizontalPadding();

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
                    curRenderInfo.BaselinePosition = glm::vec2(drawXPos, 0);
                    continue;
                }
                else
                    curRenderInfo.Valid = true;
            }
            //Skip if not valid
            else if(!curRenderInfo.Valid)
            {
                prevChar = 0;
                curRenderInfo.BaselinePosition = glm::vec2(drawXPos, 0);
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
            ssGUI::CharacterRenderInfo info = fontInterface->GetCharacterRenderInfo(curChar, curDetail.FontSize);

            //If space or tab or newline character, just append
            //It is possible for any other character that has no texture, we handle it here as well
            if (curChar == L' ' || curChar == L'\n' || curChar == L'\t' || info.Size == glm::vec2())
            {
                //Check newline
                if(nextCharIsAtNewline)
                {
                    drawXPos = GetTextHorizontalPadding();
                    curRenderInfo.CharacterAtNewline = true;
                    nextCharIsAtNewline = false;
                }
                
                curRenderInfo.BaselinePosition = glm::vec2(drawXPos, 0);
                ssGUI::CharacterRenderInfo whitespaceInfo = fontInterface->GetCharacterRenderInfo(L' ', curDetail.FontSize);
                
                float whitespaceWidth = 0;
                whitespaceWidth = whitespaceInfo.Advance * whitespaceInfo.TargetSizeMultiplier;
                whitespaceWidth += GetCharacterSpace();

                switch (curChar)
                {
                    case L' ':  
                        drawXPos += whitespaceWidth;
                        CharactersRenderInfos[i].Advance = whitespaceWidth; 
                        break;
                    case L'\t': 
                    {
                        float newPos = GetClosestTabSpace(GetTextHorizontalPadding(), whitespaceWidth * GetTabSize(), drawXPos);
                        float actualTabSpace = newPos - drawXPos;
                        drawXPos += actualTabSpace;
                        CharactersRenderInfos[i].Advance = actualTabSpace;
                        break;
                    }
                    case L'\n': 
                        nextCharIsAtNewline = true;
                        break;
                    //Characters that have no texture
                    default:
                        drawXPos += info.Advance * info.TargetSizeMultiplier;
                        CharactersRenderInfos[i].Advance = info.Advance * info.TargetSizeMultiplier; 
                        break;
                }

                if(drawXPos + GetTextHorizontalPadding() > GetSize().x)
                    Overflow = true;   
            }
            else 
            {
                if(!info.Valid)
                {
                    prevChar = 0;
                    curRenderInfo.BaselinePosition = glm::vec2(drawXPos, 0);
                    curRenderInfo.Valid = false;
                    continue;
                }
                
                float characterLength = info.Advance * info.TargetSizeMultiplier;
                curRenderInfo = info;

                //Check newline
                if(nextCharIsAtNewline)
                {
                    drawXPos = GetTextHorizontalPadding();
                    curRenderInfo.CharacterAtNewline = true;
                    nextCharIsAtNewline = false;
                }

                curRenderInfo.BaselinePosition = glm::vec2(drawXPos, 0);

                drawXPos += characterLength + GetCharacterSpace();

                if(drawXPos + GetTextHorizontalPadding() > GetSize().x)
                    Overflow = true;                    
            }
        }

        ssLOG_FUNC_EXIT();
    }

    void Text::ApplyFontLineSpacing()
    {
        ssLOG_FUNC_ENTRY();
        if(CharactersRenderInfos.empty())
        {
            ssLOG_FUNC_EXIT();
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
                        CharactersRenderInfos[i].BaselinePosition.y += currentOffset;
                        CharactersRenderInfos[i].LineMinY = -curMaxFontNewline;
                        CharactersRenderInfos[i].LineMaxY = -curMaxFontNewline + 
                                                            backendFont->GetLineSpacing(curMaxFontNewline) + 
                                                            GetLineSpace();
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
                        CharactersRenderInfos[i].BaselinePosition.y += currentOffset;
                        CharactersRenderInfos[i].LineMinY = -curMaxFontNewline;//-backendFont->GetLineSpacing(curMaxFontNewline) - GetLineSpace();
                        CharactersRenderInfos[i].LineMaxY = -curMaxFontNewline + 
                                                            backendFont->GetLineSpacing(curMaxFontNewline) + 
                                                            GetLineSpace();
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
                    CharactersRenderInfos[i].BaselinePosition.y += currentOffset;
                    CharactersRenderInfos[i].LineMinY = -curMaxFontNewline;
                    CharactersRenderInfos[i].LineMaxY = -curMaxFontNewline + 
                                                        backendFont->GetLineSpacing(curMaxFontNewline) + 
                                                        GetLineSpace();
                }

                //Update vertical overflow
                if(!IsOverflow() && CharactersRenderInfos[currentIndex].BaselinePosition.y + GetTextVerticalPadding() * 2 > GetSize().y)
                    Overflow = true;

                break;
            }
            
            currentIndex++;
        }

        ssLOG_FUNC_EXIT();
    }

    void Text::ApplyTextAlignment()
    {
        ssLOG_FUNC_ENTRY();
        
        if(CharactersRenderInfos.empty())
        {
            ssLOG_FUNC_EXIT();
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
            if(CharactersRenderInfos[i].BaselinePosition.y != currentLineHeight)
            {
                //Align the previous line first
                if(i > 0)
                {
                    lineEndPos =    CharactersRenderInfos[i-1].BaselinePosition.x + 
                                    CharactersRenderInfos[i-1].DrawOffset.x * CharactersRenderInfos[i-1].TargetSizeMultiplier +
                                    CharactersRenderInfos[i-1].Size.x * CharactersRenderInfos[i-1].TargetSizeMultiplier + 
                                    GetTextHorizontalPadding();
                    float alignOffset = 0; 

                    switch(CurrentHorizontalAlignment)
                    {
                        case ssGUI::Enums::AlignmentHorizontal::LEFT:
                            break;
                    
                        case ssGUI::Enums::AlignmentHorizontal::CENTER:
                            alignOffset = (GetSize().x * 0.5 - lineStartPos) - (lineEndPos - lineStartPos) * 0.5;
                            break;
                        
                        case ssGUI::Enums::AlignmentHorizontal::RIGHT:
                            alignOffset = GetSize().x - lineEndPos;
                            break;
                    }

                    for(int j = lineStartIndex; j < i; j++)
                        CharactersRenderInfos[j].BaselinePosition.x += alignOffset;
                }

                //Then record where the newline starts
                lineStartPos = 0;
                currentLineHeight = CharactersRenderInfos[i].BaselinePosition.y;
                lineStartIndex = i;
            }
            
            //End of character
            if(i == CharactersRenderInfos.size() - 1)
            {
                lineEndPos =    CharactersRenderInfos[i].BaselinePosition.x + 
                                CharactersRenderInfos[i].DrawOffset.x * CharactersRenderInfos[i].TargetSizeMultiplier +
                                CharactersRenderInfos[i].Size.x * CharactersRenderInfos[i].TargetSizeMultiplier + 
                                GetTextHorizontalPadding();
                float alignOffset = 0; 

                switch(CurrentHorizontalAlignment)
                {
                    case ssGUI::Enums::AlignmentHorizontal::LEFT:
                        break;
                
                    case ssGUI::Enums::AlignmentHorizontal::CENTER:
                        alignOffset = (GetSize().x * 0.5 - lineStartPos) - (lineEndPos - lineStartPos) * 0.5;
                        break;
                    
                    case ssGUI::Enums::AlignmentHorizontal::RIGHT:
                        alignOffset = GetSize().x - lineEndPos;
                        break;
                }

                for(int j = lineStartIndex; j <= i; j++)
                    CharactersRenderInfos[j].BaselinePosition.x += alignOffset;
            }
        }
        
        //Find out how tall all the texts are and align accordingly
        float alignOffset = 0;
        lineStartPos = 0;
        
        for(int i = 0; i < CharactersRenderInfos.size(); i++)
        {
            if( CharactersRenderInfos[i].BaselinePosition.y + 
                CharactersRenderInfos[i].DrawOffset.y * CharactersRenderInfos[i].TargetSizeMultiplier < lineStartPos)
            {
                lineStartPos =  CharactersRenderInfos[i].BaselinePosition.y + 
                                CharactersRenderInfos[i].DrawOffset.y * CharactersRenderInfos[i].TargetSizeMultiplier;
            }
        }

        lineEndPos = CharactersRenderInfos[CharactersRenderInfos.size() - 1].BaselinePosition.y + GetTextVerticalPadding();
        switch(CurrentVerticalAlignment)
        {
            case ssGUI::Enums::AlignmentVertical::TOP:
                alignOffset = GetTextVerticalPadding();
                break;
        
            case ssGUI::Enums::AlignmentVertical::CENTER:
                alignOffset = (GetSize().y * 0.5 - lineStartPos) - (lineEndPos - lineStartPos) * 0.5;
                break;
            
            case ssGUI::Enums::AlignmentVertical::BOTTOM:
                alignOffset = GetSize().y - lineEndPos;
                break;
        }

        for(int i = 0; i < CharactersRenderInfos.size(); i++)
            CharactersRenderInfos[i].BaselinePosition.y += alignOffset;

        ssLOG_FUNC_EXIT();
    }
    
    void Text::ApplyTextHighlight()
    {
        ssLOG_FUNC_ENTRY();

        if(StartSelectionIndex == EndSelectionIndex)
        {
            ssLOG_FUNC_EXIT();
            return;
        }
        
        if(StartSelectionIndex < 0 || StartSelectionIndex > CharactersRenderInfos.size() ||
            EndSelectionIndex < 0 || EndSelectionIndex > CharactersRenderInfos.size())
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        auto drawHighlight = [&](int startIndex, int inclusiveEndIndex, glm::u8vec4 highlightColor)
        {
            ssGUI::DrawingEntity highlightEntity;
        
            highlightEntity.Vertices.push_back( CharactersRenderInfos[startIndex].BaselinePosition + 
                                                GetGlobalPosition() +
                                                glm::vec2(0, CharactersRenderInfos[startIndex].LineMinY));

            highlightEntity.Vertices.push_back( CharactersRenderInfos[inclusiveEndIndex].BaselinePosition + 
                                                GetGlobalPosition() +
                                                glm::vec2(  CharactersRenderInfos[inclusiveEndIndex].Advance * CharactersRenderInfos[inclusiveEndIndex].TargetSizeMultiplier, 
                                                            CharactersRenderInfos[inclusiveEndIndex].LineMinY));

            highlightEntity.Vertices.push_back( CharactersRenderInfos[inclusiveEndIndex].BaselinePosition + 
                                                GetGlobalPosition() +
                                                glm::vec2(  CharactersRenderInfos[inclusiveEndIndex].Advance * CharactersRenderInfos[inclusiveEndIndex].TargetSizeMultiplier, 
                                                            CharactersRenderInfos[inclusiveEndIndex].LineMaxY));

            highlightEntity.Vertices.push_back( CharactersRenderInfos[startIndex].BaselinePosition + GetGlobalPosition() +
                                                glm::vec2(0, CharactersRenderInfos[startIndex].LineMaxY));

            for(int i = 0; i < 4; i++)
                highlightEntity.Colors.push_back(highlightColor);

            highlightEntity.EntityName = TEXT_HIGHLIGHT_SHAPE_NAME;
            DrawingEntities.push_back(highlightEntity);
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
                drawHighlight(lineStartIndex, i, SelectionColor);
        }

        ssLOG_FUNC_EXIT();
    }

    void Text::ApplyTextUnderline()
    {
        ssLOG_FUNC_ENTRY();

        auto drawUnderline = [&](int startIndex, int inclusiveEndIndex, glm::u8vec4 underlineColor, float thickness, float underlineOffset)
        {
            ssGUI::DrawingEntity underlineEntity;
        
            underlineEntity.Vertices.push_back( CharactersRenderInfos[startIndex].BaselinePosition + 
                                                GetGlobalPosition() +
                                                glm::vec2(0, underlineOffset));

            underlineEntity.Vertices.push_back( CharactersRenderInfos[inclusiveEndIndex].BaselinePosition + 
                                                GetGlobalPosition() + 
                                                glm::vec2(  CharactersRenderInfos[inclusiveEndIndex].Advance * CharactersRenderInfos[inclusiveEndIndex].TargetSizeMultiplier, 
                                                            underlineOffset));

            underlineEntity.Vertices.push_back( CharactersRenderInfos[inclusiveEndIndex].BaselinePosition + 
                                                GetGlobalPosition() + 
                                                glm::vec2(  CharactersRenderInfos[inclusiveEndIndex].Advance * CharactersRenderInfos[inclusiveEndIndex].TargetSizeMultiplier, 
                                                            underlineOffset + thickness));

            underlineEntity.Vertices.push_back( CharactersRenderInfos[startIndex].BaselinePosition + 
                                                GetGlobalPosition() + 
                                                glm::vec2(  0, underlineOffset + thickness));

            for(int i = 0; i < 4; i++)
                underlineEntity.Colors.push_back(underlineColor);

            underlineEntity.EntityName = TEXT_UNDERLINE_SHAPE_NAME;
            DrawingEntities.push_back(underlineEntity);
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

        //ssGUI::Backend::BackendFontInterface* fontInterface = nullptr;
        for (int i = 0; i < CurrentCharactersDetails.Size(); i++)
        {
            ssGUI::CharacterDetails& curDetail = GetInternalCharacterDetail(i);
            wchar_t curChar = curDetail.Character;

            //If current character/font is invalid
            if(!CharactersRenderInfos[i].Valid)
            {               
                continue;
            }
            
            //if(curDetail.FontIndex != -1)
            //    fontInterface = GetFont(curDetail.FontIndex)->GetBackendFontInterface();
            //else
            //    fontInterface = GetDefaultFont(curDetail.DefaultFontIndex)->GetBackendFontInterface();
            
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

        ssLOG_FUNC_EXIT();
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

//https://stackoverflow.com/questions/306533/how-do-i-get-a-list-of-files-in-a-directory-in-c
    /* Returns a list of files in a directory (except the ones that begin with a dot) */
// #include <dirent.h>
// #include <sys/stat.h>
//     void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory)
//     {
//     #ifdef WINDOWS
//         HANDLE dir;
//         WIN32_FIND_DATA file_data;

//         if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
//             return; /* No files found */

//         do {
//             const std::string file_name = file_data.cFileName;
//             const std::string full_file_name = directory + "/" + file_name;
//             const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

//             if (file_name[0] == '.')
//                 continue;

//             if (is_directory)
//                 continue;

//             out.push_back(full_file_name);
//         } while (FindNextFile(dir, &file_data));

//         FindClose(dir);
//     #else
//         DIR *dir;
//         class dirent *ent;
//         class stat st;

//         dir = opendir(directory.c_str());
//         while ((ent = readdir(dir)) != NULL) {
//             const std::string file_name = ent->d_name;
//             const std::string full_file_name = directory + "/" + file_name;

//             if (file_name[0] == '.')
//                 continue;

//             if (stat(full_file_name.c_str(), &st) == -1)
//                 continue;

//             const bool is_directory = (st.st_mode & S_IFDIR) != 0;

//             if (is_directory)
//                 continue;

//             out.push_back(full_file_name);
//         }
//         closedir(dir);
//     #endif
//     } // GetFilesInDirectory

    void Text::ConstructRenderInfo()
    {
        ssLOG_FUNC_ENTRY();
        
        glm::vec2 drawPos = GetGlobalPosition();

        //Drawing background
        ssGUI::DrawingEntity backgroundEntitiy;
        
        backgroundEntitiy.Vertices.push_back(drawPos);
        backgroundEntitiy.Vertices.push_back(drawPos + glm::vec2(GetSize().x, 0));
        backgroundEntitiy.Vertices.push_back(drawPos + glm::vec2(GetSize().x, GetSize().y));
        backgroundEntitiy.Vertices.push_back(drawPos + glm::vec2(0, GetSize().y));
        
        backgroundEntitiy.Colors.push_back(GetBackgroundColor());
        backgroundEntitiy.Colors.push_back(GetBackgroundColor());
        backgroundEntitiy.Colors.push_back(GetBackgroundColor());
        backgroundEntitiy.Colors.push_back(GetBackgroundColor());
        
        backgroundEntitiy.EntityName = GUI_OBJECT_BG_SHAPE_NAME;
        
        DrawingEntities.push_back(backgroundEntitiy);

        if(GetFontsCount() == 0 && GetDefaultFontsCount() == 0)
        {
            ssGUI_WARNING(ssGUI_GUI_OBJECT_TAG, "Failed to find any fonts");
            ssLOG_FUNC_EXIT();
            return;
        }

        //TODO: Some optimization can be done here depending on what characters are changed

        if(RecalculateTextNeeded)
            ComputeCharactersPositionAndSize(); 

        if(IsTextSelectionAllowed())
            ApplyTextHighlight();

        DrawAllCharacters();
        
        ApplyTextUnderline();

        ssLOG_FUNC_EXIT();
    }

    void Text::MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                        ssGUI::GUIObject* mainWindow)
    {
        //Check any changes to default fonts
        //TODO: Maybe need optimization
        bool defaultFontsChanged = false; 
        if(LastDefaultFontsID != DefaultFontsChangeID) 
            defaultFontsChanged = true; 
 
        if(defaultFontsChanged) 
        { 
            LastDefaultFontsID = DefaultFontsChangeID;
            RecalculateTextNeeded = true; 
            RedrawObject(); 
        }
        
        if(inputStatus.MouseInputBlockedObject == nullptr && IsBlockInput())
        {
            //Mouse Input blocking
            glm::ivec2 currentMousePos = inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)->GetBackendWindowInterface());

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
                int closestIndex = GetNearestCharacterIndexFromPos(currentMousePos, true);

                if(closestIndex == GetLastValidCharacterIndex() && IsPosAfterLastCharacter(currentMousePos))
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

        //Deselect when focus is lost
        if( IsTextSelectionAllowed() && IsDeselectWhenFocusLost() && GetStartSelectionIndex() != GetEndSelectionIndex() && 
            GetStartSelectionIndex() != -1 && !IsFocused())
        {
            SetStartSelectionIndex(GetStartSelectionIndex());
            SetEndSelectionIndex(GetStartSelectionIndex());
        }

        if(inputStatus.KeyInputBlockedObject == nullptr)
        {
            //Text copying when ctrl+c is pressed and there is something highlighted
            bool ctrlPressed =  inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::LEFT_CTRL) || 
                                inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::RIGHT_CTRL);
            
            if( IsTextSelectionAllowed() && 
                GetStartSelectionIndex() >= 0 && 
                GetEndSelectionIndex() >= 0 && 
                ctrlPressed &&
                !inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::LetterKey::C) &&
                inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::LetterKey::C))
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
    const std::string Text::TEXT_CHARACTER_SHAPE_NAME = "Text Character";
    const std::string Text::TEXT_HIGHLIGHT_SHAPE_NAME = "Text Highlight";
    const std::string Text::TEXT_UNDERLINE_SHAPE_NAME = "Text Underline";

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
                    LastDefaultFontsID(0)
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

    Text::~Text()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
    }
    
    void Text::ComputeCharactersPositionAndSize()
    {
        ssLOG_FUNC_ENTRY();
        if(GetFontsCount() == 0 && GetDefaultFontsCount() == 0)
        {
            ssLOG_FUNC_EXIT();
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
            ssLOG_FUNC_EXIT();
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

        ssLOG_FUNC_EXIT();
    }
    
    void Text::SetText(std::wstring text)
    {
        RecalculateTextNeeded = true;

        CurrentCharactersDetails.Clear();
        ProcessedCharacterDetails.clear();
        std::vector<ssGUI::CharacterDetails> newDetails;
        ConstructCharacterDetails(text, newDetails);
        AddCharacterDetails(newDetails);
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
        AddCharacterDetails(newDetails);
        RedrawObject();
    }

    void Text::AddText(std::wstring text)
    {
        std::vector<ssGUI::CharacterDetails> newDetails;
        ConstructCharacterDetails(text, newDetails);
        AddCharacterDetails(newDetails);
        RedrawObject();
    }

    void Text::AddText(std::string text)
    {
        std::vector<ssGUI::CharacterDetails> newDetails;
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring currentText = converter.from_bytes(text);
        ConstructCharacterDetails(currentText, newDetails);
        AddCharacterDetails(newDetails);
        RedrawObject();
    }

    void Text::AddText(std::wstring text, int index)
    {
        std::vector<ssGUI::CharacterDetails> newDetails;
        ConstructCharacterDetails(text, newDetails);
        AddCharacterDetails(index, newDetails);
        RedrawObject();
    }

    void Text::AddText(std::string text, int index)
    {
        std::vector<ssGUI::CharacterDetails> newDetails;
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring currentText = converter.from_bytes(text);
        ConstructCharacterDetails(currentText, newDetails);
        AddCharacterDetails(index, newDetails);
        RedrawObject();
    }

    void Text::RemoveText(int startIndex, int exclusiveEndIndex)
    {
        RemoveCharacterDetails(startIndex, exclusiveEndIndex);
        RedrawObject();
    }
    
    void Text::GetText(std::wstring& retText) const
    {
        for(int i = 0; i < CurrentCharactersDetails.Size(); i++)
            retText += CurrentCharactersDetails[i].Character;
    }
    
    void Text::GetText(std::string& retText) const
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    
        for(int i = 0; i < CurrentCharactersDetails.Size(); i++)
            retText += converter.to_bytes(CurrentCharactersDetails[i].Character);
    }
    
    std::wstring Text::GetText() const
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
    
    void Text::SetCharacterDetails(int index, ssGUI::CharacterDetails details)
    {
        if(index < 0 || index >= CurrentCharactersDetails.Size())
            return;

        CurrentCharactersDetails[index] = details;

        RecalculateTextNeeded = true;
        RedrawObject();
    }

    ssGUI::CharacterDetails Text::GetCharacterDetails(int index)
    {
        if(index < 0 || index >= CurrentCharactersDetails.Size())
            return ssGUI::CharacterDetails();
        
        return CurrentCharactersDetails[index];
    }

    int Text::GetCharactersDetailsCount() const
    {
        return CurrentCharactersDetails.Size();
    }

    void Text::AddCharacterDetails(int index, ssGUI::CharacterDetails details)
    {
        if(index < 0 || index > CurrentCharactersDetails.Size())
            return;

        CurrentCharactersDetails.Add(details, index);

        RecalculateTextNeeded = true;
        RedrawObject();
    }

    void Text::AddCharacterDetails(ssGUI::CharacterDetails details)
    {
        CurrentCharactersDetails.Add(details);
        RecalculateTextNeeded = true;
        RedrawObject();
    }
    
    void Text::AddCharacterDetails(int index, std::vector<ssGUI::CharacterDetails>& details)
    {
        if(index < 0 || index > CurrentCharactersDetails.Size())
            return;

        CurrentCharactersDetails.Add(details, index);

        RecalculateTextNeeded = true;
        RedrawObject();
    }

    void Text::AddCharacterDetails(std::vector<ssGUI::CharacterDetails>& details)
    {
        CurrentCharactersDetails.Add(details);
    }

    void Text::RemoveCharacterDetails(int index)
    {
        if(index < 0 || index >= CurrentCharactersDetails.Size())
            return;

        CurrentCharactersDetails.Remove(index);
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    void Text::RemoveCharacterDetails(int startIndex, int exclusiveEndIndex)
    {
        if(startIndex < 0 || startIndex >= CurrentCharactersDetails.Size())
            return;

        if(exclusiveEndIndex < 0 || exclusiveEndIndex > CurrentCharactersDetails.Size())
            return;

        CurrentCharactersDetails.Remove(startIndex, exclusiveEndIndex);
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    void Text::ClearAllCharacterDetails()
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
                CharactersRenderInfos[index].BaselinePosition + CharactersRenderInfos[index].DrawOffset :
                CharactersRenderInfos[index].BaselinePosition);
    }

    bool Text::IsOverflow() const
    {
        return Overflow;
    }

    void Text::SetNewTextFontSize(float size)
    {
        FontSize = size;
    }

    float Text::GetNewTextFontSize() const
    {
        return FontSize;
    }

    void Text::SetNewTextColor(glm::u8vec4 color)
    {
        TextColor = color;
    }

    glm::u8vec4 Text::GetNewTextColor() const
    {
        return TextColor;
    } 

    void Text::SetNewTextUnderlined(bool underline)
    {
        TextUnderline = underline;
    }

    bool Text::IsNewTextUnderlined() const
    {
        return TextUnderline;
    }

    void Text::ApplyNewTextSettingsToExistingText()
    {
        RecalculateTextNeeded = true;
        
        for(size_t i = 0; i < CurrentCharactersDetails.Size(); i++)
        {
            CurrentCharactersDetails[i].FontSize = GetNewTextFontSize();
            CurrentCharactersDetails[i].CharacterColor = GetNewTextColor();
            CurrentCharactersDetails[i].Underlined = IsNewTextUnderlined();
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

    void Text::SetTextHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal align)
    {
        CurrentHorizontalAlignment = align;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    ssGUI::Enums::AlignmentHorizontal Text::GetTextHorizontalAlignment() const
    {
        return CurrentHorizontalAlignment;
    }

    void Text::SetTextVerticalAlignment(ssGUI::Enums::AlignmentVertical align)
    {
        CurrentVerticalAlignment = align;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    ssGUI::Enums::AlignmentVertical Text::GetTextVerticalAlignment() const
    {
        return CurrentVerticalAlignment;
    }
    
    void Text::SetTextAlignment(ssGUI::Enums::AlignmentHorizontal hori, ssGUI::Enums::AlignmentVertical vert)
    {
        CurrentHorizontalAlignment = hori;
        CurrentVerticalAlignment = vert;
        RecalculateTextNeeded = true;
        RedrawObject();    
    }

    void Text::AddFont(ssGUI::Font* font)
    {
        if(font == nullptr)
            return;

        if(IsEventCallbackExist(ssGUI::Enums::EventType::BEFORE_FONT_CHANGE))
            GetEventCallback(ssGUI::Enums::EventType::BEFORE_FONT_CHANGE)->Notify(this);
        
        CurrentFonts.push_back(font);
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    void Text::AddFont(ssGUI::Font* font, int index)
    {
        if(font == nullptr)
            return;
        
        if(IsEventCallbackExist(ssGUI::Enums::EventType::BEFORE_FONT_CHANGE))
            GetEventCallback(ssGUI::Enums::EventType::BEFORE_FONT_CHANGE)->Notify(this);
        
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

        if(font != CurrentFonts[index] && IsEventCallbackExist(ssGUI::Enums::EventType::BEFORE_FONT_CHANGE))
            GetEventCallback(ssGUI::Enums::EventType::BEFORE_FONT_CHANGE)->Notify(this);

        CurrentFonts[index] = font;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    void Text::RemoveFont(int index)
    {
        if(index < 0 || index >= CurrentFonts.size())
            return;

        if(IsEventCallbackExist(ssGUI::Enums::EventType::BEFORE_FONT_CHANGE))
            GetEventCallback(ssGUI::Enums::EventType::BEFORE_FONT_CHANGE)->Notify(this);

        CurrentFonts.erase(CurrentFonts.begin() + index);
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    int Text::GetFontsCount() const
    {
        return CurrentFonts.size();
    }

    void Text::SetTextHorizontalPadding(float padding)
    {
        HorizontalPadding = padding;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    float Text::GetTextHorizontalPadding() const
    {
        return HorizontalPadding;
    }

    void Text::SetTextVerticalPadding(float padding)
    {
        VerticalPadding = padding;
        RecalculateTextNeeded = true;
        RedrawObject();
    }

    float Text::GetTextVerticalPadding() const
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
        RedrawObject();
    }

    glm::u8vec4 Text::GetSelectionColor() const
    {
        return SelectionColor;
    }

    void Text::SetTextSelectedColor(glm::u8vec4 color)
    {
        TextSelectedColor = color;
        RedrawObject();
    }

    glm::u8vec4 Text::GetTextSelectedColor() const
    {
        return TextSelectedColor;
    }
    
    void Text::SetDeselectWhenFocusLost(bool deselectWhenFocusLost)
    {
        DeselectWhenFocusLost = deselectWhenFocusLost;
    }
    
    bool Text::IsDeselectWhenFocusLost() const
    {
        return DeselectWhenFocusLost;
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
            if(endIndex < startIndex)
                return -1;

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
            if(checkWithin(pos, globalPos + midInfo.BaselinePosition + midInfo.DrawOffset, midInfo.Size))
            {
                return midIndex;
            }

            if(endIndex <= startIndex)
                return -1;

            ssGUI::CharacterRenderInfo& startInfo = CharactersRenderInfos[startIndex];
            ssGUI::CharacterRenderInfo& endInfo = CharactersRenderInfos[endIndex];

            //Check if check position is contained
            if(pos.y < globalPos.y + startInfo.LineMinY ||
                pos.y > globalPos.y + endInfo.BaselinePosition.y)
            {
                return -1;
            }

            //Check if pos is before midChar or after
            //If above this line
            if(pos.y < globalPos.y + midInfo.BaselinePosition.y + midInfo.LineMinY)
                endIndex = midIndex - 1;

            //If below this line
            else if(pos.y > globalPos.y + midInfo.BaselinePosition.y + midInfo.LineMaxY)
                startIndex = midIndex + 1;

            //If within this line
            else
            {
                //Before this character
                if(pos.x < globalPos.x + midInfo.BaselinePosition.x + midInfo.DrawOffset.x)
                    endIndex = midIndex - 1;

                //After this character
                else if(pos.x > globalPos.x + midInfo.BaselinePosition.x + midInfo.DrawOffset.x)
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
                if(pos.y > globalPos.y + midInfo.BaselinePosition.y + midInfo.LineMaxY)
                    return endIndex;
                
                if(!useLeftEdge)
                {
                    return glm::distance2(pos, globalPos + midInfo.BaselinePosition + midInfo.DrawOffset + midInfo.Size * 0.5f) < 
                            glm::distance2(pos, globalPos + endInfo.BaselinePosition + endInfo.DrawOffset + endInfo.Size * 0.5f) ?
                            midIndex : endIndex;
                }
                else
                {
                    return glm::distance2(pos, globalPos + midInfo.BaselinePosition + midInfo.DrawOffset + glm::vec2(0, midInfo.Size.y * 0.5f)) < 
                            glm::distance2(pos, globalPos + endInfo.BaselinePosition + endInfo.DrawOffset + glm::vec2(0, endInfo.Size.y * 0.5f)) ?
                            midIndex : endIndex;
                }
            }

            if(endIndex == midIndex)
            {
                //If above the line, just go for the startIndex character
                if(pos.y < globalPos.y + midInfo.BaselinePosition.y + midInfo.LineMinY)
                    return startIndex;
                
                if(!useLeftEdge)
                {
                    return glm::distance2(pos, globalPos + midInfo.BaselinePosition + midInfo.DrawOffset + midInfo.Size * 0.5f) < 
                            glm::distance2(pos, globalPos + startInfo.BaselinePosition + startInfo.DrawOffset + startInfo.Size * 0.5f) ?
                            midIndex : startIndex;
                }
                else
                {
                    return glm::distance2(pos, globalPos + midInfo.BaselinePosition + midInfo.DrawOffset + glm::vec2(0, midInfo.Size.y * 0.5f)) < 
                            glm::distance2(pos, globalPos + startInfo.BaselinePosition + startInfo.DrawOffset + glm::vec2(0, startInfo.Size.y * 0.5f)) ?
                            midIndex : startIndex;
                }
            }

            //Check if pos is before midChar or after
            //If above this line
            if(pos.y < globalPos.y + midInfo.BaselinePosition.y + midInfo.LineMinY)
            {
                endIndex = midIndex;
            }
            //If below this line
            else if(pos.y > globalPos.y + midInfo.BaselinePosition.y + midInfo.LineMaxY)
            {
                startIndex = midIndex;
            }
            //If within this line
            else
            {
                //Before this character
                if(pos.x < globalPos.x + midInfo.BaselinePosition.x + midInfo.DrawOffset.x)
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
        if(pos.y >= GetGlobalPosition().y + CharactersRenderInfos[lastIndex].BaselinePosition.y + CharactersRenderInfos[lastIndex].LineMinY &&
            pos.y <= GetGlobalPosition().y + CharactersRenderInfos[lastIndex].BaselinePosition.y + CharactersRenderInfos[lastIndex].LineMaxY)
        {
            //If newline, it will be pointing to character before it
            if(GetInternalCharacterDetail(lastIndex).Character == '\n')
                return false;
            //Otherwise check if the cursor is to the right of the character
            else if(pos.x > GetGlobalPosition().x + CharactersRenderInfos[lastIndex].BaselinePosition.x + CharactersRenderInfos[lastIndex].Advance)
                return true;
            else
                return false;
        }
        //If cursor is after this line
        else if(pos.y > GetGlobalPosition().y + CharactersRenderInfos[lastIndex].BaselinePosition.y + CharactersRenderInfos[lastIndex].LineMaxY)
            return true;
        else
            return false;
    }

    ssGUI::Font* Text::AddDefaultFont()
    {
        ssGUI::Font* font = ssGUI::Factory::Create<ssGUI::Font>();
        DefaultFonts.push_back(font);
        DefaultFontsChangeID++;
        return font;
    }

    ssGUI::Font* Text::AddDefaultFont(int index)
    {
        ssGUI::Font* font = ssGUI::Factory::Create<ssGUI::Font>();
        DefaultFonts.insert(DefaultFonts.begin() + index, font);
        DefaultFontsChangeID++;
        return font;
    }
    
    ssGUI::Font* Text::GetDefaultFont(int index)
    {
        if(index < 0 || index >= DefaultFonts.size())
            return nullptr;
        
        return DefaultFonts[index];
    }

    void Text::RemoveDefaultFont(int index)
    {
        if(index < 0 || index >= DefaultFonts.size())
            return;

        ssGUI::Factory::Dispose(DefaultFonts[index]);
        DefaultFonts.erase(DefaultFonts.begin() + index);
        DefaultFontsChangeID++;
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
        ssLOG_FUNC_ENTRY();
        Text* temp = new Text(*this);
        CloneExtensionsAndEventCallbacks(temp);
        
        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
            {
                ssLOG_FUNC_EXIT();
                return nullptr;
            }
        }

        ssLOG_FUNC_EXIT();
        return temp;
    }
    
    void Text::InitiateDefaultResources()
    {
        ssLOG_FUNC_ENTRY();

        if(!DefaultFonts.empty())
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        const uint8_t* fileContent = NotoSans_Regular;
        size_t fileSize = NotoSans_Regular_size;

        if(fileContent == nullptr)
        {
            ssGUI_ERROR(ssGUI_GUI_OBJECT_TAG, "Failed to load embedded font");
            ssLOG_FUNC_EXIT();
            return;
        }

        auto font = new ssGUI::Font();
        if(!font->GetBackendFontInterface()->LoadFromMemory(const_cast<uint8_t*>(fileContent), fileSize))
        {
            ssGUI_ERROR(ssGUI_GUI_OBJECT_TAG, "Failed to load default font");
            ssGUI::Factory::Dispose(font);
        }
        else
        {
            DefaultFonts.push_back(font);
            DefaultFontsChangeID++;
        }
        ssLOG_FUNC_EXIT();
    }
    
    void Text::CleanUpDefaultResources()
    {
        if(DefaultFonts.empty())
            return;
        
        for(int i = 0; i < DefaultFonts.size(); i++)
            ssGUI::Dispose(DefaultFonts[i]);
        
        DefaultFonts.clear();
        DefaultFontsChangeID++;
    }
}