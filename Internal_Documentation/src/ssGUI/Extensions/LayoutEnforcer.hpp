#ifndef SSGUI_LAYOUT_ENFORCER
#define SSGUI_LAYOUT_ENFORCER

#include "ssGUI/Extensions/Extension.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

//namespace: ssGUI::Extensions
namespace ssGUI::Extensions
{
    /*class: ssGUI::Extensions::LayoutEnforcer
    This allows the user to "pull" the layout items back by recording the container state.

    Variables & Constructor:
    ============================== C++ ==============================
    private:
        ssGUI::GUIObject* Container;
        bool Enabled;
        glm::vec2 ContainerStartPos;
        glm::vec2 ContainerStartSize;
        bool ContainerResizeStarted;

    =================================================================
    ============================== C++ ==============================
    LayoutEnforcer::LayoutEnforcer() : Container(nullptr), Enabled(true), ContainerStartPos(), ContainerStartSize(), ContainerResizeStarted(false)
    {}
    =================================================================
    */
    class LayoutEnforcer : public Extension
    {
        public:
            friend class ssGUI::Factory;

        private:
            ssGUI::GUIObject* Container;
            bool Enabled;
            glm::vec2 ContainerStartPos;
            glm::vec2 ContainerStartSize;
            bool ContainerResizeStarted;

            LayoutEnforcer& operator=(LayoutEnforcer const& other);

        protected:
            LayoutEnforcer();
            virtual ~LayoutEnforcer() override;
            LayoutEnforcer(LayoutEnforcer const& other);
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};

            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;
        
        public:
            static const std::string EXTENSION_NAME;

            //Override from Extension
            //function: SetEnabled
            //See <Extension::SetEnabled>
            virtual void SetEnabled(bool enabled) override;

            //function: IsEnabled
            //See <Extension::IsEnabled>
            virtual bool IsEnabled() const override;

            //function: Internal_Update
            //See <Extension::Internal_Update>
            virtual void Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;
            
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
            virtual LayoutEnforcer* Clone(ssGUI::GUIObject* newContainer) override;
    };
}

#endif