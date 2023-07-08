#include "ssGUI/Extensions/Docker.hpp"


#include "ssGUI/Extensions/Dockable.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/Layout.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Extensions
{
    ssGUI::Window* Docker::DefaultGeneratedDockerWindow = nullptr;

    Docker::Docker() :  Container(nullptr),
                        Enabled(true),
                        ChildrenDockerUseThisSettings(true),
                        UseTriggerPercentage(true),
                        TriggerHorizontalPercentage(0.5),
                        TriggerVerticalPercentage(0.5),
                        TriggerHorizontalPixel(15),
                        TriggerVerticalPixel(15),
                        TriggerAreaColor(87, 207, 255, 127),
                        DockPreviewColor(255, 255, 255, 127),
                        DockPreivew(nullptr),
                        DockTrigger(nullptr),
                        ChildRemoveGuard(false)
    {}

    Docker::~Docker()
    {
        //Cleanup dock preview and trigger if exist
        DiscardPreview();
        DiscardTriggerArea();
        
        //Remove EventCallback
        if(Container != nullptr && Container->IsEventCallbackExist(ssGUI::Enums::EventType::CHILD_REMOVED))
        {
            auto ecb = Container->GetEventCallback(ssGUI::Enums::EventType::CHILD_REMOVED);
            ecb->RemoveEventListener(EXTENSION_NAME);
            if(ecb->GetEventListenerCount() == 0)
                Container->RemoveEventCallback(ssGUI::Enums::EventType::CHILD_REMOVED);
        }
    }

    Docker::Docker(Docker const& other)
    {
        Container = nullptr;
        Enabled = other.IsEnabled();
        ChildrenDockerUseThisSettings = other.IsChildrenDockerUseThisSettings();

        UseTriggerPercentage = other.IsUseTriggerPercentage();
        TriggerHorizontalPercentage = other.GetTriggerHorizontalPercentage();
        TriggerVerticalPercentage = other.GetTriggerVerticalPercentage();
        TriggerHorizontalPixel = other.GetTriggerHorizontalPixel();
        TriggerVerticalPixel = other.GetTriggerVerticalPixel();
        TriggerAreaColor = other.GetTriggerAreaColor();
        DockPreviewColor = other.GetDockPreviewColor();

        DockPreivew = nullptr;
        DockTrigger = nullptr;

        ChildRemoveGuard = false;
    }

    void Docker::ConstructRenderInfo()
    {}

    void Docker::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {}

    void Docker::CreateWidgetIfNotPresent(ssGUI::GUIObject** widget, glm::u8vec4 color)
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

            (*widget)->AddExtension<ssGUI::Extensions::AdvancedPosition>();
            (*widget)->AddExtension<ssGUI::Extensions::AdvancedSize>();
            (*widget)->AddTag(ssGUI::Tags::FLOATING);
            (*widget)->SetBackgroundColor(color);
        }

        if((*widget)->GetParent() != Container)
            (*widget)->SetParent(Container);
        
        ssLOG_FUNC_EXIT();
    }

    void Docker::DrawPreview()
    {
        ssLOG_FUNC_ENTRY();
        
        CreateWidgetIfNotPresent(&DockPreivew, GetDockPreviewColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockPreivew->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* as = static_cast<ssGUI::Extensions::AdvancedSize*>(DockPreivew->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ap->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
        ap->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::CENTER);

        as->SetHorizontalPercentage(1);
        as->SetVerticalPercentage(1);

        ssLOG_FUNC_EXIT();
    }

    void Docker::DiscardPreview()
    {
        if(DockPreivew != nullptr)
        {
            DockPreivew->Delete();
            DockPreivew = nullptr;
        }
    }

    void Docker::DrawTriggerArea()
    {
        ssLOG_FUNC_ENTRY();
        
        CreateWidgetIfNotPresent(&DockTrigger, GetTriggerAreaColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockTrigger->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* as = static_cast<ssGUI::Extensions::AdvancedSize*>(DockTrigger->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        // as->SetHorizontalUsePercentage(UseTriggerPercentage);
        // as->SetVerticalUsePercentage(UseTriggerPercentage);

        if(UseTriggerPercentage)
        {
            ap->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
            ap->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::CENTER);
            as->SetHorizontalPercentage(GetTriggerHorizontalPercentage());
            as->SetHorizontalPixel(0);
            as->SetVerticalPercentage(GetTriggerVerticalPercentage());
            as->SetVerticalPixel(0);
        }
        else
        {
            ap->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
            ap->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::CENTER);
            as->SetHorizontalPixel(GetTriggerHorizontalPixel());
            as->SetHorizontalPercentage(0);
            as->SetVerticalPixel(GetTriggerVerticalPixel());
            as->SetVerticalPercentage(0);
        }

        ssLOG_FUNC_EXIT();
    }

    void Docker::DiscardTriggerArea()
    {
        if(DockTrigger != nullptr)
        {
            DockTrigger->Delete();
            DockTrigger = nullptr;
        }
    }

    int Docker::GetRealChildrenCount(ssGUI::GUIObject* checkObj)
    {
        int realChildrenCount = 0;
        checkObj->StashChildrenIterator();
        checkObj->MoveChildrenIteratorToFirst();
        while(!checkObj->IsChildrenIteratorEnd())
        {
            auto checkObjChild = checkObj->GetCurrentChild();

            if(!checkObjChild->HasTag(ssGUI::Tags::FLOATING) && !checkObjChild->HasTag(ssGUI::Tags::OVERLAY))
                realChildrenCount++;

            checkObj->MoveChildrenIteratorNext();
        }
        checkObj->PopChildrenIterator();

        return realChildrenCount;
    }

    //This function basically make sure there's no unnecessary generated Docker that only contains 1 child
    void Docker::ChildRemoved(ssGUI::GUIObject* child)
    {
        ssLOG_FUNC_ENTRY();
        
        if(ChildRemoveGuard ||
            Container->IsUserCreated() ||
            !Container->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        ChildRemoveGuard = true;
        
        int realChildrenCount = GetRealChildrenCount(Container);

        if(realChildrenCount > 1)
        {
            ChildRemoveGuard = false;
            ssLOG_FUNC_EXIT();
            return;
        }

        //If there's no children left in the docker, remove this.
        if(realChildrenCount == 0)
        {
            //Don't need to stash children iterator as this will get deleted anyway
            //Transfer the rest of the floating or overlay children
            Container->MoveChildrenIteratorToFirst();
            while(!Container->IsChildrenIteratorEnd())
            {
                auto containerChild = Container->GetCurrentChild();
                containerChild->SetParent(Container->GetParent());
                Container->MoveChildrenIteratorNext();
            }
            
            Container->Delete();
            ChildRemoveGuard = false;
            ssLOG_FUNC_EXIT();
            return;
        }
        //If there's only 1 child left in the docker
        else
        {            
            //Don't need to stash children iterator as this will get deleted anyway
            Container->MoveChildrenIteratorToFirst();
            ssGUI::GUIObject* childLeft = nullptr;
            while (!Container->IsChildrenIteratorEnd())
            {
                auto currentChild = Container->GetCurrentChild();
                // Container->GetCurrentChild()->SetParent(Container);
                if(!currentChild->HasTag(ssGUI::Tags::FLOATING) && !currentChild->HasTag(ssGUI::Tags::OVERLAY))
                {
                    childLeft = currentChild;
                    break;
                }    
                
                Container->MoveChildrenIteratorNext();
            }
            
            //Set size when transferring child
            glm::vec2 originalSize = Container->GetSize();
            glm::vec2 originalPos = Container->GetGlobalPosition();
            
            //Check if Docker is floating, if so record/restore global position
            bool originalDockerIsFloating = Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW ? 
                (static_cast<ssGUI::Window*>(Container)->HasTitlebar()) : false;

            //Transfer the children.
            {
                auto containerParent = Container->GetParent();

                if(containerParent != nullptr)
                {
                    containerParent->StashChildrenIterator();
                    containerParent->MoveChildrenIteratorToChild(Container);
                    ssGUI::Hierarchy::ChildToken posIt = containerParent->GetCurrentChildToken();
                    // posIt++;

                    //Move the only child to the container's parent and restore order
                    childLeft->SetParent(containerParent);
                    containerParent->MoveChildrenIteratorToLast();
                    ssGUI::Hierarchy::ChildToken childIt = containerParent->GetCurrentChildToken();
                    containerParent->ChangeChildOrderToAfterPosition(childIt, posIt);
                    containerParent->PopChildrenIterator();
                }

                //Don't need to stash children iterator as this will get deleted anyway
                //Transfer the rest of the floating or overlay children
                //TODO: Might be necessary to maintain order for the overlay or floating children. Not too sure :/
                Container->MoveChildrenIteratorToFirst();
                while(!Container->IsChildrenIteratorEnd())
                {
                    auto containerChild = Container->GetCurrentChild();
                    containerChild->SetParent(containerParent);
                    Container->MoveChildrenIteratorNext();
                }

                //Remove container because this is now empty docker
                Container->Delete();
            }

            //Restore size
            childLeft->SetSize(originalSize);

            //Restore position if needed
            if(originalDockerIsFloating)
            {
                childLeft->SetGlobalPosition(originalPos);

                //Check childLeft is generated docker. If so, need to make it floatable
                if(!childLeft->IsUserCreated() && childLeft->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME) &&
                    childLeft->GetType() == ssGUI::Enums::GUIObjectType::WINDOW)
                {
                    static_cast<ssGUI::Window*>(childLeft)->SetTitlebar(true);
                    static_cast<ssGUI::Window*>(childLeft)->SetResizeType(ssGUI::Enums::ResizeType::ALL);
                    auto bgColor = childLeft->GetBackgroundColor();
                    bgColor.a = 255;
                    childLeft->SetBackgroundColor(bgColor);
                    
                    //Enable all extensions back
                    auto allExtensions = childLeft->GetListOfExtensions();
                    for(auto extension : allExtensions)
                    {
                        if(extension->GetExtensionName() == ssGUI::Extensions::Docker::EXTENSION_NAME)
                            continue;
                        
                        extension->SetEnabled(true);
                    }

                    //Enable all children back
                    childLeft->StashChildrenIterator();
                    childLeft->MoveChildrenIteratorToFirst();
                    while(!childLeft->IsChildrenIteratorEnd())
                    {
                        childLeft->GetCurrentChild()->SetEnabled(true);
                        childLeft->MoveChildrenIteratorNext();
                    }
                    childLeft->PopChildrenIterator();
                }
            }
        }

        ChildRemoveGuard = false;

        ssLOG_FUNC_EXIT();
    }

    const std::string Docker::EXTENSION_NAME = "Docker";

    void Docker::SetDefaultGeneratedFloatingDockerWindow(ssGUI::Window* window)
    {
        DefaultGeneratedDockerWindow = window;
    }

    ssGUI::Window* Docker::GetDefaultGeneratedFloatingDockerWindow()
    {
        return DefaultGeneratedDockerWindow;
    }

    void Docker::SetChildrenDockerUseThisSettings(bool use)
    {
        ChildrenDockerUseThisSettings = use;
    }

    bool Docker::IsChildrenDockerUseThisSettings() const
    {
        return ChildrenDockerUseThisSettings;
    }

    void Docker::SetUseTriggerPercentage(bool use)
    {
        UseTriggerPercentage = use;
    }

    bool Docker::IsUseTriggerPercentage() const
    {
        return UseTriggerPercentage;
    }

    void Docker::SetTriggerHorizontalPercentage(float percent)
    {
        TriggerHorizontalPercentage = percent;
    }

    float Docker::GetTriggerHorizontalPercentage() const
    {
        return TriggerHorizontalPercentage;
    }

    void Docker::SetTriggerVerticalPercentage(float percent)
    {
        TriggerVerticalPercentage = percent;
    }

    float Docker::GetTriggerVerticalPercentage() const
    {
        return TriggerVerticalPercentage;
    }

    void Docker::SetTriggerHorizontalPixel(int pixel)
    {
        TriggerHorizontalPixel = pixel;
    }

    int Docker::GetTriggerHorizontalPixel() const
    {
        return TriggerHorizontalPixel;
    }

    void Docker::SetTriggerVerticalPixel(int pixel)
    {
        TriggerVerticalPixel = pixel;
    }

    int Docker::GetTriggerVerticalPixel() const
    {
        return TriggerVerticalPixel;
    }

    void Docker::SetTriggerAreaColor(glm::u8vec4 color)
    {
        TriggerAreaColor = color;
    }

    glm::u8vec4 Docker::GetTriggerAreaColor() const
    {
        return TriggerAreaColor;
    }

    void Docker::SetDockPreviewColor(glm::u8vec4 color)
    {
        DockPreviewColor = color;
    }

    glm::u8vec4 Docker::GetDockPreviewColor() const
    {
        return DockPreviewColor;
    }

    bool Docker::IsValidDocking() const
    {
        return ValidDocking;
    }

    void Docker::SetEnabled(bool enabled)
    {
        Enabled = enabled;
    }

    bool Docker::IsEnabled() const
    {
        return Enabled;
    }

    //Extension methods
    void Docker::Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, ssGUI::GUIObject* mainWindow)
    {
        ssLOG_FUNC_ENTRY();
        
        if(!isPreUpdate || Container == nullptr || !Enabled)
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        //Check if Layout extension exists
        if(!Container->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        int baseChildCount = 0;
        if(DockPreivew != nullptr)
            baseChildCount++;
        if(DockTrigger != nullptr)
            baseChildCount++;

        //If there are no other children, tell layout to exclude trigger and preview
        if(Container->GetChildrenCount() > baseChildCount)
        {
            //static_cast<ssGUI::Extensions::Layout*>(Container->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME))->SetEnabled(true);
            DiscardPreview();
            DiscardTriggerArea();
            ssLOG_FUNC_EXIT();
            return;
        }

        //If global dock mode is true, check the cursor against the trigger area
        if(Dockable::GlobalDockMode && inputStatus.DockingBlockedObject == nullptr)
        {
            ssGUI::GUIObject* curParent = Container;
            bool underDockingTopLevel = false;

            //Find out if the top level parent for the object to be docked is deeping than the this object's top parent
            //If so, we can proceed to dock
            do
            {                
                curParent = curParent->GetParent();

                if(curParent == nullptr)
                    break;

                if(curParent == Dockable::DockingTopLevelParent)
                    underDockingTopLevel = true;
            }
            while (curParent != nullptr && curParent->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW);

            ssGUI::GUIObject* mainWindow = curParent != nullptr && curParent->GetType() == ssGUI::Enums::GUIObjectType::MAIN_WINDOW ?
                                            curParent :
                                            nullptr;

            //This is not supposed to happen
            if(mainWindow == nullptr)
            {
                ssGUI_WARNING(ssGUI_EXT_TAG, "what?");
                ssLOG_FUNC_EXIT();
                return;
            }

            //If this is not the same parent as the one which is being docked, exit
            if(mainWindow != Dockable::MainWindowUnderDocking || !underDockingTopLevel)
            {
                DiscardPreview();
                DiscardTriggerArea();
                ValidDocking = false;
                ssLOG_FUNC_EXIT();
                return;
            }
            
            glm::vec2 containerPos = Container->GetGlobalPosition();
            glm::vec2 containerSize = Container->GetSize();
            int titleBarOffset = Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && Container->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW ? 
                                    static_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() : 0;
            containerSize.y -= titleBarOffset;
            
            glm::vec2 triggerSize = IsUseTriggerPercentage() ? glm::vec2(containerSize.x * GetTriggerHorizontalPercentage(), containerSize.y * GetTriggerVerticalPercentage()) : 
                                        glm::vec2(GetTriggerHorizontalPixel(), GetTriggerVerticalPixel());

            //bool previewDrawn = false;

            //Check if the cursor is inside the window
            ssGUI::Backend::BackendMainWindowInterface* mainWindowInterface = dynamic_cast<ssGUI::MainWindow*>(mainWindow)->GetBackendWindowInterface();
            bool mouseInsideWindow =    inputInterface->GetCurrentMousePosition(mainWindowInterface).x >= containerPos.x && 
                                        inputInterface->GetCurrentMousePosition(mainWindowInterface).x <= containerPos.x + containerSize.x &&
                                        inputInterface->GetCurrentMousePosition(mainWindowInterface).y >= containerPos.y + titleBarOffset && 
                                        inputInterface->GetCurrentMousePosition(mainWindowInterface).y <= containerPos.y + containerSize.y;

            if(!mouseInsideWindow)
            {
                DiscardPreview();
                DiscardTriggerArea();
                ValidDocking = false;
                ssLOG_FUNC_EXIT();
                return;
            }

            //Center
            if( inputInterface->GetCurrentMousePosition(mainWindowInterface).x >= containerPos.x + (containerSize.x - triggerSize.x) * 0.5 && 
                inputInterface->GetCurrentMousePosition(mainWindowInterface).x <= containerPos.x + (containerSize.x + triggerSize.x) * 0.5 &&
                inputInterface->GetCurrentMousePosition(mainWindowInterface).y >= containerPos.y + titleBarOffset + (containerSize.y - triggerSize.y) * 0.5 && 
                inputInterface->GetCurrentMousePosition(mainWindowInterface).y <= containerPos.y + titleBarOffset + (containerSize.y + triggerSize.y) * 0.5)
            {
                DrawPreview();
                DiscardTriggerArea();
                //previewDrawn = true;
                ValidDocking = true;
                Dockable::TargetDockSide = Dockable::DockSide::CENTER;
            }
            else
            {                
                Dockable::TargetDockSide = Dockable::DockSide::NONE;
                ValidDocking = false;
                DrawTriggerArea();
                DiscardPreview();
            }

            inputStatus.DockingBlockedObject = Container;
            Dockable::ObjectToDockNextTo = Container;
        }
        else
        {
            DiscardPreview();
            DiscardTriggerArea();
            ValidDocking = false;
        }

        ssLOG_FUNC_EXIT();
    }

    void Docker::Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {}

    std::string Docker::GetExtensionName()
    {
        return EXTENSION_NAME;
    }

    void Docker::BindToObject(ssGUI::GUIObject* bindObj)
    {
        ssLOG_FUNC_ENTRY();
        
        Container = bindObj;
        ssGUI::GUIObject* containerParent = Container->GetParent();

        //Check if we are using the parent's docker & layout settings or not
        if(containerParent != nullptr && containerParent->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME)
            && static_cast<ssGUI::Extensions::Docker*>(containerParent->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME))->IsChildrenDockerUseThisSettings()
            && static_cast<ssGUI::Extensions::Docker*>(containerParent->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME))->IsEnabled())
        {            
            ssGUI::Extensions::Docker* parentDocker = static_cast<ssGUI::Extensions::Docker*>(containerParent->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME));
            Copy(parentDocker);

            if(containerParent->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
            {
                ssGUI::Extensions::Layout* parentLayout = static_cast<ssGUI::Extensions::Layout*>(containerParent->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME));
                if(Container->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
                {
                    bool originalOrientation = static_cast<ssGUI::Extensions::Layout*>(Container->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME))->IsHorizontalLayout();
                    static_cast<ssGUI::Extensions::Layout*>(Container->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME))->Copy(parentLayout);
                    static_cast<ssGUI::Extensions::Layout*>(Container->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME))->SetHorizontalLayout(originalOrientation);
                }
                else
                    Container->AddExtensionCopy(parentLayout);            
            }
            else if(!Container->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
                Container->AddExtension<ssGUI::Extensions::Layout>();
        }
        //Otherwise we generate layout extension if there isn't one
        else
        {            
            //Check if there's a layout extension for container
            if(!Container->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
                Container->AddExtension<ssGUI::Extensions::Layout>();
        }

        if(!Container->IsEventCallbackExist(ssGUI::Enums::EventType::CHILD_REMOVED))    
            Container->AddEventCallback(ssGUI::Enums::EventType::CHILD_REMOVED);
        
        Container->GetEventCallback(ssGUI::Enums::EventType::CHILD_REMOVED)->AddEventListener
        (
            EXTENSION_NAME,
            [](ssGUI::EventInfo info)
            {
                if(!info.Container->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME))
                {
                    ssGUI_ERROR(ssGUI_EXT_TAG, "Failed to find docker extension. Probably something wrong with cloning");
                    ssLOG_EXIT_PROGRAM();
                    return;
                }

                // if(src->HasTag(ssGUI::Tags::FLOATING) || src->HasTag(ssGUI::Tags::OVERLAY))
                    // return;

                ssGUI::Extensions::Docker* containerDocker = static_cast<ssGUI::Extensions::Docker*>
                        (info.Container->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME));

                containerDocker->ChildRemoved(info.EventSource);
            }
        );

        ssLOG_FUNC_EXIT();
    }

    void Docker::Copy(ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        
        ssGUI::Extensions::Docker* docker = static_cast<ssGUI::Extensions::Docker*>(extension);
        
        Enabled = docker->IsEnabled();
        ChildrenDockerUseThisSettings = docker->IsChildrenDockerUseThisSettings();

        UseTriggerPercentage = docker->IsUseTriggerPercentage();
        TriggerHorizontalPercentage = docker->GetTriggerHorizontalPercentage();
        TriggerVerticalPercentage = docker->GetTriggerVerticalPercentage();
        TriggerHorizontalPixel = docker->GetTriggerHorizontalPixel();
        TriggerVerticalPixel = docker->GetTriggerVerticalPixel();
        TriggerAreaColor = docker->GetTriggerAreaColor();
        DockPreviewColor = docker->GetDockPreviewColor();
    }

    ObjectsReferences* Docker::Internal_GetObjectsReferences()
    {
        return nullptr;
    }

    Docker* Docker::Clone()
    {
        Docker* temp = new Docker(*this);
        return temp;
    }
}

}