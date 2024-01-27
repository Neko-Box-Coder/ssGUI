#ifndef SSGUI_MASK_ENFORCER_H
#define SSGUI_MASK_ENFORCER_H

#include "ssGUI/DataClasses/TargetShape.hpp"
#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/Extensions/Mask.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include <unordered_map>

namespace ssGUI 
{
    
//namespace: ssGUI::Extensions
namespace Extensions
{
    /*class: ssGUI::Extensions::MaskEnforcer
    This extension masks the container by calling <Mask::MaskObject> that is linked to this extension.
    Normally you should not add this extension manually. You should manage objects to be masked via <Mask>.
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUI::GUIObject* Container;                                                        //See <BindToObject>
        bool Enabled;                                                                       //See <IsEnabled>
        std::unordered_map<ssGUIObjectIndex, std::vector<ssGUI::TargetShape>> TargetMasks;  //See <AddTargetMaskObject>
        std::vector<ssGUIObjectIndex> MasksToAddEventCallbacks;                             //(Internal variable) List of masks to register event callbacks when no Container is binded
        bool BlockingContainerInput;                                                        //(Internal variable) Used to block inputs outside the mask area
        ObjectsReferences CurrentObjectsReferences;                                         //(Internal variable) Used to track the object that has the mask
    =================================================================
    ============================== C++ ==============================
    MaskEnforcer::MaskEnforcer() :  Container(nullptr),
                                    Enabled(true),
                                    TargetMasks(),
                                    MasksToAddEventCallbacks(),
                                    BlockingContainerInput(false),
                                    CurrentObjectsReferences()
    {}
    =================================================================
    */
    class MaskEnforcer : public Extension
    {
        public:
            friend class ssGUI::Factory;

        private:
            MaskEnforcer& operator=(MaskEnforcer const& other);

        protected:
            ssGUI::GUIObject* Container;                                                        //See <BindToObject>
            bool Enabled;                                                                       //See <IsEnabled>
            std::unordered_map<ssGUIObjectIndex, std::vector<ssGUI::TargetShape>> TargetMasks;  //See <AddTargetMaskObject>
            std::vector<ssGUIObjectIndex> MasksToAddEventCallbacks;                             //(Internal variable) List of masks to register event callbacks when no Container is binded
            bool BlockingContainerInput;                                                        //(Internal variable) Used to block inputs outside the mask area
            ObjectsReferences CurrentObjectsReferences;                                         //(Internal variable) Used to track the object that has the mask

            void ApplyEventCallbacksToMasks();
            void AddBeforeRenderEventCallback(ssGUI::GUIObject* maskObject);

            MaskEnforcer();
            virtual ~MaskEnforcer() override;
            MaskEnforcer(MaskEnforcer const& other);
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};

            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(ssGUI::Backend::DrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;
        
        public:
            static const std::string EXTENSION_NAME;
            static const std::string ListenerKey;
            
            //function: AddTargetMaskObject
            //Add a <Mask> extension to mask this object. targetMaskObj should have <Mask> attached.
            virtual void AddTargetMaskObject(ssGUI::GUIObject* targetMaskObj);

            //function: AddTargetMaskObject
            //Add a <Mask> extension to mask this object. targetMaskObj should have <Mask> attached.
            //Only shapes with specified indices will be masked.
            //GUI Object shape index can be obtained with <ssGUI::Renderer::Extension_GetGUIObjectFirstShapeIndex>.
            virtual void AddTargetMaskObject(ssGUI::GUIObject* targetMaskObj, const std::vector<ssGUI::TargetShape>& targetShapes);
            
            //function: HasTargetMaskObject
            //Returns true if targetMaskObj is added to this extension
            virtual bool HasTargetMaskObject(ssGUI::GUIObject* targetMaskObj);

            //function: RemoveTargetMaskObject
            //Removes targetMaskObj from this extension
            virtual void RemoveTargetMaskObject(ssGUI::GUIObject* targetMaskObj);

            //function: ChangeTargetShapeForMask
            //Allows to change which shapes to be masked
            //GUI Object shape index can be obtained with <ssGUI::Renderer::Extension_GetGUIObjectFirstShapeIndex>.
            virtual void ChangeTargetShapeForMask(ssGUI::GUIObject* targetMaskObj, const std::vector<ssGUI::TargetShape>& targetShapes);
            
            //function: GetTargetMaskObjects
            //Returns a list of target mask objects, and a list of shape indices to be masked.
            //If list of shape indices is empty, masking is applied to all shapes
            //GUI Object shape index can be obtained with <ssGUI::Renderer::Extension_GetGUIObjectFirstShapeIndex>.
            virtual std::vector<std::pair<ssGUI::GUIObject*, std::vector<ssGUI::TargetShape>>> GetTargetMaskObjects();

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
                                            ssGUI::Backend::SystemInputInterface* inputInterface, 
                                            ssGUI::InputStatus& currentInputStatus, 
                                            ssGUI::InputStatus& lastInputStatus, 
                                            ssGUI::GUIObject* mainWindow) override;
            
            //function: Internal_Draw
            //See <Extension::Internal_Draw>
            virtual void Internal_Draw(bool isPreRender, ssGUI::Backend::DrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;
            
            //function: GetExtensionName
            //See <Extension::GetExtensionName>
            virtual std::string GetExtensionName() const override;
            
            //function: BindToObject
            //See <Extension::BindToObject>
            virtual void BindToObject(ssGUI::GUIObject* bindObj) override;

            //function: Copy
            //See <Extension::Copy>
            virtual void Copy(const ssGUI::Extensions::Extension* extension) override;

            //function: Internal_GetObjectsReferences
            //See <Extension::Internal_GetObjectsReferences>
            virtual ObjectsReferences* Internal_GetObjectsReferences() override;

            //function: Clone
            //See <Extension::Clone>
            virtual MaskEnforcer* Clone() override;
    };
}

}

#endif