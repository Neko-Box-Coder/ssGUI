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
            
            static void SetDefaultGeneratedDockerSettings(ssGUI::Extensions::Docker* defaultDocker);
            static ssGUI::Extensions::Docker* GetDefaultGeneratedDockerSettings();

            static void SetDefaultGeneratedLayoutSettings(ssGUI::Extensions::Layout* defaultLayout);
            static ssGUI::Extensions::Layout* GetDefaultGeneratedLayoutSettings();
 
            virtual void SetFloatingDockerColor(glm::u8vec4 color);
            virtual glm::u8vec4 GetFloatingDockerColor() const;

            virtual void SetChildrenDockerUseThisSettings(bool use);
            virtual bool IsChildrenDockerUseThisSettings() const;


            virtual void SetUseTriggerPercentage(bool use);
            virtual bool IsUseTriggerPercentage() const;

            virtual void SetTriggerHorizontalPercentage(float percent);
            virtual float GetTriggerHorizontalPercentage() const;

            virtual void SetTriggerVerticalPercentage(float percent);
            virtual float GetTriggerVerticalPercentage() const;

            virtual void SetTriggerHorizontalPixel(int pixel);
            virtual int GetTriggerHorizontalPixel() const;

            virtual void SetTriggerVerticalPixel(int pixel);
            virtual int GetTriggerVerticalPixel() const;

            virtual void SetTriggerAreaColor(glm::u8vec4 color);
            virtual glm::u8vec4 GetTriggerAreaColor() const;

            virtual void SetDockPreviewColor(glm::u8vec4 color);
            virtual glm::u8vec4 GetDockPreviewColor() const;
 
            //Override from Extension
            virtual void SetEnabled(bool enabled) override;

            virtual bool IsEnabled() const override;

            //function: Update
            virtual void Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;;
            
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