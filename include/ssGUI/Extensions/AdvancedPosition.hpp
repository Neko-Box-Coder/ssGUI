#ifndef SSGUI_ADVANCED_POSITION
#define SSGUI_ADVANCED_POSITION

#include <unordered_map>
#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/BaseClasses/Window.hpp"
#include "ssGUI/BaseClasses/GUIObject.hpp"  //This is needed as Extension is only forward declaring ssGUI::GUIObject

#include "ssGUI/BaseClasses/MainWindow.hpp"

/*
#include "ssGUI/EventCallbacks/RecursiveChildrenAddedEventCallback.hpp"
#include "ssGUI/EventCallbacks/RecursiveChildrenRemovedEventCallback.hpp"
#include "ssGUI/EventCallbacks/MinMaxSizeChangedEventCallback.hpp"*/

//namespace: ssGUI::Extensions
namespace ssGUI::Extensions
{
    //class: AdvancedPosition
    class AdvancedPosition : public Extension
    {
        //Forward declaration
        public:
            enum class HorizontalAnchor;
            enum class VerticalAnchor;

        private:
            AdvancedPosition& operator=(AdvancedPosition const& other);
        
        protected:
            ssGUI::GUIObject* Container;
            bool Enabled;

            HorizontalAnchor CurrentHorizontal;
            VerticalAnchor CurrentVertical;

            bool HorizontalUsePercentage;
            bool VerticalUsePercentage;

            int HorizontalPixelValue;
            int VerticalPixelValue;

            float HorizontalPercentageValue;
            float VerticalPercentageValue;

            bool OverrideDefaultPosition;

            AdvancedPosition(AdvancedPosition const& other);

        public:
            static const std::string EXTENSION_NAME;

            AdvancedPosition();
            virtual ~AdvancedPosition() override;

            enum class HorizontalAnchor
            {
                LEFT,
                CENTER,
                RIGHT
            };

            enum class VerticalAnchor
            {
                TOP,
                CENTER,
                BOTTOM
            };

            virtual void SetHorizontalAnchor(HorizontalAnchor anchor);

            virtual HorizontalAnchor GetHorizontalAnchor() const;

            virtual void SetVerticalAnchor(VerticalAnchor anchor);

            virtual VerticalAnchor GetVerticalAnchor() const;

            virtual void SetHorizontalUsePercentage(bool percentage);

            virtual bool IsHorizontalUsePercentage() const;

            virtual void SetVerticalUsePercentage(bool percentage);

            virtual bool IsVerticalUsePercentage() const;

            virtual void SetHorizontalPixel(int pixel);

            //This value will not be valid if using percentage for horizontal
            virtual int GetHorizontalPixel() const;

            virtual void SetVerticalPixel(int pixel);

            //This value will not be valid if using percentage for vertical
            virtual int GetVerticalPixel() const;
            
            //0 - 1
            virtual void SetHorizontalPercentage(float percentage);

            //This value will not be valid if using pixel for horizontal
            virtual float GetHorizontalPercentage() const;

            //0 - 1
            virtual void SetVerticalPercentage(float percentage);

            //This value will not be valid if using pixel for vertical
            virtual float GetVerticalPercentage() const;

            virtual void SetOverrideDefaultPosition(bool override);

            virtual float IsOverrideDefaultPosition() const;

            //Override from Extension
            virtual void SetEnabled(bool enabled) override;

            virtual bool IsEnabled() const override;

            //function: Update
            virtual void Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;;
            
            //function: Draw
            virtual void Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset) override;
            
            //function: GetExtensionName
            virtual std::string GetExtensionName() override;
            
            //function: BindToObject
            virtual void BindToObject(ssGUI::GUIObject* bindObj) override;

            virtual void Copy(ssGUI::Extensions::Extension* extension) override;

            //function: Clone
            virtual Extension* Clone(ssGUI::GUIObject* newContainer) override;
    };
}




#endif