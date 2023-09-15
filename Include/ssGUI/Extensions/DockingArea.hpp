#ifndef SSGUI_DOCKING_AREA_HPP
#define SSGUI_DOCKING_AREA_HPP

#include "ssGUI/Extensions/Extension.hpp"
#include <unordered_set>

namespace ssGUI 
{
    
//namespace: ssGUI::Extensions
namespace Extensions
{
    class Layout;

    /*class: ssGUI::Extensions::DockingArea
    This extension allows any object with <ssGUI::Extensions::DockableV2> to be docked under the container, via GUI or code.
    This will automatically add a <ssGUI::Extensions::Layout> if it is not present.
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUI::GUIObject* Container;                                    //See <BindToObject>
        bool Enabled;                                                   //See <IsEnabled>
        
        ObjectsReferences CurrentObjectsReferences;                     //See <Internal_GetObjectsReferences>

        //TODO: Reuse the same object for both preview and trigger?
        ssGUIObjectIndex DockPreview;                                   //(Internal variable) Dock preview GUI visual object
        ssGUIObjectIndex DockTrigger;                                   //(Internal variable) Dock trigger GUI visual object

        glm::vec2 TriggerSizePercentage;                                //See <GetTriggerSizePercentage>
        glm::ivec2 TriggerSizePixels;                                   //See <GetTriggerSizePixels>
        glm::u8vec4 TriggerColor;                                       //See <GetTriggerColor>
        glm::u8vec4 PreviewColor;                                       //See <GetTriggerColor>

        bool PauseChildRemovedEventListener;                            //(Internal variable) Used to prevent recursively trigger event callback
        
        ssGUI::Extensions::Layout* DefaultLayout;                       //See <GetDefaultLayout>
    =================================================================
    ============================== C++ ==============================
    DockingArea::DockingArea() :    Container(nullptr),
                                    Enabled(true),
                                    CurrentObjectsReferences(),
                                    DockPreview(-1),
                                    DockTrigger(-1),
                                    TriggerSizePercentage(0.5, 0.5),
                                    TriggerSizePixels(),
                                    TriggerColor(87, 207, 255, 127),
                                    PreviewColor(255, 255, 255, 127),
                                    PauseChildRemovedEventListener(false),
                                    DefaultLayout(nullptr)
    {}
    =================================================================
    */
    class DockingArea : public Extension
    {
        public:
            friend class ssGUI::Factory;

        private:
            DockingArea& operator=(DockingArea const& other);
        
        protected:
            ssGUI::GUIObject* Container;                                    //See <BindToObject>
            bool Enabled;                                                   //See <IsEnabled>
            
            ObjectsReferences CurrentObjectsReferences;                     //See <Internal_GetObjectsReferences>

            //TODO: Reuse the same object for both preview and trigger?
            ssGUIObjectIndex DockPreview;                                   //(Internal variable) Dock preview GUI visual object
            ssGUIObjectIndex DockTrigger;                                   //(Internal variable) Dock trigger GUI visual object

            glm::vec2 TriggerSizePercentage;                                //See <GetTriggerSizePercentage>
            glm::ivec2 TriggerSizePixels;                                   //See <GetTriggerSizePixels>
            glm::u8vec4 TriggerColor;                                       //See <GetTriggerColor>
            glm::u8vec4 PreviewColor;                                       //See <GetTriggerColor>
    
            bool PauseChildRemovedEventListener;                            //(Internal variable) Used to prevent recursively trigger event callback
            
            ssGUI::Extensions::Layout* DefaultLayout;                       //See <GetDefaultLayout>

            DockingArea();
            virtual ~DockingArea() override;
            DockingArea(DockingArea const& other);
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};

            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(   ssGUI::Backend::BackendDrawingInterface* drawingInterface, 
                                                ssGUI::GUIObject* mainWindow, 
                                                glm::vec2 mainWindowPositionOffset) override;

            virtual void CreateDockPreviewAndTrigger();
            virtual void DrawPreview();
            virtual void DiscardPreview();
            virtual void DrawTriggerArea();
            virtual void DiscardTriggerArea();
            virtual ssGUI::GUIObject* GetFirstDockingParent(ssGUI::GUIObject* startObject) const;
            
            virtual void NotifyBeforeDockingEvent(ssGUI::GUIObject* externalObject, bool& isAbort);
            virtual void NotifyDockedEvent(ssGUI::GUIObject* externalObject);

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

            //function: SetTriggerSizePercentage
            //Sets the size of the dock trigger in percentage (0 = 0%, 1 = 100%) against container.
            //Note that the final dock trigger size is percentage + pixels.
            virtual void SetTriggerSizePercentage(glm::vec2 triggerSize);
            
            //function: GetTriggerSizePercentage
            //Gets the size of the dock trigger in percentage (0 = 0%, 1 = 100%) against container.
            //Note that the final dock trigger size is percentage + pixels.
            virtual glm::vec2 GetTriggerSizePercentage();
            
            //function: SetTriggerSizePixels
            //Sets the size of the dock trigger in pixels.
            //Note that the final dock trigger size is percentage + pixels.
            virtual void SetTriggerSizePixels(glm::ivec2 triggerSize);
            
            //function: GetTriggerSizePixels
            //Gets the size of the dock trigger in pixels.
            //Note that the final dock trigger size is percentage + pixels.
            virtual glm::ivec2 GetTriggerSizePixels();
            
            //function: SetTriggerColor
            //Sets the dock trigger color
            virtual void SetTriggerColor(glm::u8vec4 color);
            
            //function: GetTriggerColor    
            //Gets the dock trigger color           
            virtual glm::u8vec4 GetTriggerColor();
            
            //====================================================================
            //Group: Dock Preview
            //====================================================================
            
            //function: SetDockPreviewColor
            //Sets the docking preview color
            virtual void SetDockPreviewColor(glm::u8vec4 color);
            
            //function: GetDockPreviewColor
            //Gets the docking preview color
            virtual glm::u8vec4 GetDockPreviewColor();
            
            //====================================================================
            //Group: Docking And Default Layout
            //====================================================================
            
            //function: DockExternalObject
            //Dock exteranl object under the container           
            virtual void DockExternalObject(ssGUI::GUIObject* externalObject);
            
            /*function: SetDefaultLayout
            When binding to a GUI Object, this will create the <ssGUI::Extensions::Layout> if it doesn't exist.
            This sets the default <ssGUI::Extensions::Layout>. 
            If this is nullptr, it will create its own <ssGUI::Extensions::Layout> instead.
            You can create extensions without binding to a GUI object by using <ssGUI::Create>.
            */
            virtual void SetDefaultLayout(ssGUI::Extensions::Layout* layout);
            
            /*function: GetDefaultLayout
            When binding to a GUI Object, this will create the <ssGUI::Extensions::Layout> if it doesn't exist.
            This gets the default <ssGUI::Extensions::Layout>. 
            If this is nullptr, it will create its own <ssGUI::Extensions::Layout> instead.
            You can create extensions without binding to a GUI object by using <ssGUI::Create>.
            */
            virtual ssGUI::Extensions::Layout* GetDefaultLayout() const;

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
                                            ssGUI::InputStatus& lastInputStatus, 
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
            virtual DockingArea* Clone() override;
    };
}

}

#endif