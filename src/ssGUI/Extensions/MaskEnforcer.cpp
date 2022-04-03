#include "ssGUI/Extensions/MaskEnforcer.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

namespace ssGUI::Extensions
{
    MaskEnforcer::MaskEnforcer(MaskEnforcer const& other)
    {
        TargetMasks = other.TargetMasks;
        Container = nullptr;
        Enabled = other.IsEnabled();
        BlockingContainerInput = false;

        CurrentObjectsReferences = other.CurrentObjectsReferences;
    }

    void MaskEnforcer::ConstructRenderInfo()
    {
        for(std::set<ssGUIObjectIndex>::iterator it = TargetMasks.begin(); it != TargetMasks.end(); it++)
        {
            if(CurrentObjectsReferences.GetObjectReference(*it) == nullptr || 
            !CurrentObjectsReferences.GetObjectReference(*it)->IsExtensionExist(ssGUI::Extensions::Mask::EXTENSION_NAME))
            {
                continue;
            }

            if(!CurrentObjectsReferences.GetObjectReference(*it)->GetExtension(ssGUI::Extensions::Mask::EXTENSION_NAME)->IsEnabled())
                continue;

            ssGUI::Extensions::Mask* currentMask = static_cast<ssGUI::Extensions::Mask*>(CurrentObjectsReferences.GetObjectReference(*it)->
                GetExtension(ssGUI::Extensions::Mask::EXTENSION_NAME));

            currentMask->MaskObject(Container, /*- (mainWindowP->GetGlobalPosition() + mainWindowPositionOffset)*/glm::vec2());
        }
    }

    void MaskEnforcer::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        ConstructRenderInfo();
    }
    
    const std::string MaskEnforcer::EXTENSION_NAME = "Mask Enforcer";
    
    MaskEnforcer::MaskEnforcer() : TargetMasks(), Container(nullptr), Enabled(true), BlockingContainerInput(false), CurrentObjectsReferences()
    {}

    MaskEnforcer::~MaskEnforcer()
    {
        CurrentObjectsReferences.CleanUp();
    }
    
    void MaskEnforcer::AddTargetMaskObject(ssGUI::GUIObject* targetMaskObj)
    {        
        if(CurrentObjectsReferences.IsObjectReferenceExist(targetMaskObj))
        {
            ssGUIObjectIndex maskObjIndex = CurrentObjectsReferences.GetObjectIndex(targetMaskObj);
            if(TargetMasks.find(maskObjIndex) == TargetMasks.end())
                TargetMasks.insert(maskObjIndex);
        }
        
        ssGUIObjectIndex maskObjIndex = CurrentObjectsReferences.AddObjectReference(targetMaskObj);
        TargetMasks.insert(maskObjIndex);

        if(Container != nullptr)
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
            TargetMasks.erase(index);

        if(Container != nullptr)
            Container->RedrawObject();
    }

    std::vector<ssGUI::GUIObject*> MaskEnforcer::GetTargetMaskObjects()
    {
        std::vector<ssGUI::GUIObject*> returnVec = std::vector<ssGUI::GUIObject*>();

        for(std::set<ssGUIObjectIndex>::iterator it = TargetMasks.begin(); it != TargetMasks.end(); it++)
        {
            if(CurrentObjectsReferences.GetObjectReference(*it) != nullptr && 
                CurrentObjectsReferences.GetObjectReference(*it)->IsExtensionExist(ssGUI::Extensions::Mask::EXTENSION_NAME))
            {
                returnVec.push_back(CurrentObjectsReferences.GetObjectReference(*it));
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
    void MaskEnforcer::Internal_Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();

        if(!Enabled || Container == nullptr)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        //Updating Target Masks and blocking any mouse input outside the mask
        if(IsPreUpdate)
        {
            if(Container->GetExtensionDrawOrder(GetExtensionName()) != Container->GetExtensionsCount() - 1)
                Container->ChangeExtensionDrawOrder(GetExtensionName(), Container->GetExtensionsCount() - 1);

            std::vector<ssGUIObjectIndex> indicesToDelete = std::vector<ssGUIObjectIndex>();

            for(std::set<ssGUIObjectIndex>::iterator it = TargetMasks.begin(); it != TargetMasks.end(); it++)
            {
                if(CurrentObjectsReferences.GetObjectReference(*it) == nullptr || 
                    !CurrentObjectsReferences.GetObjectReference(*it)->IsExtensionExist(ssGUI::Extensions::Mask::EXTENSION_NAME))
                {
                    indicesToDelete.push_back(*it);
                    continue;
                }
                
                if(!CurrentObjectsReferences.GetObjectReference(*it)->GetExtension(ssGUI::Extensions::Mask::EXTENSION_NAME)->IsEnabled())
                    continue;

                if(globalInputStatus.MouseInputBlocked || windowInputStatus.MouseInputBlocked)                
                    continue;
                
                //If so, check if the cursor is inside the mask
                if(!static_cast<ssGUI::Extensions::Mask*>(CurrentObjectsReferences.GetObjectReference(*it)->GetExtension(ssGUI::Extensions::Mask::EXTENSION_NAME))
                    ->IsPointContainedInMask(inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow))))
                {
                    //If not, cut off the input
                    BlockingContainerInput = true;
                    windowInputStatus.MouseInputBlocked = true;
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
        if(!IsPreUpdate)
        {   
            if(BlockingContainerInput)
            {
                BlockingContainerInput = false;
                windowInputStatus.MouseInputBlocked = false;
            }
        }

        FUNC_DEBUG_EXIT();
    }

    void MaskEnforcer::Internal_Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {        
        FUNC_DEBUG_ENTRY();
        
        if(!Enabled || Container == nullptr || IsPreRender)
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
        
        TargetMasks = maskEnforcer->TargetMasks;
        Enabled = maskEnforcer->IsEnabled();
        CurrentObjectsReferences = maskEnforcer->CurrentObjectsReferences;
    }

    ObjectsReferences* MaskEnforcer::Internal_GetObjectsReferences()
    {
        return &CurrentObjectsReferences;
    }

    Extension* MaskEnforcer::Clone(ssGUI::GUIObject* newContainer)
    {
        MaskEnforcer* temp = new MaskEnforcer(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
}