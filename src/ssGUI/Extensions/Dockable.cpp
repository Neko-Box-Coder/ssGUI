#include "ssGUI/Extensions/Dockable.hpp"


#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/Extensions/Docker.hpp"

namespace ssGUI::Extensions
{
    bool Dockable::GlobalDockMode = false;
    ssGUI::MainWindow* Dockable::MainWindowUnderDocking = nullptr;
    ssGUI::GUIObject* Dockable::DockingTopLevelParent = nullptr;
    ssGUI::GUIObject* Dockable::TargetDockObject = nullptr;
    Dockable::DockSide Dockable::TargetDockSide = Dockable::DockSide::NONE;
    
    Dockable::Dockable(Dockable const& other)
    {
        Container = nullptr;
        Enabled = other.IsEnabled();
        TopLevelParent = other.TopLevelParent;
        CurrentObjectsReferences = other.CurrentObjectsReferences;
        UseTriggerPercentage = other.IsUseTriggerPercentage();
        TriggerPixel = other.GetTriggerPixel();
        TriggerAreaColor = other.GetTriggerAreaColor();
        DockPreviewColor = other.GetDockPreviewColor();
        OriginalParent = nullptr;
        ContainerIsDocking = false;
        DockPreivewTop = nullptr;
        DockPreivewRight = nullptr;
        DockPreivewBottom = nullptr;
        DockPreivewLeft = nullptr;
        DockTriggerTop = nullptr;
        DockTriggerRight = nullptr;
        DockTriggerBottom = nullptr;
        DockTriggerLeft = nullptr;
        WindowDragStateChangedEventIndex = -1;
    }

    void Dockable::CreateWidgetIfNotPresent(ssGUI::GUIObject** widget, glm::u8vec4 color)
    {
        FUNC_DEBUG_ENTRY();
        
        //If widget is not present, create it
        if((*widget) == nullptr)
        {            
            (*widget) = new ssGUI::Widget();
            (*widget)->SetUserCreated(false);
            (*widget)->SetHeapAllocated(true);
            static_cast<ssGUI::Widget*>((*widget))->SetInteractable(false);
            static_cast<ssGUI::Widget*>((*widget))->SetBlockInput(false);

            ssGUI::Extensions::AdvancedPosition* ap = new ssGUI::Extensions::AdvancedPosition();
            ssGUI::Extensions::AdvancedSize* as = new ssGUI::Extensions::AdvancedSize(); 

            (*widget)->AddExtension(ap);
            (*widget)->AddExtension(as);
        }
        
        (*widget)->AddTag(ssGUI::Tags::OVERLAY);
        (*widget)->SetParent(Container);
        (*widget)->SetBackgroundColour(color);

        FUNC_DEBUG_EXIT();
    }

    
    void Dockable::DrawLeftPreview()
    {
        FUNC_DEBUG_ENTRY();
        
        CreateWidgetIfNotPresent(&DockPreivewLeft, GetDockPreviewColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockPreivewLeft->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* as = static_cast<ssGUI::Extensions::AdvancedSize*>(DockPreivewLeft->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::LEFT);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);

        as->SetHorizontalUsePercentage(true);
        as->SetVerticalUsePercentage(true);
        as->SetHorizontalPercentage(0.5);
        as->SetVerticalPercentage(1);
        
        FUNC_DEBUG_EXIT();
    }

    void Dockable::DrawTopPreview()
    {
        FUNC_DEBUG_ENTRY();
        
        CreateWidgetIfNotPresent(&DockPreivewTop, GetDockPreviewColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockPreivewTop->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* as = static_cast<ssGUI::Extensions::AdvancedSize*>(DockPreivewTop->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::TOP);

        as->SetHorizontalUsePercentage(true);
        as->SetVerticalUsePercentage(true);
        as->SetHorizontalPercentage(1);
        as->SetVerticalPercentage(0.5);
        
        FUNC_DEBUG_EXIT();
    }

    void Dockable::DrawRightPreview()
    {
        FUNC_DEBUG_ENTRY();
        
        CreateWidgetIfNotPresent(&DockPreivewRight, GetDockPreviewColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockPreivewRight->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* as = static_cast<ssGUI::Extensions::AdvancedSize*>(DockPreivewRight->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::RIGHT);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);

        as->SetHorizontalUsePercentage(true);
        as->SetVerticalUsePercentage(true);
        as->SetHorizontalPercentage(0.5);
        as->SetVerticalPercentage(1);

        FUNC_DEBUG_EXIT();
    }

    void Dockable::DrawBottomPreview()
    {
        FUNC_DEBUG_ENTRY();
        
        CreateWidgetIfNotPresent(&DockPreivewBottom, GetDockPreviewColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockPreivewBottom->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* as = static_cast<ssGUI::Extensions::AdvancedSize*>(DockPreivewBottom->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::BOTTOM);

        as->SetHorizontalUsePercentage(true);
        as->SetVerticalUsePercentage(true);
        as->SetHorizontalPercentage(1);
        as->SetVerticalPercentage(0.5);
        
        FUNC_DEBUG_EXIT();
    }

    void Dockable::DiscardLeftPreview()
    {
        if(DockPreivewLeft != nullptr)
        {
            DockPreivewLeft->Delete();
            DockPreivewLeft = nullptr;
        }
    }

    void Dockable::DiscardTopPreview()
    {
        if(DockPreivewTop != nullptr)
        {
            DockPreivewTop->Delete();
            DockPreivewTop = nullptr;
        }
    }

    void Dockable::DiscardRightPreview()
    {
        if(DockPreivewRight != nullptr)
        {
            DockPreivewRight->Delete();
            DockPreivewRight = nullptr;
        }
    }

    void Dockable::DiscardBottomPreview()
    {
        if(DockPreivewBottom != nullptr)
        {
            DockPreivewBottom->Delete();
            DockPreivewBottom = nullptr;
        }
    }


    void Dockable::DrawTriggerAreas()
    {
        //return;
        FUNC_DEBUG_ENTRY();
        
        CreateWidgetIfNotPresent(&DockTriggerTop, GetTriggerAreaColor());
        CreateWidgetIfNotPresent(&DockTriggerRight, GetTriggerAreaColor());
        CreateWidgetIfNotPresent(&DockTriggerBottom, GetTriggerAreaColor());
        CreateWidgetIfNotPresent(&DockTriggerLeft, GetTriggerAreaColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* apTop = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockTriggerTop->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* asTop = static_cast<ssGUI::Extensions::AdvancedSize*>(DockTriggerTop->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ssGUI::Extensions::AdvancedPosition* apRight = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockTriggerRight->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* asRight = static_cast<ssGUI::Extensions::AdvancedSize*>(DockTriggerRight->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ssGUI::Extensions::AdvancedPosition* apBottom = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockTriggerBottom->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* asBottom = static_cast<ssGUI::Extensions::AdvancedSize*>(DockTriggerBottom->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ssGUI::Extensions::AdvancedPosition* apLeft = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockTriggerLeft->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* asLeft = static_cast<ssGUI::Extensions::AdvancedSize*>(DockTriggerLeft->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        asTop->SetHorizontalUsePercentage(UseTriggerPercentage);
        asTop->SetVerticalUsePercentage(UseTriggerPercentage);
        asRight->SetHorizontalUsePercentage(UseTriggerPercentage);
        asRight->SetVerticalUsePercentage(UseTriggerPercentage);
        asBottom->SetHorizontalUsePercentage(UseTriggerPercentage);
        asBottom->SetVerticalUsePercentage(UseTriggerPercentage);
        asLeft->SetHorizontalUsePercentage(UseTriggerPercentage);
        asLeft->SetVerticalUsePercentage(UseTriggerPercentage);

        if(UseTriggerPercentage)
        {
            apTop->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
            apTop->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::TOP);
            asTop->SetHorizontalPercentage(1 - GetTriggerPercentage() * 2);
            asTop->SetVerticalPercentage(GetTriggerPercentage());

            apRight->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::RIGHT);
            apRight->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);
            asRight->SetHorizontalPercentage(GetTriggerPercentage());
            asRight->SetVerticalPercentage(1 - GetTriggerPercentage() * 2);

            apBottom->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
            apBottom->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::BOTTOM);
            asBottom->SetHorizontalPercentage(1 - GetTriggerPercentage() * 2);
            asBottom->SetVerticalPercentage(GetTriggerPercentage());

            apLeft->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::LEFT);
            apLeft->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);
            asLeft->SetHorizontalPercentage(GetTriggerPercentage());
            asLeft->SetVerticalPercentage(1 - GetTriggerPercentage() * 2);
        }
        else
        {
            apTop->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
            apTop->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::TOP);
            asTop->SetHorizontalPixel(Container->GetSize().x - GetTriggerPixel() * 2);
            asTop->SetVerticalPixel(GetTriggerPixel());

            apRight->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::RIGHT);
            apRight->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);
            asRight->SetHorizontalPixel(GetTriggerPixel());
            asRight->SetVerticalPixel(Container->GetSize().y - GetTriggerPixel() * 2);

            apBottom->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
            apBottom->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::BOTTOM);
            asBottom->SetHorizontalPixel(Container->GetSize().x - GetTriggerPixel() * 2);
            asBottom->SetVerticalPixel(GetTriggerPercentage());

            apLeft->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::LEFT);
            apLeft->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);
            asLeft->SetHorizontalPixel(GetTriggerPixel());
            asLeft->SetVerticalPixel(Container->GetSize().y - GetTriggerPixel() * 2);
        }

        FUNC_DEBUG_EXIT();
    }

    //TODO : Add mutex for multi-threading support
    void Dockable::OnWindowDragStarted()
    {        
        FUNC_DEBUG_ENTRY();
        
        ContainerIsDocking = true;
        GlobalDockMode = true;

        //Find the Main Window
        ssGUI::GUIObject* curParent = TopLevelParent == -1 || CurrentObjectsReferences.GetObjectReference(TopLevelParent) == nullptr ? 
                                        Container->GetParent() : CurrentObjectsReferences.GetObjectReference(TopLevelParent);
        while (curParent->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW && curParent != nullptr)
        {
            curParent = curParent->GetParent();
        }
        
        //If this has no parent. Docking is disabled
        if(curParent == nullptr)
        {
            ContainerIsDocking = false;
            GlobalDockMode = false;
            OriginalParent = nullptr;
            MainWindowUnderDocking = nullptr;
            DockingTopLevelParent = nullptr;
            FUNC_DEBUG_EXIT();
            return;
        }

        MainWindowUnderDocking = static_cast<ssGUI::MainWindow*>(curParent);

        Container->AddTag(ssGUI::Tags::FLOATING);
        ssGUI::GUIObject* containerParent = Container->GetParent();

        //Docking mechanism
        //Check if container is docked under docker. If so, set the size for the child after container to fill the gap
        if(containerParent->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME) && containerParent->GetChildrenCount() > 1)
        {
            containerParent->FindChild(Container);
            if(!containerParent->IsChildrenIteratorLast())
            {
                containerParent->MoveChildrenIteratorNext();
                glm::ivec2 childSize = containerParent->GetCurrentChild()->GetSize();
                if(static_cast<ssGUI::Extensions::Layout*>(containerParent->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME))->IsHorizontalLayout())
                    containerParent->GetCurrentChild()->SetSize(glm::ivec2(childSize.x + Container->GetSize().x, childSize.y));
                else
                    containerParent->GetCurrentChild()->SetSize(glm::ivec2(childSize.x, childSize.y + Container->GetSize().y));                
            }
        }

        //Parent the container to the MainWindow.
        OriginalParent = Container->GetParent();

        if(TopLevelParent == -1 || CurrentObjectsReferences.GetObjectReference(TopLevelParent) == nullptr)
        {
            Container->SetParent(MainWindowUnderDocking);
            DockingTopLevelParent = MainWindowUnderDocking;
        }
        else
        {
            Container->SetParent(CurrentObjectsReferences.GetObjectReference(TopLevelParent));
            DockingTopLevelParent = CurrentObjectsReferences.GetObjectReference(TopLevelParent);
        }

        FUNC_DEBUG_EXIT();
    }

    void Dockable::RemoveUnnecessaryDocker(ssGUI::GUIObject* checkObj)
    {
        FUNC_DEBUG_ENTRY();

        if(checkObj == nullptr || checkObj->IsUserCreated() || !
            checkObj->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME))
        {
            FUNC_DEBUG_EXIT();
            return;
        }
        
        //Delete all empty generated dockers until a docker that holds an actual child/children
        ssGUI::GUIObject* childrenHolder = checkObj;
        std::vector<ssGUI::GUIObject*> objsToDelete;
        while (childrenHolder->GetChildrenCount() <= 1)
        {   
            if(childrenHolder->GetChildrenCount() == 1)
            {
                childrenHolder->MoveChildrenIteratorToFirst();

                if(childrenHolder->GetCurrentChild()->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME) &&
                    !childrenHolder->GetCurrentChild()->IsUserCreated())
                {
                    ssGUI::GUIObject* newChildrenHolder = childrenHolder->GetCurrentChild();
                    newChildrenHolder->SetParent(childrenHolder->GetParent());
                    newChildrenHolder->SetPosition(childrenHolder->GetPosition());
                    newChildrenHolder->SetSize(childrenHolder->GetSize());
                    objsToDelete.push_back(childrenHolder);
                    childrenHolder = newChildrenHolder;
                }
                else 
                    break;
            }
            else
            {
                objsToDelete.push_back(childrenHolder);
                break;
            }
        }
        
        //If the childrenHolder is empty, just clean up empty dockers
        if(childrenHolder->GetChildrenCount() <= 0)
            goto cleanUpEmptyDockers;
        
        //If the childrenHolder only has 1 child, 
        //just move the child out of childrenHolder and restore order, size and position.
        if(childrenHolder->GetChildrenCount() == 1)
        {
            childrenHolder->MoveChildrenIteratorToFirst();
            ssGUI::GUIObject* onlyChild = childrenHolder->GetCurrentChild();
            onlyChild->SetParent(childrenHolder->GetParent());

            //Restore order
            if(childrenHolder->GetParent() != nullptr)
            {
                childrenHolder->GetParent()->FindChild(childrenHolder);
                std::list<ssGUIObjectIndex>::iterator originalIt = childrenHolder->GetParent()->GetCurrentChildReferenceIterator();
                childrenHolder->GetParent()->MoveChildrenIteratorToLast();
                std::list<ssGUIObjectIndex>::iterator lastIt = childrenHolder->GetParent()->GetCurrentChildReferenceIterator();
                childrenHolder->GetParent()->ChangeChildOrderToAfterPosition(lastIt, originalIt);
            }
            //Restore size
            onlyChild->SetSize(childrenHolder->GetSize());
            //Restore position
            onlyChild->SetPosition(childrenHolder->GetPosition());

            objsToDelete.push_back(childrenHolder);
        }
        //Check if parent of childrenHolder has docker
        //If so, transfer the children to it and set the correct layout orientation.
        else if(childrenHolder->GetParent() != nullptr && 
            childrenHolder->GetParent()->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME) &&
            childrenHolder->GetParent()->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
        {
            std::vector<ssGUI::GUIObject*> objsToMove;
            std::vector<glm::ivec2> objsSizes;
            
            childrenHolder->MoveChildrenIteratorToFirst();
            while (!childrenHolder->IsChildrenIteratorEnd())
            {
                objsToMove.push_back(childrenHolder->GetCurrentChild());
                objsSizes.push_back(childrenHolder->GetCurrentChild()->GetSize());
                childrenHolder->MoveChildrenIteratorNext();
            }
            
            ssGUI::Extensions::Layout* childrenHolderParentLayout = static_cast<ssGUI::Extensions::Layout*>
                                                                    (childrenHolder->GetParent()->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME));

            ssGUI::Extensions::Layout* childrenHolderLayout = static_cast<ssGUI::Extensions::Layout*>
                                                                (childrenHolder->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME));


            //If the only child of childrenHolder's parent is childrenHolder, transfer children
            if(childrenHolder->GetParent()->GetChildrenCount() == 1)
            {
                //Restore sizes
                for(int i = 0; i < objsToMove.size(); i++)
                {
                    objsToMove[i]->SetParent(childrenHolder->GetParent());
                    objsToMove[i]->SetSize(objsSizes[i]);
                }
                //Restore layout settings 
                if(childrenHolder->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
                {
                    childrenHolderParentLayout->SetHorizontalLayout(childrenHolderLayout->IsHorizontalLayout());
                    childrenHolderParentLayout->SetReverseOrder(childrenHolderLayout->IsReverseOrder());
                }
                objsToDelete.push_back(childrenHolder);
            }
            //else if the layout orientation of childrenHolder's parent is the same as childrenHolder, transfer children
            else if(childrenHolderLayout->IsHorizontalLayout() == childrenHolderParentLayout->IsHorizontalLayout())
            {
                //Store order
                childrenHolder->GetParent()->FindChild(childrenHolder);
                std::list<ssGUIObjectIndex>::iterator it = childrenHolder->GetParent()->GetCurrentChildReferenceIterator();

                //If both layout are in same order
                if(childrenHolderLayout->IsReverseOrder() == childrenHolderParentLayout->IsReverseOrder())
                {
                    //Restore sizes
                    for(int i = 0; i < objsToMove.size(); i++)
                    {
                        objsToMove[i]->SetParent(childrenHolder->GetParent());
                        objsToMove[i]->SetSize(objsSizes[i]);

                        //Restore order
                        childrenHolder->GetParent()->MoveChildrenIteratorToLast();
                        childrenHolder->GetParent()->ChangeChildOrderToAfterPosition
                            (childrenHolder->GetParent()->GetCurrentChildReferenceIterator(), it);

                        it++;
                    }
                }
                //If both layout are not in same order
                else
                {
                    //Restore sizes
                    for(int i = objsToMove.size() - 1; i >= 0; i--)
                    {
                        objsToMove[i]->SetParent(childrenHolder->GetParent());
                        objsToMove[i]->SetSize(objsSizes[i]);

                        //Restore order
                        childrenHolder->GetParent()->MoveChildrenIteratorToLast();
                        childrenHolder->GetParent()->ChangeChildOrderToAfterPosition
                            (childrenHolder->GetParent()->GetCurrentChildReferenceIterator(), it);

                        it++;
                    }
                }

                objsToDelete.push_back(childrenHolder);
            }
        }
        

        //Remove empty dockers
        cleanUpEmptyDockers:;
        
        for(auto obj : objsToDelete)
            obj->Delete();
        
        
        FUNC_DEBUG_EXIT();
    }

    /*
    void Dockable::RemoveOriginalParentIfNeeded()
    {
        FUNC_DEBUG_ENTRY();
        if(OriginalParent->GetChildrenCount() == 1)
        {
            OriginalParent->MoveChildrenIteratorToFirst();
            ssGUI::GUIObject* child = OriginalParent->GetCurrentChild();
            
            //Restore order and size
            if(OriginalParent->GetParent() != nullptr)
            {
                child->SetPosition(OriginalParent->GetPosition());
                
                OriginalParent->GetParent()->FindChild(OriginalParent);
                std::list<ssGUIObjectIndex>::iterator posObjectIt = OriginalParent->GetParent()->GetCurrentChildReferenceIterator();
                child->SetParent(OriginalParent->GetParent());
                OriginalParent->GetParent()->MoveChildrenIteratorToLast();
                std::list<ssGUIObjectIndex>::iterator lastIt = OriginalParent->GetParent()->GetCurrentChildReferenceIterator();
                child->GetParent()->ChangeChildOrderToBeforePosition(lastIt, posObjectIt);
                child->SetSize(OriginalParent->GetSize());
                OriginalParent->Delete();
                
                //Check if the child is a docker and if the parent of child now does not have lauout. (Floating)
                //If so, make the child draggable 
                if(!child->IsUserCreated() &&
                    child->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME) && 
                    !child->GetParent()->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
                {
                    ssGUI::Window* childDocker = dynamic_cast<ssGUI::Window*>(child);

                    childDocker->SetTitlebar(true);
                    childDocker->SetDraggable(true);
                    childDocker->SetResizeType(ssGUI::Enums::ResizeType::ALL);
                    childDocker->SetBackgroundColour(static_cast<ssGUI::Extensions::Docker*>(child->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME))->GetFloatingDockerColor());
                }
                //Check if the child is a docker and the parent of child now is a docker ot not. 
                //If so, move the contents of child to the parent recursively
                else if(!child->IsUserCreated());
            }
            else
            {
                child->SetParent(OriginalParent->GetParent());   
                OriginalParent->Delete();
            }
        }
        else
            OriginalParent->Delete();

        FUNC_DEBUG_EXIT();
    }*/

    void Dockable::FindDockLayout(ssGUI::Extensions::Layout*& dockLayout)
    {
        FUNC_DEBUG_ENTRY();
        
        ssGUI::Extensions::Layout* parentLayout = static_cast<ssGUI::Extensions::Layout*>(TargetDockObject->GetParent()->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME));

        if(TargetDockObject->GetParent()->GetChildrenCount() > 1)
        {
            if(parentLayout->IsHorizontalLayout() && (TargetDockSide == DockSide::LEFT || TargetDockSide == DockSide::RIGHT))
                dockLayout = parentLayout;
            else if(!parentLayout->IsHorizontalLayout() && (TargetDockSide == DockSide::TOP || TargetDockSide == DockSide::BOTTOM))
                dockLayout = parentLayout;
        }
        else
        {
            if(TargetDockSide == DockSide::LEFT || TargetDockSide == DockSide::RIGHT)
                parentLayout->SetHorizontalLayout(true);
            else
                parentLayout->SetHorizontalLayout(false);

            dockLayout = parentLayout;
        }

        FUNC_DEBUG_EXIT();
    }

    void Dockable::CreateEmptyParentForDocking(ssGUI::Extensions::Layout*& dockLayout)
    {
        FUNC_DEBUG_ENTRY();
        ssGUI::Window* newParent = new ssGUI::Window();

        newParent->SetUserCreated(false);
        newParent->SetHeapAllocated(true);
        newParent->SetSize(TargetDockObject->GetSize());
        newParent->SetAnchorType(TargetDockObject->GetAnchorType());
        newParent->SetPosition(TargetDockObject->GetPosition());
        newParent->SetParent(TargetDockObject->GetParent());

        //The docker will automatically create layout extension if not exist
        newParent->AddExtension(new ssGUI::Extensions::Docker());
        dockLayout = static_cast<ssGUI::Extensions::Layout*>(newParent->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME));

        //Check if the generated docker does not use parent docker or not. If so, use default docker & layout settings if present
        if(newParent->GetParent()->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME) 
            && !static_cast<ssGUI::Extensions::Docker*>(newParent->GetParent()->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME))->IsChildrenDockerUseThisSettings()
            && static_cast<ssGUI::Extensions::Docker*>(newParent->GetParent()->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME))->IsEnabled())
        {
            if(ssGUI::Extensions::Docker::GetDefaultGeneratedDockerSettings() != nullptr)
                newParent->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME)->Copy(ssGUI::Extensions::Docker::GetDefaultGeneratedDockerSettings());

            if(ssGUI::Extensions::Docker::GetDefaultGeneratedLayoutSettings() != nullptr)
                newParent->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME)->Copy(ssGUI::Extensions::Docker::GetDefaultGeneratedLayoutSettings());
        }
        
        //Then change the Layout orientation to the same as the docking orientation
        if(TargetDockSide == DockSide::LEFT || TargetDockSide == DockSide::RIGHT)
            dockLayout->SetHorizontalLayout(true);
        else
            dockLayout->SetHorizontalLayout(false);

        //Floating
        //ssGUI::GUIObject* topLevelParent = (TopLevelParent == -1 ? nullptr : CurrentObjectsReferences.GetObjectReference(TopLevelParent));
        if(!TargetDockObject->GetParent()->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME))
        {
            newParent->SetBackgroundColour(static_cast<ssGUI::Extensions::Docker*>(newParent->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME))->GetFloatingDockerColor());
        }
        //Non Floating
        else
        {
            newParent->SetTitlebar(false);
            newParent->SetBackgroundColour(glm::u8vec4(0, 0, 0, 0));
            newParent->SetResizeType(ssGUI::Enums::ResizeType::NONE);
        }

        //Restore order
        TargetDockObject->GetParent()->FindChild(TargetDockObject);
        std::list<ssGUIObjectIndex>::iterator dockObjectIt = TargetDockObject->GetParent()->GetCurrentChildReferenceIterator();
        TargetDockObject->GetParent()->MoveChildrenIteratorToLast();
        std::list<ssGUIObjectIndex>::iterator lastIt = TargetDockObject->GetParent()->GetCurrentChildReferenceIterator();
        
        TargetDockObject->GetParent()->ChangeChildOrderToBeforePosition(lastIt, dockObjectIt);
        TargetDockObject->SetParent(newParent);
        //Setting a new parent from the dock will causes it to revert to original size. 
        //Therefore will need to set the size to match the new parent again.
        TargetDockObject->SetSize(newParent->GetSize());
        FUNC_DEBUG_EXIT();
    }

    void Dockable::OnWindowDragFinished()
    {                
        FUNC_DEBUG_ENTRY();

        //Remove the floating tag to allow docking
        Container->RemoveTag(ssGUI::Tags::FLOATING);

        //Docking mechanism for dockable window
        if(TargetDockObject != nullptr && TargetDockObject->GetParent() != nullptr && TargetDockSide != DockSide::NONE)
        {
            //If it is just docker, just need to add it as a child
            if(TargetDockObject->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME))
            {
                Container->SetParent(TargetDockObject);
                goto reset;
            }
            //Otherwise dock as normal
            ssGUI::Extensions::Layout* dockLayout = nullptr;
            
            //Check if the parent of the targetDockObject has the Docker extension
            if(TargetDockObject->GetParent()->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME) && TargetDockObject->GetParent()->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME))
                FindDockLayout(dockLayout);

            //Create an empty parent and add Layout extension if dock layout isn't found
            if(dockLayout == nullptr)
                CreateEmptyParentForDocking(dockLayout);
            
            //This inserts the container to the end. Halfing the size for TargetDockObject and Container so they fit the original space
            TargetDockObject->SetSize(glm::ivec2(TargetDockObject->GetSize().x * 0.5, TargetDockObject->GetSize().y * 0.5));
            glm::ivec2 newContainerSize = TargetDockObject->GetSize();
            Container->SetParent(TargetDockObject->GetParent());
            Container->SetSize(newContainerSize);

            //Insert the Container after/before it
            TargetDockObject->GetParent()->FindChild(TargetDockObject);
            std::list<ssGUIObjectIndex>::iterator dockObjectIt = TargetDockObject->GetParent()->GetCurrentChildReferenceIterator();
            TargetDockObject->GetParent()->MoveChildrenIteratorToLast();
            std::list<ssGUIObjectIndex>::iterator lastIt = TargetDockObject->GetParent()->GetCurrentChildReferenceIterator();
            
            if(!dockLayout->IsReverseOrder())
            {                
                //Before
                if(TargetDockSide == DockSide::LEFT || TargetDockSide == DockSide::TOP)
                    TargetDockObject->GetParent()->ChangeChildOrderToBeforePosition(lastIt, dockObjectIt);
                //After
                else
                    TargetDockObject->GetParent()->ChangeChildOrderToAfterPosition(lastIt, dockObjectIt);
            }
            else
            {
                //Before
                if(TargetDockSide == DockSide::RIGHT || TargetDockSide == DockSide::BOTTOM)
                    TargetDockObject->GetParent()->ChangeChildOrderToBeforePosition(lastIt, dockObjectIt);
                //After
                else
                    TargetDockObject->GetParent()->ChangeChildOrderToAfterPosition(lastIt, dockObjectIt);
            }
        }
        
        reset:
        if(OriginalParent != nullptr)
        {
            ssGUI::GUIObject* parentOfOriginalParent = OriginalParent->GetParent();

            //Check if originalParent's child has any unnecessary docker
            if(!OriginalParent->Internal_IsDeleted() && OriginalParent->GetChildrenCount() == 1)
            {
                OriginalParent->MoveChildrenIteratorToFirst();
                RemoveUnnecessaryDocker(OriginalParent->GetCurrentChild());
            }
            
            //Check if originalParent has any unnecessary docker if it doesn't have any child
            if(OriginalParent->GetChildrenCount() == 0) 
                RemoveUnnecessaryDocker(OriginalParent);
            
            //Check if originalParent has any unnecessary docker if it is docked
            else if(parentOfOriginalParent != nullptr && 
                    parentOfOriginalParent->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME))
            {
                RemoveUnnecessaryDocker(OriginalParent);
            }
            //check if originalParent has any unnecessary docker if it is NOT docked and has 1 or no child
            else if(OriginalParent->GetChildrenCount() <= 1 && parentOfOriginalParent != nullptr && 
                    !parentOfOriginalParent->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME))
            {
                RemoveUnnecessaryDocker(OriginalParent);
            }

            //Check if the parent of originalParent (and its child) has any unnecessary docker if the originalParent is deleted
            if(parentOfOriginalParent != nullptr && OriginalParent->Internal_IsDeleted())
            {
                if(parentOfOriginalParent->GetChildrenCount() == 1)
                {
                    parentOfOriginalParent->MoveChildrenIteratorToFirst();
                    RemoveUnnecessaryDocker(parentOfOriginalParent->GetCurrentChild());
                }

                RemoveUnnecessaryDocker(parentOfOriginalParent);
            }
        }
            
        //Reset docking variables
        OriginalParent = nullptr;
        ContainerIsDocking = false;
        GlobalDockMode = false;
        MainWindowUnderDocking = nullptr;
        DockingTopLevelParent = nullptr;
        TargetDockObject = nullptr;
        TargetDockSide = DockSide::NONE;

        FUNC_DEBUG_EXIT();
    }
    
    void Dockable::DiscardTriggerAreas()
    {
        if(DockTriggerTop != nullptr)
        {
            DockTriggerTop->Delete();
            DockTriggerTop = nullptr;
        }

        if(DockTriggerRight != nullptr)
        {
            DockTriggerRight->Delete();
            DockTriggerRight = nullptr;
        }

        if(DockTriggerBottom != nullptr)
        {
            DockTriggerBottom->Delete();
            DockTriggerBottom = nullptr;
        }

        if(DockTriggerLeft != nullptr)
        {
            DockTriggerLeft->Delete();
            DockTriggerLeft = nullptr;
        }
    }
    
    const std::string Dockable::EXTENSION_NAME = "Dockable";

    Dockable::Dockable() : Container(nullptr), Enabled(true), TopLevelParent(-1), CurrentObjectsReferences(), UseTriggerPercentage(true), 
                            TriggerPercentage(0.25f), TriggerPixel(15), TriggerAreaColor(glm::u8vec4(87, 207, 255, 127)), DockPreviewColor(glm::u8vec4(255, 255, 255, 127)), OriginalParent(nullptr),
                            ContainerIsDocking(false), DockPreivewTop(nullptr), DockPreivewRight(nullptr), DockPreivewBottom(nullptr), DockPreivewLeft(nullptr),
                            DockTriggerTop(nullptr), DockTriggerRight(nullptr), DockTriggerBottom(nullptr), DockTriggerLeft(nullptr), WindowDragStateChangedEventIndex(-1)
    {}
    
    Dockable::~Dockable()
    {
        if(WindowDragStateChangedEventIndex != -1 && Container != nullptr && 
            Container->IsEventCallbackExist(ssGUI::EventCallbacks::WindowDragStateChangedEventCallback::EVENT_NAME))
        {
            Container->GetEventCallback(ssGUI::EventCallbacks::WindowDragStateChangedEventCallback::EVENT_NAME)->
                RemoveEventListener(WindowDragStateChangedEventIndex);
            
            if(Container->GetEventCallback(ssGUI::EventCallbacks::WindowDragStateChangedEventCallback::EVENT_NAME)->
                GetEventListenerCount() == 0)
            {
                Container->RemoveEventCallback(ssGUI::EventCallbacks::WindowDragStateChangedEventCallback::EVENT_NAME);
            }
        }

        CurrentObjectsReferences.CleanUp();
    }

    void Dockable::SetTriggerPercentage(float percentage)
    {
        TriggerPercentage = percentage;
    }

    float Dockable::GetTriggerPercentage() const
    {
        return TriggerPercentage;
    }
    
    void Dockable::SetTriggerPixel(int pixel)
    {
        TriggerPixel = pixel;
    }

    int Dockable::GetTriggerPixel() const
    {
        return TriggerPixel;
    }

    void Dockable::SetUseTriggerPercentage(bool usePercentage)
    {
        UseTriggerPercentage = usePercentage;
    }

    bool Dockable::IsUseTriggerPercentage() const
    {
        return UseTriggerPercentage;
    }

    void Dockable::SetTriggerAreaColor(glm::u8vec4 color)
    {
        TriggerAreaColor = color;
    }

    glm::u8vec4 Dockable::GetTriggerAreaColor() const
    {
        return TriggerAreaColor;
    }

    void Dockable::SetDockPreviewColor(glm::u8vec4 color)
    {
        DockPreviewColor = color;
    }

    glm::u8vec4 Dockable::GetDockPreviewColor() const
    {
        return DockPreviewColor;
    }

    void Dockable::SetTopLevelParent(ssGUI::GUIObject* parent)
    {
        if(parent != nullptr)
        {
            if(TopLevelParent != -1 && CurrentObjectsReferences.GetObjectReference(TopLevelParent) != nullptr)
                CurrentObjectsReferences.SetObjectReference(TopLevelParent, parent);
            else
                TopLevelParent = CurrentObjectsReferences.AddObjectReference(parent);
        }
        else
            TopLevelParent = -1;
    }

    ssGUI::GUIObject* Dockable::GetTopLevelParent() const
    {
        if(TopLevelParent != -1 && CurrentObjectsReferences.GetObjectReference(TopLevelParent) != nullptr)
            return nullptr;
        else
            return CurrentObjectsReferences.GetObjectReference(TopLevelParent);
    }   

    void Dockable::SetEnabled(bool enabled)
    {
        Enabled = enabled;
    }

    bool Dockable::IsEnabled() const
    {
        return Enabled;
    }

    //function: Update
    void Dockable::Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();
        
        if(!IsPreUpdate || Container == nullptr || !Enabled)
        {
            FUNC_DEBUG_EXIT();
            return;
        }
        
        //If global dock mode is true, check topLevelParent first, then check the cursor against the trigger area
        if(GlobalDockMode && !ContainerIsDocking && !globalInputStatus.DockingBlocked)
        {
            ssGUI::GUIObject* curParent = Container;
            while (curParent->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW && curParent != nullptr && curParent != DockingTopLevelParent)
            {
                curParent = curParent->GetParent();
            }

            //If this is not the same parent as the one which is being docked, exit
            if((DockingTopLevelParent == MainWindowUnderDocking && curParent != MainWindowUnderDocking) || 
                curParent != DockingTopLevelParent)
            {
                DiscardLeftPreview();
                DiscardTopPreview();
                DiscardRightPreview();
                DiscardBottomPreview();
                DiscardTriggerAreas();
                FUNC_DEBUG_EXIT();
                return;
            }
            
            glm::ivec2 containerPos = Container->GetGlobalPosition();
            glm::ivec2 containerSize = Container->GetSize();
            int titleBarOffset = Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && Container->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW ? 
                                    static_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() : 0;
            containerSize.y -= titleBarOffset;
            
            glm::ivec2 triggerSize = IsUseTriggerPercentage() ? glm::ivec2(glm::vec2(containerSize) * GetTriggerPercentage()) : glm::ivec2(GetTriggerPixel(), GetTriggerPixel());

            //Inform layout to exclude triggers and previews visual widgets
            ssGUI::Extensions::Layout* containerLayout = Container->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME) ? 
                                                            static_cast<ssGUI::Extensions::Layout*>(Container->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME)) : nullptr;

            bool previewDrawn = false;

            //Left
            if(inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).x >= containerPos.x && 
                inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).x <= containerPos.x + triggerSize.x &&
                inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).y >= containerPos.y + titleBarOffset + triggerSize.y && 
                inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).y <= containerPos.y + titleBarOffset + containerSize.y - triggerSize.y)
            {
                
                DiscardTopPreview();
                DiscardRightPreview();
                DiscardBottomPreview();
                DrawLeftPreview();
                previewDrawn = true;
                TargetDockSide = DockSide::LEFT;                
            }
            //Top
            else if(inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).x >= containerPos.x + triggerSize.x && 
                    inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).x <= containerPos.x + containerSize.x - triggerSize.x &&
                    inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).y >= containerPos.y + titleBarOffset && 
                    inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).y <= containerPos.y + titleBarOffset + triggerSize.y)
            {
                DiscardLeftPreview();
                DiscardRightPreview();
                DiscardBottomPreview();
                DrawTopPreview();
                previewDrawn = true;
                TargetDockSide = DockSide::TOP;
            }
            //Right
            else if(inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).x >= containerPos.x + containerSize.x - triggerSize.x && 
                    inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).x <= containerPos.x + containerSize.x &&
                    inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).y >= containerPos.y + titleBarOffset + triggerSize.y && 
                    inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).y <= containerPos.y + titleBarOffset + containerSize.y - triggerSize.y)
            {
                DiscardLeftPreview();
                DiscardTopPreview();
                DiscardBottomPreview();
                DrawRightPreview();
                previewDrawn = true;
                TargetDockSide = DockSide::RIGHT;
            }
            //Bottom
            else if(inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).x >= containerPos.x + triggerSize.x && 
                    inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).x <= containerPos.x + containerSize.x - triggerSize.x &&
                    inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).y >= containerPos.y + containerSize.y - triggerSize.y && 
                    inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).y <= containerPos.y + containerSize.y)
            {
                DiscardLeftPreview();
                DiscardTopPreview();
                DiscardRightPreview();
                DrawBottomPreview();  
                previewDrawn = true;
                TargetDockSide = DockSide::BOTTOM;
            }
            else
            {                
                DiscardLeftPreview();
                DiscardTopPreview();
                DiscardRightPreview();
                DiscardBottomPreview();
            }
            
            //Check if the cursor is inside the window
            if(inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).x >= containerPos.x && inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).x <= containerPos.x + containerSize.x &&
                inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).y >= containerPos.y + titleBarOffset && inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).y <= containerPos.y + containerSize.y)
            {
                
                if(!previewDrawn)
                {
                    TargetDockSide = DockSide::NONE;
                    DrawTriggerAreas();
                }
                else                    
                    DiscardTriggerAreas();
                
                    
                globalInputStatus.DockingBlocked = true;
                windowInputStatus.DockingBlocked = true;
                TargetDockObject = Container;
            }
            else
            {
                TargetDockSide = DockSide::NONE;
                DiscardTriggerAreas();
            }
        }
        else
        {   
            DiscardLeftPreview();
            DiscardTopPreview();
            DiscardRightPreview();
            DiscardBottomPreview();
            DiscardTriggerAreas();
        }

        FUNC_DEBUG_EXIT();
    }

    void Dockable::Internal_Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {}
    
    //function: GetExtensionName
    std::string Dockable::GetExtensionName()
    {
        return EXTENSION_NAME;
    }
    
    //function: BindToObject
    void Dockable::BindToObject(ssGUI::GUIObject* bindObj)
    {
        FUNC_DEBUG_ENTRY();
        
        Container = bindObj;

        ssGUI::EventCallbacks::EventCallback* event = nullptr;
        if(Container->IsEventCallbackExist(ssGUI::EventCallbacks::WindowDragStateChangedEventCallback::EVENT_NAME))
            event = Container->GetEventCallback(ssGUI::EventCallbacks::WindowDragStateChangedEventCallback::EVENT_NAME);
        else
        {
            event = new ssGUI::EventCallbacks::WindowDragStateChangedEventCallback();
            Container->AddEventCallback(event);
        }

        if(Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW)
        {
            WindowDragStateChangedEventIndex = event->AddEventListener([&](ssGUI::GUIObject* src)
            {
                //When the current window started being dragged
                if(static_cast<ssGUI::Window*>(src)->GetWindowDragState() == ssGUI::Enums::WindowDragState::STARTED)
                {
                    OnWindowDragStarted();
                }
                //When the current window finished being dragged
                else if(static_cast<ssGUI::Window*>(src)->GetWindowDragState() == ssGUI::Enums::WindowDragState::ENDED)
                {
                    OnWindowDragFinished();
                }
            });
        }

        FUNC_DEBUG_EXIT();
    }

    void Dockable::Copy(ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        
        ssGUI::Extensions::Dockable* dockable = static_cast<ssGUI::Extensions::Dockable*>(extension);

        Enabled = dockable->IsEnabled();
        TopLevelParent = dockable->TopLevelParent;
        CurrentObjectsReferences = dockable->CurrentObjectsReferences;
        UseTriggerPercentage = dockable->IsUseTriggerPercentage();
        TriggerPixel = dockable->GetTriggerPixel();
        TriggerAreaColor = dockable->GetTriggerAreaColor();
        DockPreviewColor = dockable->GetDockPreviewColor();   
    }

    ObjectsReferences* Dockable::Internal_GetObjectsReferences()
    {
        return &CurrentObjectsReferences;
    }

    Extension* Dockable::Clone(ssGUI::GUIObject* newContainer)
    {
        Dockable* temp = new Dockable(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
}


