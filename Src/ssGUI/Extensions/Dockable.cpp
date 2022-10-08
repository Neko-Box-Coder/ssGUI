#include "ssGUI/Extensions/Dockable.hpp"

#include "ssGUI/ssGUITags.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/Extensions/Docker.hpp"
#include "ssGUI/EventCallbacks/WindowDragStateChangedEventCallback.hpp"

namespace ssGUI::Extensions
{
    bool Dockable::GlobalDockMode = false;
    ssGUI::MainWindow* Dockable::MainWindowUnderDocking = nullptr;
    ssGUI::GUIObject* Dockable::DockingTopLevelParent = nullptr;
    ssGUI::GUIObject* Dockable::TargetDockObject = nullptr;
    Dockable::DockSide Dockable::TargetDockSide = Dockable::DockSide::NONE;
    
    Dockable::Dockable() :  Container(nullptr),
                            Enabled(true),
                            TopLevelParent(-1),
                            CurrentObjectsReferences(),
                            UseTriggerPercentage(true),
                            TriggerPercentage(0.25f),
                            TriggerPixel(15),
                            TriggerAreaColor(87, 207, 255, 127),
                            DockPreviewColor(255, 255, 255, 127),
                            OriginalParent(nullptr),
                            ContainerIsDocking(false),
                            DockPreivewTop(nullptr),
                            DockPreivewRight(nullptr),
                            DockPreivewBottom(nullptr),
                            DockPreivewLeft(nullptr),
                            DockTriggerTop(nullptr),
                            DockTriggerRight(nullptr),
                            DockTriggerBottom(nullptr),
                            DockTriggerLeft(nullptr)
    {}
    
    Dockable::~Dockable()
    {
        if(Container != nullptr && Container->IsEventCallbackExist(ssGUI::EventCallbacks::WindowDragStateChangedEventCallback::EVENT_NAME))
        {
            Container->GetEventCallback(ssGUI::EventCallbacks::WindowDragStateChangedEventCallback::EVENT_NAME)->
                RemoveEventListener(EXTENSION_NAME);
            
            if(Container->GetEventCallback(ssGUI::EventCallbacks::WindowDragStateChangedEventCallback::EVENT_NAME)->
                GetEventListenerCount() == 0)
            {
                Container->RemoveEventCallback(ssGUI::EventCallbacks::WindowDragStateChangedEventCallback::EVENT_NAME);
            }
        }

        DiscardTriggerAreas();
        DiscardLeftPreview();
        DiscardRightPreview();
        DiscardTopPreview();
        DiscardBottomPreview();

        CurrentObjectsReferences.CleanUp();
    }

    Dockable::Dockable(Dockable const& other)
    {
        Container = nullptr;
        Enabled = other.IsEnabled();
        TopLevelParent = other.TopLevelParent;
        CurrentObjectsReferences = other.CurrentObjectsReferences;
        UseTriggerPercentage = other.IsUseTriggerPercentage();
        TriggerPercentage = other.GetTriggerPercentage();
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
    }

    void Dockable::ConstructRenderInfo()
    {}

    void Dockable::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {}

    void Dockable::CreateWidgetIfNotPresent(ssGUI::GUIObject** widget, glm::u8vec4 color)
    {
        ssLOG_FUNC_ENTRY();
        
        //If widget is not present, create it
        if((*widget) == nullptr)
        {            
            (*widget) = new ssGUI::Widget();
            (*widget)->SetUserCreated(false);
            (*widget)->SetHeapAllocated(true);
            static_cast<ssGUI::Widget*>((*widget))->SetInteractable(false);
            static_cast<ssGUI::Widget*>((*widget))->SetBlockInput(false);

            auto ap = ssGUI::Factory::Create<ssGUI::Extensions::AdvancedPosition>();
            auto as = ssGUI::Factory::Create<ssGUI::Extensions::AdvancedSize>(); 

            (*widget)->AddExtension(ap);
            (*widget)->AddExtension(as);
            (*widget)->AddTag(ssGUI::Tags::OVERLAY);
            (*widget)->SetBackgroundColor(color);
        }
        
        if((*widget)->GetParent() != Container)
            (*widget)->SetParent(Container);

        ssLOG_FUNC_EXIT();
    }

    
    void Dockable::DrawLeftPreview()
    {
        ssLOG_FUNC_ENTRY();
        
        CreateWidgetIfNotPresent(&DockPreivewLeft, GetDockPreviewColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockPreivewLeft->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* as = static_cast<ssGUI::Extensions::AdvancedSize*>(DockPreivewLeft->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::LEFT);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);

        as->SetVerticalPercentage(1);
        as->SetHorizontalPercentage(0.5);
        
        ssLOG_FUNC_EXIT();
    }

    void Dockable::DrawTopPreview()
    {
        ssLOG_FUNC_ENTRY();
        
        CreateWidgetIfNotPresent(&DockPreivewTop, GetDockPreviewColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockPreivewTop->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* as = static_cast<ssGUI::Extensions::AdvancedSize*>(DockPreivewTop->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::TOP);

        as->SetHorizontalPercentage(1);
        as->SetVerticalPercentage(0.5);
        
        ssLOG_FUNC_EXIT();
    }

    void Dockable::DrawRightPreview()
    {
        ssLOG_FUNC_ENTRY();
        
        CreateWidgetIfNotPresent(&DockPreivewRight, GetDockPreviewColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockPreivewRight->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* as = static_cast<ssGUI::Extensions::AdvancedSize*>(DockPreivewRight->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::RIGHT);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);

        as->SetHorizontalPercentage(0.5);
        as->SetVerticalPercentage(1);

        ssLOG_FUNC_EXIT();
    }

    void Dockable::DrawBottomPreview()
    {
        ssLOG_FUNC_ENTRY();
        
        CreateWidgetIfNotPresent(&DockPreivewBottom, GetDockPreviewColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockPreivewBottom->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* as = static_cast<ssGUI::Extensions::AdvancedSize*>(DockPreivewBottom->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::BOTTOM);

        as->SetHorizontalPercentage(1);
        as->SetVerticalPercentage(0.5);
        
        ssLOG_FUNC_EXIT();
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
        ssLOG_FUNC_ENTRY();
        
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

        if(UseTriggerPercentage)
        {
            apTop->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
            apTop->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::TOP);
            asTop->SetHorizontalPercentage(1 - GetTriggerPercentage() * 2);
            asTop->SetHorizontalPixel(0);
            asTop->SetVerticalPercentage(GetTriggerPercentage());
            asTop->SetVerticalPixel(0);

            apRight->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::RIGHT);
            apRight->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);
            asRight->SetHorizontalPercentage(GetTriggerPercentage());
            asRight->SetHorizontalPixel(0);
            asRight->SetVerticalPercentage(1 - GetTriggerPercentage() * 2);
            asRight->SetVerticalPixel(0);

            apBottom->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
            apBottom->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::BOTTOM);
            asBottom->SetHorizontalPercentage(1 - GetTriggerPercentage() * 2);
            asBottom->SetHorizontalPixel(0);
            asBottom->SetVerticalPercentage(GetTriggerPercentage());
            asBottom->SetVerticalPixel(0);

            apLeft->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::LEFT);
            apLeft->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);
            asLeft->SetHorizontalPercentage(GetTriggerPercentage());
            asLeft->SetHorizontalPixel(0);
            asLeft->SetVerticalPercentage(1 - GetTriggerPercentage() * 2);
            asLeft->SetVerticalPixel(0);
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

        ssLOG_FUNC_EXIT();
    }

    //TODO : Add mutex for multi-threading support
    void Dockable::OnWindowDragStarted()
    {        
        ssLOG_FUNC_ENTRY();
        
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
            ssLOG_FUNC_EXIT();
            return;
        }

        MainWindowUnderDocking = static_cast<ssGUI::MainWindow*>(curParent);

        Container->AddTag(ssGUI::Tags::FLOATING);
        ssGUI::GUIObject* containerParent = Container->GetParent();

        //Docking mechanism
        //Check if container is docked under docker. If so, set the size for the child after container to fill the gap
        if(containerParent->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME) && containerParent->GetChildrenCount() > 1)
        {
            containerParent->StashChildrenIterator();
            containerParent->FindChild(Container);
            if(!containerParent->IsChildrenIteratorLast())
            {
                containerParent->MoveChildrenIteratorNext();
                glm::vec2 childSize = containerParent->GetCurrentChild()->GetSize();
                if(static_cast<ssGUI::Extensions::Layout*>(containerParent->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME))->IsHorizontalLayout())
                    containerParent->GetCurrentChild()->SetSize(glm::vec2(childSize.x + Container->GetSize().x, childSize.y));
                else
                    containerParent->GetCurrentChild()->SetSize(glm::vec2(childSize.x, childSize.y + Container->GetSize().y));                
            }
            containerParent->PopChildrenIterator();
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

        ssLOG_FUNC_EXIT();
    }

    void Dockable::FindDockLayout(ssGUI::Extensions::Layout*& dockLayout)
    {
        ssLOG_FUNC_ENTRY();
        
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

        ssLOG_FUNC_EXIT();
    }

    void Dockable::CreateEmptyParentForDocking(ssGUI::Extensions::Layout*& dockLayout)
    {
        ssLOG_FUNC_ENTRY();
        ssGUI::Window* newParent = nullptr;
        if(ssGUI::Extensions::Docker::GetDefaultGeneratedDockerWindow() == nullptr)
            newParent = new ssGUI::Window();
        else
            newParent = static_cast<ssGUI::Window*>(ssGUI::Extensions::Docker::GetDefaultGeneratedDockerWindow()->Clone(true));

        newParent->SetUserCreated(false);
        newParent->SetHeapAllocated(true);
        newParent->SetSize(TargetDockObject->GetSize());
        newParent->SetAnchorType(TargetDockObject->GetAnchorType());
        newParent->SetPosition(TargetDockObject->GetPosition());
        newParent->SetParent(TargetDockObject->GetParent());

        //The docker will automatically create docker & layout extension if not exist
        if(!newParent->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME))
        {
            newParent->AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Docker>());
            dockLayout = static_cast<ssGUI::Extensions::Layout*>(newParent->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME));
        }

        if(!newParent->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
            newParent->AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Layout>());

        //Check if the generated docker does not use parent docker & layout or not.
        if(newParent->GetParent()->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME) 
            && static_cast<ssGUI::Extensions::Docker*>(newParent->GetParent()->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME))->IsChildrenDockerUseThisSettings()
            && static_cast<ssGUI::Extensions::Docker*>(newParent->GetParent()->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME))->IsEnabled())
        {
            auto parentDocker = static_cast<ssGUI::Extensions::Docker*>(newParent->GetParent()->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME));
            newParent->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME)->Copy(parentDocker);

            if(newParent->GetParent()->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME) 
                && static_cast<ssGUI::Extensions::Docker*>(newParent->GetParent()->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME))->IsEnabled())
            {
                auto parentLayout = static_cast<ssGUI::Extensions::Docker*>(newParent->GetParent()->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME));
                newParent->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME)->Copy(parentLayout);
            }
        }
        
        //Then change the Layout orientation to the same as the docking orientation
        if(TargetDockSide == DockSide::LEFT || TargetDockSide == DockSide::RIGHT)
            dockLayout->SetHorizontalLayout(true);
        else
            dockLayout->SetHorizontalLayout(false);

        //If not floating, turn window to invisible
        if(TargetDockObject->GetParent()->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME) && 
            TargetDockObject->GetParent()->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME)->IsEnabled())
        {
            newParent->SetTitlebar(false);
            auto newBGColor = newParent->GetBackgroundColor();
            newBGColor.a = 0;
            newParent->SetBackgroundColor(newBGColor);
            newParent->SetResizeType(ssGUI::Enums::ResizeType::NONE);

            //Disable all extensions except docker, assuming all extensions are enabled by default (When default is not overriden)
            if(ssGUI::Extensions::Docker::GetDefaultGeneratedDockerWindow() == nullptr)
            {
                auto allExtensions = newParent->GetListOfExtensions();
                for(auto extension : allExtensions)
                {
                    if(extension->GetExtensionName() == ssGUI::Extensions::Docker::EXTENSION_NAME ||
                        extension->GetExtensionName() == ssGUI::Extensions::Layout::EXTENSION_NAME)
                    {
                        continue;
                    }
                    
                    extension->SetEnabled(false);
                }
                // newParent->RemoveExtension(ssGUI::Extensions::Border::EXTENSION_NAME);
            }
            
            //Set all the children to be disabled since it is not floating
            newParent->StashChildrenIterator();
            newParent->MoveChildrenIteratorToFirst();
            while(!newParent->IsChildrenIteratorEnd())
            {
                newParent->GetCurrentChild()->SetEnabled(false);
                newParent->MoveChildrenIteratorNext();
            }
            newParent->PopChildrenIterator();
        }

        //Restore order
        TargetDockObject->GetParent()->StashChildrenIterator();
        TargetDockObject->GetParent()->FindChild(TargetDockObject);
        ssGUI::Hierarchy::ChildToken dockObjectIt = ObjectToDockNextTo->GetParent()->GetCurrentChildToken();
        TargetDockObject->GetParent()->MoveChildrenIteratorToLast();
        ssGUI::Hierarchy::ChildToken lastIt = ObjectToDockNextTo->GetParent()->GetCurrentChildToken();
        
        TargetDockObject->GetParent()->ChangeChildOrderToBeforePosition(lastIt, dockObjectIt);
        TargetDockObject->SetParent(newParent);
        //Setting a new parent from the dock will causes it to revert to original size. 
        //Therefore will need to set the size to match the new parent again.
        TargetDockObject->SetSize(newParent->GetSize());
        TargetDockObject->GetParent()->PopChildrenIterator();
        ssLOG_FUNC_EXIT();
    }

    //TODO: Check top level parent for up coming docking via code feature
    void Dockable::OnWindowDragFinished()
    {                
        ssLOG_FUNC_ENTRY();

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
            TargetDockObject->SetSize(glm::vec2(TargetDockObject->GetSize().x * 0.5, TargetDockObject->GetSize().y * 0.5));
            glm::vec2 newContainerSize = TargetDockObject->GetSize();
            Container->SetParent(TargetDockObject->GetParent());
            Container->SetSize(newContainerSize);

            //Insert the Container after/before it
            TargetDockObject->GetParent()->StashChildrenIterator();
            TargetDockObject->GetParent()->FindChild(TargetDockObject);
            ssGUI::Hierarchy::ChildToken dockObjectIt = ObjectToDockNextTo->GetParent()->GetCurrentChildToken();
            TargetDockObject->GetParent()->MoveChildrenIteratorToLast();
            ssGUI::Hierarchy::ChildToken lastIt = ObjectToDockNextTo->GetParent()->GetCurrentChildToken();
            TargetDockObject->GetParent()->PopChildrenIterator();

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
        //Reset docking variables
        OriginalParent = nullptr;
        ContainerIsDocking = false;
        GlobalDockMode = false;
        MainWindowUnderDocking = nullptr;
        DockingTopLevelParent = nullptr;
        TargetDockObject = nullptr;
        TargetDockSide = DockSide::NONE;

        ssLOG_FUNC_EXIT();
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

    void Dockable::Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, ssGUI::GUIObject* mainWindow)
    {
        ssLOG_FUNC_ENTRY();
        
        if(!isPreUpdate || Container == nullptr || !Enabled)
        {
            ssLOG_FUNC_EXIT();
            return;
        }
        
        //If global dock mode is true, check topLevelParent first, then check the cursor against the trigger area
        if(GlobalDockMode && !ContainerIsDocking && inputStatus.DockingBlockedObject == nullptr)
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
                ssLOG_FUNC_EXIT();
                return;
            }
            
            glm::vec2 containerPos = Container->GetGlobalPosition();
            glm::vec2 containerSize = Container->GetSize();
            int titleBarOffset = Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && Container->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW ? 
                                    static_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() : 0;
            glm::vec2 windowContentSize = Container->GetSize();
            windowContentSize.y -= titleBarOffset;
            
            glm::vec2 triggerSize = IsUseTriggerPercentage() ? glm::vec2(glm::vec2(windowContentSize) * GetTriggerPercentage()) : glm::vec2(GetTriggerPixel(), GetTriggerPixel());

            bool previewDrawn = false;

            //Left
            if(inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).x >= containerPos.x && 
                inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).x <= containerPos.x + triggerSize.x &&
                inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).y >= containerPos.y + titleBarOffset + triggerSize.y && 
                inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).y <= containerPos.y + titleBarOffset + windowContentSize.y - triggerSize.y)
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
                    inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).x <= containerPos.x + windowContentSize.x - triggerSize.x &&
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
            else if(inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).x >= containerPos.x + windowContentSize.x - triggerSize.x && 
                    inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).x <= containerPos.x + windowContentSize.x &&
                    inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).y >= containerPos.y + titleBarOffset + triggerSize.y && 
                    inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).y <= containerPos.y + titleBarOffset + windowContentSize.y - triggerSize.y)
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
                    inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).x <= containerPos.x + windowContentSize.x - triggerSize.x &&
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
            if(inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).x >= containerPos.x && 
                inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).x <= containerPos.x + windowContentSize.x &&
                inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).y >= containerPos.y + titleBarOffset && 
                inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).y <= containerPos.y + containerSize.y)
            {
                if(!previewDrawn)
                {
                    TargetDockSide = DockSide::NONE;
                    DrawTriggerAreas();
                }
                else                    
                    DiscardTriggerAreas();
                
                    
                inputStatus.DockingBlockedObject = Container;
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

        ssLOG_FUNC_EXIT();
    }

    void Dockable::Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {}
    
    std::string Dockable::GetExtensionName()
    {
        return EXTENSION_NAME;
    }
    
    void Dockable::BindToObject(ssGUI::GUIObject* bindObj)
    {
        ssLOG_FUNC_ENTRY();
        
        Container = bindObj;

        ssGUI::EventCallbacks::EventCallback* event = nullptr;
        if(Container->IsEventCallbackExist(ssGUI::EventCallbacks::WindowDragStateChangedEventCallback::EVENT_NAME))
            event = Container->GetEventCallback(ssGUI::EventCallbacks::WindowDragStateChangedEventCallback::EVENT_NAME);
        else
        {
            event = ssGUI::Factory::Create<ssGUI::EventCallbacks::WindowDragStateChangedEventCallback>();
            Container->AddEventCallback(event);
        }

        if(Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW)
        {
            event->AddEventListener
            (
                EXTENSION_NAME,
                [](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* refs)
                {
                    if(!container->IsExtensionExist(ssGUI::Extensions::Dockable::EXTENSION_NAME))
                    {
                        ssLOG_LINE("Failed to find Dockable extension. Probably something wrong with cloning");
                        ssLOG_EXIT_PROGRAM();
                        return;
                    }

                    ssGUI::Extensions::Dockable* containerDockable = static_cast<ssGUI::Extensions::Dockable*>
                            (container->GetExtension(ssGUI::Extensions::Dockable::EXTENSION_NAME));
                    
                    //When the current window started being dragged
                    if(static_cast<ssGUI::Window*>(src)->GetWindowDragState() == ssGUI::Enums::WindowDragState::STARTED)
                    {
                        containerDockable->OnWindowDragStarted();
                    }
                    //When the current window finished being dragged
                    else if(static_cast<ssGUI::Window*>(src)->GetWindowDragState() == ssGUI::Enums::WindowDragState::ENDED)
                    {
                        containerDockable->OnWindowDragFinished();
                    }
                }
            );
        }

        ssLOG_FUNC_EXIT();
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

    Dockable* Dockable::Clone(ssGUI::GUIObject* newContainer)
    {
        Dockable* temp = new Dockable(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
}


