#include "ssGUI/Extensions/MaskEnforcer.hpp"

#include "ssGUI/BaseClasses/MainWindow.hpp" //For getting mouse position

namespace ssGUI::Extensions
{
    MaskEnforcer::MaskEnforcer(MaskEnforcer const& other)
    {
        TargetMasks = other.TargetMasks;
        Container = nullptr;
        Enabled = other.IsEnabled();
        BlockingContainerInput = false;
        LastMaskGlobalPosition = std::map<ssGUIObjectIndex, glm::ivec2>();
        LastMaskSize = std::map<ssGUIObjectIndex, glm::ivec2>();
        LastContainerGlobalPosition = glm::ivec2();
        LastContainerSize = glm::ivec2();

        LastVertices = std::vector<glm::ivec2>();
        LastUVs = std::vector<glm::ivec2>();
        LastColours = std::vector<glm::u8vec4>();
        LastCounts = std::vector<int>();
        CurrentObjectsReferences = other.CurrentObjectsReferences;
        Cached = false;
        AllowCaching = other.IsAllowingCaching();
    }
    
    const std::string MaskEnforcer::EXTENSION_NAME = "Mask Enforcer";
    
    MaskEnforcer::MaskEnforcer() : TargetMasks(), Container(nullptr), Enabled(true), BlockingContainerInput(false), LastMaskGlobalPosition(), 
                                    LastMaskSize(), LastContainerGlobalPosition(), LastContainerSize(), LastVertices(), LastUVs(),
                                    LastColours(), LastCounts(), CurrentObjectsReferences(), Cached(false), AllowCaching(true)
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
        DiscardCache();
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

    void MaskEnforcer::SetAllowingCaching(bool allowCaching)
    {
        AllowCaching = allowCaching;
    }

    bool MaskEnforcer::IsAllowingCaching() const
    {
        return AllowCaching;
    }

    void MaskEnforcer::DiscardCache()
    {
        Cached = false;
    }

    void MaskEnforcer::SetEnabled(bool enabled)
    {
        Enabled = enabled;
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

    void MaskEnforcer::Internal_Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {        
        FUNC_DEBUG_ENTRY();
        
        if(!Enabled || Container == nullptr || IsPreRender)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        bool renderNeeded = false;
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

            //Check cache
            if(!renderNeeded)
            {
                if(!AllowCaching || !Cached)    
                    renderNeeded = true;
                else if(LastMaskSize.find(*it) == LastMaskSize.end())
                    renderNeeded = true;
                else if(LastMaskSize[*it] != currentMask->GetSize() || LastContainerSize != Container->GetSize() ||
                    //Check relative distance from the container to the mask
                    Container->GetGlobalPosition() - currentMask->GetGlobalPosition() != LastContainerGlobalPosition - LastMaskGlobalPosition[*it])
                {
                    renderNeeded = true;
                }
            }

            //Render
            if(renderNeeded)
            {
                LastMaskGlobalPosition[*it] = currentMask->GetGlobalPosition();
                LastMaskSize[*it] = currentMask->GetSize();
                LastContainerSize = Container->GetSize();
                LastContainerGlobalPosition = Container->GetGlobalPosition();
                currentMask->MaskObject(Container, /*- (mainWindowP->GetGlobalPosition() + mainWindowPositionOffset)*/glm::ivec2());
            }
        }

        //Caching
        if(AllowCaching && renderNeeded)
        {
            LastVertices.assign(Container->Extension_GetDrawingVertices().begin(), Container->Extension_GetDrawingVertices().end());
            LastUVs.assign(Container->Extension_GetDrawingUVs().begin(), Container->Extension_GetDrawingUVs().end());
            LastColours.assign(Container->Extension_GetDrawingColours().begin(), Container->Extension_GetDrawingColours().end());
            LastCounts.assign(Container->Extension_GetDrawingCounts().begin(), Container->Extension_GetDrawingCounts().end());

            Cached = true;
        }
        else if(AllowCaching && !renderNeeded)
        {
            Container->Extension_GetDrawingVertices().assign(LastVertices.begin(), LastVertices.end());
            Container->Extension_GetDrawingUVs().assign(LastUVs.begin(), LastUVs.end());
            Container->Extension_GetDrawingColours().assign(LastColours.begin(), LastColours.end());
            Container->Extension_GetDrawingCounts().assign(LastCounts.begin(), LastCounts.end());

            glm::ivec2 posDifference = Container->GetGlobalPosition() - LastContainerGlobalPosition;
        
            for(int i = 0; i < Container->Extension_GetDrawingVertices().size(); i++)
                Container->Extension_GetDrawingVertices()[i] += posDifference;
        }
        
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
        Cached = false;
        AllowCaching = maskEnforcer->IsAllowingCaching();
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