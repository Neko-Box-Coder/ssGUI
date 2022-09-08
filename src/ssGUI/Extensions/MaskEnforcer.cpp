#include "ssGUI/Extensions/MaskEnforcer.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

namespace ssGUI::Extensions
{
    MaskEnforcer::MaskEnforcer() : Container(nullptr), Enabled(true), TargetMasks(), BlockingContainerInput(false), CurrentObjectsReferences()
    {}

    MaskEnforcer::~MaskEnforcer()
    {
        CurrentObjectsReferences.CleanUp();
    }
    
    MaskEnforcer::MaskEnforcer(MaskEnforcer const& other)
    {
        Container = nullptr;
        Enabled = other.IsEnabled();
        TargetMasks = other.TargetMasks;
        BlockingContainerInput = false;

        CurrentObjectsReferences = other.CurrentObjectsReferences;
    }

    void MaskEnforcer::ConstructRenderInfo()
    {
        FUNC_DEBUG_ENTRY();

        for(auto it = TargetMasks.begin(); it != TargetMasks.end(); it++)
        {
            if(CurrentObjectsReferences.GetObjectReference(it->first) == nullptr || 
            !CurrentObjectsReferences.GetObjectReference(it->first)->IsExtensionExist(ssGUI::Extensions::Mask::EXTENSION_NAME))
            {
                continue;
            }

            if(!CurrentObjectsReferences.GetObjectReference(it->first)->GetExtension(ssGUI::Extensions::Mask::EXTENSION_NAME)->IsEnabled())
                continue;

            ssGUI::Extensions::Mask* currentMask = static_cast<ssGUI::Extensions::Mask*>(CurrentObjectsReferences.GetObjectReference(it->first)->
                GetExtension(ssGUI::Extensions::Mask::EXTENSION_NAME));

            currentMask->MaskObject(Container, /*- (mainWindowP->GetGlobalPosition() + mainWindowPositionOffset)*/glm::vec2(), it->second);
        }

        FUNC_DEBUG_EXIT();
    }

    void MaskEnforcer::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        ConstructRenderInfo();
    }
    
    const std::string MaskEnforcer::EXTENSION_NAME = "Mask Enforcer";
        
    void MaskEnforcer::AddTargetMaskObject(ssGUI::GUIObject* targetMaskObj)
    {        
        AddTargetMaskObject(targetMaskObj, std::vector<int>());
    }

    void MaskEnforcer::AddTargetMaskObject(ssGUI::GUIObject* targetMaskObj, std::vector<int> targetShapeIndices)
    {
        FUNC_DEBUG_ENTRY();
        if(CurrentObjectsReferences.IsObjectReferenceExist(targetMaskObj))
        {
            ssGUIObjectIndex maskObjIndex = CurrentObjectsReferences.GetObjectIndex(targetMaskObj);
            if(TargetMasks.find(maskObjIndex) == TargetMasks.end())
                TargetMasks.insert({maskObjIndex, targetShapeIndices});
        }
        else
        {
            ssGUIObjectIndex maskObjIndex = CurrentObjectsReferences.AddObjectReference(targetMaskObj);
            TargetMasks.insert({maskObjIndex, targetShapeIndices});
        }

        if(Container != nullptr)
            Container->RedrawObject();
        
        FUNC_DEBUG_EXIT();
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
            TargetMasks.erase(index);

        if(Container != nullptr)
            Container->RedrawObject();
    }

    void MaskEnforcer::ChangeTargetShapeForMask(ssGUI::GUIObject* targetMaskObj, std::vector<int> targetShapeIndices)
    {
        if(!CurrentObjectsReferences.IsObjectReferenceExist(targetMaskObj))
            return;

        ssGUIObjectIndex maskObjIndex = CurrentObjectsReferences.GetObjectIndex(targetMaskObj);
        if(TargetMasks.find(maskObjIndex) == TargetMasks.end())
            return;
        
        TargetMasks[maskObjIndex] = targetShapeIndices;
    }

    std::vector<std::pair<ssGUI::GUIObject*, std::vector<int>>> MaskEnforcer::GetTargetMaskObjects()
    {
        auto returnVec = std::vector<std::pair<ssGUI::GUIObject*, std::vector<int>>>();

        for(auto it = TargetMasks.begin(); it != TargetMasks.end(); it++)
        {
            if(CurrentObjectsReferences.GetObjectReference(it->first) != nullptr && 
                CurrentObjectsReferences.GetObjectReference(it->first)->IsExtensionExist(ssGUI::Extensions::Mask::EXTENSION_NAME))
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
    void MaskEnforcer::Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();

        if(!Enabled || Container == nullptr)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        //Updating Target Masks and blocking any mouse input outside the mask
        if(isPreUpdate)
        {
            if(Container->GetExtensionDrawOrder(GetExtensionName()) != Container->GetExtensionsCount() - 1)
                Container->ChangeExtensionDrawOrder(GetExtensionName(), Container->GetExtensionsCount() - 1);

            std::vector<ssGUIObjectIndex> indicesToDelete = std::vector<ssGUIObjectIndex>();

            for(auto it = TargetMasks.begin(); it != TargetMasks.end(); it++)
            {
                if(CurrentObjectsReferences.GetObjectReference(it->first) == nullptr || 
                    !CurrentObjectsReferences.GetObjectReference(it->first)->IsExtensionExist(ssGUI::Extensions::Mask::EXTENSION_NAME))
                {
                    indicesToDelete.push_back(it->first);
                    continue;
                }
                
                if(!CurrentObjectsReferences.GetObjectReference(it->first)->GetExtension(ssGUI::Extensions::Mask::EXTENSION_NAME)->IsEnabled())
                    continue;

                if(inputStatus.MouseInputBlockedObject != nullptr)                
                    continue;
                
                //If so, check if the cursor is inside the mask
                if(!static_cast<ssGUI::Extensions::Mask*>(CurrentObjectsReferences.GetObjectReference(it->first)->GetExtension(ssGUI::Extensions::Mask::EXTENSION_NAME))
                    ->IsPointContainedInMask(inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow))))
                {
                    //If not, cut off the input
                    BlockingContainerInput = true;
                    inputStatus.MouseInputBlockedObject = Container;
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
                inputStatus.MouseInputBlockedObject = nullptr;
            }
        }

        FUNC_DEBUG_EXIT();
    }

    void MaskEnforcer::Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {        
        FUNC_DEBUG_ENTRY();
        
        if(!Enabled || Container == nullptr || isPreRender)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        if(Container->IsRedrawNeeded())
            ConstructRenderInfo();
        
        FUNC_DEBUG_EXIT();
    }

    std::string MaskEnforcer::GetExtensionName()
    {
        return EXTENSION_NAME;
    }

    void MaskEnforcer::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;
    }

    void MaskEnforcer::Copy(ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        
        ssGUI::Extensions::MaskEnforcer* maskEnforcer = static_cast<ssGUI::Extensions::MaskEnforcer*>(extension);
        
        Enabled = maskEnforcer->IsEnabled();
        TargetMasks = maskEnforcer->TargetMasks;
        BlockingContainerInput = maskEnforcer->BlockingContainerInput;
        CurrentObjectsReferences = maskEnforcer->CurrentObjectsReferences;
    }

    ObjectsReferences* MaskEnforcer::Internal_GetObjectsReferences()
    {
        return &CurrentObjectsReferences;
    }

    MaskEnforcer* MaskEnforcer::Clone(ssGUI::GUIObject* newContainer)
    {
        MaskEnforcer* temp = new MaskEnforcer(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
}