#include "ssGUI/Extensions/Layout.hpp"

namespace ssGUI::Extensions
{    
    void Layout::LayoutChildren(int startPos, int length, std::vector<int>& childrenPos, std::vector<int>& childrenLength, 
                                std::vector<int>& minChildrenLength, std::vector<int>& maxChildrenLength, int lastChildChangeIndex,
                                int sizeDiff)
    {        
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
        OnChildAddedEventIndex = -1;
        OnChildRemovedEventIndex = -1;
        LastUpdateChildrenSize = std::unordered_map<ssGUI::GUIObject*, glm::ivec2>();
        ObjectsToExclude = other.ObjectsToExclude;
        OriginalChildrenSize = std::unordered_map<ssGUI::GUIObject*, glm::ivec2>();
        OriginalChildrenResizeType = std::unordered_map<ssGUI::GUIObject*, ssGUI::Enums::ResizeType>();
        MinMaxSizeChangedEventIndices = std::unordered_map<ssGUI::GUIObject*, int>();
    }

    const std::string Layout::EXTENSION_NAME = "Layout";

    Layout::Layout() : HorizontalLayout(false), PreferredSizeMultipliers(), DisableChildrenResizing(false), 
                        OverrideChildrenResizeTypes(true), UpdateContainerMinMaxSize(true), ReverseOrder(false), CoverFullLength(true),
                        Container(nullptr), Enabled(true), Padding(5), Spacing(5), OnChildAddedEventIndex(-1), OnChildRemovedEventIndex(-1), 
                        LastUpdateChildrenSize(), ObjectsToExclude(), OriginalChildrenSize(), OriginalChildrenResizeType(), MinMaxSizeChangedEventIndices()
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
        if(Container == nullptr)
            return;
        
        if(GetOverrideChildrenResizeType())
        {
            if(DisableChildrenResizing)
                return;
        }

        for(auto it = Container->GetChildrenStartIterator(); it != Container->GetChildrenEndIterator(); ++it)
        {            
            if(ObjectsToExclude.find(*it) != ObjectsToExclude.end())
                continue;
            
            if((*it)->GetType() != ssGUI::Enums::GUIObjectType::WINDOW)
                continue;

            ssGUI::Window* windowP = dynamic_cast<ssGUI::Window*>(*it);
            
            //Recording original resize type and override it
            if(GetOverrideChildrenResizeType())
            {
                if(OriginalChildrenResizeType.find(*it) == OriginalChildrenResizeType.end())
                    OriginalChildrenResizeType[(*it)] = windowP->GetResizeType();
                
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
                if(++it == Container->GetChildrenEndIterator())
                {
                    windowP->SetResizeType(ssGUI::Enums::ResizeType::NONE);
                }
                it--; //Decrement the iterator as it is incremented to check the last element
            }
            
            //Restoring original resize type
            if(!GetOverrideChildrenResizeType() && OriginalChildrenResizeType.find(*it) != OriginalChildrenResizeType.end())
            {                
                windowP->SetResizeType(OriginalChildrenResizeType[(*it)]);

                OriginalChildrenResizeType.erase(*it);
            }
        }
    }

    void Layout::SyncContainerMinMaxSize()
    {
        if(Container == nullptr || Container->GetChildrenCount() - ObjectsToExclude.size() == 0)
            return;
        
        if(HorizontalLayout)
        {
            int minSizeTotalX = 0;
            int maxSizeTotalX = 0;
            int minMaxY = std::numeric_limits<int>::max();
            int maxMinY = -1;
            
            for(auto it = Container->GetChildrenStartIterator(); it != Container->GetChildrenEndIterator(); it++)
            {
                if(ObjectsToExclude.find(*it) != ObjectsToExclude.end())
                    continue;
                
                minSizeTotalX += (*it)->GetMinSize().x;

                if(maxSizeTotalX == std::numeric_limits<int>::max())
                    continue;
                else if((*it)->GetMaxSize().x == std::numeric_limits<int>::max())
                    maxSizeTotalX = std::numeric_limits<int>::max();
                else
                    maxSizeTotalX += (*it)->GetMaxSize().x;
                
                if((*it)->GetMaxSize().y < minMaxY)
                    minMaxY = (*it)->GetMaxSize().y;

                if((*it)->GetMinSize().y > maxMinY)
                    maxMinY = (*it)->GetMinSize().y;
                
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
            
            for(auto it = Container->GetChildrenStartIterator(); it != Container->GetChildrenEndIterator(); it++)
            {
                if(ObjectsToExclude.find(*it) != ObjectsToExclude.end())
                    continue;
                
                minSizeTotalY += (*it)->GetMinSize().y;

                if(maxSizeTotalY == std::numeric_limits<int>::max())
                    continue;
                else if((*it)->GetMaxSize().y == std::numeric_limits<int>::max())
                    maxSizeTotalY = std::numeric_limits<int>::max();
                else
                    maxSizeTotalY += (*it)->GetMaxSize().y;
                
                if((*it)->GetMaxSize().x < minMaxX)
                    minMaxX = (*it)->GetMaxSize().x;

                if((*it)->GetMinSize().x > maxMinX)
                    maxMinX = (*it)->GetMinSize().x;
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
    }

    bool Layout::GetOverrideChildrenResizeType() const
    {
        return OverrideChildrenResizeTypes;
    }

    void Layout::SetOverrideChildrenResizeType(bool override)
    {
        OverrideChildrenResizeTypes = override;
        
        if(Container == nullptr)
            return;
        
        const std::string onChildAddedEventName = ssGUI::EventCallbacks::RecursiveChildrenAddedEventCallback::EVENT_NAME;
        const std::string onChildRemovedEventName = ssGUI::EventCallbacks::RecursiveChildrenRemovedEventCallback::EVENT_NAME;

        if(override)
        {
            //If callback doesn't exist, add it
            if(!Container->IsEventCallbackExist(onChildAddedEventName))
            {
                Container->AddEventCallback(static_cast<ssGUI::EventCallbacks::EventCallback*>(
                    new ssGUI::EventCallbacks::RecursiveChildrenAddedEventCallback()));
            }
            
            //If event index is not assigned, assign it
            if(OnChildAddedEventIndex == -1)
            {
                OnChildAddedEventIndex = Container->GetEventCallback(onChildAddedEventName)->AddEventListener(
                    std::bind(&ssGUI::Extensions::Layout::Internal_OnRecursiveChildAdded, this, std::placeholders::_1));
            }
            
            if(!Container->IsEventCallbackExist(onChildRemovedEventName))
            {
                Container->AddEventCallback(static_cast<ssGUI::EventCallbacks::EventCallback*>(
                    new ssGUI::EventCallbacks::RecursiveChildrenRemovedEventCallback()));
            }
            
            if(OnChildRemovedEventIndex == -1)
            {
                OnChildRemovedEventIndex = Container->GetEventCallback(onChildRemovedEventName)->AddEventListener(
                    std::bind(&ssGUI::Extensions::Layout::Internal_OnRecursiveChildRemoved, this, std::placeholders::_1));
            }

        }
        else
        {
            //If callback exists, check if event index exists. 
            //If so remove event listener and remove the callback if there's no other listeners. Then reset the event index
            if(Container->IsEventCallbackExist(onChildAddedEventName) && OnChildAddedEventIndex != -1)
            {
                Container->GetEventCallback(onChildAddedEventName)->RemoveEventListener(OnChildAddedEventIndex);

                if(Container->GetEventCallback(onChildAddedEventName)->GetEventListenerCount() == 0)
                    Container->RemoveEventCallback(onChildAddedEventName);
                
                OnChildAddedEventIndex = -1;
            }

            if(Container->IsEventCallbackExist(onChildRemovedEventName) && OnChildRemovedEventIndex != -1)
            {
                Container->GetEventCallback(onChildRemovedEventName)->RemoveEventListener(OnChildRemovedEventIndex);

                if(Container->GetEventCallback(onChildRemovedEventName)->GetEventListenerCount() == 0)
                    Container->RemoveEventCallback(onChildRemovedEventName);
                
                OnChildRemovedEventIndex = -1;
            }
        }

        UpdateChildrenResizeTypes();
    }

    bool Layout::GetUpdateContainerMinMaxSize() const
    {
        return UpdateContainerMinMaxSize;
    }

    void Layout::SetUpdateContainerMinMaxSize(bool update)
    {
        UpdateContainerMinMaxSize = update;
        
        if(Container == nullptr)
            return;

        const std::string onChildAddedEventName = ssGUI::EventCallbacks::RecursiveChildrenAddedEventCallback::EVENT_NAME;
        const std::string onChildRemovedEventName = ssGUI::EventCallbacks::RecursiveChildrenRemovedEventCallback::EVENT_NAME;
        const std::string onMinMaxSizeChangedEventName = ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback::EVENT_NAME;

        if(update)
        {
            //If callback doesn't exist, add it
            if(!Container->IsEventCallbackExist(onChildAddedEventName))
            {
                Container->AddEventCallback(static_cast<ssGUI::EventCallbacks::EventCallback*>(
                    new ssGUI::EventCallbacks::RecursiveChildrenAddedEventCallback()));
            }
            
            //If event index is not assigned, assign it
            if(OnChildAddedEventIndex == -1)
            {
                OnChildAddedEventIndex = Container->GetEventCallback(onChildAddedEventName)->AddEventListener(
                    std::bind(&ssGUI::Extensions::Layout::Internal_OnRecursiveChildAdded, this, std::placeholders::_1));
            }
            
            if(!Container->IsEventCallbackExist(onChildRemovedEventName))
            {
                Container->AddEventCallback(static_cast<ssGUI::EventCallbacks::EventCallback*>(
                    new ssGUI::EventCallbacks::RecursiveChildrenRemovedEventCallback()));
            }
            
            if(OnChildRemovedEventIndex == -1)
            {
                OnChildRemovedEventIndex = Container->GetEventCallback(onChildRemovedEventName)->AddEventListener(
                    std::bind(&ssGUI::Extensions::Layout::Internal_OnRecursiveChildRemoved, this, std::placeholders::_1));
            }

            //Iterate each child to see if event callback exists
            auto childIt = Container->GetChildrenStartIterator();
            while(childIt != Container->GetChildrenEndIterator())
            {
                if(ObjectsToExclude.find(*childIt) != ObjectsToExclude.end())
                    continue;
                
                if(!(*childIt)->IsEventCallbackExist(onMinMaxSizeChangedEventName))
                {
                    (*childIt)->AddEventCallback(static_cast<ssGUI::EventCallbacks::EventCallback*>(
                        new ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback()));
                }

                if(MinMaxSizeChangedEventIndices.find((*childIt)) == MinMaxSizeChangedEventIndices.end())
                {
                    MinMaxSizeChangedEventIndices[(*childIt)] = (*childIt)->GetEventCallback(onMinMaxSizeChangedEventName)->AddEventListener(
                        std::bind(&ssGUI::Extensions::Layout::Internal_OnChildMinMaxSizeChanged, this, std::placeholders::_1));
                }

                childIt++;
            }

            SyncContainerMinMaxSize();
        }
        else
        {
            //If callback exists, check if event index exists. 
            //If so remove event listener and remove the callback if there's no other listeners. Then reset the event index
            if(Container->IsEventCallbackExist(onChildAddedEventName) && OnChildAddedEventIndex != -1)
            {
                Container->GetEventCallback(onChildAddedEventName)->RemoveEventListener(OnChildAddedEventIndex);

                if(Container->GetEventCallback(onChildAddedEventName)->GetEventListenerCount() == 0)
                    Container->RemoveEventCallback(onChildAddedEventName);
                
                OnChildAddedEventIndex = -1;
            }

            if(Container->IsEventCallbackExist(onChildRemovedEventName) && OnChildRemovedEventIndex != -1)
            {
                Container->GetEventCallback(onChildRemovedEventName)->RemoveEventListener(OnChildRemovedEventIndex);

                if(Container->GetEventCallback(onChildRemovedEventName)->GetEventListenerCount() == 0)
                    Container->RemoveEventCallback(onChildRemovedEventName);
                
                OnChildRemovedEventIndex = -1;
            }
            
            //Iterate each child to see if event callback exists
            auto childIt = Container->GetChildrenStartIterator();
            while(childIt != Container->GetChildrenEndIterator())
            {
                if(ObjectsToExclude.find(*childIt) != ObjectsToExclude.end())
                    continue;
                
                if((*childIt)->IsEventCallbackExist(onMinMaxSizeChangedEventName) 
                    && MinMaxSizeChangedEventIndices.find((*childIt)) != MinMaxSizeChangedEventIndices.end())
                {
                    (*childIt)->GetEventCallback(onMinMaxSizeChangedEventName)->RemoveEventListener(MinMaxSizeChangedEventIndices[(*childIt)]);
                    
                    if((*childIt)->GetEventCallback(onMinMaxSizeChangedEventName)->GetEventListenerCount() == 0)
                        (*childIt)->RemoveEventCallback(onMinMaxSizeChangedEventName);

                    MinMaxSizeChangedEventIndices.erase((*childIt));
                }
                childIt++;
            }
        }
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
        if(ObjectsToExclude.find(obj) == ObjectsToExclude.end())
        {
            ObjectsToExclude.insert(obj);
        }
    }

    void Layout::UnexcludeObject(ssGUI::GUIObject* obj)
    {
        if(ObjectsToExclude.find(obj) != ObjectsToExclude.end())
        {
            ObjectsToExclude.erase(obj);

            if(obj->GetParent() == Container)
                Internal_OnRecursiveChildAdded(obj);
        }
    }

    void Layout::Internal_OnRecursiveChildAdded(ssGUI::GUIObject* child)
    {
        if(child->GetParent() != Container)
            return;

        if(ObjectsToExclude.find(child) != ObjectsToExclude.end())
            return;

        if(child->HasTag(ssGUI::Tags::OVERLAY) || child->HasTag(ssGUI::Tags::FLOATING))
        {
            ObjectsToExclude.insert(child);
            return;
        }
        
        if(GetOverrideChildrenResizeType())
        {
            UpdateChildrenResizeTypes();
        }
        
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

            if(MinMaxSizeChangedEventIndices.find(child) == MinMaxSizeChangedEventIndices.end())
            {
                MinMaxSizeChangedEventIndices[child] = child->GetEventCallback(onMinMaxSizeChangedEventName)->AddEventListener(
                    std::bind(&ssGUI::Extensions::Layout::Internal_OnChildMinMaxSizeChanged, this, std::placeholders::_1));
            }
        }
    }

    void Layout::Internal_OnRecursiveChildRemoved(ssGUI::GUIObject* child)
    {        
        //If this is one of the excluding objects, remove it to prevent invalid object pointer
        if(ObjectsToExclude.find(child) != ObjectsToExclude.end())
        {
            ObjectsToExclude.erase(child);
            return;
        }
        
        if(GetOverrideChildrenResizeType())
        {
            UpdateChildrenResizeTypes();
        }

        //Restore resize type if it is recorded
        if(OriginalChildrenResizeType.find(child) != OriginalChildrenResizeType.end())
        {
            static_cast<ssGUI::Window*>(child)->SetResizeType(OriginalChildrenResizeType[child]);

            OriginalChildrenResizeType.erase(child);
        }
        
        if(GetUpdateContainerMinMaxSize())
        {
            SyncContainerMinMaxSize();

            const std::string onMinMaxSizeChangedEventName = ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback::EVENT_NAME;

            //Remove MinMax size changed callbacks
            if(child->IsEventCallbackExist(onMinMaxSizeChangedEventName) 
                && MinMaxSizeChangedEventIndices.find(child) != MinMaxSizeChangedEventIndices.end())
            {                
                child->GetEventCallback(onMinMaxSizeChangedEventName)->RemoveEventListener(MinMaxSizeChangedEventIndices[child]);

                if(child->GetEventCallback(onMinMaxSizeChangedEventName)->GetEventListenerCount() == 0)
                    child->RemoveEventCallback(onMinMaxSizeChangedEventName);

                MinMaxSizeChangedEventIndices.erase(child);
            }
        }   

        if(OriginalChildrenSize.find(child) != OriginalChildrenSize.end())
        {
            std::cout<<"removed: "<<OriginalChildrenSize[child].x<<","<<OriginalChildrenSize[child].y<<"\n";
            child->SetSize(OriginalChildrenSize[child]);
            std::cout<<"removed 2: "<<child->GetSize().x<<","<<child->GetSize().y<<"\n";
            OriginalChildrenSize.erase(child);   
        }
    }

    void Layout::Internal_OnChildMinMaxSizeChanged(ssGUI::GUIObject* child)
    {        
        if(ObjectsToExclude.find(child) != ObjectsToExclude.end())
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
        if(IsPreUpdate || Container == nullptr || Container->GetChildrenCount() == 0 || !Enabled)
            return;

        //Update excluded objects if there's an overlay tag
        for(auto it = Container->GetChildrenStartIterator(); it != Container->GetChildrenEndIterator(); it++)
        {
            if(ObjectsToExclude.find(*it) != ObjectsToExclude.end())
                continue;
            
            if((*it)->HasTag(ssGUI::Tags::OVERLAY) || (*it)->HasTag(ssGUI::Tags::FLOATING))
            {
                ObjectsToExclude.insert(*it);
                continue;
            }
            
            //Record the original size if not exists (Can't be done in OnChildAdded as that's after the child being added)
            if(OriginalChildrenSize.find(*it) == OriginalChildrenSize.end())
            {
                std::cout<<"added: "<<(*it)->GetSize().x<<","<<(*it)->GetSize().y<<"\n";
                OriginalChildrenSize[*it] = (*it)->GetSize();
            }
        }

        //Check if object to exclude has valid parents
        int excludeCount = 0;
        std::vector<ssGUI::GUIObject*> objToRemove;
        for(auto it = ObjectsToExclude.begin(); it != ObjectsToExclude.end(); it++)
        {
            if((*it)->GetParent() != Container)
                objToRemove.push_back(*it);
            else
                excludeCount++;
        }

        for(auto obj : objToRemove)
            ObjectsToExclude.erase(obj);

        //Check preffered size multipliers length
        if(PreferredSizeMultipliers.size() != Container->GetChildrenCount() - excludeCount)
        {
            PreferredSizeMultipliers.resize(Container->GetChildrenCount() - excludeCount, -1);
        }

        //Disable children resizing
        if(DisableChildrenResizing)
        {
            for(auto it = Container->GetChildrenStartIterator(); it != Container->GetChildrenEndIterator(); it++)
            {
                if(ObjectsToExclude.find(*it) != ObjectsToExclude.end())
                    continue;
                
                if((*it)->GetType() == ssGUI::Enums::GUIObjectType::WINDOW)
                    dynamic_cast<ssGUI::Window*>(*it)->SetResizeType(ssGUI::Enums::ResizeType::NONE);
            }
        }

        //Update min max size
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

        for(auto it = Container->GetChildrenStartIterator(); it != Container->GetChildrenEndIterator(); it++)
        {
            if(ObjectsToExclude.find(*it) != ObjectsToExclude.end())
                continue;

            glm::ivec2 currentPos = (*it)->GetGlobalPosition();
            glm::ivec2 currentSize = (*it)->GetSize();
            glm::ivec2 currentMinSize = (*it)->GetMinSize();
            glm::ivec2 currentMaxSize = (*it)->GetMaxSize();
            
            if(IsHorizontalLayout())
            {
                int verticalPos = Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW ? 
                                    ContainerPos.y + dynamic_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() :
                                    ContainerPos.y + GetPadding();     

                int verticalSize = Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW ? 
                                    containerSize.y - GetPadding() - dynamic_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() :
                                    containerSize.y - GetPadding() * 2;

                
                if(currentPos.y != verticalPos)
                    (*it)->SetGlobalPosition(glm::ivec2(currentPos.x, verticalPos));
            
                if(currentSize.y != verticalSize)
                    (*it)->SetSize(glm::ivec2(currentSize.x, verticalSize));

                childrenPos.push_back(currentPos.x);
                childrenSize.push_back(currentSize.x);
                childrenMinSize.push_back(currentMinSize.x);
                childrenMaxSize.push_back(currentMaxSize.x);                
            }
            else
            {
                if(currentPos.x != ContainerPos.x + GetPadding())
                    (*it)->SetGlobalPosition(glm::ivec2(ContainerPos.x + GetPadding(), currentPos.y));
            
                if(currentSize.x != containerSize.x - GetPadding() * 2)
                    (*it)->SetSize(glm::ivec2(containerSize.x - GetPadding() * 2, currentSize.y));

                childrenPos.push_back(currentPos.y);
                childrenSize.push_back(currentSize.y);
                childrenMinSize.push_back(currentMinSize.y);
                childrenMaxSize.push_back(currentMaxSize.y);
            }
            
        }
        
        //Check if size is different from last update (Iterating from back to front so to get the first child with size change)
        int itIndex = childrenPos.size() - 1;
        for(auto it = Container->GetChildrenReverseStartIterator(); it != Container->GetChildrenReverseEndIterator(); it++)
        {
            if(ObjectsToExclude.find(*it) != ObjectsToExclude.end())
                continue;
            
            if(LastUpdateChildrenSize.find(*it) != LastUpdateChildrenSize.end())
            {
                if(IsHorizontalLayout() && LastUpdateChildrenSize[(*it)].x != childrenSize[itIndex])
                {
                    sizeDiff = childrenSize[itIndex] - LastUpdateChildrenSize[(*it)].x;
                    lastChildChangeIndex = itIndex;
                }
                else if(!IsHorizontalLayout() && LastUpdateChildrenSize[(*it)].y != childrenSize[itIndex])
                {
                    sizeDiff = childrenSize[itIndex] - LastUpdateChildrenSize[(*it)].y;
                    lastChildChangeIndex = itIndex;
                }                
            }
            itIndex--;
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
        for(auto it = Container->GetChildrenStartIterator(); it != Container->GetChildrenEndIterator(); it++)
        {
            if(ObjectsToExclude.find(*it) != ObjectsToExclude.end())
                continue;
            
            glm::ivec2 currentPos = (*it)->GetGlobalPosition();
            glm::ivec2 currentSize = (*it)->GetSize();
            glm::ivec2 currentMinSize = (*it)->GetMinSize();

            if(IsHorizontalLayout())
            {
                (*it)->SetSize(glm::ivec2(childrenSize[index], currentSize.y));
                (*it)->SetGlobalPosition(glm::ivec2(childrenPos[index], currentPos.y));
            }
            else
            {
                (*it)->SetSize(glm::ivec2(currentSize.x, childrenSize[index]));
                (*it)->SetGlobalPosition(glm::ivec2(currentPos.x, childrenPos[index]));
            }

            index++;
            
            //Update ChildrenSize
            LastUpdateChildrenSize[(*it)] = (*it)->GetSize();   
        }
    }

    void Layout::Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {
        //Draw nothing
    }

    std::string Layout::GetExtensionName()
    {
        return EXTENSION_NAME;
    }

    void Layout::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;

        //By default, layout extension will override children resize type and will also update container min max size
        if(GetOverrideChildrenResizeType())
            SetOverrideChildrenResizeType(true);
        
        if(GetUpdateContainerMinMaxSize())
            SetUpdateContainerMinMaxSize(true);
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
        ObjectsToExclude = layout->ObjectsToExclude;
    }

    Extension* Layout::Clone(ssGUI::GUIObject* newContainer)
    {
        Layout* temp = new Layout(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
}