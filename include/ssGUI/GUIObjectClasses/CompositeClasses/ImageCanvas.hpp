#ifndef SSGUI_IMAGE_CANVAS
#define SSGUI_IMAGE_CANVAS

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
        ssGUIObjectIndex HorizontalScrollbar = -1;          //See <GetHorizontalScrollbar>
        bool ShowHorizontalScrollbar = true;                //See <IsShowHorizontalScrollbar>
        ssGUIObjectIndex VerticalScrollbar = -1;            //See <GetVerticalScrollbar>
        bool ShowVerticalScrollbar = true;                  //See <IsShowVerticalScrollbar>
        
        std::vector<glm::vec2> ImageVertices = {};          //See <GetImageVertices>
        float ImageMinX = 0;                                //See <GetImageMinX>
        float ImageMaxX = 0;                                //See <GetImageMaxX>
        float ImageMinY = 0;                                //See <GetImageMinY>
        float ImageMaxY = 0;                                //See <GetImageMaxY>

        bool Panable = true;                                //See <IsUsingDefaultPanning>
        glm::vec2 CurrentPosition = glm::vec2(0.5, 0.5);    //See <GetImagePosition>
        //TODO: Maybe implement these at some point
        // float HorizontalPanBound = 0;
        // float VerticalPanBound = 0;

        bool Zoomable = true;                               //See <IsUsingDefaultZooming>
        float MinZoom = 0;                                  //See <GetDefaultMinZoom>
        float MaxZoom = 10;                                 //See <GetDefaultMaxZoom>
        float CurrentZoom = 1;                              //See <GetImageScale>
        float ZoomAmount = 0.1;                             //(Internal variable) Amount of zoom.
                                                            //TODO: Will have public method for this in the future
        bool MeasureScaleByHeight = true;                   //See <IsMeasureScaleByHeight>

        bool Rotatable = true;                              //See <IsUsingDefaultRotating>
        float CurrentRotation = 0;                          //See <GetImageRotation>
        float OnRotateStartRotation = 0;                    //(Internal variable) Start rotation when user is rotating the image
        glm::vec2 OnRotateStartPosition = glm::vec2();      //(Internal variable) Start position when user is rotating the image
        int InnerCircleId = 0;                              //(Internal variable) Shape ID for the inner circle when user is rotating
        int BackgroundCircleId = 0;                         //(Internal variable) Shape ID for the background circle when user is rotating
        int OuterCircleId = 0;                              //(Internal variable) Shape ID for the outer circle when user is rotating

        bool MousePressed = false;                          //(Internal variable) Used to indicate if user has clicked on this widget to pan the image
        glm::vec2 MouseButtonDownPosition = glm::vec2();    //(Internal variable) The initial position when the user clicked on this widget
    =================================================================
    ============================== C++ ==============================
    ImageCanvas::ImageCanvas()
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
            [index](ssGUI::GUIObject* source, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* references)
            {
                auto imageCanvas = static_cast<ssGUI::ImageCanvas*>(references->GetObjectReference(index));
                if(imageCanvas == nullptr)
                    return;

                //Find min and max x
                float minX = imageCanvas->GetImageMinX();
                float maxX = imageCanvas->GetImageMaxX();

                auto scrollbar = static_cast<ssGUI::Scrollbar*>(source);
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
            [index](ssGUI::GUIObject* source, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* references)
            {
                auto imageCanvas = static_cast<ssGUI::ImageCanvas*>(references->GetObjectReference(index));
                if(imageCanvas == nullptr)
                    return;

                //Find min and max y
                float minY = imageCanvas->GetImageMinY();
                float maxY = imageCanvas->GetImageMaxY();

                auto scrollbar = static_cast<ssGUI::Scrollbar*>(source);
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

        hScrollbar->SetParent(this);
        vScrollbar->SetParent(this);

        HorizontalScrollbar = CurrentObjectsReferences.GetObjectIndex(hScrollbar);
        VerticalScrollbar = CurrentObjectsReferences.GetObjectIndex(vScrollbar);
    }
    =================================================================
    */
    class ImageCanvas : public Image
    {
        private:
            ImageCanvas& operator=(ImageCanvas const& other) = default;

        protected:
            ssGUIObjectIndex HorizontalScrollbar = -1;          //See <GetHorizontalScrollbar>
            bool ShowHorizontalScrollbar = true;                //See <IsShowHorizontalScrollbar>
            ssGUIObjectIndex VerticalScrollbar = -1;            //See <GetVerticalScrollbar>
            bool ShowVerticalScrollbar = true;                  //See <IsShowVerticalScrollbar>
            
            std::vector<glm::vec2> ImageVertices = {};          //See <GetImageVertices>
            float ImageMinX = 0;                                //See <GetImageMinX>
            float ImageMaxX = 0;                                //See <GetImageMaxX>
            float ImageMinY = 0;                                //See <GetImageMinY>
            float ImageMaxY = 0;                                //See <GetImageMaxY>

            bool Panable = true;                                //See <IsUsingDefaultPanning>
            glm::vec2 CurrentPosition = glm::vec2(0.5, 0.5);    //See <GetImagePosition>
            //TODO: Maybe implement these at some point
            // float HorizontalPanBound = 0;
            // float VerticalPanBound = 0;

            bool Zoomable = true;                               //See <IsUsingDefaultZooming>
            float MinZoom = 0;                                  //See <GetDefaultMinZoom>
            float MaxZoom = 10;                                 //See <GetDefaultMaxZoom>
            float CurrentZoom = 1;                              //See <GetImageScale>
            float ZoomAmount = 0.1;                             //(Internal variable) Amount of zoom.
                                                                //TODO: Will have public method for this in the future
            bool MeasureScaleByHeight = true;                   //See <IsMeasureScaleByHeight>

            bool Rotatable = true;                              //See <IsUsingDefaultRotating>
            float CurrentRotation = 0;                          //See <GetImageRotation>
            float OnRotateStartRotation = 0;                    //(Internal variable) Start rotation when user is rotating the image
            glm::vec2 OnRotateStartPosition = glm::vec2();      //(Internal variable) Start position when user is rotating the image
            int InnerCircleId = 0;                              //(Internal variable) Shape ID for the inner circle when user is rotating
            int BackgroundCircleId = 0;                         //(Internal variable) Shape ID for the background circle when user is rotating
            int OuterCircleId = 0;                              //(Internal variable) Shape ID for the outer circle when user is rotating

            bool MousePressed = false;                          //(Internal variable) Used to indicate if user has clicked on this widget to pan the image
            glm::vec2 MouseButtonDownPosition = glm::vec2();    //(Internal variable) The initial position when the user clicked on this widget


            ImageCanvas(ImageCanvas const& other);

            //https://stackoverflow.com/questions/1727881/how-to-use-the-pi-constant-in-c
            constexpr inline static double pi() { return 3.1415926535897932384626433832795028841971; };

            //Return angle in radians. Positive if angle between a and b is clockwise
            virtual double GetAngle(glm::vec2 a, glm::vec2 b);

            virtual glm::vec2 RotateVec(glm::vec2 vec, float radians);

            virtual void ConstructRenderInfo() override;

            virtual void MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                                    ssGUI::GUIObject* mainWindow) override;
        public:
            //string: ListenerKey
            static const std::string ListenerKey;

            ImageCanvas();
            virtual ~ImageCanvas() override;

            //function: GetUVFromGlobalPosition
            //Gets the UV position (0 to image size) from global position
            virtual glm::vec2 GetUVFromGlobalPosition(glm::vec2 globalPos);
            
            //function: GetGlobalPositionFromUV
            //Gets the global position from the UV position (0 to image size)
            virtual glm::vec2 GetGlobalPositionFromUV(glm::vec2 uv);

            //function: SetImageRotation
            //Sets the rotation of the image, by default it uses degrees
            virtual void SetImageRotation(float rotation, bool radians = false);

            //function: GetImageRotation
            //Gets the rotation fo the image, by default it uses degrees
            virtual float GetImageRotation(bool radians = false) const;

            //function: SetImageScale
            //Sets the scale of the image in percentage (0 to 1), 1 meaning the height/width (determined by <IsMeasureScaleByHeight>)
            //of the image is the same size of ImageCanvas GUI Object.
            virtual void SetImageScale(float scale);
            
            //function: GetImageScale
            //Gets the scale of the image in percentage (0 to 1), 1 meaning the height/width (determined by <IsMeasureScaleByHeight>)
            //of the image is the same size of ImageCanvas.
            virtual float GetImageScale() const;

            //function: GetOriginalImageScale
            //Gets the scale to set in order to have the original size of the image
            virtual float GetOriginalImageScale() const;

            //function: IsMeasureScaleByHeight
            //Returns if the scaling is measured using height of ImageCanvas, false if it is measured using width of ImageCanvas. 
            virtual bool IsMeasureScaleByHeight() const;
            
            //function: SetMeasureScaleByHeight
            //Sets if the scaling is measured using height of ImageCanvas, false if it is measured using width of ImageCanvas. 
            virtual void SetMeasureScaleByHeight(bool scaleByHeight);

            //function: SetImagePosition
            //Sets the position of center of the image, represented by percentage (0 to 1).
            //Setting it to (0, 0) will set the center of the image to be top-left, (0.5, 0.5) will be center of ImageCanvas, etc.
            virtual void SetImagePosition(glm::vec2 position);

            //function: GetImagePosition
            //Gets the position of center of the image, represented by percentage (0 to 1).
            //(0, 0) will indicates the center of the image to be top-left, (0.5, 0.5) will be center of ImageCanvas, etc.
            virtual glm::vec2 GetImagePosition() const;

            //function: GetImageVertices
            //Gets all the vertices (from last frame or this frame depending on when you call this) of the image itself in global position
            virtual const std::vector<glm::vec2>& GetImageVertices() const;

            //function: GetImageMinX
            //Gets the minimum global X position of the image (from last frame or this frame depending on when you call this)
            virtual float GetImageMinX() const;
            
            //function: GetImageMaxX
            //Gets the maximum global X position of the image (from last frame or this frame depending on when you call this)
            virtual float GetImageMaxX() const;
            
            //function: GetImageMinY
            //Gets the minimum global Y position of the image (from last frame or this frame depending on when you call this)
            virtual float GetImageMinY() const;
            
            //function: GetImageMaxY
            //Gets the maximum global Y position of the image (from last frame or this frame depending on when you call this)
            virtual float GetImageMaxY() const;

            //function: UseDefaultPanning
            //Sets if user can pan by default when holding space button
            virtual void UseDefaultPanning(bool pan);

            //function: IsUsingDefaultPanning
            //Returns if user can pan by default when holding space button
            virtual bool IsUsingDefaultPanning() const;
            
            //function: UseDefaultZooming
            //Sets if user can zoom by default when using the scroll wheel
            virtual void UseDefaultZooming(bool zoom);

            //function: IsUsingDefaultZooming
            //Returns if user can zoom by default when using the scroll wheel
            virtual bool IsUsingDefaultZooming() const;

            //function: SetDefaultMinZoom
            //Sets the minimum scale value when using default zooming (inclusive)
            virtual void SetDefaultMinZoom(float min);
            
            //function: GetDefaultMinZoom
            //Gets the minimum scale value when using default zooming (inclusive)
            virtual float GetDefaultMinZoom() const;

            //function: SetDefaultMaxZoom
            //Sets the maximum scale value when using default zooming (inclusive)
            virtual void SetDefaultMaxZoom(float max);

            //function: GetDefaultMaxZoom
            //Gets the maximum scale value when using default zooming (inclusive)
            virtual float GetDefaultMaxZoom() const;

            //function: UseDefaultRotating
            //Sets if the user can rotate by default when holding the R key
            virtual void UseDefaultRotating(bool rotate);
            
            //function: IsUsingDefaultRotating
            //Returns if the user can rotate by default when holding the R key
            virtual bool IsUsingDefaultRotating() const;

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
            //Sets the horizontal scrollbar object used by ImageCanvas
            virtual void SetHorizontalScrollbar(ssGUI::Scrollbar* scrollbar);
            
            //function: GetHorizontalScrollbar
            //Gets the horizontal scrollbar object used by ImageCanvas
            virtual ssGUI::Scrollbar* GetHorizontalScrollbar() const;

            //function: SetVerticalScrollbar
            //Sets the vertical scrollbar object used by ImageCanvas
            virtual void SetVerticalScrollbar(ssGUI::Scrollbar* scrollbar);
            
            //function: GetVerticalScrollbar
            //Gets the vertical scrollbar object used by ImageCanvas
            virtual ssGUI::Scrollbar* GetVerticalScrollbar() const;

            //function: GetFitting
            //*Fitting has no effect on <ImageCanvas>*
            virtual ssGUI::Enums::ImageFitting GetFitting() const override;

            //function: SetFitting
            //*Fitting has no effect on <ImageCanvas>*
            virtual void SetFitting(ssGUI::Enums::ImageFitting fitting) override;

            //function: GetType
            //See <Widget::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: Clone
            //See <Widget::Clone>
            virtual ImageCanvas* Clone(bool cloneChildren) override;
    };
}

#endif