#include "ssGUI/Extensions/AdvancedPosition.hpp"

namespace ssGUI::Extensions
{

    AdvancedPosition::AdvancedPosition(AdvancedPosition const& other)
    {
        Container = nullptr;
        Enabled = other.IsEnabled();
        CurrentHorizontal = other.GetHorizontalAnchor();
        CurrentVertical = other.GetVerticalAnchor();
        HorizontalUsePercentage = other.IsHorizontalUsePercentage();
        VerticalUsePercentage = other.IsVerticalUsePercentage();
        HorizontalPixelValue = other.GetHorizontalPixel();
        VerticalPixelValue = other.GetVerticalPixel();
        HorizontalPercentageValue = other.GetHorizontalPercentage();
        VerticalPercentageValue = other.GetVerticalPercentage();
    }

    const std::string AdvancedPosition::EXTENSION_NAME = "Advanced Position";
    
    AdvancedPosition::AdvancedPosition() : Container(nullptr), Enabled(true), CurrentHorizontal(AdvancedPosition::HorizontalAnchor::CENTER), 
                                            CurrentVertical(AdvancedPosition::VerticalAnchor::CENTER), HorizontalUsePercentage(true),
                                            VerticalUsePercentage(true), HorizontalPixelValue(0), VerticalPixelValue(0),
                                            HorizontalPercentageValue(0), VerticalPercentageValue(0)
    {}

    AdvancedPosition::~AdvancedPosition()
    {}

    void AdvancedPosition::SetHorizontalAnchor(HorizontalAnchor anchor)
    {
        CurrentHorizontal = anchor;
    }

    AdvancedPosition::HorizontalAnchor AdvancedPosition::GetHorizontalAnchor() const
    {
        return CurrentHorizontal;
    }

    void AdvancedPosition::SetVerticalAnchor(VerticalAnchor anchor)
    {
        CurrentVertical = anchor;
    }

    AdvancedPosition::VerticalAnchor AdvancedPosition::GetVerticalAnchor() const
    {
        return CurrentVertical;
    }

    void AdvancedPosition::SetHorizontalUsePercentage(bool percentage)
    {
        HorizontalUsePercentage = percentage;
    }

    bool AdvancedPosition::IsHorizontalUsePercentage() const
    {
        return HorizontalUsePercentage;
    }

    void AdvancedPosition::SetVerticalUsePercentage(bool percentage)
    {
        VerticalUsePercentage = percentage;
    }

    bool AdvancedPosition::IsVerticalUsePercentage() const
    {
        return VerticalUsePercentage;
    }

    void AdvancedPosition::SetHorizontalPixel(int pixel)
    {
        HorizontalPixelValue = pixel;
    }

    int AdvancedPosition::GetHorizontalPixel() const
    {
        return HorizontalPixelValue;
    }

    void AdvancedPosition::SetVerticalPixel(int pixel)
    {
        VerticalPixelValue = pixel;
    }

    int AdvancedPosition::GetVerticalPixel() const
    {
        return VerticalPixelValue;
    }

    void AdvancedPosition::SetHorizontalPercentage(float percentage)
    {
        HorizontalPercentageValue = percentage;
    }

    float AdvancedPosition::GetHorizontalPercentage() const
    {
        return HorizontalPercentageValue;
    }

    void AdvancedPosition::SetVerticalPercentage(float percentage)
    {
        VerticalPercentageValue = percentage;
    }

    float AdvancedPosition::GetVerticalPercentage() const
    {
        return VerticalPercentageValue;
    }

    void AdvancedPosition::SetEnabled(bool enabled)
    {
        Enabled = enabled;
    }

    bool AdvancedPosition::IsEnabled() const
    {
        return Enabled;
    }

    void AdvancedPosition::Internal_Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();        
        //TODO : Cache if parent's global position and size hasn't changed
        
        //This should be done in post update
        if(IsPreUpdate || Container == nullptr || Container->GetParent() == nullptr || !Enabled)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        
        ssGUI::GUIObject* parent = Container->GetParent();

        glm::vec2 finalPos;

        //See if there are any center anchor point. If so, use top-left default anchor point
        if(GetHorizontalAnchor() == HorizontalAnchor::CENTER || GetVerticalAnchor() == VerticalAnchor::CENTER)
        {
            Container->SetAnchorType(ssGUI::Enums::AnchorType::TOP_LEFT);
            float anchorPointX; //Anchor point in parent local space 
            float anchorPointY; //Anchor point in parent local space 
            
            //Horizontal
            if(HorizontalUsePercentage)
            {
                float distanceFromAnchor = parent->GetSize().x * GetHorizontalPercentage();
                
                switch (GetHorizontalAnchor())
                {
                    case AdvancedPosition::HorizontalAnchor::LEFT:
                        anchorPointX = 0;
                        finalPos.x = distanceFromAnchor;
                        break;
                    case AdvancedPosition::HorizontalAnchor::CENTER:
                        anchorPointX = parent->GetSize().x * 0.5f;
                        finalPos.x = anchorPointX - Container->GetSize().x * 0.5 + distanceFromAnchor;
                        break;
                    case AdvancedPosition::HorizontalAnchor::RIGHT:
                        anchorPointX = parent->GetSize().x;
                        finalPos.x = anchorPointX - Container->GetSize().x + distanceFromAnchor * -1.f;
                        break;   
                }
            }
            else
            {
                switch (GetHorizontalAnchor())
                {
                    case AdvancedPosition::HorizontalAnchor::LEFT:
                        anchorPointX = 0;
                        finalPos.x = GetHorizontalPixel();
                        break;
                    case AdvancedPosition::HorizontalAnchor::CENTER:
                        anchorPointX = parent->GetSize().x * 0.5f;
                        finalPos.x = anchorPointX - Container->GetSize().x * 0.5 + GetHorizontalPixel();
                        break;
                    case AdvancedPosition::HorizontalAnchor::RIGHT:
                        anchorPointX = parent->GetSize().x;
                        finalPos.x = anchorPointX - Container->GetSize().x + GetHorizontalPixel() * -1.f;
                        break;   
                }
            }

            //Vertical
            float windowOffset = 0;
            if(parent->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && parent->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
                windowOffset = static_cast<ssGUI::Window*>(parent)->GetTitlebarHeight();
            
            if(VerticalUsePercentage)
            {
                float distanceFromAnchor = (parent->GetSize().y - windowOffset) * GetVerticalPercentage();

                switch (GetVerticalAnchor())
                {
                    case AdvancedPosition::VerticalAnchor::TOP:
                        anchorPointY = 0;
                        finalPos.y = distanceFromAnchor;
                        break;
                    case AdvancedPosition::VerticalAnchor::CENTER:
                        anchorPointY = (parent->GetSize().y - windowOffset) * 0.5f;
                        finalPos.y = anchorPointY - Container->GetSize().y * 0.5 + distanceFromAnchor;
                        break;
                    case AdvancedPosition::VerticalAnchor::BOTTOM:
                        anchorPointY = (parent->GetSize().y - windowOffset);
                        finalPos.y = anchorPointY - Container->GetSize().y + distanceFromAnchor * -1.f;
                        break;   
                }
            }
            else
            {
                switch (GetVerticalAnchor())
                {
                    case AdvancedPosition::VerticalAnchor::TOP:
                        anchorPointY = 0;
                        finalPos.y = GetVerticalPixel();
                        break;
                    case AdvancedPosition::VerticalAnchor::CENTER:
                        anchorPointY = (parent->GetSize().y - windowOffset) * 0.5f;
                        finalPos.y = anchorPointY - Container->GetSize().y * 0.5 + GetVerticalPixel();
                        break;
                    case AdvancedPosition::VerticalAnchor::BOTTOM:
                        anchorPointY = (parent->GetSize().y - windowOffset);
                        finalPos.y = anchorPointY - Container->GetSize().y + GetVerticalPixel() * -1.f;
                        break;   
                }
            }
        }
        //If there's isn't, use the default anchor point
        else
        {
            //Set anchor point
            //Top Left
            if(GetHorizontalAnchor() == AdvancedPosition::HorizontalAnchor::LEFT && GetVerticalAnchor() == AdvancedPosition::VerticalAnchor::TOP)
                Container->SetAnchorType(ssGUI::Enums::AnchorType::TOP_LEFT);
            //Top Right
            else if(GetHorizontalAnchor() == AdvancedPosition::HorizontalAnchor::RIGHT && GetVerticalAnchor() == AdvancedPosition::VerticalAnchor::TOP)
                Container->SetAnchorType(ssGUI::Enums::AnchorType::TOP_RIGHT);
            //Bottom Right
            else if(GetHorizontalAnchor() == AdvancedPosition::HorizontalAnchor::RIGHT && GetVerticalAnchor() == AdvancedPosition::VerticalAnchor::BOTTOM)
                Container->SetAnchorType(ssGUI::Enums::AnchorType::BOTTOM_RIGHT);
            //Bottom Left
            else
                Container->SetAnchorType(ssGUI::Enums::AnchorType::BOTTOM_LEFT);

            finalPos.x = IsHorizontalUsePercentage() ? parent->GetSize().x * GetHorizontalPercentage() : GetHorizontalPixel();
            
            if(IsVerticalUsePercentage())
            {
                //Vertical
                float windowOffset = 0;
                if(parent->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && parent->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
                    windowOffset = static_cast<ssGUI::Window*>(parent)->GetTitlebarHeight();
                
                finalPos.y = (parent->GetSize().y - windowOffset) * GetVerticalPercentage();
            }
            else
                finalPos.y = GetVerticalPixel();   
        }

        //Use finalPos
        Container->SetPosition(glm::ivec2(finalPos));
        FUNC_DEBUG_EXIT();
    }

    void AdvancedPosition::Internal_Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {
        //Nothing to draw
    }

    std::string AdvancedPosition::GetExtensionName()
    {
        return EXTENSION_NAME;
    }

    void AdvancedPosition::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;
    }

    void AdvancedPosition::Copy(ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        
        ssGUI::Extensions::AdvancedPosition* ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(extension);
        Enabled = ap->IsEnabled();
        CurrentHorizontal = ap->GetHorizontalAnchor();
        CurrentVertical = ap->GetVerticalAnchor();
        HorizontalUsePercentage = ap->IsHorizontalUsePercentage();
        VerticalUsePercentage = ap->IsVerticalUsePercentage();
        HorizontalPixelValue = ap->GetHorizontalPixel();
        VerticalPixelValue = ap->GetVerticalPixel();
        HorizontalPercentageValue = ap->GetHorizontalPercentage();
        VerticalPercentageValue = ap->GetVerticalPercentage();
    }

    ObjectsReferences* AdvancedPosition::Internal_GetObjectsReferences()
    {
        return nullptr;
    }

    Extension* AdvancedPosition::Clone(ssGUI::GUIObject* newContainer)
    {
        AdvancedPosition* temp = new AdvancedPosition(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
    
}