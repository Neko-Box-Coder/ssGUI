#ifndef SSGUI_MASK_ENFORCER
#define SSGUI_MASK_ENFORCER

#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/Extensions/Mask.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

//namespace: ssGUI::Extensions
namespace ssGUI::Extensions
{
    /*class: MaskEnforcer
    This extension masks the container by calling <Mask::MaskObject> that is linked to this extension.
    Normally you should not add this extension manually. You should manage objects to be masked via <Mask>.
    
    Variables & Constructor:
    ============================== C++ ==============================
    private:
        ssGUIObjectIndex CurrentMask;
        ssGUI::GUIObject* Container;
        bool Enabled;
        bool BlockingContainerInput;

        glm::ivec2 LastMaskGlobalPosition;
        glm::ivec2 LastMaskSize;
        glm::ivec2 LastContainerGlobalPosition;
        glm::ivec2 LastContainerSize;
        glm::ivec2 LastMainWindowGlobalPosition;

        std::vector<glm::ivec2> LastVertices;
        std::vector<glm::ivec2> LastUVs;
        std::vector<glm::u8vec4> LastColours;
        std::vector<int> LastCounts;

        ObjectsReferences CurrentObjectsReferences;

        bool Cached;
        bool AllowCaching;
    =================================================================
    ============================== C++ ==============================
    MaskEnforcer::MaskEnforcer() : CurrentMask(-1), Container(nullptr), Enabled(true), BlockingContainerInput(false), LastMaskGlobalPosition(), 
                                    LastMaskSize(), LastContainerGlobalPosition(), LastContainerSize(), LastVertices(), LastUVs(),
                                    LastColours(), LastCounts(), CurrentObjectsReferences(), Cached(false), AllowCaching(false)
    {}
    =================================================================
    */
    class MaskEnforcer : public Extension
    {
        private:
            ssGUIObjectIndex CurrentMask;
            ssGUI::GUIObject* Container;
            bool Enabled;
            bool BlockingContainerInput;

            glm::ivec2 LastMaskGlobalPosition;
            glm::ivec2 LastMaskSize;
            glm::ivec2 LastContainerGlobalPosition;
            glm::ivec2 LastContainerSize;
            glm::ivec2 LastMainWindowGlobalPosition;

            std::vector<glm::ivec2> LastVertices;
            std::vector<glm::ivec2> LastUVs;
            std::vector<glm::u8vec4> LastColours;
            std::vector<int> LastCounts;

            ObjectsReferences CurrentObjectsReferences;

            bool Cached;
            bool AllowCaching;
            MaskEnforcer& operator=(MaskEnforcer const& other);

        protected:
            MaskEnforcer(MaskEnforcer const& other);
        
        public:
            static const std::string EXTENSION_NAME;

            MaskEnforcer();
            virtual ~MaskEnforcer() override;
            
            //function: BindToMaskGUIObject
            //Binds this extension to a <Mask> extension. maskGUIObj should have <Mask> attached.
            void BindToMaskGUIObject(ssGUI::GUIObject* maskGUIObj);

            //function: GetMaskGUIObject
            //Returns the GUI Object that _should_ have <Mask> attached
            ssGUI::GUIObject* GetMaskGUIObject();
            
            //function: SetAllowingCaching
            //If true, the masked content will only be calculated only when there's any changes to the size or position or mask (to be changed)
            void SetAllowingCaching(bool allowCaching);
            
            //function: IsAllowingCaching
            //If true, the masked content will only be calculated only when there's any changes to the size or position or mask (to be changed)
            bool IsAllowingCaching() const;
            
            //function: DiscardCache
            void DiscardCache();
        
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