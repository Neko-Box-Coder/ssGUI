#ifndef SSGUI_MASK_ENFORCER
#define SSGUI_MASK_ENFORCER

#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/Extensions/Mask.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

//namespace: ssGUI::Extensions
namespace ssGUI::Extensions
{
    //class: MaskEnforcer
    class MaskEnforcer : public Extension
    {
        private:
            ssGUI::Extensions::Mask* CurrentMask;
            ssGUI::GUIObject* Container;
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

            bool Cached;
            bool AllowCaching;
            MaskEnforcer& operator=(MaskEnforcer const& other);

        protected:
            MaskEnforcer(MaskEnforcer const& other);
        
        public:
            static const std::string EXTENSION_NAME;

            MaskEnforcer();
            virtual ~MaskEnforcer() override;
            
            //function: BindToMask
            void BindToMask(ssGUI::Extensions::Mask* bindMask);
            
            //function: SetAllowingCaching
            void SetAllowingCaching(bool allowCaching);
            
            //function: IsAllowingCaching
            bool IsAllowingCaching() const;
            
            //function: DiscardCache
            void DiscardCache();
        
            //Extension methods
            //function: Update
            virtual void Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;;
            
            //function: Draw
            virtual void Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset) override;
            
            //function: GetExtensionName
            virtual std::string GetExtensionName() override;
            
            //function: BindToObject
            virtual void BindToObject(ssGUI::GUIObject* bindObj) override;

            //function: Clone
            virtual Extension* Clone(ssGUI::GUIObject* newContainer) override;
    };



}

#endif