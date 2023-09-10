#ifndef SSGUI_DOCKABLE_V2_HPP
#define SSGUI_DOCKABLE_V2_HPP

#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/Enums/DockSide.hpp"

namespace ssGUI 
{
    
class Window;
    
//namespace: ssGUI::Extensions
namespace Extensions
{
    class DockingArea;
    
    /*class: ssGUI::Extensions::DockableV2
    This extension allows the container to be docked under any object with <ssGUI::Extensions::DockingArea>.
    It also allows other objects with this extension binded to be docked next to the container, via GUI or code.
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUI::GUIObject* Container;                                    //See <BindToObject>
        bool Enabled;                                                   //See <IsEnabled>

        bool Undockable;                                                //See <IsUndockable>
        ObjectsReferences CurrentObjectsReferences;                     //(Internal variable) Used to manage Top Level Parent

        //TODO: Reuse the same object for both preview and trigger?
        ssGUIObjectIndex DockPreview[4];                                //(Internal variable) Dock preview GUI visual object
        ssGUIObjectIndex DockTrigger[4];                                //(Internal variable) Dock trigger GUI visual object

        float TriggerMarginPercentage;                                  //See <GetTriggerMarginPercentage>
        int TriggerMarginPixels;                                        //See <GetTriggerMarginPixels>

        glm::vec2 TriggerSizePercentage;                                //See <GetTriggerSizePercentage>
        glm::ivec2 TriggerSizePixels;                                   //See <GetTriggerSizePixels>
        glm::u8vec4 TriggerColor;                                       //See <GetTriggerColor>
        glm::u8vec4 PreviewColor;                                       //See <GetDockPreviewColor>
        
        ssGUIObjectIndex TopLevelParent;                                //See <GetTopLevelParent>
        glm::vec2 LastDraggedDistance;                                  //(Internal variable) Used for adjusting the undocked object position
        bool DockedSinceLastUpdate;                                     //(Internal variable) Used for determining if undocking is possible
        bool AttemptToUndock;                                           //(Internal variable) Flag for undocking

        ssGUI::Extensions::DockingArea* DefaultDockingArea;             //See <GetDefaultDockingArea>
        ssGUIObjectIndex DefaultNewParent;                              //See <GetDefaultNewParent>
    =================================================================
    ============================== C++ ==============================
    DockableV2::DockableV2() :  Container(nullptr),
                                Enabled(true),
                                Undockable(true),
                                CurrentObjectsReferences(),
                                DockPreview(),
                                DockTrigger(),
                                TriggerMarginPercentage(),
                                TriggerMarginPixels(),
                                TriggerSizePercentage(0.25f, 0.5f),
                                TriggerSizePixels(),
                                TriggerColor(87, 207, 255, 127),
                                PreviewColor(255, 255, 255, 127),
                                TopLevelParent(-1),
                                LastDraggedDistance(),
                                DockedSinceLastUpdate(false),
                                AttemptToUndock(false),
                                DefaultDockingArea(nullptr),
                                DefaultNewParent(-1)
    {
        DockPreview[0] = -1;
        DockPreview[1] = -1;
        DockPreview[2] = -1;
        DockPreview[3] = -1;
        
        DockTrigger[0] = -1;
        DockTrigger[1] = -1;
        DockTrigger[2] = -1;
        DockTrigger[3] = -1;
    }
    =================================================================
    */
    class DockableV2 : public Extension
    {
        public:
            friend class ssGUI::Factory;

        private:
            DockableV2& operator=(DockableV2 const& other);
        
        protected:
            ssGUI::GUIObject* Container;                                    //See <BindToObject>
            bool Enabled;                                                   //See <IsEnabled>

            bool Undockable;                                                //See <IsUndockable>
            ObjectsReferences CurrentObjectsReferences;                     //(Internal variable) Used to manage Top Level Parent

            //TODO: Reuse the same object for both preview and trigger?
            ssGUIObjectIndex DockPreview[4];                                //(Internal variable) Dock preview GUI visual object
            ssGUIObjectIndex DockTrigger[4];                                //(Internal variable) Dock trigger GUI visual object

            float TriggerMarginPercentage;                                  //See <GetTriggerMarginPercentage>
            int TriggerMarginPixels;                                        //See <GetTriggerMarginPixels>

            glm::vec2 TriggerSizePercentage;                                //See <GetTriggerSizePercentage>
            glm::ivec2 TriggerSizePixels;                                   //See <GetTriggerSizePixels>
            glm::u8vec4 TriggerColor;                                       //See <GetTriggerColor>
            glm::u8vec4 PreviewColor;                                       //See <GetDockPreviewColor>
            
            ssGUIObjectIndex TopLevelParent;                                //See <GetTopLevelParent>
            glm::vec2 LastDraggedDistance;                                  //(Internal variable) Used for adjusting the undocked object position
            bool DockedSinceLastUpdate;                                     //(Internal variable) Used for determining if undocking is possible
            bool AttemptToUndock;                                           //(Internal variable) Flag for undocking

            ssGUI::Extensions::DockingArea* DefaultDockingArea;             //See <GetDefaultDockingArea>
            ssGUIObjectIndex DefaultNewParent;                              //See <GetDefaultNewParent>

            DockableV2();
            virtual ~DockableV2() override;
            DockableV2(DockableV2 const& other);
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};

            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(   ssGUI::Backend::BackendDrawingInterface* drawingInterface, 
                                                ssGUI::GUIObject* mainWindow, 
                                                glm::vec2 mainWindowPositionOffset) override;

            virtual void CreateDockPreviewAndTrigger();
            
            virtual void DrawPreview(ssGUI::Enums::DockSide side);
            
            virtual void DiscardPreview();
            
            virtual void DrawTriggerAreas();
            
            virtual void DiscardTriggerAreas();

            virtual void AttemptUndockingFromGUI(   ssGUI::Window* containerWindow, 
                                                    ssGUI::MainWindow* mainWindow,
                                                    const ssGUI::InputStatus& lastInputStatus);

            virtual bool IsAttemptingToUndockFromGUI();

            virtual void ResetState();
            
            virtual ssGUI::GUIObject* GetFirstDockingParent(ssGUI::GUIObject* startObject) const;
            
            virtual void NotifyBeforeDockingEvent(ssGUI::GUIObject* externalObject, ssGUI::Enums::DockSide dockSide, bool& isAbort);
            virtual void NotifyDockedEvent(ssGUI::GUIObject* externalObject, ssGUI::Enums::DockSide dockSide);

        public:
            //====================================================================
            //Group: Constants
            //====================================================================

            //string: EXTENSION_NAME
            static const std::string EXTENSION_NAME;
            
            //string: ListenerKey
            static const std::string ListenerKey;

            //====================================================================
            //Group: Dock Trigger
            //====================================================================

            /*function: SetTriggerSizePercentage
            Sets the width (x component) and length (y component) of the dock triggers in percentage (0 = 0%, 1 = 100%) against container.
            For top and bottom triggers, width is the vertical direction and length is the horizontal direction.
            For left and right triggers, width is the horizontal direction and length is the vertical direction.
            Note that the final dock trigger size is percentage + pixels.
            */
            virtual void SetTriggerSizePercentage(glm::vec2 widthLength);
            
            /*function: GetTriggerSizePercentage
            Gets the width (x component) and length (y component) of the dock triggers in percentage (0 = 0%, 1 = 100%) against container.
            For top and bottom triggers, width is the vertical direction and length is the horizontal direction.
            For left and right triggers, width is the horizontal direction and length is the vertical direction.
            Note that the final dock trigger size is percentage + pixels.
            */
            virtual glm::vec2 GetTriggerSizePercentage() const;
            
            /*function: SetTriggerSizePixels
            Sets the width (x component) and length (y component) of the dock triggers in pixels.
            For top and bottom triggers, width is the vertical direction and length is the horizontal direction.
            For left and right triggers, width is the horizontal direction and length is the vertical direction.
            Note that the final dock trigger size is percentage + pixels.
            */
            virtual void SetTriggerSizePixels(glm::ivec2 widthLength);
            
            /*function: GetTriggerSizePixels
            Gets the width (x component) and length (y component) of the dock triggers in pixels.
            For top and bottom triggers, width is the vertical direction and length is the horizontal direction.
            For left and right triggers, width is the horizontal direction and length is the vertical direction.
            Note that the final dock trigger size is percentage + pixels.
            */
            virtual glm::ivec2 GetTriggerSizePixels() const;
            
            //function: SetTriggerColor
            //Sets the dock triggers color
            virtual void SetTriggerColor(glm::u8vec4 color);
            
            //function: GetTriggerColor
            //Gets the dock triggers color
            virtual glm::u8vec4 GetTriggerColor() const;

            /*function: SetTriggerMarginPercentage
            Sets the the distance between the trigger and the edge or the container in percentage (0 = 0%, 1 = 100%) against container.
            Note that the final dock trigger margin is percentage + pixels.
            */
            virtual void SetTriggerMarginPercentage(float marginFromEdge);
            
            /*function: GetTriggerMarginPercentage
            Gets the the distance between the trigger and the edge or the container in percentage (0 = 0%, 1 = 100%) against container.
            Note that the final dock trigger margin is percentage + pixels.
            */
            virtual float GetTriggerMarginPercentage() const;
            
            /*function: SetTriggerMarginPixels
            Sets the the distance between the trigger and the edge or the container in pixels.
            Note that the final dock trigger margin is percentage + pixels.
            */
            virtual void SetTriggerMarginPixels(int marginFromEdge);
            
            /*function: GetTriggerMarginPixels
            Gets the the distance between the trigger and the edge or the container in pixels.
            Note that the final dock trigger margin is percentage + pixels.
            */
            virtual int GetTriggerMarginPixels() const;

            //====================================================================
            //Group: Dock Preview
            //====================================================================

            //function: SetDockPreviewColor
            //Sets the docking preview color
            virtual void SetDockPreviewColor(glm::u8vec4 color);
            
            //function: GetDockPreviewColor
            //Gets the docking preview color
            virtual glm::u8vec4 GetDockPreviewColor() const;

            //====================================================================
            //Group: Docking
            //====================================================================

            //function: DockExternalObject
            //Dock exteranl object under the container           
            virtual void DockExternalObject(ssGUI::GUIObject* externalObject, ssGUI::Enums::DockSide side);

            /*function: SetTopLevelParent
            Sets the top level parent for the container that can dock against.
            Anything that is outside of the top level parent are not dockable by this container.
            Setting nullptr will set the top level parent to the associated main window,
            */
            virtual void SetTopLevelParent(ssGUI::GUIObject* topLevelParent);
            
            /*function: GetTopLevelParent
            Gets the top level parent for the container that can dock against.
            Anything that is outside of the top level parent are not dockable by this container.
            Nullptr will set the top level parent to the associated main window,
            */
            virtual ssGUI::GUIObject* GetTopLevelParent() const;
            
            /*function: SetUndockable
            When the container gets undocked (i.e. not being docked to anything), 
            it will be parented under the the top level parent.
            
            Sets the container to be undockable or not.
            If true, the container won't be abled to be undocked.
            
            Setting this to true does not affect the ability for this to be docked to other GUI objects.
            */
            virtual void SetUndockable(bool undockable);
            
            /*function: IsUndockable
            When the container gets undocked (i.e. not being docked to anything), 
            it will be parented under the the top level parent.
            
            Returns if the container is undockable or not.
            If true, the container won't be abled to be undocked.
            
            Setting this to true does not affect the ability for this to be docked to other GUI objects.
            */
            virtual bool IsUndockable() const;

            //====================================================================
            //Group: Default Docking Area And New Parent
            //====================================================================

            /*function: AddDefaultDockingArea
            When getting docked by other GUI objects with different orientation, 
            this will create an empty parent along with the docking area extension.
            
            This sets the default <ssGUI::Extensions::DockingArea>.
            
            If this is nullptr, it will create its own <ssGUI::Extensions::DockingArea> instead.
            You can create extensions without binding to a GUI object by using <ssGUI::Create>.
            */
            virtual void AddDefaultDockingArea(ssGUI::Extensions::DockingArea* dockingArea);
            
            /*function: GetDefaultDockingArea
            When getting docked by other GUI objects with different orientation, 
            this will create an empty parent along with the docking area extension.
            
            This gets the default <ssGUI::Extensions::DockingArea>.
            
            If this is nullptr, it will create its own <ssGUI::Extensions::DockingArea> instead.
            You can create extensions without binding to a GUI object by using <ssGUI::Create>.
            */
            virtual ssGUI::Extensions::DockingArea* GetDefaultDockingArea() const;
            
            /*function: AddDefaultNewParent
            When getting docked by other GUI objects with different orientation, 
            this will create an empty parent along with the docking area extension.
            
            This sets the default new parent to be created.
            
            If this is nullptr, it will create its own modified empty window instead.
            You can create GUI Objects without binding to a GUI object by using <ssGUI::Create>.
            */
            virtual void AddDefaultNewParent(ssGUI::GUIObject* defaultNewParent);
            
            /*function: GetDefaultNewParent
            When getting docked by other GUI objects with different orientation, 
            this will create an empty parent along with the docking area extension.
            
            This gets the default new parent to be created.
            
            If this is nullptr, it will create its own modified empty window instead.
            You can create GUI Objects without binding to a GUI object by using <ssGUI::Create>.
            */
            virtual ssGUI::GUIObject* GetDefaultNewParent() const;

            //====================================================================
            //Group: Overrides
            //====================================================================

            //Override from Extension
            //function: SetEnabled
            //See <Extension::SetEnabled>
            virtual void SetEnabled(bool enabled) override;

            //function: IsEnabled
            //See <Extension::IsEnabled>
            virtual bool IsEnabled() const override;
            
            //function: Internal_Update
            //See <Extension::Internal_Update>
            virtual void Internal_Update(   bool isPreUpdate, 
                                            ssGUI::Backend::BackendSystemInputInterface* inputInterface, 
                                            ssGUI::InputStatus& currentInputStatus, 
                                            const ssGUI::InputStatus& lastInputStatus, 
                                            ssGUI::GUIObject* mainWindow) override;
            
            //function: Internal_Draw
            //See <Extension::Internal_Draw>
            virtual void Internal_Draw( bool isPreRender, 
                                        ssGUI::Backend::BackendDrawingInterface* drawingInterface, 
                                        ssGUI::GUIObject* mainWindow, 
                                        glm::vec2 mainWindowPositionOffset) override;
            
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
            virtual DockableV2* Clone() override;
    };
}

}

#endif