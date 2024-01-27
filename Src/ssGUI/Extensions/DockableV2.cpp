#include "ssGUI/Extensions/DockableV2.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/Border.hpp"
#include "ssGUI/Extensions/DockingArea.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/GUIObjectClasses/GUIObject.hpp"
#include "ssGUI/GUIObjectClasses/Widget.hpp"
#include "ssGUI/GUIObjectClasses/Window.hpp"
#include "ssGUI/ssGUITags.hpp"
#include "ssGUI/DataClasses/DockEventInfo.hpp"

namespace ssGUI
{

namespace Extensions
{
    DockableV2::DockableV2() :  Container(nullptr),
                                Enabled(true),
                                Undockable(true),
                                CurrentObjectsReferences(),
                                DockPreview(),
                                DockTrigger(),
                                TriggerMarginPercentage(),
                                TriggerMarginPixels(),
                                TriggerSizePercentage(0.25f, 0.5f),
                                TriggerSizePixels(),
                                TriggerColor(87, 207, 255, 127),
                                PreviewColor(255, 255, 255, 127),
                                TopLevelParent(-1),
                                LastDraggedDistance(),
                                DockedSinceLastUpdate(false),
                                AttemptToUndock(false),
                                DefaultDockingArea(nullptr),
                                DefaultNewParent(-1)
    {
        DockPreview[0] = -1;
        DockPreview[1] = -1;
        DockPreview[2] = -1;
        DockPreview[3] = -1;
        
        DockTrigger[0] = -1;
        DockTrigger[1] = -1;
        DockTrigger[2] = -1;
        DockTrigger[3] = -1;
    }

    DockableV2::~DockableV2()
    {}

    DockableV2::DockableV2(DockableV2 const& other)
    {
        Container = nullptr;
        Copy(&other);
    }
    
    void DockableV2::ConstructRenderInfo()
    {}

    void DockableV2::ConstructRenderInfo(ssGUI::Backend::DrawingInterface* drawingInterface, 
                                                ssGUI::GUIObject* mainWindow, 
                                                glm::vec2 mainWindowPositionOffset)
    {}

    void DockableV2::CreateDockPreviewAndTrigger()
    {
        if(Container == nullptr)
        {
            ssGUI_ERROR(ssGUI_EXT_TAG, "Trying to create dock preview and trigger without container");
            return;
        }
    
        if(DockPreview[0] < 0 || CurrentObjectsReferences.GetObjectReference(DockPreview[0]) == nullptr)
        {
            ssGUI::Widget* previewObjs[4];
            
            for(int i = 0; i < 4; ++i)
            {
                previewObjs[i] = Container->AddChild<ssGUI::Widget>(true);
                previewObjs[i]->SetUserCreated(false);
                previewObjs[i]->SetInteractable(false);
                previewObjs[i]->SetBlockInput(false);
                previewObjs[i]->AddExtension<ssGUI::Extensions::AdvancedPosition>();
                previewObjs[i]->AddExtension<ssGUI::Extensions::AdvancedSize>();
                previewObjs[i]->AddTag(ssGUI::Tags::FLOATING);
                previewObjs[i]->SetEnabled(false);
                DockPreview[i] = CurrentObjectsReferences.AddObjectReference(previewObjs[i]);
            }
            
            previewObjs[(int)ssGUI::Enums::DockSide::TOP]   ->GetExtension<ssGUI::Extensions::AdvancedPosition>()
                                                            ->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::TOP);
            
            previewObjs[(int)ssGUI::Enums::DockSide::TOP]   ->GetExtension<ssGUI::Extensions::AdvancedSize>()
                                                            ->SetSizingPercentage(1, 0.5);
            
            previewObjs[(int)ssGUI::Enums::DockSide::RIGHT] ->GetExtension<ssGUI::Extensions::AdvancedPosition>()
                                                            ->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::RIGHT);
            
            previewObjs[(int)ssGUI::Enums::DockSide::RIGHT] ->GetExtension<ssGUI::Extensions::AdvancedSize>()
                                                            ->SetSizingPercentage(0.5, 1);

            previewObjs[(int)ssGUI::Enums::DockSide::BOTTOM]->GetExtension<ssGUI::Extensions::AdvancedPosition>()
                                                            ->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::BOTTOM);
            
            previewObjs[(int)ssGUI::Enums::DockSide::BOTTOM]->GetExtension<ssGUI::Extensions::AdvancedSize>()
                                                            ->SetSizingPercentage(1, 0.5);
            
            previewObjs[(int)ssGUI::Enums::DockSide::LEFT] ->GetExtension<ssGUI::Extensions::AdvancedPosition>()
                                                            ->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::LEFT);
            
            previewObjs[(int)ssGUI::Enums::DockSide::LEFT] ->GetExtension<ssGUI::Extensions::AdvancedSize>()
                                                            ->SetSizingPercentage(0.5, 1);
        }
        
        if(DockTrigger[0] < 0 || CurrentObjectsReferences.GetObjectReference(DockTrigger[0]) == nullptr)
        {
            for(int i = 0; i < 4; ++i)
            {
                auto* triggerObj = Container->AddChild<ssGUI::Widget>(true);
                triggerObj->SetUserCreated(false);
                triggerObj->SetInteractable(false);
                triggerObj->SetBlockInput(false);
                triggerObj->AddExtension<ssGUI::Extensions::AdvancedPosition>();
                triggerObj->AddExtension<ssGUI::Extensions::AdvancedSize>();
                triggerObj->AddTag(ssGUI::Tags::FLOATING);
                triggerObj->SetEnabled(false);
                DockTrigger[i] = CurrentObjectsReferences.AddObjectReference(triggerObj);
            }
        }
    }
    
    void DockableV2::DrawPreview(ssGUI::Enums::DockSide side)
    {
        for(int i = 0; i < 4; ++i)
        {
            auto* dockPreview = CurrentObjectsReferences.GetObjectReference<ssGUI::Widget>(DockPreview[i]);
            if(i == (int)side)
            {
                dockPreview->SetEnabled(true);
                dockPreview->SetBackgroundColor(PreviewColor);
                dockPreview->SetEnabled(true);
            }
            else
                dockPreview->SetEnabled(false);
        }
    }
    
    void DockableV2::DiscardPreview()
    {
        for(int i = 0; i < 4; ++i)
            CurrentObjectsReferences.GetObjectReference(DockPreview[i])->SetEnabled(false);
    }
    
    void DockableV2::DrawTriggerAreas()
    {
        ssGUI::Widget* triggerObjs[4];
        
        for(int i = 0; i < 4; ++i)
        {
            triggerObjs[i] = CurrentObjectsReferences.GetObjectReference<ssGUI::Widget>(DockTrigger[i]);
            triggerObjs[i]->SetEnabled(true);
            triggerObjs[i]->SetBackgroundColor(TriggerColor);
        }

        float widthPercentage = GetTriggerSizePercentage().x;
        float lengthPercentage = GetTriggerSizePercentage().y;
        
        float widthPixels = GetTriggerSizePixels().x;
        float lengthPixels = GetTriggerSizePixels().y;

        triggerObjs[(int)ssGUI::Enums::DockSide::TOP]   ->GetExtension<ssGUI::Extensions::AdvancedPosition>()
                                                        ->SetAlignment( ssGUI::Enums::AlignmentHorizontal::CENTER,
                                                                        ssGUI::Enums::AlignmentVertical::TOP);
        
        triggerObjs[(int)ssGUI::Enums::DockSide::RIGHT] ->GetExtension<ssGUI::Extensions::AdvancedPosition>()
                                                        ->SetAlignment( ssGUI::Enums::AlignmentHorizontal::RIGHT,
                                                                        ssGUI::Enums::AlignmentVertical::CENTER);
                                                                        
        triggerObjs[(int)ssGUI::Enums::DockSide::BOTTOM]->GetExtension<ssGUI::Extensions::AdvancedPosition>()
                                                        ->SetAlignment( ssGUI::Enums::AlignmentHorizontal::CENTER, 
                                                                        ssGUI::Enums::AlignmentVertical::BOTTOM);
                        
        triggerObjs[(int)ssGUI::Enums::DockSide::LEFT] ->GetExtension<ssGUI::Extensions::AdvancedPosition>()
                                                        ->SetAlignment( ssGUI::Enums::AlignmentHorizontal::LEFT,
                                                                        ssGUI::Enums::AlignmentVertical::CENTER);
                
        for(int i = 0; i < 4; ++i)
        {
            if(i == (int)ssGUI::Enums::DockSide::TOP || i == (int)ssGUI::Enums::DockSide::BOTTOM)
            {
                triggerObjs[i]  ->GetExtension<ssGUI::Extensions::AdvancedPosition>()
                                ->SetAlignmentPercentage(0, GetTriggerMarginPercentage());

                triggerObjs[i]  ->GetExtension<ssGUI::Extensions::AdvancedPosition>()
                                ->SetAlignmentPixel(0, GetTriggerMarginPixels());

                triggerObjs[i]  ->GetExtension<ssGUI::Extensions::AdvancedSize>()
                                ->SetSizingPercentage(lengthPercentage, widthPercentage);
                
                triggerObjs[i]  ->GetExtension<ssGUI::Extensions::AdvancedSize>()
                                ->SetSizingPixel(lengthPixels, widthPixels);
            }
            else
            {
                triggerObjs[i]  ->GetExtension<ssGUI::Extensions::AdvancedPosition>()
                                ->SetAlignmentPercentage(GetTriggerMarginPercentage(), 0);

                triggerObjs[i]  ->GetExtension<ssGUI::Extensions::AdvancedPosition>()
                                ->SetAlignmentPixel(GetTriggerMarginPixels(), 0);
            
                triggerObjs[i]  ->GetExtension<ssGUI::Extensions::AdvancedSize>()
                                ->SetSizingPercentage(widthPercentage, lengthPercentage);

                triggerObjs[i]  ->GetExtension<ssGUI::Extensions::AdvancedSize>()
                                ->SetSizingPixel(widthPixels, lengthPixels);
            }
        }                                                                        
    }
    
    void DockableV2::DiscardTriggerAreas()
    {
        for(int i = 0; i < 4; ++i)
            CurrentObjectsReferences.GetObjectReference(DockTrigger[i])->SetEnabled(false);
    }
    
    void DockableV2::AttemptUndockingFromGUI(   ssGUI::Window* containerWindow, 
                                                ssGUI::MainWindow* mainWindow,
                                                ssGUI::InputStatus& lastInputStatus)
    {
        //If we are attempting to undock the window
        if(!AttemptToUndock)
        {
            //If the container is already got docked this frame or last frame, don't undock
            if(DockedSinceLastUpdate)
                return;
            
            //Remember and try undocking next frame
            AttemptToUndock = true;
            //ssLOG_LINE("Last Parent for " << containerWindow << " is " << LastParent);
            return;
        }
        
        //We are now on the next frame, try to undock
        AttemptToUndock = false;
        
        //If the container is already got docked this frame or last frame, don't undock
        if(DockedSinceLastUpdate)
            return;
        
        //Check if the user's cursor is on the container, if so just don't undock
        if(lastInputStatus.MouseInputBlockedData.GetBlockDataType() == ssGUI::Enums::BlockDataType::GUI_OBJECT)
        {
            auto* currentCheckObj = lastInputStatus.MouseInputBlockedData.GetBlockData<ssGUI::GUIObject>();
            
            while(currentCheckObj != nullptr)
            {
                if(currentCheckObj == containerWindow)
                    return;
                
                currentCheckObj = currentCheckObj->GetParent();
            }
        }
        
        //Undocking
        glm::vec2 posBeforeUndocking = containerWindow->GetGlobalPosition();
        
        if(CurrentObjectsReferences.GetObjectReference(TopLevelParent) == nullptr)
            containerWindow->SetParent(mainWindow);
        else
            containerWindow->SetParent(CurrentObjectsReferences.GetObjectReference(TopLevelParent));
        
        containerWindow->SetGlobalPosition(posBeforeUndocking + LastDraggedDistance);
        LastDraggedDistance = glm::vec2();
    }

    bool DockableV2::IsAttemptingToUndockFromGUI()
    {
        return AttemptToUndock;
    }

    void DockableV2::ResetState()
    {
        DiscardPreview(); 
        DiscardTriggerAreas();
        DockedSinceLastUpdate = false;
    }
    
    ssGUI::GUIObject* DockableV2::GetFirstDockingParent(ssGUI::GUIObject* startObject) const
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
    
    void DockableV2::NotifyBeforeDockingEvent(ssGUI::GUIObject* externalObject, ssGUI::Enums::DockSide dockSide, bool& isAbort)
    {
        ssGUI::DockEventInfo eventInfo;
        eventInfo.CurrentDockSide = dockSide;
        
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
    
    void DockableV2::NotifyDockedEvent(ssGUI::GUIObject* externalObject, ssGUI::Enums::DockSide dockSide)
    {
        ssGUI::DockEventInfo eventInfo;
        eventInfo.CurrentDockSide = dockSide;
        
        if(Container->IsEventCallbackExist(ssGUI::Enums::EventType::EXTERNAL_OBJECT_DOCKED))
            Container->GetEventCallback(ssGUI::Enums::EventType::EXTERNAL_OBJECT_DOCKED)->Notify(externalObject, &eventInfo);
        
        if(externalObject->IsEventCallbackExist(ssGUI::Enums::EventType::OBJECT_DOCKED))
            externalObject->GetEventCallback(ssGUI::Enums::EventType::OBJECT_DOCKED)->Notify(externalObject, &eventInfo);
    }
    
    const std::string DockableV2::EXTENSION_NAME = "Dockable V2";
    
    const std::string DockableV2::ListenerKey = "Dockable V2";
    
    void DockableV2::SetTriggerSizePercentage(glm::vec2 widthLength)
    {
        TriggerSizePercentage = widthLength;
    }
            
    glm::vec2 DockableV2::GetTriggerSizePercentage() const
    {
        return TriggerSizePercentage;
    }
            
    void DockableV2::SetTriggerSizePixels(glm::ivec2 widthLength)
    {
        TriggerSizePixels = widthLength;
    }
            
    glm::ivec2 DockableV2::GetTriggerSizePixels() const
    {
        return TriggerSizePixels;
    }
            
    void DockableV2::SetTriggerColor(glm::u8vec4 color)
    {
        TriggerColor = color;
    }
            
    glm::u8vec4 DockableV2::GetTriggerColor() const
    {
        return TriggerColor;
    }
    
    void DockableV2::SetTriggerMarginPercentage(float marginFromEdge)
    {
        TriggerMarginPercentage = marginFromEdge;
    }
    
    float DockableV2::GetTriggerMarginPercentage() const
    {
        return TriggerMarginPercentage;
    }
    
    void DockableV2::SetTriggerMarginPixels(int marginFromEdge)
    {
        TriggerMarginPixels = marginFromEdge;
    }
    
    int DockableV2::GetTriggerMarginPixels() const
    {
        return TriggerMarginPixels;
    }
            
    void DockableV2::SetDockPreviewColor(glm::u8vec4 color)
    {
        PreviewColor = color;
    }
            
    glm::u8vec4 DockableV2::GetDockPreviewColor() const
    {
        return PreviewColor;
    }
    
    void DockableV2::DockExternalObject(ssGUI::GUIObject* externalObject, ssGUI::Enums::DockSide side)
    {    
        //The container should be docked and has a parent that has docking area and layout
        if( Container == nullptr || 
            Container->GetParent() == nullptr || 
            !Container->GetParent()->IsExtensionExist<ssGUI::Extensions::DockingArea>() ||
            !Container->GetParent()->IsExtensionExist<ssGUI::Extensions::Layout>())
        {
            return;
        }

        //Check if there's more than one docked children
        bool isMoreThanOneDockedChild = false;
        
        std::vector<ssGUI::GUIObject*> children = Container->GetParent()->GetListOfChildren();
        int dockedChildrenCount = 0;
        for(int i = 0; i < children.size(); ++i)
        {
            bool isDocking =    (children.at(i)->IsExtensionExist<ssGUI::Extensions::DockableV2>() ||
                                children.at(i)->IsExtensionExist<ssGUI::Extensions::DockingArea>());
        
            if( isDocking &&
                !children.at(i)->HasTag(ssGUI::Tags::FLOATING) &&
                !children.at(i)->HasTag(ssGUI::Tags::OVERLAY))
            {
                dockedChildrenCount++;
                
                if(dockedChildrenCount >= 2)
                {
                    isMoreThanOneDockedChild = true;
                    break;
                }
            }
        }
        
        //Notify event callbacks before getting docked
        bool isAbort = false;
        NotifyBeforeDockingEvent(externalObject, side, isAbort);
        if(isAbort)
            return;
        
        glm::vec2 originalSize = Container->GetSize();
        
        //If we are the only child
        if(!isMoreThanOneDockedChild)
        {
            //Change the orientation of layout
            ssGUI::Extensions::Layout* parentLayout = Container->GetParent()->GetExtension<ssGUI::Extensions::Layout>();
            if( (side == ssGUI::Enums::DockSide::LEFT || side == ssGUI::Enums::DockSide::RIGHT) && 
                !parentLayout->IsHorizontalLayout())
            {
                parentLayout->SetHorizontalLayout(true);
            }
            else if((side == ssGUI::Enums::DockSide::TOP || side == ssGUI::Enums::DockSide::BOTTOM) &&
                    parentLayout->IsHorizontalLayout())
            {
                parentLayout->SetHorizontalLayout(false);
            }
            
            //Append or prepend the incoming object depending on the dock side and is layout reversed
            bool isInsertBefore = side == ssGUI::Enums::DockSide::LEFT || side == ssGUI::Enums::DockSide::TOP;
            
            isInsertBefore =    Container->GetParent()->GetExtension<ssGUI::Extensions::Layout>()->IsReverseOrder() ?
                                !isInsertBefore :
                                isInsertBefore;
            
            externalObject->SetParent(Container->GetParent());
            
            if(isInsertBefore)
                Container->GetParent()->MoveChildBeforeTargetChild(externalObject, Container);
        }
        //Otherwise, if we are not the only child
        else
        {
            ssGUI::Extensions::Layout* parentLayout = Container->GetParent()->GetExtension<ssGUI::Extensions::Layout>();
            bool horizontalAligned =    (side == ssGUI::Enums::DockSide::LEFT || side == ssGUI::Enums::DockSide::RIGHT) && 
                                        parentLayout->IsHorizontalLayout();
            bool verticalAligned =  (side == ssGUI::Enums::DockSide::TOP || side == ssGUI::Enums::DockSide::BOTTOM) &&
                                    !parentLayout->IsHorizontalLayout();
            
            //If the orientation of layout matches the dock side direction
            if(horizontalAligned || verticalAligned)
            {
                //Insert before or after if they match
                bool isInsertBefore = side == ssGUI::Enums::DockSide::LEFT || side == ssGUI::Enums::DockSide::TOP;
            
                isInsertBefore =    Container->GetParent()->GetExtension<ssGUI::Extensions::Layout>()->IsReverseOrder() ?
                                    !isInsertBefore :
                                    isInsertBefore;
                
                externalObject->SetParent(Container->GetParent());
            
                if(isInsertBefore)
                    Container->GetParent()->MoveChildBeforeTargetChild(externalObject, Container);
                else
                    Container->GetParent()->MoveChildAfterTargetChild(externalObject, Container);
            }
            //If they don't match
            else
            {
                //Create an empty parent with docking area and with the correct orientation
                ssGUI::GUIObject* emptyParent = CurrentObjectsReferences.GetObjectReference(DefaultNewParent);
                if(emptyParent == nullptr)
                {
                    emptyParent = Container->GetParent()->AddChildAfter<ssGUI::Window>(Container);
                    
                    auto* emptyWindow = static_cast<ssGUI::Window*>(emptyParent);
                    emptyWindow->SetTitlebar(false);
                    emptyWindow->SetBackgroundColor(glm::u8vec4(0,0,0,0));
                    emptyWindow->RemoveExtension<ssGUI::Extensions::Border>();
                    emptyWindow->SetUserCreated(false);
                }
                
                //Copy parent non user created DockingArea extension if possible
                if(DefaultDockingArea != nullptr)
                    emptyParent->AddExtensionCopy(DefaultDockingArea);
                else
                    emptyParent->AddExtensionCopy(Container->GetParent()->GetExtension<ssGUI::Extensions::DockingArea>());

                auto* layout = emptyParent->GetExtension<ssGUI::Extensions::Layout>();
                
                if(side == ssGUI::Enums::DockSide::LEFT || side == ssGUI::Enums::DockSide::RIGHT)
                    layout->SetHorizontalLayout(true);
                else
                    layout->SetHorizontalLayout(false);
                
                //Moving Container and the incoming object to this parent
                if(side == ssGUI::Enums::DockSide::LEFT || side == ssGUI::Enums::DockSide::TOP)
                {
                    externalObject->SetParent(emptyParent);
                    Container->SetParent(emptyParent);
                }
                else
                {
                    Container->SetParent(emptyParent);
                    externalObject->SetParent(emptyParent);
                }
                
                emptyParent->SetSize(originalSize);
            }
        }
        
        //Set the size of container and docked object
        ssGUI::Extensions::Layout* parentLayout = Container->GetParent()->GetExtension<ssGUI::Extensions::Layout>();
        glm::vec2 childSize;
        if(side == ssGUI::Enums::DockSide::LEFT || side == ssGUI::Enums::DockSide::RIGHT)
            childSize = glm::vec2((originalSize.x - parentLayout->GetSpacing()) / 2, originalSize.y);
        else
            childSize = glm::vec2(originalSize.x, (originalSize.y - parentLayout->GetSpacing()) / 2);
        
        Container->SetSize(childSize);
        externalObject->SetSize(childSize);
        
        //Notify event callbacks for getting docked
        NotifyDockedEvent(externalObject, side);
    }
    
    void DockableV2::SetTopLevelParent(ssGUI::GUIObject* topLevelParent)
    {
        if(topLevelParent == nullptr)
        {
            TopLevelParent = -1;
            return;
        }
        
        TopLevelParent = CurrentObjectsReferences.AddObjectReference(topLevelParent);
    }
    
    ssGUI::GUIObject* DockableV2::GetTopLevelParent() const
    {
        return CurrentObjectsReferences.GetObjectReference(TopLevelParent);
    }
    
    void DockableV2::SetUndockable(bool undockable)
    {
        Undockable = undockable;
    }
    
    bool DockableV2::IsUndockable() const
    {
        return Undockable;
    }
    
    void DockableV2::AddDefaultDockingArea(ssGUI::Extensions::DockingArea* dockingArea)
    {
        DefaultDockingArea = dockingArea;
    }
    
    ssGUI::Extensions::DockingArea* DockableV2::GetDefaultDockingArea() const
    {
        return DefaultDockingArea;
    }
    
    void DockableV2::AddDefaultNewParent(ssGUI::GUIObject* defaultNewParent)
    {
        if(defaultNewParent == nullptr)
            DefaultNewParent = -1;
    
        DefaultNewParent = CurrentObjectsReferences.AddObjectReference(defaultNewParent);
    }
    
    ssGUI::GUIObject* DockableV2::GetDefaultNewParent() const
    {
        return CurrentObjectsReferences.GetObjectReference(DefaultNewParent);
    }
    
    bool DockableV2::IsEnabled() const
    {
        return Enabled;
    }
    
    void DockableV2::SetEnabled(bool enabled)
    {
        Enabled = enabled;
    }
    
    void DockableV2::Internal_Update(bool isPreUpdate, 
                                            ssGUI::Backend::SystemInputInterface* inputInterface, 
                                            ssGUI::InputStatus& currentInputStatus, 
                                            ssGUI::InputStatus& lastInputStatus, 
                                            ssGUI::GUIObject* mainWindow)
    {
        ssGUI_LOG_FUNC();
        
        #define DISCARD_AND_RETURN() do { ResetState(); return; } while(0)
        
        //This is function is executed twice, one before the Container GUI object update and one after.
        //You can use the isPreUpdate variable to decide when to execute the extension update
        if(isPreUpdate || Container == nullptr || Container->GetParent() == nullptr || !Enabled)
            return;
        
        if(Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW)
        {
            auto* window = static_cast<ssGUI::Window*>(Container);
            
            //If the window is being dragged / finished being dragged, set the input status
            if( window->GetWindowDragState() == ssGUI::Enums::WindowDragState::DRAGGING || 
                window->GetWindowDragState() == ssGUI::Enums::WindowDragState::ENDED)
            {
                if(currentInputStatus.CurrentDragData.GetDragDataType() == ssGUI::Enums::DragDataType::NONE)
                    currentInputStatus.CurrentDragData.SetDragData(window);
                
                //If the user is trying to redock or undock the window
                if(Container->GetParent()->IsExtensionExist<ssGUI::Extensions::DockingArea>())
                {
                    //Show preview or trigger when being dragged
                    if(window->GetWindowDragState() == ssGUI::Enums::WindowDragState::DRAGGING)
                    {
                        inputInterface->SetCursorType(ssGUI::Enums::CursorType::MOVE);
                        ssGUI::WindowResizeDragData data = window->GetResizeDragData();
                        LastDraggedDistance = data.TransformTotalMovedDistance;
                        //LastParent = CurrentObjectsReferences.AddObjectReference(window->GetParent());
                    }
                    //Check if we want to undock or let it be docked to something else
                    else if(Undockable)
                        AttemptUndockingFromGUI(window, static_cast<ssGUI::MainWindow*>(mainWindow), lastInputStatus);
                }
            
                DISCARD_AND_RETURN();
            }
            //Evaluate if we can undock it or not
            else if(IsAttemptingToUndockFromGUI())
            {
                AttemptUndockingFromGUI(window, static_cast<ssGUI::MainWindow*>(mainWindow), lastInputStatus);
                DISCARD_AND_RETURN();
            }
        }
        
        //If Container is being docked by external dockable
        
        //First check if we can be docked or not
        if( Container->GetParent() == nullptr || 
            !Container->GetParent()->IsExtensionExist<ssGUI::Extensions::DockingArea>())
        {
            DISCARD_AND_RETURN();
        }
        
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
        
        //Account for window titlebar
        glm::vec2 containerContentTopLeft = Container->GetGlobalPosition();
        glm::vec2 containerContentSize = Container->GetSize();
        
        if(Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW)
        {
            containerContentTopLeft.y += static_cast<ssGUI::Window*>(Container)->GetTitlebarHeight();
            containerContentSize.y -= static_cast<ssGUI::Window*>(Container)->GetTitlebarHeight();
        }
        
        glm::vec2 containerCenter = containerContentTopLeft + containerContentSize / 2.f;
        
        float widthPercentage = GetTriggerSizePercentage().x;
        float lengthPercentage = GetTriggerSizePercentage().y;
        
        float widthPixels = GetTriggerSizePixels().x;
        float lengthPixels = GetTriggerSizePixels().y;
        
        int showPreview = -1;
        bool showTrigger = false;
        int dockObj = -1;
        
        //For each trigger area
        for(int i = 0; i < 4; ++i)
        {
            glm::vec2 triggerSize;
            glm::vec2 triggerMin;    
            glm::vec2 triggerMax;
            glm::vec2 triggerHalfSize;
            float marginOffset;
            
            //Get the bound of the trigger area
            switch(i)
            {
                case (int)ssGUI::Enums::DockSide::TOP:
                    triggerSize = containerContentSize * glm::vec2(lengthPercentage, widthPercentage) + glm::vec2(lengthPixels, widthPixels);
                    triggerHalfSize = triggerSize / 2.f;
                    triggerMin = glm::vec2(containerCenter.x - triggerHalfSize.x, containerContentTopLeft.y);
                    triggerMax = glm::vec2(containerCenter.x + triggerHalfSize.x, containerContentTopLeft.y + triggerSize.y);
                    
                    marginOffset = GetTriggerMarginPercentage() * containerContentSize.y + GetTriggerMarginPixels();
                    triggerMin.y += marginOffset;
                    triggerMax.y += marginOffset;
                    break;
                
                case (int)ssGUI::Enums::DockSide::RIGHT:
                    triggerSize = containerContentSize * glm::vec2(widthPercentage, lengthPercentage) + glm::vec2(widthPixels, lengthPixels);
                    triggerHalfSize = triggerSize / 2.f;
                    triggerMin = glm::vec2(containerMaxBound.x - triggerSize.x, containerCenter.y - triggerHalfSize.y);
                    triggerMax = glm::vec2(containerMaxBound.x, containerCenter.y + triggerHalfSize.y);
                    
                    marginOffset = GetTriggerMarginPercentage() * containerContentSize.x + GetTriggerMarginPixels();
                    triggerMin.x -= marginOffset;
                    triggerMax.x -= marginOffset;
                    break;
                    
                case (int)ssGUI::Enums::DockSide::BOTTOM:
                    triggerSize = containerContentSize * glm::vec2(lengthPercentage, widthPercentage) + glm::vec2(lengthPixels, widthPixels);
                    triggerHalfSize = triggerSize / 2.f;
                    triggerMin = glm::vec2(containerCenter.x - triggerHalfSize.x, containerMaxBound.y - triggerSize.y);
                    triggerMax = glm::vec2(containerCenter.x + triggerHalfSize.x, containerMaxBound.y);
                    
                    marginOffset = GetTriggerMarginPercentage() * containerContentSize.y + GetTriggerMarginPixels();
                    triggerMin.y -= marginOffset;
                    triggerMax.y -= marginOffset;
                    break;
                
                case (int)ssGUI::Enums::DockSide::LEFT:
                    triggerSize = containerContentSize * glm::vec2(widthPercentage, lengthPercentage) + glm::vec2(widthPixels, lengthPixels);
                    triggerHalfSize = triggerSize / 2.f;
                    triggerMin = glm::vec2(containerContentTopLeft.x, containerCenter.y - triggerHalfSize.y);
                    triggerMax = glm::vec2(containerContentTopLeft.x + triggerSize.x, containerCenter.y + triggerHalfSize.y);
                    
                    marginOffset = GetTriggerMarginPercentage() * containerContentSize.x + GetTriggerMarginPixels();
                    triggerMin.x += marginOffset;
                    triggerMax.x += marginOffset;
                    break;
            }

            //Check if the mouse is inside the trigger area
            if( mousePos.x >= triggerMin.x && 
                mousePos.x < triggerMax.x &&
                mousePos.y >= triggerMin.y && 
                mousePos.y < triggerMax.y)
            {
                //If so, show the preview if mouse held
                if(inputInterface->IsInputExistCurrentFrame(ssGUI::Enums::MouseButton::LEFT))
                    showPreview = i;
                //Otherwise, dock the object if mouse up
                else if(inputInterface->IsButtonOrKeyUp(ssGUI::Enums::MouseButton::LEFT))
                    dockObj = i;
            }
            //Otherwise, show the trigger area
            else
                showTrigger = true;
        }
        
        //Perform corresponding action
        if(dockObj != -1)
        {
            lastInputStatus.CurrentDragData.SetInterceptor(Container);
            DockExternalObject(lastInputStatus.CurrentDragData.GetDragData<ssGUI::GUIObject>(), static_cast<ssGUI::Enums::DockSide>(dockObj));
            DiscardPreview();
            DiscardTriggerAreas();
        }
        else if(showPreview != -1)
        {
            DrawPreview(static_cast<ssGUI::Enums::DockSide>(showPreview));
            DiscardTriggerAreas();
        }
        else if(showTrigger)
        {
            DrawTriggerAreas();
            DiscardPreview();
        }
    }

    void DockableV2::Internal_Draw(  bool isPreRender, 
                                            ssGUI::Backend::DrawingInterface* drawingInterface, 
                                            ssGUI::GUIObject* mainWindow, 
                                            glm::vec2 mainWindowPositionOffset)
    {
    }

    std::string DockableV2::GetExtensionName() const
    {
        return EXTENSION_NAME;
    }

    void DockableV2::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;
        CreateDockPreviewAndTrigger();
        Container->AddEventCallback(ssGUI::Enums::EventType::OBJECT_DOCKED)->AddEventListener
        (
            ListenerKey,
            Container,
            [](ssGUI::EventInfo& info)
            {
                if(!info.Container->IsExtensionExist<ssGUI::Extensions::DockableV2>())
                {
                    info.DeleteCurrentListener = true;
                    return;
                }
                
                info.Container->GetExtension<ssGUI::Extensions::DockableV2>()->DockedSinceLastUpdate = true;
            }
        );
    }

    void DockableV2::Copy(const ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        
        auto* original = static_cast<const ssGUI::Extensions::DockableV2*>(extension);
        Enabled = original->IsEnabled();
        
        Undockable = original->Undockable;
        CurrentObjectsReferences = original->CurrentObjectsReferences;
        
        for(int i = 0; i < 4; ++i)
        {
            DockPreview[i] = original->DockPreview[i];
            DockTrigger[i] = original->DockTrigger[i];
        }
        
        TriggerMarginPercentage = original->TriggerMarginPercentage;
        TriggerMarginPixels = original->TriggerMarginPixels;
        TriggerSizePercentage = original->TriggerSizePercentage;
        TriggerSizePixels = original->TriggerSizePixels;
        TriggerColor = original->TriggerColor;
        PreviewColor = original->PreviewColor;
        TopLevelParent = original->TopLevelParent;
        LastDraggedDistance = original->LastDraggedDistance;
        DockedSinceLastUpdate = original->DockedSinceLastUpdate;
        AttemptToUndock = original->AttemptToUndock;
        DefaultDockingArea = original->DefaultDockingArea;
        DefaultNewParent = original->DefaultNewParent;
    }

    ObjectsReferences* DockableV2::Internal_GetObjectsReferences()
    {
        return nullptr;
    }

    DockableV2* DockableV2::Clone()
    {
        DockableV2* temp = new DockableV2(*this);
        return temp;
    }
}

}