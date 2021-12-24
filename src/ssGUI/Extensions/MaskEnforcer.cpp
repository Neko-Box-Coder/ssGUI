#include "ssGUI/Extensions/MaskEnforcer.hpp"

namespace ssGUI::Extensions
{
    MaskEnforcer::MaskEnforcer(MaskEnforcer const& other)
    {
        CurrentMask = other.CurrentMask;
        Container = nullptr;
        Enabled = other.IsEnabled();
        BlockingContainerInput = false;
        LastMaskGlobalPosition = glm::ivec2();
        LastMaskSize = glm::ivec2();
        LastContainerGlobalPosition = glm::ivec2();
        LastContainerSize = glm::ivec2();
        LastMainWindowGlobalPosition = glm::ivec2();

        LastVertices = std::vector<glm::ivec2>();
        LastUVs = std::vector<glm::ivec2>();
        LastColours = std::vector<glm::u8vec4>();
        LastCounts = std::vector<int>();
        CurrentObjectsReferences = other.CurrentObjectsReferences;
        Cached = false;
        AllowCaching = other.IsAllowingCaching();
    }
    
    const std::string MaskEnforcer::EXTENSION_NAME = "Mask Enforcer";
    
    MaskEnforcer::MaskEnforcer() : CurrentMask(-1), Container(nullptr), Enabled(true), BlockingContainerInput(false), LastMaskGlobalPosition(), 
                                    LastMaskSize(), LastContainerGlobalPosition(), LastContainerSize(), LastVertices(), LastUVs(),
                                    LastColours(), LastCounts(), CurrentObjectsReferences(), Cached(false), AllowCaching(false)
    {}

    MaskEnforcer::~MaskEnforcer()
    {}
    
    void MaskEnforcer::BindToMaskGUIObject(ssGUI::GUIObject* maskGUIObj)
    {        
        if(CurrentMask != -1)
        {
            CurrentObjectsReferences.RemoveObjectReference(CurrentMask);    
        }

        CurrentMask = CurrentObjectsReferences.AddObjectReference(maskGUIObj);
        DiscardCache();
    }

    ssGUI::GUIObject* MaskEnforcer::GetMaskGUIObject()
    {
        return CurrentMask == -1 ? nullptr : CurrentObjectsReferences.GetObjectReference(CurrentMask);
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
    void MaskEnforcer::Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();
        
        if(CurrentMask == -1 || CurrentObjectsReferences.GetObjectReference(CurrentMask) == nullptr ||
            !CurrentObjectsReferences.GetObjectReference(CurrentMask)->IsExtensionExist(ssGUI::Extensions::Mask::EXTENSION_NAME) || 
            !CurrentObjectsReferences.GetObjectReference(CurrentMask)->GetExtension(ssGUI::Extensions::Mask::EXTENSION_NAME)->IsEnabled() ||
            Container == nullptr || globalInputStatus.MouseInputBlocked || windowInputStatus.MouseInputBlocked || !Enabled)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        //Check if there's any mouse input
        if(IsPreUpdate)
        {
            //If so, check if the cursor is inside the mask
            if(!static_cast<ssGUI::Extensions::Mask*>(CurrentObjectsReferences.GetObjectReference(CurrentMask)->GetExtension(ssGUI::Extensions::Mask::EXTENSION_NAME))
                ->IsPointContainedInMask(inputInterface->GetCurrentMousePosition(mainWindow)))
            {
                //If not, cut off the input
                BlockingContainerInput = true;
                windowInputStatus.MouseInputBlocked = true;
            }  
        }
        //Check if the mouse input is blocked, if so convert back so it doesn't affect other gui objects
        else
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
        
        if(IsPreRender || CurrentMask == -1 || CurrentObjectsReferences.GetObjectReference(CurrentMask) == nullptr ||
            !CurrentObjectsReferences.GetObjectReference(CurrentMask)->IsExtensionExist(ssGUI::Extensions::Mask::EXTENSION_NAME) || 
            !CurrentObjectsReferences.GetObjectReference(CurrentMask)->GetExtension(ssGUI::Extensions::Mask::EXTENSION_NAME)->IsEnabled() ||
            Container == nullptr || !Enabled)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        bool renderNeeded = false;
        ssGUI::Extensions::Mask* currentMask = static_cast<ssGUI::Extensions::Mask*>(CurrentObjectsReferences.GetObjectReference(CurrentMask)->GetExtension(ssGUI::Extensions::Mask::EXTENSION_NAME));

        if(!AllowCaching || !Cached || LastMaskSize != currentMask->GetSize() || LastContainerSize != Container->GetSize() ||
            Container->GetGlobalPosition() - currentMask->GetGlobalPosition() != LastContainerGlobalPosition - LastMaskGlobalPosition)
        {
            renderNeeded = true;
        }

        if(renderNeeded)
        {
            LastMaskSize = currentMask->GetSize();
            LastContainerSize = Container->GetSize();
            LastContainerGlobalPosition = Container->GetGlobalPosition();
            LastMaskGlobalPosition = currentMask->GetGlobalPosition();
            LastMainWindowGlobalPosition = mainWindowP->GetGlobalPosition();
            
            currentMask->MaskObject(Container, /*- (mainWindowP->GetGlobalPosition() + mainWindowPositionOffset)*/glm::ivec2());

            if(AllowCaching)
            {
                LastVertices.assign(Container->Extension_GetDrawingVerticies().begin(), Container->Extension_GetDrawingVerticies().end());
                LastUVs.assign(Container->Extension_GetDrawingUVs().begin(), Container->Extension_GetDrawingUVs().end());
                LastColours.assign(Container->Extension_GetDrawingColours().begin(), Container->Extension_GetDrawingColours().end());
                LastCounts.assign(Container->Extension_GetDrawingCounts().begin(), Container->Extension_GetDrawingCounts().end());
            }

            Cached = true;
        }
        else
        {             
            Container->Extension_GetDrawingVerticies().assign(LastVertices.begin(), LastVertices.end());
            Container->Extension_GetDrawingUVs().assign(LastUVs.begin(), LastUVs.end());
            Container->Extension_GetDrawingColours().assign(LastColours.begin(), LastColours.end());
            Container->Extension_GetDrawingCounts().assign(LastCounts.begin(), LastCounts.end());

            glm::ivec2 posDifference = currentMask->GetGlobalPosition() - LastMaskGlobalPosition;
        
            for(int i = 0; i < Container->Extension_GetDrawingVerticies().size(); i++)
                Container->Extension_GetDrawingVerticies()[i] += posDifference;
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
        
        CurrentMask = maskEnforcer->CurrentMask;
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