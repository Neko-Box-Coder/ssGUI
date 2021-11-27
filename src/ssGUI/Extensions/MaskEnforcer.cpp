#include "ssGUI/Extensions/MaskEnforcer.hpp"

namespace ssGUI::Extensions
{
    MaskEnforcer::MaskEnforcer(MaskEnforcer const& other)
    {
        BindToMask(other.CurrentMask);
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

        Cached = false;
        AllowCaching = other.IsAllowingCaching();
    }
    
    const std::string MaskEnforcer::EXTENSION_NAME = "Mask Enforcer";
    
    MaskEnforcer::MaskEnforcer() : CurrentMask(nullptr), Container(nullptr), Enabled(true), BlockingContainerInput(false), LastMaskGlobalPosition(), 
                                    LastMaskSize(), LastContainerGlobalPosition(), LastContainerSize(), LastVertices(), LastUVs(),
                                    LastColours(), LastCounts(), Cached(false), AllowCaching(false)
    {}

    MaskEnforcer::~MaskEnforcer()
    {}
    
    void MaskEnforcer::BindToMask(ssGUI::Extensions::Mask* bindMask)
    {
        CurrentMask = bindMask;
        DiscardCache();
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
        if(CurrentMask == nullptr || Container == nullptr || globalInputStatus.MouseInputBlocked || windowInputStatus.MouseInputBlocked || !Enabled)
            return;

        //Check if there's any mouse input
        if(IsPreUpdate)
        {
            //If so, check if the cursor is inside the mask
            if(!CurrentMask->IsPointContainedInMask(inputInterface->GetCurrentMousePosition(mainWindow)))
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
    }

    void MaskEnforcer::Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {
        if(IsPreRender || CurrentMask == nullptr)
            return;

        bool renderNeeded = false;

        if(!AllowCaching || !Cached || LastMaskSize != CurrentMask->GetSize() || LastContainerSize != Container->GetSize() ||
            Container->GetGlobalPosition() - CurrentMask->GetGlobalPosition() != LastContainerGlobalPosition - LastMaskGlobalPosition)
        {
            renderNeeded = true;
        }

        if(renderNeeded)
        {
            LastMaskSize = CurrentMask->GetSize();
            LastContainerSize = Container->GetSize();
            LastContainerGlobalPosition = Container->GetGlobalPosition();
            LastMaskGlobalPosition = CurrentMask->GetGlobalPosition();
            LastMainWindowGlobalPosition = mainWindowP->GetGlobalPosition();
            
            CurrentMask->MaskObject(Container, /*- (mainWindowP->GetGlobalPosition() + mainWindowPositionOffset)*/glm::ivec2());

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

            glm::ivec2 posDifference = CurrentMask->GetGlobalPosition() - LastMaskGlobalPosition;
        
            for(int i = 0; i < Container->Extension_GetDrawingVerticies().size(); i++)
                Container->Extension_GetDrawingVerticies()[i] += posDifference;
        }
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
        
        BindToMask(maskEnforcer->CurrentMask);
        Enabled = maskEnforcer->IsEnabled();

        Cached = false;
        AllowCaching = maskEnforcer->IsAllowingCaching();
    }

    Extension* MaskEnforcer::Clone(ssGUI::GUIObject* newContainer)
    {
        MaskEnforcer* temp = new MaskEnforcer(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
}