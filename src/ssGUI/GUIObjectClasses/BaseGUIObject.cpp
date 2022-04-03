#include "ssGUI/GUIObjectClasses/BaseGUIObject.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //This is for getting the MainWindow offset
#include "ssGUI/ssGUIManager.hpp" //This is for accessing DeletedObjs

namespace ssGUI
{    
    BaseGUIObject::BaseGUIObject(BaseGUIObject const& other)
    {
        Parent = -1;
        Children = std::list<ssGUIObjectIndex>();
        CurrentChild = Children.end();
        CurrentChildIteratorFrontEnd = true;
        CurrentChildIteratorBackEnd = true;
        Visible = other.IsVisible();
        BackgroundColour = other.GetBackgroundColor();
        UserCreated = other.IsUserCreated();
        ObjectDelete = other.Internal_IsDeleted();
        HeapAllocated = true;//other.IsHeapAllocated();
        CurrentObjectsReferences = other.CurrentObjectsReferences;
        DestroyEventCalled = other.DestroyEventCalled;
        Redraw = other.Redraw;
        AcceptRedrawRequest = other.AcceptRedrawRequest;
        StashedChildIterators = other.StashedChildIterators;
        Position = other.GetPosition();
        GlobalPosition = other.GlobalPosition;
        Size = other.GetSize();
        MinSize = other.GetMinSize();
        MaxSize = other.GetMaxSize();
        Anchor = other.GetAnchorType();
        DrawingVerticies = other.DrawingVerticies;
        DrawingUVs = other.DrawingUVs;
        DrawingColours = other.DrawingColours;
        DrawingCounts = other.DrawingCounts;
        DrawingProperties = other.DrawingProperties;
        LastDrawingVerticies = other.LastDrawingVerticies;
        LastDrawingUVs = other.LastDrawingUVs;
        LastDrawingColours = other.LastDrawingColours;
        LastDrawingCounts = other.LastDrawingCounts;
        LastDrawingProperties = other.LastDrawingProperties;
        LastGlobalPosition = other.LastGlobalPosition;
        Extensions = std::unordered_map<std::string, ssGUI::Extensions::Extension*>();
        ExtensionsDrawOrder = std::vector<std::string>();
        ExtensionsUpdateOrder = std::vector<std::string>();
        EventCallbacks = std::unordered_map<std::string, ssGUI::EventCallbacks::EventCallback*>();
        CurrentTags = other.CurrentTags;// std::unordered_set<std::string>();

        SetParent(other.GetParent()); //Note : Reason of using SetParent is to inform the parent to add this as a child
    }
    
    void BaseGUIObject::SyncPosition()
    {
        FUNC_DEBUG_ENTRY();
        
        if(Parent == -1)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        if(CurrentObjectsReferences.GetObjectReference(Parent) == nullptr)
        {
            Parent = -1;
            FUNC_DEBUG_EXIT();
            return;
        }
        
        glm::vec2 parentGlobalPositon = CurrentObjectsReferences.GetObjectReference(Parent)->GetParent() == nullptr ? 
                                        glm::vec2() : CurrentObjectsReferences.GetObjectReference(Parent)->GetGlobalPosition();
        glm::vec2 parentSize = CurrentObjectsReferences.GetObjectReference(Parent)->GetSize();
        glm::vec2 anchorPosition = parentGlobalPositon;
        glm::vec2 anchorDirection = glm::vec2(1, 1);
        glm::vec2 positionOffset = GetSize();
        
        //Find anchor position
        switch(Anchor)
        {
            case ssGUI::Enums::AnchorType::TOP_LEFT:
                anchorPosition = parentGlobalPositon;
                positionOffset = glm::vec2(0, 0);
                break;
            case ssGUI::Enums::AnchorType::TOP_RIGHT:
                anchorPosition = parentGlobalPositon + glm::vec2(parentSize.x, 0);
                anchorDirection.x = -1;
                positionOffset = glm::vec2(GetSize().x, 0);
                break;
            case ssGUI::Enums::AnchorType::BOTTOM_LEFT:
                anchorPosition = parentGlobalPositon + glm::vec2(0, parentSize.y);
                anchorDirection.y = -1;
                positionOffset = glm::vec2(0, GetSize().y);
                break;
            case ssGUI::Enums::AnchorType::BOTTOM_RIGHT:
                anchorPosition = parentGlobalPositon + glm::vec2(parentSize.x, parentSize.y);
                anchorDirection *= -1;
                positionOffset = glm::vec2(GetSize().x, GetSize().y);
                break;
        }

        //TODO : Maybe move this part to window as override
        if(CurrentObjectsReferences.GetObjectReference(Parent)->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW && 
            CurrentObjectsReferences.GetObjectReference(Parent)->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && 
            (Anchor == ssGUI::Enums::AnchorType::TOP_LEFT || Anchor == ssGUI::Enums::AnchorType::TOP_RIGHT))
        {
            anchorPosition.y += dynamic_cast<ssGUI::Window*>(CurrentObjectsReferences.GetObjectReference(Parent))->GetTitlebarHeight();
        }

        //Local Position = (Global position - Anchor Point) * anchorDirection
        Position.x = (GlobalPosition.x + positionOffset.x - anchorPosition.x) * anchorDirection.x;
        Position.y = (GlobalPosition.y + positionOffset.y - anchorPosition.y) * anchorDirection.y;

        FUNC_DEBUG_EXIT();
    }
    
    void BaseGUIObject::SyncGlobalPosition()
    {
        FUNC_DEBUG_ENTRY();
        
        if(Parent == -1)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        if(CurrentObjectsReferences.GetObjectReference(Parent) == nullptr)
        {
            Parent = -1;
            FUNC_DEBUG_EXIT();
            return;
        }
        
        glm::vec2 parentGlobalPositon = CurrentObjectsReferences.GetObjectReference(Parent)->GetParent() == nullptr ? glm::vec2() : CurrentObjectsReferences.GetObjectReference(Parent)->GetGlobalPosition();
        glm::vec2 parentSize = CurrentObjectsReferences.GetObjectReference(Parent)->GetSize();
        glm::vec2 anchorPosition = parentGlobalPositon;
        glm::vec2 anchorDirection = glm::vec2(1, 1);
        glm::vec2 positionOffset = GetSize();

        // std::cout<<"parent pos:"<<parentGlobalPositon.x<<", "<<parentGlobalPositon.y<<"\n";
        // std::cout<<"parentSize: "<<parentSize.x<<", "<<parentSize.y<<"\n";
        
        //Find anchor position
        switch(Anchor)
        {
            case ssGUI::Enums::AnchorType::TOP_LEFT:
                anchorPosition = parentGlobalPositon;
                positionOffset = glm::vec2(0, 0);
                break;
            case ssGUI::Enums::AnchorType::TOP_RIGHT:
                anchorPosition = parentGlobalPositon + glm::vec2(parentSize.x, 0);
                anchorDirection.x = -1;
                positionOffset = glm::vec2(GetSize().x, 0);
                break;
            case ssGUI::Enums::AnchorType::BOTTOM_LEFT:
                anchorPosition = parentGlobalPositon + glm::vec2(0, parentSize.y);
                anchorDirection.y = -1;
                positionOffset = glm::vec2(0, GetSize().y);
                break;
            case ssGUI::Enums::AnchorType::BOTTOM_RIGHT:
                anchorPosition = parentGlobalPositon + glm::vec2(parentSize.x, parentSize.y);
                anchorDirection *= -1;
                positionOffset = glm::vec2(GetSize().x, GetSize().y);
                break;
        }

        //TODO : Maybe move this part to window as override
        if(CurrentObjectsReferences.GetObjectReference(Parent)->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW && 
            CurrentObjectsReferences.GetObjectReference(Parent)->GetType() == ssGUI::Enums::GUIObjectType::WINDOW &&
            (Anchor == ssGUI::Enums::AnchorType::TOP_LEFT || Anchor == ssGUI::Enums::AnchorType::TOP_RIGHT))
        {
            anchorPosition.y += dynamic_cast<ssGUI::Window*>(CurrentObjectsReferences.GetObjectReference(Parent))->GetTitlebarHeight();
        }

        // std::cout<<"anchorPosition: "<<anchorPosition.x<<", "<<anchorPosition.y<<"\n";
        // std::cout<<"anchorDirection: "<<anchorDirection.x<<", "<<anchorDirection.y<<"\n";
        // std::cout<<"positionOffset: "<<positionOffset.x<<", "<<positionOffset.y<<"\n";

        //Global Position = AnchorPoint + local position * anchorDirection
        GlobalPosition.x = anchorPosition.x + Position.x * anchorDirection.x - positionOffset.x;
        GlobalPosition.y = anchorPosition.y + Position.y * anchorDirection.y - positionOffset.y;

        FUNC_DEBUG_EXIT();
    }

    void BaseGUIObject::ConstructRenderInfo()
    {
        //This should be overriden.
    }

    void BaseGUIObject::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        //This can be overriden if needed.
    }

    void BaseGUIObject::NotifyAndRemoveOnObjectDestroyEventCallbackIfExist()
    {
        if(DestroyEventCalled)
            return;
        
        DestroyEventCalled = true;
        if(IsEventCallbackExist(ssGUI::EventCallbacks::OnObjectDestroyEventCallback::EVENT_NAME))
        {
            GetEventCallback(ssGUI::EventCallbacks::OnObjectDestroyEventCallback::EVENT_NAME)->Notify(this);
            RemoveEventCallback(ssGUI::EventCallbacks::OnObjectDestroyEventCallback::EVENT_NAME);
        }
    }

    ssGUI::GUIObject* BaseGUIObject::CloneChildren(ssGUI::GUIObject* originalRoot, ssGUI::GUIObject* clonedRoot)
    {
        FUNC_DEBUG_ENTRY();
        
        //1. First get a list of objects needed to be cloned and create a hashmap of original objects with the index of it
        std::vector<ssGUI::GUIObject*> originalObjsToClone = std::vector<ssGUI::GUIObject*>();
        std::unordered_map<ssGUI::GUIObject*, int> originalObjsIndex = std::unordered_map<ssGUI::GUIObject*, int>();
        std::vector<ssGUI::GUIObject*> clonedObjs = std::vector<ssGUI::GUIObject*>();

        originalObjsToClone.push_back(originalRoot);
        originalObjsIndex[originalRoot] = 0;
        clonedObjs.push_back(clonedRoot);

        int currentIndex = 0;
        do
        {
            ssGUI::GUIObject* curObj = originalObjsToClone[currentIndex];
            curObj->StashChildrenIterator();
            curObj->MoveChildrenIteratorToFirst();
            while (!curObj->IsChildrenIteratorEnd())
            {
                originalObjsToClone.push_back(curObj->GetCurrentChild());
                originalObjsIndex[curObj->GetCurrentChild()] = originalObjsToClone.size() - 1;
                curObj->MoveChildrenIteratorNext();
            }
            curObj->PopChildrenIterator();
            currentIndex++;
        }
        while (currentIndex < originalObjsToClone.size());

        //2. Clone the objects (starting from 1 because root is cloned)
        for(int i = 1; i < originalObjsToClone.size(); i++)
        {
            auto curClonedObj = originalObjsToClone[i]->Clone(false);
            curClonedObj->SetParent(nullptr);
            clonedObjs.push_back(curClonedObj);
        }

        //3. Set ObjectsReferences for both the object itself, extension and event callbacks using the hashmap
        for(int i = 0; i < clonedObjs.size(); i++)
        {
            auto curClonedObj = clonedObjs[i];
            auto curObjOR = clonedObjs[i]->Internal_GetObjectsReferences();
            auto curObjORindices = curObjOR->GetListOfObjectsIndices();

            //Fix the object's objects references
            for(auto orIndex : curObjORindices)
            {
                auto refObj = curObjOR->GetObjectReference(orIndex);

                //if the referencing object is found to be cloned
                if(originalObjsIndex.find(refObj) != originalObjsIndex.end())
                    curObjOR->SetObjectReference(orIndex, clonedObjs[originalObjsIndex[refObj]]);
            }

            //Fix the object's extensions' objects references
            auto extensionsList = curClonedObj->GetListOfExtensions();
            for(auto extension : extensionsList)
            {
                auto curExtensionOR = extension->Internal_GetObjectsReferences();
                if(curExtensionOR == nullptr)
                    continue;
                
                auto curExtensionORindices = curExtensionOR->GetListOfObjectsIndices();
                for(auto orIndex : curExtensionORindices)
                {
                    auto refObj = curExtensionOR->GetObjectReference(orIndex);

                    //if the referencing object is found to be cloned
                    if(originalObjsIndex.find(refObj) != originalObjsIndex.end())
                        curExtensionOR->SetObjectReference(orIndex, clonedObjs[originalObjsIndex[refObj]]);
                }
            }

            //Fix the object's event callbacks' objects references
            auto eventCallbackList = curClonedObj->GetListOfEventCallbacks();
            for(auto eventCallback : eventCallbackList)
            {
                auto curEventCallbackOR = eventCallback->Internal_GetObjectsReferences();
                if(curEventCallbackOR == nullptr)
                    continue;
                
                auto curEventCallbackORindices = curEventCallbackOR->GetListOfObjectsIndices();
                for(auto orIndex : curEventCallbackORindices)
                {
                    auto refObj = curEventCallbackOR->GetObjectReference(orIndex);

                    //if the referencing object is found to be cloned
                    if(originalObjsIndex.find(refObj) != originalObjsIndex.end())
                        curEventCallbackOR->SetObjectReference(orIndex, clonedObjs[originalObjsIndex[refObj]]);
                }
            }
        }

        //4. Set the correct parent to the cloned objects (Don't need to set root's parent as it is already set when it was cloned)
        for(int i = 1; i < clonedObjs.size(); i++)
        {
            auto oriParent = originalObjsToClone[i]->GetParent();
            if(originalObjsIndex.find(oriParent) == originalObjsIndex.end())
            {
                DEBUG_LINE("Unable to clone, original parent can't be found: "<<oriParent);
                DEBUG_EXIT_PROGRAM();
                return nullptr;
            }
            
            clonedObjs[i]->SetParent(clonedObjs[originalObjsIndex.at(oriParent)]);
        }

        FUNC_DEBUG_EXIT();
        return clonedRoot;
    }

    void BaseGUIObject::CloneExtensionsAndEventCallbacks(ssGUI::GUIObject* clonedObj)
    {
        FUNC_DEBUG_ENTRY();
        for(auto extension : Extensions)
        {
            if(!clonedObj->IsExtensionExist(extension.second->GetExtensionName()))
                extension.second->Clone(clonedObj);
            else
                clonedObj->GetExtension(extension.first)->Copy(extension.second);
        }

        //Copy draw and update order
        for(int i = 0; i < ExtensionsDrawOrder.size(); i++)
            clonedObj->ChangeExtensionDrawOrder(ExtensionsDrawOrder[i], i);
        
        for(int i = 0; i < ExtensionsUpdateOrder.size(); i++)
            clonedObj->ChangeExtensionUpdateOrder(ExtensionsUpdateOrder[i], i);

        for(auto eventCallback : EventCallbacks)
        {
            std::vector<ssGUI::GUIObject*> tempVec = std::vector<ssGUI::GUIObject*>();
            
            if(!clonedObj->IsEventCallbackExist(eventCallback.second->GetEventCallbackName()))
                eventCallback.second->Clone(clonedObj, true);
        }

        FUNC_DEBUG_EXIT();
    }

    void BaseGUIObject::CacheRendering()
    {
        LastDrawingVerticies.assign(DrawingVerticies.begin(), DrawingVerticies.end());
        LastDrawingUVs.assign(DrawingUVs.begin(), DrawingUVs.end());
        LastDrawingColours.assign(DrawingColours.begin(), DrawingColours.end());
        LastDrawingCounts.assign(DrawingCounts.begin(), DrawingCounts.end());
        LastDrawingProperties.assign(DrawingProperties.begin(), DrawingProperties.end());
    }

    void BaseGUIObject::DisableRedrawObjectRequest()
    {
        AcceptRedrawRequest = false;
    }

    void BaseGUIObject::EnableRedrawObjectRequest()
    {
        AcceptRedrawRequest = true;
    }

    BaseGUIObject::BaseGUIObject() : Parent(-1), Children(), CurrentChild(Children.end()), CurrentChildIteratorFrontEnd(true), Visible(true),
                                        CurrentChildIteratorBackEnd(true), BackgroundColour(glm::u8vec4(255, 255, 255, 255)), UserCreated(true), 
                                        ObjectDelete(false), HeapAllocated(false), CurrentObjectsReferences(), DestroyEventCalled(false), Redraw(true), 
                                        AcceptRedrawRequest(true), Position(glm::vec2(0, 0)), GlobalPosition(glm::vec2(0, 0)), Size(glm::vec2(50, 50)), 
                                        MinSize(glm::vec2(25, 25)), MaxSize(glm::vec2(std::numeric_limits<float>::max(), std::numeric_limits<float>::max())),
                                        Anchor(ssGUI::Enums::AnchorType::TOP_LEFT), DrawingVerticies(), DrawingUVs(), DrawingColours(), 
                                        DrawingCounts(), DrawingProperties(), LastDrawingVerticies(), LastDrawingUVs(), LastDrawingColours(), 
                                        LastDrawingCounts(), LastDrawingProperties(), LastGlobalPosition(), Extensions(), ExtensionsDrawOrder(), 
                                        ExtensionsUpdateOrder(), EventCallbacks(), CurrentTags()
    {}

    BaseGUIObject::~BaseGUIObject()
    {
        if(!ObjectDelete)
        {
            NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
            
            CurrentObjectsReferences.CleanUp();
        }

        //Only clean up extensions and event callbacks when the GUI Object is destroyed
        for(auto it : Extensions)
            delete it.second;
        
        for(auto it : EventCallbacks)
            delete it.second;
    }

    //Below are from GUIObject.hpp
    glm::vec2 BaseGUIObject::GetPosition() const
    {
        return Position;
    }

    void BaseGUIObject::SetPosition(glm::vec2 position)
    {
        if(Position != position)
            RedrawObject();
        
        Position = position;
    }

    glm::vec2 BaseGUIObject::GetGlobalPosition()
    {
        //Update global position
        SyncGlobalPosition();
        
        return GlobalPosition;
    }

    void BaseGUIObject::SetGlobalPosition(glm::vec2 position)
    {
        if(GlobalPosition != position)
            RedrawObject();

        GlobalPosition = position;

        //Update local position
        SyncPosition();
    }

    glm::vec2 BaseGUIObject::GetSize() const
    {
        return Size;
    }

    void BaseGUIObject::SetSize(glm::vec2 size)
    {
        glm::vec2 oriSize = Size;
        
        size.x = size.x > MaxSize.x ? MaxSize.x : size.x;
        size.y = size.y > MaxSize.y ? MaxSize.y : size.y;
        size.x = size.x < MinSize.x ? MinSize.x : size.x;
        size.y = size.y < MinSize.y ? MinSize.y : size.y;

        Size = size;

        if(Size != oriSize)
        {
            if(IsEventCallbackExist(ssGUI::EventCallbacks::SizeChangedEventCallback::EVENT_NAME))
                GetEventCallback(ssGUI::EventCallbacks::SizeChangedEventCallback::EVENT_NAME)->Notify(this);
            RedrawObject();
        }
    }

    glm::vec2 BaseGUIObject::GetMinSize() const
    {
        return MinSize;
    }

    void BaseGUIObject::SetMinSize(glm::vec2 minSize)
    {
        if(MinSize != minSize)
            RedrawObject();
        
        MinSize = minSize;
        MaxSize.x = MinSize.x > MaxSize.x ? MinSize.x : MaxSize.x;
        MaxSize.y = MinSize.y > MaxSize.y ? MinSize.y : MaxSize.y;
        glm::vec2 size = GetSize(); //MainWindow is not updating the Size component therefore need to use GetSize()

        if(size.x < MinSize.x && Size.y < MinSize.y)
            SetSize(MinSize);
        else if(size.x < MinSize.x)
            SetSize(glm::vec2(MinSize.x, size.y));
        else if(size.y < MinSize.y)        
            SetSize(glm::vec2(size.x, MinSize.y));
            
        if(IsEventCallbackExist(ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback::EVENT_NAME))
            GetEventCallback(ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback::EVENT_NAME)->Notify(this);
    }

    glm::vec2 BaseGUIObject::GetMaxSize() const
    {
        return MaxSize;
    }

    void BaseGUIObject::SetMaxSize(glm::vec2 maxSize)
    {
        if(MaxSize != maxSize)
            RedrawObject();
        
        MaxSize = maxSize;
        MinSize.x = MaxSize.x < MinSize.x ? MaxSize.x : MinSize.x;
        MinSize.y = MaxSize.y < MinSize.y ? MaxSize.y : MinSize.y;
        glm::vec2 size = GetSize();

        if(size.x > MaxSize.x && size.y > MaxSize.y)
            SetSize(MaxSize);
        else if(size.x > MaxSize.x)
            SetSize(glm::vec2(MaxSize.x, size.y));
        else if(size.y > MaxSize.y)
            SetSize(glm::vec2(size.x, MaxSize.y));

        if(IsEventCallbackExist(ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback::EVENT_NAME))
            GetEventCallback(ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback::EVENT_NAME)->Notify(this);
    }

    ssGUI::GUIObject* BaseGUIObject::GetParent() const 
    {
        return Parent == -1 ? nullptr : CurrentObjectsReferences.GetObjectReference(Parent);
    }

    void BaseGUIObject::SetParent(ssGUI::GUIObject* newParent)
    {        
        FUNC_DEBUG_ENTRY();
        
        #if USE_DEBUG
        DEBUG_LINE("Setting "<<this<<" parent from "<< CurrentObjectsReferences.GetObjectReference(Parent)<<" to "<<newParent);
        #endif

        RedrawObject();

        ssGUI::GUIObject* originalParent = nullptr;
        //If setting parent to the same, just need to move this to the end of child
        if(Parent != -1 && newParent == CurrentObjectsReferences.GetObjectReference(Parent))
        {
            StashChildrenIterator();
            auto result = newParent->FindChild(this);
            if(!result)
            {
                PopChildrenIterator();
                DEBUG_LINE("Invalid parent detected");
                DEBUG_EXIT_PROGRAM();
                return;
            }

            auto it = newParent->GetCurrentChildReferenceIterator();
            newParent->MoveChildrenIteratorToLast();
            auto lastIt = newParent->GetCurrentChildReferenceIterator();
            newParent->ChangeChildOrderToAfterPosition(it, lastIt);
            PopChildrenIterator();
            FUNC_DEBUG_EXIT();
            return;
        }

        //If invalid parent, flag it
        if(Parent != -1 && CurrentObjectsReferences.GetObjectReference(Parent) == nullptr)
            Parent = -1;
        //Remove child from the original parent
        else if(Parent != -1)
        {
            //Check if this object is the parent of the new parent
            if(newParent != nullptr)
            {
                ssGUI::GUIObject* checkParent = newParent;
                while (checkParent != nullptr)
                {
                    if(checkParent == static_cast<ssGUI::GUIObject*>(this))
                    {
                        DEBUG_LINE("Invalid parent detected");
                        DEBUG_EXIT_PROGRAM();
                        return;
                    }

                    checkParent = checkParent->GetParent();
                }
            }

            //Record original parent
            originalParent = CurrentObjectsReferences.GetObjectReference(Parent);
            
            //Send event callback if any object is subscribed to on recursive children remove
            while (originalParent != nullptr)
            {
                if(originalParent == newParent)
                    break;

                if(originalParent->IsEventCallbackExist(ssGUI::EventCallbacks::OnRecursiveChildRemoveEventCallback::EVENT_NAME))
                    originalParent->GetEventCallback(ssGUI::EventCallbacks::OnRecursiveChildRemoveEventCallback::EVENT_NAME)->Notify(this);
                
                originalParent = originalParent->GetParent();
            }

            //Record original parent again
            originalParent = CurrentObjectsReferences.GetObjectReference(Parent);

            //Inform original parent to remove child
            CurrentObjectsReferences.GetObjectReference(Parent)->Internal_RemoveChild(this);
        }

        //If the new parent is nullptr, then remove the reference of the original parent and flag it
        if (newParent == nullptr)
        {
            // if(Parent != -1)
            //     CurrentObjectsReferences.RemoveObjectReference(Parent);
            
            Parent = -1;
        }
        //Otherwise, add it to objects reference
        else
        {
            if(Parent != -1)
                CurrentObjectsReferences.RemoveObjectReference(Parent);
            
            Parent = CurrentObjectsReferences.AddObjectReference(newParent);
        }

        //Record the parent tree before sending the removed callback as changes can be made to the parenting in event callback notification logic.
        //This will ensure should the correct objects are notified even their parenting hierarchy is changed during callback,
        std::vector<ssGUI::GUIObject*> objsToNotify;
        ssGUI::GUIObject* currentParent = originalParent;

        //Add any objects that are subscribed to recursive child removed
        while (currentParent != nullptr)
        {
            if(currentParent == newParent)
                break;

            objsToNotify.push_back(currentParent);            
            currentParent = currentParent->GetParent();
        }

        //Send event callback if any object is subscribed to child removed
        if(originalParent != nullptr && originalParent != newParent)
        {
            if(originalParent->IsEventCallbackExist(ssGUI::EventCallbacks::ChildRemovedEventCallback::EVENT_NAME))
                originalParent->GetEventCallback(ssGUI::EventCallbacks::ChildRemovedEventCallback::EVENT_NAME)->Notify(this);
        }

        //Send event callback to objects that are subscribed to recursive child removed
        for(auto obj : objsToNotify)
        {
            if(obj->IsEventCallbackExist(ssGUI::EventCallbacks::RecursiveChildRemovedEventCallback::EVENT_NAME))
                obj->GetEventCallback(ssGUI::EventCallbacks::RecursiveChildRemovedEventCallback::EVENT_NAME)->Notify(this);
        }

        //Exit if this object is parented to nothing
        if(newParent == nullptr)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        //Send event callback if any object is subscribed to on recursive child add event
        currentParent = newParent;
        while (currentParent != nullptr)
        {
            if(currentParent == static_cast<ssGUI::GUIObject*>(this))
            {
                DEBUG_LINE("Invalid parent detected");
                DEBUG_EXIT_PROGRAM();
                return;
            }
            
            if(currentParent->IsEventCallbackExist(ssGUI::EventCallbacks::OnRecursiveChildAddEventCallback::EVENT_NAME))
                currentParent->GetEventCallback(ssGUI::EventCallbacks::OnRecursiveChildAddEventCallback::EVENT_NAME)->Notify(this);    
            
            currentParent = currentParent->GetParent();
        }

        //Inform new parent to add this as child
        newParent->Internal_AddChild(this);
        
        //Update local position
        SetGlobalPosition(GetGlobalPosition());
        
        //Send event callback if any object is subscribed to child added
        if(newParent->IsEventCallbackExist(ssGUI::EventCallbacks::ChildAddedEventCallback::EVENT_NAME))
            newParent->GetEventCallback(ssGUI::EventCallbacks::ChildAddedEventCallback::EVENT_NAME)->Notify(this);
        
        //Send event callback if any object is subscribed to recursive children added
        currentParent = CurrentObjectsReferences.GetObjectReference(Parent);
        while (currentParent != nullptr)
        {
            if(currentParent == static_cast<ssGUI::GUIObject*>(this))
            {
                DEBUG_LINE("Invalid parent detected");
                DEBUG_EXIT_PROGRAM();
                return;
            }
            
            if(currentParent->IsEventCallbackExist(ssGUI::EventCallbacks::RecursiveChildAddedEventCallback::EVENT_NAME))
                currentParent->GetEventCallback(ssGUI::EventCallbacks::RecursiveChildAddedEventCallback::EVENT_NAME)->Notify(this);    
            
            currentParent = currentParent->GetParent();
        }

        FUNC_DEBUG_EXIT();
    }

    int BaseGUIObject::GetChildrenCount() const
    {
        return Children.size();
    }

    void BaseGUIObject::MoveChildrenIteratorToFirst()
    {
        if(GetChildrenCount() == 0)
        {
            CurrentChildIteratorFrontEnd = true;
            CurrentChildIteratorBackEnd = true;
            return;
        }

        CurrentChildIteratorFrontEnd = false;
        CurrentChildIteratorBackEnd = false;
        CurrentChild = Children.begin();
    }

    void BaseGUIObject::MoveChildrenIteratorToLast()
    {
        if(GetChildrenCount() == 0)
        {
            CurrentChildIteratorFrontEnd = true;
            CurrentChildIteratorBackEnd = true;
            return;
        }

        CurrentChild = Children.end();
        CurrentChild--;
        CurrentChildIteratorFrontEnd = false;
        CurrentChildIteratorBackEnd = false;        
    }

    void BaseGUIObject::MoveChildrenIteratorNext()
    {
        if(GetChildrenCount() == 0)
        {
            CurrentChildIteratorFrontEnd = true;
            CurrentChildIteratorBackEnd = true;
            return;
        }
        
        //If at front end, move to first element
        if(CurrentChildIteratorFrontEnd && !CurrentChildIteratorBackEnd)
        {
            CurrentChildIteratorFrontEnd = false;
            CurrentChild = Children.begin();
        }
        //If at last element, set back end to true
        else if(!CurrentChildIteratorFrontEnd && !CurrentChildIteratorBackEnd && CurrentChild == --Children.end())
        {
            CurrentChildIteratorFrontEnd = false;
            CurrentChildIteratorBackEnd = true;
        }
        //If not at either end, just move to next child
        else if(!CurrentChildIteratorFrontEnd && !CurrentChildIteratorBackEnd)
        {
            CurrentChild++;
        }
    }

    void BaseGUIObject::MoveChildrenIteratorPrevious()
    {
        if(GetChildrenCount() == 0)
        {
            CurrentChildIteratorFrontEnd = true;
            CurrentChildIteratorBackEnd = true;
            return;
        }

        //If at back end, move to last element
        if(!CurrentChildIteratorFrontEnd && CurrentChildIteratorBackEnd)
        {
            CurrentChildIteratorBackEnd = false;
            CurrentChild = --Children.end();
        }
        //If at first element, set front end to true
        else if(!CurrentChildIteratorFrontEnd && !CurrentChildIteratorBackEnd && CurrentChild == Children.begin())
        {
            CurrentChildIteratorFrontEnd = true;
            CurrentChildIteratorBackEnd = false;
        }
        //If not at either end, just move to previous child
        else if(!CurrentChildIteratorFrontEnd && !CurrentChildIteratorBackEnd)
        {
            CurrentChild--;
        }
    }

    bool BaseGUIObject::IsChildrenIteratorLast()
    {
        if(GetChildrenCount() == 0)
            return false;
        
        if(CurrentChild == --Children.end())
            return true;
        
        return false;
    }

    bool BaseGUIObject::IsChildrenIteratorFirst()
    {
        if(GetChildrenCount() == 0)
            return false;

        if(CurrentChild == Children.begin())
            return true;

        return false;
    }

    bool BaseGUIObject::IsChildrenIteratorEnd()
    {
        return CurrentChildIteratorFrontEnd || CurrentChildIteratorBackEnd;
    }

    void BaseGUIObject::StashChildrenIterator()
    {
        StashedChildIterators.push_back(
            std::tuple<bool, bool, std::list<ssGUIObjectIndex>::iterator>
            (CurrentChildIteratorFrontEnd, CurrentChildIteratorBackEnd, CurrentChild));
    }

    void BaseGUIObject::PopChildrenIterator()
    {
        if(StashedChildIterators.empty())
            return;
        
        auto poppedTuple = StashedChildIterators.back();
        CurrentChildIteratorFrontEnd = std::get<0>(poppedTuple);
        CurrentChildIteratorBackEnd = std::get<1>(poppedTuple);
        CurrentChild = std::get<2>(poppedTuple);
        StashedChildIterators.pop_back();
    }

    bool BaseGUIObject::FindChild(ssGUI::GUIObject* child)
    {
        if(child == nullptr)
            return false;
        
        std::list<ssGUIObjectIndex>::iterator it = Children.begin();
        std::list<ssGUIObjectIndex>::iterator endIt = Children.end();
        bool found = false;

        while (true)
        {
            if(it == endIt)
                break;
            
            if(CurrentObjectsReferences.GetObjectReference(*it) == child)
            {
                found = true;
                break;
            }

            it++;
        }

        CurrentChild = it;
        CurrentChildIteratorFrontEnd = false;
        CurrentChildIteratorBackEnd = false;
        return found;
    }

    ssGUI::GUIObject* BaseGUIObject::GetCurrentChild()
    {
        if(!IsChildrenIteratorEnd())
        {
            if(CurrentObjectsReferences.GetObjectReference(*CurrentChild) == nullptr)
            {
                DEBUG_LINE("invalid child found");
                DEBUG_EXIT_PROGRAM();
                return nullptr;
            }
            
            return CurrentObjectsReferences.GetObjectReference(*CurrentChild);
        }
        else
            return nullptr;
    }

    std::list<ssGUIObjectIndex>::iterator BaseGUIObject::GetCurrentChildReferenceIterator()
    {
        return CurrentChild;
    }

    void BaseGUIObject::ChangeChildOrderToBeforePosition(std::list<ssGUIObjectIndex>::iterator child, std::list<ssGUIObjectIndex>::iterator position)
    {
        Children.splice(position, Children, child);

        if(IsEventCallbackExist(ssGUI::EventCallbacks::ChildPositionChangedEventCallback::EVENT_NAME))
            GetEventCallback(ssGUI::EventCallbacks::ChildPositionChangedEventCallback::EVENT_NAME)->Notify(CurrentObjectsReferences.GetObjectReference(*child));   
    }
    
    void BaseGUIObject::ChangeChildOrderToAfterPosition(std::list<ssGUIObjectIndex>::iterator child, std::list<ssGUIObjectIndex>::iterator position)
    {
        Children.splice(++position, Children, child);

        if(IsEventCallbackExist(ssGUI::EventCallbacks::ChildPositionChangedEventCallback::EVENT_NAME))
            GetEventCallback(ssGUI::EventCallbacks::ChildPositionChangedEventCallback::EVENT_NAME)->Notify(CurrentObjectsReferences.GetObjectReference(*child));   
    }   
    
    void BaseGUIObject::Internal_AddChild(ssGUI::GUIObject* obj)
    {
        StashChildrenIterator();
        
        if(FindChild(obj))
        {
            PopChildrenIterator();
            return;
        }
        PopChildrenIterator();

        ssGUIObjectIndex childIndex = CurrentObjectsReferences.AddObjectReference(obj);
        
        Children.push_back(childIndex);
    }
    
    void BaseGUIObject::Internal_RemoveChild(ssGUI::GUIObject* obj)
    {
        FUNC_DEBUG_ENTRY();
        
        #if USE_DEBUG
        DEBUG_LINE(this<<" removing child "<<obj);
        #endif
        
        StashChildrenIterator();
        if(!FindChild(obj))
        {
            PopChildrenIterator();
            DEBUG_LINE("Remove failed");
            DEBUG_EXIT_PROGRAM();
            return;
        }
        
        //Go through all stashed child iterators and invalid iterators associated with the child to be removed.
        for(int i = 0; i < StashedChildIterators.size(); i++)
        {
            auto curStashedTuple = StashedChildIterators[i];
            if(CurrentObjectsReferences.GetObjectReference(*(std::get<2>(curStashedTuple))) == obj)
            {
                std::get<0>(curStashedTuple) = false;
                std::get<1>(curStashedTuple) = false;
                StashedChildIterators[i] = curStashedTuple;
            }
        }

        std::list<ssGUIObjectIndex>::iterator it = GetCurrentChildReferenceIterator();
        Children.remove(*it);
        PopChildrenIterator();

        #if USE_DEBUG
        DEBUG_LINE("Remove success");
        #endif
        FUNC_DEBUG_EXIT();
    }

    ssGUI::Enums::GUIObjectType BaseGUIObject::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::BASE_OBJECT;
    }

    ssGUI::Enums::AnchorType BaseGUIObject::GetAnchorType() const
    {
        return Anchor;
    }
    
    void BaseGUIObject::SetAnchorType(ssGUI::Enums::AnchorType anchorType)
    {
        Anchor = anchorType;
    }

    void BaseGUIObject::SetVisible(bool visible)
    {
        Visible = visible;
        RedrawObject();
    }

    bool BaseGUIObject::IsVisible() const
    {
        return Visible;
    }

    void BaseGUIObject::SetUserCreated(bool created)
    {
        UserCreated = created;
    }

    bool BaseGUIObject::IsUserCreated() const
    {
        return UserCreated;
    }

    void BaseGUIObject::SetBackgroundColor(glm::u8vec4 color)
    {
        BackgroundColour = color;
        RedrawObject();
    }

    glm::u8vec4 BaseGUIObject::GetBackgroundColor() const
    {
        //std::cout<<"GetBackgroundColor: "<<BackgroundColour.r<<","<<BackgroundColour.g<<","<<BackgroundColour.b<<","<<BackgroundColour.a<<"\n";
        
        return BackgroundColour;
    }

    void BaseGUIObject::Delete()
    {
        FUNC_DEBUG_ENTRY();

        //To ensure delete is only called exactly once
        if(Internal_IsDeleted())
        {
            FUNC_DEBUG_EXIT();
            return;
        }
        ObjectDelete = true;

        #if USE_DEBUG
        DEBUG_LINE(this<<" object is getting deleted");
        #endif

        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();

        //Delete children first
        std::vector<ssGUI::GUIObject*> childrenToDelete;
        for(auto childIndex : Children)
        {
            ssGUI::GUIObject* child = CurrentObjectsReferences.GetObjectReference(childIndex);
            if(child != nullptr)
                childrenToDelete.push_back(child);
        }

        for(auto child : childrenToDelete)
            child->Delete();

        //Tell parent to redraw because of missing GUI object
        if(GetParent() != nullptr)
            GetParent()->RedrawObject();

        SetParent(nullptr);
        CurrentObjectsReferences.CleanUp();
        ObjsToDelete.push_back(this);

        FUNC_DEBUG_EXIT();
    }

    bool BaseGUIObject::Internal_IsDeleted() const
    {
        return ObjectDelete;
    }

    void BaseGUIObject::SetHeapAllocated(bool heap)
    {
        HeapAllocated = heap;
    }

    bool BaseGUIObject::IsHeapAllocated() const
    {
        return HeapAllocated;
    }

    std::vector<glm::vec2>& BaseGUIObject::Extension_GetDrawingVertices()
    {
        return DrawingVerticies;
    }

    std::vector<glm::vec2>& BaseGUIObject::Extension_GetDrawingUVs()
    {
        return DrawingUVs;
    }

    std::vector<glm::u8vec4>& BaseGUIObject::Extension_GetDrawingColours()
    {
        return DrawingColours;
    }

    std::vector<int>& BaseGUIObject::Extension_GetDrawingCounts()
    {
        return DrawingCounts;
    }

    std::vector<ssGUI::DrawingProperty>& BaseGUIObject::Extension_GetDrawingProperties()
    {
        return DrawingProperties;
    }

    void BaseGUIObject::AddExtension(ssGUI::Extensions::Extension* extension)
    {
        if(IsExtensionExist(extension->GetExtensionName()))
            return;

        Extensions[extension->GetExtensionName()] = extension;
        ExtensionsDrawOrder.push_back(extension->GetExtensionName());
        ExtensionsUpdateOrder.push_back(extension->GetExtensionName());
        extension->BindToObject(this);
        RedrawObject();
    }

    ssGUI::Extensions::Extension* BaseGUIObject::GetExtension(std::string extensionName)
    {
        if(!IsExtensionExist(extensionName))
            return nullptr;

        return Extensions[extensionName];
    }

    std::vector<ssGUI::Extensions::Extension*> BaseGUIObject::GetListOfExtensions()
    {
        std::vector<ssGUI::Extensions::Extension*> returnVector = std::vector<ssGUI::Extensions::Extension*>();
        
        for(auto extension : Extensions)
            returnVector.push_back(extension.second);
        
        return returnVector;
    }
    

    bool BaseGUIObject::IsExtensionExist(std::string extensionName) const
    {
        return Extensions.find(extensionName) != Extensions.end();
    }

    void BaseGUIObject::RemoveExtension(std::string extensionName)
    {
        if(!IsExtensionExist(extensionName))
            return;
        ssGUI::Extensions::Extension* targetExtension = (*Extensions.find(extensionName)).second;
        // ssGUI::ObjectsReferences* ptr = targetExtension->Internal_GetObjectsReferences();
        // if(ptr != nullptr)
        //     ptr->CleanUp();
        ExtensionsDrawOrder.erase(ExtensionsDrawOrder.begin() + GetExtensionDrawOrder(extensionName));
        ExtensionsUpdateOrder.erase(ExtensionsUpdateOrder.begin() + GetExtensionUpdateOrder(extensionName));
        Extensions.erase(extensionName);
        delete targetExtension;
        RedrawObject();
    }

    int BaseGUIObject::GetExtensionsCount() const
    {
        return ExtensionsDrawOrder.size();
    }

    int BaseGUIObject::GetExtensionDrawOrder(std::string extensionName) const
    {
        if(!IsExtensionExist(extensionName))
            return -1;
        
        auto it = find(ExtensionsDrawOrder.begin(), ExtensionsDrawOrder.end(), extensionName);

        if(it == ExtensionsDrawOrder.end())
            return -1;
        else
            return it - ExtensionsDrawOrder.begin();
    }

    void BaseGUIObject::ChangeExtensionDrawOrder(std::string extensionName, int order)
    {
        int drawIndex = GetExtensionDrawOrder(extensionName);

        if(drawIndex == -1)
            return;
        
        if(order < 0 || order >= GetExtensionsCount())
            return;

        //https://stackoverflow.com/questions/45447361/how-to-move-certain-elements-of-stdvector-to-a-new-index-within-the-vector
        if (drawIndex > order)
            std::rotate(ExtensionsDrawOrder.rend() - drawIndex - 1, ExtensionsDrawOrder.rend() - drawIndex, ExtensionsDrawOrder.rend() - order);
        else        
            std::rotate(ExtensionsDrawOrder.begin() + drawIndex, ExtensionsDrawOrder.begin() + drawIndex + 1, ExtensionsDrawOrder.begin() + order + 1);
        
        RedrawObject();
    }

    int BaseGUIObject::GetExtensionUpdateOrder(std::string extensionName) const
    {
        if(!IsExtensionExist(extensionName))
            return -1;
        
        auto it = find(ExtensionsUpdateOrder.begin(), ExtensionsUpdateOrder.end(), extensionName);

        if(it == ExtensionsUpdateOrder.end())
            return -1;
        else
            return it - ExtensionsUpdateOrder.begin();
    }

    void BaseGUIObject::ChangeExtensionUpdateOrder(std::string extensionName, int order)
    {
        int updateIndex = GetExtensionUpdateOrder(extensionName);

        if(updateIndex == -1)
            return;
        
        if(order < 0 || order >= GetExtensionsCount())
            return;

        //https://stackoverflow.com/questions/45447361/how-to-move-certain-elements-of-stdvector-to-a-new-index-within-the-vector
        if (updateIndex > order)
            std::rotate(ExtensionsUpdateOrder.rend() - updateIndex - 1, ExtensionsUpdateOrder.rend() - updateIndex, ExtensionsUpdateOrder.rend() - order);
        else        
            std::rotate(ExtensionsUpdateOrder.begin() + updateIndex, ExtensionsUpdateOrder.begin() + updateIndex + 1, ExtensionsUpdateOrder.begin() + order + 1);
    
        RedrawObject();
    }

    void BaseGUIObject::AddEventCallback(ssGUI::EventCallbacks::EventCallback* eventCallback)
    {
        if(IsEventCallbackExist(eventCallback->GetEventCallbackName()))
            return;

        EventCallbacks[eventCallback->GetEventCallbackName()] = eventCallback;
        eventCallback->BindToObject(this);
        RedrawObject();
    }

    ssGUI::EventCallbacks::EventCallback* BaseGUIObject::GetEventCallback(std::string eventCallbackName)
    {
        if(!IsEventCallbackExist(eventCallbackName))
            return nullptr;

        return EventCallbacks[eventCallbackName];
    }

    bool BaseGUIObject::IsEventCallbackExist(std::string eventCallbackName) const
    {
        return EventCallbacks.find(eventCallbackName) != EventCallbacks.end();
    }

    void BaseGUIObject::RemoveEventCallback(std::string eventCallbackName)
    {
        if(!IsEventCallbackExist(eventCallbackName))
            return;

        auto ecb = EventCallbacks.at(eventCallbackName);
        EventCallbacks.erase(eventCallbackName);
        delete ecb;
        RedrawObject();
    }

    std::vector<ssGUI::EventCallbacks::EventCallback*> BaseGUIObject::GetListOfEventCallbacks()
    {
        std::vector<ssGUI::EventCallbacks::EventCallback*> returnVector = std::vector<ssGUI::EventCallbacks::EventCallback*>();
        
        for(auto eventCallback : EventCallbacks)
            returnVector.push_back(eventCallback.second);
        
        return returnVector;
    }

    void BaseGUIObject::AddTag(std::string tag)
    {
        if(CurrentTags.find(tag) == CurrentTags.end())
            CurrentTags.insert(tag);
    }

    void BaseGUIObject::RemoveTag(std::string tag)
    {
        if(CurrentTags.find(tag) != CurrentTags.end())
            CurrentTags.erase(tag);
    }

    bool BaseGUIObject::HasTag(std::string tag) const
    {
        return CurrentTags.find(tag) != CurrentTags.end();
    }

    void BaseGUIObject::RedrawObject()
    {
        if(AcceptRedrawRequest)
            Redraw = true;
    }

    bool BaseGUIObject::IsRedrawNeeded() const
    {
        return Redraw;
    }

    ObjectsReferences* BaseGUIObject::Internal_GetObjectsReferences()
    {
        return &CurrentObjectsReferences;
    }

    void BaseGUIObject::Internal_Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        FUNC_DEBUG_ENTRY();

        if(!IsVisible())
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        if(Redraw)
        {
            DisableRedrawObjectRequest();
            
            for(auto extension : ExtensionsDrawOrder)
                Extensions.at(extension)->Internal_Draw(true, drawingInterface, mainWindow, mainWindowPositionOffset);

            for(auto extension : ExtensionsDrawOrder)
                Extensions.at(extension)->Internal_Draw(false, drawingInterface, mainWindow, mainWindowPositionOffset);

            EnableRedrawObjectRequest();

            drawingInterface->DrawEntities(DrawingVerticies, DrawingUVs, DrawingColours, DrawingCounts, DrawingProperties);
            CacheRendering();
            DrawingVerticies.clear();
            DrawingUVs.clear();
            DrawingColours.clear();
            DrawingCounts.clear();
            DrawingProperties.clear();
            Redraw = false;
        }
        else
            drawingInterface->DrawEntities(LastDrawingVerticies, LastDrawingUVs, LastDrawingColours, LastDrawingCounts, LastDrawingProperties);

        FUNC_DEBUG_EXIT();
    }

    void BaseGUIObject::Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();
        
        //If it is not visible, don't even update/draw it
        if(!IsVisible())
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        for(auto extension : ExtensionsUpdateOrder)
        {
            //Guard against extension being deleted by other extensions
            if(!IsExtensionExist(extension))
                continue;

            Extensions.at(extension)->Internal_Update(true, inputInterface, globalInputStatus, windowInputStatus, mainWindow);
        }

        for(auto extension : ExtensionsUpdateOrder)
        {
            //Guard against extension being deleted by other extensions
            if(!IsExtensionExist(extension))
                continue;

            Extensions.at(extension)->Internal_Update(false, inputInterface, globalInputStatus, windowInputStatus, mainWindow);
        }
        
        //Check position different for redraw
        if(GetGlobalPosition() != LastGlobalPosition)
            RedrawObject();

        LastGlobalPosition = GetGlobalPosition();

        FUNC_DEBUG_EXIT();
    }

    GUIObject* BaseGUIObject::Clone(bool cloneChildren)
    {
        FUNC_DEBUG_ENTRY();
        BaseGUIObject* temp = new BaseGUIObject(*this);
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
