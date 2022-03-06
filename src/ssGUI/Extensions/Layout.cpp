#include "ssGUI/Extensions/Layout.hpp"

namespace ssGUI::Extensions
{    
    Layout::Layout(Layout const& other)
    {
        FUNC_DEBUG_ENTRY();
        HorizontalLayout = other.IsHorizontalLayout();
        PreferredSizeMultipliers = other.PreferredSizeMultipliers;
        DisableChildrenResizing = other.IsChildrenResizingDisabled();
        OverrideChildrenResizeTypes = other.GetOverrideChildrenResizeType();
        UpdateContainerMinMaxSize = other.GetUpdateContainerMinMaxSize();
        ReverseOrder = other.IsReverseOrder();
        CoverFullLength = other.IsCoverFullLength();    
        Container = nullptr;
        Enabled = other.IsEnabled();
        Padding = other.GetPadding();
        Spacing = other.GetSpacing();
        Overflow = other.GetOverflow();
        OnChildAddEventIndex = -1;
        ChildAddedEventIndex = -1;
        ChildRemovedEventIndex = -1;
        ChildPositionChangedEventIndex = -1;
        CurrentObjectsReferences = other.CurrentObjectsReferences;
        LastUpdateChildrenSize = other.LastUpdateChildrenSize;//std::unordered_map<ssGUIObjectIndex, glm::ivec2>();
        ObjectsToExclude = other.ObjectsToExclude;
        SpecialObjectsToExclude = other.SpecialObjectsToExclude;
        OriginalChildrenSize = other.OriginalChildrenSize;//std::unordered_map<ssGUIObjectIndex, glm::ivec2>();
        OriginalChildrenResizeType = other.OriginalChildrenResizeType;//std::unordered_map<ssGUIObjectIndex, ssGUI::Enums::ResizeType>();
        
        //TODO : Re-add the event listeners
        MinMaxSizeChangedEventIndices = std::unordered_map<ssGUIObjectIndex, int>();//other.MinMaxSizeChangedEventIndices;
        FUNC_DEBUG_EXIT();
    }

    void Layout::LayoutChildren(int startPos, int length, std::vector<int>& childrenPos, std::vector<int>& childrenLength, 
                                std::vector<int>& minChildrenLength, std::vector<int>& maxChildrenLength, int lastChildChangeIndex,
                                int sizeDiff)
    {        
        FUNC_DEBUG_ENTRY();
        
        int endPos = startPos + length;
        int currentPos = IsReverseOrder() ? endPos : startPos;
        int currentLength = 0;
        int remainingLength = length;

        int remainingMinLengths = 0;
        for(auto minLength : minChildrenLength)
            remainingMinLengths += minLength;

        //Exclude spacing in the length calculation
        for(auto temp : childrenLength)
            remainingLength -= GetSpacing();
        
        //Don't need to exclude spacing for the last element, therefore adding it back 
        remainingLength += GetSpacing();

        //Change the child after the last size change child to fit the layout container
        if(lastChildChangeIndex != -1 && ++lastChildChangeIndex <= childrenPos.size() - 2)
        {
            if(childrenLength[lastChildChangeIndex] - sizeDiff >= minChildrenLength[lastChildChangeIndex] && 
                childrenLength[lastChildChangeIndex] - sizeDiff <= maxChildrenLength[lastChildChangeIndex])
            {
                childrenLength[lastChildChangeIndex] -= sizeDiff;
            }
            else if(childrenLength[lastChildChangeIndex] - sizeDiff < minChildrenLength[lastChildChangeIndex])
                childrenLength[lastChildChangeIndex] = minChildrenLength[lastChildChangeIndex];
            else if(childrenLength[lastChildChangeIndex] - sizeDiff > maxChildrenLength[lastChildChangeIndex])
                childrenLength[lastChildChangeIndex] = maxChildrenLength[lastChildChangeIndex];
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
                    int lengthIncrease = maxChildrenLength[i] - childrenLength[i];
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

    void Layout::UpdateChildrenResizeTypes()
    {
        FUNC_DEBUG_ENTRY();
        
        if(Container == nullptr)
        {
            FUNC_DEBUG_EXIT();
            return;
        }
        
        if(GetOverrideChildrenResizeType())
        {
            if(DisableChildrenResizing)
            {
                FUNC_DEBUG_EXIT();
                return;
            }
        }

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
            
            //Recording original resize type and override it
            if(GetOverrideChildrenResizeType())
            {
                if(OriginalChildrenResizeType.find(currentChildIndex) == OriginalChildrenResizeType.end())
                    OriginalChildrenResizeType[currentChildIndex] = windowP->GetResizeType();
                
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
            }
            
            //Restoring original resize type
            if(!GetOverrideChildrenResizeType() && OriginalChildrenResizeType.find(currentChildIndex) != OriginalChildrenResizeType.end())
            {                
                windowP->SetResizeType(OriginalChildrenResizeType[currentChildIndex]);

                OriginalChildrenResizeType.erase(currentChildIndex);
            }

            Container->MoveChildrenIteratorNext();
        }

        FUNC_DEBUG_EXIT();
    }

    void Layout::SyncContainerMinMaxSize()
    {
        FUNC_DEBUG_ENTRY();

        if(Container == nullptr || Container->GetChildrenCount() - ObjectsToExclude.size() == 0)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        //Check the number of valid children
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

        if(validChildrenSize == 0)
        {
            FUNC_DEBUG_EXIT();
            return;
        }
        
        if(HorizontalLayout)
        {
            int minSizeTotalX = 0;
            int maxSizeTotalX = 0;
            int minMaxY = std::numeric_limits<int>::max();
            int maxMinY = -1;
            
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

                if(maxSizeTotalX == std::numeric_limits<int>::max())
                {
                    Container->MoveChildrenIteratorNext();
                    continue;
                }
                else if(Container->GetCurrentChild()->GetMaxSize().x == std::numeric_limits<int>::max())
                    maxSizeTotalX = std::numeric_limits<int>::max();
                else
                    maxSizeTotalX += Container->GetCurrentChild()->GetMaxSize().x;
                
                if(Container->GetCurrentChild()->GetMaxSize().y < minMaxY)
                    minMaxY = Container->GetCurrentChild()->GetMaxSize().y;

                if(Container->GetCurrentChild()->GetMinSize().y > maxMinY)
                    maxMinY = Container->GetCurrentChild()->GetMinSize().y;
                
                Container->MoveChildrenIteratorNext();
            }

            if(minMaxY < maxMinY)
                minMaxY = maxMinY;

            int paddingTotalX = GetPadding() * 2;
            int spacingTotalX = (Container->GetChildrenCount() - ObjectsToExclude.size() - 1) * GetSpacing();

            minSizeTotalX += paddingTotalX + spacingTotalX;
            maxSizeTotalX = maxSizeTotalX == std::numeric_limits<int>::max() ? std::numeric_limits<int>::max() : 
                            maxSizeTotalX + paddingTotalX + spacingTotalX;

            if(Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW)
            {
                minMaxY = minMaxY == std::numeric_limits<int>::max() ? std::numeric_limits<int>::max() :
                            minMaxY + dynamic_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() + GetPadding();
                maxMinY += dynamic_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() + GetPadding();
            }
            else
            {
                minMaxY = minMaxY == std::numeric_limits<int>::max() ? std::numeric_limits<int>::max() :
                            minMaxY + GetPadding() * 2;
                maxMinY += GetPadding() * 2;
            }

            Container->SetMinSize(glm::ivec2(minSizeTotalX, maxMinY));
            Container->SetMaxSize(glm::ivec2(maxSizeTotalX, minMaxY));
        }
        else
        {
            int minSizeTotalY = 0;
            int maxSizeTotalY = 0;
            int minMaxX = std::numeric_limits<int>::max();
            int maxMinX = -1;
            
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

                if(maxSizeTotalY == std::numeric_limits<int>::max())
                {
                    Container->MoveChildrenIteratorNext();
                    continue;
                }
                else if(Container->GetCurrentChild()->GetMaxSize().y == std::numeric_limits<int>::max())
                    maxSizeTotalY = std::numeric_limits<int>::max();
                else
                    maxSizeTotalY += Container->GetCurrentChild()->GetMaxSize().y;
                
                if(Container->GetCurrentChild()->GetMaxSize().x < minMaxX)
                    minMaxX = Container->GetCurrentChild()->GetMaxSize().x;

                if(Container->GetCurrentChild()->GetMinSize().x > maxMinX)
                    maxMinX = Container->GetCurrentChild()->GetMinSize().x;

                Container->MoveChildrenIteratorNext();
            }

            if(minMaxX < maxMinX)
                minMaxX = maxMinX;

            int paddingTotalY = 0;
            
            if(Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW)
                paddingTotalY = GetPadding() + dynamic_cast<ssGUI::Window*>(Container)->GetTitlebarHeight();
            else
                paddingTotalY = GetPadding() * 2;

            int spacingTotalY = (Container->GetChildrenCount() - ObjectsToExclude.size() - 1) * GetSpacing();

            minSizeTotalY += paddingTotalY + spacingTotalY;
            maxSizeTotalY = maxSizeTotalY == std::numeric_limits<int>::max() ? std::numeric_limits<int>::max() : 
                            maxSizeTotalY + paddingTotalY + spacingTotalY;

            minMaxX = minMaxX == std::numeric_limits<int>::max() ? std::numeric_limits<int>::max() :
                            minMaxX + GetPadding() * 2;
            maxMinX += GetPadding() * 2;

            Container->SetMinSize(glm::ivec2(maxMinX, minSizeTotalY));
            Container->SetMaxSize(glm::ivec2(minMaxX, maxSizeTotalY));
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
        FUNC_DEBUG_EXIT();
    }

    void Layout::DisableChildrenResizingInUpdate()
    {
        FUNC_DEBUG_ENTRY();
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
        FUNC_DEBUG_EXIT();
    }

    void Layout::GetAndValidateChildrenDetails(std::vector<int>& childrenPos, std::vector<int>& childrenSize, std::vector<int>& childrenMinSize,
                                    std::vector<int>& childrenMaxSize, glm::ivec2 containerPos, glm::ivec2 containerSize)
    {
        FUNC_DEBUG_ENTRY();
        Container->MoveChildrenIteratorToFirst();
        while(!Container->IsChildrenIteratorEnd())
        {
            ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(Container->GetCurrentChild());
            
            if(childIndex != -1 && ObjectsToExclude.find(childIndex) != ObjectsToExclude.end())
            {   
                Container->MoveChildrenIteratorNext();
                continue;
            }

            glm::ivec2 currentPos = Container->GetCurrentChild()->GetGlobalPosition();
            glm::ivec2 currentSize = Container->GetCurrentChild()->GetSize();
            glm::ivec2 currentMinSize = Container->GetCurrentChild()->GetMinSize();
            glm::ivec2 currentMaxSize = Container->GetCurrentChild()->GetMaxSize();
            
            if(IsHorizontalLayout())
            {
                int verticalPos = Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && Container->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW && 
                                    static_cast<ssGUI::Window*>(Container)->HasTitlebar() ? 
                                    containerPos.y + dynamic_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() :
                                    containerPos.y + GetPadding();     

                int verticalSize = Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && Container->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW && 
                                    static_cast<ssGUI::Window*>(Container)->HasTitlebar() ? 
                                    containerSize.y - GetPadding() - dynamic_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() :
                                    containerSize.y - GetPadding() * 2;

                if(currentPos.y != verticalPos)
                    Container->GetCurrentChild()->SetGlobalPosition(glm::ivec2(currentPos.x, verticalPos));
            
                if(currentSize.y != verticalSize)
                    Container->GetCurrentChild()->SetSize(glm::ivec2(currentSize.x, verticalSize));

                childrenPos.push_back(currentPos.x);
                childrenSize.push_back(currentSize.x);
                childrenMinSize.push_back(currentMinSize.x);
                childrenMaxSize.push_back(currentMaxSize.x);  
            }
            else
            {
                if(currentPos.x != containerPos.x + GetPadding())
                    Container->GetCurrentChild()->SetGlobalPosition(glm::ivec2(containerPos.x + GetPadding(), currentPos.y));
            
                if(currentSize.x != containerSize.x - GetPadding() * 2)
                    Container->GetCurrentChild()->SetSize(glm::ivec2(containerSize.x - GetPadding() * 2, currentSize.y));

                childrenPos.push_back(currentPos.y);
                childrenSize.push_back(currentSize.y);
                childrenMinSize.push_back(currentMinSize.y);
                childrenMaxSize.push_back(currentMaxSize.y);
            }

            Container->MoveChildrenIteratorNext();
        }
        FUNC_DEBUG_EXIT();
    }

    void Layout::GetLastDifferentChild(std::vector<int>& childrenPos, std::vector<int>& childrenSize, int& sizeDiff, int& lastChildChangeIndex)
    {
        FUNC_DEBUG_ENTRY();
        //There are children added or deleted. No resizing is done
        if(LastUpdateChildrenSize.size() != childrenPos.size())
        {
            lastChildChangeIndex = -1;
            sizeDiff = 0;
            FUNC_DEBUG_EXIT();
            return;
        }

        int itIndex = childrenPos.size() - 1;
        
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
                    sizeDiff = childrenSize[itIndex] - LastUpdateChildrenSize[childIndex].x;
                    lastChildChangeIndex = itIndex;
                }
                else if(!IsHorizontalLayout() && LastUpdateChildrenSize[childIndex].y != childrenSize[itIndex])
                {
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
        FUNC_DEBUG_EXIT();
    }

    void Layout::AssignPositionsAndSizesToChildren(std::vector<int>& childrenPos, std::vector<int>& childrenSize)
    {
        FUNC_DEBUG_ENTRY();
        int index = 0;
        Container->MoveChildrenIteratorToFirst();
        LastUpdateChildrenSize = std::unordered_map<ssGUIObjectIndex, glm::ivec2>();
        while(!Container->IsChildrenIteratorEnd())
        {
            ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(Container->GetCurrentChild());
            
            if(childIndex != -1 && ObjectsToExclude.find(childIndex) != ObjectsToExclude.end())
            {
                Container->MoveChildrenIteratorNext();
                continue;
            }
            
            glm::ivec2 currentPos = Container->GetCurrentChild()->GetGlobalPosition();
            glm::ivec2 currentSize = Container->GetCurrentChild()->GetSize();
            glm::ivec2 currentMinSize = Container->GetCurrentChild()->GetMinSize();
            
            if(IsHorizontalLayout())
            {
                Container->GetCurrentChild()->SetSize(glm::ivec2(childrenSize[index], currentSize.y));
                Container->GetCurrentChild()->SetGlobalPosition(glm::ivec2(childrenPos[index], currentPos.y));
            }
            else
            {
                Container->GetCurrentChild()->SetSize(glm::ivec2(currentSize.x, childrenSize[index]));
                Container->GetCurrentChild()->SetGlobalPosition(glm::ivec2(currentPos.x, childrenPos[index]));
            }
            
            //Update ChildrenSize
            if(childIndex == -1)
                childIndex = CurrentObjectsReferences.AddObjectReference(Container->GetCurrentChild());
            LastUpdateChildrenSize[childIndex] = Container->GetCurrentChild()->GetSize();
            
            index++;
            Container->MoveChildrenIteratorNext();
        }
        FUNC_DEBUG_EXIT();
    }

    void Layout::ConstructRenderInfo()
    {}

    void Layout::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {}

    const std::string Layout::EXTENSION_NAME = "Layout";

    Layout::Layout() : HorizontalLayout(false), PreferredSizeMultipliers(), DisableChildrenResizing(false), 
                        OverrideChildrenResizeTypes(true), UpdateContainerMinMaxSize(true), ReverseOrder(false), CoverFullLength(true),
                        Container(nullptr), Enabled(true), Padding(0/*5*/), Spacing(5), Overflow(false), OnChildAddEventIndex(-1), ChildAddedEventIndex(-1), 
                        ChildRemovedEventIndex(-1), ChildPositionChangedEventIndex(-1), CurrentObjectsReferences(), LastUpdateChildrenSize(), 
                        ObjectsToExclude(), SpecialObjectsToExclude(), OriginalChildrenSize(), OriginalChildrenResizeType(), MinMaxSizeChangedEventIndices()
    {}

    Layout::~Layout()
    {
        if(Container != nullptr)
        {
            auto eventCallbackCleanUp = [&](ssGUI::GUIObject* target, std::string eventCallbackName, int removeIndex)
            {
                target->GetEventCallback(eventCallbackName)->RemoveEventListener(removeIndex);
            
                if(target->GetEventCallback(eventCallbackName)->GetEventListenerCount() == 0)
                    target->RemoveEventCallback(eventCallbackName);
            };

            eventCallbackCleanUp(Container, ssGUI::EventCallbacks::OnRecursiveChildAddEventCallback::EVENT_NAME, OnChildAddEventIndex);
            eventCallbackCleanUp(Container, ssGUI::EventCallbacks::RecursiveChildAddedEventCallback::EVENT_NAME, ChildAddedEventIndex);
            eventCallbackCleanUp(Container, ssGUI::EventCallbacks::RecursiveChildRemovedEventCallback::EVENT_NAME, ChildRemovedEventIndex);
            eventCallbackCleanUp(Container, ssGUI::EventCallbacks::ChildPositionChangedEventCallback::EVENT_NAME, ChildPositionChangedEventIndex);
        
            for(auto it : MinMaxSizeChangedEventIndices)
            {
                ssGUI::GUIObject* obj = CurrentObjectsReferences.GetObjectReference(it.first);

                if(obj != nullptr)
                    eventCallbackCleanUp(obj, ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback::EVENT_NAME, it.second);
            }
        }
        CurrentObjectsReferences.CleanUp();
    }

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

    

    bool Layout::GetOverrideChildrenResizeType() const
    {
        return OverrideChildrenResizeTypes;
    }

    void Layout::SetOverrideChildrenResizeType(bool override)
    {
        FUNC_DEBUG_ENTRY();
        
        OverrideChildrenResizeTypes = override;
        
        if(Container == nullptr)
        {
            FUNC_DEBUG_EXIT();
            return;
        }
        
        const std::string onChildAddedEventName = ssGUI::EventCallbacks::RecursiveChildAddedEventCallback::EVENT_NAME;
        const std::string onChildRemovedEventName = ssGUI::EventCallbacks::RecursiveChildRemovedEventCallback::EVENT_NAME;
        UpdateChildrenResizeTypes();

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
                {
                    Container->GetCurrentChild()->AddEventCallback(static_cast<ssGUI::EventCallbacks::EventCallback*>(
                        new ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback()));
                }

                if(MinMaxSizeChangedEventIndices.find(currentChildIndex) == MinMaxSizeChangedEventIndices.end())
                {
                    MinMaxSizeChangedEventIndices[currentChildIndex] = Container->GetCurrentChild()->GetEventCallback(onMinMaxSizeChangedEventName)->AddEventListener(
                        std::bind(&ssGUI::Extensions::Layout::Internal_OnChildMinMaxSizeChanged, this, std::placeholders::_1));
                }

                Container->MoveChildrenIteratorNext();
            }

            SyncContainerMinMaxSize();
        }
        else
        {            
            //Iterate each child to see if event callback exists
            Container->MoveChildrenIteratorToFirst();
            while(!Container->IsChildrenIteratorEnd())
            {
                ssGUIObjectIndex currentChildIndex = CurrentObjectsReferences.GetObjectIndex(Container->GetCurrentChild());

                //Add the child to reference if not present
                if(currentChildIndex == -1)
                    currentChildIndex = CurrentObjectsReferences.AddObjectReference(Container->GetCurrentChild());
                
                if(Container->GetCurrentChild()->IsEventCallbackExist(onMinMaxSizeChangedEventName) 
                    && MinMaxSizeChangedEventIndices.find(currentChildIndex) != MinMaxSizeChangedEventIndices.end())
                {
                    Container->GetCurrentChild()->GetEventCallback(onMinMaxSizeChangedEventName)->RemoveEventListener(MinMaxSizeChangedEventIndices[currentChildIndex]);
                    
                    //If we were the only one using the event callback, remove it
                    if(Container->GetCurrentChild()->GetEventCallback(onMinMaxSizeChangedEventName)->GetEventListenerCount() == 0)
                        Container->GetCurrentChild()->RemoveEventCallback(onMinMaxSizeChangedEventName);

                    MinMaxSizeChangedEventIndices.erase(currentChildIndex);
                }

                Container->MoveChildrenIteratorNext();
            }
        }

        FUNC_DEBUG_EXIT();
    }

    int Layout::GetPadding() const
    {
        return Padding;
    }

    void Layout::SetPadding(int padding)
    {
        Padding = padding;
    }

    int Layout::GetSpacing() const
    {
        return Spacing;
    }

    void Layout::SetSpacing(int spacing)
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

        if(GetOverrideChildrenResizeType())
            UpdateChildrenResizeTypes();
        
        if(GetUpdateContainerMinMaxSize())
        {
            SyncContainerMinMaxSize();
            const std::string onMinMaxSizeChangedEventName = ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback::EVENT_NAME;
            
            //Add MinMax size changed callback
            if(!child->IsEventCallbackExist(onMinMaxSizeChangedEventName))
            {
                child->AddEventCallback(static_cast<ssGUI::EventCallbacks::EventCallback*>(
                    new ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback()));
            }
            
            if(MinMaxSizeChangedEventIndices.find(childIndex) == MinMaxSizeChangedEventIndices.end())
            {
                MinMaxSizeChangedEventIndices[childIndex] = child->GetEventCallback(onMinMaxSizeChangedEventName)->
                    AddEventListener(std::bind(&ssGUI::Extensions::Layout::Internal_OnChildMinMaxSizeChanged, this, std::placeholders::_1));
            }
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

        //Restore resize type if it is recorded
        if(OriginalChildrenResizeType.find(childIndex) != OriginalChildrenResizeType.end())
        {
            static_cast<ssGUI::Window*>(child)->SetResizeType(OriginalChildrenResizeType[childIndex]);
            OriginalChildrenResizeType.erase(childIndex);
        }

        //Remove MinMax size changed callbacks
        const std::string onMinMaxSizeChangedEventName = ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback::EVENT_NAME;
        if(child->IsEventCallbackExist(onMinMaxSizeChangedEventName) 
            && MinMaxSizeChangedEventIndices.find(childIndex) != MinMaxSizeChangedEventIndices.end())
        {                
            child->GetEventCallback(onMinMaxSizeChangedEventName)->RemoveEventListener(MinMaxSizeChangedEventIndices[childIndex]);
            if(child->GetEventCallback(onMinMaxSizeChangedEventName)->GetEventListenerCount() == 0)
                child->RemoveEventCallback(onMinMaxSizeChangedEventName);
            MinMaxSizeChangedEventIndices.erase(childIndex);
        }

        if(GetUpdateContainerMinMaxSize())
            SyncContainerMinMaxSize();

        if(GetOverrideChildrenResizeType())
            UpdateChildrenResizeTypes();

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
    void Layout::Internal_Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();
        
        if(IsPreUpdate || Container == nullptr || Container->GetChildrenCount() == 0 || !Enabled)
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
        Container->MoveChildrenIteratorToFirst();
        while(!Container->IsChildrenIteratorEnd())
        {
            ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(Container->GetCurrentChild());
            if(childIndex != -1 && ObjectsToExclude.find(childIndex) != ObjectsToExclude.end())
                excludeCount++;

            Container->MoveChildrenIteratorNext();
        }
        
        //Check preffered size multipliers length
        if(PreferredSizeMultipliers.size() != Container->GetChildrenCount() - excludeCount)
            PreferredSizeMultipliers.resize(Container->GetChildrenCount() - excludeCount, -1);
        
        //Disable children resizing
        if(DisableChildrenResizing)
            DisableChildrenResizingInUpdate();
        
        //TODO : Probably don't need this as events will sort this out
        if(UpdateContainerMinMaxSize)
            SyncContainerMinMaxSize();

        //Set all children's width and get all children pos and size and min size
        std::vector<int> childrenPos;
        std::vector<int> childrenSize;
        std::vector<int> childrenMinSize;
        std::vector<int> childrenMaxSize;

        glm::ivec2 containerPos = Container->GetGlobalPosition();
        glm::ivec2 containerSize = Container->GetSize();
        
        GetAndValidateChildrenDetails(childrenPos, childrenSize, childrenMinSize, childrenMaxSize, containerPos, containerSize);

        //Check if size is different from last update (Iterating from back to front so to get the first child with size change)
        int lastChildChangeIndex = -1;
        int sizeDiff = 0;

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

    void Layout::Internal_Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
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
        if(GetOverrideChildrenResizeType())
            SetOverrideChildrenResizeType(true);
        
        if(GetUpdateContainerMinMaxSize())
            SetUpdateContainerMinMaxSize(true);

        if(!Container->IsEventCallbackExist(ssGUI::EventCallbacks::OnRecursiveChildAddEventCallback::EVENT_NAME))
            Container->AddEventCallback(new ssGUI::EventCallbacks::OnRecursiveChildAddEventCallback());

        OnChildAddEventIndex = static_cast<ssGUI::EventCallbacks::OnRecursiveChildAddEventCallback*>(
            Container->GetEventCallback(ssGUI::EventCallbacks::OnRecursiveChildAddEventCallback::EVENT_NAME))->AddEventListener
            (
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
                    FUNC_DEBUG_EXIT("OnRecursiveChildAddEventCallback");
                }
            );

        const std::string onChildAddedEventName = ssGUI::EventCallbacks::RecursiveChildAddedEventCallback::EVENT_NAME;
        const std::string onChildRemovedEventName = ssGUI::EventCallbacks::RecursiveChildRemovedEventCallback::EVENT_NAME;
        const std::string childPositionChangedEventName = ssGUI::EventCallbacks::ChildPositionChangedEventCallback::EVENT_NAME;

        if(!Container->IsEventCallbackExist(onChildAddedEventName))
            Container->AddEventCallback(new ssGUI::EventCallbacks::RecursiveChildAddedEventCallback());
        
        if(!Container->IsEventCallbackExist(onChildRemovedEventName))
            Container->AddEventCallback(new ssGUI::EventCallbacks::RecursiveChildRemovedEventCallback());

        if(!Container->IsEventCallbackExist(childPositionChangedEventName))
            Container->AddEventCallback(new ssGUI::EventCallbacks::ChildPositionChangedEventCallback());

        ChildAddedEventIndex = Container->GetEventCallback(onChildAddedEventName)->AddEventListener(
            std::bind(&ssGUI::Extensions::Layout::Internal_OnRecursiveChildAdded, this, std::placeholders::_1));
        ChildRemovedEventIndex = Container->GetEventCallback(onChildRemovedEventName)->AddEventListener(
            std::bind(&ssGUI::Extensions::Layout::Internal_OnRecursiveChildRemoved, this, std::placeholders::_1));

        ChildPositionChangedEventIndex = Container->GetEventCallback(childPositionChangedEventName)->AddEventListener
            (
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

                    if(containerLayout->GetOverrideChildrenResizeType())
                        containerLayout->UpdateChildrenResizeTypes();
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
        OverrideChildrenResizeTypes = layout->GetOverrideChildrenResizeType();
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

    Extension* Layout::Clone(ssGUI::GUIObject* newContainer)
    {
        Layout* temp = new Layout(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        
        return temp;
    }
}