#include "ssGUI/Extensions/AdvancedPosition.hpp"

#include "ssLogger/ssLog.hpp"

namespace ssGUI::Extensions
{
    AdvancedPosition::AdvancedPosition() :  Container(nullptr),
                                            Enabled(true),
                                            CurrentHorizontal(AdvancedPosition::HorizontalAnchor::CENTER),
                                            CurrentVertical(AdvancedPosition::VerticalAnchor::CENTER),
                                            HorizontalPixelValue(0),
                                            VerticalPixelValue(0),
                                            HorizontalPercentageValue(0),
                                            VerticalPercentageValue(0)
    {}

    AdvancedPosition::~AdvancedPosition()
    {}
    
    AdvancedPosition::AdvancedPosition(AdvancedPosition const& other)
    {
        Container = nullptr;
        Enabled = other.IsEnabled();
        CurrentHorizontal = other.GetHorizontalAnchor();
        CurrentVertical = other.GetVerticalAnchor();
        HorizontalPixelValue = other.GetHorizontalPixel();
        VerticalPixelValue = other.GetVerticalPixel();
        HorizontalPercentageValue = other.GetHorizontalPercentage();
        VerticalPercentageValue = other.GetVerticalPercentage();
    }

    void AdvancedPosition::ConstructRenderInfo()
    {}

    void AdvancedPosition::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {}

    const std::string AdvancedPosition::EXTENSION_NAME = "Advanced Position";    

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

    void AdvancedPosition::SetHorizontalPixel(float pixel)
    {
        HorizontalPixelValue = pixel;
    }

    float AdvancedPosition::GetHorizontalPixel() const
    {
        return HorizontalPixelValue;
    }

    void AdvancedPosition::SetVerticalPixel(float pixel)
    {
        VerticalPixelValue = pixel;
    }

    float AdvancedPosition::GetVerticalPixel() const
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

    void AdvancedPosition::Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, ssGUI::GUIObject* mainWindow)
    {
        ssLOG_FUNC_ENTRY();        
        //TODO : Cache if parent's global position and size hasn't changed
        
        //This should be done in post update
        if(isPreUpdate || Container == nullptr || Container->GetParent() == nullptr || !Enabled)
        {
            ssLOG_FUNC_EXIT();
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
            float distanceFromAnchor = parent->GetSize().x * GetHorizontalPercentage();
            
            switch (GetHorizontalAnchor())
            {
                case AdvancedPosition::HorizontalAnchor::LEFT:
                    anchorPointX = 0;
                    finalPos.x = distanceFromAnchor + GetHorizontalPixel();
                    break;
                case AdvancedPosition::HorizontalAnchor::CENTER:
                    anchorPointX = parent->GetSize().x * 0.5f;
                    finalPos.x = anchorPointX - Container->GetSize().x * 0.5 + distanceFromAnchor + GetHorizontalPixel();
                    break;
                case AdvancedPosition::HorizontalAnchor::RIGHT:
                    anchorPointX = parent->GetSize().x;
                    finalPos.x = anchorPointX - Container->GetSize().x + (distanceFromAnchor + GetHorizontalPixel()) * -1.f;
                    break;   
            }

            //Vertical
            float windowOffset = 0;
            if(parent->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && parent->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
                windowOffset = static_cast<ssGUI::Window*>(parent)->GetTitlebarHeight();
            
            distanceFromAnchor = (parent->GetSize().y - windowOffset) * GetVerticalPercentage();

            switch (GetVerticalAnchor())
            {
                case AdvancedPosition::VerticalAnchor::TOP:
                    anchorPointY = 0;
                    finalPos.y = distanceFromAnchor + GetVerticalPixel();
                    break;
                case AdvancedPosition::VerticalAnchor::CENTER:
                    anchorPointY = (parent->GetSize().y - windowOffset) * 0.5f;
                    finalPos.y = anchorPointY - Container->GetSize().y * 0.5 + distanceFromAnchor + GetVerticalPixel();
                    break;
                case AdvancedPosition::VerticalAnchor::BOTTOM:
                    anchorPointY = (parent->GetSize().y - windowOffset);
                    finalPos.y = anchorPointY - Container->GetSize().y + (distanceFromAnchor + GetVerticalPixel()) * -1.f;
                    break;   
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

            finalPos.x = parent->GetSize().x * GetHorizontalPercentage() + GetHorizontalPixel();
            
            //Vertical
            float windowOffset = 0;
            if(parent->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && parent->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
                windowOffset = static_cast<ssGUI::Window*>(parent)->GetTitlebarHeight();
            
            finalPos.y = (parent->GetSize().y - windowOffset) * GetVerticalPercentage() + GetVerticalPixel();
        }

        //Use finalPos
        Container->SetPosition(finalPos);
        ssLOG_FUNC_EXIT();
    }

    void AdvancedPosition::Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
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
        HorizontalPixelValue = ap->GetHorizontalPixel();
        VerticalPixelValue = ap->GetVerticalPixel();
        HorizontalPercentageValue = ap->GetHorizontalPercentage();
        VerticalPercentageValue = ap->GetVerticalPercentage();
    }

    ObjectsReferences* AdvancedPosition::Internal_GetObjectsReferences()
    {
        return nullptr;
    }

    AdvancedPosition* AdvancedPosition::Clone(ssGUI::GUIObject* newContainer)
    {
        AdvancedPosition* temp = new AdvancedPosition(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
    
}