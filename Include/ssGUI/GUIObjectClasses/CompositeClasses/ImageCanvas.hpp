#ifndef SSGUI_IMAGE_CANVAS_H
#define SSGUI_IMAGE_CANVAS_H

#include "ssGUI/GUIObjectClasses/Widget.hpp"
#include "ssGUI/GUIObjectClasses/Image.hpp"

//namespace: ssGUI
namespace ssGUI
{
    class Image;
    class Scrollbar;

    //TODO: Allow to trigger default pan, zoom and rotate mode programmatically.

    /*class: ssGUI::ImageCanvas
    This class provides panning, rotating and zooming functionality to view the image. 
    It also provides position conversion between global and UV spaces.

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUIObjectIndex HorizontalScrollbar;               //See <GetHorizontalScrollbar>
        bool ShowHorizontalScrollbar;                       //See <IsShowHorizontalScrollbar>
        ssGUIObjectIndex VerticalScrollbar;                 //See <GetVerticalScrollbar>
        bool ShowVerticalScrollbar;                         //See <IsShowVerticalScrollbar>
        
        std::vector<glm::vec2> ImageVertices;               //See <GetImageVertices>
        float ImageMinX;                                    //See <GetImageMinGlobalX>
        float ImageMaxX;                                    //See <GetImageMaxGlobalX>
        float ImageMinY;                                    //See <GetImageMinGlobalY>
        float ImageMaxY;                                    //See <GetImageMaxGlobalY>
        float CurrentZoom;                                  //See <GetViewportZoom>
        glm::vec2 CurrentPosition;                          //See <GetViewportCenterPosition>
        float CurrentRotation;                              //See <GetViewportRotation>

        ssGUI::Enums::GenericButtonAndKeyInput PanKey;
        bool ScrollZoom;
        float MinScrollZoom;
        float MaxScrollZoom;
        float ScrollZoomAmount;
                                                            //TODO: Will have public method for this in the future
        ssGUI::Enums::GenericButtonAndKeyInput RotateKey;

        float OnRotateStartRotation;                        //(Internal variable) Start rotation when user is rotating the image
        glm::vec2 OnRotateStartPosition;                    //(Internal variable) Start position when user is rotating the image
        int InnerCircleId;                                  //(Internal variable) Shape ID for the inner circle when user is rotating
        int BackgroundCircleId;                             //(Internal variable) Shape ID for the background circle when user is rotating
        int OuterCircleId;                                  //(Internal variable) Shape ID for the outer circle when user is rotating

        bool MousePressed;                                  //(Internal variable) Used to indicate if user has clicked on this widget to pan the image
        glm::vec2 MouseButtonDownPosition;                  //(Internal variable) The initial position when the user clicked on this widget

        static int ImageCanvasObjectCount;                  //(Internal variable) Used for deallocating default resources
        static ssGUI::ImageData* DefaultRotationCursor;     //(Internal variable) Default rotation cursor when rotating an image
                                                            //TODO: Add ability to set it
    =================================================================
    ============================== C++ ==============================
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
        AddExtension<ssGUI::Extensions::MaskEnforcer>()->AddTargetMaskObject(this, {1, 2, 3, 4});
        
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
    
    int ImageCanvas::ImageCanvasObjectCount = 0;
    ssGUI::ImageData* ImageCanvas::DefaultRotationCursor = nullptr;
    =================================================================
    */
    class ImageCanvas : public Image
    {
        //TODO: Maybe implement pan bound at some point
    
        private:
            ImageCanvas& operator=(ImageCanvas const& other) = default;

        protected:
            ssGUIObjectIndex HorizontalScrollbar;               //See <GetHorizontalScrollbar>
            bool ShowHorizontalScrollbar;                       //See <IsShowHorizontalScrollbar>
            ssGUIObjectIndex VerticalScrollbar;                 //See <GetVerticalScrollbar>
            bool ShowVerticalScrollbar;                         //See <IsShowVerticalScrollbar>
            
            std::vector<glm::vec2> ImageVertices;               //See <GetImageVertices>
            float ImageMinX;                                    //See <GetImageMinGlobalX>
            float ImageMaxX;                                    //See <GetImageMaxGlobalX>
            float ImageMinY;                                    //See <GetImageMinGlobalY>
            float ImageMaxY;                                    //See <GetImageMaxGlobalY>
            float CurrentZoom;                                  //See <GetViewportZoom>
            glm::vec2 CurrentPosition;                          //See <GetViewportCenterPosition>
            float CurrentRotation;                              //See <GetViewportRotation>

            ssGUI::Enums::GenericButtonAndKeyInput PanKey;
            bool ScrollZoom;
            float MinScrollZoom;
            float MaxScrollZoom;
            float ScrollZoomAmount;
                                                                //TODO: Will have public method for this in the future
            ssGUI::Enums::GenericButtonAndKeyInput RotateKey;

            float OnRotateStartRotation;                        //(Internal variable) Start rotation when user is rotating the image
            glm::vec2 OnRotateStartPosition;                    //(Internal variable) Start position when user is rotating the image
            int InnerCircleId;                                  //(Internal variable) Shape ID for the inner circle when user is rotating
            int BackgroundCircleId;                             //(Internal variable) Shape ID for the background circle when user is rotating
            int OuterCircleId;                                  //(Internal variable) Shape ID for the outer circle when user is rotating

            bool MousePressed;                                  //(Internal variable) Used to indicate if user has clicked on this widget to pan the image
            glm::vec2 MouseButtonDownPosition;                  //(Internal variable) The initial position when the user clicked on this widget

            static int ImageCanvasObjectCount;                  //(Internal variable) Used for deallocating default resources
            static ssGUI::ImageData* DefaultRotationCursor;     //(Internal variable) Default rotation cursor when rotating an image
                                                                //TODO: Add ability to set it
            ImageCanvas(ImageCanvas const& other);

            //https://stackoverflow.com/questions/1727881/how-to-use-the-pi-constant-in-c
            constexpr inline static double pi() { return 3.1415926535897932384626433832795028841971; };

            //Return angle in radians. Positive if angle between a and b is clockwise
            virtual double GetAngle(glm::vec2 a, glm::vec2 b);

            virtual glm::vec2 RotateVec(glm::vec2 vec, float radians) const;

            virtual void ConstructRenderInfo() override;

            virtual void MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                                    ssGUI::GUIObject* mainWindow) override;
        public:
            //====================================================================
            //Group: Constants
            //====================================================================
            
            //string: ListenerKey
            static const std::string ListenerKey;

            ImageCanvas();
            virtual ~ImageCanvas() override;

            //====================================================================
            //Group: Viewport Manipulations
            //====================================================================

            //function: SetViewportRotation
            //Sets the rotation of the viewport, by default it uses degrees.
            //Please note that the viewport rotates around the center of itself.
            virtual void SetViewportRotation(float rotation, bool radians = false);

            //function: GetViewportRotation
            //Gets the rotation of the viewport, by default it uses degrees.
            //Please note that the viewport rotates around the center of itself.
            virtual float GetViewportRotation(bool radians = false) const;

            //function: SetViewportZoom
            //Sets the scale of the image in percentage (1 = 100%) of the image.
            virtual void SetViewportZoom(float scale);
            
            //function: GetViewportZoom
            //Gets the scale of the image in percentage (1 = 100%) of the image.
            virtual float GetViewportZoom() const;

            //function: SetViewportCenterPosition
            //Sets the viewport center position in UV space (scales from 0 to image size).
            //For example if it is set to (0, 0), the top left corner of the image would be in the middle of viewport.
            virtual void SetViewportCenterPosition(glm::vec2 position);

            //function: GetViewportCenterPosition
            //Gets the viewport center position in UV space (scales from 0 to image size).
            //For example if it is set to (0, 0), the top left corner of the image would be in the middle of viewport.
            virtual glm::vec2 GetViewportCenterPosition() const;

            //function: CenterViewportToImage
            //Sets the viewport center position to be at the center of the image.
            //Basically the same as 
            //> SetViewportCenterPosition(glm::vec2(GetImageData()->GetSize()) * 0.5f)
            virtual void CenterViewportToImage();
            
            //====================================================================
            //Group: UV/Global Space Translations
            //====================================================================
            
            //function: GetUVFromGlobalPosition
            //Gets the UV position (scales from 0 to image size) from global position.
            //Note that the UV position returned can go out of bound.
            virtual glm::vec2 GetUVFromGlobalPosition(glm::vec2 globalPos);
            
            //function: GetGlobalPositionFromUV
            //Gets the global position from the UV position (scales from 0 to image size)
            //Note that the global position returned can go outside of the widget.
            virtual glm::vec2 GetGlobalPositionFromUV(glm::vec2 uv);

            //function: GetViewportRightDirectionInUV
            //Gets the right facing direction (i.e. glm::vec2(1, 0)) from global space to UV space.
            //Please note that is is scaled with viewport zoom as well.
            virtual glm::vec2 GetViewportRightDirectionInUV() const;
            
            //function: GetViewportDownDirectionInUV
            //Gets the downward facing direction (i.e. glm::vec2(0, 1)) from global space to UV space.
            //Please note that is is scaled with viewport zoom as well.
            virtual glm::vec2 GetViewportDownDirectionInUV() const;
            
            //function: GetImageVertices
            //Gets all the vertices (from last frame or this frame depending on when you call this) of the image itself in global position
            virtual const std::vector<glm::vec2>& GetImageVertices() const;

            //function: GetImageMinGlobalX
            //Gets the minimum global X position of the image (from last frame or this frame depending on when you call this)
            virtual float GetImageMinGlobalX() const;
            
            //function: GetImageMaxGlobalX
            //Gets the maximum global X position of the image (from last frame or this frame depending on when you call this)
            virtual float GetImageMaxGlobalX() const;
            
            //function: GetImageMinGlobalY
            //Gets the minimum global Y position of the image (from last frame or this frame depending on when you call this)
            virtual float GetImageMinGlobalY() const;
            
            //function: GetImageMaxGlobalY
            //Gets the maximum global Y position of the image (from last frame or this frame depending on when you call this)
            virtual float GetImageMaxGlobalY() const;

            
            //====================================================================
            //Group: Viewport Interactions
            //====================================================================

            //function: SetPanKey
            //Sets the key to be used for panning the canvas. <ssGUI::Enums::NO_INPUT> to disable it.
            virtual void SetPanKey(ssGUI::Enums::GenericButtonAndKeyInput key);
            
            //function: GetPanKey
            //Gets the key to be used for panning the canvas. <ssGUI::Enums::NO_INPUT> if disabled.
            virtual ssGUI::Enums::GenericButtonAndKeyInput GetPanKey() const;
            
            //function: SetUseScrollZooming
            //Sets if the mouse scroll wheel is used for zooming the viewport
            virtual void SetUseScrollZooming(bool useScrollZoom);
            
            //function: IsUseScrollZooming
            //Returns if the mouse scroll wheel is used for zooming the viewport
            virtual bool IsUseScrollZooming() const;
            
            //function: SetMinScrollZoom
            //Sets the minimum zoom value for scroll zooming
            virtual void SetMinScrollZoom(float min);
            
            //function: GetMinScrollZoom
            //Returns the minimum zoom value for scroll zooming
            virtual float GetMinScrollZoom() const;
            
            //function: SetMaxScrollZoom
            //Sets the maximum zoom value for scroll zooming
            virtual void SetMaxScrollZoom(float max);
            
            //function: GetMaxScrollZoom
            //Returns the maximum zoom value for scroll zooming
            virtual float GetMaxScrollZoom() const;
            
            //function: SetRotateKey
            //Sets the key to be used for rotating the canvas. <ssGUI::Enums::NO_INPUT> to disable it.
            virtual void SetRotateKey(ssGUI::Enums::GenericButtonAndKeyInput key);
            
            //function: GetRotateKey
            //Returns the key to be used for rotating the canvas. <ssGUI::Enums::NO_INPUT> if disabled.
            virtual ssGUI::Enums::GenericButtonAndKeyInput GetRotateKey() const;
            
            //function: SetShowHorizontalScrollbar
            //Sets if the horizontal scrollbar is available to use when the image is zoomed larger than the ImageCanvas
            virtual void SetShowHorizontalScrollbar(bool scrollbar);
            
            //function: IsShowHorizontalScrollbar
            //Returns if the horizontal scrollbar is available to use when the image is zoomed larger than the ImageCanvas
            virtual bool IsShowHorizontalScrollbar() const;

            //function: SetShowVerticalScrollbar
            //Sets if the vertical scrollbar is available to use when the image is zoomed larger than the ImageCanvas
            virtual void SetShowVerticalScrollbar(bool scrollbar);
            
            //function: IsShowVerticalScrollbar
            //Returns if the vertical scrollbar is available to use when the image is zoomed larger than the ImageCanvas
            virtual bool IsShowVerticalScrollbar() const;

            //function: SetHorizontalScrollbar
            //Sets the horizontal scrollbar object used by ImageCanvas.
            //Pass in nullptr for unsetting (and deleting) the horizontal scrollbar.
            virtual void SetHorizontalScrollbar(ssGUI::Scrollbar* scrollbar);
            
            //function: GetHorizontalScrollbar
            //Gets the horizontal scrollbar object used by ImageCanvas.
            //Nullptr if horizontal scrollbar is unset.
            virtual ssGUI::Scrollbar* GetHorizontalScrollbar() const;

            //function: SetVerticalScrollbar
            //Sets the vertical scrollbar object used by ImageCanvas.
            //Pass in nullptr for unsetting (and deleting) the vertical scrollbar.
            virtual void SetVerticalScrollbar(ssGUI::Scrollbar* scrollbar);
            
            //function: GetVerticalScrollbar
            //Gets the vertical scrollbar object used by ImageCanvas.
            //Nullptr if vertical scrollbar is unset.
            virtual ssGUI::Scrollbar* GetVerticalScrollbar() const;

            //====================================================================
            //Group: Overrides
            //====================================================================

            //function: GetFitting
            //*Fitting has no effect on <ImageCanvas>*
            virtual ssGUI::Enums::ImageFitting GetFitting() const override;

            //function: SetFitting
            //*Fitting has no effect on <ImageCanvas>*
            virtual void SetFitting(ssGUI::Enums::ImageFitting fitting) override;

            //function: SetImageData
            //Setting image data will automatically call <CenterViewportToImage>.
            //For description of SetImageData, see <ssGUI::Image::SetImageData>
            virtual void SetImageData(ssGUI::ImageData* imageData) override;

            //function: GetType
            //See <ssGUI::Widget::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: Clone
            //See <Widget::Clone>
            virtual ImageCanvas* Clone(bool cloneChildren) override;
    
            //function: InitiateDefaultResources
            //See <GUIObject::InitiateDefaultResources>
            virtual void InitiateDefaultResources() override;
            
            //function: CleanUpDefaultResources
            //See <GUIObject::CleanUpDefaultResources>
            virtual void CleanUpDefaultResources() override;
    };
}

#endif