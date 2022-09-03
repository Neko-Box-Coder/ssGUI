#include "ssGUI/Extensions/AdvancedSize.hpp"

namespace ssGUI::Extensions
{
    AdvancedSize::AdvancedSize() : Container(nullptr), Enabled(true), HorizontalPixelValue(0), 
                                    VerticalPixelValue(0), HorizontalPercentageValue(0), 
                                    VerticalPercentageValue(0), SettingsChanged(true), LastParentSize()
    {}

    AdvancedSize::~AdvancedSize()
    {}

    AdvancedSize::AdvancedSize(AdvancedSize const& other)
    {
        Container = nullptr;
        Enabled = other.IsEnabled();
        HorizontalPixelValue = other.GetHorizontalPixel();
        VerticalPixelValue = other.GetVerticalPixel();
        HorizontalPercentageValue = other.GetHorizontalPercentage();
        VerticalPercentageValue = other.GetVerticalPercentage();
        LastParentSize = other.LastParentSize;
    }

    void AdvancedSize::ConstructRenderInfo()
    {}

    void AdvancedSize::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {}

    const std::string AdvancedSize::EXTENSION_NAME = "Advanced Size";

    void AdvancedSize::SetHorizontalPixel(float pixel)
    {
        HorizontalPixelValue = pixel;
        SettingsChanged = true;
    }

    float AdvancedSize::GetHorizontalPixel() const
    {
        return HorizontalPixelValue;
    }

    void AdvancedSize::SetVerticalPixel(float pixel)
    {
        VerticalPixelValue = pixel;
        SettingsChanged = true;
    }

    float AdvancedSize::GetVerticalPixel() const
    {
        return VerticalPixelValue;
    }

    void AdvancedSize::SetHorizontalPercentage(float percentage)
    {
        HorizontalPercentageValue = percentage;
        SettingsChanged = true;
    }

    float AdvancedSize::GetHorizontalPercentage() const
    {
        return HorizontalPercentageValue;
    }

    void AdvancedSize::SetVerticalPercentage(float percentage)
    {
        VerticalPercentageValue = percentage;
        SettingsChanged = true;
    }

    float AdvancedSize::GetVerticalPercentage() const
    {
        return VerticalPercentageValue;
    }

    void AdvancedSize::SetEnabled(bool enabled)
    {
        Enabled = enabled;
        SettingsChanged = true;
    }

    bool AdvancedSize::IsEnabled() const
    {
        return Enabled;
    }

    void AdvancedSize::Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();
        
        //This should be done in post update
        if(isPreUpdate || Container == nullptr || Container->GetParent() == nullptr || !Enabled)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        //Size caching if parent's size hasn't changed
        ssGUI::GUIObject* parent = Container->GetParent();
        if(parent->GetSize() == LastParentSize && !SettingsChanged)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        glm::vec2 finalSize;

        //Horizontal
        finalSize.x = parent->GetSize().x * GetHorizontalPercentage() + GetHorizontalPixel();

        //Vertical
        float windowOffset = 0;
        if(parent->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && parent->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
            windowOffset = static_cast<ssGUI::Window*>(parent)->GetTitlebarHeight();

        finalSize.y = (parent->GetSize().y - windowOffset) * GetVerticalPercentage() + GetVerticalPixel();

        //Use finalSize
        Container->SetSize(finalSize);

        LastParentSize = parent->GetSize();
        SettingsChanged = false;
        
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
        HorizontalPixelValue = as->GetHorizontalPixel();
        VerticalPixelValue = as->GetVerticalPixel();
        HorizontalPercentageValue = as->GetHorizontalPercentage();
        VerticalPercentageValue = as->GetVerticalPercentage();
        SettingsChanged = true;
        LastParentSize = as->LastParentSize;
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