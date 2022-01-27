#ifndef SSGUI_DOCKER
#define SSGUI_DOCKER

#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/ssGUITags.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

//namespace: ssGUI::Extensions
namespace ssGUI::Extensions
{
    class Layout;
    
    //class: Docker
    /* This extensions can either be added explictly or generated automatically.
    
    In the case of being generated Automactically:
    This extension allows this GUI object to acts as an conatiner for grouping the docked windows together.
    An empty window (with <Layout> and <Docker> attached) is automatically created (or destroyed) if needed for grouping docked windows.

    In the case of being added explictly:
    This extension allows windows with <Dockable> extension attached to be docked to this GUI object.
    This GUI object does not have to be a window.
    */
    class Docker : public Extension
    {
        private:
            Docker& operator=(Docker const& other);

        protected:
            Docker(Docker const& other);

            ssGUI::GUIObject* Container;
            bool Enabled;
            bool ChildrenDockerUseThisSettings;         //This works by having the child checking if parent has docker or not. Therefore if the docker extension is added after the child being added, this won't work.

            glm::u8vec4 FloatingDockerColor;                //Only applies when the docker is floating. Otherwise it is transparent.
            bool UseTriggerPercentage;
            float TriggerHorizontalPercentage;
            float TriggerVerticalPercentage;
            int TriggerHorizontalPixel;
            int TriggerVerticalPixel;
            glm::u8vec4 TriggerAreaColor;
            glm::u8vec4 DockPreviewColor;

            ssGUI::GUIObject* DockPreivew;
            ssGUI::GUIObject* DockTrigger;

            static ssGUI::Extensions::Docker* DefaultGeneratedDockerSettings;
            static ssGUI::Extensions::Layout* DefaultGeneratedLayoutSettings;

            virtual void CreateWidgetIfNotPresent(ssGUI::GUIObject** widget, glm::u8vec4 color);
            virtual void DrawPreview();
            virtual void DiscardPreview();
            virtual void DrawTriggerArea();
            virtual void DiscardTriggerArea();

        
        public:
            static const std::string EXTENSION_NAME;

            Docker();
            virtual ~Docker() override;
            
            //function: SetDefaultGeneratedDockerSettings
            /*Sets the default docker settings for the dockers that are generated (after this call).
            The settings are *copied* and stored locally staticly.
            Therefore, you should not call this function every frame.
            */
            static void SetDefaultGeneratedDockerSettings(ssGUI::Extensions::Docker* defaultDocker);

            //function: GetDefaultGeneratedDockerSettings
            //Returns the default docker settings. If nullptr is returned, it will use docker default initialization values.
            static ssGUI::Extensions::Docker* GetDefaultGeneratedDockerSettings();

            //function: SetDefaultGeneratedLayoutSettings
            /*Sets the default layout settings for the layout that are generated (after this call).
            The settings are *copied* and stored locally staticly.
            Therefore, you should not call this function every frame.
            */
            static void SetDefaultGeneratedLayoutSettings(ssGUI::Extensions::Layout* defaultLayout);

            //function: GetDefaultGeneratedLayoutSettings
            //Returns the default layout settings. If nullptr is returned, it will use layout default initialization values.
            static ssGUI::Extensions::Layout* GetDefaultGeneratedLayoutSettings();
 
            //function: SetFloatingDockerColor
            //When a window is docked to a floating dockable window (meaning the window is not docked to anything), a floating docker window will be created.
            //Floating docker color is the color of that window.
            virtual void SetFloatingDockerColor(glm::u8vec4 color);

            //function: GetFloatingDockerColor
            //When a window is docked to a floating dockable window (meaning the window is not docked to anything), a floating docker window will be created.
            //Floating docker color is the color of that window.
            virtual glm::u8vec4 GetFloatingDockerColor() const;

            //function: SetChildrenDockerUseThisSettings
            //If true, this will set any docker to use this settings *when it is added as a child*
            virtual void SetChildrenDockerUseThisSettings(bool use);

            //function: IsChildrenDockerUseThisSettings
            //If true, this will set any docker to use this settings *when it is added as a child*
            virtual bool IsChildrenDockerUseThisSettings() const;

            //function: SetUseTriggerPercentage
            virtual void SetUseTriggerPercentage(bool use);

            //function: IsUseTriggerPercentage
            virtual bool IsUseTriggerPercentage() const;

            //function: SetTriggerHorizontalPercentage
            virtual void SetTriggerHorizontalPercentage(float percent);

            //function: GetTriggerHorizontalPercentage
            virtual float GetTriggerHorizontalPercentage() const;

            //function: SetTriggerVerticalPercentage
            virtual void SetTriggerVerticalPercentage(float percent);

            //function: GetTriggerVerticalPercentage
            virtual float GetTriggerVerticalPercentage() const;

            //function: SetTriggerHorizontalPixel
            virtual void SetTriggerHorizontalPixel(int pixel);

            //function: GetTriggerHorizontalPixel
            virtual int GetTriggerHorizontalPixel() const;

            //function: SetTriggerVerticalPixel
            virtual void SetTriggerVerticalPixel(int pixel);

            //function: GetTriggerVerticalPixel
            virtual int GetTriggerVerticalPixel() const;

            //function: SetTriggerAreaColor
            virtual void SetTriggerAreaColor(glm::u8vec4 color);

            //function: GetTriggerAreaColor 
            virtual glm::u8vec4 GetTriggerAreaColor() const;

            //function: SetDockPreviewColor
            virtual void SetDockPreviewColor(glm::u8vec4 color);

            //function: GetDockPreviewColor
            virtual glm::u8vec4 GetDockPreviewColor() const;
 
            //Override from Extension
            //function: SetEnabled
            //See <Extension::SetEnabled>
            virtual void SetEnabled(bool enabled) override;

            //function: IsEnabled
            //See <Extension::IsEnabled>
            virtual bool IsEnabled() const override;

            //function: Internal_Update
            //See <Extension::Internal_Update>
            virtual void Internal_Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;;
            
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