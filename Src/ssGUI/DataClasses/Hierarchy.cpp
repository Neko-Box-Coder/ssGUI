#include "ssGUI/DataClasses/Hierarchy.hpp"

#include "ssGUI/GUIObjectClasses/GUIObject.hpp"
#include "ssGUI/DataClasses/EventCallbackManager.hpp"
#include "ssGUI/DataClasses/Renderer.hpp"
#include "ssGUI/DataClasses/Transform.hpp"

#include "ssGUI/EventCallbacks/OnObjectDestroyEventCallback.hpp"
#include "ssGUI/EventCallbacks/OnRecursiveChildRemoveEventCallback.hpp"
#include "ssGUI/EventCallbacks/ChildRemovedEventCallback.hpp"
#include "ssGUI/EventCallbacks/RecursiveChildRemovedEventCallback.hpp"
#include "ssGUI/EventCallbacks/OnRecursiveChildAddEventCallback.hpp"
#include "ssGUI/EventCallbacks/ChildAddedEventCallback.hpp"
#include "ssGUI/EventCallbacks/RecursiveChildAddedEventCallback.hpp"
#include "ssGUI/EventCallbacks/ChildPositionChangedEventCallback.hpp"
#include "ssGUI/EventCallbacks/FocusedEventCallback.hpp"
#include "ssGUI/EventCallbacks/FocusLostEventCallback.hpp"


namespace ssGUI
{
    Hierarchy::Hierarchy(Hierarchy const& other)
    {
        Parent = -1; 
        Children = std::list<ssGUIObjectIndex>(); 
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
        ssLOG_FUNC_ENTRY();
        if(DestroyEventCalled)
        {
            ssLOG_FUNC_EXIT();
            return;
        }
        
        DestroyEventCalled = true;
        if(CurrentEventCallbackManager->IsEventCallbackExist(ssGUI::EventCallbacks::OnObjectDestroyEventCallback::EVENT_NAME))
        {
            CurrentEventCallbackManager->GetEventCallback(ssGUI::EventCallbacks::OnObjectDestroyEventCallback::EVENT_NAME)->Notify(CurrentObject);
            CurrentEventCallbackManager->RemoveEventCallback(ssGUI::EventCallbacks::OnObjectDestroyEventCallback::EVENT_NAME);
        }
        ssLOG_FUNC_EXIT();
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

    void Hierarchy::SetParent(ssGUI::GUIObject* newParent)
    {        
        ssLOG_FUNC_ENTRY();
        
        #if USE_DEBUG
        ssLOG_LINE("Setting "<<CurrentObject<<" parent from "<< CurrentObjectsReferences.GetObjectReference(Parent)<<" to "<<newParent);
        #endif

        CurrentRenderer->RedrawObject();

        ssGUI::GUIObject* originalParent = nullptr;
        //If setting parent to the same, just need to move this to the end of child
        if(Parent != -1 && newParent == CurrentObjectsReferences.GetObjectReference(Parent))
        {
            StashChildrenIterator();
            auto result = newParent->FindChild(CurrentObject);
            if(!result)
            {
                PopChildrenIterator();
                ssLOG_LINE("Invalid parent detected");
                ssLOG_EXIT_PROGRAM();
                return;
            }

            auto it = newParent->GetCurrentChildReferenceIterator();
            newParent->MoveChildrenIteratorToLast();
            auto lastIt = newParent->GetCurrentChildReferenceIterator();
            newParent->ChangeChildOrderToAfterPosition(it, lastIt);
            PopChildrenIterator();
            ssLOG_FUNC_EXIT();
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
                        ssLOG_LINE("Invalid parent detected");
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

                if(originalParent->IsEventCallbackExist(ssGUI::EventCallbacks::OnRecursiveChildRemoveEventCallback::EVENT_NAME))
                    originalParent->GetEventCallback(ssGUI::EventCallbacks::OnRecursiveChildRemoveEventCallback::EVENT_NAME)->Notify(CurrentObject);
                
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
            if(originalParent->IsEventCallbackExist(ssGUI::EventCallbacks::ChildRemovedEventCallback::EVENT_NAME))
                originalParent->GetEventCallback(ssGUI::EventCallbacks::ChildRemovedEventCallback::EVENT_NAME)->Notify(CurrentObject);
        }

        //Send event callback to objects that are subscribed to recursive child removed
        for(auto obj : objsToNotify)
        {
            if(obj->IsEventCallbackExist(ssGUI::EventCallbacks::RecursiveChildRemovedEventCallback::EVENT_NAME))
                obj->GetEventCallback(ssGUI::EventCallbacks::RecursiveChildRemovedEventCallback::EVENT_NAME)->Notify(CurrentObject);
        }

        //Exit if this object is parented to nothing
        if(newParent == nullptr)
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        //Send event callback if any object is subscribed to on recursive child add event
        currentParent = newParent;
        while (currentParent != nullptr)
        {
            if(currentParent == static_cast<ssGUI::GUIObject*>(CurrentObject))
            {
                ssLOG_LINE("Invalid parent detected");
                ssLOG_EXIT_PROGRAM();
                return;
            }
            
            if(currentParent->IsEventCallbackExist(ssGUI::EventCallbacks::OnRecursiveChildAddEventCallback::EVENT_NAME))
                currentParent->GetEventCallback(ssGUI::EventCallbacks::OnRecursiveChildAddEventCallback::EVENT_NAME)->Notify(CurrentObject);    
            
            currentParent = currentParent->GetParent();
        }

        //Inform new parent to add this as child
        newParent->Internal_AddChild(CurrentObject);
        
        //Update local position
        CurrentTransform->SetGlobalPosition(CurrentTransform->GetGlobalPosition());

        //Update focus status for recursive parent objects if this object is focused
        if(IsFocused())
            SetFocus(true);
        
        //Send event callback if any object is subscribed to child added
        if(newParent->IsEventCallbackExist(ssGUI::EventCallbacks::ChildAddedEventCallback::EVENT_NAME))
            newParent->GetEventCallback(ssGUI::EventCallbacks::ChildAddedEventCallback::EVENT_NAME)->Notify(CurrentObject);
        
        //Send event callback if any object is subscribed to recursive children added
        currentParent = CurrentObjectsReferences.GetObjectReference(Parent);
        while (currentParent != nullptr)
        {
            if(currentParent == static_cast<ssGUI::GUIObject*>(CurrentObject))
            {
                ssLOG_LINE("Invalid parent detected");
                ssLOG_EXIT_PROGRAM();
                return;
            }
            
            if(currentParent->IsEventCallbackExist(ssGUI::EventCallbacks::RecursiveChildAddedEventCallback::EVENT_NAME))
                currentParent->GetEventCallback(ssGUI::EventCallbacks::RecursiveChildAddedEventCallback::EVENT_NAME)->Notify(CurrentObject);    
            
            currentParent = currentParent->GetParent();
        }

        ssLOG_FUNC_EXIT();
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
            std::tuple<bool, bool, std::list<ssGUIObjectIndex>::iterator>
            (CurrentChildIteratorFrontEnd, CurrentChildIteratorBackEnd, CurrentChild));
    }

    void Hierarchy::PopChildrenIterator()
    {
        if(StashedChildIterators.empty())
            return;
        
        auto poppedTuple = StashedChildIterators.back();
        CurrentChildIteratorFrontEnd = std::get<0>(poppedTuple);
        CurrentChildIteratorBackEnd = std::get<1>(poppedTuple);
        CurrentChild = std::get<2>(poppedTuple);
        StashedChildIterators.pop_back();
    }

    bool Hierarchy::FindChild(ssGUI::GUIObject* child)
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

    ssGUI::GUIObject* Hierarchy::GetCurrentChild()
    {
        if(!IsChildrenIteratorEnd())
        {
            if(CurrentObjectsReferences.GetObjectReference(*CurrentChild) == nullptr)
            {
                ssLOG_LINE("invalid child found");
                ssLOG_EXIT_PROGRAM();
                return nullptr;
            }
            
            return CurrentObjectsReferences.GetObjectReference(*CurrentChild);
        }
        else
            return nullptr;
    }

    std::list<ssGUIObjectIndex>::iterator Hierarchy::GetCurrentChildReferenceIterator()
    {
        return CurrentChild;
    }

    void Hierarchy::ChangeChildOrderToBeforePosition(std::list<ssGUIObjectIndex>::iterator child, std::list<ssGUIObjectIndex>::iterator position)
    {
        Children.splice(position, Children, child);

        if(CurrentEventCallbackManager->IsEventCallbackExist(ssGUI::EventCallbacks::ChildPositionChangedEventCallback::EVENT_NAME))
            CurrentEventCallbackManager->GetEventCallback(ssGUI::EventCallbacks::ChildPositionChangedEventCallback::EVENT_NAME)->Notify(CurrentObjectsReferences.GetObjectReference(*child));   
    }
    
    void Hierarchy::ChangeChildOrderToAfterPosition(std::list<ssGUIObjectIndex>::iterator child, std::list<ssGUIObjectIndex>::iterator position)
    {
        Children.splice(++position, Children, child);

        if(CurrentEventCallbackManager->IsEventCallbackExist(ssGUI::EventCallbacks::ChildPositionChangedEventCallback::EVENT_NAME))
            CurrentEventCallbackManager->GetEventCallback(ssGUI::EventCallbacks::ChildPositionChangedEventCallback::EVENT_NAME)->Notify(CurrentObjectsReferences.GetObjectReference(*child));   
    }   
    
    std::vector<ssGUI::GUIObject*> Hierarchy::GetListOfChildren() const
    {
        std::vector<ssGUI::GUIObject*> childrenCopy;

        for(auto childIndex : Children)
        {
            auto child = CurrentObjectsReferences.GetObjectReference(childIndex);
            if(child == nullptr)
                continue;

            childrenCopy.push_back(child);
        }

        return childrenCopy;
    }

    void Hierarchy::Internal_AddChild(ssGUI::GUIObject* obj)
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
    
    void Hierarchy::Internal_RemoveChild(ssGUI::GUIObject* obj)
    {
        ssLOG_FUNC_ENTRY();
        
        #if USE_DEBUG
        ssLOG_LINE(CurrentObject<<" removing child "<<obj);
        #endif
        
        StashChildrenIterator();
        if(!FindChild(obj))
        {
            PopChildrenIterator();
            ssLOG_LINE("Remove failed");
            ssLOG_EXIT_PROGRAM();
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
        ssLOG_LINE("Remove success");
        #endif
        ssLOG_FUNC_EXIT();
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
        ssLOG_FUNC_ENTRY();
        
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
        
        bool originalFocus = Focused;
        Focused = focus;

        if(focus != originalFocus)
        {
            if(focus && CurrentEventCallbackManager->IsAnyEventCallbackExist<ssGUI::EventCallbacks::FocusedEventCallback>())
                CurrentEventCallbackManager->GetAnyEventCallback<ssGUI::EventCallbacks::FocusedEventCallback>()->Notify(CurrentObject);
            else if(!focus && CurrentEventCallbackManager->IsAnyEventCallbackExist<ssGUI::EventCallbacks::FocusLostEventCallback>())
                CurrentEventCallbackManager->GetAnyEventCallback<ssGUI::EventCallbacks::FocusLostEventCallback>()->Notify(CurrentObject);
        }

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

        ssLOG_FUNC_EXIT();
    }

    void Hierarchy::Internal_SetSelfFocus(bool focus)
    {
        bool originalFocus = Focused;
        Focused = focus;

        if(focus != originalFocus)
        {
            if(focus && CurrentEventCallbackManager->IsAnyEventCallbackExist<ssGUI::EventCallbacks::FocusedEventCallback>())
                CurrentEventCallbackManager->GetAnyEventCallback<ssGUI::EventCallbacks::FocusedEventCallback>()->Notify(CurrentObject);
            else if(!focus && CurrentEventCallbackManager->IsAnyEventCallbackExist<ssGUI::EventCallbacks::FocusLostEventCallback>())
                CurrentEventCallbackManager->GetAnyEventCallback<ssGUI::EventCallbacks::FocusLostEventCallback>()->Notify(CurrentObject);
        }
    }

    void Hierarchy::Delete()
    {
        ssLOG_FUNC_ENTRY();

        //To ensure delete is only called exactly once
        if(Internal_IsDeleted())
        {
            ssLOG_FUNC_EXIT();
            return;
        }
        ObjectDelete = true;

        #if USE_DEBUG
        ssLOG_LINE(CurrentObject<<" object is getting deleted");
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
        ssGUI::GUIObject::ObjsToDelete.push_back(CurrentObject);

        ssLOG_FUNC_EXIT();
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
    
    void Hierarchy::Internal_ManualDeletion(std::vector<ssGUI::ssGUIObjectIndex> generatedObjs)
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