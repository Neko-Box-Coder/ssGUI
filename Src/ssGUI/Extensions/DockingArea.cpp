#include "ssGUI/Extensions/DockingArea.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/GUIObjectClasses/GUIObject.hpp"
#include "ssGUI/GUIObjectClasses/Widget.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"
#include "ssGUI/Extensions/DockableV2.hpp"
#include "ssGUI/GUIObjectClasses/MainWindow.hpp"
#include "ssGUI/DataClasses/DockEventInfo.hpp"

#include "ssGUI/ssGUITags.hpp"

namespace ssGUI
{

namespace Extensions
{
    DockingArea::DockingArea() :    Container(nullptr),
                                    Enabled(true),
                                    CurrentObjectsReferences(),
                                    DockPreview(-1),
                                    DockTrigger(-1),
                                    TriggerSizePercentage(0.5, 0.5),
                                    TriggerSizePixels(),
                                    TriggerColor(87, 207, 255, 127),
                                    PreviewColor(255, 255, 255, 127),
                                    PauseChildRemovedEventListener(false),
                                    DefaultLayout(nullptr)
    {}

    DockingArea::~DockingArea()
    {}

    DockingArea::DockingArea(DockingArea const& other)
    {
        Container = nullptr;
        Copy(&other);
    }
    
    void DockingArea::ConstructRenderInfo()
    {}

    void DockingArea::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, 
                                                ssGUI::GUIObject* mainWindow, 
                                                glm::vec2 mainWindowPositionOffset)
    {}

    void DockingArea::CreateDockPreviewAndTrigger()
    {
        if(Container == nullptr)
        {
            ssGUI_ERROR(ssGUI_EXT_TAG, "Trying to create dock preview and trigger without container");
            return;
        }
    
        if(DockPreview < 0 || CurrentObjectsReferences.GetObjectReference(DockPreview) == nullptr)
        {
            auto* previewObj = Container->AddChild<ssGUI::Widget>(true);
            previewObj->SetUserCreated(false);
            previewObj->SetInteractable(false);
            previewObj->SetBlockInput(false);
            previewObj->AddExtension<ssGUI::Extensions::AdvancedPosition>();
            previewObj->AddExtension<ssGUI::Extensions::AdvancedSize>()->SetSizingPercentage(1, 1);
            previewObj->AddTag(ssGUI::Tags::FLOATING);
            previewObj->SetEnabled(false);
            DockPreview = CurrentObjectsReferences.AddObjectReference(previewObj);
        }
        
        if(DockTrigger < 0 || CurrentObjectsReferences.GetObjectReference(DockTrigger) == nullptr)
        {
            auto* triggerObj = Container->AddChild<ssGUI::Widget>(true);
            triggerObj->SetUserCreated(false);
            triggerObj->SetInteractable(false);
            triggerObj->SetBlockInput(false);
            triggerObj->AddExtension<ssGUI::Extensions::AdvancedPosition>();
            triggerObj->AddExtension<ssGUI::Extensions::AdvancedSize>();
            triggerObj->AddTag(ssGUI::Tags::FLOATING);
            triggerObj->SetEnabled(false);
            DockTrigger = CurrentObjectsReferences.AddObjectReference(triggerObj);
        }
    }

    void DockingArea::DrawPreview()
    {
        auto* dockPreview = CurrentObjectsReferences.GetObjectReference<ssGUI::Widget>(DockPreview);
        if(dockPreview->IsEnabled())
            return;
        
        dockPreview->SetBackgroundColor(PreviewColor);
        dockPreview->SetEnabled(true);
    }
    
    void DockingArea::DiscardPreview()
    {
        auto* dockPreview = CurrentObjectsReferences.GetObjectReference<ssGUI::Widget>(DockPreview);
        dockPreview->SetEnabled(false);
    }
    
    void DockingArea::DrawTriggerArea()
    {
        auto* dockTrigger = CurrentObjectsReferences.GetObjectReference<ssGUI::Widget>(DockTrigger);

        dockTrigger->SetBackgroundColor(TriggerColor);
        
        auto* as = dockTrigger->GetExtension<ssGUI::Extensions::AdvancedSize>();
        as->SetSizingPercentage(TriggerSizePercentage.x, TriggerSizePercentage.y);
        as->SetSizingPixel(TriggerSizePixels.x, TriggerSizePixels.y);
        
        dockTrigger->SetEnabled(true);
    }
    
    void DockingArea::DiscardTriggerArea()
    {
        auto* dockTrigger = CurrentObjectsReferences.GetObjectReference<ssGUI::Widget>(DockTrigger);
        dockTrigger->SetEnabled(false);
    }
    
    ssGUI::GUIObject* DockingArea::GetFirstDockingParent(ssGUI::GUIObject* startObject) const
    {
        if(startObject == nullptr)
            return nullptr;

        ssGUI::GUIObject* currentParent = startObject;
        
        while(currentParent != nullptr)
        {
            if( currentParent->IsExtensionExist<ssGUI::Extensions::DockingArea>() ||
                currentParent->IsExtensionExist<ssGUI::Extensions::DockableV2>())
            {
                return currentParent;
            }
            
            currentParent = currentParent->GetParent();
        }
        
        return nullptr;
    }

    void DockingArea::NotifyBeforeDockingEvent(ssGUI::GUIObject* externalObject, bool& isAbort)
    {
        ssGUI::DockEventInfo eventInfo;
        eventInfo.CurrentDockSide = ssGUI::Enums::DockSide::CENTER;
        
        if(Container->IsEventCallbackExist(ssGUI::Enums::EventType::BEFORE_EXTERNAL_OBJECT_BEING_DOCKED))
            Container->GetEventCallback(ssGUI::Enums::EventType::BEFORE_EXTERNAL_OBJECT_BEING_DOCKED)->Notify(externalObject, &eventInfo);
        
        if(eventInfo.AbortDocking)
        {
            isAbort = true;
            return;
        }
        
        if(externalObject->IsEventCallbackExist(ssGUI::Enums::EventType::BEFORE_OBJECT_DOCKING))
            externalObject->GetEventCallback(ssGUI::Enums::EventType::BEFORE_OBJECT_DOCKING)->Notify(externalObject, &eventInfo);
        
        if(eventInfo.AbortDocking)
        {
            isAbort = true;
            return;    
        }
        isAbort = false;
    }
    
    void DockingArea::NotifyDockedEvent(ssGUI::GUIObject* externalObject)
    {
        ssGUI::DockEventInfo eventInfo;
        eventInfo.CurrentDockSide = ssGUI::Enums::DockSide::CENTER;
    
        if(Container->IsEventCallbackExist(ssGUI::Enums::EventType::EXTERNAL_OBJECT_DOCKED))
            Container->GetEventCallback(ssGUI::Enums::EventType::EXTERNAL_OBJECT_DOCKED)->Notify(externalObject, &eventInfo);
        
        if(externalObject->IsEventCallbackExist(ssGUI::Enums::EventType::OBJECT_DOCKED))
            externalObject->GetEventCallback(ssGUI::Enums::EventType::OBJECT_DOCKED)->Notify(externalObject, &eventInfo);
    }
    

    const std::string DockingArea::EXTENSION_NAME = "Docking Area";

    const std::string DockingArea::ListenerKey = "Docking Area";

    void DockingArea::SetTriggerSizePercentage(glm::vec2 triggerSize)
    {
        TriggerSizePercentage = triggerSize;
    }
    
    glm::vec2 DockingArea::GetTriggerSizePercentage()
    {
        return TriggerSizePercentage;
    }
    
    void DockingArea::SetTriggerSizePixels(glm::ivec2 triggerSize)
    {
        TriggerSizePixels = triggerSize;
    }
    
    glm::ivec2 DockingArea::GetTriggerSizePixels()
    {
        return TriggerSizePixels;
    }
    
    void DockingArea::SetTriggerColor(glm::u8vec4 color)
    {
        TriggerColor = color;
    }
    
    glm::u8vec4 DockingArea::GetTriggerColor()
    {
        return TriggerColor;
    }
    
    void DockingArea::SetDockPreviewColor(glm::u8vec4 color)
    {
        PreviewColor = color;
    }
    
    glm::u8vec4 DockingArea::GetDockPreviewColor()
    {
        return PreviewColor;
    }

    void DockingArea::DockExternalObject(ssGUI::GUIObject* externalObject)
    {
        if(Container == nullptr)
        {
            ssGUI_ERROR(ssGUI_EXT_TAG, "Trying to call docking without container!!");
            return;
        }
        
        bool abortDocking = false;
        
        NotifyBeforeDockingEvent(externalObject, abortDocking);
        if(abortDocking)
            return;

        externalObject->SetParent(Container);

        NotifyDockedEvent(externalObject);
    }
    
    void DockingArea::SetDefaultLayout(ssGUI::Extensions::Layout* layout)
    {
        DefaultLayout = layout;
    }

    ssGUI::Extensions::Layout* DockingArea::GetDefaultLayout() const
    {
        return DefaultLayout;
    }

    void DockingArea::SetEnabled(bool enabled)
    {
        Enabled = enabled;
    }

    bool DockingArea::IsEnabled() const
    {
        return Enabled;
    }
    
    void DockingArea::Internal_Update(bool isPreUpdate, 
                                            ssGUI::Backend::BackendSystemInputInterface* inputInterface, 
                                            ssGUI::InputStatus& currentInputStatus, 
                                            ssGUI::InputStatus& lastInputStatus, 
                                            ssGUI::GUIObject* mainWindow)
    {
        ssGUI_LOG_FUNC();

        #define DISCARD_AND_RETURN() do { DiscardPreview(); DiscardTriggerArea(); return; } while(0)
        
        //We do the docking before the container GUI Object update
        if(!isPreUpdate || Container == nullptr || Container->GetParent() == nullptr || !Enabled)
            return;
        
        //First check if we have layout or not
        if(!Container->IsExtensionExist<ssGUI::Extensions::Layout>())
            DISCARD_AND_RETURN();
        
        
        //Check if anything is trying to be docked to here
        if( !inputInterface->IsInputExistLastFrame(ssGUI::Enums::MouseButton::LEFT) &&
            !inputInterface->IsInputExistCurrentFrame(ssGUI::Enums::MouseButton::LEFT))
        {
            DISCARD_AND_RETURN();
        }
        
        if( lastInputStatus.CurrentDragData.GetDragDataType() != ssGUI::Enums::DragDataType::GUI_OBJECT ||
            (lastInputStatus.CurrentDragData.IsIntercepted() && lastInputStatus.CurrentDragData.GetInterceptor() == Container))
        {
            DISCARD_AND_RETURN();
        }
        
        if( lastInputStatus.CurrentDragData.GetDragData<ssGUI::GUIObject>()->Internal_IsDeleted() ||
            !lastInputStatus.CurrentDragData.GetDragData<ssGUI::GUIObject>()->IsExtensionExist<ssGUI::Extensions::DockableV2>())
        {
            DISCARD_AND_RETURN();
        }
    
        auto* objectBeingDocked = lastInputStatus.CurrentDragData.GetDragData<ssGUI::GUIObject>();
    
        //Check if we can intercept the docking data    
        bool isContainerBlockingMouseInout =    (Container->GetType() == ssGUI::Enums::GUIObjectType::WIDGET ? 
                                                static_cast<ssGUI::Widget*>(Container)->IsBlockInput() : 
                                                true);
        
        if(!isContainerBlockingMouseInout)
            DISCARD_AND_RETURN();        

        //Check if the mouse is being blocked by something else
        if( currentInputStatus.MouseInputBlockedData.GetBlockDataType() == ssGUI::Enums::BlockDataType::GUI_OBJECT &&
            !currentInputStatus.MouseInputBlockedData.GetBlockData<ssGUI::GUIObject>()->HasParentRecursively(objectBeingDocked) &&
            GetFirstDockingParent(currentInputStatus.MouseInputBlockedData.GetBlockData<ssGUI::GUIObject>()) != Container)
        {
            DISCARD_AND_RETURN();        
        }

        if( currentInputStatus.MouseInputBlockedData.GetBlockDataType() != ssGUI::Enums::BlockDataType::GUI_OBJECT &&
            currentInputStatus.MouseInputBlockedData.GetBlockDataType() != ssGUI::Enums::BlockDataType::NONE)
        {
            DISCARD_AND_RETURN();        
        }

        //Check if we are under the top level parent for the object being docked
        ssGUI::GUIObject* dockingObjectTopLevelParent = objectBeingDocked->GetExtension<ssGUI::Extensions::DockableV2>()->GetTopLevelParent();
        if(dockingObjectTopLevelParent != nullptr && !Container->HasParentRecursively(dockingObjectTopLevelParent))
        {
            DISCARD_AND_RETURN();
        }

        //Then check if we have already docked an object
        std::vector<ssGUI::GUIObject*> children = Container->GetListOfChildren();
        
        for(int i = 0; i < children.size(); ++i)
        {
            bool isDocking =    (children.at(i)->IsExtensionExist<ssGUI::Extensions::DockableV2>() ||
                                children.at(i)->IsExtensionExist<ssGUI::Extensions::DockingArea>());
        
            if( isDocking &&
                !children.at(i)->HasTag(ssGUI::Tags::FLOATING) &&
                !children.at(i)->HasTag(ssGUI::Tags::OVERLAY))
            {
                //If we already have a child that is docked, then don't need to do anything
                DISCARD_AND_RETURN();
            }
        }

        //Check if the mouse is inside the container
        glm::ivec2 mousePos = inputInterface->GetCurrentMousePosition(static_cast<ssGUI::MainWindow*>(mainWindow)->GetBackendWindowInterface());
        glm::vec2 containerMaxBound = Container->GetGlobalPosition() + Container->GetSize();
        
        if( mousePos.x < Container->GetGlobalPosition().x ||
            mousePos.x >= containerMaxBound.x ||
            mousePos.y < Container->GetGlobalPosition().y ||
            mousePos.y >= containerMaxBound.y)
        {
            DISCARD_AND_RETURN();
        }

        glm::vec2 containerContentTopLeft = Container->GetGlobalPosition();
        glm::vec2 containerContentSize = Container->GetSize();
        
        if(Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW)
        {
            containerContentTopLeft.y += static_cast<ssGUI::Window*>(Container)->GetTitlebarHeight();
            containerContentSize.y -= static_cast<ssGUI::Window*>(Container)->GetTitlebarHeight();
        }
        
        //Check if the mouse is inside the trigger area
        glm::vec2 containerCenter = containerContentTopLeft + containerContentSize / 2.f;
        glm::vec2 triggerSize = containerContentSize * GetTriggerSizePercentage() + glm::vec2(GetTriggerSizePixels());
        glm::vec2 triggerHalfSize = triggerSize / 2.f;
        
        glm::vec2 triggerMin = containerCenter - triggerHalfSize;
        glm::vec2 triggerMax = containerCenter + triggerHalfSize;
        
        if( mousePos.x >= triggerMin.x && 
            mousePos.x < triggerMax.x &&
            mousePos.y >= triggerMin.y && 
            mousePos.y < triggerMax.y)
        {
            //If so, show the preview if mouse held
            if(inputInterface->IsInputExistCurrentFrame(ssGUI::Enums::MouseButton::LEFT))
            {
                DrawPreview();
                DiscardTriggerArea();
            }
            //Otherwise, dock the object if mouse up
            else if(inputInterface->IsButtonOrKeyUp(ssGUI::Enums::MouseButton::LEFT))
            {
                lastInputStatus.CurrentDragData.SetInterceptor(Container);
                DockExternalObject(objectBeingDocked);
                DiscardPreview();    
                DiscardTriggerArea();
            }
        }
        //Otherwise, show the trigger area
        else
        {
            DrawTriggerArea();
            DiscardPreview();
        }
    }

    void DockingArea::Internal_Draw(  bool isPreRender, 
                                            ssGUI::Backend::BackendDrawingInterface* drawingInterface, 
                                            ssGUI::GUIObject* mainWindow, 
                                            glm::vec2 mainWindowPositionOffset)
    {
        //Nothing to draw
    }

    std::string DockingArea::GetExtensionName() const
    {
        return EXTENSION_NAME;
    }

    void DockingArea::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;
    
        CreateDockPreviewAndTrigger();
        
        //When binding to Object, check if Layout exists. If not, add layout automatically
        if(!bindObj->IsExtensionExist<ssGUI::Extensions::Layout>())
        {
            if(DefaultLayout != nullptr)
                bindObj->AddExtensionCopy(DefaultLayout);
            else
                bindObj->AddExtension<ssGUI::Extensions::Layout>();
        }

        //Add event callback for checking any empty docked parent when a child is removed
        Container->AddEventCallback(ssGUI::Enums::EventType::CHILD_REMOVED)->AddEventListener
        (
            ListenerKey,
            Container,
            [](ssGUI::EventInfo& info)
            {
                if(info.Container->GetParent() == nullptr)
                    return;
            
                auto* dockingAreaExt = info.Container->GetExtension<ssGUI::Extensions::DockingArea>();
                
                if(info.Container->IsUserCreated() || dockingAreaExt == nullptr)
                {
                    info.DeleteCurrentListener = true;
                    return;
                }

                if(dockingAreaExt->PauseChildRemovedEventListener)
                    return;

                //Check if current docking area is empty
                ssGUI::GUIObject* lastChild = nullptr;
                
                info.Container->StashChildrenIterator();
                info.Container->MoveChildrenIteratorToFirst();
                while(!info.Container->IsChildrenIteratorEnd())
                {
                    //Check if it is composite child
                    if(!info.Container->IsCurrentChildComposite())
                    {
                        if(lastChild != nullptr)
                        {
                            info.Container->PopChildrenIterator();
                            return;
                        }
                        
                        lastChild = info.Container->GetCurrentChild();
                    }
                    
                    info.Container->MoveChildrenIteratorNext();
                }
                info.Container->PopChildrenIterator();
                
                //If we have a child, move it out
                if(lastChild != nullptr)
                {
                    dockingAreaExt->PauseChildRemovedEventListener = true;
                    lastChild->SetParent(info.Container->GetParent());
                    info.Container->GetParent()->MoveChildAfterTargetChild(lastChild, info.Container);
                    dockingAreaExt->PauseChildRemovedEventListener = false;
                }

                //And then remove the container
                dockingAreaExt->PauseChildRemovedEventListener = true;
                info.DeleteCurrentListener = true;
                info.Container->Delete();
            }
        );
    }

    void DockingArea::Copy(const ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        
        auto* original = static_cast<const ssGUI::Extensions::DockingArea*>(extension);
        Enabled = original->IsEnabled();
        CurrentObjectsReferences = original->CurrentObjectsReferences;
        DockPreview = original->DockPreview;
        DockTrigger = original->DockTrigger;
        TriggerSizePercentage = original->TriggerSizePercentage;
        TriggerSizePixels = original->TriggerSizePixels;
        TriggerColor = original->TriggerColor;
        PreviewColor = original->PreviewColor;
        PauseChildRemovedEventListener = original->PauseChildRemovedEventListener;
        DefaultLayout = original->DefaultLayout;
    }

    ObjectsReferences* DockingArea::Internal_GetObjectsReferences()
    {
        return &CurrentObjectsReferences;
    }

    DockingArea* DockingArea::Clone()
    {
        DockingArea* temp = new DockingArea(*this);
        return temp;
    }
}

}