#ifndef SSGUI_MASK_ENFORCER
#define SSGUI_MASK_ENFORCER

#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/Extensions/Mask.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include <set>
#include <map>

//namespace: ssGUI::Extensions
namespace ssGUI::Extensions
{
    /*class: MaskEnforcer
    This extension masks the container by calling <Mask::MaskObject> that is linked to this extension.
    Normally you should not add this extension manually. You should manage objects to be masked via <Mask>.
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUI::GUIObject* Container;
        bool Enabled;
        std::set<ssGUIObjectIndex> TargetMasks;
        bool BlockingContainerInput;

        ObjectsReferences CurrentObjectsReferences;
    =================================================================
    ============================== C++ ==============================
    MaskEnforcer::MaskEnforcer() : TargetMasks(), Container(nullptr), Enabled(true), BlockingContainerInput(false), CurrentObjectsReferences()
    {}
    =================================================================
    */
    class MaskEnforcer : public Extension
    {
        private:
            MaskEnforcer& operator=(MaskEnforcer const& other);

        protected:
            ssGUI::GUIObject* Container;
            bool Enabled;
            std::set<ssGUIObjectIndex> TargetMasks;
            bool BlockingContainerInput;

            ObjectsReferences CurrentObjectsReferences;

            MaskEnforcer(MaskEnforcer const& other);

            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;
        
        public:
            static const std::string EXTENSION_NAME;

            MaskEnforcer();
            virtual ~MaskEnforcer() override;
            
            //function: AddTargetMaskObject
            //Add a <Mask> extension to mask this object. targetMaskObj should have <Mask> attached.
            virtual void AddTargetMaskObject(ssGUI::GUIObject* targetMaskObj);

            //function: HasTargetMaskObject
            //Returns true if targetMaskObj is added to this extension
            virtual bool HasTargetMaskObject(ssGUI::GUIObject* targetMaskObj);

            //function: RemoveTargetMaskObject
            //Removes targetMaskObj from this extension
            virtual void RemoveTargetMaskObject(ssGUI::GUIObject* targetMaskObj);

            //function: GetTargetMaskObjects
            //Returns a list of target mask objects
            virtual std::vector<ssGUI::GUIObject*> GetTargetMaskObjects();
        
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
            virtual Extension* Clone(ssGUI::GUIObject* newContainer) override;
    };
}

#endif