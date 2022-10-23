#include "ssGUI/GUIObjectClasses/CompositeClasses/ImageCanvas.hpp"

#include "ssGUI/DataClasses/RealtimeInputInfo.hpp"
#include "ssGUI/Extensions/Mask.hpp"
#include "ssGUI/Extensions/MaskEnforcer.hpp"
#include "ssGUI/Extensions/Border.hpp"
#include "ssGUI/Extensions/Shape.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/Scrollbar.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position
#include "ssLogger/ssLog.hpp"

#include "glm/geometric.hpp"
#include <cmath>

namespace ssGUI
{
    ImageCanvas::ImageCanvas(ImageCanvas const& other) : Image(other)
    {
        HorizontalScrollbar = other.HorizontalScrollbar;
        SetShowHorizontalScrollbar(other.IsShowHorizontalScrollbar());
        VerticalScrollbar = other.VerticalScrollbar;
        SetShowVerticalScrollbar(other.IsShowVerticalScrollbar());

        ImageVertices = other.ImageVertices;
        ImageMinX = other.GetImageMinX();
        ImageMaxX = other.GetImageMaxX();
        ImageMinY = other.GetImageMinY();
        ImageMaxY = other.GetImageMaxY();

        UseDefaultPanning(other.IsUsingDefaultPanning());
        SetImagePosition(other.GetImagePosition());

        UseDefaultZooming(other.IsUsingDefaultZooming());
        SetDefaultMinZoom(other.GetDefaultMinZoom());
        SetDefaultMaxZoom(other.GetDefaultMaxZoom());
        SetImageScale(other.GetImageScale());

        ZoomAmount = other.ZoomAmount;
        SetMeasureScaleByHeight(other.IsMeasureScaleByHeight());
        
        UseDefaultRotating(other.IsUsingDefaultRotating());
        SetImageRotation(other.GetImageRotation());
        OnRotateStartRotation = other.OnRotateStartRotation;
        OnRotateStartPosition = other.OnRotateStartPosition;
        InnerCircleId = other.InnerCircleId;
        BackgroundCircleId = other.BackgroundCircleId;
        OuterCircleId = other.OuterCircleId;

        MousePressed = other.MousePressed;
        MouseButtonDownPosition = other.MouseButtonDownPosition;
    }

    double ImageCanvas::GetAngle(glm::vec2 a, glm::vec2 b)
    {
        glm::vec3 a3 = glm::vec3(a, 0);
        glm::vec3 b3 = glm::vec3(b, 0);

        return atan2(glm::cross(a3, b3).z, glm::dot(glm::vec2(a), glm::vec2(b)));
    }

    glm::vec2 ImageCanvas::RotateVec(glm::vec2 vec, float radians)
    {        
        return glm::vec2(   cos(radians) * vec.x - sin(radians) * vec.y, 
                            sin(radians) * vec.x + cos(radians) * vec.y);
    }

    //You only need to override this when you are rendering anything.
    void ImageCanvas::ConstructRenderInfo()
    {
        //Get the top-left position of the widget 
        glm::vec2 drawPosition = GetGlobalPosition();

        //Background
        DrawingVerticies.push_back(drawPosition);
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingVerticies.push_back(drawPosition + glm::vec2(GetSize().x, 0));
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingVerticies.push_back(drawPosition + glm::vec2(GetSize().x, GetSize().y));
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingVerticies.push_back(drawPosition + glm::vec2(0, GetSize().y));
        DrawingUVs.push_back(glm::vec2());
        DrawingColours.push_back(GetBackgroundColor());

        DrawingCounts.push_back(4);
        DrawingProperties.push_back(ssGUI::DrawingProperty());

        if(GetImageData() == nullptr)
        {
            if(GetHorizontalScrollbar() != nullptr)
                GetHorizontalScrollbar()->SetEnabled(false);
            
            if(GetVerticalScrollbar() != nullptr)
                GetVerticalScrollbar()->SetEnabled(false);

            return;
        }

        //TODO: The code below can be moved to its own function
        glm::vec2 imgDrawPosition = GetGlobalPosition();
        glm::vec2 imgSize = GetImageData()->GetSize();

        DrawingColours.push_back(ImageTint);
        DrawingColours.push_back(ImageTint);
        DrawingColours.push_back(ImageTint);
        DrawingColours.push_back(ImageTint);

        //Scale Image
        glm::vec2 imgVertices[4] { glm::vec2() };
        
        float halfSizeX = imgSize.x * 0.5;
        float halfSizeY = imgSize.y * 0.5;

        imgVertices[0] = glm::vec2(-halfSizeX, -halfSizeY);
        imgVertices[1] = glm::vec2(halfSizeX, -halfSizeY);
        imgVertices[2] = glm::vec2(halfSizeX, halfSizeY);
        imgVertices[3] = glm::vec2(-halfSizeX, halfSizeY);

        float scaleFactor = MeasureScaleByHeight ? 
            GetSize().y / imgSize.y * CurrentZoom :
            GetSize().x / imgSize.x * CurrentZoom;

        imgVertices[0] *= scaleFactor;
        imgVertices[1] *= scaleFactor;
        imgVertices[2] *= scaleFactor;
        imgVertices[3] *= scaleFactor;

        //Rotate Image
        imgVertices[0] = RotateVec(imgVertices[0], CurrentRotation);
        imgVertices[1] = RotateVec(imgVertices[1], CurrentRotation);
        imgVertices[2] = RotateVec(imgVertices[2], CurrentRotation);
        imgVertices[3] = RotateVec(imgVertices[3], CurrentRotation);

        //Position Image
        glm::vec2 imgPos = GetSize() * CurrentPosition;
        imgVertices[0] += drawPosition + imgPos;
        imgVertices[1] += drawPosition + imgPos;
        imgVertices[2] += drawPosition + imgPos;
        imgVertices[3] += drawPosition + imgPos;

        DrawingVerticies.push_back(imgVertices[0]);
        DrawingVerticies.push_back(imgVertices[1]);
        DrawingVerticies.push_back(imgVertices[2]);
        DrawingVerticies.push_back(imgVertices[3]);

        DrawingUVs.push_back(glm::vec2(0, 0));
        DrawingUVs.push_back(glm::vec2(GetImageData()->GetSize().x, 0));
        DrawingUVs.push_back(glm::vec2(GetImageData()->GetSize()));
        DrawingUVs.push_back(glm::vec2(0, GetImageData()->GetSize().y));

        DrawingCounts.push_back(4);

        ssGUI::DrawingProperty currentProperty;
        currentProperty.imageP = ImageData->GetBackendImageInterface();
        DrawingProperties.push_back(currentProperty);

        ImageVertices.clear();
        ImageVertices.push_back(imgVertices[0]);
        ImageVertices.push_back(imgVertices[1]);
        ImageVertices.push_back(imgVertices[2]);
        ImageVertices.push_back(imgVertices[3]);

        if(GetHorizontalScrollbar() != nullptr)
        {
            if(!IsShowHorizontalScrollbar())
                GetHorizontalScrollbar()->SetEnabled(false);
            else
            {
                //Find min and max x
                float minX = imgVertices[0].x;
                float maxX = imgVertices[0].x;

                for(int i = 1; i < 4; i++)
                {
                    if(imgVertices[i].x < minX)
                        minX = imgVertices[i].x;
                    else if(imgVertices[i].x > maxX)
                        maxX = imgVertices[i].x;
                }

                if(GetSize().x < (maxX - minX))
                {
                    GetHorizontalScrollbar()->SetEnabled(true);
                    GetHorizontalScrollbar()->SetScrollbarSize(GetSize().x / (maxX - minX));
                    GetHorizontalScrollbar()->SetScrollbarValue((drawPosition.x - minX) / ((maxX - minX) - GetSize().x));
                }
                else
                    GetHorizontalScrollbar()->SetEnabled(false);       

                ImageMinX = minX;
                ImageMaxX = maxX;
            }
        }

        if(GetVerticalScrollbar() != nullptr)
        {
            if(!IsShowVerticalScrollbar())
                GetVerticalScrollbar()->SetEnabled(false);
            else
            {
                //Find min and max y
                float minY = imgVertices[0].y;
                float maxY = imgVertices[0].y;

                for(int i = 1; i < 4; i++)
                {
                    if(imgVertices[i].y < minY)
                        minY = imgVertices[i].y;
                    else if(imgVertices[i].y > maxY)
                        maxY = imgVertices[i].y;
                }

                if(GetSize().y < (maxY - minY))
                {
                    GetVerticalScrollbar()->SetEnabled(true);
                    GetVerticalScrollbar()->SetScrollbarSize(GetSize().y / (maxY - minY));
                    GetVerticalScrollbar()->SetScrollbarValue((drawPosition.y - minY) / ((maxY - minY) - GetSize().y));
                }
                else
                    GetVerticalScrollbar()->SetEnabled(false); 

                ImageMinY = minY;
                ImageMaxY = maxY;      
            }
        }
    }

    const std::string ImageCanvas::ListenerKey = "Image Canvas";

    void ImageCanvas::MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                                ssGUI::GUIObject* mainWindow)
    {        
        //By default don't show rotation UI
        auto shape = GetAnyExtension<ssGUI::Extensions::Shape>();
        if(shape != nullptr)
        {
            shape->SetAdditionalCircle(BackgroundCircleId, glm::vec2(), glm::vec2(), glm::u8vec4(), false);
            shape->SetAdditionalCircle(InnerCircleId, glm::vec2(), glm::vec2(), glm::u8vec4(), false);
            shape->SetAdditionalCircle(OuterCircleId, glm::vec2(), glm::vec2(), glm::u8vec4(), false);
        }
        
        if(!IsInteractable() || !IsBlockInput())
            return;

        //No functionality if mouse input is blocked
        if(inputStatus.MouseInputBlockedObject != nullptr && inputStatus.MouseInputBlockedObject != this && !MousePressed)
            return;

        glm::ivec2 currentMousePos = inputInterface->GetCurrentMousePosition(static_cast<ssGUI::MainWindow*>(mainWindow));
        bool mouseWithinWidget =    currentMousePos.x >= GetGlobalPosition().x &&
                                    currentMousePos.y >= GetGlobalPosition().y &&
                                    currentMousePos.x <= GetGlobalPosition().x + GetSize().x &&
                                    currentMousePos.y <= GetGlobalPosition().y + GetSize().y;

        if(GetImageData() == nullptr)
        {
            if(mouseWithinWidget)
                inputStatus.MouseInputBlockedObject = this;
            
            return;
        }

        if(!inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT))
            MousePressed = false;

        if(inputStatus.KeyInputBlockedObject == nullptr || inputStatus.KeyInputBlockedObject == this)
        {
            //Check if mosue down is within widget
            if(inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) && 
                !inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::LEFT) &&
                mouseWithinWidget)
            {
                MousePressed = true;
                MouseButtonDownPosition = currentMousePos;
                inputStatus.MouseInputBlockedObject = this;
            }

            //If space key is pressed, use delta cursor position to move image
            if(IsUsingDefaultPanning() && inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::SPACE))
            {
                inputStatus.KeyInputBlockedObject = this;
                
                if(MousePressed || mouseWithinWidget)
                    inputInterface->SetCursorType(ssGUI::Enums::CursorType::MOVE);
                
                if(MousePressed)
                {
                    glm::vec2 diff = currentMousePos - inputInterface->GetLastMousePosition(static_cast<ssGUI::MainWindow*>(mainWindow));
                    diff /= GetSize();
                    SetImagePosition(GetImagePosition() + diff);
                    inputStatus.MouseInputBlockedObject = this;
                }

                SetFocus(true);
            }

            //If r key is pressed, use start cursor position, start image position and current cursor position to rotate image
            else if(IsUsingDefaultRotating() && inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::LetterKey::R))
            {
                inputStatus.KeyInputBlockedObject = this;
                SetFocus(true);

                if(MousePressed || mouseWithinWidget)
                {
                    //Load rotation cursor if it wasn't loaded
                    if(!inputInterface->HasCustomCursor("RotationCursor"))
                    {
                        ssGUI::ImageData rotationCursor;
                        if(!rotationCursor.LoadFromPath("Resources/RotationCursor.png"))
                        {
                            ssLOG_LINE("Failed to load rotation cursor");
                            return;
                        }

                        inputInterface->CreateCustomCursor(&rotationCursor, "RotationCursor", glm::ivec2(30, 30), glm::ivec2(15, 15));
                    }
                    
                    inputInterface->SetCurrentCustomCursor("RotationCursor");
                    inputInterface->SetCursorType(ssGUI::Enums::CursorType::CUSTOM);
                }

                //Get image position
                glm::vec2 imgPos = GetSize() * GetImagePosition();
                
                //Show image center gui
                if(shape != nullptr)
                {
                    glm::vec2 outerCircleSize = glm::vec2(9, 9);
                    glm::vec2 bgCircleSize = glm::vec2(6, 6);
                    glm::vec2 innerCircleSize = glm::vec2(3, 3);
                    
                    shape->SetAdditionalCircle(BackgroundCircleId, imgPos - bgCircleSize/2.f, bgCircleSize, 
                        glm::u8vec4(255, 255, 255, 255), false);                

                    shape->SetAdditionalCircle(InnerCircleId, imgPos - innerCircleSize/2.f, innerCircleSize, 
                        glm::u8vec4(0, 0, 0, 255), false);

                    shape->SetAdditionalCircle(OuterCircleId, imgPos - outerCircleSize/2.f, outerCircleSize, 
                        glm::u8vec4(0, 0, 0, 255), false);
                }

                if(MousePressed)
                {
                    if(!inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::LEFT))
                    {
                        OnRotateStartRotation = GetImageRotation(true);
                        OnRotateStartPosition = GetImagePosition();
                    }
                    else
                    {
                        glm::vec2 curImgGlobalPos = GetGlobalPosition() + imgPos;
                        SetImageRotation(OnRotateStartRotation - GetAngle(glm::vec2(currentMousePos) - curImgGlobalPos, MouseButtonDownPosition - curImgGlobalPos), true);
                    }
                    inputStatus.MouseInputBlockedObject = this;
                }
            }
        }
    
        //Handle scrolling for image zooming
        if(IsUsingDefaultZooming() && mouseWithinWidget)
        {
            inputStatus.MouseInputBlockedObject = this;

            //Scrolling up
            if(inputInterface->GetCurrentMouseScrollDelta().y > 0)
            {
                SetFocus(true);
                SetImageScale(GetImageScale() + ZoomAmount > GetDefaultMaxZoom() ? GetDefaultMaxZoom() : GetImageScale() + ZoomAmount);   
            }
            //Scrolling down
            else if(inputInterface->GetCurrentMouseScrollDelta().y)
            {
                SetFocus(true);
                SetImageScale(GetImageScale() - ZoomAmount < GetDefaultMinZoom() ? GetDefaultMinZoom() : GetImageScale() - ZoomAmount);
            }
        }
    }

    ImageCanvas::ImageCanvas() :    HorizontalScrollbar(-1),
                                    ShowHorizontalScrollbar(true),
                                    VerticalScrollbar(-1),
                                    ShowVerticalScrollbar(true),
                                    ImageVertices(),
                                    ImageMinX(0),
                                    ImageMaxX(0),
                                    ImageMinY(0),
                                    ImageMaxY(0),
                                    Panable(true),
                                    CurrentPosition(0.5, 0.5),
                                    Zoomable(true),
                                    MinZoom(0),
                                    MaxZoom(10),
                                    CurrentZoom(1),
                                    ZoomAmount(0.1),
                                    MeasureScaleByHeight(true),
                                    Rotatable(true),
                                    CurrentRotation(0),
                                    OnRotateStartRotation(0),
                                    OnRotateStartPosition(),
                                    InnerCircleId(0),
                                    BackgroundCircleId(0),
                                    OuterCircleId(0),
                                    MousePressed(false),
                                    MouseButtonDownPosition()
    {
        ssGUI::Extensions::Mask* mask = ssGUI::Factory::Create<ssGUI::Extensions::Mask>();
        mask->SetMaskChildren(false);
        mask->SetMaskContainer(true);

        ssGUI::Extensions::MaskEnforcer* mf = ssGUI::Factory::Create<ssGUI::Extensions::MaskEnforcer>();
        mf->AddTargetMaskObject(this, {1, 2, 3, 4});
        AddExtension(mf);
        AddExtension(mask);
    
        ssGUI::Extensions::Shape* shape = ssGUI::Factory::Create<ssGUI::Extensions::Shape>();
        OuterCircleId = shape->AddAdditionalCircle(glm::vec2(), glm::vec2(), glm::u8vec4(), false);
        BackgroundCircleId = shape->AddAdditionalCircle(glm::vec2(), glm::vec2(), glm::u8vec4(), false);
        InnerCircleId = shape->AddAdditionalCircle(glm::vec2(), glm::vec2(), glm::u8vec4(), false);    
        AddExtension(shape);

        AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Border>());

        auto hScrollbar = ssGUI::Factory::Create<ssGUI::Scrollbar>();
        hScrollbar->SetUserCreated(false);
        hScrollbar->SetVertical(false, true);
        auto ecb = ssGUI::Factory::Create<ssGUI::EventCallbacks::ScrollbarValueChangedViaGuiEventCallback>();
        ssGUIObjectIndex index = ecb->AddObjectReference(this);
        ecb->AddEventListener
        (
            ListenerKey, this,
            [index](ssGUI::EventInfo info)
            {
                auto imageCanvas = static_cast<ssGUI::ImageCanvas*>(info.EventCallbackReferences->GetObjectReference(index));
                if(imageCanvas == nullptr)
                    return;

                //Find min and max x
                float minX = imageCanvas->GetImageMinX();
                float maxX = imageCanvas->GetImageMaxX();

                auto scrollbar = static_cast<ssGUI::Scrollbar*>(info.EventSource);
                float imgHalfSize = (maxX - minX) * 0.5;
                float imgScrollDist = (maxX - minX - imageCanvas->GetSize().x) * scrollbar->GetScrollbarValue();
                float imgXPos = (imgHalfSize - imgScrollDist) / imageCanvas->GetSize().x;

                imageCanvas->SetImagePosition(glm::vec2(imgXPos, imageCanvas->GetImagePosition().y));
            }
        );
        hScrollbar->AddEventCallback(ecb);

        auto as = ssGUI::Factory::Create<ssGUI::Extensions::AdvancedSize>();
        auto ap = ssGUI::Factory::Create<ssGUI::Extensions::AdvancedPosition>();
        as->SetHorizontalPercentage(1);
        as->SetHorizontalPixel(-hScrollbar->GetSize().y);
        as->SetVerticalPixel(hScrollbar->GetSize().y);
        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::LEFT);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::BOTTOM);
        hScrollbar->AddExtension(as);
        hScrollbar->AddExtension(ap);


        auto vScrollbar = ssGUI::Factory::Create<ssGUI::Scrollbar>();
        vScrollbar->SetUserCreated(false);
        vScrollbar->SetVertical(true, true);
        ecb = ssGUI::Factory::Create<ssGUI::EventCallbacks::ScrollbarValueChangedViaGuiEventCallback>();
        index = ecb->AddObjectReference(this);
        ecb->AddEventListener
        (
            ListenerKey, this,
            [index](ssGUI::EventInfo info)
            {
                auto imageCanvas = static_cast<ssGUI::ImageCanvas*>(info.EventCallbackReferences->GetObjectReference(index));
                if(imageCanvas == nullptr)
                    return;

                //Find min and max y
                float minY = imageCanvas->GetImageMinY();
                float maxY = imageCanvas->GetImageMaxY();

                auto scrollbar = static_cast<ssGUI::Scrollbar*>(info.EventSource);
                float imgHalfSize = (maxY - minY) * 0.5;
                float imgScrollDist = (maxY - minY - imageCanvas->GetSize().y) * scrollbar->GetScrollbarValue();
                float imgYPos = (imgHalfSize - imgScrollDist) / imageCanvas->GetSize().y;

                imageCanvas->SetImagePosition(glm::vec2(imageCanvas->GetImagePosition().x, imgYPos));
            }
        );
        vScrollbar->AddEventCallback(ecb);

        as = ssGUI::Factory::Create<ssGUI::Extensions::AdvancedSize>();
        ap = ssGUI::Factory::Create<ssGUI::Extensions::AdvancedPosition>();
        as->SetHorizontalPixel(vScrollbar->GetSize().x);
        as->SetVerticalPercentage(1);
        as->SetVerticalPixel(-vScrollbar->GetSize().x);
        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::RIGHT);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::TOP);
        vScrollbar->AddExtension(as);
        vScrollbar->AddExtension(ap);

        hScrollbar->SetParent(this, true);
        vScrollbar->SetParent(this, true);

        HorizontalScrollbar = CurrentObjectsReferences.GetObjectIndex(hScrollbar);
        VerticalScrollbar = CurrentObjectsReferences.GetObjectIndex(vScrollbar);
    }

    ImageCanvas::~ImageCanvas()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
    }

    glm::vec2 ImageCanvas::GetUVFromGlobalPosition(glm::vec2 globalPos)
    {
        //Position
        globalPos = globalPos - GetGlobalPosition() - GetImagePosition() * GetSize();

        //Rotate
        globalPos = RotateVec(globalPos, -CurrentRotation);

        //Scale
        glm::vec2 imgSize = GetImageData()->GetSize();
        float scaleFactor = MeasureScaleByHeight ? 
            GetSize().y / imgSize.y * GetImageScale() :
            GetSize().x / imgSize.x * GetImageScale();
        globalPos = globalPos / scaleFactor;

        return globalPos - glm::vec2(-imgSize.x * 0.5, -imgSize.y * 0.5);
    }

    glm::vec2 ImageCanvas::GetGlobalPositionFromUV(glm::vec2 uv)
    {
        //Scale
        glm::vec2 imgSize = GetImageData()->GetSize();
        uv = uv - imgSize * 0.5f;

        float scaleFactor = MeasureScaleByHeight ? 
            GetSize().y / imgSize.y * GetImageScale() :
            GetSize().x / imgSize.x * GetImageScale();

        uv *= scaleFactor;

        //Rotate Image
        uv = RotateVec(uv, CurrentRotation);

        //Position Image
        glm::vec2 imgPos = GetSize() * GetImagePosition();
        uv += GetGlobalPosition() + imgPos;
        
        return uv;
    }

    void ImageCanvas::SetImageRotation(float rotation, bool radians)
    {
        constexpr float degreeConversion = (pi() / 180);
        CurrentRotation = radians ? rotation : degreeConversion * rotation;
        RedrawObject();
    }

    float ImageCanvas::GetImageRotation(bool radians) const
    {
        constexpr float degreeConversion = (pi() / 180);
        return radians ? CurrentRotation : degreeConversion * CurrentRotation;
    }

    void ImageCanvas::SetImageScale(float scale)
    {
        if(scale <= 0)
            return;

        CurrentZoom = scale;
        RedrawObject();
    }

    float ImageCanvas::GetImageScale() const
    {
        return CurrentZoom;
    }

    void ImageCanvas::SetDefaultMinZoom(float min)
    {
        MinZoom = min;
    }

    float ImageCanvas::GetDefaultMinZoom() const
    {
        return MinZoom;
    }

    void ImageCanvas::SetDefaultMaxZoom(float max)
    {
        MaxZoom = max;
    }

    float ImageCanvas::GetDefaultMaxZoom() const
    {
        return MaxZoom;
    }
    
    float ImageCanvas::GetOriginalImageScale() const
    {
        if(GetImageData() == nullptr)
            return -1;
        
        return MeasureScaleByHeight ? 
            GetImageData()->GetSize().y / GetSize().y :
            GetImageData()->GetSize().x / GetSize().x;
    }

    bool ImageCanvas::IsMeasureScaleByHeight() const
    {
        return MeasureScaleByHeight;
    }

    void ImageCanvas::SetMeasureScaleByHeight(bool scaleByHeight)
    {
        MeasureScaleByHeight = scaleByHeight;        
    }

    void ImageCanvas::SetImagePosition(glm::vec2 position)
    {
        CurrentPosition = position;
        RedrawObject();
    }

    glm::vec2 ImageCanvas::GetImagePosition() const
    {
        return CurrentPosition;
    }

    const std::vector<glm::vec2>& ImageCanvas::GetImageVertices() const
    {
        return ImageVertices;
    }

    float ImageCanvas::GetImageMinX() const
    {
        return ImageMinX;
    }

    float ImageCanvas::GetImageMaxX() const
    {
        return ImageMaxX;
    }

    float ImageCanvas::GetImageMinY() const
    {
        return ImageMinY;
    }

    float ImageCanvas::GetImageMaxY() const
    {
        return ImageMaxY;
    }

    void ImageCanvas::UseDefaultPanning(bool pan)
    {
        Panable = pan;
        RedrawObject();
    }

    bool ImageCanvas::IsUsingDefaultPanning() const
    {
        return Panable;
    }
    
    void ImageCanvas::UseDefaultZooming(bool zoom)
    {
        Zoomable = zoom;
        RedrawObject();
    }

    bool ImageCanvas::IsUsingDefaultZooming() const
    {
        return Zoomable;
    }

    void ImageCanvas::UseDefaultRotating(bool rotate)
    {
        Rotatable = rotate;
        RedrawObject();
    }

    bool ImageCanvas::IsUsingDefaultRotating() const
    {
        return Rotatable;
    }

    void ImageCanvas::SetShowHorizontalScrollbar(bool scrollbar)
    {
        ShowHorizontalScrollbar = scrollbar;
    }

    bool ImageCanvas::IsShowHorizontalScrollbar() const
    {
        return ShowHorizontalScrollbar;
    }

    void ImageCanvas::SetShowVerticalScrollbar(bool scrollbar)
    {
        ShowVerticalScrollbar = scrollbar;
    }

    bool ImageCanvas::IsShowVerticalScrollbar() const
    {
        return ShowVerticalScrollbar;
    }

    void ImageCanvas::SetHorizontalScrollbar(ssGUI::Scrollbar* scrollbar)
    {
        if(scrollbar == nullptr)
            SetShowHorizontalScrollbar(false);

        HorizontalScrollbar = CurrentObjectsReferences.AddObjectReference(scrollbar);

        glm::vec2 globalPos = scrollbar->GetGlobalPosition();
        scrollbar->SetParent(this, true);
        scrollbar->SetGlobalPosition(globalPos);
    }

    ssGUI::Scrollbar* ImageCanvas::GetHorizontalScrollbar() const
    {
        return static_cast<ssGUI::Scrollbar*>(CurrentObjectsReferences.GetObjectReference(HorizontalScrollbar));
    }

    void ImageCanvas::SetVerticalScrollbar(ssGUI::Scrollbar* scrollbar)
    {
        if(scrollbar == nullptr)
            SetShowVerticalScrollbar(false);

        VerticalScrollbar = CurrentObjectsReferences.AddObjectReference(scrollbar);

        glm::vec2 globalPos = scrollbar->GetGlobalPosition();
        scrollbar->SetParent(this, true);
        scrollbar->SetGlobalPosition(globalPos);
    }

    ssGUI::Scrollbar* ImageCanvas::GetVerticalScrollbar() const
    {
        return static_cast<ssGUI::Scrollbar*>(CurrentObjectsReferences.GetObjectReference(VerticalScrollbar));
    }

    ssGUI::Enums::ImageFitting ImageCanvas::GetFitting() const
    {
        return ssGUI::Enums::ImageFitting::FIT_HORIZONTAL;
    }

    void ImageCanvas::SetFitting(ssGUI::Enums::ImageFitting fitting)
    {
    }

    ssGUI::Enums::GUIObjectType ImageCanvas::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::IMAGE_CANVAS | ssGUI::Image::GetType();
    }

    ImageCanvas* ImageCanvas::Clone(bool cloneChildren)
    {
        ssLOG_FUNC_ENTRY();
        ImageCanvas* temp = new ImageCanvas(*this);
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