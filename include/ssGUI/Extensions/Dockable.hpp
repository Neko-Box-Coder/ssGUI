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
    class Layout;
    
    /*class: Dockable
    This extension allows this GUI Object (window) to be docked to other GUI Objects. 
    
    Trigger areas are areas that the cursor (when dragging another window) can enter to trigger a docking preview and action.
    A docking preview gives a visual reprenstation of how the window will be docked.
    
    A top level parent can be set so that this GUI object (window) cannot be docked outside of the top level parent.
    When this GUI object (window) is being dragged, this will be parented to MainWindow unless top level parent is set (In that case, it will be parented to top level parent). 
    For example:
    ================ text ================
    A
    │
    ├─►B
    │  │
    │  ├─►C
    │  │
    │  ├─►D
    │  │
    │  └─►E
    │
    └─►F
       │
       └─►G
    ======================================

    If C is currently being docked and its top level parent is set to B, then it can only be docked to D or E.
    */
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

            int WindowDragStateChangedEventIndex;

            static bool GlobalDockMode;
            static ssGUI::MainWindow* MainWindowUnderDocking;
            static ssGUI::GUIObject* DockingTopLevelParent;
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

            virtual void RemoveUnnecessaryDocker(ssGUI::GUIObject* checkObj);
            
            //virtual void RemoveOriginalParentIfNeeded();

            virtual void FindDockLayout(ssGUI::Extensions::Layout*& dockLayout);
            virtual void CreateEmptyParentForDocking(ssGUI::Extensions::Layout*& dockLayout);
            virtual void OnWindowDragFinished();

        public:
            static const std::string EXTENSION_NAME;

            Dockable();
            virtual ~Dockable() override;

            //Allow docker to see if it is global dock mode or not
            friend class Docker;

            //function: SetTriggerPercentage
            //0 = 0%, 1 = 100%
            virtual void SetTriggerPercentage(float percentage);

            //function: GetTriggerPercentage
            //0 = 0%, 1 = 100%
            virtual float GetTriggerPercentage() const;

            //function: SetTriggerPixel           
            virtual void SetTriggerPixel(int pixel);

            //function: GetTriggerPixel
            virtual int GetTriggerPixel() const;

            //function: SetUseTriggerPercentage
            //0 = 0%, 1 = 100%
            virtual void SetUseTriggerPercentage(bool usePercentage);

            //function: IsUseTriggerPercentage
            //0 = 0%, 1 = 100%
            virtual bool IsUseTriggerPercentage() const;

            //function: SetTriggerAreaColor
            virtual void SetTriggerAreaColor(glm::u8vec4 color);

            //function: GetTriggerAreaColor
            virtual glm::u8vec4 GetTriggerAreaColor() const;

            //function: SetDockPreviewColor
            virtual void SetDockPreviewColor(glm::u8vec4 color);

            //function: GetDockPreviewColor
            virtual glm::u8vec4 GetDockPreviewColor() const;

            //function: SetTopLevelParent
            virtual void SetTopLevelParent(ssGUI::GUIObject* parent);

            //function: GetTopLevelParent
            virtual ssGUI::GUIObject* GetTopLevelParent() const;

            //Override from Extension
            //function: SetEnabled
            //See <Extension::SetEnabled>
            virtual void SetEnabled(bool enabled) override;

            //function: IsEnabled
            //See <Extension::IsEnabled>
            virtual bool IsEnabled() const override;

            //function: Internal_Update
            //See <Extension::Internal_Update>
            virtual void Internal_Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;
            
            //function: Internal_Draw
            //See <Extension::Internal_Draw>
            virtual void Internal_Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset) override;
            
            //function: GetExtensionName
            //See <Extension::GetExtensionName>
            virtual std::string GetExtensionName() override;
            
            //function: BindToObject
            //See <Extension::BindToObject>
            virtual void BindToObject(ssGUI::GUIObject* bindObj) override;

            //function: Copy
            //See <Extension::Copy>
            virtual void Copy(ssGUI::Extensions::Extension* extension) override;

            //function: Internal_GetObjectsReferences
            //See <Extension::Internal_GetObjectsReferences>
            virtual ObjectsReferences* Internal_GetObjectsReferences() override;

            //function: Clone
            //See <Extension::Clone>
            virtual Extension* Clone(ssGUI::GUIObject* newContainer) override;
    };
}

#endif