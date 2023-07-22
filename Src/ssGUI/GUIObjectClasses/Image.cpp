#include "ssGUI/GUIObjectClasses/Image.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{
    Image::Image(Image const& other) : Widget(other)
    {
        ImageData = other.ImageData;
        Fitting = other.GetFitting();
        ImageTint = other.GetImageTint();
    }

    void Image::ConstructRenderInfo()
    {
        glm::vec2 drawPosition = GetGlobalPosition();

        //Background
        ssGUI::DrawingEntity backgroundEntitiy;
        
        backgroundEntitiy.Vertices.push_back(drawPosition);
        backgroundEntitiy.Vertices.push_back(drawPosition + glm::vec2(GetSize().x, 0));
        backgroundEntitiy.Vertices.push_back(drawPosition + glm::vec2(GetSize().x, GetSize().y));
        backgroundEntitiy.Vertices.push_back(drawPosition + glm::vec2(0, GetSize().y));
        
        backgroundEntitiy.Colors.push_back(GetBackgroundColor());
        backgroundEntitiy.Colors.push_back(GetBackgroundColor());
        backgroundEntitiy.Colors.push_back(GetBackgroundColor());
        backgroundEntitiy.Colors.push_back(GetBackgroundColor());
        
        backgroundEntitiy.EntityName = GUI_OBJECT_BG_SHAPE_NAME;
        
        DrawingEntities.push_back(backgroundEntitiy);
        
        if(GetImageData() == nullptr)
            return;

        //TODO: The code below can be moved to its own function
        glm::vec2 imgDrawPosition = GetGlobalPosition();
        glm::vec2 imgSize = GetImageData()->GetSize();

        ssGUI::DrawingEntity imageEntity;

        imageEntity.Colors.push_back(ImageTint);
        imageEntity.Colors.push_back(ImageTint);
        imageEntity.Colors.push_back(ImageTint);
        imageEntity.Colors.push_back(ImageTint);

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
        glm::vec2 showUVOrigin = glm::vec2();
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

                imageEntity.Vertices.push_back(imgDrawPosition);
                imageEntity.Vertices.push_back(imgDrawPosition + glm::vec2(GetSize().x, 0));
                imageEntity.Vertices.push_back(imgDrawPosition + glm::vec2(GetSize().x, GetSize().x / showLandscapeRatio));
                imageEntity.Vertices.push_back(imgDrawPosition + glm::vec2(0, GetSize().x / showLandscapeRatio));

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

                imageEntity.Vertices.push_back(imgDrawPosition);
                imageEntity.Vertices.push_back(imgDrawPosition + glm::vec2(GetSize().y * showLandscapeRatio, 0));
                imageEntity.Vertices.push_back(imgDrawPosition + glm::vec2(GetSize().y * showLandscapeRatio, GetSize().y));
                imageEntity.Vertices.push_back(imgDrawPosition + glm::vec2(0, GetSize().y));
                break;
            default:
                ssGUI_ERROR(ssGUI_GUI_OBJECT_TAG, "Invalid ssGUI::Enums::ImageFitting: " << (int)imgFitting);
                ssLOG_EXIT_PROGRAM();
        }

        imageEntity.TexCoords.push_back(showUVOrigin);
        imageEntity.TexCoords.push_back(showUVOrigin + glm::vec2(showWidth, 0));
        imageEntity.TexCoords.push_back(showUVOrigin + glm::vec2(showWidth, showHeight));
        imageEntity.TexCoords.push_back(showUVOrigin + glm::vec2(0, showHeight));

        imageEntity.EntityName = IMAGE_SHAPE_NAME;
        imageEntity.BackendImage = ImageData->GetBackendImageInterface();
        DrawingEntities.push_back(imageEntity);
    }
    
    const std::string Image::ListenerKey = "Slider";
    const std::string Image::IMAGE_SHAPE_NAME = "Image Image";
    
    Image::Image() :    ImageData(nullptr),
                        Fitting(ssGUI::Enums::ImageFitting::FIT_WHOLE_IMAGE),
                        ImageTint(255, 255, 255, 255),
                        ImageDataChangedId(-1)
    {
        AddEventCallback(ssGUI::Enums::EventType::BEFORE_OBJECT_DESTROY)->AddEventListener
        (
            ListenerKey,
            this,
            [](ssGUI::EventInfo& info)
            {
                auto* image = static_cast<ssGUI::Image*>(info.Container);
                
                if(image->ImageData != nullptr)
                    image->ImageData->RemoveDataChangedCallback(image->ImageDataChangedId);
            }
        );
    }

    Image::~Image()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
    }

    ssGUI::ImageData* Image::GetImageData() const
    {
        return ImageData;
    }

    void Image::SetImageData(ssGUI::ImageData* imageData)
    {
        if(ImageData != nullptr)
            ImageData->RemoveDataChangedCallback(ImageDataChangedId);
        
        ImageData = imageData;
        
        if(ImageData != nullptr)
            ImageDataChangedId = ImageData->AddDataChangedCallback([&](ssGUI::ImageData*){RedrawObject();});
        
        RedrawObject();
    }

    ssGUI::Enums::ImageFitting Image::GetFitting() const
    {
        return Fitting;
    }
        
    void Image::SetFitting(ssGUI::Enums::ImageFitting fitting)
    {
        Fitting = fitting;
        RedrawObject();
    }

    void Image::SetImageTint(glm::u8vec4 tint)
    {
        ImageTint = tint;
        RedrawObject();
    }

    glm::u8vec4 Image::GetImageTint() const
    {
        return ImageTint;
    }

    //Overriding widget
    ssGUI::Enums::GUIObjectType Image::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::IMAGE | ssGUI::Enums::GUIObjectType::WIDGET;
    }

    Image* Image::Clone(bool cloneChildren)
    {
        ssLOG_FUNC_ENTRY();
        Image* temp = new Image(*this);
        CloneExtensionsAndEventCallbacks(temp);   
        
        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
            {
                ssLOG_FUNC_EXIT();
                return nullptr;
            }
        }
        
        ssLOG_FUNC_EXIT();
        return temp;
    }

}