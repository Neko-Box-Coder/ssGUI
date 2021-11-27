#include "ssGUI/BaseClasses/Image.hpp"


namespace ssGUI
{
    Image::Image(Image const& other) : Widget(other)
    {
        BackendImage = other.BackendImage->Clone();
        Fitting = other.GetFitting();
    }
    
    Image::Image() : BackendImage(), Fitting(ssGUI::Enums::ImageFitting::FIT_WHOLE_AREA) 
    {
        BackendImage = ssGUI::Backend::BackendFactory::CreateBackendImageInterface();
        AddExtension(new ssGUI::Extensions::Border());
    }

    Image::~Image()
    {
        delete BackendImage;
    }

    ssGUI::Backend::BackendImageInterface* Image::GetBackendImageInterface()
    {
        return BackendImage;
    }

    ssGUI::Enums::ImageFitting Image::GetFitting() const
    {
        return Fitting;
    }
        
    void Image::SetFitting(ssGUI::Enums::ImageFitting fitting)
    {
        Fitting = fitting;
    }

    //Overriding widget
    ssGUI::Enums::GUIObjectType Image::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::IMAGE | ssGUI::Enums::GUIObjectType::BASE_WIDGET;
    }

    void Image::Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {
        for(auto extension : Extensions)
            extension.second->Draw(true, drawingInterface, mainWindowP, mainWindowPositionOffset);
        
        if(!IsVisible())
            return;
    
        glm::ivec2 drawPosition = GetGlobalPosition();

        //Background
        DrawingVerticies.push_back(drawPosition);
        DrawingUVs.push_back(glm::ivec2());
        DrawingColours.push_back(GetBackgroundColour());

        DrawingVerticies.push_back(drawPosition + glm::ivec2(GetSize().x, 0));
        DrawingUVs.push_back(glm::ivec2());
        DrawingColours.push_back(GetBackgroundColour());

        DrawingVerticies.push_back(drawPosition + glm::ivec2(GetSize().x, GetSize().y));
        DrawingUVs.push_back(glm::ivec2());
        DrawingColours.push_back(GetBackgroundColour());

        DrawingVerticies.push_back(drawPosition + glm::ivec2(0, GetSize().y));
        DrawingUVs.push_back(glm::ivec2());
        DrawingColours.push_back(GetBackgroundColour());

        DrawingCounts.push_back(4);
        DrawingProperties.push_back(ssGUI::DrawingProperty());
        

        //TODO: The code below can be moved to its own function
        glm::ivec2 imgDrawPosition = GetGlobalPosition();
        glm::ivec2 imgSize = BackendImage->GetSize();

        DrawingColours.push_back(glm::u8vec4(255, 255, 255, 255));
        DrawingColours.push_back(glm::u8vec4(255, 255, 255, 255));
        DrawingColours.push_back(glm::u8vec4(255, 255, 255, 255));
        DrawingColours.push_back(glm::u8vec4(255, 255, 255, 255));

        float widgetLandscapeRatio = (float)GetSize().x / (float)GetSize().y;
        float imageLandscapeRatio = (float)imgSize.x / (float)imgSize.y;

        ssGUI::Enums::ImageFitting imgFitting = Fitting;

        //Image can only fit horizontally or vertically. Setting fitting to whole area or whole image will redirect it
        //to one of those setting.
        if(imgFitting == ssGUI::Enums::ImageFitting::FIT_WHOLE_AREA)
        {
            if(widgetLandscapeRatio > imageLandscapeRatio)
                imgFitting = ssGUI::Enums::ImageFitting::FIT_HORIZONTAL;
            else
                imgFitting = ssGUI::Enums::ImageFitting::FIT_VERTICAL;
        }
        else if(imgFitting == ssGUI::Enums::ImageFitting::FIT_WHOLE_IMAGE)
        {
            if(widgetLandscapeRatio > imageLandscapeRatio)
                imgFitting = ssGUI::Enums::ImageFitting::FIT_VERTICAL;
            else
                imgFitting = ssGUI::Enums::ImageFitting::FIT_HORIZONTAL;
        }

        int showWidth = 0;
        int showHeight = 0;
        glm::ivec2 showUVOrigin = glm::ivec2();
        float showLandscapeRatio = 0;
        bool showWholeImg = true;
        //Find out the UVs
        switch (imgFitting)
        {
            case ssGUI::Enums::ImageFitting::FIT_HORIZONTAL:
                showWidth = imgSize.x;
                showHeight = imgSize.x / widgetLandscapeRatio;
                showWholeImg = showHeight > imgSize.y;
                showHeight = showWholeImg ? imgSize.y : showHeight;
                
                showLandscapeRatio = (float)showWidth / (float)showHeight;

                showUVOrigin.y = (imgSize.y - showHeight) / 2;
                showUVOrigin.x = 0;

                imgDrawPosition.y = showWholeImg ? imgDrawPosition.y + (GetSize().y - (float)GetSize().x / showLandscapeRatio) / 2 : imgDrawPosition.y;

                DrawingVerticies.push_back(imgDrawPosition);
                DrawingVerticies.push_back(imgDrawPosition + glm::ivec2(GetSize().x, 0));
                DrawingVerticies.push_back(imgDrawPosition + glm::ivec2(GetSize().x, GetSize().x / showLandscapeRatio));
                DrawingVerticies.push_back(imgDrawPosition + glm::ivec2(0, GetSize().x / showLandscapeRatio));

                break;
            case ssGUI::Enums::ImageFitting::FIT_VERTICAL:
                showWidth = imgSize.y * widgetLandscapeRatio;
                showHeight = imgSize.y;
                showWholeImg = showWidth > imgSize.x;
                showWidth = showWholeImg ? imgSize.x : showWidth;

                showLandscapeRatio = (float)showWidth / (float)showHeight;

                showUVOrigin.y = 0;
                showUVOrigin.x = (imgSize.x - showWidth) / 2;

                imgDrawPosition.x = showWholeImg ? imgDrawPosition.x + (GetSize().x - (float)GetSize().y * showLandscapeRatio) / 2 : imgDrawPosition.x;

                DrawingVerticies.push_back(imgDrawPosition);
                DrawingVerticies.push_back(imgDrawPosition + glm::ivec2(GetSize().y * showLandscapeRatio, 0));
                DrawingVerticies.push_back(imgDrawPosition + glm::ivec2(GetSize().y * showLandscapeRatio, GetSize().y));
                DrawingVerticies.push_back(imgDrawPosition + glm::ivec2(0, GetSize().y));
                break;
        }

        DrawingUVs.push_back(showUVOrigin);
        DrawingUVs.push_back(showUVOrigin + glm::ivec2(showWidth, 0));
        DrawingUVs.push_back(showUVOrigin + glm::ivec2(showWidth, showHeight));
        DrawingUVs.push_back(showUVOrigin + glm::ivec2(0, showHeight));

        DrawingCounts.push_back(4);
        ssGUI::DrawingProperty currentProperty;
        currentProperty.imageP = GetBackendImageInterface();
        DrawingProperties.push_back(currentProperty);

        //Border (Depricated)
        //DrawBorder(drawingInterface, mainWindowP, mainWindowPositionOffset);

        for(auto extension : Extensions)
            extension.second->Draw(false, drawingInterface, mainWindowP, mainWindowPositionOffset);
        
        drawingInterface->DrawEntities(DrawingVerticies, DrawingUVs, DrawingColours, DrawingCounts, DrawingProperties);
        DrawingVerticies.clear();
        DrawingUVs.clear();
        DrawingColours.clear();
        DrawingCounts.clear();
        DrawingProperties.clear();
    }

    GUIObject* Image::Clone(std::vector<GUIObject*>& originalObjs, bool cloneChildren)
    {
        Image* temp = new Image(*this);

        for(auto extension : Extensions)
        {
            if(!temp->IsExtensionExist(extension.second->GetExtensionName()))
                temp->AddExtension(extension.second->Clone(this));
        }

        for(auto eventCallback : EventCallbacks)
        {
            std::vector<ssGUI::GUIObject*> tempVec = std::vector<ssGUI::GUIObject*>();

            if(!temp->IsEventCallbackExist(eventCallback.second->GetEventCallbackName()))
                temp->AddEventCallback(eventCallback.second->Clone(this, originalObjs, tempVec));
        }

        return temp;
    }

}