#include "ssGUI/DataClasses/Hierarchy.hpp"

#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/GUIObjectClasses/GUIObject.hpp"
#include "ssGUI/DataClasses/EventCallbackManager.hpp"
#include "ssGUI/DataClasses/Renderer.hpp"
#include "ssGUI/DataClasses/Transform.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

#include <algorithm>


namespace ssGUI
{
    Hierarchy::Hierarchy(Hierarchy const& other)
    {
        Parent = -1; 
        Children = std::list<ChildInfo>(); 
        CurrentChild = Children.end(); 
        CurrentChildIteratorFrontEnd = true; 
        CurrentChildIteratorBackEnd = true; 
        UserCreated = other.IsUserCreated(); 
        ObjectDelete = other.Internal_IsDeleted(); 
        HeapAllocated = true;//other.IsHeapAllocated(); 
        CurrentObjectsReferences = other.CurrentObjectsReferences; 
        DestroyEventCalled = other.DestroyEventCalled;
        StashedChildIterators = other.StashedChildIterators; 
        Focused = false; 
        CurrentRenderer = nullptr;
        CurrentEventCallbackManager = nullptr;
        CurrentTransform = nullptr;
        CurrentObject = nullptr;
    }

    void Hierarchy::NotifyAndRemoveOnObjectDestroyEventCallbackIfExist()
    {
        ssGUI_LOG_FUNC();
        if(DestroyEventCalled)
            return;

        DestroyEventCalled = true;
        if(CurrentEventCallbackManager->IsEventCallbackExist(ssGUI::Enums::EventType::BEFORE_OBJECT_DESTROY))
        {
            CurrentEventCallbackManager->GetEventCallback(ssGUI::Enums::EventType::BEFORE_OBJECT_DESTROY)->Notify(CurrentObject);
            CurrentEventCallbackManager->RemoveEventCallback(ssGUI::Enums::EventType::BEFORE_OBJECT_DESTROY);
        }
    }
    
    void Hierarchy::AddChild(ssGUI::GUIObject* guiObject,bool compositeChild)
    {
        guiObject->SetParent(CurrentObject, compositeChild);
    }
    
    void Hierarchy::AddChild(   ssGUI::GUIObject* guiObject,  
                                ssGUI::Enums::AlignmentHorizontal horizontalAlignment,
                                ssGUI::Enums::AlignmentVertical verticalAlignment,
                                bool compositeChild)
    {
        AddChild(guiObject, compositeChild);
        auto* ap = guiObject->AddExtension<ssGUI::Extensions::AdvancedPosition>();
        ap->SetAlignment(horizontalAlignment, verticalAlignment);
    }
    
    void Hierarchy::AddChildWithWrapper(ssGUI::GUIObject* guiObject, bool compositeChild)
    {
        auto* wrapper = ssGUI::Create<ssGUI::GUIObject>();
        AddChild(wrapper, compositeChild);
        wrapper->AddChild(guiObject, compositeChild);
    }
    
    void Hierarchy::AddChildWithWrapper(ssGUI::GUIObject* guiObject,  
                                        ssGUI::Enums::AlignmentHorizontal horizontalAlignment,
                                        ssGUI::Enums::AlignmentVertical verticalAlignment,
                                        bool compositeChild)
    {
        AddChildWithWrapper(guiObject, compositeChild);
        auto* ap = guiObject->AddExtension<ssGUI::Extensions::AdvancedPosition>();
        ap->SetAlignment(horizontalAlignment, verticalAlignment);
    }

    Hierarchy::Hierarchy() :    Parent(-1),
                                Children(),
                                CurrentChild(Children.end()),
                                CurrentChildIteratorFrontEnd(true),
                                CurrentChildIteratorBackEnd(true),
                                UserCreated(true),
                                ObjectDelete(false),
                                HeapAllocated(false),
                                CurrentObjectsReferences(),
                                DestroyEventCalled(false),
                                StashedChildIterators(),
                                Focused(false),
                                CurrentRenderer(nullptr),
                                CurrentEventCallbackManager(nullptr),
                                CurrentTransform(nullptr),
                                CurrentObject(nullptr)
    {}

    Hierarchy::~Hierarchy()
    {

    }

    void Hierarchy::SetDependentComponents(ssGUI::Renderer* renderer, ssGUI::EventCallbackManager* eventCallbackManager, ssGUI::Transform* transform, 
        ssGUI::GUIObject* obj)
    {
        CurrentRenderer = renderer;
        CurrentEventCallbackManager = eventCallbackManager;
        CurrentTransform = transform;
        CurrentObject = obj;
    }

    ssGUI::GUIObject* Hierarchy::GetParent() const 
    {
        return Parent == -1 ? nullptr : CurrentObjectsReferences.GetObjectReference(Parent);
    }

    void Hierarchy::SetParent(ssGUI::GUIObject* newParent, bool compositeChild)
    {        
        ssGUI_LOG_FUNC();
        
        ssGUI_DEBUG(ssGUI_DATA_TAG, "Setting "<<CurrentObject<<" parent from "<< CurrentObjectsReferences.GetObjectReference(Parent)<<" to "<<newParent);

        CurrentRenderer->RedrawObject();

        ssGUI::GUIObject* originalParent = nullptr;
        //If setting parent to the same, just need to move this to the end of child
        if(Parent != -1 && newParent == CurrentObjectsReferences.GetObjectReference(Parent))
        {
            StashChildrenIterator();
            if(!newParent->MoveChildrenIteratorToChild(CurrentObject))
            {
                PopChildrenIterator();
                ssGUI_ERROR(ssGUI_DATA_TAG, "Invalid parent detected");
                ssLOG_EXIT_PROGRAM();
                return;
            }

            ChildToken it = newParent->GetCurrentChildToken();
            newParent->MoveChildrenIteratorToLast();
            ChildToken lastIt = newParent->GetCurrentChildToken();
            newParent->ChangeChildOrderToAfterPosition(it, lastIt);
            (*it).CompositeChild = compositeChild;
            PopChildrenIterator();
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
                    if(checkParent == static_cast<ssGUI::GUIObject*>(CurrentObject))
                    {
                        ssGUI_ERROR(ssGUI_DATA_TAG, "Invalid parent detected");
                        ssLOG_EXIT_PROGRAM();
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

                if(originalParent->IsEventCallbackExist(ssGUI::Enums::EventType::BEFORE_RECURSIVE_CHILD_REMOVE))
                    originalParent->GetEventCallback(ssGUI::Enums::EventType::BEFORE_RECURSIVE_CHILD_REMOVE)->Notify(CurrentObject);
                
                originalParent = originalParent->GetParent();
            }

            //Record original parent again
            originalParent = CurrentObjectsReferences.GetObjectReference(Parent);

            //Inform original parent to remove child
            CurrentObjectsReferences.GetObjectReference(Parent)->Internal_RemoveChild(CurrentObject);
        }

        //If the new parent is nullptr, then remove the reference of the original parent and flag it
        if (newParent == nullptr)
        {
            //If this object is focused, remove focus for all objects
            if(IsFocused())
            {
                //Get root parent
                if(originalParent != nullptr)
                {
                    auto rootParent = originalParent;
                    while (rootParent != nullptr)
                    {
                        if(rootParent->GetParent() == nullptr)
                            break;
                        
                        rootParent = rootParent->GetParent();
                    }
                    
                    if(rootParent->GetType() == ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
                    {
                        rootParent->StashChildrenIterator();
                        rootParent->MoveChildrenIteratorToFirst();
                        while (!rootParent->IsChildrenIteratorEnd())
                        {
                            if(rootParent->GetCurrentChild()->IsFocused() && rootParent->GetCurrentChild() != CurrentObject)
                                rootParent->GetCurrentChild()->SetFocus(false);
                            rootParent->MoveChildrenIteratorNext();
                        }
                        rootParent->PopChildrenIterator();
                    }
                    else
                    {
                        rootParent->SetFocus(false);
                    }
                }
            }
            
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
            if(originalParent->IsEventCallbackExist(ssGUI::Enums::EventType::CHILD_REMOVED))
                originalParent->GetEventCallback(ssGUI::Enums::EventType::CHILD_REMOVED)->Notify(CurrentObject);
        }

        //Send event callback to objects that are subscribed to recursive child removed
        for(auto obj : objsToNotify)
        {
            if(obj->IsEventCallbackExist(ssGUI::Enums::EventType::RECURSIVE_CHILD_REMOVED))
                obj->GetEventCallback(ssGUI::Enums::EventType::RECURSIVE_CHILD_REMOVED)->Notify(CurrentObject);
        }

        //Exit if this object is parented to nothing
        if(newParent == nullptr)
            return;

        //Send event callback if any object is subscribed to on recursive child add event
        currentParent = newParent;
        while (currentParent != nullptr)
        {
            if(currentParent == static_cast<ssGUI::GUIObject*>(CurrentObject))
            {
                ssGUI_ERROR(ssGUI_DATA_TAG, "Invalid parent detected");
                ssLOG_EXIT_PROGRAM();
                return;
            }
            
            if(currentParent->IsEventCallbackExist(ssGUI::Enums::EventType::BEFORE_RECURSIVE_CHILD_ADD))
                currentParent->GetEventCallback(ssGUI::Enums::EventType::BEFORE_RECURSIVE_CHILD_ADD)->Notify(CurrentObject);    
            
            currentParent = currentParent->GetParent();
        }

        //Inform new parent to add this as child
        newParent->Internal_AddChild(CurrentObject, compositeChild);
        
        //Update local position
        CurrentTransform->SetGlobalPosition(CurrentTransform->GetGlobalPosition());

        //Update focus status for recursive parent objects if this object is focused
        if(IsFocused())
            SetFocus(true);
        
        //Send event callback if any object is subscribed to child added
        if(newParent->IsEventCallbackExist(ssGUI::Enums::EventType::CHILD_ADDED))
            newParent->GetEventCallback(ssGUI::Enums::EventType::CHILD_ADDED)->Notify(CurrentObject);
        
        //Send event callback if any object is subscribed to recursive children added
        currentParent = CurrentObjectsReferences.GetObjectReference(Parent);
        while (currentParent != nullptr)
        {
            if(currentParent == static_cast<ssGUI::GUIObject*>(CurrentObject))
            {
                ssGUI_ERROR(ssGUI_DATA_TAG, "Invalid parent detected");
                ssLOG_EXIT_PROGRAM();
                return;
            }
            
            if(currentParent->IsEventCallbackExist(ssGUI::Enums::EventType::RECURSIVE_CHILD_ADDED))
                currentParent->GetEventCallback(ssGUI::Enums::EventType::RECURSIVE_CHILD_ADDED)->Notify(CurrentObject);    
            
            currentParent = currentParent->GetParent();
        }
    }

    bool Hierarchy::IsCurrentChildComposite() const
    {
        if(CurrentChildIteratorFrontEnd || CurrentChildIteratorBackEnd)
            return false;

        return CurrentChild->CompositeChild;
    }

    int Hierarchy::GetChildrenCount() const
    {
        return Children.size();
    }

    void Hierarchy::MoveChildrenIteratorToFirst()
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

    void Hierarchy::MoveChildrenIteratorToLast()
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

    void Hierarchy::MoveChildrenIteratorNext()
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

    void Hierarchy::MoveChildrenIteratorPrevious()
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
    
    bool Hierarchy::MoveChildrenIteratorToChild(ssGUI::GUIObject* child)
    {
        if(child == nullptr)
        {
            ssGUI_WARNING(ssGUI_DATA_TAG, "MoveChildrenIteratorToChild is called on nullptr");
            return false;
        }
        
        ssGUIObjectIndex targetIndex = CurrentObjectsReferences.GetObjectIndex(child);

        if(targetIndex < 0)
            return false;
        
        ChildInfo targetInfo;
        targetInfo.ChildIndex = targetIndex;
        targetInfo.CompositeChild = false;
        
        CurrentChild = std::find(Children.begin(), Children.end(), targetInfo);
        return CurrentChild != Children.end();
    }

    bool Hierarchy::IsChildrenIteratorLast()
    {
        if(GetChildrenCount() == 0)
            return false;
        
        if(CurrentChild == --Children.end())
            return true;
        
        return false;
    }

    bool Hierarchy::IsChildrenIteratorFirst()
    {
        if(GetChildrenCount() == 0)
            return false;

        if(CurrentChild == Children.begin())
            return true;

        return false;
    }

    bool Hierarchy::IsChildrenIteratorEnd()
    {
        return CurrentChildIteratorFrontEnd || CurrentChildIteratorBackEnd;
    }

    void Hierarchy::StashChildrenIterator()
    {
        StashedChildIterators.push_back(
            std::tuple<bool, bool, ChildToken>
            (CurrentChildIteratorFrontEnd, CurrentChildIteratorBackEnd, CurrentChild));
    }

    void Hierarchy::PopChildrenIterator()
    {
        if(StashedChildIterators.empty())
            return;
        
        std::tuple<bool, bool, ChildToken> poppedTuple = StashedChildIterators.back();
        CurrentChildIteratorFrontEnd = std::get<0>(poppedTuple);
        CurrentChildIteratorBackEnd = std::get<1>(poppedTuple);
        CurrentChild = std::get<2>(poppedTuple);
        StashedChildIterators.pop_back();
    }

    bool Hierarchy::FindChild(ssGUI::GUIObject* child)
    {
        if(child == nullptr)
            return false;
        
        ssGUIObjectIndex targetIndex = CurrentObjectsReferences.GetObjectIndex(child);
        
        if(targetIndex < 0)
            return false;
        
        ChildInfo targetInfo;
        targetInfo.ChildIndex = targetIndex;
        targetInfo.CompositeChild = child->IsCurrentChildComposite();
        
        return std::find(Children.begin(), Children.end(), targetInfo) != Children.end();
    }
    
    ssGUI::GUIObject* Hierarchy::GetChild(std::string childName, bool recursive) const
    {
        std::vector<ssGUI::GUIObject*> children = GetListOfChildren();
        
        int nonRecursiveChildrenCount = children.size();
        for(int i = 0; i < nonRecursiveChildrenCount; i++)
        {
            if(children[i]->GetName() == childName)
                return children[i];
            
            if(recursive)
            {
                std::vector<ssGUI::GUIObject*> recursiveChildren = children[i]->GetListOfChildren();
                
                for(int j = 0; j < recursiveChildren.size(); j++)
                    children.push_back(recursiveChildren[j]);
            }
        }
        
        if(!recursive)
            return nullptr;

        int currentIndex = nonRecursiveChildrenCount;
        while(currentIndex < children.size())
        {
            if(children[currentIndex]->GetName() == childName)
                return children[currentIndex];
            
            std::vector<ssGUI::GUIObject*> recursiveChildren = children[currentIndex]->GetListOfChildren();
            
            for(int j = 0; j < recursiveChildren.size(); j++)
                children.push_back(recursiveChildren[j]);
            
            currentIndex++;
        }
        
        return nullptr;
    }
    
    void Hierarchy::GetChildrenWithTag( std::string tag, 
                                        std::vector<ssGUI::GUIObject*>& foundChildren, 
                                        bool recursive) const
    {
        std::vector<ssGUI::GUIObject*> children = GetListOfChildren();
        
        int nonRecursiveChildrenCount = children.size();
        for(int i = 0; i < nonRecursiveChildrenCount; i++)
        {
            if(children[i]->HasTag(tag))
                foundChildren.push_back(children[i]);

            if(recursive)
            {
                std::vector<ssGUI::GUIObject*> recursiveChildren = children[i]->GetListOfChildren();
                
                for(int j = 0; j < recursiveChildren.size(); j++)
                    children.push_back(recursiveChildren[j]);
            }
        }
        
        if(!recursive)
            return;

        int currentIndex = nonRecursiveChildrenCount;
        while(currentIndex < children.size())
        {
            if(children[currentIndex]->HasTag(tag))
                foundChildren.push_back(children[currentIndex]);

            std::vector<ssGUI::GUIObject*> recursiveChildren = children[currentIndex]->GetListOfChildren();
            
            for(int j = 0; j < recursiveChildren.size(); j++)
                children.push_back(recursiveChildren[j]);
            
            currentIndex++;
        }
    }

    ssGUI::GUIObject* Hierarchy::GetCurrentChild()
    {
        if(!IsChildrenIteratorEnd())
        {
            if(CurrentObjectsReferences.GetObjectReference(CurrentChild->ChildIndex) == nullptr)
            {
                ssGUI_ERROR(ssGUI_DATA_TAG, "invalid child found: "<<CurrentChild->ChildIndex);
                ssLOG_EXIT_PROGRAM();
                return nullptr;
            }
            
            return CurrentObjectsReferences.GetObjectReference(CurrentChild->ChildIndex);
        }
        else
            return nullptr;
    }

    ssGUI::Hierarchy::ChildToken Hierarchy::GetCurrentChildToken()
    {
        return CurrentChild;
    }
    
    #define ssGUI_FIND_CHILD(targetChild)\
    do\
    {\
        if(!MoveChildrenIteratorToChild(targetChild))\
        {\
            PopChildrenIterator();\
            return false;\
        }\
    } while(0);
    
    #define ssGUI_STASH_AND_FIND_CHILD(targetChild)\
    do\
    {\
        StashChildrenIterator();\
        \
        ssGUI_FIND_CHILD(targetChild);\
    } while(0);
    
    bool Hierarchy::MoveChildBeforeTargetChild(ssGUI::GUIObject* child, ssGUI::GUIObject* target)
    {
        ssGUI_STASH_AND_FIND_CHILD(target);

        ChildToken targetToken = GetCurrentChildToken();
        ssGUI_FIND_CHILD(child);
        
        ChangeChildOrderToBeforePosition(GetCurrentChildToken(), targetToken);
        PopChildrenIterator();
        return true;
    }
    
    bool Hierarchy::MoveChildAfterTargetChild(ssGUI::GUIObject* child, ssGUI::GUIObject* target)
    {
        ssGUI_STASH_AND_FIND_CHILD(target);

        ChildToken targetToken = GetCurrentChildToken();
        ssGUI_FIND_CHILD(child);
        
        ChangeChildOrderToAfterPosition(GetCurrentChildToken(), targetToken);
        PopChildrenIterator();
        return true;
    }
    
    bool Hierarchy::MoveLastChildBeforeTargetChild(ssGUI::GUIObject* target)
    {
        ssGUI_STASH_AND_FIND_CHILD(target);

        ChildToken targetToken = GetCurrentChildToken();
        MoveChildrenIteratorToLast();
        ChangeChildOrderToBeforePosition(GetCurrentChildToken(), targetToken);
        PopChildrenIterator();
        return true;
    }

    bool Hierarchy::MoveLastChildAfterTargetChild(ssGUI::GUIObject* target)
    {
        ssGUI_STASH_AND_FIND_CHILD(target);

        ChildToken targetToken = GetCurrentChildToken();
        MoveChildrenIteratorToLast();
        ChangeChildOrderToAfterPosition(GetCurrentChildToken(), targetToken);
        PopChildrenIterator();
        return true;
    }

    bool Hierarchy::MoveFirstChildBeforeTargetChild(ssGUI::GUIObject* target)
    {
        ssGUI_STASH_AND_FIND_CHILD(target);

        ChildToken targetToken = GetCurrentChildToken();
        MoveChildrenIteratorToFirst();
        ChangeChildOrderToBeforePosition(GetCurrentChildToken(), targetToken);
        PopChildrenIterator();
        return true;
    }

    bool Hierarchy::MoveFirstChildAfterTargetChild(ssGUI::GUIObject* target)
    {
        ssGUI_STASH_AND_FIND_CHILD(target);

        ChildToken targetToken = GetCurrentChildToken();
        MoveChildrenIteratorToFirst();
        ChangeChildOrderToAfterPosition(GetCurrentChildToken(), targetToken);
        PopChildrenIterator();
        return true;
    }
    
    bool Hierarchy::MoveChildToFirst(ssGUI::GUIObject* child)
    {
        ssGUI_STASH_AND_FIND_CHILD(child);
        
        ChildToken childToken = GetCurrentChildToken();
        MoveChildrenIteratorToFirst();
        ChangeChildOrderToBeforePosition(childToken, GetCurrentChildToken());
        PopChildrenIterator();
        return true;
    }

    bool Hierarchy::MoveChildToLast(ssGUI::GUIObject* child)
    {
        ssGUI_STASH_AND_FIND_CHILD(child);
        
        ChildToken childToken = GetCurrentChildToken();
        MoveChildrenIteratorToLast();
        ChangeChildOrderToAfterPosition(childToken, GetCurrentChildToken());
        PopChildrenIterator();
        return true;
    }

    void Hierarchy::ChangeChildOrderToBeforePosition(ssGUI::Hierarchy::ChildToken child, 
                                                        ssGUI::Hierarchy::ChildToken position)
    {
        Children.splice(position, Children, child);

        if(CurrentEventCallbackManager->IsEventCallbackExist(ssGUI::Enums::EventType::CHILD_POSITION_CHANGED))
        {
            CurrentEventCallbackManager->GetEventCallback(ssGUI::Enums::EventType::CHILD_POSITION_CHANGED)->
                Notify(CurrentObjectsReferences.GetObjectReference(child->ChildIndex));   
        }
    }
    
    void Hierarchy::ChangeChildOrderToAfterPosition(ssGUI::Hierarchy::ChildToken child, 
                                                    ssGUI::Hierarchy::ChildToken position)
    {
        Children.splice(++position, Children, child);

        if(CurrentEventCallbackManager->IsEventCallbackExist(ssGUI::Enums::EventType::CHILD_POSITION_CHANGED))
        {
            CurrentEventCallbackManager->GetEventCallback(ssGUI::Enums::EventType::CHILD_POSITION_CHANGED)->
                Notify(CurrentObjectsReferences.GetObjectReference(child->ChildIndex));   
        }
    }   
    
    std::vector<ssGUI::GUIObject*> Hierarchy::GetListOfChildren() const
    {
        std::vector<ssGUI::GUIObject*> childrenCopy;

        for(auto childInfo : Children)
        {
            auto child = CurrentObjectsReferences.GetObjectReference(childInfo.ChildIndex);
            if(child == nullptr)
                continue;

            childrenCopy.push_back(child);
        }

        return childrenCopy;
    }

    bool Hierarchy::HasChildRecursively(ssGUI::GUIObject* searchChild) const
    {
        return searchChild->HasParentRecursively(CurrentObject);
    }
    
    bool Hierarchy::HasParentRecursively(ssGUI::GUIObject* searchParent) const
    {
        ssGUI::GUIObject* currentParent = CurrentObject;
        while(currentParent != nullptr)
        {
            if(currentParent == searchParent)
                return true;
            
            currentParent = currentParent->GetParent();
        }
        
        return false;
    }
    


    void Hierarchy::Internal_AddChild(ssGUI::GUIObject* obj, bool compositeChild)
    {
        StashChildrenIterator();
        
        if(MoveChildrenIteratorToChild(obj))
        {
            PopChildrenIterator();
            return;
        }
        PopChildrenIterator();

        ssGUIObjectIndex childIndex = CurrentObjectsReferences.AddObjectReference(obj);
        
        ChildInfo newChild;
        newChild.ChildIndex = childIndex;
        newChild.CompositeChild = compositeChild;
        Children.push_back(newChild);
    }
    
    void Hierarchy::Internal_RemoveChild(ssGUI::GUIObject* obj)
    {
        ssGUI_LOG_FUNC();
        
        ssGUI_DEBUG(ssGUI_DATA_TAG, CurrentObject<<" removing child "<<obj);
        
        StashChildrenIterator();
        if(!MoveChildrenIteratorToChild(obj))
        {
            PopChildrenIterator();
            ssGUI_ERROR(ssGUI_DATA_TAG, "Remove failed");
            Internal_PrintChildrenStack();
            ssLOG_EXIT_PROGRAM();
            return;
        }
        
        //Go through all stashed child iterators and invalid iterators associated with the child to be removed.
        for(int i = 0; i < StashedChildIterators.size(); i++)
        {
            auto curStashedTuple = StashedChildIterators[i];
            if( !std::get<0>(curStashedTuple) && 
                !std::get<1>(curStashedTuple) &&
                CurrentObjectsReferences.GetObjectReference((*std::get<2>(curStashedTuple)).ChildIndex) == obj)
            {
                std::get<0>(curStashedTuple) = true;
                std::get<1>(curStashedTuple) = true;
                StashedChildIterators[i] = curStashedTuple;
            }
        }

        ChildToken it = GetCurrentChildToken();
        Children.remove(*it);
        PopChildrenIterator();

        ssGUI_DEBUG(ssGUI_DATA_TAG, "Remove success");
    }

    void Hierarchy::SetUserCreated(bool created)
    {
        UserCreated = created;
    }

    bool Hierarchy::IsUserCreated() const
    {
        return UserCreated;
    }

    bool Hierarchy::IsFocused() const
    {
        return Focused;
    }

    void Hierarchy::SetFocus(bool focus)
    {
        ssGUI_LOG_FUNC();
        
        //This goes down the hierarchy tree from the searchParent and disable any focus
        auto disableChildrenFocus = [&](ssGUI::GUIObject* searchParent, ssGUI::GUIObject* excludeChild)
        {
            std::vector<ssGUI::GUIObject*> objsToDisableFocus;
            
            searchParent->StashChildrenIterator();
            searchParent->MoveChildrenIteratorToFirst();
            while (!searchParent->IsChildrenIteratorEnd())
            {
                if(searchParent->GetCurrentChild()->IsFocused() && searchParent->GetCurrentChild() != excludeChild)
                    objsToDisableFocus.push_back(searchParent->GetCurrentChild());
                searchParent->MoveChildrenIteratorNext();
            }
            searchParent->PopChildrenIterator();

            if(objsToDisableFocus.empty())
                return;

            //Populate the list with GUI objects for setting focus false
            int lastObjsCheckIndex = 0;
            do
            {
                auto curObj = objsToDisableFocus[lastObjsCheckIndex];
                curObj->StashChildrenIterator();
                curObj->MoveChildrenIteratorToFirst();
                while (!curObj->IsChildrenIteratorEnd())
                {
                    if(curObj->GetCurrentChild()->IsFocused())
                        objsToDisableFocus.push_back(curObj->GetCurrentChild());
                    curObj->MoveChildrenIteratorNext();
                }
                curObj->PopChildrenIterator();
                lastObjsCheckIndex++;
            }
            while (lastObjsCheckIndex < objsToDisableFocus.size());
            
            //Set the GUI objects focus to false
            for(int i = 0; i < objsToDisableFocus.size(); i++)
                objsToDisableFocus[i]->Internal_SetSelfFocus(false);
        };
        
        Internal_SetSelfFocus(focus);

        if(focus)
        {
            auto lastParent = CurrentObject;
            auto currentParent = GetParent();

            //Set all recursive parents' focus to true
            while(currentParent != nullptr)
            {
                currentParent->Internal_SetSelfFocus(true);
                
                //Set the focus of other children from the parent to be false
                disableChildrenFocus(currentParent, lastParent);
                lastParent = currentParent;
                currentParent = currentParent->GetParent();
            }
        }
        
        //Set the focus of the children of this GUI object to be false
        disableChildrenFocus(CurrentObject, nullptr);
    }

    void Hierarchy::Internal_SetSelfFocus(bool focus)
    {
        bool originalFocus = Focused;
        Focused = focus;

        if(focus != originalFocus)
        {
            if(focus && CurrentEventCallbackManager->IsEventCallbackExist(ssGUI::Enums::EventType::FOCUSED))
                CurrentEventCallbackManager->GetEventCallback(ssGUI::Enums::EventType::FOCUSED)->Notify(CurrentObject);
            else if(!focus && CurrentEventCallbackManager->IsEventCallbackExist(ssGUI::Enums::EventType::FOCUS_LOST))
                CurrentEventCallbackManager->GetEventCallback(ssGUI::Enums::EventType::FOCUS_LOST)->Notify(CurrentObject);
        }
    }

    void Hierarchy::Delete()
    {
        ssGUI_LOG_FUNC();

        //To ensure delete is only called exactly once
        if(Internal_IsDeleted())
            return;

        ssGUI_DEBUG(ssGUI_DATA_TAG, CurrentObject<<" object is getting deleted");
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
        ObjectDelete = true;

        //Delete children first
        std::vector<ssGUI::GUIObject*> childrenToDelete;
        for(auto childPair : Children)
        {
            ssGUI::GUIObject* child = CurrentObjectsReferences.GetObjectReference(childPair.ChildIndex);
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
        ssGUI::GUIObject::ObjsToDelete.push_back(CurrentObject);
    }

    bool Hierarchy::Internal_IsDeleted() const
    {
        return ObjectDelete;
    }

    void Hierarchy::SetHeapAllocated(bool heap)
    {
        HeapAllocated = heap;
    }

    bool Hierarchy::IsHeapAllocated() const
    {
        return HeapAllocated;
    }
    
    void Hierarchy::Internal_PrintParentStack() const
    {
        ssGUI::GUIObject* currentObj = CurrentObject;
        do
        {
            ssLOG_LINE("GUI Object "<< currentObj);
            if(currentObj->GetParent())
                ssLOG_LINE("| is child of");
        }
        while(currentObj->GetParent());
    }
    
    void Hierarchy::Internal_PrintChildrenStack() const
    {
        std::queue<ssGUI::GUIObject*> childrenToIterate;
        childrenToIterate.push(CurrentObject);
        do
        {
            ssLOG_LINE("GUI Object " << childrenToIterate.front());
            
            if(childrenToIterate.front()->GetChildrenCount() == 0)
                ssLOG_LINE("  has no children");
            else
            {
                ssLOG_LINE("| has children");
                std::vector<ssGUI::GUIObject*> curChildren = childrenToIterate.front()->GetListOfChildren();
                
                for(int i = 0; i < curChildren.size(); i++)
                {
                    ssLOG_LINE(curChildren[i]);
                    childrenToIterate.push(curChildren[i]);
                }
            }
            ssLOG_LINE();
            childrenToIterate.pop();
        }
        while(!childrenToIterate.empty());
    }

    void Hierarchy::Internal_ChildrenManualDeletion(std::vector<ssGUI::ssGUIObjectIndex> generatedObjs)
    {
        for(int i = 0; i < generatedObjs.size(); i++)
        {
            auto curObj = CurrentObjectsReferences.GetObjectReference(generatedObjs[i]);

            if(curObj == nullptr)
                continue;

            if(curObj->IsHeapAllocated())
            {
                curObj->Delete();
                delete curObj;
            }
        }
    }

    ObjectsReferences* Hierarchy::Internal_GetObjectsReferences()
    {
        return &CurrentObjectsReferences;
    }
}