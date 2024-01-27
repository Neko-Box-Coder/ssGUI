#include "ssGUI/Extensions/Legacy/Dockable.hpp"

#include "ssGUI/ssGUITags.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/Extensions/Legacy/Docker.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Extensions
{
    bool Dockable::GlobalDockMode = false;
    ssGUI::MainWindow* Dockable::MainWindowUnderDocking = nullptr;
    ssGUI::GUIObject* Dockable::DockingTopLevelParent = nullptr;
    ssGUI::GUIObject* Dockable::ObjectToDockNextTo = nullptr;
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
        if(Container != nullptr && Container->IsEventCallbackExist(ssGUI::Enums::EventType::WINDOW_DRAG_STATE_CHANGED))
        {
            Container->GetEventCallback(ssGUI::Enums::EventType::WINDOW_DRAG_STATE_CHANGED)->
                RemoveEventListener(EXTENSION_NAME);
            
            if(Container->GetEventCallback(ssGUI::Enums::EventType::WINDOW_DRAG_STATE_CHANGED)->
                GetEventListenerCount() == 0)
            {
                Container->RemoveEventCallback(ssGUI::Enums::EventType::WINDOW_DRAG_STATE_CHANGED);
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
        Copy(&other);

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

    void Dockable::ConstructRenderInfo(ssGUI::Backend::DrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {}

    void Dockable::CreateWidgetIfNotPresent(ssGUI::GUIObject** widget, glm::u8vec4 color)
    {
        ssGUI_LOG_FUNC();
        
        //If widget is not present, create it
        if((*widget) == nullptr)
        {            
            (*widget) = new ssGUI::Widget();
            (*widget)->SetUserCreated(false);
            (*widget)->SetHeapAllocated(true);
            static_cast<ssGUI::Widget*>((*widget))->SetInteractable(false);
            static_cast<ssGUI::Widget*>((*widget))->SetBlockInput(false);

            (*widget)->AddExtension<ssGUI::Extensions::AdvancedPosition>();
            (*widget)->AddExtension<ssGUI::Extensions::AdvancedSize>();
            (*widget)->AddTag(ssGUI::Tags::FLOATING);
            (*widget)->SetBackgroundColor(color);
        }
        
        if((*widget)->GetParent() != Container)
            (*widget)->SetParent(Container);
    }

    
    void Dockable::DrawLeftPreview()
    {
        ssGUI_LOG_FUNC();
        
        CreateWidgetIfNotPresent(&DockPreivewLeft, GetDockPreviewColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* ap = DockPreivewLeft->GetExtension<ssGUI::Extensions::AdvancedPosition>();
        ssGUI::Extensions::AdvancedSize* as = DockPreivewLeft->GetExtension<ssGUI::Extensions::AdvancedSize>();

        ap->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::LEFT);
        ap->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::CENTER);

        as->SetVerticalPercentage(1);
        as->SetHorizontalPercentage(0.5);
    }

    void Dockable::DrawTopPreview()
    {
        ssGUI_LOG_FUNC();
        
        CreateWidgetIfNotPresent(&DockPreivewTop, GetDockPreviewColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* ap = DockPreivewTop->GetExtension<ssGUI::Extensions::AdvancedPosition>();
        ssGUI::Extensions::AdvancedSize* as = DockPreivewTop->GetExtension<ssGUI::Extensions::AdvancedSize>();

        ap->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
        ap->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::TOP);

        as->SetHorizontalPercentage(1);
        as->SetVerticalPercentage(0.5);
    }

    void Dockable::DrawRightPreview()
    {
        ssGUI_LOG_FUNC();
        
        CreateWidgetIfNotPresent(&DockPreivewRight, GetDockPreviewColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* ap = DockPreivewRight->GetExtension<ssGUI::Extensions::AdvancedPosition>();
        ssGUI::Extensions::AdvancedSize* as = DockPreivewRight->GetExtension<ssGUI::Extensions::AdvancedSize>();

        ap->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::RIGHT);
        ap->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::CENTER);

        as->SetHorizontalPercentage(0.5);
        as->SetVerticalPercentage(1);
    }

    void Dockable::DrawBottomPreview()
    {
        ssGUI_LOG_FUNC();
        
        CreateWidgetIfNotPresent(&DockPreivewBottom, GetDockPreviewColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* ap = DockPreivewBottom->GetExtension<ssGUI::Extensions::AdvancedPosition>();
        ssGUI::Extensions::AdvancedSize* as = DockPreivewBottom->GetExtension<ssGUI::Extensions::AdvancedSize>();

        ap->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
        ap->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::BOTTOM);

        as->SetHorizontalPercentage(1);
        as->SetVerticalPercentage(0.5);
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
        ssGUI_LOG_FUNC();
        
        CreateWidgetIfNotPresent(&DockTriggerTop, GetTriggerAreaColor());
        CreateWidgetIfNotPresent(&DockTriggerRight, GetTriggerAreaColor());
        CreateWidgetIfNotPresent(&DockTriggerBottom, GetTriggerAreaColor());
        CreateWidgetIfNotPresent(&DockTriggerLeft, GetTriggerAreaColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* apTop = DockTriggerTop->GetExtension<ssGUI::Extensions::AdvancedPosition>();
        ssGUI::Extensions::AdvancedSize* asTop = DockTriggerTop->GetExtension<ssGUI::Extensions::AdvancedSize>();

        ssGUI::Extensions::AdvancedPosition* apRight = DockTriggerRight->GetExtension<ssGUI::Extensions::AdvancedPosition>();
        ssGUI::Extensions::AdvancedSize* asRight = DockTriggerRight->GetExtension<ssGUI::Extensions::AdvancedSize>();

        ssGUI::Extensions::AdvancedPosition* apBottom = DockTriggerBottom->GetExtension<ssGUI::Extensions::AdvancedPosition>();
        ssGUI::Extensions::AdvancedSize* asBottom = DockTriggerBottom->GetExtension<ssGUI::Extensions::AdvancedSize>();

        ssGUI::Extensions::AdvancedPosition* apLeft = DockTriggerLeft->GetExtension<ssGUI::Extensions::AdvancedPosition>();
        ssGUI::Extensions::AdvancedSize* asLeft = DockTriggerLeft->GetExtension<ssGUI::Extensions::AdvancedSize>();

        if(UseTriggerPercentage)
        {
            apTop->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
            apTop->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::TOP);
            asTop->SetHorizontalPercentage(1 - GetTriggerPercentage() * 2);
            asTop->SetHorizontalPixel(0);
            asTop->SetVerticalPercentage(GetTriggerPercentage());
            asTop->SetVerticalPixel(0);

            apRight->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::RIGHT);
            apRight->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::CENTER);
            asRight->SetHorizontalPercentage(GetTriggerPercentage());
            asRight->SetHorizontalPixel(0);
            asRight->SetVerticalPercentage(1 - GetTriggerPercentage() * 2);
            asRight->SetVerticalPixel(0);

            apBottom->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
            apBottom->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::BOTTOM);
            asBottom->SetHorizontalPercentage(1 - GetTriggerPercentage() * 2);
            asBottom->SetHorizontalPixel(0);
            asBottom->SetVerticalPercentage(GetTriggerPercentage());
            asBottom->SetVerticalPixel(0);

            apLeft->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::LEFT);
            apLeft->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::CENTER);
            asLeft->SetHorizontalPercentage(GetTriggerPercentage());
            asLeft->SetHorizontalPixel(0);
            asLeft->SetVerticalPercentage(1 - GetTriggerPercentage() * 2);
            asLeft->SetVerticalPixel(0);
        }
        else
        {
            apTop->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
            apTop->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::TOP);
            asTop->SetHorizontalPixel(Container->GetSize().x - GetTriggerPixel() * 2);
            asTop->SetVerticalPixel(GetTriggerPixel());

            apRight->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::RIGHT);
            apRight->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::CENTER);
            asRight->SetHorizontalPixel(GetTriggerPixel());
            asRight->SetVerticalPixel(Container->GetSize().y - GetTriggerPixel() * 2);

            apBottom->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
            apBottom->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::BOTTOM);
            asBottom->SetHorizontalPixel(Container->GetSize().x - GetTriggerPixel() * 2);
            asBottom->SetVerticalPixel(GetTriggerPercentage());

            apLeft->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::LEFT);
            apLeft->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::CENTER);
            asLeft->SetHorizontalPixel(GetTriggerPixel());
            asLeft->SetVerticalPixel(Container->GetSize().y - GetTriggerPixel() * 2);
        }
    }

    //TODO : Add mutex for multi-threading support
    void Dockable::OnWindowDragStarted()
    {        
        ssGUI_LOG_FUNC();
        
        ContainerIsDocking = true;
        GlobalDockMode = true;

        //Find the Main Window
        ssGUI::GUIObject* curParent = Container;
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
            return;
        }

        MainWindowUnderDocking = static_cast<ssGUI::MainWindow*>(curParent);

        Container->AddTag(ssGUI::Tags::OVERLAY);
        ssGUI::GUIObject* containerParent = Container->GetParent();

        //Docking mechanism
        //Check if container is docked under docker. If so, set the size for the child after container to fill the gap
        // if(containerParent->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME) && containerParent->GetChildrenCount() > 1)
        // {
        //     containerParent->StashChildrenIterator();
        //     containerParent->MoveChildrenIteratorToChild(Container);
        //     if(!containerParent->IsChildrenIteratorLast())
        //     {
        //         containerParent->MoveChildrenIteratorNext();
        //         glm::vec2 childSize = containerParent->GetCurrentChild()->GetSize();
        //         if(containerParent->GetExtension<ssGUI::Extensions::Layout>()->IsHorizontalLayout())
        //             containerParent->GetCurrentChild()->SetSize(glm::vec2(childSize.x + Container->GetSize().x, childSize.y));
        //         else
        //             containerParent->GetCurrentChild()->SetSize(glm::vec2(childSize.x, childSize.y + Container->GetSize().y));                
        //     }
        //     containerParent->PopChildrenIterator();
        // }

        //Parent the container to the MainWindow.
        OriginalParent = Container->GetParent();

        if(GetTopLevelParent() == nullptr)
            DockingTopLevelParent = MainWindowUnderDocking;
        else
            DockingTopLevelParent = CurrentObjectsReferences.GetObjectReference(TopLevelParent);
    }

    void Dockable::FindDockLayout(ssGUI::Extensions::Layout*& dockLayout)
    {
        ssGUI_LOG_FUNC();
        
        ssGUI::Extensions::Layout* parentLayout = ObjectToDockNextTo->GetParent()->GetExtension<ssGUI::Extensions::Layout>();

        if(ObjectToDockNextTo->GetParent()->GetChildrenCount() > 1)
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
    }

    void Dockable::CreateEmptyParentForDocking(ssGUI::Extensions::Layout*& dockLayout)
    {
        ssGUI_LOG_FUNC();
        ssGUI::Window* newParent = nullptr;
        
        //Validation check
        if(ObjectToDockNextTo->GetParent() == nullptr)
            ssLOG_EXIT_PROGRAM();

        //If not floating, turn window to invisible
        if( ObjectToDockNextTo->GetParent()->IsExtensionExist<ssGUI::Extensions::Docker>() && 
            ObjectToDockNextTo->GetParent()->GetExtension<ssGUI::Extensions::Docker>()->IsEnabled())
        {
            newParent = new ssGUI::Window();
            newParent->SetTitlebar(false);
            auto newBGColor = newParent->GetBackgroundColor();
            newBGColor.a = 0;
            newParent->SetBackgroundColor(newBGColor);
            newParent->SetResizeType(ssGUI::Enums::ResizeType::NONE);

            //Disable all extensions except docker, assuming all extensions are enabled by default (When default is not overriden)
            auto allExtensions = newParent->GetListOfExtensions();
            for(auto extension : allExtensions)
            {
                if( extension->GetExtensionName() == ssGUI::Extensions::Docker::EXTENSION_NAME ||
                    extension->GetExtensionName() == ssGUI::Extensions::Layout::EXTENSION_NAME)
                {
                    continue;
                }
                
                extension->SetEnabled(false);
            }
        }
        else if(ssGUI::Extensions::Docker::GetDefaultGeneratedFloatingDockerWindow() == nullptr)
            newParent = new ssGUI::Window();
        else
            newParent = static_cast<ssGUI::Window*>(ssGUI::Extensions::Docker::GetDefaultGeneratedFloatingDockerWindow()->Clone(true));

        newParent->SetUserCreated(false);
        newParent->SetHeapAllocated(true);
        newParent->SetSize(ObjectToDockNextTo->GetSize());
        newParent->SetAnchorType(ObjectToDockNextTo->GetAnchorType());
        newParent->SetPosition(ObjectToDockNextTo->GetPosition());
        newParent->SetParent(ObjectToDockNextTo->GetParent());

        //The docker will automatically create docker & layout extension if not exist
        if(!newParent->IsExtensionExist<ssGUI::Extensions::Docker>())
        {
            newParent->AddExtension<ssGUI::Extensions::Docker>();
            dockLayout = newParent->GetExtension<ssGUI::Extensions::Layout>();
        }

        if(!newParent->IsExtensionExist<ssGUI::Extensions::Layout>())
            newParent->AddExtension<ssGUI::Extensions::Layout>();

        //Check if the generated docker does not use parent docker & layout or not.
        if( newParent->GetParent()->IsExtensionExist<ssGUI::Extensions::Docker>() && 
            newParent->GetParent()->GetExtension<ssGUI::Extensions::Docker>()->IsChildrenDockerUseThisSettings() && 
            newParent->GetParent()->GetExtension<ssGUI::Extensions::Docker>()->IsEnabled())
        {
            auto parentDocker = newParent->GetParent()->GetExtension<ssGUI::Extensions::Docker>();
            newParent->GetExtension<ssGUI::Extensions::Docker>()->Copy(parentDocker);

            if( newParent->GetParent()->IsExtensionExist<ssGUI::Extensions::Layout>() && 
                newParent->GetParent()->GetExtension<ssGUI::Extensions::Layout>()->IsEnabled())
            {
                auto parentLayout = newParent->GetParent()->GetExtension<ssGUI::Extensions::Layout>();
                newParent->GetExtension<ssGUI::Extensions::Layout>()->Copy(parentLayout);
            }
        }
        
        //Then change the Layout orientation to the same as the docking orientation
        if(TargetDockSide == DockSide::LEFT || TargetDockSide == DockSide::RIGHT)
            dockLayout->SetHorizontalLayout(true);
        else
            dockLayout->SetHorizontalLayout(false);

        //Restore order
        ObjectToDockNextTo->GetParent()->StashChildrenIterator();
        ObjectToDockNextTo->GetParent()->MoveChildrenIteratorToChild(ObjectToDockNextTo);
        ssGUI::Hierarchy::ChildToken dockObjectIt = ObjectToDockNextTo->GetParent()->GetCurrentChildToken();
        ObjectToDockNextTo->GetParent()->MoveChildrenIteratorToLast();
        ssGUI::Hierarchy::ChildToken lastIt = ObjectToDockNextTo->GetParent()->GetCurrentChildToken();
        
        ObjectToDockNextTo->GetParent()->ChangeChildOrderToBeforePosition(lastIt, dockObjectIt);
        ObjectToDockNextTo->SetParent(newParent);
        
        //Setting a new parent from the dock will causes it to revert to original size. 
        //Therefore will need to set the size to match the new parent again.
        ObjectToDockNextTo->SetSize(newParent->GetSize());
        ObjectToDockNextTo->GetParent()->PopChildrenIterator();
    }

    //TODO: Check top level parent for up coming docking via code feature
    void Dockable::OnWindowDragFinished()
    {                
        ssGUI_LOG_FUNC();

        //Remove the floating tag to allow docking
        Container->RemoveTag(ssGUI::Tags::OVERLAY);
        Container->RedrawObject();

        //Check if we can dock or not.
        bool validDocking = false;
        if(ObjectToDockNextTo != nullptr)
        {
            if(ObjectToDockNextTo->IsExtensionExist<ssGUI::Extensions::Docker>())
                validDocking = ObjectToDockNextTo->GetExtension<ssGUI::Extensions::Docker>()->IsValidDocking();
            else if(ObjectToDockNextTo->IsExtensionExist<ssGUI::Extensions::Dockable>())
                validDocking = ObjectToDockNextTo->GetExtension<ssGUI::Extensions::Dockable>()->IsValidDocking();
        }

        //Docking mechanism for dockable window
        //Dock when everything is valid
        if( ObjectToDockNextTo != nullptr && ObjectToDockNextTo->GetParent() != nullptr && validDocking)
        {
            //If it is just docker, just need to add it as a child
            if(ObjectToDockNextTo->IsExtensionExist<ssGUI::Extensions::Docker>())
            {
                Container->SetParent(ObjectToDockNextTo);
                goto reset;
            }

            //Otherwise dock as normal
            ssGUI::Extensions::Layout* dockLayout = nullptr;
            
            //Check if the parent of the ObjectToDockNextTo has the Docker extension
            if( ObjectToDockNextTo->GetParent()->IsExtensionExist<ssGUI::Extensions::Layout>() && 
                ObjectToDockNextTo->GetParent()->IsExtensionExist<ssGUI::Extensions::Docker>())
            {
                FindDockLayout(dockLayout);
            }

            //Create an empty parent and add Layout extension if dock layout isn't found
            if(dockLayout == nullptr)
                CreateEmptyParentForDocking(dockLayout);
            
            //This inserts the container to the end. Halfing the size for ObjectToDockNextTo and Container so they fit the original space
            ObjectToDockNextTo->SetSize(glm::vec2(ObjectToDockNextTo->GetSize().x * 0.5, ObjectToDockNextTo->GetSize().y * 0.5));
            glm::vec2 newContainerSize = ObjectToDockNextTo->GetSize();
            Container->SetParent(ObjectToDockNextTo->GetParent());
            Container->SetSize(newContainerSize);

            //Insert the Container after/before it
            ObjectToDockNextTo->GetParent()->StashChildrenIterator();
            ObjectToDockNextTo->GetParent()->MoveChildrenIteratorToChild(ObjectToDockNextTo);
            ssGUI::Hierarchy::ChildToken dockObjectIt = ObjectToDockNextTo->GetParent()->GetCurrentChildToken();
            ObjectToDockNextTo->GetParent()->MoveChildrenIteratorToLast();
            ssGUI::Hierarchy::ChildToken lastIt = ObjectToDockNextTo->GetParent()->GetCurrentChildToken();
            ObjectToDockNextTo->GetParent()->PopChildrenIterator();

            if(!dockLayout->IsReverseOrder())
            {                
                //Before
                if(TargetDockSide == DockSide::LEFT || TargetDockSide == DockSide::TOP)
                    ObjectToDockNextTo->GetParent()->ChangeChildOrderToBeforePosition(lastIt, dockObjectIt);
                //After
                else
                    ObjectToDockNextTo->GetParent()->ChangeChildOrderToAfterPosition(lastIt, dockObjectIt);
            }
            else
            {
                //Before
                if(TargetDockSide == DockSide::RIGHT || TargetDockSide == DockSide::BOTTOM)
                    ObjectToDockNextTo->GetParent()->ChangeChildOrderToBeforePosition(lastIt, dockObjectIt);
                //After
                else
                    ObjectToDockNextTo->GetParent()->ChangeChildOrderToAfterPosition(lastIt, dockObjectIt);
            }
        }
        //If we are not docking, set the parent of the window to top level parent if floating
        else if(Floatable && OriginalParent->IsExtensionExist<ssGUI::Extensions::Docker>())
        {            
            auto globalPos = Container->GetGlobalPosition();
            Container->SetParent(DockingTopLevelParent);
            Container->SetGlobalPosition(globalPos);
        }
        
        reset:            
        //Reset docking variables
        OriginalParent = nullptr;
        ContainerIsDocking = false;
        GlobalDockMode = false;
        MainWindowUnderDocking = nullptr;
        DockingTopLevelParent = nullptr;
        ObjectToDockNextTo = nullptr;
        TargetDockSide = DockSide::NONE;
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
        ssGUI_LOG_FUNC();
        
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
        if(TopLevelParent != -1 && CurrentObjectsReferences.GetObjectReference(TopLevelParent) == nullptr)
        {
            ssGUI_WARNING(ssGUI_TAG_EXT, "Invalid TopLevelParent detected, probably something wrong internally or bug happened");
            return nullptr;
        }
        else if(TopLevelParent == -1)
            return nullptr;
        else
            return CurrentObjectsReferences.GetObjectReference(TopLevelParent);
    }   

    bool Dockable::IsValidDocking() const
    {
        return ValidDocking;
    }

    bool Dockable::IsCurrentlyDocking() const
    {
        return ContainerIsDocking;
    }

    void Dockable::SetFloatable(bool floatable)
    {
        Floatable = floatable;
    }

    bool Dockable::IsFloatable() const
    {
        return Floatable;
    }

    void Dockable::SetEnabled(bool enabled)
    {
        Enabled = enabled;
    }

    bool Dockable::IsEnabled() const
    {
        return Enabled;
    }

    void Dockable::Internal_Update( bool isPreUpdate, 
                                    ssGUI::Backend::SystemInputInterface* inputInterface, 
                                    ssGUI::InputStatus& currentInputStatus, 
                                    ssGUI::InputStatus& lastInputStatus, 
                                    ssGUI::GUIObject* mainWindow)
    {
        ssGUI_LOG_FUNC();
        
        if(!isPreUpdate || Container == nullptr || !Enabled)
            return;
        
        //If global dock mode is true, check topLevelParent first, then check the cursor against the trigger area
        if(GlobalDockMode && !ContainerIsDocking && currentInputStatus.LegacyDockingBlockedObject == nullptr)
        {
            ssGUI::GUIObject* curParent = Container;
            ssGUI::GUIObject* topLevel = GetTopLevelParent();
            bool foundCurTopLevel = false;
            bool curTopLevelUnderDockingTopLevel = false;

            //Find out if the top level parent for the object to be docked is deeping than the this object's top parent
            //If so, we can proceed to dock
            do
            {
                curParent = curParent->GetParent();

                if(curParent == nullptr)
                    break;

                if(curParent == topLevel)
                    foundCurTopLevel = true;
                
                if(curParent == DockingTopLevelParent)
                    curTopLevelUnderDockingTopLevel = foundCurTopLevel;
            }
            while (curParent != nullptr && curParent->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW);


            ssGUI::GUIObject* mainWindow = curParent != nullptr && curParent->GetType() == ssGUI::Enums::GUIObjectType::MAIN_WINDOW ?
                                            curParent :
                                            nullptr;

            //This is not supposed to happen
            if(mainWindow == nullptr)
            {
                ssGUI_WARNING(ssGUI_TAG_EXT, "what?");
                return;
            }

            if(topLevel == nullptr && mainWindow == DockingTopLevelParent)
                curTopLevelUnderDockingTopLevel = true;

            //If this is not the same parent as the one which is being docked, exit
            if(!curTopLevelUnderDockingTopLevel)
            {
                DiscardLeftPreview();
                DiscardTopPreview();
                DiscardRightPreview();
                DiscardBottomPreview();
                DiscardTriggerAreas();
                ValidDocking = false;
                return;
            }
            
            glm::vec2 containerPos = Container->GetGlobalPosition();
            glm::vec2 containerSize = Container->GetSize();
            int titleBarOffset = Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && Container->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW ? 
                                    static_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() : 0;
            glm::vec2 windowContentSize = Container->GetSize();
            windowContentSize.y -= titleBarOffset;
            
            glm::vec2 triggerSize = IsUseTriggerPercentage() ? glm::vec2(glm::vec2(windowContentSize) * GetTriggerPercentage()) : glm::vec2(GetTriggerPixel(), GetTriggerPixel());

            //bool previewDrawn = false;

            //Check if the cursor is inside the window
            ssGUI::Backend::MainWindowInterface* mainWindowInterface = dynamic_cast<ssGUI::MainWindow*>(mainWindow)->GetBackendWindowInterface();
            bool mouseInsideWindow =    inputInterface->GetCurrentMousePosition(mainWindowInterface).x >= containerPos.x && 
                                        inputInterface->GetCurrentMousePosition(mainWindowInterface).x <= containerPos.x + windowContentSize.x &&
                                        inputInterface->GetCurrentMousePosition(mainWindowInterface).y >= containerPos.y + titleBarOffset && 
                                        inputInterface->GetCurrentMousePosition(mainWindowInterface).y <= containerPos.y + containerSize.y;
            
            if(!mouseInsideWindow)
            {
                DiscardLeftPreview();
                DiscardTopPreview();
                DiscardRightPreview();
                DiscardBottomPreview();
                DiscardTriggerAreas();
                ValidDocking = false;
                return;
            }

            //Left
            if( inputInterface->GetCurrentMousePosition(mainWindowInterface).x >= containerPos.x && 
                inputInterface->GetCurrentMousePosition(mainWindowInterface).x <= containerPos.x + triggerSize.x &&
                inputInterface->GetCurrentMousePosition(mainWindowInterface).y >= containerPos.y + titleBarOffset + triggerSize.y && 
                inputInterface->GetCurrentMousePosition(mainWindowInterface).y <= containerPos.y + titleBarOffset + windowContentSize.y - triggerSize.y)
            {
                
                DiscardTopPreview();
                DiscardRightPreview();
                DiscardBottomPreview();
                DrawLeftPreview();
                DiscardTriggerAreas();
                //previewDrawn = true;
                TargetDockSide = DockSide::LEFT;
                ValidDocking = true;
            }
            //Top
            else if(inputInterface->GetCurrentMousePosition(mainWindowInterface).x >= containerPos.x + triggerSize.x && 
                    inputInterface->GetCurrentMousePosition(mainWindowInterface).x <= containerPos.x + windowContentSize.x - triggerSize.x &&
                    inputInterface->GetCurrentMousePosition(mainWindowInterface).y >= containerPos.y + titleBarOffset && 
                    inputInterface->GetCurrentMousePosition(mainWindowInterface).y <= containerPos.y + titleBarOffset + triggerSize.y)
            {
                DiscardLeftPreview();
                DiscardRightPreview();
                DiscardBottomPreview();
                DrawTopPreview();
                DiscardTriggerAreas();
                //previewDrawn = true;
                TargetDockSide = DockSide::TOP;
                ValidDocking = true;

            }
            //Right
            else if(inputInterface->GetCurrentMousePosition(mainWindowInterface).x >= containerPos.x + windowContentSize.x - triggerSize.x && 
                    inputInterface->GetCurrentMousePosition(mainWindowInterface).x <= containerPos.x + windowContentSize.x &&
                    inputInterface->GetCurrentMousePosition(mainWindowInterface).y >= containerPos.y + titleBarOffset + triggerSize.y && 
                    inputInterface->GetCurrentMousePosition(mainWindowInterface).y <= containerPos.y + titleBarOffset + windowContentSize.y - triggerSize.y)
            {
                DiscardLeftPreview();
                DiscardTopPreview();
                DiscardBottomPreview();
                DrawRightPreview();
                DiscardTriggerAreas();
                //previewDrawn = true;
                TargetDockSide = DockSide::RIGHT;
                ValidDocking = true;
            }
            //Bottom
            else if(inputInterface->GetCurrentMousePosition(mainWindowInterface).x >= containerPos.x + triggerSize.x && 
                    inputInterface->GetCurrentMousePosition(mainWindowInterface).x <= containerPos.x + windowContentSize.x - triggerSize.x &&
                    inputInterface->GetCurrentMousePosition(mainWindowInterface).y >= containerPos.y + containerSize.y - triggerSize.y && 
                    inputInterface->GetCurrentMousePosition(mainWindowInterface).y <= containerPos.y + containerSize.y)
            {
                DiscardLeftPreview();
                DiscardTopPreview();
                DiscardRightPreview();
                DrawBottomPreview();  
                DiscardTriggerAreas();
                //previewDrawn = true;
                TargetDockSide = DockSide::BOTTOM;
                ValidDocking = true;
            }
            else
            {                
                DiscardLeftPreview();
                DiscardTopPreview();
                DiscardRightPreview();
                DiscardBottomPreview();
                TargetDockSide = DockSide::NONE;
                ValidDocking = false;
                DrawTriggerAreas();
            }
            
            currentInputStatus.LegacyDockingBlockedObject = Container;
            ObjectToDockNextTo = Container;
        }
        else
        {   
            DiscardLeftPreview();
            DiscardTopPreview();
            DiscardRightPreview();
            DiscardBottomPreview();
            DiscardTriggerAreas();
            ValidDocking = false;
        }
    }

    void Dockable::Internal_Draw(bool isPreRender, ssGUI::Backend::DrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {}
    
    std::string Dockable::GetExtensionName() const
    {
        return EXTENSION_NAME;
    }
    
    void Dockable::BindToObject(ssGUI::GUIObject* bindObj)
    {
        ssGUI_LOG_FUNC();
        
        Container = bindObj;

        ssGUI::EventCallback* event = nullptr;
        if(Container->IsEventCallbackExist(ssGUI::Enums::EventType::WINDOW_DRAG_STATE_CHANGED))
            event = Container->GetEventCallback(ssGUI::Enums::EventType::WINDOW_DRAG_STATE_CHANGED);
        else
        {
            event = Container->AddEventCallback(ssGUI::Enums::EventType::WINDOW_DRAG_STATE_CHANGED);
        }

        if(Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW)
        {
            event->AddEventListener
            (
                EXTENSION_NAME,
                [](ssGUI::EventInfo& info)
                {
                    if(!info.Container->IsExtensionExist<ssGUI::Extensions::Dockable>())
                    {
                        ssGUI_ERROR(ssGUI_TAG_EXT, "Failed to find Dockable extension. Probably something wrong with cloning");
                        ssLOG_EXIT_PROGRAM();
                        return;
                    }

                    ssGUI::Extensions::Dockable* containerDockable = info.Container->GetExtension<ssGUI::Extensions::Dockable>();
                    
                    //When the current window started being dragged
                    if(static_cast<ssGUI::Window*>(info.EventSource)->GetWindowDragState() == ssGUI::Enums::WindowDragState::STARTED)
                    {
                        containerDockable->OnWindowDragStarted();
                    }
                    //When the current window finished being dragged
                    else if(static_cast<ssGUI::Window*>(info.EventSource)->GetWindowDragState() == ssGUI::Enums::WindowDragState::ENDED)
                    {
                        containerDockable->OnWindowDragFinished();
                    }
                }
            );
        }
    }

    void Dockable::Copy(const ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        
        auto* dockable = static_cast<const ssGUI::Extensions::Dockable*>(extension);

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

    Dockable* Dockable::Clone()
    {
        Dockable* temp = new Dockable(*this);
        return temp;
    }
}

}
