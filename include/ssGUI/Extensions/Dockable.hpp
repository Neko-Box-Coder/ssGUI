#ifndef SSGUI_DOCKABLE
#define SSGUI_DOCKABLE


#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/ssGUITags.hpp"
#include "ssGUI/BaseClasses/MainWindow.hpp" //This is needed as Extension is only forward declaring ssGUI::GUIObject
#include "ssGUI/BaseClasses/Widget.hpp"
#include "glm/vec4.hpp"

//namespace: ssGUI::Extensions
namespace ssGUI::Extensions
{       
    //class: Dockable
    class Dockable : public Extension
    {
        private:
            Dockable& operator=(Dockable const& other);
        
        protected:
            enum class DockSide
            {
                NONE,
                TOP,
                RIGHT,
                BOTTOM,
                LEFT,
                CENTER
            };

            ssGUI::GUIObject* Container;
            bool Enabled;
            ssGUIObjectIndex TopLevelParent;

            ObjectsReferences CurrentObjectsReferences;

            bool UseTriggerPercentage;
            float TriggerPercentage;
            int TriggerPixel;
            glm::u8vec4 TriggerAreaColor;
            glm::u8vec4 DockPreviewColor;

            ssGUI::GUIObject* OriginalParent;
            bool ContainerIsDocking;

            ssGUI::GUIObject* DockPreivewTop;
            ssGUI::GUIObject* DockPreivewRight;
            ssGUI::GUIObject* DockPreivewBottom;
            ssGUI::GUIObject* DockPreivewLeft;
            ssGUI::GUIObject* DockTriggerTop;
            ssGUI::GUIObject* DockTriggerRight;
            ssGUI::GUIObject* DockTriggerBottom;
            ssGUI::GUIObject* DockTriggerLeft;

            static bool GlobalDockMode;
            static ssGUI::MainWindow* MainWindowUnderDocking;
            static ssGUI::GUIObject* TargetDockObject;          //Target Dockable Object to dock next to. This can be a docker as well. (This is NOT the object being docked)
            static Dockable::DockSide TargetDockSide;

            Dockable(Dockable const& other);

            virtual void CreateWidgetIfNotPresent(ssGUI::GUIObject** widget, glm::u8vec4 color);

            virtual void DrawLeftPreview();
            virtual void DrawTopPreview();
            virtual void DrawRightPreview();
            virtual void DrawBottomPreview();
            virtual void DiscardLeftPreview();
            virtual void DiscardTopPreview();
            virtual void DiscardRightPreview();
            virtual void DiscardBottomPreview();

            virtual void DrawTriggerAreas();
            virtual void DiscardTriggerAreas();

            virtual void OnWindowDragStarted();
            virtual void OnWindowDragFinished();

        public:
            static const std::string EXTENSION_NAME;

            Dockable();
            virtual ~Dockable() override;

            //Allow docker to see if it is global dock mode or not
            friend class Docker;

            virtual void SetTriggerPercentage(float percentage);

            virtual float GetTriggerPercentage() const;
            
            virtual void SetTriggerPixel(int pixel);

            virtual int GetTriggerPixel() const;

            virtual void SetUseTriggerPercentage(bool usePercentage);

            virtual bool IsUseTriggerPercentage() const;

            virtual void SetTriggerAreaColor(glm::u8vec4 color);

            virtual glm::u8vec4 GetTriggerAreaColor() const;

            virtual void SetDockPreviewColor(glm::u8vec4 color);

            virtual glm::u8vec4 GetDockPreviewColor() const;

            virtual void SetTopLevelParent(ssGUI::GUIObject* parent);

            virtual ssGUI::GUIObject* GetTopLevelParent() const;

            //Override from Extension
            virtual void SetEnabled(bool enabled) override;

            virtual bool IsEnabled() const override;

            //function: Update
            virtual void Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;
            
            //function: Internal_Draw
            virtual void Internal_Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset) override;
            
            //function: GetExtensionName
            virtual std::string GetExtensionName() override;
            
            //function: BindToObject
            virtual void BindToObject(ssGUI::GUIObject* bindObj) override;

            virtual void Copy(ssGUI::Extensions::Extension* extension) override;

            virtual ObjectsReferences* Internal_GetObjectsReferences() override;

            //function: Clone
            virtual Extension* Clone(ssGUI::GUIObject* newContainer) override;
    };
}

#endif