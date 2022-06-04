#include "ssGUI/Extensions/AdvancedSize.hpp"

namespace ssGUI::Extensions
{
    AdvancedSize::AdvancedSize() : Container(nullptr), Enabled(true), HorizontalUsePercentage(true), VerticalUsePercentage(true), 
                                    HorizontalPixelValue(50), VerticalPixelValue(50), HorizontalPercentageValue(0.2), 
                                    VerticalPercentageValue(0.2), OverrideDefaultSize(true)
    {}

    AdvancedSize::~AdvancedSize()
    {}

    AdvancedSize::AdvancedSize(AdvancedSize const& other)
    {
        Container = nullptr;
        Enabled = other.IsEnabled();
        HorizontalUsePercentage = other.IsHorizontalUsePercentage();
        VerticalUsePercentage = other.IsVerticalUsePercentage();
        HorizontalPixelValue = other.GetHorizontalPixel();
        VerticalPixelValue = other.GetVerticalPixel();
        HorizontalPercentageValue = other.GetHorizontalPercentage();
        VerticalPercentageValue = other.GetVerticalPercentage();
    }

    void AdvancedSize::ConstructRenderInfo()
    {}

    void AdvancedSize::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {}

    const std::string AdvancedSize::EXTENSION_NAME = "Advanced Size";

    void AdvancedSize::SetHorizontalUsePercentage(bool percentage)
    {
        HorizontalUsePercentage = percentage;
    }

    bool AdvancedSize::IsHorizontalUsePercentage() const
    {
        return HorizontalUsePercentage;
    }

    void AdvancedSize::SetVerticalUsePercentage(bool percentage)
    {
        VerticalUsePercentage = percentage;
    }

    bool AdvancedSize::IsVerticalUsePercentage() const
    {
        return VerticalUsePercentage;
    }

    void AdvancedSize::SetHorizontalPixel(float pixel)
    {
        HorizontalPixelValue = pixel;
    }

    float AdvancedSize::GetHorizontalPixel() const
    {
        return HorizontalPixelValue;
    }

    void AdvancedSize::SetVerticalPixel(float pixel)
    {
        VerticalPixelValue = pixel;
    }

    float AdvancedSize::GetVerticalPixel() const
    {
        return VerticalPixelValue;
    }

    void AdvancedSize::SetHorizontalPercentage(float percentage)
    {
        HorizontalPercentageValue = percentage;
    }

    float AdvancedSize::GetHorizontalPercentage() const
    {
        return HorizontalPercentageValue;
    }

    void AdvancedSize::SetVerticalPercentage(float percentage)
    {
        VerticalPercentageValue = percentage;
    }

    float AdvancedSize::GetVerticalPercentage() const
    {
        return VerticalPercentageValue;
    }

    void AdvancedSize::SetEnabled(bool enabled)
    {
        Enabled = enabled;
    }

    bool AdvancedSize::IsEnabled() const
    {
        return Enabled;
    }

    void AdvancedSize::Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();
        
        //TODO : Cache if parent's size hasn't changed
        
        //This should be done in post update
        if(isPreUpdate || Container == nullptr || Container->GetParent() == nullptr || !Enabled)
        {
            FUNC_DEBUG_EXIT();
            return;
        }
        
        ssGUI::GUIObject* parent = Container->GetParent();

        glm::vec2 finalSize;

        //Horizontal
        if(IsHorizontalUsePercentage())
            finalSize.x = parent->GetSize().x * GetHorizontalPercentage();
        else
            finalSize.x = GetHorizontalPixel();

        //Vertical
        if(IsVerticalUsePercentage())
        {
            float windowOffset = 0;
            if(parent->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && parent->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
                windowOffset = static_cast<ssGUI::Window*>(parent)->GetTitlebarHeight();

            finalSize.y = (parent->GetSize().y - windowOffset) * GetVerticalPercentage();
        }
        else
            finalSize.y = GetVerticalPixel();

        //Use finalPos
        Container->SetSize(finalSize);
        
        FUNC_DEBUG_EXIT();
    }

    void AdvancedSize::Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        //Nothing to draw
    }

    std::string AdvancedSize::GetExtensionName()
    {
        return EXTENSION_NAME;
    }

    void AdvancedSize::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;
    }

    void AdvancedSize::Copy(ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        
        ssGUI::Extensions::AdvancedSize* as = static_cast<ssGUI::Extensions::AdvancedSize*>(extension);
        Enabled = as->IsEnabled();
        HorizontalUsePercentage = as->IsHorizontalUsePercentage();
        VerticalUsePercentage = as->IsVerticalUsePercentage();
        HorizontalPixelValue = as->GetHorizontalPixel();
        VerticalPixelValue = as->GetVerticalPixel();
        HorizontalPercentageValue = as->GetHorizontalPercentage();
        VerticalPercentageValue = as->GetVerticalPercentage();
    }

    ObjectsReferences* AdvancedSize::Internal_GetObjectsReferences()
    {
        return nullptr;
    }

    AdvancedSize* AdvancedSize::Clone(ssGUI::GUIObject* newContainer)
    {
        AdvancedSize* temp = new AdvancedSize(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
    
}