#include "ssGUI/Extensions/Layout.hpp"

namespace ssGUI::Extensions
{    
    void Layout::LayoutChildren(int startPos, int length, std::vector<int>& childrenPos, std::vector<int>& childrenLength, 
                                std::vector<int>& minChildrenLength, std::vector<int>& maxChildrenLength, int lastChildChangeIndex,
                                int sizeDiff)
    {        
        FUNC_DEBUG_LINE("Entry");
        
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

            if(remainingLength - currentLength < remainingMinLengths - minChildrenLength[i])
            {
                currentLength = remainingLength - (remainingMinLengths - minChildrenLength[i]);
            }

            childrenLength[i] = currentLength;
            remainingMinLengths -= minChildrenLength[i];
            remainingLength -= currentLength;
        }

        //Reiterate again to cover full length
        if(CoverFullLength && remainingLength > 0)
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

        FUNC_DEBUG_LINE("Exit");
    }

    Layout::Layout(Layout const& other)
    {
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
        OnChildAddEventIndex = -1;
        ChildAddedEventIndex = -1;
        ChildRemovedEventIndex = -1;
        ChildPositionChangedEventIndex = -1;
        CurrentObjectsReferences = other.CurrentObjectsReferences;
        LastUpdateChildrenSize = other.LastUpdateChildrenSize;//std::unordered_map<ssGUIObjectIndex, glm::ivec2>();
        OriginalChildrenSize = other.OriginalChildrenSize;//std::unordered_map<ssGUIObjectIndex, glm::ivec2>();
        OriginalChildrenResizeType = other.OriginalChildrenResizeType;//std::unordered_map<ssGUIObjectIndex, ssGUI::Enums::ResizeType>();
        
        //TODO : Re-add the event listeners
        MinMaxSizeChangedEventIndices = std::unordered_map<ssGUIObjectIndex, int>();//other.MinMaxSizeChangedEventIndices;
    }

    const std::string Layout::EXTENSION_NAME = "Layout";

    Layout::Layout() : HorizontalLayout(false), PreferredSizeMultipliers(), DisableChildrenResizing(false), 
                        OverrideChildrenResizeTypes(true), UpdateContainerMinMaxSize(true), ReverseOrder(false), CoverFullLength(true),
                        Container(nullptr), Enabled(true), Padding(5), Spacing(5), OnChildAddEventIndex(-1), ChildAddedEventIndex(-1), 
                        ChildRemovedEventIndex(-1), ChildPositionChangedEventIndex(-1), CurrentObjectsReferences(), LastUpdateChildrenSize(), 
                        ObjectsToExclude(), SpecialObjectsToExclude(), OriginalChildrenSize(), OriginalChildrenResizeType(), MinMaxSizeChangedEventIndices()
    {}

    Layout::~Layout()
    {}

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

    void Layout::UpdateChildrenResizeTypes()
    {
        FUNC_DEBUG_LINE("Entry");
        
        if(Container == nullptr)
        {
            FUNC_DEBUG_LINE("Exit");
            return;
        }
        
        if(GetOverrideChildrenResizeType())
        {
            if(DisableChildrenResizing)
            {
                FUNC_DEBUG_LINE("Exit");
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

        FUNC_DEBUG_LINE("Exit");
    }

    void Layout::SyncContainerMinMaxSize()
    {
        FUNC_DEBUG_LINE("Entry");
        
        if(Container == nullptr || Container->GetChildrenCount() - ObjectsToExclude.size() == 0)
        {
            FUNC_DEBUG_LINE("Exit");
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

        FUNC_DEBUG_LINE("Exit");
    }

    bool Layout::GetOverrideChildrenResizeType() const
    {
        return OverrideChildrenResizeTypes;
    }

    void Layout::SetOverrideChildrenResizeType(bool override)
    {
        FUNC_DEBUG_LINE("Entry");
        
        OverrideChildrenResizeTypes = override;
        
        if(Container == nullptr)
        {
            FUNC_DEBUG_LINE("Exit");
            return;
        }
        
        const std::string onChildAddedEventName = ssGUI::EventCallbacks::RecursiveChildrenAddedEventCallback::EVENT_NAME;
        const std::string onChildRemovedEventName = ssGUI::EventCallbacks::RecursiveChildrenRemovedEventCallback::EVENT_NAME;

        // if(override)
        // {
        //     //If callback doesn't exist, add it
        //     if(!Container->IsEventCallbackExist(onChildAddedEventName))
        //     {
        //         Container->AddEventCallback(static_cast<ssGUI::EventCallbacks::EventCallback*>(
        //             new ssGUI::EventCallbacks::RecursiveChildrenAddedEventCallback()));
        //     }
            
        //     //If event index is not assigned, assign it
        //     if(ChildAddedEventIndex == -1)
        //     {
        //         ChildAddedEventIndex = Container->GetEventCallback(onChildAddedEventName)->AddEventListener(
        //             std::bind(&ssGUI::Extensions::Layout::Internal_OnRecursiveChildAdded, this, std::placeholders::_1));
        //     }
            
        //     if(!Container->IsEventCallbackExist(onChildRemovedEventName))
        //     {
        //         Container->AddEventCallback(static_cast<ssGUI::EventCallbacks::EventCallback*>(
        //             new ssGUI::EventCallbacks::RecursiveChildrenRemovedEventCallback()));
        //     }
            
        //     if(ChildRemovedEventIndex == -1)
        //     {
        //         ChildRemovedEventIndex = Container->GetEventCallback(onChildRemovedEventName)->AddEventListener(
        //             std::bind(&ssGUI::Extensions::Layout::Internal_OnRecursiveChildRemoved, this, std::placeholders::_1));
        //     }
        // }
        // else
        // {
        //     //If callback exists, check if event index exists. 
        //     //If so remove event listener and remove the callback if there's no other listeners. Then reset the event index
        //     if(Container->IsEventCallbackExist(onChildAddedEventName) && ChildAddedEventIndex != -1)
        //     {
        //         Container->GetEventCallback(onChildAddedEventName)->RemoveEventListener(ChildAddedEventIndex);

        //         if(Container->GetEventCallback(onChildAddedEventName)->GetEventListenerCount() == 0)
        //             Container->RemoveEventCallback(onChildAddedEventName);
                
        //         ChildAddedEventIndex = -1;
        //     }

        //     if(Container->IsEventCallbackExist(onChildRemovedEventName) && ChildRemovedEventIndex != -1)
        //     {
        //         Container->GetEventCallback(onChildRemovedEventName)->RemoveEventListener(ChildRemovedEventIndex);

        //         if(Container->GetEventCallback(onChildRemovedEventName)->GetEventListenerCount() == 0)
        //             Container->RemoveEventCallback(onChildRemovedEventName);
                
        //         ChildRemovedEventIndex = -1;
        //     }
        // }

        UpdateChildrenResizeTypes();

        FUNC_DEBUG_LINE("Exit");
    }

    bool Layout::GetUpdateContainerMinMaxSize() const
    {
        return UpdateContainerMinMaxSize;
    }

    void Layout::SetUpdateContainerMinMaxSize(bool update)
    {
        FUNC_DEBUG_LINE("Entry");
        
        UpdateContainerMinMaxSize = update;
        
        if(Container == nullptr)
        {
            FUNC_DEBUG_LINE("Exit");
            return;
        }

        const std::string onChildAddedEventName = ssGUI::EventCallbacks::RecursiveChildrenAddedEventCallback::EVENT_NAME;
        const std::string onChildRemovedEventName = ssGUI::EventCallbacks::RecursiveChildrenRemovedEventCallback::EVENT_NAME;
        const std::string onMinMaxSizeChangedEventName = ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback::EVENT_NAME;

        if(update)
        {
            // //If callback doesn't exist, add it
            // if(!Container->IsEventCallbackExist(onChildAddedEventName))
            // {
            //     Container->AddEventCallback(static_cast<ssGUI::EventCallbacks::EventCallback*>(
            //         new ssGUI::EventCallbacks::RecursiveChildrenAddedEventCallback()));
            // }
            
            // //If event index is not assigned, assign it
            // if(ChildAddedEventIndex == -1)
            // {
            //     ChildAddedEventIndex = Container->GetEventCallback(onChildAddedEventName)->AddEventListener(
            //         std::bind(&ssGUI::Extensions::Layout::Internal_OnRecursiveChildAdded, this, std::placeholders::_1));
            // }
            
            // if(!Container->IsEventCallbackExist(onChildRemovedEventName))
            // {
            //     Container->AddEventCallback(static_cast<ssGUI::EventCallbacks::EventCallback*>(
            //         new ssGUI::EventCallbacks::RecursiveChildrenRemovedEventCallback()));
            // }
            
            // if(ChildRemovedEventIndex == -1)
            // {
            //     ChildRemovedEventIndex = Container->GetEventCallback(onChildRemovedEventName)->AddEventListener(
            //         std::bind(&ssGUI::Extensions::Layout::Internal_OnRecursiveChildRemoved, this, std::placeholders::_1));
            // }

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
            // //If callback exists, check if event index exists. 
            // //If so remove event listener and remove the callback if there's no other listeners. Then reset the event index
            // if(Container->IsEventCallbackExist(onChildAddedEventName) && ChildAddedEventIndex != -1)
            // {
            //     Container->GetEventCallback(onChildAddedEventName)->RemoveEventListener(ChildAddedEventIndex);

            //     if(Container->GetEventCallback(onChildAddedEventName)->GetEventListenerCount() == 0)
            //         Container->RemoveEventCallback(onChildAddedEventName);
                
            //     ChildAddedEventIndex = -1;
            // }

            // if(Container->IsEventCallbackExist(onChildRemovedEventName) && ChildRemovedEventIndex != -1)
            // {
            //     Container->GetEventCallback(onChildRemovedEventName)->RemoveEventListener(ChildRemovedEventIndex);

            //     if(Container->GetEventCallback(onChildRemovedEventName)->GetEventListenerCount() == 0)
            //         Container->RemoveEventCallback(onChildRemovedEventName);
                
            //     ChildRemovedEventIndex = -1;
            // }
            
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

        FUNC_DEBUG_LINE("Exit");
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

    void Layout::ExcludeObject(ssGUI::GUIObject* obj)
    {
        ssGUIObjectIndex objIndex = CurrentObjectsReferences.GetObjectIndex(obj);

        //Add the child to reference if not present
        if(objIndex == -1)
            objIndex = CurrentObjectsReferences.AddObjectReference(obj);
        
        if(ObjectsToExclude.find(objIndex) == ObjectsToExclude.end())
        {
            ObjectsToExclude.insert(objIndex);
        }
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
        FUNC_DEBUG_LINE("Entry");
        
        if(child->GetParent() != Container)
        {
            FUNC_DEBUG_LINE("Exit");
            return;
        }
        
        ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(child);
        
        //Add the child to reference if not present
        if(childIndex == -1)
            childIndex = CurrentObjectsReferences.AddObjectReference(child);
        
        if(ObjectsToExclude.find(childIndex) != ObjectsToExclude.end())
        {
            FUNC_DEBUG_LINE("Exit");
            return;
        }
        
        if(child->HasTag(ssGUI::Tags::OVERLAY) || child->HasTag(ssGUI::Tags::FLOATING))
        {
            ObjectsToExclude.insert(childIndex);
            SpecialObjectsToExclude.insert(childIndex);
            FUNC_DEBUG_LINE("Exit");
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
                MinMaxSizeChangedEventIndices[childIndex] = child->GetEventCallback(onMinMaxSizeChangedEventName)->AddEventListener(
                    std::bind(&ssGUI::Extensions::Layout::Internal_OnChildMinMaxSizeChanged, this, std::placeholders::_1));
                DEBUG_LINE("childIndex added: "<<this<<", "<<childIndex<<", "<<MinMaxSizeChangedEventIndices[childIndex]);
            }
        }

        FUNC_DEBUG_LINE("Exit");
    }

    void Layout::Internal_OnRecursiveChildRemoved(ssGUI::GUIObject* child)
    {        
        FUNC_DEBUG_LINE("Entry");

        ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(child);

        //If not present, no need to continue
        if(childIndex == -1)
        {
            FUNC_DEBUG_LINE("Exit");
            return;
        }

        //If this is one of the excluding objects, remove it to prevent invalid object pointer
        if(ObjectsToExclude.find(childIndex) != ObjectsToExclude.end())
        {
            ObjectsToExclude.erase(childIndex);

            if(SpecialObjectsToExclude.find(childIndex) != SpecialObjectsToExclude.end())
                SpecialObjectsToExclude.erase(childIndex);

            FUNC_DEBUG_LINE("Exit");
            return;
        }

        //Restore resize type if it is recorded
        if(OriginalChildrenResizeType.find(childIndex) != OriginalChildrenResizeType.end())
        {
            static_cast<ssGUI::Window*>(child)->SetResizeType(OriginalChildrenResizeType[childIndex]);

            OriginalChildrenResizeType.erase(childIndex);
        }

        if(GetUpdateContainerMinMaxSize())
            SyncContainerMinMaxSize();

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

        if(GetOverrideChildrenResizeType())
            UpdateChildrenResizeTypes();

        if(OriginalChildrenSize.find(childIndex) != OriginalChildrenSize.end())
        {
            child->SetSize(OriginalChildrenSize[childIndex]);
            OriginalChildrenSize.erase(childIndex);   
        }

        //Remove the object reference to this child as it is no longer needed to be maintained
        CurrentObjectsReferences.RemoveObjectReference(childIndex);

        FUNC_DEBUG_LINE("Exit");
    }

    void Layout::Internal_OnChildMinMaxSizeChanged(ssGUI::GUIObject* child)
    {        
        ssGUIObjectIndex childIndex = CurrentObjectsReferences.GetObjectIndex(child);

        //Add the child to reference if not present
        if(childIndex == -1)
            childIndex = CurrentObjectsReferences.AddObjectReference(child);
        
        if(ObjectsToExclude.find(childIndex) != ObjectsToExclude.end())
            return;
        
        if(GetUpdateContainerMinMaxSize())
        {
            SyncContainerMinMaxSize();
        }
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
    void Layout::Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_LINE("Entry");
        
        if(IsPreUpdate || Container == nullptr || Container->GetChildrenCount() == 0 || !Enabled)
        {
            FUNC_DEBUG_LINE("Exit");
            return;
        }

        //Update SpecialObjectsToExclude if needed
        std::vector<ssGUIObjectIndex> objsToRemove;
        for(auto it : SpecialObjectsToExclude)
        {
            if(!CurrentObjectsReferences.GetObjectReference(it)->HasTag(ssGUI::Tags::OVERLAY) && 
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

        //Update excluded objects if there's an overlay tag
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
        
        //Check if object to exclude has valid parents
        int excludeCount = 0;
        std::vector<ssGUIObjectIndex> objToRemove;
        for(auto it = ObjectsToExclude.begin(); it != ObjectsToExclude.end(); it++)
        {
            if(CurrentObjectsReferences.GetObjectReference(*it)->GetParent() != Container)
                objToRemove.push_back(*it);
            else
                excludeCount++;
        }
        
        for(auto objIndex : objToRemove)
        {
            ObjectsToExclude.erase(objIndex);
            if(SpecialObjectsToExclude.find(objIndex) != SpecialObjectsToExclude.end())
                SpecialObjectsToExclude.erase(objIndex);
            
            //Don't need to hold reference for it as it is not a child
            CurrentObjectsReferences.RemoveObjectReference(objIndex);
        }
        
        //Check preffered size multipliers length
        if(PreferredSizeMultipliers.size() != Container->GetChildrenCount() - excludeCount)
        {
            PreferredSizeMultipliers.resize(Container->GetChildrenCount() - excludeCount, -1);
        }
        
        //Disable children resizing
        if(DisableChildrenResizing)
        {
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
        }
        
        //TODO : Probably don't need this as events will sort this out
        if(UpdateContainerMinMaxSize)
            SyncContainerMinMaxSize();

        //Set all children's width and get all children pos and size and min size
        std::vector<int> childrenPos;
        std::vector<int> childrenSize;
        std::vector<int> childrenMinSize;
        std::vector<int> childrenMaxSize;
        int lastChildChangeIndex = -1;
        int sizeDiff = 0;

        glm::ivec2 ContainerPos = Container->GetGlobalPosition();
        glm::ivec2 containerSize = Container->GetSize();
        
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
                int verticalPos = Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW ? 
                                    ContainerPos.y + dynamic_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() :
                                    ContainerPos.y + GetPadding();     

                int verticalSize = Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW ? 
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
                if(currentPos.x != ContainerPos.x + GetPadding())
                    Container->GetCurrentChild()->SetGlobalPosition(glm::ivec2(ContainerPos.x + GetPadding(), currentPos.y));
            
                if(currentSize.x != containerSize.x - GetPadding() * 2)
                    Container->GetCurrentChild()->SetSize(glm::ivec2(containerSize.x - GetPadding() * 2, currentSize.y));

                childrenPos.push_back(currentPos.y);
                childrenSize.push_back(currentSize.y);
                childrenMinSize.push_back(currentMinSize.y);
                childrenMaxSize.push_back(currentMaxSize.y);
            }

            Container->MoveChildrenIteratorNext();
        }
        
        //Check if size is different from last update (Iterating from back to front so to get the first child with size change)
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

            if(childIndex == -1)
                childIndex = CurrentObjectsReferences.AddObjectReference(Container->GetCurrentChild());
            
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

            itIndex--;
            Container->MoveChildrenIteratorPrevious();
        }
        
        //Check for window for laying out children differently
        if(Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && static_cast<ssGUI::Window*>(Container)->HasTitlebar())
        {
            if(IsHorizontalLayout())
            {
                LayoutChildren( ContainerPos.x + GetPadding(), 
                                containerSize.x - GetPadding() * 2, 
                                childrenPos, childrenSize, childrenMinSize, 
                                childrenMaxSize, lastChildChangeIndex, sizeDiff);
            }
            else
            {
                LayoutChildren( ContainerPos.y + ((ssGUI::Window*)Container)->GetTitlebarHeight(), 
                                containerSize.y - ((ssGUI::Window*)Container)->GetTitlebarHeight() - GetPadding(), 
                                childrenPos, childrenSize, childrenMinSize, childrenMaxSize, lastChildChangeIndex, sizeDiff);
            }   
        }
        else
        {
            if(IsHorizontalLayout())
                LayoutChildren(ContainerPos.x + GetPadding(), containerSize.x - GetPadding() * 2, 
                                childrenPos, childrenSize, childrenMinSize, childrenMaxSize, lastChildChangeIndex, sizeDiff);
            else
                LayoutChildren(ContainerPos.y + GetPadding(), containerSize.y - GetPadding() * 2, 
                                childrenPos, childrenSize, childrenMinSize, childrenMaxSize, lastChildChangeIndex, sizeDiff);
        }
        
        //Assigning position and size to children
        int index = 0;
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

        FUNC_DEBUG_LINE("Exit");
    }

    void Layout::Internal_Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {
        //Internal_Draw nothing
    }

    std::string Layout::GetExtensionName()
    {
        return EXTENSION_NAME;
    }

    void Layout::BindToObject(ssGUI::GUIObject* bindObj)
    {
        FUNC_DEBUG_LINE("Entry");
        
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
                [&](ssGUI::GUIObject* child)
                {
                    int childIndex = CurrentObjectsReferences.IsObjectReferenceExist(child) ? CurrentObjectsReferences.GetObjectIndex(child) : CurrentObjectsReferences.AddObjectReference(child);
                    
                    //Record the original size if not exists (Can't be done in OnChildAdded as that's after the child being added)
                    if(OriginalChildrenSize.find(childIndex) == OriginalChildrenSize.end())
                        OriginalChildrenSize[childIndex] = child->GetSize();
                }
            );

        const std::string onChildAddedEventName = ssGUI::EventCallbacks::RecursiveChildrenAddedEventCallback::EVENT_NAME;
        const std::string onChildRemovedEventName = ssGUI::EventCallbacks::RecursiveChildrenRemovedEventCallback::EVENT_NAME;
        const std::string childPositionChangedEventName = ssGUI::EventCallbacks::ChildPositionChangedEventCallback::EVENT_NAME;

        if(!Container->IsEventCallbackExist(onChildAddedEventName))
            Container->AddEventCallback(new ssGUI::EventCallbacks::RecursiveChildrenAddedEventCallback());
        
        if(!Container->IsEventCallbackExist(onChildRemovedEventName))
            Container->AddEventCallback(new ssGUI::EventCallbacks::RecursiveChildrenRemovedEventCallback());

        if(!Container->IsEventCallbackExist(childPositionChangedEventName))
            Container->AddEventCallback(new ssGUI::EventCallbacks::ChildPositionChangedEventCallback());

        ChildAddedEventIndex = Container->GetEventCallback(onChildAddedEventName)->AddEventListener(
            std::bind(&ssGUI::Extensions::Layout::Internal_OnRecursiveChildAdded, this, std::placeholders::_1));
        ChildRemovedEventIndex = Container->GetEventCallback(onChildRemovedEventName)->AddEventListener(
            std::bind(&ssGUI::Extensions::Layout::Internal_OnRecursiveChildRemoved, this, std::placeholders::_1));

        ChildPositionChangedEventIndex = Container->GetEventCallback(childPositionChangedEventName)->AddEventListener
            (
                [&](ssGUI::GUIObject* obj)
                {
                    if(GetOverrideChildrenResizeType())
                        UpdateChildrenResizeTypes();
                }
            );

        FUNC_DEBUG_LINE("Exit");
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