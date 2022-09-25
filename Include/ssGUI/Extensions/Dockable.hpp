#ifndef SSGUI_DOCKABLE
#define SSGUI_DOCKABLE

#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //This is needed as Extension is only forward declaring ssGUI::GUIObject
#include "ssGUI/GUIObjectClasses/Widget.hpp"
#include "glm/vec4.hpp"

//namespace: ssGUI::Extensions
namespace ssGUI::Extensions
{       
    class Layout;
    
    /*class: ssGUI::Extensions::Dockable
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

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        enum class DockSide                                     //(Internal structure) Used to identify which side is being docked
        {
            NONE,
            TOP,
            RIGHT,
            BOTTOM,
            LEFT,
            CENTER
        };

        ssGUI::GUIObject* Container;                            //See <BindToObject>
        bool Enabled;                                           //See <IsEnabled>
        ssGUIObjectIndex TopLevelParent;                        //See <GetTopLevelParent>

        ObjectsReferences CurrentObjectsReferences;             //(Internal variable) Used to manage Top Level Parent

        bool UseTriggerPercentage;                              //See <IsUseTriggerPercentage>
        float TriggerPercentage;                                //See <GetTriggerPercentage>
        int TriggerPixel;                                       //See <GetTriggerPixel>
        glm::u8vec4 TriggerAreaColor;                           //See <GetTriggerAreaColor>
        glm::u8vec4 DockPreviewColor;                           //See <GetDockPreviewColor>

        ssGUI::GUIObject* OriginalParent;                       //(Internal variable) Used to keep track of the parent being be dragged/docked
        bool ContainerIsDocking;                                //(Internal variable) Flag for indicating if the container is being dragged/docked

        ssGUI::GUIObject* DockPreivewTop;                       //(Internal variable) Dock preview GUI visual object
        ssGUI::GUIObject* DockPreivewRight;                     //(Internal variable) Dock preview GUI visual object
        ssGUI::GUIObject* DockPreivewBottom;                    //(Internal variable) Dock preview GUI visual object
        ssGUI::GUIObject* DockPreivewLeft;                      //(Internal variable) Dock preview GUI visual object
        ssGUI::GUIObject* DockTriggerTop;                       //(Internal variable) Dock trigger GUI visual object
        ssGUI::GUIObject* DockTriggerRight;                     //(Internal variable) Dock trigger GUI visual object
        ssGUI::GUIObject* DockTriggerBottom;                    //(Internal variable) Dock trigger GUI visual object
        ssGUI::GUIObject* DockTriggerLeft;                      //(Internal variable) Dock trigger GUI visual object

        static bool GlobalDockMode;                             //(Internal variable) Flag if there's any window being docked atm
        static ssGUI::MainWindow* MainWindowUnderDocking;       //(Internal variable) Target main window where the docking is happening
        static ssGUI::GUIObject* DockingTopLevelParent;         //(Internal variable) The top level parent where the docking is happening, see <GetTopLevelParent>
        static ssGUI::GUIObject* TargetDockObject;              //(Internal variable) Target Dockable Object to dock next to. This can be a docker as well. (This is **NOT** the object being docked)
        static Dockable::DockSide TargetDockSide;               //(Internal variable) The current side it is trying to dock atm
    =================================================================
    ============================== C++ ==============================
    Dockable::Dockable() :  Container(nullptr),
                            Enabled(true),
                            TopLevelParent(-1),
                            CurrentObjectsReferences(),
                            UseTriggerPercentage(true),
                            TriggerPercentage(0.25f),
                            TriggerPixel(15),
                            TriggerAreaColor(87, 207, 255, 127),
                            DockPreviewColor(255, 255, 255, 127),
                            OriginalParent(nullptr),
                            ContainerIsDocking(false),
                            DockPreivewTop(nullptr),
                            DockPreivewRight(nullptr),
                            DockPreivewBottom(nullptr),
                            DockPreivewLeft(nullptr),
                            DockTriggerTop(nullptr),
                            DockTriggerRight(nullptr),
                            DockTriggerBottom(nullptr),
                            DockTriggerLeft(nullptr)
    {}

    bool Dockable::GlobalDockMode = false;
    ssGUI::MainWindow* Dockable::MainWindowUnderDocking = nullptr;
    ssGUI::GUIObject* Dockable::DockingTopLevelParent = nullptr;
    ssGUI::GUIObject* Dockable::TargetDockObject = nullptr;
    Dockable::DockSide Dockable::TargetDockSide = Dockable::DockSide::NONE;
    =================================================================
    */
    class Dockable : public Extension
    {
        public:
            friend class ssGUI::Factory;

        private:
            Dockable& operator=(Dockable const& other);
        
        protected:
            enum class DockSide                                     //(Internal structure) Used to identify which side is being docked
            {
                NONE,
                TOP,
                RIGHT,
                BOTTOM,
                LEFT,
                CENTER
            };

            ssGUI::GUIObject* Container;                            //See <BindToObject>
            bool Enabled;                                           //See <IsEnabled>
            ssGUIObjectIndex TopLevelParent;                        //See <GetTopLevelParent>

            ObjectsReferences CurrentObjectsReferences;             //(Internal variable) Used to manage Top Level Parent

            bool UseTriggerPercentage;                              //See <IsUseTriggerPercentage>
            float TriggerPercentage;                                //See <GetTriggerPercentage>
            int TriggerPixel;                                       //See <GetTriggerPixel>
            glm::u8vec4 TriggerAreaColor;                           //See <GetTriggerAreaColor>
            glm::u8vec4 DockPreviewColor;                           //See <GetDockPreviewColor>

            ssGUI::GUIObject* OriginalParent;                       //(Internal variable) Used to keep track of the parent being be dragged/docked
            bool ContainerIsDocking;                                //(Internal variable) Flag for indicating if the container is being dragged/docked

            ssGUI::GUIObject* DockPreivewTop;                       //(Internal variable) Dock preview GUI visual object
            ssGUI::GUIObject* DockPreivewRight;                     //(Internal variable) Dock preview GUI visual object
            ssGUI::GUIObject* DockPreivewBottom;                    //(Internal variable) Dock preview GUI visual object
            ssGUI::GUIObject* DockPreivewLeft;                      //(Internal variable) Dock preview GUI visual object
            ssGUI::GUIObject* DockTriggerTop;                       //(Internal variable) Dock trigger GUI visual object
            ssGUI::GUIObject* DockTriggerRight;                     //(Internal variable) Dock trigger GUI visual object
            ssGUI::GUIObject* DockTriggerBottom;                    //(Internal variable) Dock trigger GUI visual object
            ssGUI::GUIObject* DockTriggerLeft;                      //(Internal variable) Dock trigger GUI visual object

            static bool GlobalDockMode;                             //(Internal variable) Flag if there's any window being docked atm
            static ssGUI::MainWindow* MainWindowUnderDocking;       //(Internal variable) Target main window where the docking is happening
            static ssGUI::GUIObject* DockingTopLevelParent;         //(Internal variable) The top level parent where the docking is happening, see <GetTopLevelParent>
            static ssGUI::GUIObject* TargetDockObject;              //(Internal variable) Target Dockable Object to dock next to. This can be a docker as well. (This is **NOT** the object being docked)
            static Dockable::DockSide TargetDockSide;               //(Internal variable) The current side it is trying to dock atm

            Dockable();
            virtual ~Dockable() override;
            Dockable(Dockable const& other);
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};

            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;
            
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

            virtual void FindDockLayout(ssGUI::Extensions::Layout*& dockLayout);
            virtual void CreateEmptyParentForDocking(ssGUI::Extensions::Layout*& dockLayout);
            virtual void OnWindowDragFinished();

        public:
            static const std::string EXTENSION_NAME;

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
            virtual void Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, ssGUI::GUIObject* mainWindow) override;
            
            //function: Internal_Draw
            //See <Extension::Internal_Draw>
            virtual void Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;
            
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
            virtual Dockable* Clone(ssGUI::GUIObject* newContainer) override;
    };
}

#endif