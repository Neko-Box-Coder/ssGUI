#ifndef SSGUI_BORDER
#define SSGUI_BORDER


#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/BaseClasses/GUIObject.hpp" //This is needed as Extension is only forward declaring ssGUI::GUIObject
#include "glm/vec4.hpp"

namespace ssGUI
{
    class GUIObject;
}

//namespace: ssGUI::Extensions
namespace ssGUI::Extensions
{   
    //class: Border
    class Border : public Extension
    {
        private:
            Border& operator=(Border const& other);
        
        protected:
            glm::u8vec4 BorderColour;
            int BorderWidth;
            ssGUI::GUIObject* Container;
            virtual void DrawBorder(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset);
            
            Border(Border const& other);

        public:
            static const std::string EXTENSION_NAME;

            Border();
            virtual ~Border() override;
            
            //function: GetBorderColour
            virtual glm::u8vec4 GetBorderColour() const;
            
            //function: SetBorderColour
            virtual void SetBorderColour(glm::u8vec4 colour);
            
            //function: GetBorderWidth
            virtual int GetBorderWidth() const;
            
            //function: SetBorderWidth
            virtual void SetBorderWidth(int width);
            
            //function: Update
            virtual void Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus) override;
            
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