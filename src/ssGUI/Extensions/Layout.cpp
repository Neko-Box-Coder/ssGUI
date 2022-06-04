#include "ssGUI/Extensions/Layout.hpp"

#include "ssGUI/EventCallbacks/OnRecursiveChildAddEventCallback.hpp"
#include "ssGUI/EventCallbacks/RecursiveChildAddedEventCallback.hpp"
#include "ssGUI/EventCallbacks/RecursiveChildRemovedEventCallback.hpp"
#include "ssGUI/EventCallbacks/MinMaxSizeChangedEventCallback.hpp"
#include "ssGUI/EventCallbacks/ChildPositionChangedEventCallback.hpp"
#include "ssGUI/Extensions/WindowLayoutItemEnforcer.hpp"
#include "ssGUI/ssGUITags.hpp"

namespace ssGUI::Extensions
{    
    Layout::Layout() : HorizontalLayout(false), PreferredSizeMultipliers(), DisableChildrenResizing(false), 
                        OverrideChildrenResizeTypesAndOnTop(true), UpdateContainerMinMaxSize(true), ReverseOrder(false), CoverFullLength(true),
                        Container(nullptr), Enabled(true), Padding(0/*5*/), Spacing(5), Overflow(false), 
                        CurrentObjectsReferences(), LastUpdateChildrenSize(), 
                        ObjectsToExclude(), SpecialObjectsToExclude(), OriginalChildrenSize(), OriginalChildrenResizeType(), OriginalChildrenOnTop()
    {}

    Layout::~Layout()
    {
        if(Container != nullptr)
        {
            auto eventCallbackCleanUp = [&](ssGUI::GUIObject* target, std::string eventCallbackName)
            {
                target->GetEventCallback(eventCallbackName)->RemoveEventListener(EXTENSION_NAME);
            
                if(target->GetEventCallback(eventCallbackName)->GetEventListenerCount() == 0)
                    target->RemoveEventCallback(eventCallbackName);
            };

            eventCallbackCleanUp(Container, ssGUI::EventCallbacks::OnRecursiveChildAddEventCallback::EVENT_NAME);
            eventCallbackCleanUp(Container, ssGUI::EventCallbacks::RecursiveChildAddedEventCallback::EVENT_NAME);
            eventCallbackCleanUp(Container, ssGUI::EventCallbacks::RecursiveChildRemovedEventCallback::EVENT_NAME);
            eventCallbackCleanUp(Container, ssGUI::EventCallbacks::ChildPositionChangedEventCallback::EVENT_NAME);
        
            Container->StashChildrenIterator();
            Container->MoveChildrenIteratorToFirst();
            while(!Container->IsChildrenIteratorEnd())
            {
                eventCallbackCleanUp(Container->GetCurrentChild(), ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback::EVENT_NAME);
                Container->MoveChildrenIteratorNext();
            }
            Container->PopChildrenIterator();
        }
        CurrentObjectsReferences.CleanUp();
    }    
    
    Layout::Layout(Layout const& other)
    {
        FUNC_DEBUG_ENTRY();
        HorizontalLayout = other.IsHorizontalLayout();
        PreferredSizeMultipliers = other.PreferredSizeMultipliers;
        DisableChildrenResizing = other.IsChildrenResizingDisabled();
        OverrideChildrenResizeTypesAndOnTop = other.IsOverrideChildrenResizeTypeAndOnTop();
        UpdateContainerMinMaxSize = other.GetUpdateContainerMinMaxSize();
        ReverseOrder = other.IsReverseOrder();
        CoverFullLength = other.IsCoverFullLength();    
        Container = nullptr;
        Enabled = other.IsEnabled();
        Padding = other.GetPadding();
        Spacing = other.GetSpacing();
        Overflow = other.GetOverflow();
        CurrentObjectsReferences = other.CurrentObjectsReferences;
        LastUpdateChildrenSize = other.LastUpdateChildrenSize;//std::unordered_map<ssGUIObjectIndex, glm::ivec2>();
        ObjectsToExclude = other.ObjectsToExclude;
        SpecialObjectsToExclude = other.SpecialObjectsToExclude;
        OriginalChildrenSize = other.OriginalChildrenSize;//std::unordered_map<ssGUIObjectIndex, glm::ivec2>();
        OriginalChildrenResizeType = other.OriginalChildrenResizeType;//std::unordered_map<ssGUIObjectIndex, ssGUI::Enums::ResizeType>();
        OriginalChildrenOnTop = other.OriginalChildrenOnTop;//std::unordered_map<ssGUIObjectIndex, ssGUI::Enums::ResizeType>();
        FUNC_DEBUG_EXIT();
    }

    void Layout::LayoutChildren(float startPos, float length, std::vector<float>& childrenPos, std::vector<float>& childrenLength, 
                                std::vector<float>& minChildrenLength, std::vector<float>& maxChildrenLength, int lastChildChangeIndex,
                                float sizeDiff)
    {        
        FUNC_DEBUG_ENTRY();
        
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
                currentLength = length * sizeMultiplier;
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

        FUNC_DEBUG_EXIT();
    }

    void Layout::UpdateChildrenResizeTypesAndOnTop()
    {
        FUNC_DEBUG_ENTRY();
        
        if(Container == nullptr)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

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
            
            if(Container->GetCurrentChild()->GetType() != ssGUI::Enums::GUIObjectType::WINDOW)
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

                //last element
                if(Container->IsChildrenIteratorLast())
                    windowP->SetResizeType(ssGUI::Enums::ResizeType::NONE);

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
        Container->PopChildrenIterator();

        FUNC_DEBUG_EXIT();
    }

    void Layout::SyncContainerMinMaxSize()
    {
        FUNC_DEBUG_ENTRY();

        if(Container == nullptr || Container->GetChildrenCount() - ObjectsToExclude.size() == 0 || !IsCoverFullLength())
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        //Check the number of valid children
        Container->StashChildrenIterator();
        Container->MoveChildrenIteratorToFirst();
        int validChildrenSize = 0;
        while(!Container->IsChildrenIteratorEnd())
        {            
            ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(Container->GetCurrentChild());
            
            if(childIndex != -1 && ObjectsToExclude.find(childIndex) != ObjectsToExclude.end())
            {
                Container->MoveChildrenIteratorNext();
                continue;
            }

            if(Container->GetCurrentChild() != nullptr && Container->GetCurrentChild()->GetType() == ssGUI::Enums::GUIObjectType::WINDOW &&
                !Container->GetCurrentChild()->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
            {
                ssGUI::Window* childWin = static_cast<ssGUI::Window*>(Container->GetCurrentChild());
                glm::ivec4 tColor = childWin->GetTitlebarColor();
            }

            validChildrenSize++;
            
            Container->MoveChildrenIteratorNext();
        }
        Container->PopChildrenIterator();

        if(validChildrenSize == 0)
        {
            FUNC_DEBUG_EXIT();
            return;
        }
        
        if(HorizontalLayout)
        {
            float minSizeTotalX = 0;
            float maxSizeTotalX = 0;
            float minMaxY = std::numeric_limits<float>::max();
            float maxMinY = -1;
            
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
                
                minSizeTotalX += Container->GetCurrentChild()->GetMinSize().x;
                if(!Container->IsChildrenIteratorLast())
                    minSizeTotalX += GetSpacing();

                //Don't need to do anything if maxSizeTotalX is max
                if(maxSizeTotalX != std::numeric_limits<float>::max())
                {
                    if(Container->GetCurrentChild()->GetMaxSize().x == std::numeric_limits<float>::max())
                        maxSizeTotalX = std::numeric_limits<float>::max();
                    else
                        maxSizeTotalX += Container->GetCurrentChild()->GetMaxSize().x;
                }
                
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
            maxSizeTotalX = maxSizeTotalX == std::numeric_limits<float>::max() ? std::numeric_limits<float>::max() : 
                            maxSizeTotalX + paddingTotalX + spacingTotalX;

            if(Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && dynamic_cast<ssGUI::Window*>(Container)->HasTitlebar())
            {
                minMaxY = minMaxY == std::numeric_limits<float>::max() ? std::numeric_limits<float>::max() :
                            minMaxY + dynamic_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() + GetPadding();
                maxMinY += dynamic_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() + GetPadding();
            }
            else
            {
                minMaxY = minMaxY == std::numeric_limits<float>::max() ? std::numeric_limits<float>::max() :
                            minMaxY + GetPadding() * 2;
                maxMinY += GetPadding() * 2;
            }

            Container->SetMinSize(glm::vec2(minSizeTotalX, maxMinY));
            Container->SetMaxSize(glm::vec2(maxSizeTotalX, minMaxY));
        }
        else
        {
            float minSizeTotalY = 0;
            float maxSizeTotalY = 0;
            float minMaxX = std::numeric_limits<float>::max();
            float maxMinX = -1;
            
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
                
                minSizeTotalY += Container->GetCurrentChild()->GetMinSize().y;
                if(!Container->IsChildrenIteratorLast())
                    minSizeTotalY += GetSpacing();

                //Don't need to do anything if maxSizeTotalY is max
                if(maxSizeTotalY != std::numeric_limits<float>::max())
                {
                    if(Container->GetCurrentChild()->GetMaxSize().y == std::numeric_limits<float>::max())
                        maxSizeTotalY = std::numeric_limits<float>::max();
                    else
                        maxSizeTotalY += Container->GetCurrentChild()->GetMaxSize().y;
                }
                
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
            
            if(Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW)
                paddingTotalY = GetPadding() + dynamic_cast<ssGUI::Window*>(Container)->GetTitlebarHeight();
            else
                paddingTotalY = GetPadding() * 2;

            float spacingTotalY = (Container->GetChildrenCount() - ObjectsToExclude.size() - 1) * GetSpacing();

            minSizeTotalY += paddingTotalY + spacingTotalY;
            maxSizeTotalY = maxSizeTotalY == std::numeric_limits<float>::max() ? std::numeric_limits<float>::max() : 
                            maxSizeTotalY + paddingTotalY + spacingTotalY;

            minMaxX = minMaxX == std::numeric_limits<float>::max() ? std::numeric_limits<float>::max() :
                            minMaxX + GetPadding() * 2;
            maxMinX += GetPadding() * 2;

            Container->SetMinSize(glm::vec2(maxMinX, minSizeTotalY));
            Container->SetMaxSize(glm::vec2(minMaxX, maxSizeTotalY));
        }

        FUNC_DEBUG_EXIT();
    }

    void Layout::UpdateSpeicalObjectsToExclude()
    {
        FUNC_DEBUG_ENTRY();
        std::vector<ssGUIObjectIndex> objsToRemove;
        for(auto it : SpecialObjectsToExclude)
        {
            if(CurrentObjectsReferences.GetObjectReference(it) == nullptr)
                continue;
            
            //Don't need to hold reference for it as it is not a child
            if(CurrentObjectsReferences.GetObjectReference(it) == nullptr || 
                CurrentObjectsReferences.GetObjectReference(it)->GetParent() != Container)
            {       
                objsToRemove.push_back(it);
                CurrentObjectsReferences.RemoveObjectReference(it);
            }
            //If it needs to be in the layout
            else if(!CurrentObjectsReferences.GetObjectReference(it)->HasTag(ssGUI::Tags::OVERLAY) && 
                    !CurrentObjectsReferences.GetObjectReference(it)->HasTag(ssGUI::Tags::FLOATING) && 
                    CurrentObjectsReferences.GetObjectReference(it)->IsVisible())
            {
                objsToRemove.push_back(it);
            }
        }

        for(auto objIndex : objsToRemove)
        {
            ObjectsToExclude.erase(objIndex);
            SpecialObjectsToExclude.erase(objIndex);
        }
        FUNC_DEBUG_EXIT();
    }

    void Layout::UpdateExcludedObjects()
    {
        FUNC_DEBUG_ENTRY();
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
            
            if(Container->GetCurrentChild()->HasTag(ssGUI::Tags::OVERLAY) || Container->GetCurrentChild()->HasTag(ssGUI::Tags::FLOATING) || !Container->GetCurrentChild()->IsVisible())
            {
                ObjectsToExclude.insert(childIndex);
                SpecialObjectsToExclude.insert(childIndex);
                Container->MoveChildrenIteratorNext();
                continue;
            }
            
            Container->MoveChildrenIteratorNext();
        }
        Container->PopChildrenIterator();
        FUNC_DEBUG_EXIT();
    }

    void Layout::DisableChildrenResizingInUpdate()
    {
        FUNC_DEBUG_ENTRY();
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
            
            if(Container->GetCurrentChild()->GetType() == ssGUI::Enums::GUIObjectType::WINDOW)
                dynamic_cast<ssGUI::Window*>(Container->GetCurrentChild())->SetResizeType(ssGUI::Enums::ResizeType::NONE);

            Container->MoveChildrenIteratorNext();
        }
        Container->PopChildrenIterator();
        FUNC_DEBUG_EXIT();
    }

    void Layout::GetAndValidateChildrenDetails(std::vector<float>& childrenPos, std::vector<float>& childrenSize, std::vector<float>& childrenMinSize,
                                                std::vector<float>& childrenMaxSize, glm::vec2 containerPos, glm::vec2 containerSize)
    {
        FUNC_DEBUG_ENTRY();
        Container->StashChildrenIterator();
        Container->MoveChildrenIteratorToFirst();
        while(!Container->IsChildrenIteratorEnd())
        {
            ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(Container->GetCurrentChild());
            
            if(childIndex != -1 && ObjectsToExclude.find(childIndex) != ObjectsToExclude.end())
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
                float verticalPos = Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && Container->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW && 
                                    static_cast<ssGUI::Window*>(Container)->HasTitlebar() ? 
                                    containerPos.y + dynamic_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() :
                                    containerPos.y + GetPadding();     

                float verticalSize = Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && Container->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW && 
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
        FUNC_DEBUG_EXIT();
    }

    void Layout::GetLastDifferentChild(std::vector<float>& childrenPos, std::vector<float>& childrenSize, float& sizeDiff, int& lastChildChangeIndex)
    {
        FUNC_DEBUG_ENTRY();
        lastChildChangeIndex = -1;
        sizeDiff = 0;
        //There are children added or deleted. No resizing is done
        if(LastUpdateChildrenSize.size() != childrenPos.size())
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        int itIndex = childrenPos.size() - 1;
        
        Container->StashChildrenIterator();
        Container->MoveChildrenIteratorToLast();
        while(!Container->IsChildrenIteratorEnd())
        {
            ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(Container->GetCurrentChild());
            
            if(childIndex != -1 && ObjectsToExclude.find(childIndex) != ObjectsToExclude.end())
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
        FUNC_DEBUG_EXIT();
    }

    void Layout::AssignPositionsAndSizesToChildren(std::vector<float>& childrenPos, std::vector<float>& childrenSize)
    {
        FUNC_DEBUG_ENTRY();
        int index = 0;
        Container->StashChildrenIterator();
        Container->MoveChildrenIteratorToFirst();
        LastUpdateChildrenSize = std::unordered_map<ssGUIObjectIndex, glm::vec2>();
        while(!Container->IsChildrenIteratorEnd())
        {
            ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(Container->GetCurrentChild());
            
            if(childIndex != -1 && ObjectsToExclude.find(childIndex) != ObjectsToExclude.end())
            {
                Container->MoveChildrenIteratorNext();
                continue;
            }
            
            glm::vec2 currentPos = Container->GetCurrentChild()->GetGlobalPosition();
            glm::vec2 currentSize = Container->GetCurrentChild()->GetSize();
            glm::vec2 currentMinSize = Container->GetCurrentChild()->GetMinSize();
            
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
        FUNC_DEBUG_EXIT();
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
    }

    void Layout::AddPreferredSizeMultiplier(float sizeMultiplier)
    {
        PreferredSizeMultipliers.push_back(sizeMultiplier);
    }

    void Layout::SetPreferredSizeMultiplier(int index, float sizeMultiplier)
    {
        PreferredSizeMultipliers[index] = sizeMultiplier;
    }

    float Layout::GetPreferredSizeMultiplier(int index) const
    {
        return PreferredSizeMultipliers[index];
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
        FUNC_DEBUG_ENTRY();
        
        OverrideChildrenResizeTypesAndOnTop = override;
        
        if(Container == nullptr)
        {
            FUNC_DEBUG_EXIT();
            return;
        }
        
        const std::string onChildAddedEventName = ssGUI::EventCallbacks::RecursiveChildAddedEventCallback::EVENT_NAME;
        const std::string onChildRemovedEventName = ssGUI::EventCallbacks::RecursiveChildRemovedEventCallback::EVENT_NAME;
        UpdateChildrenResizeTypesAndOnTop();

        FUNC_DEBUG_EXIT();
    }

    bool Layout::GetUpdateContainerMinMaxSize() const
    {
        return UpdateContainerMinMaxSize;
    }

    void Layout::SetUpdateContainerMinMaxSize(bool update)
    {
        FUNC_DEBUG_ENTRY();
        
        UpdateContainerMinMaxSize = update;
        
        if(Container == nullptr)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        const std::string onChildAddedEventName = ssGUI::EventCallbacks::RecursiveChildAddedEventCallback::EVENT_NAME;
        const std::string onChildRemovedEventName = ssGUI::EventCallbacks::RecursiveChildRemovedEventCallback::EVENT_NAME;
        const std::string onMinMaxSizeChangedEventName = ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback::EVENT_NAME;

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
                
                if(!Container->GetCurrentChild()->IsEventCallbackExist(onMinMaxSizeChangedEventName))
                    Container->GetCurrentChild()->AddEventCallback(ssGUI::Factory::Create<ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback>());

                //This will override the existing event listener if it already exists, otherwise it will just add it.
                Container->GetCurrentChild()->GetEventCallback(onMinMaxSizeChangedEventName)->AddEventListener
                (
                    EXTENSION_NAME,
                    std::bind(&ssGUI::Extensions::Layout::Internal_OnChildMinMaxSizeChanged, this, std::placeholders::_1)
                );

                Container->MoveChildrenIteratorNext();
            }
            Container->PopChildrenIterator();

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
                
                if(Container->GetCurrentChild()->IsEventCallbackExist(onMinMaxSizeChangedEventName))
                {
                    Container->GetCurrentChild()->GetEventCallback(onMinMaxSizeChangedEventName)->RemoveEventListener(EXTENSION_NAME);
                    
                    //If we were the only one using the event callback, remove it
                    if(Container->GetCurrentChild()->GetEventCallback(onMinMaxSizeChangedEventName)->GetEventListenerCount() == 0)
                        Container->GetCurrentChild()->RemoveEventCallback(onMinMaxSizeChangedEventName);
                }

                Container->MoveChildrenIteratorNext();
            }
            Container->PopChildrenIterator();
        }

        FUNC_DEBUG_EXIT();
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
            ObjectsToExclude.erase(objIndex);

            if(SpecialObjectsToExclude.find(objIndex) != SpecialObjectsToExclude.end())
                SpecialObjectsToExclude.erase(objIndex);

            if(obj->GetParent() == Container)
                Internal_OnRecursiveChildAdded(obj);
        }
    }

    //TODO: This can be just using the ChildAddedEventCallback instead of recursive one
    void Layout::Internal_OnRecursiveChildAdded(ssGUI::GUIObject* child)
    {
        FUNC_DEBUG_ENTRY();

        if(child->GetParent() != Container)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(child);

        //Add the child to reference if not present
        if(childIndex == -1)
            childIndex = CurrentObjectsReferences.AddObjectReference(child);

        if(ObjectsToExclude.find(childIndex) != ObjectsToExclude.end())
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        if(child->HasTag(ssGUI::Tags::OVERLAY) || child->HasTag(ssGUI::Tags::FLOATING))
        {
            ObjectsToExclude.insert(childIndex);
            SpecialObjectsToExclude.insert(childIndex);
            FUNC_DEBUG_EXIT();
            return;
        }

        if(IsOverrideChildrenResizeTypeAndOnTop())
            UpdateChildrenResizeTypesAndOnTop();
        
        if(GetUpdateContainerMinMaxSize())
        {
            SyncContainerMinMaxSize();
            const std::string onMinMaxSizeChangedEventName = ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback::EVENT_NAME;
            
            //Add MinMax size changed callback
            if(!child->IsEventCallbackExist(onMinMaxSizeChangedEventName))
                child->AddEventCallback(ssGUI::Factory::Create<ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback>());
            

            child->GetEventCallback(onMinMaxSizeChangedEventName)->AddEventListener
            (
                EXTENSION_NAME,
                std::bind(&ssGUI::Extensions::Layout::Internal_OnChildMinMaxSizeChanged, this, std::placeholders::_1)
            );
        }

        FUNC_DEBUG_EXIT();
    }

    void Layout::Internal_OnRecursiveChildRemoved(ssGUI::GUIObject* child)
    {        
        FUNC_DEBUG_ENTRY();

        // DEBUG_LINE("Container: "<<Container);
        // DEBUG_LINE("child removed: "<<child);

        // //DEBUG
        // child->MoveChildrenIteratorToFirst();
        // while (!child->IsChildrenIteratorEnd())
        // {
        //     DEBUG_LINE("child's child: "<<child->GetCurrentChild());
        //     child->MoveChildrenIteratorNext();
        // }

        ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(child);
        //If not present, no need to continue
        if(childIndex == -1)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        //If this is one of the special excluding objects, remove it to prevent invalid object pointer
        if(ObjectsToExclude.find(childIndex) != ObjectsToExclude.end() &&
            SpecialObjectsToExclude.find(childIndex) != SpecialObjectsToExclude.end())
        {
            ObjectsToExclude.erase(childIndex);
            SpecialObjectsToExclude.erase(childIndex);
            CurrentObjectsReferences.RemoveObjectReference(childIndex);
            FUNC_DEBUG_EXIT();
            return;
        }

        //If this is one of the excluding objects, just don't do anything
        if(ObjectsToExclude.find(childIndex) != ObjectsToExclude.end())
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        //Restore resize type and OnTop if it is recorded
        if(OriginalChildrenResizeType.find(childIndex) != OriginalChildrenResizeType.end())
        {
            static_cast<ssGUI::Window*>(child)->SetResizeType(OriginalChildrenResizeType[childIndex]);
            OriginalChildrenResizeType.erase(childIndex);

            static_cast<ssGUI::Window*>(child)->SetOnTopWhenFocused(OriginalChildrenOnTop[childIndex]);
            OriginalChildrenOnTop.erase(childIndex);
        }

        //Remove MinMax size changed callbacks
        const std::string onMinMaxSizeChangedEventName = ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback::EVENT_NAME;
        if(child->IsEventCallbackExist(onMinMaxSizeChangedEventName))
        {                
            child->GetEventCallback(onMinMaxSizeChangedEventName)->RemoveEventListener(EXTENSION_NAME);
            if(child->GetEventCallback(onMinMaxSizeChangedEventName)->GetEventListenerCount() == 0)
                child->RemoveEventCallback(onMinMaxSizeChangedEventName);
        }

        if(GetUpdateContainerMinMaxSize())
            SyncContainerMinMaxSize();

        if(IsOverrideChildrenResizeTypeAndOnTop())
            UpdateChildrenResizeTypesAndOnTop();

        if(OriginalChildrenSize.find(childIndex) != OriginalChildrenSize.end())
        {
            child->SetSize(OriginalChildrenSize[childIndex]);
            OriginalChildrenSize.erase(childIndex);   
        }

        //Remove the object reference to this child as it is no longer needed to be maintained
        CurrentObjectsReferences.RemoveObjectReference(childIndex);

        FUNC_DEBUG_EXIT();
    }

    void Layout::Internal_OnChildMinMaxSizeChanged(ssGUI::GUIObject* child)
    {        
        FUNC_DEBUG_ENTRY();

        ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(child);

        //Add the child to reference if not present
        if(childIndex == -1)
            childIndex = CurrentObjectsReferences.AddObjectReference(child);
        
        if(ObjectsToExclude.find(childIndex) != ObjectsToExclude.end())
        {
            FUNC_DEBUG_EXIT();
            return;
        }
        
        if(GetUpdateContainerMinMaxSize())
        {
            SyncContainerMinMaxSize();
        }

        FUNC_DEBUG_EXIT();
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
    void Layout::Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();
        
        if(isPreUpdate || Container == nullptr || Container->GetChildrenCount() == 0 || !Enabled)
        {
            FUNC_DEBUG_EXIT();
            return;
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
        
        //TODO : Probably don't need this as events will sort this out, need awhile to test to make sure.
        // if(UpdateContainerMinMaxSize)
        //     SyncContainerMinMaxSize();

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
        {
            FUNC_DEBUG_EXIT();
            return;
        }

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

        FUNC_DEBUG_EXIT();
    }

    void Layout::Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {}

    std::string Layout::GetExtensionName()
    {
        return EXTENSION_NAME;
    }

    void Layout::BindToObject(ssGUI::GUIObject* bindObj)
    {
        FUNC_DEBUG_ENTRY();
        
        Container = bindObj;

        //By default, layout extension will override children resize type and will also update container min max size
        if(IsOverrideChildrenResizeTypeAndOnTop())
            SetOverrideChildrenResizeTypeAndOnTop(true);
        
        if(GetUpdateContainerMinMaxSize())
            SetUpdateContainerMinMaxSize(true);

        if(!Container->IsEventCallbackExist(ssGUI::EventCallbacks::OnRecursiveChildAddEventCallback::EVENT_NAME))
            Container->AddEventCallback(ssGUI::Factory::Create<ssGUI::EventCallbacks::OnRecursiveChildAddEventCallback>());

        Container->GetEventCallback(ssGUI::EventCallbacks::OnRecursiveChildAddEventCallback::EVENT_NAME)->AddEventListener
        (
            EXTENSION_NAME,
            [](ssGUI::GUIObject* child, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* refs)
            {
                FUNC_DEBUG_ENTRY("OnRecursiveChildAddEventCallback");
                if(!container->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
                {
                    DEBUG_LINE("Failed to find Layout extension. Probably something wrong with cloning");
                    DEBUG_EXIT_PROGRAM();
                    return;
                }

                ssGUI::Extensions::Layout* containerLayout = static_cast<ssGUI::Extensions::Layout*>
                    (container->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME));
                ObjectsReferences* layoutOR = containerLayout->Internal_GetObjectsReferences();
                int childIndex = layoutOR->IsObjectReferenceExist(child) ? layoutOR->GetObjectIndex(child) : layoutOR->AddObjectReference(child);
                
                //Record the original size if not exists (Can't be done in OnChildAdded as that's after the child being added)
                if(containerLayout->OriginalChildrenSize.find(childIndex) == containerLayout->OriginalChildrenSize.end())
                    containerLayout->OriginalChildrenSize[childIndex] = child->GetSize();

                if(!child->IsExtensionExist(ssGUI::Extensions::WindowLayoutItemEnforcer::EXTENSION_NAME) && child->GetType() == ssGUI::Enums::GUIObjectType::WINDOW)
                    child->AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::WindowLayoutItemEnforcer>());
                FUNC_DEBUG_EXIT("OnRecursiveChildAddEventCallback");
            }
        );

        const std::string onChildAddedEventName = ssGUI::EventCallbacks::RecursiveChildAddedEventCallback::EVENT_NAME;
        const std::string onChildRemovedEventName = ssGUI::EventCallbacks::RecursiveChildRemovedEventCallback::EVENT_NAME;
        const std::string childPositionChangedEventName = ssGUI::EventCallbacks::ChildPositionChangedEventCallback::EVENT_NAME;

        if(!Container->IsEventCallbackExist(onChildAddedEventName))
            Container->AddEventCallback(ssGUI::Factory::Create<ssGUI::EventCallbacks::RecursiveChildAddedEventCallback>());
        
        if(!Container->IsEventCallbackExist(onChildRemovedEventName))
            Container->AddEventCallback(ssGUI::Factory::Create<ssGUI::EventCallbacks::RecursiveChildRemovedEventCallback>());

        if(!Container->IsEventCallbackExist(childPositionChangedEventName))
            Container->AddEventCallback(ssGUI::Factory::Create<ssGUI::EventCallbacks::ChildPositionChangedEventCallback>());

        Container->GetEventCallback(onChildAddedEventName)->AddEventListener
        (
            EXTENSION_NAME,
            std::bind(&ssGUI::Extensions::Layout::Internal_OnRecursiveChildAdded, this, std::placeholders::_1)
        );
        
        Container->GetEventCallback(onChildRemovedEventName)->AddEventListener
        (
            EXTENSION_NAME,
            std::bind(&ssGUI::Extensions::Layout::Internal_OnRecursiveChildRemoved, this, std::placeholders::_1)
        );

        Container->GetEventCallback(childPositionChangedEventName)->AddEventListener
        (
            EXTENSION_NAME,
            [](ssGUI::GUIObject* child, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* refs)
            {                    
                FUNC_DEBUG_ENTRY("ChildPositionChangedEventCallback");
                if(!container->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
                {
                    DEBUG_LINE("Failed to find Layout extension. Probably something wrong with cloning");
                    DEBUG_EXIT_PROGRAM();
                    return;
                }

                ssGUI::Extensions::Layout* containerLayout = static_cast<ssGUI::Extensions::Layout*>
                    (container->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME));

                if(containerLayout->IsOverrideChildrenResizeTypeAndOnTop())
                    containerLayout->UpdateChildrenResizeTypesAndOnTop();
                FUNC_DEBUG_EXIT("ChildPositionChangedEventCallback");
            }
        );

        FUNC_DEBUG_EXIT();
    }

    void Layout::Copy(ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        ssGUI::Extensions::Layout* layout = static_cast<ssGUI::Extensions::Layout*>(extension);
        
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
        CurrentObjectsReferences = layout->CurrentObjectsReferences;
    }

    ObjectsReferences* Layout::Internal_GetObjectsReferences()
    {
        return &CurrentObjectsReferences;
    }

    Layout* Layout::Clone(ssGUI::GUIObject* newContainer)
    {
        Layout* temp = new Layout(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        
        return temp;
    }
}