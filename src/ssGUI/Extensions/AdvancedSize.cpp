#include "ssGUI/Extensions/AdvancedSize.hpp"

namespace ssGUI::Extensions
{

    AdvancedSize::AdvancedSize(AdvancedSize const& other)
    {
        Container = nullptr;
        SetHorizontalUsePercentage(IsHorizontalUsePercentage());
        SetVerticalUsePercentage(IsVerticalUsePercentage());
        SetHorizontalPixel(GetHorizontalPixel());
        SetVerticalPixel(GetVerticalPixel());
        SetHorizontalPercentage(GetHorizontalPercentage());
        SetVerticalPercentage(GetVerticalPercentage());
        SetOverrideDefaultSize(IsOverrideDefaultSize());
    }

    const std::string AdvancedSize::EXTENSION_NAME = "Advanced Size";
    
    AdvancedSize::AdvancedSize() : Container(nullptr), HorizontalUsePercentage(false), VerticalUsePercentage(false), 
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

    void AdvancedSize::Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        //TODO : Cache if parent's size hasn't changed
        
        //This should be done in post update
        if(IsPreUpdate || !IsOverrideDefaultSize() || Container == nullptr || Container->GetParentP() == nullptr)
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
            finalSize.y = parent->GetSize().y * GetVerticalPercentage();
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

    Extension* AdvancedSize::Clone(ssGUI::GUIObject* newContainer)
    {
        AdvancedSize* temp = new AdvancedSize(*this);
        temp->BindToObject(newContainer);
        return temp;
    }
    
}