#include "ssGUI/Extensions/MaskEnforcer.hpp"
#include "ssGUI/Extensions/Legacy/Dockable.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

#include <algorithm>

namespace ssGUI
{

namespace Extensions
{
    void MaskEnforcer::ApplyEventCallbacksToMasks()
    {
        if(Container != nullptr)
        {
            for(int i = 0; i < MasksToAddEventCallbacks.size(); ++i)
            {
                ssGUI::GUIObject* maskObject = CurrentObjectsReferences.GetObjectReference(MasksToAddEventCallbacks.at(i));
                if(maskObject != nullptr)
                    AddBeforeRenderEventCallback(maskObject);
            }
            
            MasksToAddEventCallbacks.clear();
        }    
    }

    void MaskEnforcer::AddBeforeRenderEventCallback(ssGUI::GUIObject* maskObject)
    {
        auto beforeRenderEvent = maskObject->AddEventCallback(ssGUI::Enums::EventType::BEFORE_OBJECT_RENDER);
        ssGUIObjectIndex enforcerContainerIndex = beforeRenderEvent->AddObjectReference(Container);
        
        beforeRenderEvent->AddEventListener
        (
            ListenerKey,
            Container,
            [enforcerContainerIndex](ssGUI::EventInfo& info)
            {
                ssGUI::GUIObject* enforcerContainer = info.References->GetObjectReference(enforcerContainerIndex);
                
                //Check if all the things we need are still here
                if( enforcerContainer == nullptr || 
                    !enforcerContainer->IsExtensionExist<ssGUI::Extensions::MaskEnforcer>() ||
                    !info.Container->IsExtensionExist<ssGUI::Extensions::Mask>())
                {
                    info.DeleteCurrentListener = true;
                    return;
                }
                
                //Trigger enforocer
                auto* maskEnforcer = enforcerContainer->GetExtension<ssGUI::Extensions::MaskEnforcer>();
                ssGUIObjectIndex maskIndex = maskEnforcer->CurrentObjectsReferences.GetObjectIndex(info.Container);
                
                //Check if maskEnforcer still has reference the mask, if not exit.
                if(maskIndex < 0)
                {
                    info.DeleteCurrentListener = true;
                    return;
                }
                
                if(info.Container->IsRedrawNeeded())
                    enforcerContainer->RedrawObject();
            }
        );
    }

    MaskEnforcer::MaskEnforcer() :  Container(nullptr),
                                    Enabled(true),
                                    TargetMasks(),
                                    MasksToAddEventCallbacks(),
                                    BlockingContainerInput(false),
                                    CurrentObjectsReferences()
    {}

    MaskEnforcer::~MaskEnforcer()
    {
        CurrentObjectsReferences.CleanUp();
    }
    
    MaskEnforcer::MaskEnforcer(MaskEnforcer const& other)
    {
        Container = nullptr;
        Copy(&other);
        
        //TODO: Check if this is needed
        ApplyEventCallbacksToMasks();
    }

    void MaskEnforcer::ConstructRenderInfo()
    {
        ssGUI_LOG_FUNC();

        for(auto it = TargetMasks.begin(); it != TargetMasks.end(); it++)
        {
            ssGUI::GUIObject* curMaskObj = CurrentObjectsReferences.GetObjectReference(it->first);
            if(curMaskObj == nullptr || !curMaskObj->IsExtensionExist<ssGUI::Extensions::Mask>())
            {
                continue;
            }

            if(!curMaskObj->GetExtension<ssGUI::Extensions::Mask>()->IsEnabled())
                continue;

            //Sepcial check to not mask any currently docking window that has TopLevelParent set to higher than the object that has the mask extension
            if(Container->IsExtensionExist<ssGUI::Extensions::Dockable>())
            {
                auto dockableEx = Container->GetExtension<ssGUI::Extensions::Dockable>();
                if(dockableEx->IsCurrentlyDocking())
                {
                    ssGUI::GUIObject* topLevelParent =  dockableEx->GetTopLevelParent();
                    if(topLevelParent == nullptr && curMaskObj->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
                        continue;
                    else if(topLevelParent != nullptr)
                    {
                        ssGUI::GUIObject* curParent = curMaskObj;
                        bool topLevelParentUnderCurMaskObj = true;
                        while(curParent != nullptr) 
                        {
                            if(curParent != topLevelParent)
                                curParent = curParent->GetParent();
                            else
                            {
                                topLevelParentUnderCurMaskObj = false;
                                break;
                            }
                        }

                        if(topLevelParentUnderCurMaskObj)
                            continue;
                    }
                }
            }

            ssGUI::Extensions::Mask* currentMask = CurrentObjectsReferences .GetObjectReference(it->first)
                                                                            ->GetExtension<ssGUI::Extensions::Mask>();

            currentMask->MaskObject(Container, glm::vec2(), it->second);
        }
    }

    void MaskEnforcer::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        ConstructRenderInfo();
    }
    
    const std::string MaskEnforcer::EXTENSION_NAME = "Mask Enforcer";
    const std::string MaskEnforcer::ListenerKey = "Mask Enforcer";
        
    void MaskEnforcer::AddTargetMaskObject(ssGUI::GUIObject* targetMaskObj)
    {        
        AddTargetMaskObject(targetMaskObj, std::vector<ssGUI::TargetShape>());
    }

    void MaskEnforcer::AddTargetMaskObject(ssGUI::GUIObject* targetMaskObj, const std::vector<ssGUI::TargetShape>& targetShapes)
    {
        ssGUI_LOG_FUNC();
        
        ssGUIObjectIndex maskObjIndex = CurrentObjectsReferences.AddObjectReference(targetMaskObj);
        if(TargetMasks.find(maskObjIndex) == TargetMasks.end())
            TargetMasks.insert({maskObjIndex, targetShapes});

        if(Container == nullptr)
        {
            //ssGUI_ERROR(ssGUI_EXT_TAG, "You need to bind a container first before adding target mask object");
            MasksToAddEventCallbacks.push_back(maskObjIndex);
            return;
        }

        AddBeforeRenderEventCallback(targetMaskObj);
        Container->RedrawObject();
    }

    bool MaskEnforcer::HasTargetMaskObject(ssGUI::GUIObject* targetMaskObj)
    {
        ssGUIObjectIndex index = CurrentObjectsReferences.GetObjectIndex(targetMaskObj);
        if(index == -1)
            return false;
    
        return TargetMasks.find(index) != TargetMasks.end();
    }

    void MaskEnforcer::RemoveTargetMaskObject(ssGUI::GUIObject* targetMaskObj)
    {
        ssGUIObjectIndex index = CurrentObjectsReferences.GetObjectIndex(targetMaskObj);
        if(index != -1)
        {
            CurrentObjectsReferences.RemoveObjectReference(index);
            return;
        }

        if(TargetMasks.find(index) != TargetMasks.end())
        {
            TargetMasks.erase(index);
            auto foundIt = std::find(MasksToAddEventCallbacks.begin(), MasksToAddEventCallbacks.end(), index);
            if(foundIt != MasksToAddEventCallbacks.end())
                MasksToAddEventCallbacks.erase(foundIt);
            
            if(Container != nullptr && targetMaskObj->IsEventCallbackExist(ssGUI::Enums::EventType::BEFORE_OBJECT_RENDER))
            {
                targetMaskObj   ->GetEventCallback(ssGUI::Enums::EventType::BEFORE_OBJECT_RENDER)
                                ->RemoveEventListener(ListenerKey, Container);
            }
        }

        if(Container != nullptr)
            Container->RedrawObject();
    }

    void MaskEnforcer::ChangeTargetShapeForMask(ssGUI::GUIObject* targetMaskObj, const std::vector<ssGUI::TargetShape>& targetShapes)
    {
        if(!CurrentObjectsReferences.IsObjectReferenceExist(targetMaskObj))
            return;

        ssGUIObjectIndex maskObjIndex = CurrentObjectsReferences.GetObjectIndex(targetMaskObj);
        if(TargetMasks.find(maskObjIndex) == TargetMasks.end())
            return;
        
        TargetMasks[maskObjIndex] = targetShapes;
    }

    std::vector<std::pair<ssGUI::GUIObject*, std::vector<ssGUI::TargetShape>>> MaskEnforcer::GetTargetMaskObjects()
    {
        auto returnVec = std::vector<std::pair<ssGUI::GUIObject*, std::vector<ssGUI::TargetShape>>>();

        for(auto it = TargetMasks.begin(); it != TargetMasks.end(); it++)
        {
            if( CurrentObjectsReferences.GetObjectReference(it->first) != nullptr && 
                CurrentObjectsReferences.GetObjectReference(it->first)->IsExtensionExist<ssGUI::Extensions::Mask>())
            {
                returnVec.push_back
                (
                    {
                        CurrentObjectsReferences.GetObjectReference(it->first),
                        it->second
                    }
                );
            }
        }

        return returnVec;
    }

    void MaskEnforcer::SetEnabled(bool enabled)
    {
        Enabled = enabled;
        if(Container != nullptr)
            Container->RedrawObject();
    }

    bool MaskEnforcer::IsEnabled() const
    {
        return Enabled;
    }
        
    //Extension methods
    void MaskEnforcer::Internal_Update( bool isPreUpdate, 
                                        ssGUI::Backend::BackendSystemInputInterface* inputInterface, 
                                        ssGUI::InputStatus& currentInputStatus, 
                                        ssGUI::InputStatus& lastInputStatus, 
                                        ssGUI::GUIObject* mainWindow)
    {
        ssGUI_LOG_FUNC();

        if(!Enabled || Container == nullptr)
            return;

        //Updating Target Masks and blocking any mouse input outside the mask
        if(isPreUpdate)
        {
            if( Container->GetExtensionDrawOrder<ssGUI::Extensions::MaskEnforcer>() != Container->GetExtensionsCount() - 1)
                Container->ChangeExtensionDrawOrder<ssGUI::Extensions::MaskEnforcer>(Container->GetExtensionsCount() - 1);

            std::vector<ssGUIObjectIndex> indicesToDelete = std::vector<ssGUIObjectIndex>();

            for(auto it = TargetMasks.begin(); it != TargetMasks.end(); it++)
            {
                if( CurrentObjectsReferences.GetObjectReference(it->first) == nullptr || 
                    !CurrentObjectsReferences.GetObjectReference(it->first)->IsExtensionExist<ssGUI::Extensions::Mask>())
                {
                    indicesToDelete.push_back(it->first);
                    continue;
                }
                
                if(!CurrentObjectsReferences.GetObjectReference(it->first)->GetExtension<ssGUI::Extensions::Mask>()->IsEnabled())
                    continue;

                if(currentInputStatus.MouseInputBlockedData.GetBlockDataType() != ssGUI::Enums::BlockDataType::NONE)                
                    continue;
                
                //If so, check if the cursor is inside the mask
                ssGUI::Extensions::Mask* maskExt = CurrentObjectsReferences.GetObjectReference(it->first)->GetExtension<ssGUI::Extensions::Mask>();
                glm::ivec2 currentMousePos = inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)->GetBackendWindowInterface());
                if(!maskExt->IsPointContainedInMask(currentMousePos))
                {
                    //If not, cut off the input
                    BlockingContainerInput = true;
                    currentInputStatus.MouseInputBlockedData.SetBlockData(Container);
                }  
            }

            if(!indicesToDelete.empty())
                Container->RedrawObject();

            for(int i = 0; i < indicesToDelete.size(); i++)
            {
                if(CurrentObjectsReferences.GetObjectReference(indicesToDelete[i]) != nullptr)
                    CurrentObjectsReferences.RemoveObjectReference(indicesToDelete[i]);
                
                TargetMasks.erase(indicesToDelete[i]);
            }
        }

        //Check if the mouse input is blocked, if so convert back so it doesn't affect other gui objects
        if(!isPreUpdate)
        {   
            if(BlockingContainerInput)
            {
                BlockingContainerInput = false;
                currentInputStatus.MouseInputBlockedData.UnsetBlockData();
            }
        }
    }

    void MaskEnforcer::Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {        
        ssGUI_LOG_FUNC();
        
        if(!Enabled || Container == nullptr || isPreRender)
            return;

        //ssLOG_LINE("Container: " << Container);
        if(Container->IsRedrawNeeded())
            ConstructRenderInfo();
    }

    std::string MaskEnforcer::GetExtensionName() const
    {
        return EXTENSION_NAME;
    }

    void MaskEnforcer::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;
        ApplyEventCallbacksToMasks();
    }

    void MaskEnforcer::Copy(const ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        
        auto* maskEnforcer = static_cast<const ssGUI::Extensions::MaskEnforcer*>(extension);
        
        Enabled = maskEnforcer->IsEnabled();
        TargetMasks = maskEnforcer->TargetMasks;
        MasksToAddEventCallbacks = maskEnforcer->MasksToAddEventCallbacks;
        BlockingContainerInput = maskEnforcer->BlockingContainerInput;
        CurrentObjectsReferences = maskEnforcer->CurrentObjectsReferences;
    }

    ObjectsReferences* MaskEnforcer::Internal_GetObjectsReferences()
    {
        return &CurrentObjectsReferences;
    }

    MaskEnforcer* MaskEnforcer::Clone()
    {
        MaskEnforcer* temp = new MaskEnforcer(*this);
        return temp;
    }
}

}