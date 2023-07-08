#include "ssGUI/GUIObjectClasses/CompositeClasses/ImageCanvas.hpp"

#include "ssGUI/DataClasses/RealtimeInputInfo.hpp"
#include "ssGUI/EmbeddedResources.h"
#include "ssGUI/Extensions/Mask.hpp"
#include "ssGUI/Extensions/MaskEnforcer.hpp"
#include "ssGUI/Extensions/Border.hpp"
#include "ssGUI/Extensions/Shape.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/Scrollbar.hpp"
#include "ssGUI/HeaderGroups/InputGroup.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

#include "glm/geometric.hpp"
#include <cmath>

namespace ssGUI
{
    int ImageCanvas::ImageCanvasObjectCount = 0;
    ssGUI::ImageData* ImageCanvas::DefaultRotationCursor = nullptr;

    ImageCanvas::ImageCanvas(ImageCanvas const& other) : Image(other)
    {
        HorizontalScrollbar = other.HorizontalScrollbar;
        ShowHorizontalScrollbar = other.ShowVerticalScrollbar;
        VerticalScrollbar = other.VerticalScrollbar;
        ShowVerticalScrollbar = other.ShowVerticalScrollbar;

        ImageVertices = other.ImageVertices;
        ImageMinX = other.ImageMinX;
        ImageMaxX = other.ImageMaxX;
        ImageMinY = other.ImageMinY;
        ImageMaxY = other.ImageMaxY;
        CurrentZoom = other.CurrentZoom;
        CurrentPosition = other.CurrentPosition;
        CurrentRotation = other.CurrentRotation;
        
        PanKey = other.PanKey;
        ScrollZoom = other.ScrollZoom;
        MinScrollZoom = other.MinScrollZoom;
        MaxScrollZoom = other.MaxScrollZoom;
        ScrollZoomAmount = other.ScrollZoomAmount;
        
        RotateKey = other.RotateKey;
        
        OnRotateStartRotation = other.OnRotateStartRotation;
        OnRotateStartPosition = other.OnRotateStartPosition;
        InnerCircleId = other.InnerCircleId;
        BackgroundCircleId = other.BackgroundCircleId;
        OuterCircleId = other.OuterCircleId;

        MousePressed = other.MousePressed;
        MouseButtonDownPosition = other.MouseButtonDownPosition;
        
        ImageCanvasObjectCount++;
    }

    double ImageCanvas::GetAngle(glm::vec2 a, glm::vec2 b)
    {
        glm::vec3 a3 = glm::vec3(a, 0);
        glm::vec3 b3 = glm::vec3(b, 0);

        return atan2(glm::cross(a3, b3).z, glm::dot(glm::vec2(a), glm::vec2(b)));
    }

    glm::vec2 ImageCanvas::RotateVec(glm::vec2 vec, float radians) const
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
        {
            if(GetHorizontalScrollbar() != nullptr)
                GetHorizontalScrollbar()->SetEnabled(false);
            
            if(GetVerticalScrollbar() != nullptr)
                GetVerticalScrollbar()->SetEnabled(false);

            return;
        }

        //TODO: The code below can be moved to its own function
        ssGUI::DrawingEntity imageEntity;
        imageEntity.EntityName = IMAGE_CANVAS_IMAGE_SHAPE_NAME;
        imageEntity.Colors.push_back(ImageTint);
        imageEntity.Colors.push_back(ImageTint);
        imageEntity.Colors.push_back(ImageTint);
        imageEntity.Colors.push_back(ImageTint);
        
        //glm::vec2 imgDrawPosition = GetGlobalPosition();
        glm::vec2 imgSize = GetImageData()->GetSize();

        glm::vec2 imgVertices[4] { glm::vec2() };
        
        imgVertices[0] = glm::vec2(0, 0);
        imgVertices[1] = glm::vec2(imgSize.x, 0);
        imgVertices[2] = glm::vec2(imgSize.x, imgSize.y);
        imgVertices[3] = glm::vec2(0, imgSize.y);

        //Position Image
        imgVertices[0] -= CurrentPosition;
        imgVertices[1] -= CurrentPosition;
        imgVertices[2] -= CurrentPosition;
        imgVertices[3] -= CurrentPosition;
        
        //Rotate Image
        imgVertices[0] = RotateVec(imgVertices[0], -CurrentRotation);
        imgVertices[1] = RotateVec(imgVertices[1], -CurrentRotation);
        imgVertices[2] = RotateVec(imgVertices[2], -CurrentRotation);
        imgVertices[3] = RotateVec(imgVertices[3], -CurrentRotation);
        
        //Scale Image
        imgVertices[0] *= CurrentZoom;
        imgVertices[1] *= CurrentZoom;
        imgVertices[2] *= CurrentZoom;
        imgVertices[3] *= CurrentZoom;
        
        //Offset
        glm::vec2 canvasHalfSize = GetSize() / 2.f;
        imgVertices[0] += drawPosition + canvasHalfSize;
        imgVertices[1] += drawPosition + canvasHalfSize;
        imgVertices[2] += drawPosition + canvasHalfSize;
        imgVertices[3] += drawPosition + canvasHalfSize;

        imageEntity.Vertices.push_back(imgVertices[0]);
        imageEntity.Vertices.push_back(imgVertices[1]);
        imageEntity.Vertices.push_back(imgVertices[2]);
        imageEntity.Vertices.push_back(imgVertices[3]);

        imageEntity.BackendImage = ImageData->GetBackendImageInterface();

        DrawingEntities.push_back(imageEntity);

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
    const std::string ImageCanvas::IMAGE_CANVAS_IMAGE_SHAPE_NAME = "Image Canvas Image";

    void ImageCanvas::MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                                ssGUI::GUIObject* mainWindow)
    {        
        //By default don't show rotation UI
        auto shape = GetAnyExtension<ssGUI::Extensions::Shape>();
        
        if(!IsInteractable() || !IsBlockInput())
            return;

        //No functionality if mouse input is blocked
        if(inputStatus.MouseInputBlockedObject != nullptr && inputStatus.MouseInputBlockedObject != this && !MousePressed)
            return;

        glm::ivec2 currentMousePos = inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)->GetBackendWindowInterface());
        bool mouseWithinWidget =    currentMousePos.x >= GetGlobalPosition().x &&
                                    currentMousePos.y >= GetGlobalPosition().y &&
                                    currentMousePos.x <= GetGlobalPosition().x + GetSize().x &&
                                    currentMousePos.y <= GetGlobalPosition().y + GetSize().y;

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

            //If space key is pressed, use delta cursor position to move canvas
            if(inputInterface->IsButtonOrKeyPressExistCurrentFrame(GetPanKey()))
            {
                inputStatus.KeyInputBlockedObject = this;
                
                if(MousePressed || mouseWithinWidget)
                    inputInterface->SetCursorType(ssGUI::Enums::CursorType::MOVE);
                
                if(MousePressed)
                {
                    glm::vec2 lastMousePos = inputInterface->GetLastMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)->GetBackendWindowInterface());
                    glm::vec2 diff = GetUVFromGlobalPosition(currentMousePos) - GetUVFromGlobalPosition(lastMousePos);
                    SetViewportCenterPosition(GetViewportCenterPosition() - diff);
                    inputStatus.MouseInputBlockedObject = this;
                }

                SetFocus(true);
            }

            //If r key is pressed, use start cursor position and current cursor position to rotate canvas
            else if(inputInterface->IsButtonOrKeyPressExistCurrentFrame(GetRotateKey()))
            {
                inputStatus.KeyInputBlockedObject = this;
                SetFocus(true);

                if(MousePressed || mouseWithinWidget)
                {
                    //Load rotation cursor if it wasn't loaded
                    if(!inputInterface->HasCustomCursor("RotationCursor"))
                    {
                        InitiateDefaultResources();
                        if(DefaultRotationCursor == nullptr)
                        {
                            ssGUI_WARNING(ssGUI_GUI_OBJECT_TAG, "Failed to load rotation cursor");
                        }
                        else
                        {
                            inputInterface->CreateCustomCursor(DefaultRotationCursor->GetBackendImageInterface(), "RotationCursor", glm::ivec2(30, 30), glm::ivec2(15, 15));
                        }
                    }

                    if(inputInterface->HasCustomCursor("RotationCursor"))
                    {
                        inputInterface->SetCurrentCustomCursor("RotationCursor");
                        inputInterface->SetCursorType(ssGUI::Enums::CursorType::CUSTOM);
                    }
                }

                //Show rotation gui when this is the first frame when the R button is pressed
                if(shape != nullptr && !inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::LetterKey::R))
                {
                    glm::vec2 outerCircleSize = glm::vec2(9, 9);
                    glm::vec2 bgCircleSize = glm::vec2(6, 6);
                    glm::vec2 innerCircleSize = glm::vec2(3, 3);
                    
                    //TODO: Set rotation cursor to be center
                    glm::vec2 center = GetSize() * 0.5f;
                    shape->SetAdditionalCircle(BackgroundCircleId, center - bgCircleSize/2.f, bgCircleSize, 
                        glm::u8vec4(255, 255, 255, 255), false);                

                    shape->SetAdditionalCircle(InnerCircleId, center - innerCircleSize/2.f, innerCircleSize, 
                        glm::u8vec4(0, 0, 0, 255), false);

                    shape->SetAdditionalCircle(OuterCircleId, center - outerCircleSize/2.f, outerCircleSize, 
                        glm::u8vec4(0, 0, 0, 255), false);
                }

                if(MousePressed)
                {
                    if(!inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::LEFT))
                    {
                        OnRotateStartRotation = GetViewportRotation(true);
                        OnRotateStartPosition = GetViewportCenterPosition();
                    }
                    else
                    {
                        glm::vec2 canvasCenter = GetGlobalPosition() + GetSize() * 0.5f;
                        SetViewportRotation(OnRotateStartRotation + GetAngle(glm::vec2(currentMousePos) - canvasCenter, MouseButtonDownPosition - canvasCenter), true);
                    }
                    inputStatus.MouseInputBlockedObject = this;
                }
            }

            //Hide rotation circle when R button is not pressed
            else if(inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::LetterKey::R) &&
                    !inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::LetterKey::R))
            {
                if(shape != nullptr)
                {
                    shape->SetAdditionalCircle(BackgroundCircleId, glm::vec2(), glm::vec2(), glm::u8vec4(), false);
                    shape->SetAdditionalCircle(InnerCircleId, glm::vec2(), glm::vec2(), glm::u8vec4(), false);
                    shape->SetAdditionalCircle(OuterCircleId, glm::vec2(), glm::vec2(), glm::u8vec4(), false);
                }
            }
        }
    
        //Handle scrolling for image zooming
        if(IsUseScrollZooming() && mouseWithinWidget)
        {
            inputStatus.MouseInputBlockedObject = this;

            //Scrolling up
            if(inputInterface->GetCurrentMouseScrollDelta().y > 0)
            {
                SetFocus(true);
                SetViewportZoom(GetViewportZoom() + ScrollZoomAmount > GetMaxScrollZoom() ? GetMaxScrollZoom() : GetViewportZoom() + ScrollZoomAmount);   
            }
            //Scrolling down
            else if(inputInterface->GetCurrentMouseScrollDelta().y)
            {
                SetFocus(true);
                SetViewportZoom(GetViewportZoom() - ScrollZoomAmount < GetMinScrollZoom() ? GetMinScrollZoom() : GetViewportZoom() - ScrollZoomAmount);
            }
        }
    }

    #define SSGUI_CAST_KEY(x) static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(x)

    ImageCanvas::ImageCanvas() :    HorizontalScrollbar(-1),
                                    ShowHorizontalScrollbar(true),
                                    VerticalScrollbar(-1),
                                    ShowVerticalScrollbar(true),
                                    ImageVertices(),
                                    ImageMinX(0),
                                    ImageMaxX(0),
                                    ImageMinY(0),
                                    ImageMaxY(0),
                                    CurrentZoom(1),
                                    CurrentPosition(0.5, 0.5),
                                    CurrentRotation(0),
                                    PanKey(SSGUI_CAST_KEY(ssGUI::Enums::SystemKey::SPACE)),
                                    ScrollZoom(true),
                                    MinScrollZoom(0),
                                    MaxScrollZoom(10),
                                    ScrollZoomAmount(0.1),
                                    RotateKey(SSGUI_CAST_KEY(ssGUI::Enums::LetterKey::R)),
                                    OnRotateStartRotation(0),
                                    OnRotateStartPosition(),
                                    InnerCircleId(0),
                                    BackgroundCircleId(0),
                                    OuterCircleId(0),
                                    MousePressed(false),
                                    MouseButtonDownPosition()
    {
        std::vector<ssGUI::TargetShape> maskShapes;
        maskShapes.push_back(ssGUI::TargetShape(IMAGE_CANVAS_IMAGE_SHAPE_NAME));
        AddExtension<ssGUI::Extensions::MaskEnforcer>()->AddTargetMaskObject(this, maskShapes);
        
        ssGUI::Extensions::Mask* mask = AddExtension<ssGUI::Extensions::Mask>();
        mask->SetMaskChildren(false);
        mask->SetMaskContainer(true);
    
        ssGUI::Extensions::Shape* shape = AddExtension<ssGUI::Extensions::Shape>();
        OuterCircleId = shape->AddAdditionalCircle(glm::vec2(), glm::vec2(), glm::u8vec4(), false);
        BackgroundCircleId = shape->AddAdditionalCircle(glm::vec2(), glm::vec2(), glm::u8vec4(), false);
        InnerCircleId = shape->AddAdditionalCircle(glm::vec2(), glm::vec2(), glm::u8vec4(), false);    

        AddExtension<ssGUI::Extensions::Border>();

        auto hScrollbar = ssGUI::Factory::Create<ssGUI::Scrollbar>();
        hScrollbar->SetUserCreated(false);
        hScrollbar->SetVertical(false, true);
        auto ecb = hScrollbar->AddEventCallback(ssGUI::Enums::EventType::SCROLLBAR_VALUE_CHANGED_VIA_GUI);
        ssGUIObjectIndex index = ecb->AddObjectReference(this);
        ecb->AddEventListener
        (
            ListenerKey, this,
            [index](ssGUI::EventInfo info)
            {
                auto imageCanvas = static_cast<ssGUI::ImageCanvas*>(info.References->GetObjectReference(index));
                if(imageCanvas == nullptr)
                    return;

                //Find min and max x
                float minX = imageCanvas->GetImageMinGlobalX();
                float maxX = imageCanvas->GetImageMaxGlobalX();
                auto scrollbar = static_cast<ssGUI::Scrollbar*>(info.EventSource);
                
                //Get how much do we need to move in UI(Global) space
                float targetX = ((maxX - minX) - imageCanvas->GetSize().x) * scrollbar->GetScrollbarValue() + minX;
                float offsetX = targetX - imageCanvas->GetGlobalPosition().x;
                glm::vec2 currentViewportPos = imageCanvas->GetViewportCenterPosition();
                
                //Finally get the viewport horizontal direction in UV space and multiply by how much we need move
                imageCanvas->SetViewportCenterPosition(currentViewportPos + imageCanvas->GetViewportRightDirectionInUV() * offsetX);
            }
        );

        auto as = hScrollbar->AddExtension<ssGUI::Extensions::AdvancedSize>();
        auto ap = hScrollbar->AddExtension<ssGUI::Extensions::AdvancedPosition>();
        as->SetHorizontalPercentage(1);
        as->SetHorizontalPixel(-hScrollbar->GetSize().y);
        as->SetVerticalPixel(hScrollbar->GetSize().y);
        ap->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::LEFT);
        ap->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::BOTTOM);

        auto vScrollbar = ssGUI::Factory::Create<ssGUI::Scrollbar>();
        vScrollbar->SetUserCreated(false);
        vScrollbar->SetVertical(true, true);
        ecb = vScrollbar->AddEventCallback(ssGUI::Enums::EventType::SCROLLBAR_VALUE_CHANGED_VIA_GUI);
        index = ecb->AddObjectReference(this);
        ecb->AddEventListener
        (
            ListenerKey, this,
            [index](ssGUI::EventInfo info)
            {
                auto imageCanvas = static_cast<ssGUI::ImageCanvas*>(info.References->GetObjectReference(index));
                if(imageCanvas == nullptr)
                    return;

                //Find min and max y
                float minY = imageCanvas->GetImageMinGlobalY();
                float maxY = imageCanvas->GetImageMaxGlobalY();
                auto scrollbar = static_cast<ssGUI::Scrollbar*>(info.EventSource);

                //Get how much do we need to move in UI(Global) space
                float targetY = ((maxY - minY) - imageCanvas->GetSize().y) * scrollbar->GetScrollbarValue() + minY;
                float offsetY = targetY - imageCanvas->GetGlobalPosition().y;
                glm::vec2 currentViewportPos = imageCanvas->GetViewportCenterPosition();
                
                //Finally get the viewport horizontal direction in UV space and multiply by how much we need move
                imageCanvas->SetViewportCenterPosition(currentViewportPos + imageCanvas->GetViewportDownDirectionInUV() * offsetY);
            }
        );

        as = vScrollbar->AddExtension<ssGUI::Extensions::AdvancedSize>();
        ap = vScrollbar->AddExtension<ssGUI::Extensions::AdvancedPosition>();
        as->SetHorizontalPixel(vScrollbar->GetSize().x);
        as->SetVerticalPercentage(1);
        as->SetVerticalPixel(-vScrollbar->GetSize().x);
        ap->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::RIGHT);
        ap->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::TOP);

        hScrollbar->SetParent(this, true);
        vScrollbar->SetParent(this, true);

        HorizontalScrollbar = CurrentObjectsReferences.GetObjectIndex(hScrollbar);
        VerticalScrollbar = CurrentObjectsReferences.GetObjectIndex(vScrollbar);
        ImageCanvasObjectCount++;
        
        AddEventCallback(ssGUI::Enums::EventType::BEFORE_OBJECT_DESTROY)->AddEventListener
        (
            ListenerKey,
            this,
            [](ssGUI::EventInfo info)
            {
                auto* imageCanvas = static_cast<ssGUI::ImageCanvas*>(info.Container);
                
                ssGUI::ImageCanvas::ImageCanvasObjectCount--;
        
                if(ssGUI::ImageCanvas::ImageCanvasObjectCount == 0)
                    imageCanvas->CleanUpDefaultResources();
            }
        );
    }

    ImageCanvas::~ImageCanvas()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
        
        //If the object deallocation is not handled by ssGUIManager
        if(!Internal_IsDeleted())
            Internal_ChildrenManualDeletion(std::vector<ssGUI::ssGUIObjectIndex>{HorizontalScrollbar, VerticalScrollbar});
    }

    void ImageCanvas::SetViewportRotation(float rotation, bool radians)
    {
        constexpr float degreeConversion = (180 / pi());
        CurrentRotation = radians ? rotation : degreeConversion * rotation;
        RedrawObject();
    }

    float ImageCanvas::GetViewportRotation(bool radians) const
    {
        constexpr float degreeConversion = (180 / pi());
        return radians ? CurrentRotation : degreeConversion * CurrentRotation;
    }
    
    void ImageCanvas::SetViewportZoom(float scale)
    {
        if(scale <= 0)
            return;

        CurrentZoom = scale;
        RedrawObject();
    }

    float ImageCanvas::GetViewportZoom() const
    {
        return CurrentZoom;
    }
    
    void ImageCanvas::SetViewportCenterPosition(glm::vec2 position)
    {
        CurrentPosition = position;
        RedrawObject();
    }
    
    void ImageCanvas::CenterViewportToImage()
    {
        if(GetImageData() != nullptr)
            SetViewportCenterPosition(glm::vec2(GetImageData()->GetSize()) * 0.5f);        
    }

    glm::vec2 ImageCanvas::GetViewportCenterPosition() const
    {
        return CurrentPosition;
    }
    
    glm::vec2 ImageCanvas::GetUVFromGlobalPosition(glm::vec2 globalPos)
    {
        //Transform global position to local position
        globalPos -= GetGlobalPosition();
        
        //Transform the origin to the center of the viewport
        globalPos -= GetSize() * 0.5f;
        
        //Rotate
        globalPos = RotateVec(globalPos, CurrentRotation);
        
        //Scale
        globalPos /= GetViewportZoom();
        
        //Viewport Position
        globalPos += GetViewportCenterPosition();

        //UV
        return globalPos;
    }

    glm::vec2 ImageCanvas::GetGlobalPositionFromUV(glm::vec2 uv)
    {
        //Canvas Position
        uv -= GetViewportCenterPosition();
        
        //Scale
        uv *= GetViewportZoom();
        
        //Rotate
        uv = RotateVec(uv, -CurrentRotation);
        
        //Transform the origin to the top-left of the viewport
        uv += GetSize() * 0.5f;
        
        //Transform local position to global
        uv += GetGlobalPosition();
        
        return uv;
    }
    
    glm::vec2 ImageCanvas::GetViewportRightDirectionInUV() const
    {
        return RotateVec(glm::vec2(1, 0), CurrentRotation) / CurrentZoom; 
    }
    
    glm::vec2 ImageCanvas::GetViewportDownDirectionInUV() const
    {
        return RotateVec(glm::vec2(0, 1), CurrentRotation) / CurrentZoom; 
    }

    const std::vector<glm::vec2>& ImageCanvas::GetImageVertices() const
    {
        return ImageVertices;
    }

    float ImageCanvas::GetImageMinGlobalX() const
    {
        return ImageMinX;
    }

    float ImageCanvas::GetImageMaxGlobalX() const
    {
        return ImageMaxX;
    }

    float ImageCanvas::GetImageMinGlobalY() const
    {
        return ImageMinY;
    }

    float ImageCanvas::GetImageMaxGlobalY() const
    {
        return ImageMaxY;
    }
    
    void ImageCanvas::SetPanKey(ssGUI::Enums::GenericButtonAndKeyInput key)
    {
        PanKey = key;
    }
    
    ssGUI::Enums::GenericButtonAndKeyInput ImageCanvas::GetPanKey() const
    {
        return PanKey;
    }
    
    void ImageCanvas::SetUseScrollZooming(bool useScrollZoom)
    {
        ScrollZoom = useScrollZoom;
    }
    
    bool ImageCanvas::IsUseScrollZooming() const
    {
        return ScrollZoom;
    }
    
    void ImageCanvas::SetMinScrollZoom(float min)
    {
        MinScrollZoom = min;
    }
    
    float ImageCanvas::GetMinScrollZoom() const
    {
        return MinScrollZoom;
    }
    
    void ImageCanvas::SetMaxScrollZoom(float max)
    {
        MaxScrollZoom = max;
    }
    
    float ImageCanvas::GetMaxScrollZoom() const
    {
        return MaxScrollZoom;
    }
    
    void ImageCanvas::SetRotateKey(ssGUI::Enums::GenericButtonAndKeyInput key)
    {
        RotateKey = key;
    }
    
    ssGUI::Enums::GenericButtonAndKeyInput ImageCanvas::GetRotateKey() const
    {
        return RotateKey;
    }

    void ImageCanvas::SetShowHorizontalScrollbar(bool scrollbar)
    {
        ShowHorizontalScrollbar = scrollbar;
        RedrawObject();
    }

    bool ImageCanvas::IsShowHorizontalScrollbar() const
    {
        return ShowHorizontalScrollbar;
    }

    void ImageCanvas::SetShowVerticalScrollbar(bool scrollbar)
    {
        ShowVerticalScrollbar = scrollbar;
        RedrawObject();
    }

    bool ImageCanvas::IsShowVerticalScrollbar() const
    {
        return ShowVerticalScrollbar;
    }

    void ImageCanvas::SetHorizontalScrollbar(ssGUI::Scrollbar* scrollbar)
    {
        if(scrollbar == nullptr)
        {
            if(GetHorizontalScrollbar() != nullptr)
                GetHorizontalScrollbar()->Delete();
            
            SetShowHorizontalScrollbar(false);
            HorizontalScrollbar = -1;
            return;
        }

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
        {
            if(GetVerticalScrollbar() != nullptr)
                GetVerticalScrollbar()->Delete();
        
            SetShowVerticalScrollbar(false);
            VerticalScrollbar = -1;
            return;
        }

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
    
    void ImageCanvas::SetImageData(ssGUI::ImageData* imageData)
    {
        ssGUI::Image::SetImageData(imageData);
        CenterViewportToImage();
    }
    
    void ImageCanvas::SetInteractable(bool interactable)
    {
        if(GetHorizontalScrollbar() != nullptr)
            GetHorizontalScrollbar()->SetInteractable(interactable);
        
        if(GetVerticalScrollbar() != nullptr)
            GetVerticalScrollbar()->SetInteractable(interactable);
        
        Image::SetInteractable(interactable);
    }

    void ImageCanvas::SetBlockInput(bool blockInput)
    {
        if(GetHorizontalScrollbar() != nullptr)
            GetHorizontalScrollbar()->SetBlockInput(blockInput);
        
        if(GetVerticalScrollbar() != nullptr)
            GetVerticalScrollbar()->SetBlockInput(blockInput);
        
        Image::SetBlockInput(blockInput);
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
    
    void ImageCanvas::InitiateDefaultResources()
    {
        if(DefaultRotationCursor == nullptr)
        {
            auto data = ssGUI::Factory::Create<ssGUI::ImageData>();
            const uint8_t* fileContent = RotationCursor;
            size_t fileSize = RotationCursor_size;
            
            if(fileContent == nullptr)
            {
                ssGUI_WARNING(ssGUI_GUI_OBJECT_TAG, "Failed to load embedded rotation cursor");
                ssGUI::Factory::Dispose(data);
                return;
            }
            
            if(!data->LoadImgFileFromMemory(fileContent, fileSize))
            {
                ssGUI_WARNING(ssGUI_GUI_OBJECT_TAG, "Failed to load rotation cursor");
                ssGUI::Factory::Dispose(data);
                return;
            }
            else
                DefaultRotationCursor = data;
        }
    }
    
    void ImageCanvas::CleanUpDefaultResources()
    {
        if(DefaultRotationCursor != nullptr)
        {
            ssGUI::Dispose(DefaultRotationCursor);
            DefaultRotationCursor = nullptr;
        }       
    }
}