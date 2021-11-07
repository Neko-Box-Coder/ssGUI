#include "ssGUI/Extensions/Layout.hpp"

namespace ssGUI::Extensions
{
    void Layout::LayoutChildren(int startPos, int length, std::vector<int>& childrenPos, std::vector<int>& childrenLength, 
                                std::vector<int>& minChildrenLength, std::vector<int>& maxChildrenLength)
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

        //Calculate length
        for(int i = 0; i < childrenPos.size(); i++)
        {
            float sizeMultiplier = GetPreferredSizeMultiplier(i);

            if(sizeMultiplier > 0)
                currentLength = length * sizeMultiplier;
            else
                currentLength = childrenLength[i];

            if(remainingLength > remainingMinLengths &&
                remainingLength - currentLength < remainingMinLengths - minChildrenLength[i])
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
        SetHorizontalLayout(other.IsHorizontalLayout());
        PreferredSizeMultipliers = other.PreferredSizeMultipliers;
        SetDisableChildrenResizing(other.IsChildrenResizingDisabled());
        SetOverrideChildrenResizeType(other.GetOverrideChildrenResizeType());
        SetUpdateContainerMinMaxSize(other.GetUpdateContainerMinMaxSize());
        SetReverseOrder(other.IsReverseOrder());
        SetCoverFullLength(other.IsCoverFullLength());
        Container = nullptr;
        SetPadding(other.GetPadding());
        SetSpacing(other.GetSpacing());
        OnChildAddedEventIndex = -1;
        OnChildRemovedEventIndex = -1;
        OriginalChildrenResizeType = std::unordered_map<ssGUI::GUIObject*, ssGUI::Enums::ResizeType>();
        MinMaxSizeChangedEventIndices = std::unordered_map<ssGUI::GUIObject*, int>();
    }

    const std::string Layout::EXTENSION_NAME = "Layout";

    Layout::Layout() : HorizontalLayout(false), PreferredSizeMultipliers(), DisableChildrenResizing(false), 
                        OverrideChildrenResizeTypes(true), UpdateContainerMinMaxSize(true), ReverseOrder(false), CoverFullLength(true),
                        Container(nullptr), Padding(5), Spacing(5), OnChildAddedEventIndex(-1), OnChildRemovedEventIndex(-1),
                        OriginalChildrenResizeType(), MinMaxSizeChangedEventIndices()
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
        if(Container == nullptr)
            return;
        
        if(HorizontalLayout)
        {
            int minSizeTotalX = 0;
            int maxSizeTotalX = 0;
            int minMaxY = std::numeric_limits<int>::max();
            int maxMinY = -1;
            
            for(auto it = Container->GetChildrenStartIterator(); it != Container->GetChildrenEndIterator(); it++)
            {
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
            int spacingTotalX = (Container->GetChildrenCount() - 1) * GetSpacing();

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

            int spacingTotalY = (Container->GetChildrenCount() - 1) * GetSpacing();

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

    void Layout::Internal_OnRecursiveChildAdded(ssGUI::GUIObject* child)
    {
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
    }

    void Layout::Internal_OnChildMinMaxSizeChanged(ssGUI::GUIObject* child)
    {        
        if(GetUpdateContainerMinMaxSize())
        {
            SyncContainerMinMaxSize();
        }
    }

    //Override from Extension
    void Layout::Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus)
    {
        if(IsPreUpdate)
            return;

        //Check preffered size multipliers length
        if(PreferredSizeMultipliers.size() != Container->GetChildrenCount())
        {
            PreferredSizeMultipliers.resize(Container->GetChildrenCount(), -1);
        }

        //Disable children resizing
        if(DisableChildrenResizing)
        {
            for(auto it = Container->GetChildrenStartIterator(); it != Container->GetChildrenEndIterator(); it++)
            {
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

        glm::ivec2 Containeros = Container->GetGlobalPosition();
        glm::ivec2 containerSize = Container->GetSize();

        for(auto it = Container->GetChildrenStartIterator(); it != Container->GetChildrenEndIterator(); it++)
        {
            glm::ivec2 currentPos = (*it)->GetGlobalPosition();
            glm::ivec2 currentSize = (*it)->GetSize();
            glm::ivec2 currentMinSize = (*it)->GetMinSize();
            glm::ivec2 currentMaxSize = (*it)->GetMaxSize();
            
            if(IsHorizontalLayout())
            {
                int verticalPos = Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW ? 
                                    Containeros.y + dynamic_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() :
                                    Containeros.y + GetPadding();     

                int verticalSize = Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW ? 
                                    containerSize.y - GetPadding() - dynamic_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() :
                                    containerSize.y - GetPadding() * 2;

                
                if(currentPos.y != verticalPos)
                    (*it)->SetGlobalPosition(glm::ivec2(Containeros.x, verticalPos));
            
                if(currentSize.y != verticalSize)
                    (*it)->SetSize(glm::ivec2(containerSize.x, verticalSize));

                childrenPos.push_back(currentPos.x);
                childrenSize.push_back(currentSize.x);
                childrenMinSize.push_back(currentMinSize.x);
                childrenMaxSize.push_back(currentMaxSize.x);
            }
            else
            {
                if(currentPos.x != Containeros.x + GetPadding())
                    (*it)->SetGlobalPosition(glm::ivec2(Containeros.x + GetPadding(), currentPos.y));
            
                if(currentSize.x != containerSize.x - GetPadding() * 2)
                    (*it)->SetSize(glm::ivec2(containerSize.x - GetPadding() * 2, currentSize.y));

                childrenPos.push_back(currentPos.y);
                childrenSize.push_back(currentSize.y);
                childrenMinSize.push_back(currentMinSize.y);
                childrenMaxSize.push_back(currentMaxSize.y);
            }
            
        }

        //Check for window for laying out children differently
        if(Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW)
        {
            if(IsHorizontalLayout())
            {
                LayoutChildren( Containeros.x + GetPadding(), 
                                containerSize.x - GetPadding() * 2, 
                                childrenPos, childrenSize, childrenMinSize, childrenMaxSize);
            }
            else
            {
                LayoutChildren( Containeros.y + ((ssGUI::Window*)Container)->GetTitlebarHeight(), 
                                containerSize.y - ((ssGUI::Window*)Container)->GetTitlebarHeight() - GetPadding(), 
                                childrenPos, childrenSize, childrenMinSize, childrenMaxSize);
            }   
        }
        else
        {
            if(IsHorizontalLayout())
                LayoutChildren(Containeros.x + GetPadding(), containerSize.x - GetPadding() * 2, 
                                childrenPos, childrenSize, childrenMinSize, childrenMaxSize);
            else
                LayoutChildren(Containeros.y + GetPadding(), containerSize.y - GetPadding() * 2, 
                                childrenPos, childrenSize, childrenMinSize, childrenMaxSize);
        }


        //Assigning position and size to children
        int index = 0;
        for(auto it = Container->GetChildrenStartIterator(); it != Container->GetChildrenEndIterator(); it++)
        {
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

    Extension* Layout::Clone(ssGUI::GUIObject* newContainer)
    {
        Layout* temp = new Layout(*this);
        temp->BindToObject(newContainer);
        return temp;
    }
}