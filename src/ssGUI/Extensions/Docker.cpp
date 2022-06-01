#include "ssGUI/Extensions/Docker.hpp"


#include "ssGUI/Extensions/Dockable.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/Layout.hpp"


namespace ssGUI::Extensions
{
    ssGUI::Window* Docker::DefaultGeneratedDockerWindow = nullptr;

    Docker::Docker() : Container(nullptr), Enabled(true), ChildrenDockerUseThisSettings(true), UseTriggerPercentage(true),
                        TriggerHorizontalPercentage(0.5), TriggerVerticalPercentage(0.5), TriggerHorizontalPixel(15), TriggerVerticalPixel(15),
                        TriggerAreaColor(glm::u8vec4(87, 207, 255, 127)), DockPreviewColor(glm::u8vec4(255, 255, 255, 127)), DockPreivew(nullptr),
                        DockTrigger(nullptr), ChildRemoveGuard(false)
    {}

    Docker::~Docker()
    {
        //Cleanup dock preview and trigger if exist
        DiscardPreview();
        DiscardTriggerArea();
        
        //Remove EventCallback
        if(Container != nullptr && Container->IsAnyEventCallbackExist<ssGUI::EventCallbacks::ChildRemovedEventCallback>())
        {
            auto ecb = Container->GetAnyEventCallback<ssGUI::EventCallbacks::ChildRemovedEventCallback>();
            ecb->RemoveEventListener(EXTENSION_NAME);
            if(ecb->GetEventListenerCount() == 0)
                Container->RemoveAnyEventCallback<ssGUI::EventCallbacks::ChildRemovedEventCallback>();
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
        FUNC_DEBUG_ENTRY();
        
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
        
        FUNC_DEBUG_EXIT();
    }

    void Docker::DrawPreview()
    {
        FUNC_DEBUG_ENTRY();
        
        CreateWidgetIfNotPresent(&DockPreivew, GetDockPreviewColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockPreivew->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* as = static_cast<ssGUI::Extensions::AdvancedSize*>(DockPreivew->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);

        as->SetHorizontalUsePercentage(true);
        as->SetVerticalUsePercentage(true);
        as->SetHorizontalPercentage(1);
        as->SetVerticalPercentage(1);

        FUNC_DEBUG_EXIT();
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
        FUNC_DEBUG_ENTRY();
        
        CreateWidgetIfNotPresent(&DockTrigger, GetTriggerAreaColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockTrigger->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* as = static_cast<ssGUI::Extensions::AdvancedSize*>(DockTrigger->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        as->SetHorizontalUsePercentage(UseTriggerPercentage);
        as->SetVerticalUsePercentage(UseTriggerPercentage);

        if(UseTriggerPercentage)
        {
            ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
            ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);
            as->SetHorizontalPercentage(GetTriggerHorizontalPercentage());
            as->SetVerticalPercentage(GetTriggerVerticalPercentage());
        }
        else
        {
            ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
            ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);
            as->SetHorizontalPixel(GetTriggerHorizontalPixel());
            as->SetVerticalPixel(GetTriggerVerticalPixel());
        }

        FUNC_DEBUG_EXIT();
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
        FUNC_DEBUG_ENTRY();
        
        if(ChildRemoveGuard ||
            Container->IsUserCreated() ||
            !Container->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        ChildRemoveGuard = true;
        
        int realChildrenCount = GetRealChildrenCount(Container);

        if(realChildrenCount > 1)
        {
            ChildRemoveGuard = false;
            FUNC_DEBUG_EXIT();
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
            FUNC_DEBUG_EXIT();
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
                    containerParent->FindChild(Container);
                    auto posIt = containerParent->GetCurrentChildReferenceIterator();
                    // posIt++;

                    //Move the only child to the container's parent and restore order
                    childLeft->SetParent(containerParent);
                    containerParent->MoveChildrenIteratorToLast();
                    auto childIt = containerParent->GetCurrentChildReferenceIterator();
                    containerParent->ChangeChildOrderToAfterPosition(childIt, posIt);
                    containerParent->PopChildrenIterator();
                }

                //Don't need to stash children iterator as this will get deleted anyway
                //Transfer the rest of the floating or overlay children
                //NOTE: Might be necessary to maintain order for the overlay or floating children. Not too sure :/
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
                        childLeft->GetCurrentChild()->SetVisible(true);
                        childLeft->MoveChildrenIteratorNext();
                    }
                    childLeft->PopChildrenIterator();
                }
            }
        }

        ChildRemoveGuard = false;

        FUNC_DEBUG_EXIT();
    }

    const std::string Docker::EXTENSION_NAME = "Docker";

    void Docker::SetDefaultGeneratedDockerWindow(ssGUI::Window* window)
    {
        DefaultGeneratedDockerWindow = window;
    }

    ssGUI::Window* Docker::GetDefaultGeneratedDockerWindow()
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

    void Docker::SetEnabled(bool enabled)
    {
        Enabled = enabled;
    }

    bool Docker::IsEnabled() const
    {
        return Enabled;
    }

    //Extension methods
    void Docker::Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();
        
        if(!isPreUpdate || Container == nullptr || !Enabled)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        //Check if Layout extension exists
        if(!Container->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
        {
            FUNC_DEBUG_EXIT();
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
            FUNC_DEBUG_EXIT();
            return;
        }

        //If global dock mode is true, check the cursor against the trigger area
        if(Dockable::GlobalDockMode && !globalInputStatus.DockingBlocked)
        {
            ssGUI::GUIObject* curParent = Container;
            while (curParent->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW && curParent != nullptr && curParent != Dockable::DockingTopLevelParent)
            {
                curParent = curParent->GetParent();
            }

            //If this is not the same parent as the one which is being docked, exit
            if((Dockable::DockingTopLevelParent == Dockable::MainWindowUnderDocking && curParent != Dockable::MainWindowUnderDocking) || 
                curParent != Dockable::DockingTopLevelParent)
            {
                DiscardPreview();
                DiscardTriggerArea();
                FUNC_DEBUG_EXIT();
                return;
            }
            
            glm::vec2 containerPos = Container->GetGlobalPosition();
            glm::vec2 containerSize = Container->GetSize();
            int titleBarOffset = Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && Container->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW ? 
                                    static_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() : 0;
            containerSize.y -= titleBarOffset;
            
            glm::vec2 triggerSize = IsUseTriggerPercentage() ? glm::vec2(containerSize.x * GetTriggerHorizontalPercentage(), containerSize.y * GetTriggerVerticalPercentage()) : 
                                        glm::vec2(GetTriggerHorizontalPixel(), GetTriggerVerticalPixel());

            bool previewDrawn = false;

            //Center
            if(inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).x >= containerPos.x + (containerSize.x - triggerSize.x) * 0.5 && 
                inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).x <= containerPos.x + (containerSize.x + triggerSize.x) * 0.5 &&
                inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).y >= containerPos.y + titleBarOffset + (containerSize.y - triggerSize.y) * 0.5 && 
                inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).y <= containerPos.y + titleBarOffset + (containerSize.y + triggerSize.y) * 0.5)
            {
                DrawPreview();
                previewDrawn = true;
                Dockable::TargetDockSide = Dockable::DockSide::CENTER;
            }
            else
            {                
                DiscardPreview();
            }
            
            //Check if the cursor is inside the window
            if(inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).x >= containerPos.x && inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).x <= containerPos.x + containerSize.x &&
                inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).y >= containerPos.y + titleBarOffset && inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)).y <= containerPos.y + containerSize.y)
            {
                if(!previewDrawn)
                {
                    Dockable::TargetDockSide = Dockable::DockSide::NONE;
                    DrawTriggerArea();
                }
                else
                    DiscardTriggerArea();
                    
                globalInputStatus.DockingBlocked = true;
                windowInputStatus.DockingBlocked = true;
                Dockable::TargetDockObject = Container;
            }
            else
            {
                    Dockable::TargetDockSide = Dockable::DockSide::NONE;
                
                DiscardTriggerArea();
            }
        }
        else
        {
            DiscardPreview();
            DiscardTriggerArea();
        }

        FUNC_DEBUG_EXIT();
    }

    void Docker::Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {}

    std::string Docker::GetExtensionName()
    {
        return EXTENSION_NAME;
    }

    void Docker::BindToObject(ssGUI::GUIObject* bindObj)
    {
        FUNC_DEBUG_ENTRY();
        
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
                    parentLayout->Clone(Container);            
            }
            else if(!Container->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
                Container->AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Layout>());
        }
        //Otherwise we generate layout extension if there isn't one
        else
        {            
            //Check if there's a layout extension for container
            if(!Container->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
                Container->AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Layout>());
        }

        if(!Container->IsEventCallbackExist(ssGUI::EventCallbacks::ChildRemovedEventCallback::EVENT_NAME))    
            Container->AddEventCallback(ssGUI::Factory::Create<ssGUI::EventCallbacks::ChildRemovedEventCallback>());
        
        Container->GetEventCallback(ssGUI::EventCallbacks::ChildRemovedEventCallback::EVENT_NAME)->AddEventListener
        (
            EXTENSION_NAME,
            [](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* refs)
            {
                if(!container->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME))
                {
                    DEBUG_LINE("Failed to find docker extension. Probably something wrong with cloning");
                    DEBUG_EXIT_PROGRAM();
                    return;
                }

                // if(src->HasTag(ssGUI::Tags::FLOATING) || src->HasTag(ssGUI::Tags::OVERLAY))
                    // return;

                ssGUI::Extensions::Docker* containerDocker = static_cast<ssGUI::Extensions::Docker*>
                        (container->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME));

                containerDocker->ChildRemoved(src);
            }
        );

        FUNC_DEBUG_EXIT();
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

    Docker* Docker::Clone(ssGUI::GUIObject* newContainer)
    {
        Docker* temp = new Docker(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
}