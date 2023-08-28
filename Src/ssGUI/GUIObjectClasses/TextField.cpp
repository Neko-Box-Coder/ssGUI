#include "ssGUI/GUIObjectClasses/TextField.hpp"

#include "ssGUI/Extensions/RoundedCorners.hpp"
#include "ssGUI/Extensions/Outline.hpp"
#include "ssGUI/Extensions/BoxShadow.hpp"

#include "ssGUI/Factory.hpp"
#include "ssGUI/HeaderGroups/InputGroup.hpp"
#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position
#include "ssGUI/HeaderGroups/InputGroup.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{
    TextField::TextField(TextField const& other) : Text(other)
    {
        LastBlinkTime = other.LastBlinkTime;
        BlinkDuration = other.BlinkDuration;
        BlinkCaret = other.BlinkCaret;
        LastStartSelectionIndex = other.LastStartSelectionIndex;
        LastEndSelectionIndex = other.LastEndSelectionIndex;
        CaretPosition = other.CaretPosition;
        
        LastArrowNavStartTime = other.LastArrowNavStartTime;
        ArrowNavPauseDuration = other.ArrowNavPauseDuration;
        LastArrowNavTime = other.LastArrowNavTime;
        ArrowNavInterval = other.ArrowNavInterval;
    
        LastInputTime = other.LastInputTime;
        FinishedChangingNotified = other.FinishedChangingNotified;
        LastIsFocused = other.LastIsFocused;
        FinishedChangingTimeThresholdInMs = other.FinishedChangingTimeThresholdInMs;
    }

    void TextField::TextInputUpdate(std::wstring& textInput, bool& refreshBlinkTimer, bool& wordMode)
    {
        int insertIndex = GetStartSelectionIndex() > GetEndSelectionIndex() ? GetStartSelectionIndex() : GetEndSelectionIndex();
        insertIndex = (insertIndex >= 0 && insertIndex <= GetText().size()) ? insertIndex : 0;

        ssGUI::CharacterDetails baseCD;
        if(insertIndex - 1 >= 0)
            baseCD = GetCharacterDetails(insertIndex - 1);
        else if(insertIndex + 1 < GetCharactersDetailsCount())
            baseCD = GetCharacterDetails(insertIndex + 1);
        else
        {
            baseCD.CharacterColor = GetNewTextColor();
            baseCD.DefaultFontIndex = GetFontsCount() == 0 ? 0 : -1;
            baseCD.FontIndex = GetFontsCount() == 0 ? -1 : 0;
            baseCD.FontSize = GetNewTextFontSize();
            baseCD.Underlined = IsNewTextUnderlined();
        }

        int charCount = 0;
        bool alterText = false;

        //Add input characters
        for(int i = 0; i < textInput.size(); i++)
        {
            auto curChar = (int32_t)textInput.at(i);

            //Non control characters
            if((curChar > 31 && curChar < 127) || curChar > 159)
            {
                ssGUI::CharacterDetails cd = baseCD;
                cd.Character = textInput.at(i);

                AddCharacterDetails(insertIndex, cd);
                insertIndex++;
                charCount++;
                alterText = true;
            }
            //Control characters
            else
            {
                switch(curChar) 
                {
                    case 8:     //Backspace
                        if(insertIndex > 0 && GetStartSelectionIndex() == GetEndSelectionIndex() && GetStartSelectionIndex() >= 0)
                        {
                            if(wordMode)
                            {
                                int indexOfPrevWord = GetEndOfPreviousWord(insertIndex);
                                RemoveCharacterDetails(indexOfPrevWord, insertIndex);
                                charCount -= insertIndex - indexOfPrevWord;
                                insertIndex -= insertIndex - indexOfPrevWord;
                            }
                            else
                            {
                                RemoveCharacterDetails(--insertIndex);
                                charCount--;
                            }
                        }
                        alterText = true;
                        break;
                    case 9:     //Tab
                    {
                        ssGUI::CharacterDetails cd = baseCD;
                        cd.Character = '\t';
                        AddCharacterDetails(insertIndex, cd);
                        insertIndex++;
                        charCount++;
                        alterText = true;
                        break;
                    }
                    case 10:    //Newline or Line Feed
                    {
                        ssGUI::CharacterDetails cd = baseCD;
                        cd.Character = '\n';
                        AddCharacterDetails(insertIndex, cd);
                        insertIndex++;
                        charCount++;
                        alterText = true;
                        break;
                    }
                    case 12:    //Form Feed (Ctrl Enter if supported)
                    {
                        ssGUI::CharacterDetails cd = baseCD;
                        cd.Character = '\n';
                        AddCharacterDetails(insertIndex, cd);
                        insertIndex++;
                        charCount++;
                        alterText = true;
                        break;
                    }
                    case 13:    //Carriage Return (Only this character is inserted when Enter is pressed, at least on Linux for whatever reason)
                    {
                        ssGUI::CharacterDetails cd = baseCD;
                        cd.Character = '\n';
                        AddCharacterDetails(insertIndex, cd);
                        insertIndex++;
                        charCount++;
                        alterText = true;
                        break;
                    }
                    case 27:    //Escape
                        SetStartSelectionIndex(GetEndSelectionIndex());
                        break;
                    case 127:   //Delete
                        if(insertIndex < GetCharactersDetailsCount() && GetStartSelectionIndex() == GetEndSelectionIndex() && 
                            GetStartSelectionIndex() >= 0)
                        {
                            if(wordMode)
                            {
                                int indexOfNextWord = GetEndOfNextWord(insertIndex);
                                RemoveCharacterDetails(insertIndex, indexOfNextWord);
                            }
                            else
                            {
                                RemoveCharacterDetails(insertIndex);
                            }
                        }
                        
                        alterText = true;
                        break;
                    case 133:   //Next line
                        //TODO: Maybe support this? Go to next line
                        break;
                }
            }
        }

        //If the text is changed, remove any hightlighted text and move caret accordingly
        if(alterText)
        {
            //Removes the text if there's any highlighted
            if(GetStartSelectionIndex() != GetEndSelectionIndex())
            {                
                if(GetStartSelectionIndex() < GetEndSelectionIndex())
                {
                    RemoveCharacterDetails(GetStartSelectionIndex(), GetEndSelectionIndex());
                    SetEndSelectionIndex(GetStartSelectionIndex());
                }
                else
                {
                    RemoveCharacterDetails(GetEndSelectionIndex(), GetStartSelectionIndex());
                    SetStartSelectionIndex(GetEndSelectionIndex());
                }
            }

            if(GetStartSelectionIndex() == -1)
            {
                SetStartSelectionIndex(0);
                SetEndSelectionIndex(0);
            }
            SetEndSelectionIndex(GetEndSelectionIndex() + charCount);
            SetStartSelectionIndex(GetEndSelectionIndex());
            refreshBlinkTimer = true;
        }

        RedrawObject();
        RecalculateTextNeeded = true;
    }

    void TextField::CaretNavigationUpdate(ssGUI::Backend::BackendSystemInputInterface* inputInterface, bool& refreshBlinkTimer, bool& blockKeys, bool& wordMode)
    {
        //Holding shift to enable selection mode
        bool selectionMode =    inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::LEFT_SHIFT) || 
                                inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::RIGHT_SHIFT);
        
        //Arrow navigation
        if(GetStartSelectionIndex() >= 0 && GetEndSelectionIndex() >= 0)
        {
            auto getNextValidCharacterIndex = [this](int startPos, int direction) -> int
            {
                do
                {
                    if(startPos <= 0 && direction < 0)
                        return startPos;
                    
                    if(startPos >= CurrentCharactersDetails.Size() && direction > 0)
                        return CurrentCharactersDetails.Size() - 1;

                    startPos = startPos + direction;
                }
                while(!GetCharacterRenderInfo(startPos).Valid);
                return startPos;
            };
        
            if(inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::LEFT))
            {
                auto moveLeft = [&]()
                {
                    int leftMostIndex = -1;
                
                    if(!selectionMode)
                    {
                        if(GetStartSelectionIndex() <= GetEndSelectionIndex() && GetStartSelectionIndex() > GetFirstValidCharacterIndex())
                            leftMostIndex = GetStartSelectionIndex();
                        else if(GetEndSelectionIndex() <= GetStartSelectionIndex() && GetEndSelectionIndex() > GetFirstValidCharacterIndex())
                            leftMostIndex = GetEndSelectionIndex();
                        else
                            return;
                    }
                    else
                        leftMostIndex = GetEndSelectionIndex();

                    if(!wordMode)
                        SetEndSelectionIndex(getNextValidCharacterIndex(leftMostIndex, -1));
                    else
                        SetEndSelectionIndex(GetEndOfPreviousWord(leftMostIndex));
                    
                    if(!selectionMode)
                        SetStartSelectionIndex(GetEndSelectionIndex());
                };

                //When the user first press the left arrow key                
                if(!inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::SystemKey::LEFT))
                {
                    LastArrowNavStartTime = inputInterface->GetElapsedTime();
                    moveLeft();
                }
                //When the user is holding down the left arrow key
                else if(inputInterface->GetElapsedTime() - LastArrowNavStartTime > ArrowNavPauseDuration &&
                        inputInterface->GetElapsedTime() - LastArrowNavTime > ArrowNavInterval)
                {
                    LastArrowNavTime = inputInterface->GetElapsedTime();
                    moveLeft();
                }

                refreshBlinkTimer = true;
                blockKeys = true;
            }
            else if(inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::RIGHT))
            {
                auto moveRight = [&]()
                {
                    int rightMostIndex = -1;

                    if(!selectionMode)
                    {
                        if(GetStartSelectionIndex() >= GetEndSelectionIndex() && GetStartSelectionIndex() + 1 <= GetCharactersDetailsCount())
                            rightMostIndex = GetStartSelectionIndex();
                        else if(GetEndSelectionIndex() >= GetStartSelectionIndex() && GetEndSelectionIndex() + 1 <= GetCharactersDetailsCount())
                            rightMostIndex = GetEndSelectionIndex();
                        else
                            return;
                    }                    
                    else
                        rightMostIndex = GetEndSelectionIndex();
                    
                    if(!wordMode)
                        SetEndSelectionIndex(getNextValidCharacterIndex(rightMostIndex, 1));
                    else
                        SetEndSelectionIndex(GetEndOfNextWord(rightMostIndex));
                
                    if(!selectionMode)
                        SetStartSelectionIndex(GetEndSelectionIndex());
                };

                //When the user first press the right arrow key                
                if(!inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::SystemKey::RIGHT))
                {
                    LastArrowNavStartTime = inputInterface->GetElapsedTime();
                    moveRight();
                }
                //When the user is holding down the right arrow key
                else if(inputInterface->GetElapsedTime() - LastArrowNavStartTime > ArrowNavPauseDuration &&
                        inputInterface->GetElapsedTime() - LastArrowNavTime > ArrowNavInterval)
                {
                    LastArrowNavTime = inputInterface->GetElapsedTime();
                    moveRight();
                }
                
                refreshBlinkTimer = true;
                blockKeys = true;
            }
            
            if(inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::UP))
            {
                auto moveUp = [&]()
                {
                    int upMostIndex = -1;

                    if(!selectionMode)
                    {
                        if(GetStartSelectionIndex() <= GetEndSelectionIndex())
                            upMostIndex = GetStartSelectionIndex();
                        else
                            upMostIndex = GetEndSelectionIndex();
                    }
                    else
                        upMostIndex = GetEndSelectionIndex();
    
                    SetEndSelectionIndex(GetPositionForPreviousLine(upMostIndex));
                    if(!selectionMode)
                        SetStartSelectionIndex(GetEndSelectionIndex());
                };

                //When the user first press the up arrow key                
                if(!inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::SystemKey::UP))
                {
                    LastArrowNavStartTime = inputInterface->GetElapsedTime();
                    moveUp();
                }
                //When the user is holding down the up arrow key
                else if(inputInterface->GetElapsedTime() - LastArrowNavStartTime > ArrowNavPauseDuration &&
                        inputInterface->GetElapsedTime() - LastArrowNavTime > ArrowNavInterval)
                {
                    LastArrowNavTime = inputInterface->GetElapsedTime();
                    moveUp();
                }
                
                refreshBlinkTimer = true;
                blockKeys = true;
            }
            else if(inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::DOWN))
            {
                auto moveDown = [&]()
                {
                    int downMostIndex = -1;
                    
                    if(!selectionMode)
                    {
                        if(GetStartSelectionIndex() >= GetEndSelectionIndex())
                            downMostIndex = GetStartSelectionIndex();
                        else
                            downMostIndex = GetEndSelectionIndex();
                    }
                    else
                        downMostIndex = GetEndSelectionIndex();
                    
                    SetEndSelectionIndex(GetPositionForNextLine(downMostIndex));
                    if(!selectionMode)
                        SetStartSelectionIndex(GetEndSelectionIndex());
                };

                //When the user first press the right down key                
                if(!inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::SystemKey::DOWN))
                {
                    LastArrowNavStartTime = inputInterface->GetElapsedTime();
                    moveDown();
                }
                //When the user is holding down the down arrow key
                else if(inputInterface->GetElapsedTime() - LastArrowNavStartTime > ArrowNavPauseDuration &&
                        inputInterface->GetElapsedTime() - LastArrowNavTime > ArrowNavInterval)
                {
                    LastArrowNavTime = inputInterface->GetElapsedTime();
                    moveDown();
                }
                
                refreshBlinkTimer = true;
                blockKeys = true;
            }
        }
    }
    
    //You only need to override this when you are rendering anything.
    void TextField::ConstructRenderInfo()
    {
        ssGUI_LOG_FUNC();
        ssGUI::Text::ConstructRenderInfo();

        int lastValidIndex = GetLastValidCharacterIndex();
        glm::vec2 drawPos = GetGlobalPosition();
        float height = 0;
        float caretWidth = CaretPosition < 0 || CaretPosition >= GetCharactersDetailsCount() ? 
            GetNewTextFontSize() / 15.f : 
            GetCharacterDetails(CaretPosition).FontSize / 15.f;

        if(CaretPosition >= 0 && lastValidIndex >= 0)
        {
            if(CaretPosition > lastValidIndex)
            {
                //Last character
                if(GetCharacterDetails(lastValidIndex).Character != '\n')
                {
                    drawPos += GetCharacterRenderInfo(lastValidIndex).BaselinePosition + 
                        glm::vec2(GetCharacterRenderInfo(lastValidIndex).Advance, 
                        GetCharacterRenderInfo(lastValidIndex).LineMinY);
                    
                    height = GetCharacterRenderInfo(lastValidIndex).LineMaxY - 
                        GetCharacterRenderInfo(lastValidIndex).LineMinY;
                }
                //If caret is pointing Newline, go to next line
                if(GetCharacterDetails(lastValidIndex).Character == '\n')
                {
                    const ssGUI::CharacterDetails& curDetail = GetInternalCharacterDetail(lastValidIndex);
                    ssGUI::Backend::BackendFontInterface* fontInterface = nullptr;
            
                    if(curDetail.FontIndex != -1)
                        fontInterface = GetFont(curDetail.FontIndex)->GetBackendFontInterface();
                    else
                        fontInterface = GetDefaultFont(curDetail.DefaultFontIndex)->GetBackendFontInterface();
                    
                    auto horiAlignment = GetTextHorizontalAlignment(); 
                    if(horiAlignment == ssGUI::Enums::AlignmentHorizontal::LEFT)
                    {
                        drawPos.x += GetTextHorizontalPadding();
                    }
                    else if(horiAlignment == ssGUI::Enums::AlignmentHorizontal::CENTER)
                    {
                        drawPos.x += GetSize().x / 2;
                    }
                    else
                    {
                        drawPos.x += GetSize().x - GetTextHorizontalPadding();
                    }

                    drawPos.y += GetCharacterRenderInfo(lastValidIndex).BaselinePosition.y + 
                        GetCharacterRenderInfo(lastValidIndex).LineMinY +
                        fontInterface->GetLineSpacing(curDetail.FontSize) + GetLineSpace();
                    
                    height = GetCharacterRenderInfo(lastValidIndex).LineMaxY - 
                        GetCharacterRenderInfo(lastValidIndex).LineMinY;
                }
            }
            else
            {
                drawPos += GetCharacterRenderInfo(CaretPosition).BaselinePosition + 
                    glm::vec2(0, GetCharacterRenderInfo(CaretPosition).LineMinY);
                
                height = GetCharacterRenderInfo(CaretPosition).LineMaxY - 
                    GetCharacterRenderInfo(CaretPosition).LineMinY;
            }

            //Draw caret
            if(BlinkCaret || !IsInteractable() || !IsFocused())
                return;

            ssGUI::DrawingEntity caretEntity;
            if(CaretPosition > GetStartSelectionIndex())
            {
                caretEntity.Vertices.push_back(drawPos);
                caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));

                caretEntity.Vertices.push_back(drawPos + glm::vec2(caretWidth, 0));
                caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));

                caretEntity.Vertices.push_back(drawPos + glm::vec2(caretWidth, height));
                caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));

                caretEntity.Vertices.push_back(drawPos + glm::vec2(0, height));
                caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));
            }
            else
            {
                caretEntity.Vertices.push_back(drawPos + glm::vec2(-caretWidth, 0));
                caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));

                caretEntity.Vertices.push_back(drawPos);
                caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));

                caretEntity.Vertices.push_back(drawPos + glm::vec2(0, height));
                caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));

                caretEntity.Vertices.push_back(drawPos + glm::vec2(-caretWidth, height));
                caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));
            }
            caretEntity.EntityName = TEXTFIELD_CARET_SHAPE_NAME;
            DrawingEntities.push_back(caretEntity);
        }
        else
        {
            //Draw caret
            if(BlinkCaret || !IsInteractable() || !IsFocused())
                return;

            ssGUI::Backend::BackendFontInterface* fontInterface = nullptr;
            
            if(GetFontsCount() > 0)
                fontInterface = GetFont(0)->GetBackendFontInterface();
            else if(GetDefaultFontsCount() > 0)
                fontInterface = GetDefaultFont(0)->GetBackendFontInterface();
            else
                return;

            height = fontInterface->GetLineSpacing(GetNewTextFontSize()) + GetLineSpace();
            
            switch(GetTextHorizontalAlignment()) 
            {
                case ssGUI::Enums::AlignmentHorizontal::LEFT:
                    drawPos.x += GetTextHorizontalPadding();
                    break;
                case ssGUI::Enums::AlignmentHorizontal::CENTER:
                    drawPos.x += GetSize().x / 2;
                    break;
                case ssGUI::Enums::AlignmentHorizontal::RIGHT:
                    drawPos.x += GetSize().x - GetTextHorizontalPadding();
                    break;
            }

            switch(GetTextVerticalAlignment()) 
            {
                case ssGUI::Enums::AlignmentVertical::TOP:
                    drawPos.y += GetTextVerticalPadding();
                    break;
                case ssGUI::Enums::AlignmentVertical::CENTER:
                    drawPos.y += GetSize().y / 2 - height / 2;
                    break;
                case ssGUI::Enums::AlignmentVertical::BOTTOM:
                    drawPos.y += GetSize().y - GetNewTextFontSize() - GetTextVerticalPadding();
                    break;
            }

            ssGUI::DrawingEntity caretEntity;

            caretEntity.Vertices.push_back(drawPos);
            caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));

            caretEntity.Vertices.push_back(drawPos + glm::vec2(caretWidth, 0));
            caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));

            caretEntity.Vertices.push_back(drawPos + glm::vec2(caretWidth, height));
            caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));

            caretEntity.Vertices.push_back(drawPos + glm::vec2(0, height));
            caretEntity.Colors.push_back(glm::u8vec4(0, 0, 0, 255));
            caretEntity.EntityName = TEXTFIELD_CARET_SHAPE_NAME;
            DrawingEntities.push_back(caretEntity);
        }
    }
    
    void TextField::MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                                ssGUI::GUIObject* mainWindow)
    {       
        bool blockKeys = false;
        bool refreshBlinkTimer = false;
        bool wordMode = false;
        if(IsInteractable() && IsBlockInput())
        {
            std::wstring textInput = inputInterface->GetTextInput();

            //Holding ctrl to enable word mode
            wordMode =  inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::LEFT_CTRL) || 
                        inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::RIGHT_CTRL);

            //Pasting
            if( inputStatus.KeyInputBlockedObject == nullptr && 
                inputInterface->ClipbaordHasText() && wordMode && 
                inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::LetterKey::V) &&
                !inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::LetterKey::V))
            {
                std::wstring clipboardStr;
                inputInterface->GetClipboardText(clipboardStr);
                textInput += clipboardStr;
            }

            //Text input
            //TODO: Put textInput to a CharacterDetails vector and insert the whole vector instead of 1 by 1
            if(inputStatus.KeyInputBlockedObject == nullptr && IsFocused() && !textInput.empty())
            {
                blockKeys = true;
                TextInputUpdate(textInput, refreshBlinkTimer, wordMode);
            }
            
            //Event callback
            if(!textInput.empty())
            {
                if(IsEventCallbackExist(ssGUI::Enums::EventType::TEXT_FIELD_CONTENT_CHANGED_VIA_GUI))
                    GetEventCallback(ssGUI::Enums::EventType::TEXT_FIELD_CONTENT_CHANGED_VIA_GUI)->Notify(this);
                
                LastInputTime = inputInterface->GetElapsedTime();
                FinishedChangingNotified = false;
            }
            else
            {
                bool notifyFinishedChanging = false;
                
                if(!IsFocused() && LastIsFocused && !FinishedChangingNotified)
                    notifyFinishedChanging = true;
                
                if(inputInterface->GetElapsedTime() - LastInputTime > FinishedChangingTimeThresholdInMs && !FinishedChangingNotified)
                    notifyFinishedChanging = true;

                if(notifyFinishedChanging)
                {
                    FinishedChangingNotified = true;
                    
                    if(IsEventCallbackExist(ssGUI::Enums::EventType::TEXT_FIELD_CONTENT_FINISHED_CHANGING_VIA_GUI))
                        GetEventCallback(ssGUI::Enums::EventType::TEXT_FIELD_CONTENT_FINISHED_CHANGING_VIA_GUI)->Notify(this);
                }
            }
        
            LastIsFocused = IsFocused();
        }

        ssGUI::Text::MainLogic(inputInterface, inputStatus, mainWindow);
        
        if(IsInteractable())
        {
            //Caret navigation
            if(inputStatus.KeyInputBlockedObject == nullptr)
                CaretNavigationUpdate(inputInterface, refreshBlinkTimer, blockKeys, wordMode);

            //Check Carret
            if(GetStartSelectionIndex() != LastStartSelectionIndex)
            {
                CaretPosition = GetStartSelectionIndex();
                refreshBlinkTimer = true;
            }
            else if(GetEndSelectionIndex() != LastEndSelectionIndex)
            {
                CaretPosition = GetEndSelectionIndex();
                refreshBlinkTimer = true;
            }
            
            LastStartSelectionIndex = GetStartSelectionIndex();
            LastEndSelectionIndex = GetEndSelectionIndex();

            //Blinking caret
            if(refreshBlinkTimer)
                LastBlinkTime = inputInterface->GetElapsedTime() - BlinkDuration;

            uint64_t blinkDiff = inputInterface->GetElapsedTime() - LastBlinkTime;
            bool curState = BlinkCaret;
            if(blinkDiff < BlinkDuration)
                BlinkCaret = true;
            else if(blinkDiff < BlinkDuration * 2)
                BlinkCaret = false;
            else
            {
                LastBlinkTime = inputInterface->GetElapsedTime();
                BlinkCaret = true;
            }

            if(BlinkCaret != curState)
                RedrawObject();

            if(blockKeys)
                inputStatus.KeyInputBlockedObject = this;
        }
    }

    const std::string TextField::TEXTFIELD_CARET_SHAPE_NAME = "Text Caret";

    TextField::TextField() :    LastBlinkTime(0),
                                BlinkDuration(500),
                                BlinkCaret(false),
                                LastStartSelectionIndex(-1),
                                LastEndSelectionIndex(-1),
                                CaretPosition(-1),
                                LastArrowNavStartTime(0),
                                ArrowNavPauseDuration(500),
                                LastArrowNavTime(0),
                                ArrowNavInterval(20),
                                LastInputTime(0),
                                FinishedChangingNotified(true),
                                LastIsFocused(false),
                                FinishedChangingTimeThresholdInMs(500)
    {
        SetBlockInput(true);
        SetMinSize(glm::vec2(35, 35));
        SetTextAlignment(ssGUI::Enums::AlignmentHorizontal::LEFT, ssGUI::Enums::AlignmentVertical::TOP);
        
        SetBackgroundColor(glm::ivec4(127, 127, 127, 255));

        AddExtension<ssGUI::Extensions::RoundedCorners>();
        
        AddExtension<ssGUI::Extensions::Outline>()->SetOutlineColor(glm::u8vec4(0, 0, 0, 127));
        
        //Add shadow to window
        auto shadowEx = AddExtension<ssGUI::Extensions::BoxShadow>();
        shadowEx->SetBlurRadius(20);
        shadowEx->SetSizeOffset(glm::vec2(10, 10));
    }

    TextField::~TextField()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
    }

    ssGUI::Enums::GUIObjectType TextField::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::WIDGET | ssGUI::Enums::GUIObjectType::TEXT | ssGUI::Enums::GUIObjectType::TEXT_FIELD;
    }

    TextField* TextField::Clone(bool cloneChildren)
    {
        ssGUI_LOG_FUNC();
        TextField* temp = new TextField(*this);
        CloneExtensionsAndEventCallbacks(temp);   
        
        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
                return nullptr;
        }
        
        return temp;
    }
}