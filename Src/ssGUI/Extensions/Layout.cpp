#include "ssGUI/Extensions/Layout.hpp"

#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/WindowLayoutItemEnforcer.hpp"
#include "ssGUI/ssGUITags.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Extensions
{    
    Layout::Layout() :  HorizontalLayout(false),
                        PreferredSizeMultipliers(),
                        DisableChildrenResizing(false),
                        OverrideChildrenResizeTypesAndOnTop(true),
                        UpdateContainerMinMaxSize(true),
                        ReverseOrder(false),
                        CoverFullLength(true),
                        Container(nullptr),
                        Enabled(true),
                        Padding(0),
                        Spacing(5),
                        Overflow(false),
                        Updated(false),
                        CurrentObjectsReferences(),
                        LastUpdateChildrenSize(),
                        ObjectsToExclude(),
                        SpecialObjectsToExclude(),
                        OriginalChildrenSize(),
                        OriginalChildrenResizeType(),
                        OriginalChildrenOnTop()
    {}

    Layout::~Layout()
    {
        if(Container != nullptr)
        {
            auto eventCallbackCleanUp = [&](ssGUI::GUIObject* target, ssGUI::Enums::EventType eventType)
            {
                if(!target->IsEventCallbackExist(eventType))
                    return;

                target->GetEventCallback(eventType)->RemoveEventListener(EXTENSION_NAME);
            
                if(target->GetEventCallback(eventType)->GetEventListenerCount() == 0)
                    target->RemoveEventCallback(eventType);
            };

            //Revert any changes we made to the children when they got parented            
            SetOverrideChildrenResizeTypeAndOnTop(false);

            eventCallbackCleanUp(Container, ssGUI::Enums::EventType::BEFORE_RECURSIVE_CHILD_ADD);
            eventCallbackCleanUp(Container, ssGUI::Enums::EventType::RECURSIVE_CHILD_ADDED);
            eventCallbackCleanUp(Container, ssGUI::Enums::EventType::RECURSIVE_CHILD_REMOVED);
            eventCallbackCleanUp(Container, ssGUI::Enums::EventType::CHILD_POSITION_CHANGED);
        
            Container->StashChildrenIterator();
            Container->MoveChildrenIteratorToFirst();
            while(!Container->IsChildrenIteratorEnd())
            {
                eventCallbackCleanUp(Container->GetCurrentChild(), ssGUI::Enums::EventType::MIN_MAX_SIZE_CHANGED);
                Container->MoveChildrenIteratorNext();
            }
            Container->PopChildrenIterator();
        }
        CurrentObjectsReferences.CleanUp();
    }    
    
    Layout::Layout(Layout const& other)
    {
        ssGUI_LOG_FUNC();
        Container = nullptr;
        Copy(&other);
    }

    void Layout::LayoutChildren(float startPos, 
                                float length, 
                                std::vector<float>& childrenPos, 
                                std::vector<float>& childrenLength, 
                                std::vector<float>& minChildrenLength, 
                                std::vector<float>& maxChildrenLength, 
                                int lastChildChangeIndex,
                                float sizeDiff)
    {        
        ssGUI_LOG_FUNC();
        
        float endPos = startPos + length;
        float currentPos = IsReverseOrder() ? endPos : startPos;
        float currentLength = 0;
        float remainingLength = length;

        float remainingMinLengths = 0;
        for(auto minLength : minChildrenLength)
            remainingMinLengths += minLength;

        //Exclude spacing in the length calculation
        for(auto temp : childrenLength)
            remainingLength -= GetSpacing();
        
        //Don't need to exclude spacing for the last element, therefore adding it back 
        remainingLength += GetSpacing();

        float lengthWithoutPaddingAndSpacing = remainingLength;

        //Change the children after the last size change child to fit the layout container
        if(lastChildChangeIndex != -1 && ++lastChildChangeIndex <= childrenPos.size() - 2)
        {
            for(int i = lastChildChangeIndex; i < childrenLength.size(); i++)
            {
                if(childrenLength[i] - sizeDiff >= minChildrenLength[i] && 
                    childrenLength[i] - sizeDiff <= maxChildrenLength[i])
                {
                    childrenLength[i] -= sizeDiff;
                    break;
                }
                else if(childrenLength[i] - sizeDiff < minChildrenLength[i])
                {
                    sizeDiff -= (childrenLength[i] - minChildrenLength[i]);
                    childrenLength[i] = minChildrenLength[i];

                }
                else if(childrenLength[i] - sizeDiff > maxChildrenLength[i])
                {
                    sizeDiff -= (childrenLength[i] - maxChildrenLength[i]);
                    childrenLength[i] = maxChildrenLength[i];
                }
            }
        }

        //Calculate length
        for(int i = 0; i < childrenPos.size(); i++)
        {
            float sizeMultiplier = GetPreferredSizeMultiplier(i);

            if(sizeMultiplier > 0)
                currentLength = lengthWithoutPaddingAndSpacing * sizeMultiplier;
            else
                currentLength = childrenLength[i];

            if(!Overflow && remainingLength - currentLength < remainingMinLengths - minChildrenLength[i])
            {
                currentLength = remainingLength - (remainingMinLengths - minChildrenLength[i]);
            }

            childrenLength[i] = currentLength;
            remainingMinLengths -= minChildrenLength[i];
            remainingLength -= currentLength;
        }

        //Reiterate again to cover full length
        if(CoverFullLength && remainingLength > 0 && !Overflow)
        {            
            for(int i = childrenLength.size() - 1; i >= 0; i--)
            {
                if(maxChildrenLength[i] > remainingLength + childrenLength[i])
                {
                    childrenLength[i] += remainingLength;
                    break;
                }
                else
                {
                    float lengthIncrease = maxChildrenLength[i] - childrenLength[i];
                    remainingLength -= lengthIncrease;
                    childrenLength[i] += lengthIncrease;
                }
            }
        }
        
        //Calculate position
        for(int i = 0; i < childrenPos.size(); i++)
        {
            childrenPos[i] = IsReverseOrder() ? currentPos - childrenLength[i] : currentPos;
            currentPos = IsReverseOrder() ? currentPos - childrenLength[i] - GetSpacing() : currentPos + childrenLength[i] + GetSpacing();
        }
    }

    void Layout::UpdateChildrenResizeTypesAndOnTop()
    {
        ssGUI_LOG_FUNC();
        
        if(Container == nullptr)
            return;

        Container->StashChildrenIterator();
        Container->MoveChildrenIteratorToFirst();
        
       ssGUI::Window* lastWindowChild = nullptr; 
        while(!Container->IsChildrenIteratorEnd())
        {
            ssGUIObjectIndex currentChildIndex = CurrentObjectsReferences.GetObjectIndex(Container->GetCurrentChild());

            if(currentChildIndex != -1 && ObjectsToExclude.find(currentChildIndex) != ObjectsToExclude.end())
            {
                Container->MoveChildrenIteratorNext();
                continue;
            }
            
            if( Container->GetCurrentChild()->GetType() != ssGUI::Enums::GUIObjectType::WINDOW || 
                !Container->GetCurrentChild()->IsEnabled())
            {
                Container->MoveChildrenIteratorNext();
                continue;
            }

            //Add the child to reference if not present
            if(currentChildIndex == -1)
                currentChildIndex = CurrentObjectsReferences.AddObjectReference(Container->GetCurrentChild());

            ssGUI::Window* windowP = dynamic_cast<ssGUI::Window*>(Container->GetCurrentChild());
            
            //Recording original resize type and isOnTop and override it
            if(IsOverrideChildrenResizeTypeAndOnTop())
            {
                if(OriginalChildrenResizeType.find(currentChildIndex) == OriginalChildrenResizeType.end())
                {
                    OriginalChildrenResizeType[currentChildIndex] = windowP->GetResizeType();
                    OriginalChildrenOnTop[currentChildIndex] = windowP->IsOnTopWhenFocused();
                }
                
                if(HorizontalLayout)
                {
                    if(ReverseOrder)
                        windowP->SetResizeType(ssGUI::Enums::ResizeType::LEFT);
                    else
                        windowP->SetResizeType(ssGUI::Enums::ResizeType::RIGHT);
                }
                else
                {                    
                    if(ReverseOrder)
                        windowP->SetResizeType(ssGUI::Enums::ResizeType::TOP);
                    else
                        windowP->SetResizeType(ssGUI::Enums::ResizeType::BOTTOM);
                }

                lastWindowChild = windowP;
                windowP->SetOnTopWhenFocused(false);
            }
            
            //Restoring original resize type
            if(!IsOverrideChildrenResizeTypeAndOnTop() && OriginalChildrenResizeType.find(currentChildIndex) != OriginalChildrenResizeType.end())
            {                
                windowP->SetResizeType(OriginalChildrenResizeType[currentChildIndex]);
                windowP->SetOnTopWhenFocused(OriginalChildrenOnTop[currentChildIndex]);

                OriginalChildrenResizeType.erase(currentChildIndex);
                OriginalChildrenOnTop.erase(currentChildIndex);
            }

            Container->MoveChildrenIteratorNext();
        }
        
        //last element of the layout shouldn't be resized so that the container can be resized
        if(IsOverrideChildrenResizeTypeAndOnTop() && lastWindowChild != nullptr)
        {
            if(Container->IsChildrenIteratorLast())
                lastWindowChild->SetResizeType(ssGUI::Enums::ResizeType::NONE);
        }
        
        Container->PopChildrenIterator();
    }

    void Layout::SyncContainerMinMaxSize()
    {
        ssGUI_LOG_FUNC();

        if(Container == nullptr || Container->GetChildrenCount() - ObjectsToExclude.size() == 0)
            return;

        //Check the number of valid children
        Container->StashChildrenIterator();
        Container->MoveChildrenIteratorToFirst();
        int validChildrenSize = 0;
        while(!Container->IsChildrenIteratorEnd())
        {            
            ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(Container->GetCurrentChild());
            
            if((childIndex != -1 && ObjectsToExclude.find(childIndex) != ObjectsToExclude.end()) || !Container->GetCurrentChild()->IsEnabled())
            {
                Container->MoveChildrenIteratorNext();
                continue;
            }

            validChildrenSize++;
            Container->MoveChildrenIteratorNext();
        }
        Container->PopChildrenIterator();

        if(validChildrenSize == 0)
            return;
        
        if(HorizontalLayout)
        {
            float minSizeTotalX = 0;
            float maxSizeTotalX = 0;
            float minMaxY = 9999;
            float maxMinY = -1;
            
            Container->StashChildrenIterator();
            Container->MoveChildrenIteratorToFirst();
            while(!Container->IsChildrenIteratorEnd())
            {
                ssGUIObjectIndex currentChildIndex = CurrentObjectsReferences.GetObjectIndex(Container->GetCurrentChild());
                if( (currentChildIndex != -1 && ObjectsToExclude.find(currentChildIndex) != ObjectsToExclude.end()) || 
                    !Container->GetCurrentChild()->IsEnabled())
                {
                    Container->MoveChildrenIteratorNext();
                    continue;
                }

                //Add the child to reference if not present
                if(currentChildIndex == -1)
                    currentChildIndex = CurrentObjectsReferences.AddObjectReference(Container->GetCurrentChild());
                
                minSizeTotalX += Container->GetCurrentChild()->GetMinSize().x;
                if(!Container->IsChildrenIteratorLast())
                    minSizeTotalX += GetSpacing();

                maxSizeTotalX += Container->GetCurrentChild()->GetMaxSize().x;

                if(Container->GetCurrentChild()->GetMaxSize().y < minMaxY)
                    minMaxY = Container->GetCurrentChild()->GetMaxSize().y;

                if(Container->GetCurrentChild()->GetMinSize().y > maxMinY)
                    maxMinY = Container->GetCurrentChild()->GetMinSize().y;
                
                Container->MoveChildrenIteratorNext();
            }
            Container->PopChildrenIterator();

            if(minMaxY < maxMinY)
                minMaxY = maxMinY;

            float paddingTotalX = GetPadding() * 2;
            float spacingTotalX = (Container->GetChildrenCount() - ObjectsToExclude.size() - 1) * GetSpacing();

            minSizeTotalX += paddingTotalX + spacingTotalX;
            maxSizeTotalX += paddingTotalX + spacingTotalX;

            if( Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && 
                Container->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW && 
                dynamic_cast<ssGUI::Window*>(Container)->HasTitlebar())
            {
                minMaxY += dynamic_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() + GetPadding();
                maxMinY += dynamic_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() + GetPadding();
            }
            else
            {
                minMaxY += GetPadding() * 2;
                maxMinY += GetPadding() * 2;
            }

            if(IsCoverFullLength())
            {
                Container->SetMinSize(glm::vec2(minSizeTotalX, maxMinY));
                Container->SetMaxSize(glm::vec2(maxSizeTotalX, minMaxY));
            }
            else
            {
                Container->SetMinSize(glm::vec2(Container->GetMinSize().x, maxMinY));
                Container->SetMaxSize(glm::vec2(Container->GetMaxSize().x, minMaxY));
            }
        }
        else
        {
            float minSizeTotalY = 0;
            float maxSizeTotalY = 0;
            float minMaxX = 9999;
            float maxMinX = -1;
            
            Container->StashChildrenIterator();
            Container->MoveChildrenIteratorToFirst();
            while(!Container->IsChildrenIteratorEnd())   
            {
                ssGUIObjectIndex currentChildIndex = CurrentObjectsReferences.GetObjectIndex(Container->GetCurrentChild());
                if( (currentChildIndex != -1 && ObjectsToExclude.find(currentChildIndex) != ObjectsToExclude.end()) ||
                    !Container->GetCurrentChild()->IsEnabled())
                {
                    Container->MoveChildrenIteratorNext();
                    continue;
                }

                //Add the child to reference if not present
                if(currentChildIndex == -1)
                    currentChildIndex = CurrentObjectsReferences.AddObjectReference(Container->GetCurrentChild());
                
                minSizeTotalY += Container->GetCurrentChild()->GetMinSize().y;
                if(!Container->IsChildrenIteratorLast())
                    minSizeTotalY += GetSpacing();

                maxSizeTotalY += Container->GetCurrentChild()->GetMaxSize().y;

                if(Container->GetCurrentChild()->GetMaxSize().x < minMaxX)
                    minMaxX = Container->GetCurrentChild()->GetMaxSize().x;

                if(Container->GetCurrentChild()->GetMinSize().x > maxMinX)
                    maxMinX = Container->GetCurrentChild()->GetMinSize().x;

                Container->MoveChildrenIteratorNext();
            }
            Container->PopChildrenIterator();

            if(minMaxX < maxMinX)
                minMaxX = maxMinX;

            float paddingTotalY = 0;
            
            if(Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && Container->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
                paddingTotalY = GetPadding() + dynamic_cast<ssGUI::Window*>(Container)->GetTitlebarHeight();
            else
                paddingTotalY = GetPadding() * 2;

            float spacingTotalY = (Container->GetChildrenCount() - ObjectsToExclude.size() - 1) * GetSpacing();

            minSizeTotalY += paddingTotalY + spacingTotalY;
            maxSizeTotalY += paddingTotalY + spacingTotalY;

            minMaxX += GetPadding() * 2;
            maxMinX += GetPadding() * 2;
            
            if(IsCoverFullLength())
            {
                Container->SetMinSize(glm::vec2(maxMinX, minSizeTotalY));
                Container->SetMaxSize(glm::vec2(minMaxX, maxSizeTotalY));
            }
            else
            {
                Container->SetMinSize(glm::vec2(maxMinX, Container->GetMinSize().y));
                Container->SetMaxSize(glm::vec2(minMaxX, Container->GetMaxSize().y));
            }
        }
    }

    void Layout::UpdateSpeicalObjectsToExclude()
    {
        ssGUI_LOG_FUNC();
        std::vector<ssGUIObjectIndex> objsToRemove;
        for(auto it : SpecialObjectsToExclude)
        {
            //Don't need to hold reference for it as it is not a child
            if( CurrentObjectsReferences.GetObjectReference(it) == nullptr || 
                CurrentObjectsReferences.GetObjectReference(it)->GetParent() != Container)
            {       
                objsToRemove.push_back(it);
                CurrentObjectsReferences.RemoveObjectReference(it);
            }
            //If it needs to be in the layout
            else if(!CurrentObjectsReferences.GetObjectReference(it)->HasTag(ssGUI::Tags::OVERLAY) && 
                    !CurrentObjectsReferences.GetObjectReference(it)->HasTag(ssGUI::Tags::FLOATING) && 
                    CurrentObjectsReferences.GetObjectReference(it)->IsEnabled())
            {
                objsToRemove.push_back(it);
            }
        }

        for(auto objIndex : objsToRemove)
        {
            ObjectsToExclude.erase(objIndex);
            SpecialObjectsToExclude.erase(objIndex);
        }
    }

    void Layout::UpdateExcludedObjects()
    {
        ssGUI_LOG_FUNC();
        Container->StashChildrenIterator();
        Container->MoveChildrenIteratorToFirst();
        while(!Container->IsChildrenIteratorEnd())
        {
            ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(Container->GetCurrentChild());
            
            //Don't update the objects if excluded
            if(childIndex != -1 && ObjectsToExclude.find(childIndex) != ObjectsToExclude.end())
            {
                Container->MoveChildrenIteratorNext();
                continue;
            }
                
            if(childIndex == -1)
                childIndex = CurrentObjectsReferences.AddObjectReference(Container->GetCurrentChild());
            
            if(Container->GetCurrentChild()->HasTag(ssGUI::Tags::OVERLAY) || Container->GetCurrentChild()->HasTag(ssGUI::Tags::FLOATING) || !Container->GetCurrentChild()->IsEnabled())
            {
                ObjectsToExclude.insert(childIndex);
                SpecialObjectsToExclude.insert(childIndex);
                Container->MoveChildrenIteratorNext();
                continue;
            }
            
            Container->MoveChildrenIteratorNext();
        }
        Container->PopChildrenIterator();
    }

    void Layout::DisableChildrenResizingInUpdate()
    {
        ssGUI_LOG_FUNC();
        Container->StashChildrenIterator();
        Container->MoveChildrenIteratorToFirst();
        while(!Container->IsChildrenIteratorEnd())
        {
            ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(Container->GetCurrentChild());
            
            //Don't update the objects if excluded
            if((childIndex != -1 && ObjectsToExclude.find(childIndex) != ObjectsToExclude.end()) ||
                !Container->GetCurrentChild()->IsEnabled())
            {
                Container->MoveChildrenIteratorNext();
                continue;
            }
            
            if(Container->GetCurrentChild()->GetType() == ssGUI::Enums::GUIObjectType::WINDOW)
                dynamic_cast<ssGUI::Window*>(Container->GetCurrentChild())->SetResizeType(ssGUI::Enums::ResizeType::NONE);

            Container->MoveChildrenIteratorNext();
        }
        Container->PopChildrenIterator();
    }

    void Layout::GetAndValidateChildrenDetails( std::vector<float>& childrenPos, 
                                                std::vector<float>& childrenSize, 
                                                std::vector<float>& childrenMinSize,
                                                std::vector<float>& childrenMaxSize, 
                                                glm::vec2 containerPos, 
                                                glm::vec2 containerSize)
    {
        ssGUI_LOG_FUNC();
        Container->StashChildrenIterator();
        Container->MoveChildrenIteratorToFirst();
        while(!Container->IsChildrenIteratorEnd())
        {
            ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(Container->GetCurrentChild());
            
            if( (childIndex != -1 && ObjectsToExclude.find(childIndex) != ObjectsToExclude.end()) ||
                !Container->GetCurrentChild()->IsEnabled())
            {   
                Container->MoveChildrenIteratorNext();
                continue;
            }

            glm::vec2 currentPos = Container->GetCurrentChild()->GetGlobalPosition();
            glm::vec2 currentSize = Container->GetCurrentChild()->GetSize();
            glm::vec2 currentMinSize = Container->GetCurrentChild()->GetMinSize();
            glm::vec2 currentMaxSize = Container->GetCurrentChild()->GetMaxSize();
            
            if(IsHorizontalLayout())
            {
                float verticalPos = Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && 
                                    Container->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW && 
                                    static_cast<ssGUI::Window*>(Container)->HasTitlebar() ? 
                                    containerPos.y + dynamic_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() :
                                    containerPos.y + GetPadding();     

                float verticalSize =    Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && 
                                        Container->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW && 
                                        static_cast<ssGUI::Window*>(Container)->HasTitlebar() ? 
                                        containerSize.y - GetPadding() - dynamic_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() :
                                        containerSize.y - GetPadding() * 2;

                if(currentPos.y != verticalPos)
                    Container->GetCurrentChild()->SetGlobalPosition(glm::vec2(currentPos.x, verticalPos));
            
                if(currentSize.y != verticalSize)
                    Container->GetCurrentChild()->SetSize(glm::vec2(currentSize.x, verticalSize));

                childrenPos.push_back(currentPos.x);
                childrenSize.push_back(currentSize.x);
                childrenMinSize.push_back(currentMinSize.x);
                childrenMaxSize.push_back(currentMaxSize.x);  
            }
            else
            {
                if(currentPos.x != containerPos.x + GetPadding())
                    Container->GetCurrentChild()->SetGlobalPosition(glm::vec2(containerPos.x + GetPadding(), currentPos.y));
            
                if(currentSize.x != containerSize.x - GetPadding() * 2)
                    Container->GetCurrentChild()->SetSize(glm::vec2(containerSize.x - GetPadding() * 2, currentSize.y));

                childrenPos.push_back(currentPos.y);
                childrenSize.push_back(currentSize.y);
                childrenMinSize.push_back(currentMinSize.y);
                childrenMaxSize.push_back(currentMaxSize.y);
            }

            Container->MoveChildrenIteratorNext();
        }
        Container->PopChildrenIterator();
    }

    void Layout::GetLastDifferentChild(std::vector<float>& childrenPos, std::vector<float>& childrenSize, float& sizeDiff, int& lastChildChangeIndex)
    {
        ssGUI_LOG_FUNC();
        lastChildChangeIndex = -1;
        sizeDiff = 0;
        //There are children added or deleted. No resizing is done
        if(LastUpdateChildrenSize.size() != childrenPos.size())
            return;

        int itIndex = childrenPos.size() - 1;
        
        Container->StashChildrenIterator();
        Container->MoveChildrenIteratorToLast();
        while(!Container->IsChildrenIteratorEnd())
        {
            ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(Container->GetCurrentChild());
            
            if( (childIndex != -1 && ObjectsToExclude.find(childIndex) != ObjectsToExclude.end()) ||
                !Container->GetCurrentChild()->IsEnabled())
            {
                Container->MoveChildrenIteratorPrevious();
                continue;
            }

            //New child. Meaning no resizing is done therefore can just exit
            if(childIndex == -1)
            {
                sizeDiff = 0;
                lastChildChangeIndex = -1;
                break;
            }
            
            if(LastUpdateChildrenSize.find(childIndex) != LastUpdateChildrenSize.end())
            {
                if(IsHorizontalLayout() && LastUpdateChildrenSize[childIndex].x != childrenSize[itIndex])
                {
                    if(lastChildChangeIndex != -1)
                    {
                        lastChildChangeIndex = -1;
                        sizeDiff = 0;
                        break;
                    }
                    
                    sizeDiff = childrenSize[itIndex] - LastUpdateChildrenSize[childIndex].x;
                    lastChildChangeIndex = itIndex;
                }
                else if(!IsHorizontalLayout() && LastUpdateChildrenSize[childIndex].y != childrenSize[itIndex])
                {
                    if(lastChildChangeIndex != -1)
                    {
                        lastChildChangeIndex = -1;
                        sizeDiff = 0;
                        break;
                    }

                    sizeDiff = childrenSize[itIndex] - LastUpdateChildrenSize[childIndex].y;
                    lastChildChangeIndex = itIndex;
                }                
            }
            //New child. Meaning no resizing is done therefore can just exit
            else
            {
                sizeDiff = 0;
                lastChildChangeIndex = -1;
                break;
            }

            itIndex--;
            Container->MoveChildrenIteratorPrevious();
        }
        Container->PopChildrenIterator();
    }

    void Layout::AssignPositionsAndSizesToChildren(std::vector<float>& childrenPos, std::vector<float>& childrenSize)
    {
        ssGUI_LOG_FUNC();
        int index = 0;
        Container->StashChildrenIterator();
        Container->MoveChildrenIteratorToFirst();
        LastUpdateChildrenSize = std::unordered_map<ssGUIObjectIndex, glm::vec2>();
        while(!Container->IsChildrenIteratorEnd())
        {
            ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(Container->GetCurrentChild());
            
            if( (childIndex != -1 && ObjectsToExclude.find(childIndex) != ObjectsToExclude.end()) ||
                !Container->GetCurrentChild()->IsEnabled())
            {
                Container->MoveChildrenIteratorNext();
                continue;
            }
            
            glm::vec2 currentPos = Container->GetCurrentChild()->GetGlobalPosition();
            glm::vec2 currentSize = Container->GetCurrentChild()->GetSize();
            //glm::vec2 currentMinSize = Container->GetCurrentChild()->GetMinSize();
            
            if(IsHorizontalLayout())
            {
                Container->GetCurrentChild()->SetSize(glm::vec2(childrenSize[index], currentSize.y));
                Container->GetCurrentChild()->SetGlobalPosition(glm::vec2(childrenPos[index], currentPos.y));
            }
            else
            {
                Container->GetCurrentChild()->SetSize(glm::vec2(currentSize.x, childrenSize[index]));
                Container->GetCurrentChild()->SetGlobalPosition(glm::vec2(currentPos.x, childrenPos[index]));
            }
            
            //Update ChildrenSize
            if(childIndex == -1)
                childIndex = CurrentObjectsReferences.AddObjectReference(Container->GetCurrentChild());
            LastUpdateChildrenSize[childIndex] = Container->GetCurrentChild()->GetSize();
            
            index++;
            Container->MoveChildrenIteratorNext();
        }
        Container->PopChildrenIterator();
    }

    void Layout::ConstructRenderInfo()
    {}

    void Layout::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {}

    const std::string Layout::EXTENSION_NAME = "Layout";

    bool Layout::IsHorizontalLayout() const
    {
        return HorizontalLayout;
    }

    void Layout::SetHorizontalLayout(bool horizontal)
    {
        HorizontalLayout = horizontal;
        UpdateChildrenResizeTypesAndOnTop();
    }

    void Layout::AddPreferredSizeMultiplier(float sizeMultiplier)
    {
        PreferredSizeMultipliers.push_back(sizeMultiplier);
    }

    void Layout::AddPreferredSizeMultipliers(float sizeMultipliers[], int count)
    {
        for(int i = 0; i < count; i++)
            PreferredSizeMultipliers.push_back(sizeMultipliers[i]);
    }
    
    void Layout::ClearAllPreferredSizeMultiplier()
    {
        PreferredSizeMultipliers.clear();
    }

    void Layout::SetPreferredSizeMultiplier(int index, float sizeMultiplier)
    {
        PreferredSizeMultipliers[index] = sizeMultiplier;
    }

    float Layout::GetPreferredSizeMultiplier(int index) const
    {
        return PreferredSizeMultipliers[index];
    }

    void Layout::RemovePreferredSizeMultiplier(int index)
    {
        if(index < 0 || index >= PreferredSizeMultipliers.size())
            return;
        
        PreferredSizeMultipliers.erase(PreferredSizeMultipliers.begin() + index);
    }

    int Layout::GerPreferredSizeMultiplierCount() const
    {
        return PreferredSizeMultipliers.size();
    }

    bool Layout::IsChildrenResizingDisabled() const
    {
        return DisableChildrenResizing;
    }

    void Layout::SetDisableChildrenResizing(bool disableResizing)
    {
        DisableChildrenResizing = disableResizing;
    }

    bool Layout::IsReverseOrder() const
    {
        return ReverseOrder;
    }

    void Layout::SetReverseOrder(bool reverseOrder)
    {
        ReverseOrder = reverseOrder;
        UpdateChildrenResizeTypesAndOnTop();
    }

    bool Layout::IsCoverFullLength() const
    {
        return CoverFullLength;
    }

    void Layout::SetCoverFullLength(bool fullLength)
    {
        CoverFullLength = fullLength;
    }

    

    bool Layout::IsOverrideChildrenResizeTypeAndOnTop() const
    {
        return OverrideChildrenResizeTypesAndOnTop;
    }

    void Layout::SetOverrideChildrenResizeTypeAndOnTop(bool override)
    {
        ssGUI_LOG_FUNC();
        
        OverrideChildrenResizeTypesAndOnTop = override;
        
        if(Container == nullptr)
            return;
        
        UpdateChildrenResizeTypesAndOnTop();
    }

    bool Layout::GetUpdateContainerMinMaxSize() const
    {
        return UpdateContainerMinMaxSize;
    }

    void Layout::SetUpdateContainerMinMaxSize(bool update)
    {
        ssGUI_LOG_FUNC();
        
        UpdateContainerMinMaxSize = update;
        
        if(Container == nullptr)
            return;

        if(update)
        {
            //Iterate each child to see if event callback exists
            Container->StashChildrenIterator();
            Container->MoveChildrenIteratorToFirst();
            while(!Container->IsChildrenIteratorEnd())
            {
                ssGUIObjectIndex currentChildIndex = CurrentObjectsReferences.GetObjectIndex(Container->GetCurrentChild());
                if(currentChildIndex != -1 && ObjectsToExclude.find(currentChildIndex) != ObjectsToExclude.end())
                {
                    Container->MoveChildrenIteratorNext();
                    continue;
                }

                //Add the child to reference if not present
                if(currentChildIndex == -1)
                    currentChildIndex = CurrentObjectsReferences.AddObjectReference(Container->GetCurrentChild());
                
                if(!Container->GetCurrentChild()->IsEventCallbackExist(ssGUI::Enums::EventType::MIN_MAX_SIZE_CHANGED))
                    Container->GetCurrentChild()->AddEventCallback(ssGUI::Enums::EventType::MIN_MAX_SIZE_CHANGED);

                //This will override the existing event listener if it already exists, otherwise it will just add it.
                Container->GetCurrentChild()->GetEventCallback(ssGUI::Enums::EventType::MIN_MAX_SIZE_CHANGED)->AddEventListener
                (
                    EXTENSION_NAME,
                    [this](ssGUI::EventInfo& info){Internal_OnChildMinMaxSizeChanged(info.EventSource);}     //TODO: Use ObjectsReferences instead of this
                    // std::bind(&ssGUI::Extensions::Layout::Internal_OnChildMinMaxSizeChanged, this, std::placeholders::_1)
                );

                Container->MoveChildrenIteratorNext();
            }
            Container->PopChildrenIterator();

            UpdateExcludedObjects();
            SyncContainerMinMaxSize();
        }
        else
        {            
            //Iterate each child to see if event callback exists
            Container->StashChildrenIterator();
            Container->MoveChildrenIteratorToFirst();
            while(!Container->IsChildrenIteratorEnd())
            {
                ssGUIObjectIndex currentChildIndex = CurrentObjectsReferences.GetObjectIndex(Container->GetCurrentChild());

                //Add the child to reference if not present
                if(currentChildIndex == -1)
                    currentChildIndex = CurrentObjectsReferences.AddObjectReference(Container->GetCurrentChild());
                
                if(Container->GetCurrentChild()->IsEventCallbackExist(ssGUI::Enums::EventType::MIN_MAX_SIZE_CHANGED))
                {
                    Container->GetCurrentChild()->GetEventCallback(ssGUI::Enums::EventType::MIN_MAX_SIZE_CHANGED)->RemoveEventListener(EXTENSION_NAME);
                    
                    //If we were the only one using the event callback, remove it
                    if(Container->GetCurrentChild()->GetEventCallback(ssGUI::Enums::EventType::MIN_MAX_SIZE_CHANGED)->GetEventListenerCount() == 0)
                        Container->GetCurrentChild()->RemoveEventCallback(ssGUI::Enums::EventType::MIN_MAX_SIZE_CHANGED);
                }

                Container->MoveChildrenIteratorNext();
            }
            Container->PopChildrenIterator();
        }
    }

    float Layout::GetPadding() const
    {
        return Padding;
    }

    void Layout::SetPadding(float padding)
    {
        Padding = padding;
    }

    float Layout::GetSpacing() const
    {
        return Spacing;
    }

    void Layout::SetSpacing(float spacing)
    {
        Spacing = spacing;
    }

    void Layout::SetOverflow(bool overflow)
    {
        Overflow = overflow;
    }

    bool Layout::GetOverflow() const
    {
        return Overflow;
    }

    void Layout::ExcludeObject(ssGUI::GUIObject* obj)
    {
        ssGUIObjectIndex objIndex = CurrentObjectsReferences.GetObjectIndex(obj);

        //Add the child to reference if not present
        if(objIndex == -1)
            objIndex = CurrentObjectsReferences.AddObjectReference(obj);
    
        if(ObjectsToExclude.find(objIndex) == ObjectsToExclude.end())
            ObjectsToExclude.insert(objIndex);
    }

    void Layout::UnexcludeObject(ssGUI::GUIObject* obj)
    {
        ssGUIObjectIndex objIndex = CurrentObjectsReferences.GetObjectIndex(obj);

        //Add the child to reference if not present
        if(objIndex == -1)
            objIndex = CurrentObjectsReferences.AddObjectReference(obj);
        
        if(ObjectsToExclude.find(objIndex) != ObjectsToExclude.end())
        {
            //If this is a special object, ignore the unexclude request
            if(SpecialObjectsToExclude.find(objIndex) != SpecialObjectsToExclude.end())
                return;
            
            ObjectsToExclude.erase(objIndex);

            if(obj->GetParent() == Container)
                Internal_OnRecursiveChildAdded(obj);
        }
    }

    void Layout::ForceUpdateLayout( ssGUI::Backend::BackendSystemInputInterface* inputInterface, 
                                    ssGUI::InputStatus& currentInputStatus, 
                                    ssGUI::InputStatus& lastInputStatus, 
                                    ssGUI::GUIObject* mainWindow)
    {
        Updated = false;
        Internal_Update(false, inputInterface, currentInputStatus, lastInputStatus, mainWindow);
    }

    void Layout::Internal_OnRecursiveChildAdded(ssGUI::GUIObject* child)
    {
        ssGUI_LOG_FUNC();

        if(child->GetParent() != Container)
            return;

        ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(child);

        //Add the child to reference if not present
        if(childIndex == -1)
            childIndex = CurrentObjectsReferences.AddObjectReference(child);

        if(ObjectsToExclude.find(childIndex) != ObjectsToExclude.end())
            return;

        if(child->HasTag(ssGUI::Tags::OVERLAY) || child->HasTag(ssGUI::Tags::FLOATING))
        {
            ObjectsToExclude.insert(childIndex);
            SpecialObjectsToExclude.insert(childIndex);
            return;
        }

        //Rely on tag instead
        // Container->StashChildrenIterator();
        // if(Container->MoveChildrenIteratorToChild(child) && Container->IsCurrentChildComposite())
        // {
        //     Container->PopChildrenIterator();
        //     ObjectsToExclude.insert(childIndex);
        //     SpecialObjectsToExclude.insert(childIndex);
        //     return;
        // }
        // Container->PopChildrenIterator();


        if(IsOverrideChildrenResizeTypeAndOnTop())
            UpdateChildrenResizeTypesAndOnTop();
        
        if(GetUpdateContainerMinMaxSize())
        {
            UpdateSpeicalObjectsToExclude();
            UpdateExcludedObjects();
            SyncContainerMinMaxSize();
            
            //Add MinMax size changed callback
            if(!child->IsEventCallbackExist(ssGUI::Enums::EventType::MIN_MAX_SIZE_CHANGED))
                child->AddEventCallback(ssGUI::Enums::EventType::MIN_MAX_SIZE_CHANGED);

            ssGUIObjectIndex containerId = child->GetEventCallback(ssGUI::Enums::EventType::MIN_MAX_SIZE_CHANGED)->AddObjectReference(Container);
            child->GetEventCallback(ssGUI::Enums::EventType::MIN_MAX_SIZE_CHANGED)->AddEventListener
            (
                EXTENSION_NAME,
                Container,
                [containerId](ssGUI::EventInfo& info)
                {
                    ssGUI::GUIObject* layoutContainer = info.References->GetObjectReference(containerId);
                    
                    if(layoutContainer == nullptr)
                    {
                        info.DeleteCurrentListener = true;
                        return;
                    }
                    
                    if(layoutContainer->IsExtensionExist<ssGUI::Extensions::Layout>())
                        layoutContainer ->GetExtension<ssGUI::Extensions::Layout>()->Internal_OnChildMinMaxSizeChanged(info.EventSource);
                }
                // std::bind(&ssGUI::Extensions::Layout::Internal_OnChildMinMaxSizeChanged, this, std::placeholders::_1)
            );
        }
    }

    void Layout::Internal_OnRecursiveChildRemoved(ssGUI::GUIObject* child)
    {        
        ssGUI_LOG_FUNC();

        ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(child);
        //If not present, no need to continue
        if(childIndex == -1)
            return;

        //If this is one of the excluding objects, remove it to prevent invalid object pointer
        if(ObjectsToExclude.find(childIndex) != ObjectsToExclude.end())
            ObjectsToExclude.erase(childIndex);

        //If this is one of the special excluding objects, remove it to prevent invalid object pointer
        if(SpecialObjectsToExclude.find(childIndex) != SpecialObjectsToExclude.end())
            SpecialObjectsToExclude.erase(childIndex);

        //Restore resize type and OnTop if it is recorded
        if(OriginalChildrenResizeType.find(childIndex) != OriginalChildrenResizeType.end())
        {
            static_cast<ssGUI::Window*>(child)->SetResizeType(OriginalChildrenResizeType[childIndex]);
            OriginalChildrenResizeType.erase(childIndex);

            static_cast<ssGUI::Window*>(child)->SetOnTopWhenFocused(OriginalChildrenOnTop[childIndex]);
            OriginalChildrenOnTop.erase(childIndex);
        }

        //Remove MinMax size changed callbacks
        if(child->IsEventCallbackExist(ssGUI::Enums::EventType::MIN_MAX_SIZE_CHANGED))
        {                
            child->GetEventCallback(ssGUI::Enums::EventType::MIN_MAX_SIZE_CHANGED)->RemoveEventListener(EXTENSION_NAME);
            if(child->GetEventCallback(ssGUI::Enums::EventType::MIN_MAX_SIZE_CHANGED)->GetEventListenerCount() == 0)
                child->RemoveEventCallback(ssGUI::Enums::EventType::MIN_MAX_SIZE_CHANGED);
        }

        if(GetUpdateContainerMinMaxSize())
        {
            UpdateSpeicalObjectsToExclude();
            UpdateExcludedObjects();
            SyncContainerMinMaxSize();
        }

        if(IsOverrideChildrenResizeTypeAndOnTop())
            UpdateChildrenResizeTypesAndOnTop();

        //TODO: Maybe this is not needed
        // if(OriginalChildrenSize.find(childIndex) != OriginalChildrenSize.end())
        // {
        //     child->SetSize(OriginalChildrenSize[childIndex]);
        //     OriginalChildrenSize.erase(childIndex);   
        // }

        //Remove the object reference to this child as it is no longer needed to be maintained
        CurrentObjectsReferences.RemoveObjectReference(childIndex);
    }

    void Layout::Internal_OnChildMinMaxSizeChanged(ssGUI::GUIObject* child)
    {        
        ssGUI_LOG_FUNC();

        ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(child);

        //Add the child to reference if not present
        if(childIndex == -1)
            childIndex = CurrentObjectsReferences.AddObjectReference(child);
        
        if(ObjectsToExclude.find(childIndex) != ObjectsToExclude.end())
            return;
        
        if(GetUpdateContainerMinMaxSize())
            SyncContainerMinMaxSize();
    }

    void Layout::SetEnabled(bool enabled)
    {
        Enabled = enabled;
    }

    bool Layout::IsEnabled() const
    {
        return Enabled;
    }

    //Override from Extension
    void Layout::Internal_Update(   bool isPreUpdate, 
                                    ssGUI::Backend::BackendSystemInputInterface* inputInterface, 
                                    ssGUI::InputStatus& currentInputStatus, 
                                    ssGUI::InputStatus& lastInputStatus, 
                                    ssGUI::GUIObject* mainWindow)
    {
        ssGUI_LOG_FUNC();
        
        if(isPreUpdate || Container == nullptr || Container->GetChildrenCount() == 0 || !Enabled || Updated)
            return;

        Updated = true;

        if( Container->GetParent() != nullptr && 
            Container->GetParent()->IsExtensionExist<ssGUI::Extensions::Layout>())
        {
            Container->GetParent()->GetExtension<ssGUI::Extensions::Layout>()->Internal_Update( isPreUpdate, 
                                                                                                inputInterface, 
                                                                                                currentInputStatus, 
                                                                                                lastInputStatus, 
                                                                                                mainWindow);
        }

        //Update SpecialObjectsToExclude if needed
        UpdateSpeicalObjectsToExclude();

        //Update excluded objects if there's an overlay tag
        UpdateExcludedObjects();
        
        //Find number of childrens that are excluded
        int excludeCount = 0;
        Container->StashChildrenIterator();
        Container->MoveChildrenIteratorToFirst();
        while(!Container->IsChildrenIteratorEnd())
        {
            ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(Container->GetCurrentChild());
            if(childIndex != -1 && ObjectsToExclude.find(childIndex) != ObjectsToExclude.end())
                excludeCount++;

            Container->MoveChildrenIteratorNext();
        }
        Container->PopChildrenIterator();

        //Check preffered size multipliers length
        if(PreferredSizeMultipliers.size() != Container->GetChildrenCount() - excludeCount)
            PreferredSizeMultipliers.resize(Container->GetChildrenCount() - excludeCount, -1);
        
        //Disable children resizing
        if(DisableChildrenResizing)
            DisableChildrenResizingInUpdate();

        if(IsOverrideChildrenResizeTypeAndOnTop())
            UpdateChildrenResizeTypesAndOnTop();
        
        //Set all children's width and get all children pos and size and min size
        std::vector<float> childrenPos;
        std::vector<float> childrenSize;
        std::vector<float> childrenMinSize;
        std::vector<float> childrenMaxSize;

        glm::vec2 containerPos = Container->GetGlobalPosition();
        glm::vec2 containerSize = Container->GetSize();
        
        GetAndValidateChildrenDetails(childrenPos, childrenSize, childrenMinSize, childrenMaxSize, containerPos, containerSize);

        //Check if size is different from last update (Iterating from back to front so to get the first child with size change)
        int lastChildChangeIndex = -1;
        float sizeDiff = 0;

        GetLastDifferentChild(childrenPos, childrenSize, sizeDiff, lastChildChangeIndex);

        //Check if there's any child to resize. If not, just go back
        if(childrenSize.empty())
            return;

        //Check for window for laying out children differently
        if(Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && Container->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW && 
            static_cast<ssGUI::Window*>(Container)->HasTitlebar())
        {
            if(IsHorizontalLayout())
            {
                LayoutChildren( containerPos.x + GetPadding(), 
                                containerSize.x - GetPadding() * 2, 
                                childrenPos, childrenSize, childrenMinSize, 
                                childrenMaxSize, lastChildChangeIndex, sizeDiff);
            }
            else
            {
                LayoutChildren( containerPos.y + ((ssGUI::Window*)Container)->GetTitlebarHeight(), 
                                containerSize.y - ((ssGUI::Window*)Container)->GetTitlebarHeight() - GetPadding(), 
                                childrenPos, childrenSize, childrenMinSize, childrenMaxSize, lastChildChangeIndex, sizeDiff);
            }   
        }
        else
        {
            if(IsHorizontalLayout())
                LayoutChildren(containerPos.x + GetPadding(), containerSize.x - GetPadding() * 2, 
                                childrenPos, childrenSize, childrenMinSize, childrenMaxSize, lastChildChangeIndex, sizeDiff);
            else
                LayoutChildren(containerPos.y + GetPadding(), containerSize.y - GetPadding() * 2, 
                                childrenPos, childrenSize, childrenMinSize, childrenMaxSize, lastChildChangeIndex, sizeDiff);
        }
        
        //Assigning position and size to children
        AssignPositionsAndSizesToChildren(childrenPos, childrenSize);
    }

    void Layout::Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {}

    std::string Layout::GetExtensionName() const
    {
        return EXTENSION_NAME;
    }

    void Layout::BindToObject(ssGUI::GUIObject* bindObj)
    {
        ssGUI_LOG_FUNC();
        
        Container = bindObj;

        //By default, layout extension will override children resize type and will also update container min max size
        if(IsOverrideChildrenResizeTypeAndOnTop())
            SetOverrideChildrenResizeTypeAndOnTop(true);
        
        if(GetUpdateContainerMinMaxSize())
            SetUpdateContainerMinMaxSize(true);

        //TODO: Need to add and use child add and remove any recursive child event instead. 
        //      The current way may harm performance significantly.
        //      Right now it will add recursive child to object reference meaning the deeper the nesting,
        //      the more references it has.
        Container->AddEventCallback(ssGUI::Enums::EventType::BEFORE_RECURSIVE_CHILD_ADD)->AddEventListener
        (
            EXTENSION_NAME,
            [](ssGUI::EventInfo& info)
            {
                ssGUI_LOG_FUNC("OnRecursiveChildAddEventCallback");
                if(!info.Container->IsExtensionExist<ssGUI::Extensions::Layout>())
                {
                    ssGUI_ERROR(ssGUI_EXT_TAG, "Failed to find Layout extension. Probably something wrong with cloning");
                    ssLOG_EXIT_PROGRAM();
                    return;
                }

                ssGUI::Extensions::Layout* containerLayout = info.Container->GetExtension<ssGUI::Extensions::Layout>();
                ObjectsReferences* layoutOR = containerLayout->Internal_GetObjectsReferences();
                int childIndex =    layoutOR->IsObjectReferenceExist(info.EventSource) ? 
                                    layoutOR->GetObjectIndex(info.EventSource) : 
                                    layoutOR->AddObjectReference(info.EventSource);
                
                //Record the original size if not exists (Can't be done in OnChildAdded as that's after the child being added)
                if(containerLayout->OriginalChildrenSize.find(childIndex) == containerLayout->OriginalChildrenSize.end())
                    containerLayout->OriginalChildrenSize[childIndex] = info.EventSource->GetSize();

                if( !info.EventSource->IsExtensionExist<ssGUI::Extensions::WindowLayoutItemEnforcer>() && 
                    info.EventSource->GetType() == ssGUI::Enums::GUIObjectType::WINDOW)
                {
                    info.EventSource->AddExtension<ssGUI::Extensions::WindowLayoutItemEnforcer>();
                }
            }
        );

        Container->AddEventCallback(ssGUI::Enums::EventType::RECURSIVE_CHILD_ADDED)->AddEventListener
        (
            EXTENSION_NAME,
            [](ssGUI::EventInfo& info)
            {
                info.Container->GetExtension<ssGUI::Extensions::Layout>()->Internal_OnRecursiveChildAdded(info.EventSource);
            }
        );
        
        Container->AddEventCallback(ssGUI::Enums::EventType::RECURSIVE_CHILD_REMOVED)->AddEventListener
        (
            EXTENSION_NAME,
            [](ssGUI::EventInfo& info)
            {
                info.Container->GetExtension<ssGUI::Extensions::Layout>()->Internal_OnRecursiveChildRemoved(info.EventSource);
            }
        );

        Container->AddEventCallback(ssGUI::Enums::EventType::CHILD_POSITION_CHANGED)->AddEventListener
        (
            EXTENSION_NAME,
            [](ssGUI::EventInfo& info)
            {                    
                ssGUI_LOG_FUNC("ChildPositionChangedEventCallback");
                if(!info.Container->IsExtensionExist<ssGUI::Extensions::Layout>())
                {
                    ssGUI_ERROR(ssGUI_EXT_TAG, "Failed to find Layout extension. Probably something wrong with cloning");
                    ssLOG_EXIT_PROGRAM();
                    return;
                }

                ssGUI::Extensions::Layout* containerLayout = info.Container->GetExtension<ssGUI::Extensions::Layout>();

                if(containerLayout->IsOverrideChildrenResizeTypeAndOnTop())
                    containerLayout->UpdateChildrenResizeTypesAndOnTop();
            }
        );
     
        Container->AddEventCallback(ssGUI::Enums::EventType::OBJECT_RENDERED)->AddEventListener
        (
            EXTENSION_NAME,
            [](ssGUI::EventInfo& info)
            {
                info.Container->GetExtension<ssGUI::Extensions::Layout>()->Updated = false;
            }
        );   
    }

    void Layout::Copy(const ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        
        auto* layout = static_cast<const ssGUI::Extensions::Layout*>(extension);
        
        HorizontalLayout = layout->IsHorizontalLayout();
        PreferredSizeMultipliers = layout->PreferredSizeMultipliers;
        DisableChildrenResizing = layout->IsChildrenResizingDisabled();
        OverrideChildrenResizeTypesAndOnTop = layout->IsOverrideChildrenResizeTypeAndOnTop();
        UpdateContainerMinMaxSize = layout->GetUpdateContainerMinMaxSize();
        ReverseOrder = layout->IsReverseOrder();
        CoverFullLength = layout->IsCoverFullLength();    
        Enabled = layout->IsEnabled();
        Padding = layout->GetPadding();
        Spacing = layout->GetSpacing();
        Overflow = layout->GetOverflow();
        Updated = layout->Updated;
        CurrentObjectsReferences = layout->CurrentObjectsReferences;
        LastUpdateChildrenSize = layout->LastUpdateChildrenSize;
        ObjectsToExclude = layout->ObjectsToExclude;
        SpecialObjectsToExclude = layout->SpecialObjectsToExclude;
        OriginalChildrenSize = layout->OriginalChildrenSize;
        OriginalChildrenResizeType = layout->OriginalChildrenResizeType;
        OriginalChildrenOnTop = layout->OriginalChildrenOnTop;
    }

    ObjectsReferences* Layout::Internal_GetObjectsReferences()
    {
        return &CurrentObjectsReferences;
    }

    Layout* Layout::Clone()
    {
        Layout* temp = new Layout(*this);
        return temp;
    }
}

}