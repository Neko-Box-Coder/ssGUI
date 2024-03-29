#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Extensions
{
    AdvancedSize::AdvancedSize() :  Container(nullptr),
                                    Enabled(true),
                                    HorizontalPixelValue(0),
                                    VerticalPixelValue(0),
                                    HorizontalPercentageValue(0),
                                    VerticalPercentageValue(0),
                                    SettingsChanged(true),
                                    LastParentSize()
    {}

    AdvancedSize::~AdvancedSize()
    {}

    AdvancedSize::AdvancedSize(AdvancedSize const& other)
    {
        Container = nullptr;
        Copy(&other);
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
    
    void AdvancedSize::SetSizingPixel(float hori, float vert)
    {
        SetHorizontalPixel(hori);        
        SetVerticalPixel(vert);        
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
    
    void AdvancedSize::SetSizingPercentage(float hori, float vert)
    {
        SetHorizontalPercentage(hori);
        SetVerticalPercentage(vert);
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

    void AdvancedSize::Internal_Update( bool isPreUpdate, 
                                        ssGUI::Backend::BackendSystemInputInterface* inputInterface, 
                                        ssGUI::InputStatus& currentInputStatus, 
                                        ssGUI::InputStatus& lastInputStatus, 
                                        ssGUI::GUIObject* mainWindow)
    {
        ssGUI_LOG_FUNC();
        
        //This should be done in pre update
        if(!isPreUpdate || Container == nullptr || Container->GetParent() == nullptr || !Enabled)
            return;

        //Size caching if parent's size hasn't changed
        ssGUI::GUIObject* parent = Container->GetParent();
        if(parent->GetSize() == LastParentSize && !SettingsChanged)
            return;

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
    }

    void AdvancedSize::Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        //Nothing to draw
    }

    std::string AdvancedSize::GetExtensionName() const
    {
        return EXTENSION_NAME;
    }

    void AdvancedSize::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;
    }

    void AdvancedSize::Copy(const ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        
        auto* as = static_cast<const ssGUI::Extensions::AdvancedSize*>(extension);
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

    AdvancedSize* AdvancedSize::Clone()
    {
        AdvancedSize* temp = new AdvancedSize(*this);
        return temp;
    }   
}

}