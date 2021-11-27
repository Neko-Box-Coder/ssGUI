#include "ssGUI/Extensions/AdvancedSize.hpp"

namespace ssGUI::Extensions
{

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
        OverrideDefaultSize = other.IsOverrideDefaultSize();
    }

    const std::string AdvancedSize::EXTENSION_NAME = "Advanced Size";
    
    AdvancedSize::AdvancedSize() : Container(nullptr), Enabled(true), HorizontalUsePercentage(false), VerticalUsePercentage(false), 
                                    HorizontalPixelValue(50), VerticalPixelValue(50), HorizontalPercentageValue(0), 
                                    VerticalPercentageValue(0), OverrideDefaultSize(true)
    {}

    AdvancedSize::~AdvancedSize()
    {}

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

    void AdvancedSize::SetHorizontalPixel(int pixel)
    {
        HorizontalPixelValue = pixel;
    }

    int AdvancedSize::GetHorizontalPixel() const
    {
        return HorizontalPixelValue;
    }

    void AdvancedSize::SetVerticalPixel(int pixel)
    {
        VerticalPixelValue = pixel;
    }

    int AdvancedSize::GetVerticalPixel() const
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

    void AdvancedSize::SetOverrideDefaultSize(bool override)
    {
        OverrideDefaultSize = override;
    }

    float AdvancedSize::IsOverrideDefaultSize() const
    {
        return OverrideDefaultSize;
    }

    void AdvancedSize::SetEnabled(bool enabled)
    {
        Enabled = enabled;
    }

    bool AdvancedSize::IsEnabled() const
    {
        return Enabled;
    }

    void AdvancedSize::Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        //TODO : Cache if parent's size hasn't changed
        
        //This should be done in post update
        if(IsPreUpdate || !IsOverrideDefaultSize() || Container == nullptr || Container->GetParentP() == nullptr || !Enabled)
            return;
        
        ssGUI::GUIObject* parent = Container->GetParentP();

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
        Container->SetSize(glm::ivec2(finalSize));
    }

    void AdvancedSize::Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
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
        OverrideDefaultSize = as->IsOverrideDefaultSize();
    }


    Extension* AdvancedSize::Clone(ssGUI::GUIObject* newContainer)
    {
        AdvancedSize* temp = new AdvancedSize(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
    
}